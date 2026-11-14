#pragma once
#include <type_traits>

#include "IM_BigInt.h"
#include "IM_check.h"


template<typename T>
T euclidian(T a, T b) // 유클리드 알고리즘
{
	CHECK_INT(T);


	T r0 = a;
	T r1 = b;

	while (r1)
	{
		T q = r0 / r1;

		T r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;
	}

	return r0;
}




template<typename T>
T gcd(T a, T b)
{
	CHECK_INT(T);

	return euclidian(a, b);
}

template<typename T>
T lcm(T a, T b)
{
	CHECK_INT(T);

	return a / gcd(a, b) * b; // 오버플로 대비 나누기 먼저
}





void test_IM_int()
{
	int a = 40;
	int b = 24;

	int g = gcd(b, a);
	int l = lcm(a, b);

	printf("%d, %d\n", g, l);

}