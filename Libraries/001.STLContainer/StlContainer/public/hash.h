#pragma once

#include "SizeType.h"
#include <type_traits>

using namespace std;

template<typename Key>
class Hash
{
public:
	static sizeType hash(const Key& key, sizeType bucketCount);

};

template<typename Key>
inline sizeType Hash<Key>::hash(const Key& key, sizeType bucketCount)
{
	if constexpr (std::is_same_v<Key, int>)
	{
		return key % bucketCount;
	}

	return 0;
}









