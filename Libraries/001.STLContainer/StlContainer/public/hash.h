#pragma once

#include "SizeType.h"
#include <type_traits>

using namespace std;

template<typename Key>
class Hash
{
public:
	static size_t hash(const Key& key);

};

template<typename Key>
inline size_t Hash<Key>::hash(const Key& key)
{
	if constexpr (std::is_same_v<Key, int>)
	{
		return key;
	}

	return 0;
}









