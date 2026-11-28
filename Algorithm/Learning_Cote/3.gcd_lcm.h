#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef int T;

T gcd(T big, T small)
{
	while (small)
	{
		T nextSmall = big % small;
		big = small;
		small = nextSmall;
	}

	return big;
}

T lcm(T big, T small)
{
	return big / gcd(big, small) * small;
}