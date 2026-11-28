#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef int T;

int cmp(const T* a, const T* b)
{
	// T 별로 다름
	if (*a == *b)	return 0;
	if (*a > *b)	return 1;
					return -1;
}

int bsearch(const T* offset, int n, const T* key)
{
	int start = 0;
	int end = n - 1;

	while (start <= end)
	{
		int mid = (start + end) / 2;
		int comp = cmp(key, &offset[mid]);
		if (comp == 0)
		{
			return mid;
		}
		else if (comp < 0)
		{
			end = mid - 1;
		}
		else
		{
			start = mid + 1;
		}
	}

	return -1;
}