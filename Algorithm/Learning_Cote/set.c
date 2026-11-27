#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

typedef int T;

typedef struct Node
{
    int index;
    struct Node* next;
    T type;
} Node;

typedef struct Set
{
    Node** buckets;
    int n;
    Node dummy;
}Set;

void set_init(Set* set, int cap)
{
    set->n = cap;
    set->buckets = (Node**)calloc(cap, sizeof(Node*));
    set->dummy.next = NULL;
}

int set_hash(Set* set, T type)
{
    // Type별로 다르게 처리 필요
    return type % set->n;
}


void set_insert(Set* set, T type)
{
    int index = set_hash(set, type);

    if (set->buckets[index] != NULL)
    {
        Node* curr = set->buckets[index]->next;

        while (curr != NULL && curr->index == index)
        {
            if (curr->type == type)
            {
                return;
            }

            curr = curr->next;
        }
    }



    // 없다면, 새로 생성하여 삽입
    Node* created = (Node*)malloc(sizeof(Node));
    created->index = index;
    created->next = NULL;
    created->type = type;

    // 인덱스의 머리가 없다면, dummy를 머리로, created 는 목이자 꼬리로
    if (set->buckets[index] == NULL)
    {
        created->next = set->dummy.next;
        set->dummy.next = created;
        set->buckets[index] = &set->dummy;

        if (created->next != NULL)
        {
            set->buckets[created->next->index] = created;
        }
    }
    // 인덱스의 머리가 이미 있다면, created를 목으로
    else
    {
        created->next = set->buckets[index]->next;
        set->buckets[index]->next = created;
    }
}



bool set_find(Set* set, T type)
{
    int index = set_hash(set, type);

    if (set->buckets[index] != NULL)
    {
        Node* curr = set->buckets[index]->next;

        while (curr != NULL && curr->index == index)
        {
            if (curr->type == type)
            {
                return true;
            }

            curr = curr->next;
        }
    }


    return false;
}



void set_getAll(Set* set, T* offset, int* n)
{
    int i = 0;

    Node* curr = set->dummy.next;
    while (curr != NULL)
    {
        offset[i++] = curr->type;   
        curr = curr->next;
    }

    *n = i;
}

void set_erase(Set* set, T type)
{
    int index = set_hash(set, type);
    if (set->buckets[index] == NULL) return;

    Node* prev = set->buckets[index];
    Node* curr = prev->next;

    while (curr != NULL && curr->index == index)
    {
        if (curr->type == type)
        {
            prev->next = curr->next;

            
            if (curr->next != NULL)
            {
                int nextIndex = curr->next->index;

                // 현 노드가 다른 인덱스의 머리였다면, 
                // prev를 다른 인덱스의 머리로
                if (nextIndex != index)
                {
                    set->buckets[nextIndex] = prev;
                }
            }

            // 현 index가 텅 비게 됐다면, 인덱스의 머리를 제거
            if (prev == set->buckets[index] &&
                (curr->next == NULL || curr->next->index != index))
            {
                set->buckets[index] = NULL;
            }


            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

























void test_set()
{
    Set* set = (Set*)malloc(sizeof(Set));
    set_init(set, 7);

    set_insert(set, 3);
    set_insert(set, 5);
    set_insert(set, 7);

    printf("%d, %d, %d\n", set_find(set, 3), set_find(set, 4),
        set_find(set, 5));

    set_erase(set, 5);
    printf("%d\n", set_find(set, 5));

    int* offset = (int*)malloc(sizeof(int) * 3);
    int count;
    set_getAll(set, offset, &count);
    for (int i = 0; i < count; ++i)
    {
        printf("%d ", offset[i]);
    }
}


