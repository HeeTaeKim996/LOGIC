
/*
	※ 정리
	binarySearch 가 단순하게 나오는 문제는 드물지만, 이를 활용하는 문제는
	lv 3 ~ 에서 등장한다
	배열의 요소를 A라 할 때, A 에 대한 이진검색 문제는 쉬워서 나오지 않지만,
	
	f(A) 에 대한 검색을 하는 문제가 나온다. f(A) 문제 포인트를 정리하면,
	
	1) 함수 f가 단조 함수일 때, 검색하고자 하는 것이 f(A) = x 일 때의 A 이다
	2) f 는 단사 함수가 아니기에, f(A) = x 를 만족하는 A는 복수다.
	   그래서 문제에서 최소 또는 최대 A를 묻는다. 
	   이를 충족하기 위해, ==, > , < 분기식에서 == 을 빼고,
	   ( >= and < ) or ( <= and >) 식으로 바꿔 처리하면,
	   f(A) = x 를 만족하는 A에서 최대 또는 최소의 A를 구할 수 있다
*/












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