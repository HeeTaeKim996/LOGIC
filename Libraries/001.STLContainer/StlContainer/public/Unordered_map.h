#pragma once

#include "HashTable.h"







template<typename Key, typename Value>
struct MakeKeyFromData
{
	const Key& operator()(const std::pair<Key, Value>& data) const
	{
		return data.first;
	}
};



// Hash = std:hash<Key> : Key 가 기본타입이면, 해시 함수가 자동 할당됨
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class Unordered_map
{
private:
	using DataType = std::pair<Key, Value>;
	using HashTableType = HashTable<Key, DataType, Hash, MakeKeyFromData<Key, Value>>;

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

	std::pair<iterator, bool> insert(const std::pair<Key, Value>& kv) { return _table.insert(kv); }
	std::pair<iterator, bool> insert_or_assign(const std::pair<Key, Value>& kv) { return _table.insert_or_assign(kv); }

	void clear() { _table.clear(); }

	iterator find(const Key& key) { return _table.find(key); }

	sizeType erase(const Key& key) { return _table.erase(key); }


public:
	Value& operator[](const Key& key);
	Value& operator[](Key&& key);

};







template<typename Key, typename Value, typename Hash>
inline Value& Unordered_map<Key, Value, Hash>::operator[](const Key& key)
{
	auto findIt = _table.find(key);	

	if (findIt != _table.end())
	{
		return findIt->second;
	}

	auto insertPair = _table.insert(std::pair<Key, Value>(key, Value()));
	return insertPair.first->second;
}

template<typename Key, typename Value, typename Hash>
inline Value& Unordered_map<Key, Value,Hash>::operator[](Key&& key)
{
	auto findIt = _table.find(key);

	if (findIt != _table.end())
	{
		return findIt->second;
	}

	auto insertPair = _table.insert(std::pair<Key, Value>(Std::move(key), Value()));
	return insertPair.first->second;
}


