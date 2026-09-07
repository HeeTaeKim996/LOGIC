

/*
_EXPORT_STD template <class _RanIt>
_CONSTEXPR20 void sort(const _RanIt _First, const _RanIt _Last) { // order [_First, _Last)
    _STD sort(_First, _Last, less<>{});
}

==>> Algorithm 의 sort 함수다. 보다시피 인자로 특정 클래스를 받는 게 아니라,
     _First, _Last 로 들어온 인자가 동일한 클래스인지만 검수하고 있다.

     Interater 클래스들은 동일 인터페이스를 상속받지 않는다. 
     단지 아래처럼 규격에 필요한 함수들을 구현만 해놓으면 된다.
     
     규격 자체를 정의하는 인터페이스로 구현이 되지 않았기에, 인터페이스를 만들었어도,
     특정 함수가 요구하는 함수 구현이 안돼있을 수 있다. 이렇다면 컴파일 시 함수 구현이 안된 걸로
     오류가 남
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
	{
	}

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


	Vector_iterator operator + (sizeType n) const { return Vector_iterator(_ptr + n); }
	Vector_iterator operator - (sizeType n) const { return Vector_iterator(_ptr - n); }

	template<typename U>
	bool operator == (const Vector_iterator<U>& other) const { return _ptr == other._ptr; }
	template<typename U>
	bool operator != (const Vector_iterator<U>& other) const { return _ptr != other._ptr; }

	T* get() const { return _ptr; }

private:
	T* _ptr;
};