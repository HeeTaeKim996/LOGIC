#pragma once

#include "Std.h"
#include "Mem.h"
#include "SizeType.h"

// 참조 원본 깃허브
// - https://github.com/ross1573/mini_vector




template<typename T>	class Vector_iterator;
template<typename T>	class Vector_reverse_iterator;






template<typename T>
class Vector
{
public:
	using iterator = Vector_iterator<T>;
	using const_iterator = Vector_iterator<const T>;
	using reverse_iterator = Vector_reverse_iterator<T>;
	using const_reverse_iterator = Vector_reverse_iterator<const T>;

private:
	T* _arr;
	sizeType _size;
	sizeType _capacity;
	


public:
	Vector();
	explicit Vector(const sizeType size); // ※ explicit : 생성자에서의 암시적 형변환을 방지. (EX. explicit 이 없을시, Vector vec(10); 은 Vector vec = 10; 로 대체될 수 있음
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector(const sizeType size, const T& value);

	template<typename FromIt>
	Vector(FromIt begin, FromIt end);

	~Vector();

public:
	sizeType size() const noexcept { return _size; }
	sizeType capacity() const noexcept { return _capacity; }
	bool empty() const noexcept { return _size == 0; }

	T* data()				{ return _arr; }
	const T* data() const	{ return _arr; }

	iterator begin()				{ return iterator(_arr); }
	const_iterator begin() const	{ return const_iterator(_arr); }

	iterator end()				{ return iterator(_arr + _size); }
	const_iterator end() const	{ return const_iterator(_arr + _size); }

	reverse_iterator rbegin()				{ return reverse_iterator(_arr + _size - 1); }
	const_reverse_iterator rbegin() const	{ return const_reverse_iterator(_arr + _size - 1); }

	reverse_iterator rend()					{ return reverse_iterator(_arr - 1); }
	const_reverse_iterator rend() const		{ return const_reverse_iterator(_arr - 1); }

	T& front()				{ return *_arr; }
	const T& front() const	{ return *_arr; }

	T& back()				{ return *(_arr + _size - 1); }
	const T& back() const	{ return *(_arr + _size - 1); }

	T& at(sizeType index)				{ return _arr[index]; }
	const T& at(sizeType index) const	{ return _arr[index]; }

	T& operator[](sizeType index)				{ return _arr[index]; }
	const T& operator[](sizeType index) const { return _arr[index]; }



	Vector& operator = (const Vector& other);
	Vector& operator = (Vector&& other);



public:
	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();
	template <typename... Args>
	T& emplace_back(Args&&... args);

	void insert(const_iterator itPos, const T& value);
	void insert(const_iterator itPos, T* begin, T* end);
	template <typename... Args>
	T& emplace(const_iterator itPos, Args&&... args);

	void erase(const T* pos);
	void erase(const T* begin, const T* end);

	void reserve(sizeType capacity);

	void resize(sizeType size);
	void resize(sizeType size, const T& value);

	void swap(Vector& other);
	
	void clear();

	void shrink_to_fit();
};





template<typename T>
inline Vector<T>::Vector() : Vector(0)
{
}

template<typename T>
inline Vector<T>::Vector(const sizeType size)
	: _arr(m::allocate<T>(size)), _size(size), _capacity(size)
{
}

template<typename T>
inline Vector<T>::Vector(const Vector& other)
	: _size(other._size), _capacity(other._capacity), _arr(m::allocate<T>(_capacity))
{
	//  원본에 이해가 안되는 부분이 2개 있었고, 수정했다
	//	1) allocate(_capacity) 를 할당하지 않고 공란을 뒀다. _capacity 를 할당했다
	//	2) copyRange 하면 안된다. 위에 보면 allocate 로 공간 할당만 한 상태이다. 이때 해야할 건 copyConstructRange 이다

	m::copyConstructRange<T>(_arr, other._arr, other._arr + other._size);
}

template<typename T>
inline Vector<T>::Vector(Vector&& other)
	: _arr(other._arr), _size(other._size), _capacity(other._capacity)
{
	other._size = 0;
	other._capacity = 0;
	other._arr = nullptr;
}

template<typename T>
inline Vector<T>::Vector(sizeType size, const T& value)
	: _arr(m::allocate<T>(size)), _size(size), _capacity(size)
{
	m::constructRange<T>(_arr, _arr + _size, value);
}



template<typename T>
template<typename FromIt>
inline Vector<T>::Vector(FromIt begin, FromIt end)
	: _arr(nullptr), _size(0), _capacity(0)
{
	

	for(; begin != end; ++begin)
	{
		push_back(*begin);
	}
}







template<typename T>
inline Vector<T>::~Vector()
{
	m::destructRange<T>(_arr, _arr + _size);
	m::deallocate<T>(_arr);
	_arr = nullptr;
}






template<typename T>
inline  Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	T* dst =  m::allocate<T>(other._capacity);
	m::copyConstructRange<T>(dst, other._arr, other._arr + other._size);

	m::destructRange<T>(_arr, _arr + _size);
	m::deallocate<T>(_arr);

	_arr = dst;
	_size		= other._size;
	_capacity	= other._capacity;
	

	return *this;
}

template<typename T>
inline  Vector<T>& Vector<T>::operator=(Vector<T>&& other)
{
	m::destructRange<T>(_arr, _arr + _size);
	m::deallocate<T>(_arr);
	
	_size		= other._size;
	_capacity	= other._capacity;
	_arr		= other._arr;

	other._size = 0;
	other._capacity = 0;
	other._arr = nullptr;
	
	return *this;
}





































template<typename T>
inline void Vector<T>::push_back(const T& value)
{
	if (_size < _capacity)
	{
		m::construct<T>(_arr + _size, value); // T 가 생성자로 const T& 를 받는다는 전제
	}
	else
	{
		sizeType newCap = _capacity == 0 ? 1 : _capacity * 2;
		T* newArr = m::allocate<T>(newCap);

		T* dst = newArr + _size;
		m::construct<T>(dst, value);
		dst -= 1;

		m::moveConstructBackward<T>(dst, _arr + _size - 1, _arr - 1); // 여기서 (2) 는 rbegin 역할, (3) 은 rend

		m::destructRange<T>(_arr, _arr + _size);
		m::deallocate<T>(_arr);

		_arr = newArr;
		_capacity = newCap;
	}

	_size++;
}


template<typename T>
inline void Vector<T>::push_back(T&& value)
{
	if (_size < _capacity)
	{
		m::construct<T>(_arr + _size++, static_cast<T&&>(value)); // T 가 생성자로 rvalue를 받는다는 전제
	}
	else
	{
		sizeType newCap = _capacity == 0 ? 1 : _capacity * 2;
		T* newArr = m::allocate<T>(newCap);

		T* dst = newArr + _size;
		m::construct<T>(dst, static_cast<T&&>(value));
		dst -= 1;
		m::moveConstructBackward<T>(dst, _arr + _size - 1, _arr - 1);

		m::destructRange<T>(_arr, _arr + _size);
		m::deallocate<T>(_arr);

		_arr = newArr;
		_capacity = newCap;
		_size++;
	}
}



template<typename T>
inline void Vector<T>::pop_back()
{
	// 방어코드는 작성하지 않는다. stl vector 도 성능을 위해 사용자에게 책임을 맡긴다 함
	// if (_size == 0) return;

	m::destruct<T>(_arr + --_size);
}





template<typename T>
template<typename... Args>
inline T& Vector<T>::emplace_back(Args&&... args)
{
	if (_size < _capacity)
	{
		m::construct<T>(_arr + _size, Std::forward<Args>(args)...);
	}
	else
	{
		sizeType newCap = _capacity == 0 ? 1 : _capacity * 2;
		T* newArr = m::allocate<T>(newCap);

		T* dst = newArr + _size;
		m::construct<T>(dst, Std::forward<Args>(args)...);
		dst -= 1;
		m::moveConstructBackward<T>(dst, _arr + _size - 1, _arr - 1);

		m::destructRange<T>(_arr, _arr + _size);
		m::deallocate<T>(_arr);

		_arr = newArr;
		_capacity = newCap;
	}
	++_size;

	return back(); // emplace_back 은 T의 생성자 인자를 받아 T를 생성하고 삽입한 후, T를 리턴
}


















template<typename T>
inline void Vector<T>::insert(const_iterator itPos, const T& value)
{
	if (itPos == end())
	{
		push_back(value);
	}
	else
	{
		T* begin = const_cast<T*>(&value);
		T* end = begin + 1;

		insert(itPos, begin, end);
	}
}

template<typename T>
inline void Vector<T>::insert(const_iterator itPos, T* srcBegin, T* srcEnd)
{
	sizeType insertingSize = sizeType(srcEnd - srcBegin);

	sizeType newSize = _size + insertingSize;

	T* pos = const_cast<T*>(itPos.get());

	if (newSize > _capacity)
	{
		sizeType newCap = _capacity * 2 > newSize ? _capacity * 2 : newSize;
		T* newArr = m::allocate<T>(newCap);

		T* dst = newArr;


		m::moveConstructRange<T>(dst, _arr, pos);
		dst += sizeType(pos - _arr);

		m::copyConstructRange<T>(dst, srcBegin, srcEnd);
		dst += insertingSize;

		m::moveConstructRange<T>(dst, pos, _arr + _size);

		m::destructRange<T>(_arr, _arr + _size);
		m::deallocate<T>(_arr);

		_arr = newArr;
		_capacity = newCap;
	}
	else
	{
		// 삽입으로 인해 기존 end 밖으로 밀려나는 애들을, T 생성과 함께 복사(?)
		m::moveConstructBackward<T>(_arr + newSize - 1,
			_arr + _size - 1, _arr + _size - insertingSize - 1);

		// 삽입으로 인해 뒤로 밀려나지만, end 밖으로 밀려나지는 않는 애들을 복사
		m::moveBackward<T>(_arr + _size - 1, _arr + _size - insertingSize - 1, pos - 1);

		// 새로 삽입되는 애들을 복사
		m::copyRange<T>(pos, srcBegin, srcEnd);
	}

	_size = newSize;
}

template<typename T>
template<typename ...Args>
inline T& Vector<T>::emplace(const_iterator itPos, Args && ...args)
{
	if (itPos == end())
	{
		return emplace_back(Std::forward<Args>(args)...);
	}
	else
	{
		T* pos = const_cast<T*>(itPos.get());

		if (_size < _capacity)
		{
			m::construct<T>(_arr + _size, static_cast<T&&>(*(_arr + _size - 1)));
			m::moveBackward<T>(_arr + _size - 1, _arr + _size - 2, pos - 1);
			
			m::destruct<T>(pos);
			m::construct<T>(pos, Std::forward<Args>(args)...);
		}
		else
		{
			sizeType newCap = _capacity == 0 ? 1 : _capacity * 2;
			T* newArr = m::allocate<T>(newCap);
			T* dst = newArr + _size;
			T* newPos = newArr + (pos - _arr);

			m::moveConstructBackward<T>(dst, _arr + _size - 1, pos - 1);
			dst -= sizeType(_arr + _size - pos);

			m::construct<T>(newPos, Std::forward<Args>(args)...);
			dst -= 1;

			m::moveConstructBackward<T>(dst, pos - 1, _arr - 1);

			m::destructRange<T>(_arr, _arr + _size);
			m::deallocate<T>(_arr);

			_arr = newArr;
			_capacity = newCap;
			pos = newPos;
		}
		++_size;

		return *pos;
	}
}















template<typename T>
inline void Vector<T>::erase(const T* itPos)
{
	T* pos = const_cast<T*>(itPos);


	m::moveRange<T>(pos, pos + 1, _arr + _size);
	m::destruct<T>(_arr + _size - 1);

	_size--;
}

template<typename T>
inline void Vector<T>::erase(const T* itBegin, const T* itEnd)
{
	T* begin = const_cast<T*>(itBegin);
	T* end = const_cast<T*>(itEnd);

	m::destructRange<T>(begin, end);
	m::moveRange<T>(begin, end, _arr + _size);
	
	_size -= (sizeType)(itEnd - itBegin);
}

template<typename T>
inline void Vector<T>::reserve(sizeType capacity)
{
	if (_capacity >= capacity) return;

	T* dst = m::allocate<T>(capacity);

	m::moveConstructRange<T>(dst, _arr, _arr + _size);

	m::destructRange<T>(_arr, _arr + _size);
	m::deallocate<T>(_arr);

	_arr = dst;
	_capacity = capacity;

}

template<typename T>
inline void Vector<T>::resize(sizeType newSize)
{
	if (_size == newSize) return;

	if (newSize > _size)
	{
		if (newSize > _capacity)
		{
			sizeType newCap = _capacity * 2 > newSize ? _capacity * 2 : newSize;
			T* dst = m::allocate<T>(newCap);

			m::moveConstructRange<T>(dst, _arr, _arr + _size);
			m::constructRange<T>(dst + _size, dst + newSize); // (3) 항인 Args... 를 생략하면, 기본 생성자로 생성

			m::destructRange<T>(_arr, _arr + _size);
			m::deallocate<T>(_arr);

			_arr = dst;
			_capacity = newCap;
		}
		else
		{
			m::constructRange<T>(_arr + _size, _arr + newSize);
		}
	}
	else
	{
		m::destructRange<T>(_arr + newSize, _arr + _size);
	}

	_size = newSize;
}

template<typename T>
inline void Vector<T>::resize(sizeType newSize, const T& value)
{
	if (newSize == _size) return;

	if (newSize > _size)
	{
		if (newSize > _capacity)
		{
			sizeType newCap = _capacity * 2 > newSize ? _capacity * 2 : newSize;
			T* dst = m::allocate<T>(newCap);

			m::moveConstructRange<T>(dst, _arr, _arr + _size);
			m::constructRange<T>(dst + _size, dst + newSize, value);

			m::destructRange<T>(_arr, _arr + _size);
			m::deallocate<T>(_arr);

			_arr = dst;
			_capacity = newCap;
		}
		else
		{
			m::constructRange<T>(_arr + _size, _arr + newSize, value);
		}
	}
	else
	{
		m::destructRange<T>(_arr + newSize, _arr + _size);
	}

	_size = newSize;
}








template<typename T>
inline void Vector<T>::swap(Vector& other)
{
	sizeType tempSize = _size;
	sizeType tempCapacity = _capacity;
	T* tempArr = _arr;

	_size = other._size;
	_capacity = other._capacity;
	_arr = other._arr;

	other._size = tempSize;
	other._capacity = tempCapacity;
	other._arr = tempArr;
}



template<typename T>
inline void Vector<T>::clear()
{
	m::destructRange<T>(_arr, _arr + _size);
	_size = 0;
}

template<typename T>
inline void Vector<T>::shrink_to_fit()
{
	if (_size == 0)
	{
		_capacity = 0;
		m::deallocate<T>(_arr);
		_arr = nullptr;
		return;
	}

	sizeType newCap = _size;
	if (newCap == _capacity) return;

	T* dst = m::allocate<T>(newCap);
	m::moveConstructRange<T>(dst, _arr, _arr + _size);

	m::destructRange<T>(_arr, _arr + _size);
	m::deallocate<T>(_arr);

	_arr = dst;
	_capacity = newCap;
}





















/*----------------------------------------------
		 생성자 인자로부터 타입 추론
-----------------------------------------------*/
// DeductionGuide ( 타입 추론. FromIt 이터레이터가 public 으로 using value_type = T; 를 지정하면, 타입 추론 가능 ) 
template<typename FromIt>
Vector(FromIt, FromIt) -> Vector<typename FromIt::value_type>;






























/*----------------------------------------
				 Iterator
----------------------------------------*/

/*
	using iterator               = _Vector_iterator<_Scary_val>;
	using const_iterator         = _Vector_const_iterator<_Scary_val>;
	using reverse_iterator       = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

	이렇게 stl vector iterator 4개 구현
*/

template<typename T> // 여기서 T 는 원형 타입(EX. int) 과 const 타입 (EX. const int) 를 모두 받을 수 있다
class Vector_iterator
{
private:
	// typename 우측에 T등이 없고, Vector_iterator 우측에 <T> 등이 없다.
	// 이건 모든 Vector_iterator<무언가> 는 다른 모든 Vector_iterator<무언가> 의 firend 이다. 임을 선언
	template<typename>
	friend class Vector_iterator;
public:
	using value_type = T;


public:
	Vector_iterator(T* ptr) : _ptr(ptr) {}


	// 여기서 U 는 src(원형 타입), 여기서 T는 dst(const 타입)
	// std::is_convertible_v<U*, T*> 는 U 가 T로 암시 형변환이 가능한지를 검수. 
	// 예를 들어 U 는 int, T 는 const int 라면, int -> const int 는 암시 형변환이 가능하므로, 형변환 가능
	// /		U 는 const int, T 는 int 라면, const int -> int 는 암시 형변환이 불가능하므로, 형변환 불가능
	// 이를 통해 iterator -> const_iterator 의 형변환이 가능해진다
	// 따라서 아래의 U 는 원형 타입, T 는 const 타입 이라 보면 된다
	template<
		typename U,
		typename = std::enable_if_t<
		std::is_convertible_v<U*, T*>	
		>
	>
	Vector_iterator(const Vector_iterator<U>& other)
		: _ptr(other._ptr) // 여기(const 타입)서 other._ptr(원형 타입의 _ptr)에 접근할 수 있는 이유는, 위에 선언한 firend class Vector_iterator 부분 때문.
	{}

public:
	T& operator * () const { return *_ptr; }
	T* operator ->() const { return _ptr; }
	Vector_iterator& operator ++ () { ++_ptr; return *this; } // 전위 연산자
	inline Vector_iterator operator ++(int) // 후위 연산자
	{
		Vector_iterator temp = *this;
		++_ptr; 
		return temp;
	}
	
	Vector_iterator& operator -- () { --_ptr; return *this; }
	inline Vector_iterator operator --(int)
	{
		Vector_iterator temp = *this;
		--_ptr;
		return temp;
	}


	Vector_iterator operator + (sizeType n) const { return Vector_iterator(_ptr + n);}
	Vector_iterator operator - (sizeType n) const { return Vector_iterator(_ptr - n); }

	template<typename U>
	bool operator == (const Vector_iterator<U>& other) const { return _ptr == other._ptr; }
	template<typename U>
	bool operator != (const Vector_iterator<U>& other) const{ return _ptr != other._ptr; }

	T* get() const { return _ptr; }

private:
	T* _ptr;
};






template<typename T>
class Vector_reverse_iterator
{
private:
	template<typename>
	friend class Vector_reverse_iterator;

public:
	Vector_reverse_iterator(T* ptr) : _ptr(ptr) {}

	template<
		typename U,
		typename = std::enable_if_t<
		std::is_convertible_v<U*, T*>
		>
	>
	Vector_reverse_iterator(const Vector_reverse_iterator<U>& other) : _ptr(other._ptr) {}

public:
	T& operator * () const { return *_ptr; }
	T* operator ->() const { return _ptr; }
	Vector_reverse_iterator& operator ++ () { --_ptr; return *this; }
	inline Vector_reverse_iterator operator ++(int)
	{
		Vector_reverse_iterator temp = *this;
		--_ptr;
		return temp;
	}

	Vector_reverse_iterator& operator -- () { ++_ptr; return *this; }
	inline Vector_reverse_iterator operator --(int)
	{
		Vector_reverse_iterator temp = *this;
		++_ptr;
		return temp;
	}

	Vector_reverse_iterator operator + (sizeType n) const { return Vector_reverse_iterator(_ptr - n); }
	Vector_reverse_iterator operator - (sizeType n) const { return Vector_reverse_iterator(_ptr + n); }
	
	template<typename U>
	bool operator == (const Vector_reverse_iterator<U>& other) const { return _ptr == other._ptr; }
	template<typename U>
	bool operator != (const Vector_reverse_iterator<U>& other) const { return _ptr != other._ptr; }

	T* get() const { return _ptr; }

private:
	T* _ptr;
};