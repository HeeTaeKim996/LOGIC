#include "Vector.h"

#include <vector>
#include <algorithm>

#include <queue>
#include "Priority_queue.h"
#include <functional>
#include <unordered_map>
#include "Unordered_map.h"

using namespace std;

int main()
{
#if 0 // test Vec
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
	
	temps.insert(temps.begin() +1, Temp(9, 0));
	temps.emplace(temps.begin() + 1, 7, 0);

	for (int i = 0; i < temps.size(); ++i)
	{
		printf("%d, ", temps[i].i);
	}
#endif


#if 0 // Test Priority_queue

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

#endif


#if 1 // Unordered_map
	Unordered_map<int, int> umap;
	umap.insert(pair<int, int>(1, 2));
	umap.insert(pair<int, int>(3, 4));

	auto it = umap.find(1);
	printf("%d : %d \n", it->key, it->value);
	auto it1 = umap.find(3);
	printf("%d : %d \n", it1->key, it1->value);
	

#endif
}

