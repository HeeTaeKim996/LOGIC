#pragma once



int euclidianAlgorithm(int x, int m)
{
	if (x <= 1) return x;

	int r0 = m;
	int r1 = x;
		
	while (r1)
	{
		int q = r0 / r1;
		
		int r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;
	}

	return r0;
}


int extendedEuclidianAlgorithm(int x, int m)
{
	if (x <= 1) return x;

	int r0 = m;
	int r1 = x;

	int t0 = 0;
	int t1 = 1;

	while (r1)
	{
		int q = r0 / r1;

		int r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;

		int t2 = t0 - q * t1;
		t0 = t1;
		t1 = t2;
	}

	if (t0 < 0) t0 += m;
	return t0;
}