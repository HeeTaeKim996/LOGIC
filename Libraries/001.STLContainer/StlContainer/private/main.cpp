
#include <vector>
#include <algorithm>

#include <queue>
#include <functional>
#include <unordered_map>


#include "use/use_vector.h"
#include "use/use_priority_queue.h"
#include "use/use_unordered_map.h"
#include "use/use_unordered_set.h"
#include "use/use_map.h"
#include "use/use_set.h"

#include "use/use_memCounter.h"

using namespace std;

template<typename Key, typename Value>
struct TempGetKey
{
	const Key& operator()(const std::pair<Key, Value>& data) const
	{
		return data.first;
	}
};

int main()
{
#if 0 // test Vec
	{
		Vector<int> vecs;
		vecs.push_back(1);
		vecs.push_back(2);
		vecs.push_back(3);

		vecs.insert(vecs.begin() + 1, 9);


		for (int i = 0; i < vecs.size(); ++i)
		{
			printf("%d, ", vecs[i]);
		}
		printf("\n");
		for (auto it = vecs.begin(); it != vecs.end(); ++it)
		{
			printf("%d, ", *it);
		}
		printf("\nReverse: \n");
		for (auto it = vecs.rbegin(); it != vecs.rend(); ++it)
		{
			printf("%d, ", *it);
		}

		struct Temp
		{
			Temp(int inI, int inJ) : i(inI), j(inJ) {}
			Temp() {}
			int i;
			int j = 0;
		};

		printf("\n\nTemps : \n");
		Vector<Temp> temps;
		temps.push_back(Temp(1, 0));
		temps.push_back(Temp(2, 0));
		temps.push_back(Temp(3, 0));

		temps.insert(temps.begin() + 1, Temp(9, 0));
		temps.emplace(temps.begin() + 1, 7, 0);

		for (int i = 0; i < temps.size(); ++i)
		{
			printf("%d, ", temps[i].i);
		}
	}
	

	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Vec Test ==========


#if 0 // Test Priority_queue
	{
		Priority_queue<int> pq;
		pq.push(1);
		pq.push(3);
		pq.push(2);

		printf("\n pq1 : ");
		while (pq.empty() == false)
		{
			int top = pq.top();
			printf("%d, ", top);
			pq.pop();
		}


		struct Temp
		{
			Temp(int inY, int inX) : y(inY), x(inX) {}

			int y;
			int x;
		};

		struct Less
		{
			constexpr bool operator()(const Temp& a, const Temp& b) const
			{
				if (a.y == b.y)
				{
					return a.x < b.x;
				}
				return a.y < b.y;
			}
		};
		Less less;

		Priority_queue<Temp, Vector<Temp>, Less> pq2(less);
		pq2.push(Temp(0, 0));
		pq2.push(Temp(1, 0));
		pq2.push(Temp(0, 1));
		pq2.push(Temp(1, 1));
		printf("\n pq2 : ");
		while (pq2.empty() == false)
		{
			Temp top = pq2.top();
			printf("(%d, %d) ", top.y, top.x);
			pq2.pop();
		}

		// stl priority_queue 테스트
		priority_queue<Temp, vector<Temp>, Less> pq3(less);
		pq3.push(Temp(0, 0));
		pq3.push(Temp(1, 0));
		pq3.push(Temp(0, 1));
		pq3.push(Temp(1, 1));
		printf("\n pq3 : ");
		while (pq3.empty() == false)
		{
			Temp top = pq3.top();
			printf("(%d, %d) ", top.y, top.x);
			pq3.pop();
		}


		// auto 로 대체되는지 테스트
		auto less2 = [](const Temp& a, const Temp& b) -> bool
			{
				if (a.y == b.y)
				{
					return a.x > b.x;
				}
				return a.y > b.y;
			};

		Priority_queue<Temp, Vector<Temp>, bool(*)(const Temp&, const Temp&)> pq4(less2);
		pq4.push(Temp(0, 0));
		pq4.push(Temp(1, 0));
		pq4.push(Temp(0, 1));
		pq4.push(Temp(1, 1));
		printf("\n pq4 : ");
		while (pq4.empty() == false)
		{
			Temp top = pq4.top();
			printf("(%d, %d) ", top.y, top.x);
			pq4.pop();
		}
	}
	

	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Priority_queue Test ==========


#if 0 // Unordered_map
	{
		printf("========== Unordered_map Test ==========\n\n");

		Unordered_map<int, int> umap;


		// ========================================
		// 1. insert 테스트
		// ========================================
		printf("[1. insert]\n");

		auto result1 = umap.insert(pair<int, int>(1, 2));
		printf("insert(1, 2) : success = %d\n", result1.second);

		auto result2 = umap.insert(pair<int, int>(3, 4));
		printf("insert(3, 4) : success = %d\n", result2.second);

		// 중복 키 삽입
		auto result3 = umap.insert(pair<int, int>(1, 999));
		printf("insert(1, 999) : success = %d\n", result3.second);

		printf("size : %d\n\n", (int)umap.size()); // size : 2나와야 함


		// ========================================
		// 2. operator[] 테스트
		// ========================================
		printf("[2. operator[]]\n");

		// 기존 키 값 변경
		umap[1] = 7;

		// 존재하지 않는 키 생성
		umap[5] = 6;

		printf("umap[1] : %d\n", umap[1]);
		printf("umap[3] : %d\n", umap[3]);
		printf("umap[5] : %d\n\n", umap[5]);


		// ========================================
		// 3. find 테스트
		// ========================================
		printf("[3. find]\n");

		auto it1 = umap.find(1);

		if (it1 != umap.end())
		{
			printf("find(1) : %d : %d\n",
				it1->first,
				it1->second);
		}
		else
		{
			printf("find(1) : Not Found\n");
		}


		auto it2 = umap.find(3);

		if (it2 != umap.end())
		{
			printf("find(3) : %d : %d\n",
				it2->first,
				it2->second);
		}
		else
		{
			printf("find(3) : Not Found\n");
		}


		auto it3 = umap.find(999);

		if (it3 != umap.end())
		{
			printf("find(999) : %d : %d\n",
				it3->first,
				it3->second);
		}
		else
		{
			printf("find(999) : Not Found\n");
		}

		printf("\n");


		// ========================================
		// 4. 전체 순회 테스트
		// ========================================
		printf("[4. iterator]\n");

		for (auto it = umap.begin();
			it != umap.end();
			++it)
		{
			printf("%d : %d\n",
				it->first,
				it->second);
		}

		printf("\n");


		// ========================================
		// 5. erase 테스트
		// ========================================
		printf("[5. erase]\n");

		printf("erase(3) : %d\n",
			(int)umap.erase(3));

		printf("erase(999) : %d\n",
			(int)umap.erase(999));

		auto erasedIt = umap.find(3);

		if (erasedIt == umap.end())
		{
			printf("find(3) after erase : Not Found\n");
		}

		printf("size : %d\n\n",
			(int)umap.size());


		// ========================================
		// 6. resize 테스트
		// ========================================
		printf("[6. resize]\n");

		for (int i = 10; i < 30; i++)
		{
			umap.insert(pair<int, int>(i, i * 10));
		}

		bool allFound = true;

		for (int i = 10; i < 30; i++)
		{
			auto it = umap.find(i);

			if (it == umap.end())
			{
				printf("ERROR : %d Not Found\n", i);
				allFound = false;
			}
			else if (it->second != i * 10)
			{
				printf("ERROR : %d Value Error\n", i);
				allFound = false;
			}
		}

		printf("resize test : %s\n\n",
			allFound ? "SUCCESS" : "FAILED");


		// ========================================
		// 7. clear 테스트
		// ========================================
		printf("[7. clear]\n");

		umap.clear();

		printf("empty : %d\n",
			umap.empty());

		printf("size : %d\n",
			(int)umap.size());

		printf("begin == end : %d\n",
			umap.begin() == umap.end());


		// ========================================
		// 8. clear 이후 재사용 테스트
		// ========================================
		printf("\n[8. reuse after clear]\n");

		umap.insert(pair<int, int>(100, 200));

		auto it = umap.find(100);

		if (it != umap.end())
		{
			printf("%d : %d\n",
				it->first,
				it->second);
		}
		else
		{
			printf("ERROR : reuse failed\n");
		}

		printf("\n========== Test End ==========\n");
	}



	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Unordered_map Test ==========

#if 0 // Unordered_set

	{

		printf("========== Unordered_set Test ==========\n\n");

		Unordered_set<int> uset;


		// ========================================
		// 1. insert 테스트
		// ========================================
		printf("[1. insert]\n");

		auto result1 = uset.insert(1);
		printf("insert(1) : success = %d\n", result1.second);

		auto result2 = uset.insert(3);
		printf("insert(3) : success = %d\n", result2.second);

		// 중복 삽입
		auto result3 = uset.insert(1);
		printf("insert(1) duplicate : success = %d\n", result3.second);

		printf("size : %d\n\n", (int)uset.size());


		// ========================================
		// 2. find 테스트
		// ========================================
		printf("[2. find]\n");

		auto it1 = uset.find(1);

		if (it1 != uset.end())
		{
			printf("find(1) : %d\n", *it1);
		}
		else
		{
			printf("find(1) : Not Found\n");
		}


		auto it2 = uset.find(3);

		if (it2 != uset.end())
		{
			printf("find(3) : %d\n", *it2);
		}
		else
		{
			printf("find(3) : Not Found\n");
		}


		auto it3 = uset.find(999);

		if (it3 != uset.end())
		{
			printf("find(999) : %d\n", *it3);
		}
		else
		{
			printf("find(999) : Not Found\n");
		}

		printf("\n");


		// ========================================
		// 3. 전체 순회 테스트
		// ========================================
		printf("[3. iterator]\n");

		for (auto it = uset.begin();
			it != uset.end();
			++it)
		{
			printf("%d\n", *it);
		}

		printf("\n");


		// ========================================
		// 4. erase 테스트
		// ========================================
		printf("[4. erase]\n");

		printf("erase(3) : %d\n",
			(int)uset.erase(3));

		printf("erase(999) : %d\n",
			(int)uset.erase(999));


		auto erasedIt = uset.find(3);

		if (erasedIt == uset.end())
		{
			printf("find(3) after erase : Not Found\n");
		}

		printf("size : %d\n\n",
			(int)uset.size());


		// ========================================
		// 5. resize 테스트
		// ========================================
		printf("[5. resize]\n");

		for (int i = 10; i < 60; i++)
		{
			uset.insert(i);
		}


		bool allFound = true;

		for (int i = 10; i < 60; i++)
		{
			auto it = uset.find(i);

			if (it == uset.end())
			{
				printf("ERROR : %d Not Found\n", i);
				allFound = false;
			}
			else if (*it != i)
			{
				printf("ERROR : %d Value Error\n", i);
				allFound = false;
			}
		}

		printf("resize test : %s\n\n",
			allFound ? "SUCCESS" : "FAILED");


		// ========================================
		// 6. clear 테스트
		// ========================================
		printf("[6. clear]\n");

		uset.clear();

		printf("empty : %d\n",
			uset.empty());

		printf("size : %d\n",
			(int)uset.size());

		printf("begin == end : %d\n",
			uset.begin() == uset.end());


		// ========================================
		// 7. clear 이후 재사용 테스트
		// ========================================
		printf("\n[7. reuse after clear]\n");

		uset.insert(100);

		auto reuseIt = uset.find(100);

		if (reuseIt != uset.end())
		{
			printf("find(100) : %d\n", *reuseIt);
		}
		else
		{
			printf("ERROR : reuse failed\n");
		}


		printf("\n========== Test End ==========\n");
	}


	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Unordered_set Test ==========


		
#if 0
	{
		printf("========== Map Test ==========\n\n");

		Map<int, int> map;


		// ========================================
		// 1. insert 테스트
		// ========================================
		printf("[1. insert]\n");

		auto result1 = map.insert({ 3, 30 });
		printf("insert(3, 30) : success = %d\n", result1.second);

		auto result2 = map.insert({ 1, 10 });
		printf("insert(1, 10) : success = %d\n", result2.second);

		auto result3 = map.insert({ 5, 50 });
		printf("insert(5, 50) : success = %d\n", result3.second);

		// 중복 삽입
		auto result4 = map.insert({ 3, 300 });
		printf("insert(3, 300) duplicate : success = %d\n", result4.second);

		printf("size : %d\n\n", (int)map.size());


		// ========================================
		// 2. find 테스트
		// ========================================
		printf("[2. find]\n");

		auto it1 = map.find(3);

		if (it1 != map.end())
		{
			printf("find(3) : {%d, %d}\n", it1->first, it1->second);
		}
		else
		{
			printf("find(3) : Not Found\n");
		}

		auto it2 = map.find(999);

		if (it2 != map.end())
		{
			printf("find(999) : {%d, %d}\n", it2->first, it2->second);
		}
		else
		{
			printf("find(999) : Not Found\n");
		}

		printf("\n");


		// ========================================
		// 3. 전체 순회 테스트
		// ========================================
		printf("[3. iterator]\n");

		for (auto it = map.begin(); it != map.end(); ++it)
		{
			printf("{%d, %d}\n", it->first, it->second);
		}

		printf("\n");


		// ========================================
		// 4. operator[] 테스트
		// ========================================
		printf("[4. operator[]]\n");

		map[3] = 3000;
		printf("map[3] = %d\n", map[3]);

		map[7] = 70;
		printf("map[7] = %d\n", map[7]);

		printf("size : %d\n\n", (int)map.size());




		// ========================================
		// 6. 정렬 순서 순회 테스트
		// ========================================
		printf("[6. sorted order]\n");

		for (auto it = map.begin();
			it != map.end();
			++it)
		{
			printf("%d -> %d\n", it->first, it->second);
		}

		printf("\n");


		// ========================================
		// 7. Map iterator -> Vector 테스트
		// ========================================
		printf("[7. iterator -> Vector]\n");

		//Vector<std::pair<const int, int>> vec(map.begin(), map.end());
		Vector vec(map.begin(), map.end());

		printf("Vector size : %d\n", (int)vec.size());

		for (auto it = vec.begin(); it != vec.end(); ++it)
		{
			printf("{%d, %d}\n", it->first, it->second);
		}

		printf("\n");


		// ========================================
		// 8. erase 테스트
		// ========================================
		printf("[8. erase]\n");

		printf("erase(3) : %d\n",
			(int)map.erase(3));

		printf("erase(999) : %d\n",
			(int)map.erase(999));

		auto erasedIt = map.find(3);

		if (erasedIt == map.end())
		{
			printf("find(3) after erase : Not Found\n");
		}

		printf("size : %d\n\n",
			(int)map.size());


		// ========================================
		// 9. clear 테스트
		// ========================================
		printf("[9. clear]\n");

		map.clear();

		printf("empty : %d\n",
			map.empty());

		printf("size : %d\n",
			(int)map.size());

		printf("begin == end : %d\n",
			map.begin() == map.end());


		// ========================================
		// 10. clear 이후 재사용 테스트
		// ========================================
		printf("\n[10. reuse after clear]\n");

		map.insert({ 100, 1000 });

		auto reuseIt = map.find(100);

		if (reuseIt != map.end())
		{
			printf("find(100) : {%d, %d}\n",
				reuseIt->first,
				reuseIt->second);
		}
		else
		{
			printf("ERROR : reuse failed\n");
		}


		printf("\n========== Test End ==========\n");
	}


	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Map Test ==========

#if 1
	{
		printf("\n========== Set Test ==========\n");

		Set<int> set;

		// [1. insert]
		printf("\n[1. insert]\n");

		auto result1 = set.insert(3);
		printf("insert(3) : success = %d\n", result1.second);

		auto result2 = set.insert(1);
		printf("insert(1) : success = %d\n", result2.second);

		auto result3 = set.insert(5);
		printf("insert(5) : success = %d\n", result3.second);

		auto result4 = set.insert(3);
		printf("insert(3) duplicate : success = %d\n", result4.second);

		printf("size : %zu\n", set.size());


		// [2. find]
		printf("\n[2. find]\n");

		auto it1 = set.find(3);

		if (it1 != set.end())
			printf("find(3) : %d\n", *it1);
		else
			printf("find(3) : Not Found\n");

		auto it2 = set.find(999);

		if (it2 != set.end())
			printf("find(999) : %d\n", *it2);
		else
			printf("find(999) : Not Found\n");


		// [3. iterator]
		printf("\n[3. iterator]\n");

		for (auto it = set.begin(); it != set.end(); ++it)
		{
			printf("%d\n", *it);
		}


		// [4. sorted order]
		printf("\n[4. sorted order]\n");

		for (auto it = set.begin(); it != set.end(); ++it)
		{
			printf("%d\n", *it);
		}



		// ========================================
		// 4.++  Set iterator -> Vector 테스트
		// ========================================
		printf("\n[4.++ iterator -> Vector]\n");

		Vector vec(set.begin(), set.end());

		printf("Vector size : %d\n", (int)vec.size());

		for (auto it = vec.begin(); it != vec.end(); ++it)
		{
			printf("%d,\t", *it, *it);
		}
		printf("\n\n");



		// [5. erase]
		printf("\n[5. erase]\n");

		printf("erase(3) : %zu\n", set.erase(3));
		printf("erase(999) : %zu\n", set.erase(999));

		auto it3 = set.find(3);

		if (it3 != set.end())
			printf("find(3) after erase : %d\n", *it3);
		else
			printf("find(3) after erase : Not Found\n");

		printf("size : %zu\n", set.size());


		// [6. clear]
		printf("\n[6. clear]\n");

		set.clear();

		printf("empty : %d\n", set.empty());
		printf("size : %zu\n", set.size());
		printf("begin == end : %d\n", set.begin() == set.end());


		// [7. reuse after clear]
		printf("\n[7. reuse after clear]\n");

		set.insert(100);
		set.insert(200);
		set.insert(50);

		printf("size : %zu\n", set.size());

		for (auto it = set.begin(); it != set.end(); ++it)
		{
			printf("%d\n", *it);
		}


		printf("\n========== Test End ==========\n");
	}


	printf("\n\n MemCounter : \n");
	MemCounterManager::instance()->printAll();
#endif // ========== Set Test ==========
}

