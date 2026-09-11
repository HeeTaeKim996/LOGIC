#pragma once

#include "RedBlackTree.h"



template<typename Key, typename Value>
struct GetKeyFromData
{
	const Key& operator()(const std::pair<const Key, Value>& data) const
	{
		return data.first;
	}
};


// Less = std::less<Key> : Less 가 기본 타입이면, Less 함수가 std::less<Key> 로 자동 할당됨
template<typename Key, typename Value, typename Less = std::less<Key>>
class Map
{
private:
	using TreeType = RedBlackTree<Key, std::pair<const Key, Value>, Less, GetKeyFromData<Key, Value>>;

public:
	using iterator = typename TreeType::iterator;

private:
	TreeType _tree;


public:

public:
	bool empty() const noexcept { return _tree.empty(); }
	sizeType size() const noexcept { return _tree.size(); }

	
	iterator begin() { return _tree.begin(); }
	iterator end() { return _tree.end(); }

	std::pair<iterator, bool> insert(const std::pair<Key, Value>& kv) { return _tree.insert(kv); }

	void clear() { _tree.clear(); }

	iterator find(const Key& key) { return _tree.find(key); }

	sizeType erase(const Key& key) { return _tree.erase(key); }


public:
	Value& operator[](const Key& key);
	Value& operator[](Key&& key);
};






template<typename Key, typename Value, typename Less>
inline Value& Map<Key, Value, Less>::operator[](const Key& key)
{
	auto findIt = _tree.find(key);

	if (findIt != _tree.end())
	{
		return findIt->second;
	}

	auto insertPair = _tree.insert(std::pair<Key, Value>(key, Value()));
	return insertPair.first->second;
}

template<typename Key, typename Value, typename Less>
inline Value& Map<Key, Value, Less>::operator[](Key&& key)
{
	auto findIt = _tree.find(key);

	if (findIt != _tree.end())
	{
		return findIt->second;
	}

	auto insertPair = _tree.insert(std::pair<Key, Value>(Std::move(key), Value()));
	return insertPair.first->second;
}