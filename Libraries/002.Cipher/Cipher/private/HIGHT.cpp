#include "HIGHT.h"
#include <cassert>
#include <cstring>





HIGHT::HIGHT(const uint8* key)
{
	expandKeys(key);
}











void HIGHT::encrypt(uint8* p)
{
	p[0] = (uint8)(p[0] + whiteningKeys[0]);
	p[2] = p[2] ^ whiteningKeys[1];
	p[4] = (uint8)(p[4] + whiteningKeys[2]);
	p[6] = p[6] ^ whiteningKeys[3];


	uint8 next[8];
	for (int r = 0; r < 31; ++r)
	{
		next[1] = p[0];
		next[3] = p[2];
		next[5] = p[4];
		next[7] = p[6];	
		
		next[0] = p[7] ^ (uint8)(F0(p[6]) + subKeys[r][3]);
		next[2] = (uint8)(p[1] + (F1(p[0]) ^ subKeys[r][0]));
		next[4] = p[3] ^ (uint8)(F0(p[2]) + subKeys[r][1]);
		next[6] = (uint8)(p[5] + (F1(p[4]) ^ subKeys[r][2]));

		memcpy(p, next, 8);
	}

	p[1] = (uint8)(p[1] + (F1(p[0]) ^ subKeys[31][0]));
	p[3] = p[3] ^ (uint8)(F0(p[2]) + subKeys[31][1]);
	p[5] = (uint8)(p[5] + (F1(p[4]) ^ subKeys[31][2]));
	p[7] = p[7] ^ (uint8)(F0(p[6]) + subKeys[31][3]);


	p[0] = (uint8)(p[0] + whiteningKeys[4]);
	p[2] = p[2] ^ whiteningKeys[5];
	p[4] = (uint8)(p[4] + whiteningKeys[6]);
	p[6] = p[6] ^ whiteningKeys[7];
}

void HIGHT::decrypt(uint8* c)
{
	c[0] = (uint8)(c[0] - whiteningKeys[4]);
	c[2] = c[2] ^ whiteningKeys[5];
	c[4] = (uint8)(c[4] - whiteningKeys[6]);
	c[6] = c[6] ^ whiteningKeys[7];

	c[1] = (uint8)(c[1] - (F1(c[0]) ^ subKeys[31][0]));
	c[3] = c[3] ^ (uint8)(F0(c[2]) + subKeys[31][1]);
	c[5] = (uint8)(c[5] - (F1(c[4]) ^ subKeys[31][2]));
	c[7] = c[7] ^ (uint8)(F0(c[6]) + subKeys[31][3]);

	uint8 next[8];
	for (int r = 30; r >= 0; --r)
	{
		next[0] = c[1];
		next[2] = c[3];
		next[4] = c[5];
		next[6] = c[7];

		next[7] = c[0] ^ (uint8)(F0(c[7]) + subKeys[r][3]);
		next[1] = (uint8)(c[2] - (F1(c[1]) ^ subKeys[r][0]));
		next[3] = c[4] ^ (uint8)(F0(c[3]) + subKeys[r][1]);
		next[5] = (uint8)(c[6] - (F1(c[5]) ^ subKeys[r][2]));

		memcpy(c, next, 8);
	}


	c[0] = (uint8)(c[0] - whiteningKeys[0]);
	c[2] = c[2] ^ whiteningKeys[1];
	c[4] = (uint8)(c[4] - whiteningKeys[2]);
	c[6] = c[6] ^ whiteningKeys[3];
}

































uint8 HIGHT::F0(uint8 x)
{
	return rotLeft_8(x, 1) ^ rotLeft_8(x, 2) ^ rotLeft_8(x, 7);
}

uint8 HIGHT::F1(uint8 x)
{
	return rotLeft_8(x, 3) ^ rotLeft_8(x, 4) ^ rotLeft_8(x, 6);
}

uint8 HIGHT::rotLeft_8(uint8 x, int n)
{
	// n 컴파일 값으로 들어오기에, 오류 검출 필요 없음
	return (uint8)((x << n) | (x >> (8 - n)));

}




































void HIGHT::expandKeys(const uint8* key)
{
	uint8 delta[128];

	uint8 s[134];

	static const uint8 init[7] = { 0, 1, 0, 1, 1, 0, 1 };

	memcpy(s, init, 7);
	for (int i = 0; i + 7 < 134; ++i)
	{
		s[i + 7] = s[i + 3] ^ s[i];
	}

	for (int i = 0; i < 128; ++i)
	{
		uint8 d = 0;
		for (int b = 6; b >= 0; --b)
		{
			d = (uint8)((d << 1) | s[i + b]);
		}
		delta[i] = d;
	}


	for (int i = 0; i < 4; ++i)
	{
		whiteningKeys[i + 4] = key[i];
		whiteningKeys[i] = key[i + 12];
	}

	uint8* sk = &subKeys[0][0]; 
	for (int r = 0; r < 8; ++r)
	{
		for (int i = 0; i < 8; ++i)
		{
			sk[16 * r + i] = (uint8)(key[(i - r) & 7] 
				+ delta[16 * r + i]);
			sk[16 * r + i + 8] = (uint8)(key[((i - r) & 7) + 8] 
				+ delta[16 * r + i + 8]);
		}
	}
}
































#include <functional>
void HIGHT::test()
{
	uint8 key[16] = {
		0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
		0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };

	uint8 pt[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	uint8 ct[8] = {
		0xf2, 0x03, 0x4f, 0xd9, 0xae, 0x18, 0xf4, 0x00 };

	
	auto func = [&](uint8* p, uint8* c) -> void
		{
			uint8 temp[8];
			memcpy(temp, p, 8);
			HIGHT hight(key);
			
			hight.encrypt(temp);
			if (memcmp(temp, c, 8) != 0)
			{
				printf("암호화 실패\n");
				return;
			}

			hight.decrypt(temp);
			if (memcmp(temp, p, 8) != 0)
			{
				printf("복호화 실패\n");
				return;
			}


			printf("암복호화 성공\n");
		};

	func(pt, ct);
}