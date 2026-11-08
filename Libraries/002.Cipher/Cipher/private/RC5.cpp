#include "RC5.h"
#include "Endian.h"

RC5::RC5(const uint8* key)
{
	makeKeys(key);
}


void RC5::encrypt(uint8* p)
{
	uint32 A = LittleEndian::loadTo32(p);
	uint32 B = LittleEndian::loadTo32(p + 4);

	A += S[0];
	B += S[1];

	for (int i = 1; i <= 12; ++i)
	{
		A = rotLeft(A ^ B, B & 31) + S[2 * i];
		B = rotLeft(A ^ B, A & 31) + S[2 * i + 1];
	}

	LittleEndian::storeFrom32(p, A);
	LittleEndian::storeFrom32(p + 4, B);

}


void RC5::decrypt(uint8* c)
{
	uint32 A = LittleEndian::loadTo32(c);
	uint32 B = LittleEndian::loadTo32(c + 4);

	for (int i = 12; i >= 1; --i)
	{
		B = rotRight(B - S[2 * i + 1], A & 31) ^ A;
		A = rotRight(A - S[2 * i], B & 31) ^ B;
	}

	B -= S[1];
	A -= S[0];

	LittleEndian::storeFrom32(c, A);
	LittleEndian::storeFrom32(c + 4, B);
}








void RC5::makeKeys(const uint8* key) // 키길이 16 바이트 
{
	uint32 wordKeys[4]; // L 의 워드수 = 16 / 4 = 4

	for (int i = 0; i < 4; ++i)
	{
		wordKeys[i] = (uint32)key[i * 4]
			| (uint32)key[i * 4 + 1] << 8
			| (uint32)key[i * 4 + 2] << 16
			| (uint32)key[i * 4 + 3] << 24;
	}

	S[0] = P;
	for (int i = 1; i < 26; ++i)
	{
		S[i] = S[i - 1] + Q;
	}

	uint32 A = 0; 
	uint32 B = 0;
	
	int i = 0;
	int j = 0;

	// 섞기 반복수 = S의 워드수 * 3 = 26 * 3 = 78 
	for (int n = 0; n < 78; ++n)
	{
		A = rotLeft(S[i] + A + B, 3);
		S[i] = A;

		B = rotLeft(wordKeys[j] + A + B, (A + B) & 31);
		wordKeys[j] = B;

		i = (i + 1) % 26;
		j = (j + 1) % 4;
	}
}

uint32 RC5::rotLeft(uint32 x, uint32 n)
{
	if (n)
	{
		return (x << n) | (x >> 32 - n);
	}
	return x;
}

uint32 RC5::rotRight(uint32 x, uint32 n)
{
	if(n)
	{ 
		return (x >> n) | (x << 32 - n);
	}
	return x;
}







#include <stdio.h>
#include <cstring>
void RC5::test()
{
	uint64 key[2] = { 0x0000000000000000ULL,
		0x0000000000000000ULL };
	uint64 pt = 0x0000000000000000ULL;
	uint64 ct = 0x6D8F4B15EEDBA521ULL;

	auto func = [&](uint64 pt, uint64 ct)
		{
			uint64 temp = pt;

			RC5 rc5(reinterpret_cast<uint8*>(key));
			rc5.encrypt(reinterpret_cast<uint8*>(&temp));

			if (std::memcmp(&temp, &ct, 8) == 0)
			{
				printf("통과\n");
			}
			else
			{
				printf("실패\n");
			}

			rc5.decrypt(reinterpret_cast<uint8*>(&temp));

			if (std::memcmp(&temp, &pt, 8) == 0)
			{
				printf("통과\n");
			}
			else
			{
				printf("실패\n");
			}
		};

	func(pt, ct);

}