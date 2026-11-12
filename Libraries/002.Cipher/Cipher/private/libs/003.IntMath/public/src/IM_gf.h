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




void test_IM_gf()
{

}