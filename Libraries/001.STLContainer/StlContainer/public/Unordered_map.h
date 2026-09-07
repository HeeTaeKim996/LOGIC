
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
		Node() {} // 더미 헤드를 위한 기본 생성자

		Node(const Key& inKey, const Value& inValue, size_t inHash)
			: data(inKey, inValue), hash(inHash)
		{
		}

		std::pair<Key, Value> data;
		size_t hash;
		Node* next = nullptr;
	};


public:
	Unordered_map();
	~Unordered_map();

public:
	bool empty() const noexcept { return _size == 0; }
	sizeType size() const noexcept { return _size; }



	iterator end() { return iterator(nullptr); }


public:
	pair<iterator, bool> insert(const pair<Key, Value>& kv);

	void clear();
	Value& operator[](const Key& key);
	Value& operator[](Key&& key);


		
	
	iterator find(const Key& key);
	sizeType erase(const Key& key); // bool 로 해도 되지만, stl 에서 저렇게 사용함


private:
	void resize();

	Node* createNode(const Key& key, const Value& value, const size_t inHash);
	void deleteNode(Node* deleted);


private:
	Node** _buckets;
	sizeType _bucketCount;
	sizeType _size;

	Node _dummyHead;
	// 아래 insert 함수를 보면 _dummyHead 의 역할이 잘 나온다. 더미헤드의 역할을 간략히 설명하면,
	// 임시로 _buckets[index] 의 머리 역할을 하며, A 버켓의 꼬리를 B버켓의 머리로 연결해주는 역할을 한다
	// 자세한 역할은 insert를 보며 이해하자
};



template<typename Key, typename Value>
inline Unordered_map<Key, Value>::Unordered_map()
{
#define INIT_BUCKET_COUNT 8

	_bucketCount = INIT_BUCKET_COUNT;
	_buckets = new Node* [_bucketCount] {};
	_size = 0;
	_dummyHead.next = nullptr;

}

template<typename Key, typename Value>
inline Unordered_map<Key, Value>::~Unordered_map()
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




template<typename Key, typename Value>
inline std::pair<Unordered_map_iterator<Key, Value>, bool> Unordered_map<Key, Value>
	::insert(const std::pair<Key, Value>& kv)
{
	const auto& [key, value] = kv;
	size_t hash = Hash<Key>::hash(key);
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
			if (curr->data.first == key)
			{
				// 기존 값이 이미 있다면, 기존 값을 갱신하지 않고, false를 반환한다
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

	Node* created = createNode(key, value, hash);
	


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
			sizeType nextIndex = created->next->hash % _bucketCount;
			_buckets[nextIndex] = created;
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





template<typename Key, typename Value>
inline Value& Unordered_map<Key, Value>::operator[](const Key& key)
{
	size_t hash = Hash<Key>::hash(key);
	sizeType index = hash % _bucketCount;

	if (_buckets[index] == nullptr)
	{

	}
}

template<typename Key, typename Value>
inline Value& Unordered_map<Key, Value>::operator[](Key&& key)
{
	// TODO: insert return statement here
}

template<typename Key, typename Value>
inline Unordered_map_iterator<Key, Value> Unordered_map<Key, Value>::find(const Key& key)
{
	size_t hash = Hash<Key>::hash(key);
	sizeType index = hash % _bucketCount;

	if (_buckets[index] == nullptr) return end();

	// 머리(_buckets[index]) 는 해당 버킷 소속이 아닌, 다른 버킷의 꼬리(또는 더미헤드)이기에, 
	// 검사하지 않고 목부터 검사한다
	Node* curr = _buckets[index]->next;
	while (curr != nullptr)
	{
		if (curr->hash % _bucketCount != index) break;

		if (curr->data.first == key) return iterator(curr);
		
		curr = curr->next;
	}
	

	return end();
}

template<typename Key, typename Value>
inline sizeType Unordered_map<Key, Value>::erase(const Key& key)
{
	size_t hash = Hash<Key>::hash(key);
	sizeType index = hash % _bucketCount;

	if (_buckets[index] == nullptr) return 0;

	Node* prev = _buckets[index];
	Node* curr = prev->next;


	while (curr != nullptr && curr->hash % _bucketCount == index)
	{
		if (curr->data.first == key)
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

template<typename Key, typename Value>
inline void Unordered_map<Key, Value>::resize()
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
				// curr 을 해당 버켓의 머리로
				newBuckets[curr->next->hash % newBucketCount] = curr;
			}
		}
		else
		{
			// 기존 목을 한칸 아래로 내리고, created 를 해당 버켓의 목으로
			curr->next = newBuckets[index]->next;
			newBuckets[index]->next = curr;
		}

		curr = nextNode;
	}

	delete[] _buckets;
	_buckets = newBuckets;
	_bucketCount = newBucketCount;
}

template<typename Key, typename Value>
inline typename Unordered_map<Key, Value>::Node* Unordered_map<Key, Value>::createNode
	(const Key& key, const Value& value, const size_t inHash)
{
	_size++;
	return new Node(key, value, inHash);
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
	const std::pair<Key, Value>* operator -> () { return &_node->data; }

	inline Unordered_map_iterator& operator ++ ()
	{
		_node = _node->next;
		return *this;
	}

	inline Unordered_map_iterator operator++(int)
	{
		Unordered_map_iterator temp = *this;
		_node = _node->next;
		return temp;
	}

	inline bool operator != (const Unordered_map_iterator& other) const
	{
		return _node != other._node;
	}

private:
	Node* _node;
};