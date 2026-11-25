#include <stdlib.h>
#include <stdbool.h>



/*

같은 숫자는 싫어 (Lv.1)
올바른 괄호 (Lv.2)
짝지어 제거하기 (Lv.2)
기능개발 (Lv.2)
프로세스 (Lv.2)
주식가격 (Lv.2)
다리를 지나는 트럭 (Lv.2)


*/

typedef struct Stk
{
    char* offset;
    int capacity;
    int top;
}Stk;

void stk_init(Stk* stk, int inCap)
{
    stk->capacity = inCap;
    stk->offset = (char*)malloc(sizeof(char) * stk->capacity);
    stk->top = -1;
}

void stk_push(Stk* stk, char c)
{
    stk->offset[++stk->top] = c;
}

char stk_pop(Stk* stk)
{
    return stk->offset[stk->top--];
}

char stk_top(Stk* stk)
{
    return stk->offset[stk->top];
}

bool stk_empty(Stk* stk)
{
    return stk->top == -1;
}
