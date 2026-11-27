#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef int T;

typedef struct Pq
{
	T* offset;
	int n;
} Pq;


void pq_init(Pq* pq, int cap)
{
	pq->offset = (T*)malloc(sizeof(T) * cap);
	pq->n = 0;
}


static int less(T* a, T* b)
{
	// type 및 정렬 기준따라 내용 다름
	return *a > *b;
}


void pq_downHeap(Pq* pq, int parent)
{
	T mover = pq->offset[parent];

	int lastChild = pq->n - 1;
	int child;
	for (; parent * 2 + 1 <= lastChild; parent = child)
	{
		int cl = parent * 2 + 1;
		int cr = cl + 1;

		child = (cr <= lastChild
			&& less(&pq->offset[cl], &pq->offset[cr])) ? cr : cl;

		if (less(&mover, &pq->offset[child]) == false) break;

		pq->offset[parent] = pq->offset[child];
	}

	pq->offset[parent] = mover;
}


void pq_upHeap(Pq* pq, int child)
{
	T mover = pq->offset[child];

	int parent;
	for (; child > 0; child = parent)
	{
		parent = (child - 1) / 2;

		if (less(&pq->offset[parent], &mover) == false) break;

		pq->offset[child] = pq->offset[parent];
	}

	pq->offset[child] = mover;
}

void pq_push(Pq* pq, T t)
{
	pq->offset[pq->n++] = t;

	pq_upHeap(pq, pq->n - 1);
}

void pq_pop(Pq* pq)
{
	pq->offset[0] = pq->offset[--pq->n];

	if (pq->n > 0)
	{
		pq_downHeap(pq, 0);
	}
}

T pq_top(Pq* pq)
{
	return pq->offset[0];
}

bool pq_empty(Pq* pq)
{
	return pq->n <= 0;
}
#include <string.h>
void pq_initWithArr(Pq* pq, T* offset, int n, int cap)
{
	pq->n = n;
	pq->offset = (T*)malloc(sizeof(T) * cap);
	memcpy(pq->offset, offset, sizeof(T) * n);

	// 끝자식 = n-1. 끝자식의 부모 = ((n - 1) - 1) / 2 = n / 2 - 1
	for (int parent = n / 2; parent-- > 0; )
	{
		pq_downHeap(pq, parent);
	}
}








#include <string>
#include <vector>

using namespace std;

int solution(vector<int> scoville, int K) 
{
	// to C
	int n = scoville.size();
	int* scov = (int*)malloc(sizeof(int) * n);
	memcpy(scov, &scoville[0], sizeof(int) * n);
	
		
	Pq* pq = (Pq*)malloc(sizeof(Pq));
	pq_initWithArr(pq, scov, n, n + 1);
	
    int answer = 0;


	while (pq_empty(pq) == false && pq_top(pq) < K)
	{
		int first = pq_top(pq);
		pq_pop(pq);

		if (pq_empty(pq)) return -1;

		int second = pq_top(pq);
		pq_pop(pq);

		int shake = first + 2 * second;
		pq_push(pq, shake);

		answer++;

	}


    return answer;
}