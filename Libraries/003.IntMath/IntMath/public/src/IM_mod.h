#pragma once
#include <type_traits>
#include <stdio.h>
#include "IM_BigInt.h"

#include "IM_check.h"





// ※ 모듈러면 signed 에서 처리해야하니, signed 정수로 전제

template<typename T>
T extendedEuclidian(T k, T m) // 확장 유클리드 알고리즘(모듈러 곱 역원 도출)
{

	CHECK_SIGN_INT(T);
	
	if (k <= 1) return k;

	T r0 = m;
	T r1 = k;

	T t0 = 0;
	T t1 = 1;

	while (r1)
	{
		T q = r0 / r1;

		T r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;

		T t2 = t0 - q * t1;
		t0 = t1;
		t1 = t2;
	}

	if (t0 < 0) t0 += static_cast<T>(m);
	return static_cast<T>(t0);
}


template<typename T>
T mpow(T g, T x, T p)
{
	CHECK_SIGN_INT(T);

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


template<typename T>
static T norm(T a, T p)
{
	a %= p;
	if (a < 0) a += p;
	return a;
}




template<typename T>
T madd(T a, T b, T p)
{
	CHECK_SIGN_INT(T);
	return norm(a + b, p);
}

template<typename T>
T msub(T a, T b, T p)
{
	CHECK_SIGN_INT(T);
	return norm(a - b, p);
}


template<typename T>
T mmul(T a, T b, T p)
{
	CHECK_SIGN_INT(T);
	return norm(a * b, p);
}

void test_IM_mod()
{
	{
		int k = 5;
		int m = 13;

		int invK = extendedEuclidian(k, m);

		printf("%d, ", invK);


		printf("\nBigIntTest\n");
		BigInt a("5");
		BigInt b("3");
		BigInt p("13");

		extendedEuclidian(a, p).printNum();
		mpow(a, b, p).printNum();
		madd(a, b, p).printNum();
		msub(a, b, p).printNum();
		mmul(a, b, p).printNum();
	}


}