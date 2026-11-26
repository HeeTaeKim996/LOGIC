#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



#include <string.h>

typedef char Key;
typedef int Val;

typedef struct Node
{
	int index;
	struct Node* next;
	Key key;
	Val val;
} Node;

typedef struct Map
{
	Node** buckets;
	int n;
	Node dummy;
} Map;

void map_init(Map* map, int cap)
{
	map->n = cap;
	map->buckets = (Node**)calloc(cap, sizeof(Node*));
	map->dummy.next = NULL;
}

int map_hash(Map* map, Key key)
{
	// Key 의 타입 별로 다르게 처리 필요
	return (int)key % map->n;
}

void map_insert(Map* map, Key key, Val val)
{
	int index = map_hash(map, key);

	if (map->buckets[index] != NULL)
	{
		Node* curr = map->buckets[index]->next;

		while (curr != NULL && curr->index == index)
		{
			if (curr->key == key)
			{
				curr->val = val;
				return;
			}

			curr = curr->next;
		}
	}

	// 없다면, 새로 생성하여 삽입
	Node* created = (Node*)malloc(sizeof(Node));
	created->index = index;
	created->next = NULL;
	created->key = key;
	created->val = val;

	// 인덱스의 머리가 없다면, dummy를 머리로, created 는 목이자 꼬리로
	if (map->buckets[index] == NULL)
	{
		created->next = map->dummy.next;
		map->dummy.next = created;
		map->buckets[index] = &map->dummy;

		if (created->next != NULL)
		{
			map->buckets[created->next->index] = created;
		}
	}
	// 인덱스의 머리가 이미 있다면, created를 목으로
	else
	{
		created->next = map->buckets[index]->next;
		map->buckets[index]->next = created;
	}
}

Val* map_at(Map* map, Key key)
{
	int index = map_hash(map, key);

	if (map->buckets[index] != NULL)
	{
		Node* curr = map->buckets[index]->next;

		while (curr != NULL && curr->index == index)
		{
			if (curr->key == key)
			{
				return &curr->val;
			}

			curr = curr->next;
		}
	}

	return NULL;
}









int* solution(const char* keymap[], size_t keymap_len, 
    const char* targets[], size_t targets_len) {

	Map* map = (Map*)malloc(sizeof(Map));
	map_init(map, 26);

	for (size_t i = 0; i < keymap_len; ++i)
	{
		const char* c = keymap[i];
		int rep = 1;
		while(*c)
		{
			int* val = map_at(map, *c);
			if (val == NULL)
			{
				map_insert(map, *c, rep);
			}
			else
			{
				if (rep < *val)
				{
					*val = rep;
				}
			}

			c++;
			rep++;
		}
	}




    int* answer = (int*)malloc(sizeof(int) * targets_len);
	for (size_t i = 0; i < targets_len; ++i)
	{
		int total = 0;
		const char* c = targets[i];
		while (*c)
		{
			int* val = map_at(map, *c);
			if (val == NULL)
			{
				total = -1;
				break;
			}

			total += *val;

			c++;
		}

		answer[i] = total;
	}


    return answer;
}