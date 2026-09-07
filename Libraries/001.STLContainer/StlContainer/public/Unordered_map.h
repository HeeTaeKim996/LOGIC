
#include "SizeType.h"

#include "hash.h"

#include <string>

template<typename Key, typename Value>
class Unordered_map_iterator;


template<typename Key, typename Value>
class Unordered_map
{
public:
	using iterator = Unordered_map_iterator<Key, Value>;


private:
	friend class Unordered_map_iterator<Key, Value>;

	struct Node
	{
		Node(const Key& inKey, const Value& inValue)
			: key(inKey), value(inValue)
		{
		}


		Key key;
		Value value;

		Node* next = nullptr;
	};


public:
	Unordered_map();
	~Unordered_map();

public:
	iterator end() { return iterator(nullptr); }


public:
	pair<iterator, bool> insert(const pair<Key, Value>& kv);
	iterator find(const Key& key);
	sizeType erase(const Key& key); // bool 로 해도 되지만, stl 에서 저렇게 사용함


private:
	void moveNode(Node** buckets, Node* movingNode, sizeType bucketCount);
	void resize();

	Node* createNode(const Key& key, const Value& value);
	void deleteNode(Node* deleted);


private:
	Node** _buckets;
	sizeType _bucketCount;
	sizeType _size;
};



template<typename Key, typename Value>
inline Unordered_map<Key, Value>::Unordered_map()
{
#define INIT_BUCKET_COUNT 8

	_buckets = new Node* [INIT_BUCKET_COUNT] {};
	_bucketCount = INIT_BUCKET_COUNT;
	_size = 0;

}

template<typename Key, typename Value>
inline Unordered_map<Key, Value>::~Unordered_map()
{
	for (sizeType i = 0; i < _bucketCount; i++)
	{
		Node* node = _buckets[i];
		while (node != nullptr)
		{
			Node* deleted = node;
			node = node->next;
			delete deleted;
		}
	}
	
	delete[] _buckets;
	_buckets = nullptr;
}


template<typename Key, typename Value>
inline std::pair<Unordered_map_iterator<Key, Value>, bool> Unordered_map<Key, Value>
	::insert(const std::pair<Key, Value>& kv)
{
	const auto& [key, value] = kv;

	sizeType index = Hash<Key>::hash(key, _bucketCount);



	Node* node = _buckets[index];
	while (node != nullptr)
	{
		if (node->key == key)
		{
			node->value = value;
			return std::pair<iterator, bool>(iterator(node), false);
		}

		node = node->next;
	}

	// 여기까지 왔다면, 노드가 없으니, 루트에 새로 삽입
	
	if (_size + 1 > _bucketCount)
	{
		resize();
		index = Hash<Key>::hash(key, _bucketCount);
	}

	Node* created = createNode(key, value);

	created->next = _buckets[index];
	_buckets[index] = created;

	return std::pair<iterator, bool>(iterator(node), true);
}

template<typename Key, typename Value>
inline void Unordered_map<Key, Value>::moveNode(Node** buckets, Node* movingNode, 
	sizeType bucketCount)
{
	sizeType index = Hash<Key>::hash(movingNode->key, bucketCount);

	movingNode->next = buckets[index];
	buckets[index] = movingNode;
}

template<typename Key, typename Value>
inline Unordered_map_iterator<Key, Value> Unordered_map<Key, Value>::find(const Key& key)
{
	sizeType index = Hash<Key>::hash(key, _bucketCount);

	Node* node = _buckets[index];
	while (node != nullptr)
	{
		if (node->key == key) return iterator(node);
		node = node->next;
	}

	return end();
}

template<typename Key, typename Value>
inline sizeType Unordered_map<Key, Value>::erase(const Key& key)
{
	sizeType index = Hash<Key>::hash(key, _bucketCount);


	Node** curr = _buckets + index;
	while (*curr != nullptr)
	{
		if ((*curr)->key == key)
		{
			Node* deleted = *curr;
			*curr = deleted->next;
			
			deleteNode(deleted);

			return 1;
		}

		curr = &((*curr)->next);
	}

	return 0;
}

template<typename Key, typename Value>
inline void Unordered_map<Key, Value>::resize()
{
	sizeType newBucketCount = _bucketCount * 2;
	Node** newBuckets = new Node* [newBucketCount] {};


	for (sizeType i = 0; i < _bucketCount; i++)
	{
		Node* node = _buckets[i];
		while (node != nullptr)
		{
			Node* movingNode = node;
			node = node->next;

			//movingNode->next = nullptr;
			// 어차피 modeNode 에서 movingNode->next = buckets[index]; 로 초기화됨

			moveNode(newBuckets, movingNode, newBucketCount);
		}
	}

	delete[] _buckets;
	_buckets = newBuckets;
	_bucketCount = newBucketCount;
}

template<typename Key, typename Value>
inline typename Unordered_map<Key, Value>::Node* Unordered_map<Key, Value>::createNode
	(const Key& key, const Value& value)
{
	_size++;
	return new Node(key, value);
}

template<typename Key, typename Value>
inline void Unordered_map<Key, Value>::deleteNode(Node* deleted)
{
	delete deleted;
	_size--;
}


























template<typename Key, typename Value>
class Unordered_map_iterator
{
	using Map = Unordered_map < Key, Value>;
	using Node = typename Map::Node;
public:
	Unordered_map_iterator(Node* node)
		:_node(node)
	{}

public:
	Node* operator -> () { return _node; }

	// operator 는 추후 구현



private:
	Node* _node;
};