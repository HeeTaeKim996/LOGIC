#include "src/IDEA.h"
#include <cstring>
#include <functional>

IDEA::IDEA(uint8* key)
{
	makeEncKeys(key);
	makeDecKeys();
}




void IDEA::crypt(uint8* m, const uint16* keys)
{

#ifdef WHEN_ENDIAN_MATTERS
	uint16 x[4];
	endianCopy_to16(x, m, 4);
#else
	uint16* x = reinterpret_cast<uint16*>(m);
#endif


	auto baseBox = [&](const uint16* f)
		{
			x[0] = mul(x[0], f[0]);
			x[1] = add(x[1], f[1]);
			x[2] = add(x[2], f[2]);
			x[3] = mul(x[3], f[3]);
		};

	auto xorBox = [&](const uint16* f)
		{
			uint16 t1 = x[0] ^ x[2];
			uint16 t2 = x[1] ^ x[3];

			t1 = mul(t1, f[4]);
			t2 = add(t1, t2);
			t2 = mul(t2, f[5]);
			t1 = add(t1, t2);

			x[0] ^= t2;
			x[2] ^= t2;
			x[1] ^= t1;
			x[3] ^= t1;
		};


	for (int r = 0; r < 8; ++r)
	{
		const uint16* k = &keys[r * 6];

		baseBox(k);

		xorBox(k);

		swap(x[1], x[2]); // 라운드 스왑
	}

	swap(x[1], x[2]); // 끝 스왑

	baseBox(&keys[48]);

#ifdef WHEN_ENDIAN_MATTERS
	endianCopy_to8(m, x, 4);
#endif
}






































uint16 IDEA::add(uint16 x, uint16 y)
{
	// 이번에 처음 알았는데 x, y 가 uint16 이라도,
	// x + y 를 하면 int 보다 작을시 C++ 에서 자동으로 int로
	// 승격하여 처리한다 함. 그래서 연산 때마다
	// x[~] = (uint16)(x + y); 처리하기 귀찮아서 이렇게 만듬

	return uint16(x + y);
}




uint16 IDEA::mul(uint16 x, uint16 k)
{
	uint64 a = x ? x : CEIL;
	uint64 b = k ? k : CEIL;

	return (uint16)((a * b) % M); 
	// uint16 끼리의 곱이면 uint32 로 충분하지만, 
	// x, k 가 모두 0으로  2 ^ 16 (CEIL) 이 되는 경우,
	// 2 ^ 32 = uint32 로 표현 못하기에, uint64 사용
}



uint16 IDEA::extendedEuclidian(uint16 k)
{
	int32 r0 = M;
	int32 r1 = k;

	int32 t0 = 0;
	int32 t1 = 1;

	while (r1)
	{
		int32 q = r0 / r1;

		int32 r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;

		int32 t2 = t0 - q * t1;
		t0 = t1;
		t1 = t2;
	}

	if (t0 < 0) t0 += M;

	return static_cast<uint16>(t0);
}

void IDEA::swap(uint16& x, uint16& y)
{
	uint16 temp = x;
	x = y;
	y = temp;
}



void IDEA::makeEncKeys(uint8* key) // 128(16)
{
	// 단순하다. 원본키 우선 넣고, 중복되면 섞어서 다시 넣기

	uint16 k[8];
#ifdef WHEN_ENDIAN_MATTERS
	endianCopy_to16(k, key, 8);
#else
	straightCopy_to16(k, key, 8);
#endif 



	int n = 0;
	while (n < 52)
	{
		for (int i = 0; i < 8 && n < 52; ++i)
		{
			encKeys[n++] = k[i];
		}

		// 섞기
		uint16 t[8];
		for (int i = 0; i < 8; ++i)
		{
			t[i] = (uint16)(
				(k[(i + 1) % 8] << 9)
				| k[(i + 2) % 8] >> 7);
		}

		std::memcpy(k, t, 16);
	}
}

void IDEA::makeDecKeys()
{
	/*	
	encrypt 과정을 baseBox 를 B, xorBox 를 X, Swap 을 S 라 하자
	
    ■ B 에 대한 dec 처리
	add 는 sub 이 되도록 k 를 -k 로 할당, 
	mul 은 extendedEuclidian 알고리즘으로 k' 를 할당하면 된다.
	이를 사용하면 개별 B에 대한 처리를 할 수 있다.
	
	■ X 에 대한 dec 처리
	X 에서 t1, t2 를 잘 봐라. t1 = 0 ^ 2, t2 = 1 ^ 3 이다.
	이후 t1, t2 를 셔플링하고,
	0, 2 에는 같은 t1 을, 1, 3 에는 같은 t2 를 ^ 한다.
	따라서 여전히 I 전 후에의 0 ^ 2, 1 ^ 3 은 동일할 수밖에 없다.
	X의 dec 처리는 추가 처리가 필요가 없다. 동일한 키를 할당하면 된다.

	■ enc, dec 의 순서 불일치 부분 처리 

	라운드 수를 3로 축약해서 이해하자
	enc : B - X - S - B - X - S - B - X - B   ( -> 방향으로 진행 )
	dec : B - X - B - S - X - B - S - X - B	  ( <- 방향으로 진행 )
	※ 마지막 라운드 S는 끝 스왑 S 에 의해 중복되어 상쇄됨
	
	아래 코드를 보면 알다시피 dec 를 enc 의 <- 방향으로 키를 할당했다.
	※ B(0, 1, 2, 3) 와 X(4, 5)에 대한 키 할당 방법은 위 둘에서 이미 설명함

	enc, dec 를 -> 방향으로 보면, S - B 가 B - S 로 된 것 외에는 순서가 일치
	S-B 스와핑 문제는 첫머리 (B-X) 와 끝머리(X-B) 에는 문제 없지만, 중간
	라운딩 부분에서 문제가 발생한다.
	이를 해결하기 위해, 첫/끝 제외 중간 라운딩 부분의 B 키를 할당할 때, 
	스왑되는 1, 2 인덱스를 미리 스와핑하여 할당한다

	*/

	for (int r = 0; r <= 8; ++r)
	{
		uint16* dec = &decKeys[6 * r];
		const uint16* enc = &encKeys[48 - 6 * r];
		// 첫 스타트는 48, 49, 50, 51
		// (B 대응키 할당 목적. 어차피 그 다음은 X 고 그건 이후 
		// [-2], [-1] 로 처리하니까.
		
		dec[0] = extendedEuclidian(enc[0]);
		dec[3] = extendedEuclidian(enc[3]);

		if (r == 0 || r == 8) 
			// 첫/끝 단계 B 는 위치 그대로 
		{
			dec[1] = (uint16)(0 - enc[1]);
			dec[2] = (uint16)(0 - enc[2]);
		}
		else
			// 첫/끝 제외 중간 B들은 라운드 스왑을 반영하여 스왑
		{
			dec[1] = (uint16)(0 - enc[2]);
			dec[2] = (uint16)(0 - enc[1]);
		}

		if (r < 8) 
			// 마지막 라운드 제외 4, 5 할당
			// enc 기준으로 보면 '이전' 라운드의 xorBox 를 할당
		{
			dec[4] = enc[-2];
			dec[5] = enc[-1];
		}
	}
}



void IDEA::test()
{
	uint64 key[2] = { 0x0400030002000100ULL, 
		0x0800070006000500ULL };
	uint64 pt = 0x0300020001000000ULL;
	uint64 ct = 0xE56D98012BEDFB11ULL;


	auto func = [&](uint64 pt, uint64 ct)
		{
			uint64 temp = pt;

			IDEA idea(reinterpret_cast<uint8*>(key));
			idea.encrypt(reinterpret_cast<uint8*>(&temp));
			
			if (std::memcmp(&temp, &ct, 8) == 0)
			{
				printf("통과\n");
			}
			else
			{
				printf("실패\n");
			}

			idea.decrypt(reinterpret_cast<uint8*>(&temp));

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