#include "pch.h"
#include <iostream>
#include "Search.h"
#include "Sort.h"
#include "Array.h"
#include "3.1.StringMatch.h"
#include "3.2.Trie.h"

#include "use/use_vector.h"
#include "use/use_priority_queue.h"

#include "use/use_unordered_map.h"

#include "use/use_unordered_set.h"

int main()
{
	//Search::test();
	//Sort::test();
	//test_Array();
	//StringMatch::test();
	//Trie::test();


	vector<int> vec;
	vec.push_back(1);
	vec.push_back(3);
	vec.push_back(2);

	for (int i = 0; i < vec.size(); i++)
	{
		printf("%d, ", vec[i]);
	}

	Priority_queue<int> pq;
	pq.push(1);
	pq.push(3);
	pq.push(2);
	printf("\npq : ");
	while (pq.empty() == false)
	{
		int top = pq.top();
		printf("%d, ", top);
		pq.pop();
	}


	printf("\nUMAP test\n");
	unordered_map<int, int> umap;
	umap.insert({ 1, 2});
	umap.insert({ 3, 4 });
	auto it = umap.find(1);
	printf("t1 : %d -> %d \n", it->first, it->second);
	auto it2 = umap.find(3);
	printf("t1 : %d -> %d \n", it2->first, it2->second);

	printf("\n USET test\n");
	Unordered_set<int> uset;
	uset.insert(1);
	uset.insert(3);
	auto it3 = uset.find(1);
	if (it3 != uset.end())
	{
		printf("Found : %d\n", *it3);
	}
	auto it4 = uset.find(999);
	if (it4 == uset.end())
	{
		printf("Not Found\n");
	}
}


