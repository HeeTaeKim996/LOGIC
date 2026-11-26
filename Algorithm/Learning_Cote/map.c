#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

typedef int Key;
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
	return key % map->n;
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


void map_erase(Map* map, Key key)
{
	int index = map_hash(map, key);
	if (map->buckets[index] == NULL) return;

	Node* prev = map->buckets[index];
	Node* curr = prev->next;

	while (curr != NULL && curr->index == index)
	{
		if (curr->key == key)
		{
			prev->next = curr->next;

			
			if (curr->next != NULL)
			{
				int nextIndex = curr->next->index;

				// 현 노드가 다른 인덱스의 머리였다면, 
				// prev를 다른 인덱스의 머리로
				if (nextIndex != index)
				{
					map->buckets[nextIndex] = prev;
				}
			}

			// 현 index 가 텅 비게 됐다면, 인덱스의 머리를 제거
			if (prev == map->buckets[index]
				&& (curr->next == NULL || curr->next->index != index))
			{
				map->buckets[index] = NULL;
			}

			free(curr);
			return;
		}

		prev = curr;
		curr = curr->next;
	}
}

































void test_map()
{
	Map* map = (Map*)malloc(sizeof(Map));
	map_init(map, 5);

	map_insert(map, 1, 11);
	map_insert(map, 2, 22);
	map_insert(map, 5, 55);

	printf("%d, %d, %d\n", *map_at(map, 1), *map_at(map, 2),
		*map_at(map, 5));

	map_erase(map, 2);
	if (map_at(map, 2) == NULL)
	{
		printf("삭제 성공\n");
	}
}