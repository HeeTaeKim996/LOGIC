#pragma once




#include "SizeType.h"
#include "Std.h"
#include <string>
#include "RedBlackTree.h"


/*
	■ 정리
	- 해시함수는 std::hash<..> 사용
	- 체이닝은 폐쇄(Separate, Closed) 체이닝. 단방향 LinkedList 로 구현하되, DummyHead 를 사용하여,
	  A 버켓의 꼬리가 B버켓의 헤드가 되는 구조로, 모든 노드를 연결
*/


// Hash = std:hash<Key> : Key 가 기본타입이면, 해시 함수가 자동 할당될 예정 
//		(Unordered_map, Unordered_set 에서 할당 )
// 
// KeyOfValue  : Unordered_map, Unordered_set 에서 할당 

template<typename Key, typename T, typename Hash, typename KeyOfValue>
class HashTableIterator;

template<typename Key, typename T,typename Hash, typename KeyOfValue>
class HashTable
{
public:
	using iterator = HashTableIterator<Key, T, Hash, KeyOfValue>;

private:
	friend class iterator;

	struct Node
	{
		T data;
		size_t hash;
		Node* next = nullptr;
	};

private:
	Node** _buckets;
	sizeType _bucketCount;
	sizeType _size;
	
	Node _dummyHead;
	// 아래 insert 함수를 보면 _dummyHead 의 역할이 잘 나온다. 더미헤드의 역할을 간략히 설명하면,
	// 임시로 _buckets[index] 의 머리 역할을 하며, A 버켓의 꼬리를 B버켓의 머리로 연결해주는 역할을 한다
	// 자세한 역할은 insert를 보며 이해하자

	Hash _hash;
	KeyOfValue _keyOfValue;


public:
	HashTable(const Hash& hash = Hash(), const KeyOfValue& keyOfValue = KeyOfValue());
	~HashTable();

public:
	bool empty() const noexcept { return _size == 0; }
	sizeType size() const noexcept { return _size; }

	iterator begin() { return iterator(_dummyHead.next); }
	iterator end() { return iterator(nullptr); }

public:
	std::pair<iterator, bool> insert(const T& data);
	std::pair<iterator, bool> insert_or_assign(const T& data);

	void clear();

	iterator find(const Key& key);

	sizeType erase(const Key& key); // 리턴을 bool 로 해도 되지만, stl 에서 저렇게 사용함

private:
	void resize();
	Node* createNode(const T& data, const size_t& inHash);
	void deleteNode(Node* deleted);
};

















#define INIT_BUCKET_COUNT 8
template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline HashTable<Key, T, Hash, KeyOfValue>::HashTable(const Hash& hash, const KeyOfValue& keyOfValue)
	: _hash(hash), _size(0), _bucketCount(INIT_BUCKET_COUNT), _buckets(new Node* [INIT_BUCKET_COUNT] {}),
	_keyOfValue(keyOfValue)
{
	_dummyHead.next = nullptr;
}

template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline HashTable<Key, T, Hash, KeyOfValue>::~HashTable()
{
	Node* curr = _dummyHead.next;
	while (curr != nullptr)
	{
		Node* deleted = curr;
		curr = curr->next;
		delete deleted;
	}

	delete[] _buckets;
	_buckets = nullptr;
}

















template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline std::pair<HashTableIterator<Key, T, Hash, KeyOfValue>, bool> 
	HashTable<Key, T, Hash, KeyOfValue>::insert(const T& data)
{
	const Key& key = _keyOfValue(data);
	size_t hash = _hash(key);
	sizeType index = hash % _bucketCount;

	// ※ 꼬다리부터 머리(_buckets[index]) 방향으로  next를 무는 구조이다
	// ※ 여기서 머리는 _buckets[index]로, 해당 버켓에 소속되지 않은 노드이며, 다른 버켓의 
	//   꼬다리이다
	// ※ 내가 목 이라 표현하는 건, _buckets[index]->next 로, 실질적으로 해당 버켓에
	//   가장 마지막에 삽입된 노드이다
	// ※ 꼬리(머리) 는 해당 버켓에 가장 처음으로 삽입된 노드이다


	// 이미 있는지 검사 ( 머리가 아닌 목부터 검사 )
	if (_buckets[index] != nullptr)
	{
		Node* curr = _buckets[index]->next;
		while (curr != nullptr && curr->hash % _bucketCount == index)
		{
			if (_keyOfValue(curr->data) == key)
			{
				// 기존 값이 이미 있다면, 기존 값을 갱신하지 않고, false를 반환
				return { iterator(curr), false };
			}

			curr = curr->next;
		}
	}


	// 검색 실패. 추가한다


	// 크기가 작다면, 확장
	if (_size + 1 > _bucketCount)
	{
		resize();
		index = hash % _bucketCount;
	}

	Node* created = createNode(data, hash);

	if (_buckets[index] == nullptr)
	{
		// 더미헤드는 현 버켓에 할당되고, 기존 더미헤드가 물었던 다른 버켓의 
		// 목을 created가 next로 가진다
		created->next = _dummyHead.next;
		_dummyHead.next = created;
		_buckets[index] = &_dummyHead;

		// create 가 next로 문 머리가 nullptr 이 아니라면 (특정 버켓에 소속됐다면), 
		// created 가 그 특정 버켓의 머리가 된다 ( 직전에 더미가 위치했던 곳으로 이동 )
		// 또한 created 는 현 버켓에 최초로 추가된 노드이므로, 현 버켓의 꼬리이기도 하다
		if (created->next != nullptr)
		{
			_buckets[created->next->hash % _bucketCount] = created;
		}
	}
	else
	{
		// 머리(꼬리) 가 할당된 버켓에 노드가 추가되면, 기존 목이 아래로 내려가고,
		// 새로운 노드가 목이 된다
		created->next = _buckets[index]->next;
		_buckets[index]->next = created;
	}

	return { iterator(created), true };
}




template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline std::pair<HashTableIterator<Key, T, Hash, KeyOfValue>, bool>
HashTable<Key, T, Hash, KeyOfValue>::insert_or_assign(const T& data)
{
	// 아래 주석 내용 하나 빼고는 insert 와 모두 동일하다. 코드 분석할 필요 없다. isnert 랑 똑같다 보면 된다

	const Key& key = _keyOfValue(data);
	size_t hash = _hash(key);
	sizeType index = hash % _bucketCount;


	if (_buckets[index] != nullptr)
	{
		Node* curr = _buckets[index]->next;
		while (curr != nullptr && curr->hash % _bucketCount == index)
		{
			if (_keyOfValue(curr->data) == key)
			{
				// insert 와의 유일한 차이는 아래 curr->data = data; 가 추가된 것 뿐
				curr->data = data;
				return { iterator(curr), false };
			}

			curr = curr->next;
		}
	}


	if (_size + 1 > _bucketCount)
	{
		resize();
		index = hash % _bucketCount;
	}

	Node* created = createNode(data, hash);

	if (_buckets[index] == nullptr)
	{

		created->next = _dummyHead.next;
		_dummyHead.next = created;
		_buckets[index] = &_dummyHead;


		if (created->next != nullptr)
		{
			_buckets[created->next->hash % _bucketCount] = created;
		}
	}
	else
	{
		created->next = _buckets[index]->next;
		_buckets[index]->next = created;
	}

	return { iterator(created), true };
}
























template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline void HashTable<Key, T, Hash, KeyOfValue>::clear()
{
	Node* curr = _dummyHead.next;
	while (curr != nullptr)
	{
		Node* deleted = curr;
		curr = curr->next;
		delete deleted;
	}

	for (int i = 0; i < _bucketCount; i++)
	{
		_buckets[i] = nullptr;
	}

	_size = 0;
	_dummyHead.next = nullptr;
}










template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline HashTableIterator<Key, T, Hash, KeyOfValue>
	HashTable<Key, T, Hash, KeyOfValue>::find(const Key& key)
{
	size_t hash = _hash(key);
	sizeType index = hash % _bucketCount;

	if (_buckets[index] == nullptr) return end();

	// 머리(_buckets[index]) 는 해당 버킷 소속이 아닌, 다른 버킷의 꼬리(또는 더미헤드)이기에, 
	// 검사하지 않고 목부터 검사한다
	Node* curr = _buckets[index]->next;
	while (curr != nullptr)
	{
		if (curr->hash % _bucketCount != index) break;

		if (_keyOfValue(curr->data) == key) return iterator(curr);

		curr = curr->next;
	}

	return end();
}











template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline sizeType HashTable<Key, T, Hash, KeyOfValue>::erase(const Key& key)
{
	size_t hash = _hash(key);
	sizeType index = hash % _bucketCount;

	if (_buckets[index] == nullptr) return 0;

	Node* prev = _buckets[index];
	Node* curr = prev->next;

	while (curr != nullptr && curr->hash % _bucketCount == index)
	{
		if (_keyOfValue(curr->data) == key)
		{
			prev->next = curr->next;


			if (curr->next != nullptr)
			{
				sizeType nextIndex = curr->next->hash % _bucketCount;

				// curr->next 가 다른 버켓 소속이라는 건, 현재 삭제되는 curr 가 해당 버킷의 머리(_buckets[index]) 
				// 임을 의미하기에, prev(현 버킷의 새로운 꼬리)를 해당 버킷의 머리(_buckets[index]) 에 옮긴다
				if (nextIndex != index)
				{
					_buckets[nextIndex] = prev;
				}

			}

			// 현재 버켓이 텅 빈 경우, _buckets[index] 를 비우다 ( 다시 DummyHead를 받을 수 있게 )
			// (어차피 _buckets[index]-> next 는 이미 위에서 갱신됐기에, 비워도 문제 안된다 )
			if (prev == _buckets[index] &&
				(curr->next == nullptr || curr->next->hash % _bucketCount != index))
			{
				_buckets[index] = nullptr;
			}

			deleteNode(curr);
			return 1;
		}

		prev = curr;
		curr = curr->next;
	}

	return 0;
}






































template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline void HashTable<Key, T, Hash, KeyOfValue>::resize()
{
	sizeType newBucketCount = _bucketCount * 2;
	Node** newBuckets = new Node* [newBucketCount] {};

	Node* curr = _dummyHead.next;
	_dummyHead.next = nullptr;

	while (curr != nullptr)
	{
		Node* nextNode = curr->next;

		sizeType index = curr->hash % newBucketCount;

		if (newBuckets[index] == nullptr)
		{
			// curr 은 해당 버켓의 꼬리
			curr->next = _dummyHead.next;
			_dummyHead.next = curr;
			newBuckets[index] = &_dummyHead;

			if (curr->next != nullptr)
			{
				// curr 을 해당 버킷의 머리로
				newBuckets[curr->next->hash % newBucketCount] = curr;
			}
		}
		else
		{
			// 기존 목을 한칸 아래로 내리고, curr 를 해당 버킷의 목으로
			curr->next = newBuckets[index]->next;
			newBuckets[index]->next = curr;
		}

		curr = nextNode;
	}

	delete[] _buckets;
	_buckets = newBuckets;
	_bucketCount = newBucketCount;
}


template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline typename HashTable<Key, T, Hash, KeyOfValue>::Node*
HashTable<Key, T, Hash, KeyOfValue>::createNode(const T& data, const size_t& inHash)
{
	_size++;

	Node* node = new Node();
	node->data = data;
	node->hash = inHash;
	return node;
}





template<typename Key, typename T, typename Hash, typename KeyOfValue>
inline void HashTable<Key, T, Hash, KeyOfValue>::deleteNode(Node* deleted)
{
	_size--;

	delete deleted;
}








































template<typename Key, typename T, typename Hash, typename KeyOfValue>
class HashTableIterator
{
	using Node = typename HashTable<Key, T, Hash, KeyOfValue>::Node;

public:
	HashTableIterator(Node* node) : _node(node) {}

public:
	T* operator -> () { return &(_node->data); }
	T& operator *() { return _node->data; }


	inline HashTableIterator& operator ++ ()
	{
		_node = _node->next;
		return *this;
	}

	inline HashTableIterator operator++(int)
	{
		HashTableIterator temp = *this;
		_node = _node->next;
		return temp;
	}

	inline bool operator != (const HashTableIterator& other) const
	{
		return _node != other._node;
	}

	inline bool operator == (const HashTableIterator& other) const
	{
		return _node == other._node;
	}

private:
	Node* _node;
};