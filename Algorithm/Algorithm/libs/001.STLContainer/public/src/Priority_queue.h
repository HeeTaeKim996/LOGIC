

#include "SizeType.h"
#include "Mem.h"
#include "Std.h"

#include "Vector.h"

#include <functional>


template<typename T>
struct Less
{
	constexpr bool operator()(const T& a, const T& b) const
	{
		return a < b;
	}
};




// Container, LessT 는 인자로 넘기지 않는 한, 기본값으로 생성됨. 하지만 인자로 less 를 넘기는 순간, 
// less 는 위 정의한 Less 의 상속자가 아니기에, 사용자가 직접 <...> 에 Contianer 와 LessT 를 정의해야 함
template<typename T, typename Container = Vector<T>,  typename LessT = Less<T>>
class Priority_queue
{
public:
	Priority_queue();
	
	// 유저가 less 를 주입할 때, 주입하는 less 는 위 형식만 지키면 되고, Compare<T> 를 상속받지 않아도 된다
	explicit Priority_queue(const LessT& less, const Vector<T>& vec = Vector<T>());
	explicit Priority_queue(const LessT& less, Vector<T>&& vec);


	// 소멸자는 굳이 구현하지 않는다. 어차피 멤버 변수로 Vector<T> 를 들고 있기에, 
	// 이클래스 소멸시 자동으로 Vector<T> 의 ~Vector가 호출된다

public:
	const T& top() const { return _vec.front(); }
	bool empty() const { return _vec.empty(); }
	sizeType size() const noexcept { return _vec.size(); }


public:
	void upHeap(sizeType child);
	void downHeap(sizeType parent);
	void sortVec();

public:
	void push(const T& value);
	void push(T&& value);

	template<typename... Args>
	void emplace(Args&&... args);

	void pop();

private:
	Vector<T> _vec;
	LessT _less;
};





template<typename T, typename Container, typename LessT>
inline Priority_queue<T, Container, LessT>::Priority_queue()
	: _vec(0)
{
}

template<typename T, typename Container, typename LessT>
inline Priority_queue<T, Container, LessT>::Priority_queue(const LessT& less, const Vector<T>& vec)
	: _less(less), _vec(vec)
{
	sortVec();
}

template<typename T, typename Container,  typename LessT>
inline Priority_queue<T, Container, LessT>::Priority_queue(const LessT& less, Vector<T>&& vec)
	: _less(less), _vec(Std::move(vec))
{
	sortVec();
}










template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::upHeap(sizeType child)
{
	T mover = _vec[child];

	sizeType parent;
	for (; child > 0; child = parent)
	{
		parent = (child - 1) / 2;

		if (_less(_vec[parent], mover) == false) break;

		_vec[child] = _vec[parent];
	}

	_vec[child] = mover;
}

template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::downHeap(sizeType parent)
{
	T mover = _vec[parent];

	sizeType lastChild = _vec.size() - 1;
	sizeType child;
	for (; parent * 2 + 1 <= lastChild; parent = child)
	{
		sizeType cl = parent * 2 + 1;
		sizeType cr = cl + 1;

		child = (cr <= lastChild && _less(_vec[cl], _vec[cr])) ? cr : cl;

		if (_less(mover, _vec[child]) == false) break;

		_vec[parent] = _vec[child];
	}

	_vec[parent] = mover;
}



template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::sortVec()
{
	if (_vec.size() > 1)
	{
		sizeType parent = _vec.size() / 2 - 1;
		// lastChild 는  n - 1. lastChild의 부모는 (n - 1 - 1 ) / 2 = n / 2 - 1
		while (true)
		{
			downHeap(parent);

			if (parent-- == 0) break;
		}
	}
}









template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::push(const T& value)
{
	_vec.push_back(value);

	upHeap(_vec.size() - 1);
}

template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::push(T&& value)
{
	_vec.push_back(Std::move(value));

	upHeap(_vec.size() - 1);
}

template<typename T, typename Container, typename LessT>
template<typename ...Args>
inline void Priority_queue<T, Container, LessT>::emplace(Args&& ...args)
{
	_vec.emplace_back(std::forward<Args>(args)...);

	upHeap(_vec.size() - 1);
}










template<typename T, typename Container, typename LessT>
inline void Priority_queue<T, Container, LessT>::pop()
{
	_vec[0] = Std::move(_vec[_vec.size() - 1]);
	_vec.pop_back();

	if (_vec.empty() == false)
	{
		downHeap(0);
	}
}