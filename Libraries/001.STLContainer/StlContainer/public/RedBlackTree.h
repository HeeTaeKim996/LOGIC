#pragma once



#include "SizeType.h"
#include "Std.h"




template<typename Key, typename Data, typename Less, typename KeyOfValue>
class TreeIterator;


template<typename Data>
struct RBNode
{
	// _root 를 위한 기본 생성자
	RBNode() : parent(nullptr) {}


	RBNode(const Data& inData, RBNode* inParent)
		: data(inData), parent(inParent) {
	}

	Data data;
	RBNode* parent;
	RBNode* left = nullptr;
	RBNode* right = nullptr;

	bool isRed = false;
};



template<typename Key, typename Data, typename Less, typename KeyOfValue>
class RedBlackTree
{
public:
	using iterator = TreeIterator<Key, Data, Less, KeyOfValue>;
	using Node = RBNode<Data>;

private:
	friend class iterator;

private:
	Node* _root = nullptr;
	sizeType _size = 0;

	Less _less;
	KeyOfValue _keyOfValue;

public:
	bool empty() const noexcept { return _size == 0; }
	sizeType size() const noexcept { return _size; }

public:
	std::pair<iterator, bool> insert(const Data& data);
	sizeType erase(const Key& key);

private:
	void L(Node* parent);
	void R(Node* parent);

	void transplant(Node* oldNode, Node* newNode);
	Node* minimum(Node* node);

	void insertFixup(Node* node);
	void eraseFixup(Node* node, Node* parent);

private:
	Node* createNode(const Data& data, Node* parent);
	void deleteNode(Node* deleted);

};





























template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline std::pair<TreeIterator<Key, Data, Less, KeyOfValue>, bool> 
	RedBlackTree<Key, Data, Less, KeyOfValue>::insert(const Data& data)
{
	const Key& key = _keyOfValue(data);

	Node* parent = nullptr;
	Node* curr = _root;

	while (curr)
	{
		parent = curr;

		const Key& currKey = _keyOfValue(curr->data);

		if (_less(key, currKey))
		{
			curr = curr->left;
		}
		else if (_less(currKey, key))
		{
			curr = curr->right;
		}
		else
		{
			// 이미 존재하기에, return false
			return { iterator(curr), false };
		}
	}



	Node* newNode = createNode(data, parent);
	newNode->isRed = true; // newNode 의 컬러는 항상 레드로 시작


	if (parent == nullptr)
	{
		_root = newNode;
	}
	else
	{

		if (_less(key, _keyOfValue(parent->data)))
		{
			parent->left = newNode;
		}
		else
		{
			parent->right = newNode;
		}
	}


	// 위에 내용까지는 일반적인 트리의 insert와 같지만, 아래의 insertFixup 코드로, 생성시마다 위치를 재조정
	insertFixup(newNode);

	// 새로 생성했기에, return true
	return { iterator(newNode), true };
}




template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline sizeType RedBlackTree<Key, Data, Less, KeyOfValue>::erase(const Key& key)
{
	Node* target = _root;

	while (target)
	{
		const Key& targetKey = _keyOfValue(target->data);

		if (_less(key, targetKey))
		{
			target = target->left;
		}
		else if (_less(targetKey, key))
		{
			target = target->right;
		}
		else
		{
			break;
		}
	}

	// 존재하지 않기에, return 0
	if (target == nullptr) return 0;




	Node* fixupNode;
	bool fixup_WILL_PlacedNode_WAS_Red;

	Node* fixupParent;

	// 왼쪽 자식이 없다면, 오른쪽 자식을 erased 위치로 승격시킨다 (오른쪽 자식이 successor 노드)
	if (target->left == nullptr)
	{
		fixupNode = target->right;
		fixup_WILL_PlacedNode_WAS_Red = target->isRed;

		fixupParent = target->parent;

		transplant(target, target->right);
	}
	// 오른쪽 자식이 없다면, 오른쪽 자식을 erased 위치로 승격시킨다 (왼쪽 자식이 successor 노드)
	else if (target->right == nullptr)
	{
		fixupNode = target->left;
		fixup_WILL_PlacedNode_WAS_Red = target->isRed;

		fixupParent = target->parent;

		transplant(target, target->left);
	}
	// 양쪽 다 자식이 있다면, right 의 자식 중 가장 left 를 erased 위치로 승격시킨다
	else
	{
		Node* successorNode = minimum(target->right);

		fixupNode = successorNode->right;
		fixup_WILL_PlacedNode_WAS_Red = successorNode->isRed;

		// target->right 이 successorNode 라면, 
		// 단순히 target parent 의 위와 left만 successorNode에게 인계하면 되지만,
		// (아래 if-else 문 제외 코드들)
		
		// target->right 이 successorNode 가 아니라면, 
		// target 의 right 인계처리와, successorNode 의 위 인계처리 작업이
		// 추가로 필요하기에, 아래 else 코드가 추가됨
		if (target->right == successorNode)
		{
			fixupParent = successorNode;
		}
		else
		{
			fixupParent = successorNode->parent;

			// successorNode 의 위 인계 처리
			transplant(successorNode, fixupNode);

			// target의 right 인계 처리
			successorNode->right = target->right;
			target->right->parent = successorNode;
		}


		// target 의 위, left 인계처리
		transplant(target, successorNode);

		successorNode->left = target->left;
		target->left->parent = successorNode;


		// 위 두 케이스는 안그런데, 여기서만 SuccessorNode 의 색상을 targetNode의 색상으로 교체한다.
		// 이유는 아직 이해 못했다. 추후 나오겠지
		successorNode->isRed = target->isRed; 
	}

	deleteNode(target);

	if (fixup_WILL_PlacedNode_WAS_Red == false)
	{
		eraseFixup(fixupNode, fixupParent);
	}


	return 1;
}


































template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::L(Node* parent)
{
	Node* right = parent->right;

	// right->left 를 parent->right 로 인계
	parent->right = right->left;
	if (right->left)
	{
		right->left->parent = parent;
	}

	// parent->parent 를 right->parent 로 인계
	transplant(parent, right);


	// parent 와 parent->right 의 관계 전이(역전)
	right->left = parent;
	parent->parent = right;
}



template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::R(Node* parent)
{
	Node* left = parent->left;

	// left->right 를 parent->left 로 인계
	parent->left = left->right;
	if (left->right)
	{
		left->right->parent = parent;
	}

	// parent->parent 를 left->parent 로 인계
	transplant(parent, left);

	// parnet 와 parent->left 의 관계 전이(역전)
	left->right = parent;
	parent->parent = left;
}

template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::transplant(Node* oldNode, Node* newNode)
{
	if (oldNode->parent == nullptr)
	{
		_root = newNode;
	}
	else if (oldNode == oldNode->parent->left)
	{
		oldNode->parent->left = newNode;
	}
	else
	{
		oldNode->parent->right = newNode;
	}

	if (newNode)
	{
		newNode->parent = oldNode->parent;
	}
}

template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline RBNode<Data>*
	RedBlackTree<Key, Data, Less, KeyOfValue>::minimum(Node* node)
{
	while (node->left)
	{
		node = node->left;
	}

	return node;
}






















template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::insertFixup(Node* node)
{
	if (node == _root)
	{
		node->isRed = false;
		return;
	}

	while (node->parent && node->parent->isRed)
	{
		Node* parent = node->parent;
		Node* grandParent = parent->parent;


		if (parent == grandParent->left)
		{
			Node* uncle = grandParent->right;

			if (uncle && uncle->isRed)
			{
				parent->isRed = false;
				uncle->isRed = false;
				grandParent->isRed = true;

				node = grandParent;
			}
			else
			{
				if (node == parent->right)
				{
					node = parent;
					L(node);
					parent = node->parent;
				}

				parent->isRed = false;
				grandParent->isRed = true;
				R(grandParent);

				break;
			}
		}
		else
		{
			Node* uncle = grandParent->left;

			if (uncle && uncle->isRed)
			{
				parent->isRed = false;
				uncle->isRed = false;
				grandParent->isRed = true;

				node = grandParent;
			}
			else
			{
				if (node == parent->left)
				{
					node = parent;
					R(node);
					parent = node->parent;
				}

				parent->isRed = false;
				grandParent->isRed = true;
				L(grandParent);

				break;
			}
		}
	}

	_root->isRed = false;
}

template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::eraseFixup(Node* node, Node* parent)
{
}












template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline RBNode<Data>* 
	RedBlackTree<Key, Data, Less, KeyOfValue>::createNode(const Data& data, Node* parent)
{
	_size++;
	return new RBNode(data, parent);
}



template<typename Key, typename Data, typename Less, typename KeyOfValue>
inline void RedBlackTree<Key, Data, Less, KeyOfValue>::deleteNode(Node* deleted)
{
	_size--;
	delete deleted;
}









































template<typename Key, typename Data, typename Less, typename KeyOfValue>
class TreeIterator
{
	using Node = RBNode<Data>;
public:
	TreeIterator(Node* node) : _node(node) {}


private:
	Node* _node;
};