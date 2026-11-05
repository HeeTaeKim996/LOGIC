#pragma once

#include <stdio.h>

#include "Array.h"
#include <functional>

using namespace std;

class BinarySearch
{
public:
	static void test();


public:
	template<typename T>
	static int binarySearch(const Array<T>& arrs, const T& find, function<int(const T& a, const T& b)> compare);
};

template<typename T>
inline int BinarySearch::binarySearch(const Array<T>& arrs, const T& find, function<int(const T& a, const T& b)> compare)
{
	if (arrs.size() == 0) return -1;

	int start = 0;
	int end = arrs.size() - 1;


	while (start <= end)
	{
		int mid = (start + end) / 2;

		const T& midVal = arrs[mid];

		int comp = compare(find, midVal);

		if (comp < 0)
		{
			end = mid - 1;
		}
		else if (comp > 0)
		{
			start = mid + 1;
		}
		else
		{
			return mid;
		}
	}

	return -1;
}







inline void BinarySearch::test()
{
	Array<int> arrs
	(
		{ 1, 2, 3, 4, 5 }
	);

	auto compare = [](const int& a, const int& b) -> int
		{
			if (a < b)
			{
				return -1;
			}
			else if (a > b)
			{
				return 1;
			}
			return 0;
		};


	for (int i = 0; i <= 6; i++)
	{
		printf("%d, ", BinarySearch::binarySearch<int>(arrs, i, compare));
	}
}
