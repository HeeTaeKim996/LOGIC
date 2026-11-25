#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Que
{
	int* offset;
	int n;
	int front;
	int rear;
}Que;

void que_init(Que* que, int cap)
{
	que->n = cap;
	que->offset = (int*)malloc(sizeof(int) * cap);
	que->front = 0;
	que->rear = 0;
}

bool que_empty(Que* que)
{
	return que->front == que->rear;
}

bool que_full(Que* que)
{
	return (que->rear + 1) % que->n == que->front;
}

void que_enque(Que* que, int i)
{
	que->rear = (que->rear + 1) % que->n;
	que->offset[que->rear] = i;
}

int que_front(Que* que)
{
	int index = (que->front + 1) % que->n;
	return que->offset[index];
}

int que_deque(Que* que)
{
	que->front = (que->front + 1) % que->n;
	return que->offset[que->front];
}


void test_queue()
{
	Que* que = (Que*)malloc(sizeof(Que));
	que_init(que, 6);
	
	for (int i = 0; i < 5; ++i)
	{
		que_enque(que, i);
	}

	printf("%d\n", que_front(que));

	
	int rep = 3;
	while (rep-- > 0)
	{
		que_deque(que);
	}

	for (int i = 5; i <= 7; ++i)
	{
		que_enque(que, i);
	}

	rep = 5;
	while (rep-- > 0)
	{
		printf("%d ", que_deque(que));
	}
}