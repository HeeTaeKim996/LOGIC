#pragma once

#include "HashTable.h"


template<typename Key>
struct JustReturnKey
{
	const Key& operator()(const Key& key) const
	{
		return key;
	}
};

template<typename Key, typename Hash = std::hash<Key>>
class Unordered_set
{
private:
	using HashTableType = HashTable<Key, Key, Hash, JustReturnKey<Key>>;

public:
	using iterator = typename HashTableType::iterator;

private:
	HashTableType _table;

public:
	// 생성자-소멸자는 만들지 않는다. _table 의 생성자-소멸자에서 모두 자동 처리됨

public:
	bool empty() const noexcept { return _table.empty(); }
	sizeType size() const noexcept { return _table.size(); }

	iterator begin() { return _table.begin(); }
	iterator end() { return _table.end(); }

	std::pair<iterator, bool> insert(const Key& key) { return _table.insert(key); }
	
	void clear() { _table.clear(); }

	iterator find(const Key& key) { return _table.find(key); }

	sizeType erase(const Key& key) { return _table.erase(key); }
};
