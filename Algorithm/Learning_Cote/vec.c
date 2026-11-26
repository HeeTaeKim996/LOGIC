#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int Type;

typedef struct Vec
{
	Type* offset;
	int top;
}Vec;

void vec_init(Vec* vec, int cap)
{
	vec->offset = (Type*)malloc(sizeof(Type) * cap);
	vec->top = -1;
}

Type get(Vec* vec, int i)
{
	return vec->offset[i];
}

bool vec_empty(Vec* vec)
{
	return vec->top <= -1;
}

void vec_push_back(Vec* vec, Type t)
{
	vec->offset[++vec->top] = t;
}

Type vec_pop_back(Vec* vec)
{
	return vec->offset[vec->top--];
}

void vec_insert(Vec* vec, int index, Type t)
{
	memmove(&vec->offset[index+1], &vec->offset[index],
		sizeof(Type) *  (vec->top - index + 1));
	vec->offset[index] = t;
	vec->top++;
}

void vec_erase(Vec* vec, int index)
{
	memmove(&vec->offset[index], &vec->offset[index + 1],
		sizeof(Type) * (vec->top - index));
	vec->top--;
}