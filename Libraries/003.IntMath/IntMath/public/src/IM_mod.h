#pragma once
#include <type_traits>
#include <stdio.h>

template<typename T>
T extendedEuclidian(T k, T m) // 확장 유클리드 알고리즘(모듈러 곱 역원 도출)
{
	static_assert(std::is_integral_v<T>, "T must be Integer");
	using S = std::make_signed_t<T>;

	if (k <= 1) return k;

	S r0 = m;
	S r1 = k;

	S t0 = 0;
	S t1 = 1;

	while (r1)
	{
		S q = r0 / r1;

		S r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;

		S t2 = t0 - q * t1;
		t0 = t1;
		t1 = t2;
	}

	if (t0 < 0) t0 += static_cast<S>(m);
	return static_cast<T>(t0);
}


template<typename T>
T mpow(T g, T x, T p)
{
	static_assert(std::is_integral_v<T>, "T must be Integer");

	// 키가 uint64 까지만 있다 가정했고, out * g 에서 오버플로도 배제함
	// 이것도 고려하면 코드가 복잡해서 가장 단순한 버전으로 처리

	T out = 1;

	while (x)
	{
		if (x & T(1))
		{
			out = (out * g) % p;
		}
		g = (g * g) % p;
		x >>= 1;
	}

	return out;
}



void test_IM_mod()
{
	{
		int k = 5;
		int m = 13;

		int invK = extendedEuclidian(k, m);

		printf("%d, ", invK);
	}


}