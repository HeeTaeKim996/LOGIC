#pragma once


#include "RedBlackTree.h"

template<typename Key>
struct JustReturnKeyFromSet
{
	const Key& operator()(const Key& key) const
	{
		return key;
	}
};

template<typename Key, typename Less = std::less<Key>>
class Set
{
private:
	using TreeType = RedBlackTree<Key, Key, Less, JustReturnKeyFromSet<Key>>;

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

	std::pair<iterator, bool> insert(const Key& key) { return _tree.insert(key); }

	void clear() { _tree.clear(); }

	iterator find(const Key& key) { return _tree.find(key); }

	sizeType erase(const Key& key) { return _tree.erase(key); }
};