#pragma once


typedef int T;

T euclidianAlgorithm(T big, T small)
{
	while (small)
	{
		T nextSmall = big % small;
		big = small;
		small = nextSmall;
	}

	return big;
}


T extendedEuclidianAlgorithm(T x, T m)
{
	if (x <= 1) return x;

	T r0 = m;
	T r1 = x;

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

	if (t0 < 0) t0 += m;
	return t0;
}

