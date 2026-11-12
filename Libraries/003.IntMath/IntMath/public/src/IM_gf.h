#pragma once
#include <type_traits>



template<typename T>
T gmul(T a, T b, T mod, unsigned n) 
{
	static_assert(std::is_integral_v<T>, "T must be Integer");

	const T msb = static_cast<T>(T(1) << (n - 1));
	T res = 0;

	while (b)
	{
		if (b & 1)
		{
			res ^= a;
		}

		const bool carry = (a & msb) != 0;
		a = static_cast<T>(a << 1);
		if (carry) a ^= mod;
		// 나머지 XOR 연산 을 multiplied a 에 미리 처리하여 성능 업
		// [ (a * b) % m == ( (a % m) * (b % m) ) % m ]

	
		b >>= 1;
	}

	return res;
}




template<typename T>
static T degree(T a)
{
	T degree = T(0);

	while ((a >>= 1) > 0) // 전위 판정
	{
		++degree;
	}

	return degree; // 인덱스 0부터
}



template<typename T>
T gdiv(T a, T b)
{
	static_assert(std::is_integral_v<T>, "T must be Integer");

	T quotient = 0;

	while (a && degree(a) >= degree(b))
	{
		T temp = b;
		T shift = 0;

		while (degree(temp) < degree(a))
		{
			temp <<= 1;
			++shift;
		}

		a ^= temp;
		quotient |= static_cast<T>(1) << shift;
	}

	return quotient;
}


template<typename T>
T gres(T a, T b)
{
	if (b == 0) return -1;

	while (a && degree(a) >= degree(b))
	{
		T temp = b;

		while (degree(temp) < degree(a))
		{
			temp <<= 1;
		}

		a ^= temp;
	}

	return a;
}





inline void test_IM_gf()
{
	using u8 = uint8_t;
	u8 a = 0x53, b = 0xCA;

	u8 m = gmul<u8>(a, b, 0x1B, 8);
	printf("gmul(53, CA) = %x   (기대값: 1)\n", m);

	// 환원 없는 곱은 16비트 필요
	uint16_t raw = 0;
	for (int i = 0; i < 8; ++i)
		if (b & (1u << i)) raw ^= (uint16_t)(a << i);
	uint16_t r = gres<uint16_t>(raw, 0x11B);
	uint16_t q = gdiv<uint16_t>(raw, 0x11B);
	printf("raw = %x, gres = %x, gdiv = %x\n", raw, r, q);
}