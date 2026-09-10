#pragma once



#include "SizeType.h"
#include "Std.h"
/*
	내용이 굉장히 어렵다. 이 내용을 주석을 보며 이해하기 어렵기에, 
	따로 노트로 정리를 했다. 정리한 내용은 pdf 로,
	$(SolutionDir)study\RedBlackTree\RedBlackTree.pdf 
	에 위치한다

	위 정리한 pdf 를 보며, 아래 함수인 L(LeftRotate), R(RightRotate), 
	insert, insertFixup, erase, eraesFixup 을 비교해가며 이해하자
*/



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



		// successorNode 의 색을 target 색으로 승계한다. target->left, target->right 는 그대로 위치하고
		// successorNode 가 색상을 승계하기에, 이부분은 문제가 되지 않는다. 
		// 문제가 되는 부분은 successorNode <- right 로 대치되는 부분이다. 
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
	// 루트는 항상 블랙
	if (node == _root)
	{
		node->isRed = false;
		return;
	}


	// 부모가 레드인 경우에만 반복
	while (node->parent && node->parent->isRed)
	{
		Node* parent = node->parent;
		Node* grandParent = parent->parent;


		// 부모가 증부모의 왼쪽 자식인 경우
		if (parent == grandParent->left)
		{
			Node* uncle = grandParent->right;

			// case1) 
			// 삼촌도 레드라면, 부모-삼촌이 모두 레드인 경우로, 삼촌-부모를 모두 블랙으로 전환,
			// 증부모를 레드로 폭발시키고, 증부모를 node로 할당하여 증부모 기준으로 다시 루프를 반복
			if (uncle && uncle->isRed)
			{
				parent->isRed = false;
				uncle->isRed = false;
				grandParent->isRed = true;

				node = grandParent;
			}
			// case2) 
			// 삼촌이 블랙이라면, 부모를 증부모 위치로, 증부모를 삼촌 위치로 로테이트
			// 증부모 위치의 부모는 블랙, 삼촌 위치의 증부모는 레드로 전환
			else
			{
				// 증부모와 부모를 R하기 앞서, 자신이 부모의 오른쪽 자신인 경우 
				// ( 증부모 - 부모 - 자신 ) 의 얼라인이 안된 경우, 얼라인을 맞춰줌
				// ※ 이경우 자신 이 기존 부모와 증부모의 부모가 됨
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
		// 부모가 증부모의 오른쪽 자식인 경우 ( 코드는 위와 대칭이다 )
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
	// ※ 여기서 블랙이라 함은, nullptr( RB트리에서 개념적으로 블랙인 종단 노드) 를 포함한다

	// ※ 아래의 sibling 이 종단노드(nullptr) 일 가능성은 없다. 
	//	 현 eraseFixup 함수가 호출되는 조건은, 
	//   erase 함수에서 erased 되는 노드가 종단노드(nullptr) 가 아니며, 색상이 블랙인 노드다.
	//   
	//	 해당 노드의 빈자리를 매꾸는 노드가 fixupNode 이기에, fixUpNode의 sibling 의 차수는 확정적으로 
	//   fixUpNode 보다 1 높다. 따라서 fixUpNode(아래의 'node') 가 종단노드(nullptr) 일 수는 있어도, 
	//   sibling 은 블랙노드일 수는 있어도 종단노드 일 가능성은 없기에, if(sibling == nullptr) 방어코드는 배제한다


	// 노드가 루트가 아니면서 블랙인 경우에 한해 반복
	while (node != _root && (node == nullptr || node->isRed == false))
	{
		// 노드가 parent 의 왼쪽 자식인 경우
		if (parent && node == parent->left)
		{
			Node* sibling = parent->right;

			// case1) 형제가 레드인 경우 -> 노드의 형제가 블랙이 되도록 조정한다. 이후 2), 3), 4) 에 맡긴다
			if (sibling->isRed)
			{
				sibling->isRed = false;
				parent->isRed = true;

				L(parent);

				// L 로 인해, 기존 parent->right->left 가 parent 의 right 로 인계됐으므로,
				// 기존 parent->right->left 가 현재의 sibling 이 됨
				sibling = parent->right;
			}

			// ※ 하단의 case2), case3), case4) 는 case1) 에 의해, sibling 이 블랙임을 전제로 한다

			// case2) 형제의 자식들이 모두 블랙인 경우
			if (
				(sibling->left == nullptr || sibling->left->isRed == false)
				&&
				(sibling->right == nullptr || sibling->right->isRed == false)
				)
			{
				sibling->isRed = true;

				node = parent;
				parent = parent->parent;
			}
			else
			{
				// case3) 형제의 (노드 기준으로 가까운) 자식이 레드면서, (노드 기준으러 먼) 자식은 블랙
				if (sibling->right == nullptr || sibling->right->isRed == false)
				{
					// ※ case2 가 false 이며 case3 은 true 이기에, sibling->left != nullptr
					sibling->left->isRed = false;

					sibling->isRed = true;
					R(sibling);
					sibling = parent->right;
				}

				// case4) 형제의 먼 자식이 레드 
				sibling->isRed = parent->isRed;
				parent->isRed = false;

				// ※ case2 가 false 이며 case3 을 거쳤기에, sibling->right != nullptr && sibling->right 는 레드
				sibling->right->isRed = false;

				L(parent);

				return;
			}
		}
		// 노드가 parent 의 오른쪽 자식인 경우 ( 위와 코드 구조는 대칭으로 모두 같다 )
		else if (parent)
		{
			Node* sibling = parent->left;

			// case1) 형제가 레드인 경우
			if (sibling->isRed)
			{
				sibling->isRed = false;
				parent->isRed = true;

				R(parent);

				sibling = parent->left;
			}

			// case2) 형제의 두 자식이 모두 블랙
			if (
				(sibling->left == nullptr || sibling->left->isRed == false) &&
				(sibling->right == nullptr || sibling->right->isRed == false)
				)
			{
				sibling->isRed = true;
				
				node = parent;
				parent = node->parent;
			}
			else
			{
				// case3) 형제의 가까운 자식이 레드면서, 먼 자식은 블랙
				if (sibling->left == nullptr || sibling->left->isRed == false)
				{
					sibling->right->isRed = false;
					

					sibling->isRed = true;
					L(sibling);
					sibling = parent->left;
				}

				// case4) 형제의 먼 자식이 레드
				sibling->isRed = parent->isRed;
				parent->isRed = false;


				sibling->left->isRed = false;
				
				R(parent);

				return;
			}
		}
	}

	if (node)
	{
		node->isRed = false;
	}
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