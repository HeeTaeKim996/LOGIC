#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>




typedef int T;


int cmp(T* a,  T* b)
{
	if (*a == *b)	return 0;
	if (*a > *b)	return 1;
					return -1;
}













/*-------------------------------
		 불안정정렬 - 퀵정렬
		 (qsort로 대체 가능)
--------------------------------*/
void swap(T* a, T* b)
{
	T t = *a;
	*a = *b;
	*b = t;
}

void threeSort(T* a, T* b, T* c)
{
	if (cmp(a, b) > 0) swap(a, b);
	if (cmp(b, c) > 0) swap(b, c);
	if (cmp(a, b) > 0) swap(a, b);
}

void partition(T* offset, int left, int right)
{
	int mid = (left + right) / 2;

	threeSort(&offset[left], &offset[mid], &offset[right]);

	T x = offset[mid];

	swap(&offset[mid], &offset[right - 1]);

	int pl = left + 1;
	int pr = right - 2;

	do
	{
		while (cmp(&offset[pl], &x) < 0) pl++;
		while (cmp(&offset[pr], &x) > 0) pr--;

		if (pl <= pr)
		{
			swap(&offset[pl], &offset[pr]);
			pl++;
			pr--;
		}
	} while (pl <= pr);

	if (left < pr) partition(offset, left, pr);
	if (pl < right) partition(offset, pl, right);
}


void quickSort(T* offset, int n)
{
	partition(offset, 0, n - 1);
}












/*------------------------------
		 안정정렬(병합정렬)
		 (직접구현필요)
------------------------------*/
#include <string.h>

T* PS;

void merge(T* offset, int left, int right)
{
	if (left == right) return;

	int mid = (left + right) / 2;

	merge(offset, left, mid);
	merge(offset, mid + 1, right);

	int bufferCount = mid - left + 1;
	memcpy(PS, &offset[left], bufferCount * sizeof(T));

	mid++; // 이후 mid 는 right 의 포인터로 사용됨

	int bufferPointer = 0; // left 의 포인터

	while (bufferPointer < bufferCount && mid <= right)
	{
		offset[left++] = cmp(&PS[bufferPointer], &offset[mid]) <= 0 ?
			PS[bufferPointer++] : offset[mid++];
	}
	if (bufferPointer < bufferCount)
	{
		memcpy(&offset[left], &PS[bufferPointer],
			(bufferCount - bufferPointer) * sizeof(T));
	}
}

void mergeSort(T* offset, int n)
{
	merge(offset, 0, n - 1);
}

































void test_sort()
{
	int i1[] = { 7, 5, 1, 3, 2, 6, 9, 4 ,8 };
	int i2[] = { 7, 5, 1, 3, 2, 6, 9, 4 ,8 };

	quickSort(i1, 9);
	for (int i = 0; i < 9; ++i) printf("%d ", i1[i]);
	printf("\n");

	PS = (int*)malloc(sizeof(int) * 9);
	mergeSort(i2, 9);
	for (int i = 0; i < 9; ++i) printf("%d ", i2[i]);
}