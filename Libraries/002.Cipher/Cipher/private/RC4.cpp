#include "RC4.h"
#include <cassert>

RC4::RC4(const uint8* key, int keyBytes)
{
	assert(keyBytes >= 1 && keyBytes <= 256);
	makeKeys(key, keyBytes);
}

void RC4::crypt(uint8* data, int len)
{
	// 바이트 암복호화 때마다 sBox를 섞고, 
	// sBox 에서 k를 꺼내 바이트를 xor한다	

	for (int n = 0; n < len; ++n)
	{
		x = (x + 1) % 256;
		y = (y + sBox[x]) % 256;

		swap(sBox[x], sBox[y]);

		uint8 k = sBox[(sBox[x] + sBox[y]) % 256];
		data[n] ^= k;
	}
}

void RC4::makeKeys(const uint8* key, 
	int keyBytes) // 1 ~ 256
{
	// sBox 를 섞는다

	for (int i = 0; i < 256; ++i)
	{
		sBox[i] = i;
	}

	int i = 0;
	for (int j = 0; j < 256; ++j)
	{
		i = (i + sBox[j] + key[j % keyBytes]) % 256;

		swap(sBox[i], sBox[j]);
	}

	x = 0;
	y = 0;
}

void RC4::swap(uint8& a, uint8& b)
{
	uint8 temp = a;
	a = b;
	b = temp;
}




















#include <stdio.h>
#include <functional>
#include <cstring>
void RC4::test()
{
	uint64 key = 0xEFCDAB8967452301ULL;

	const uint64 pt1 = 0xEFCDAB8967452301ULL;
	const uint64 ct1 = 0x96C5E0998087B775ULL;

	const uint64 pt2 = 0x0000000000000000ULL;
	const uint64 ct2 = 0x79084B10E7C29474ULL;


	auto func = [&](const uint64& pt, const uint64& ct) 
		-> void
		{
			RC4 rc1(reinterpret_cast<uint8*>(&key), 8);
			RC4 rc2(reinterpret_cast<uint8*>(&key), 8);

			uint64 temp = pt;
			
			rc1.crypt(reinterpret_cast<uint8*>(&temp), 8);
			if (std::memcmp(&temp, &ct, 8) == 0)
			{
				printf("성공\n");
			}
			else
			{
				printf("실패\n");
			}

			rc2.crypt(reinterpret_cast<uint8*>(&temp), 8);
			if (std::memcmp(&temp, &pt, 8) == 0)
			{
				printf("성공\n");
			}
			else
			{
				printf("실패\n");
			}
		};

	func(pt1, ct1);
	func(pt2, ct2);
}