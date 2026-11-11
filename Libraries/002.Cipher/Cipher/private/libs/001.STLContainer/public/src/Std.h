#pragma once

/*
	■ std::Forward 를 구현했다

	□
	namespace Std 내부의 내용은 설명을 들어도 이해를 1도 못했다.
	그냥 저부분은 단순하게 이해하자. 
	1) remove_reference 부분은 T&, T&& 로 받은 것을, T로 만든다.
	2) forward 는 T&, T&& 로 받은 걸 우선 인자 받을 때 remove_refernece 를 사용하여 T로 만든다. 
	   이후 static_cast 로 명시 형변환하여, T&& 또는 T& 로 만든다
	   (& - && 충돌 -> &. && - && 충돌 -> &&)

	※ std::move 는 강제로 && 변환, 
	  std::forward 는 &로 인자를 받을시 & 로 변환, && 로 인자를 받을시 && 로 변환
	   

	□ 
	아래 namespace StdExplain 을 보며, std::forward 와 std::move 의 역할을 이해하는 게 내 정신건강에 좋을 듯 싶다.
	함수 인자로 && 를 받아도, 함수 내부에서 해당 인자를 그냥 사용하면, 함수 인자가 내부에 있는 순간, 해당 인자는 이동(rvalue) 이 아닌 복사(lvalue) 가 된다.
	따라서 복사 비용을 아끼려면, 함수 내부에서도 인자를 그냥 사용하지 말고, std::forward 로 명시로 rValue로 만든 다음에 사용해야 한다
*/




namespace Std
{
	// &, && 가 없는 일반 타입으로 T type 을 들고 있음
	template <typename T>
	struct remove_reference
	{
		typedef T type;
	};

	// lvalue reference 를 제거
	template<typename T>
	struct remove_reference<T&> : remove_reference<T> {};

	// rvalue reference 를 제거
	template<typename T>
	struct remove_reference<T&&> : remove_reference<T> {};

	// lvalue 를 받아 원래 T 의 참조 성질로 변환 ( return 에 T& 가 아닌 T&& 에 주의. & + && (충돌) -> &
	template<typename T>
	constexpr T&& forward(typename remove_reference<T>::type& args) noexcept
	{
		return static_cast<T&&>(args);
	}

	// rvalue를 받아 원래 T의 참조 성질로 변환
	template<typename T>
	constexpr T&& forward(typename remove_reference<T>::type&& args) noexcept
	{
		return static_cast<T&&>(args);
	}

	// ※ constexpr	: 컴파일 시점에 수행될 수 있는 함수임을 명시. 
	//		-> 위 forward 의 경우 인자값이 컴파일 시점에 고정돼있기 때문에 사용 가능
	// 
	// ※ noexcept	: 예외 처리와 관련된 추가 코드를 생성하지 않도록 지시하여, 코드 크기를 줄이고 실행 성능 최적화
	//
	//  => 둘은 쌍으로 자주 사용되더라. 아무튼 함수가 컴파일 시점에 실행 가능한, 함수의 인자가 컴파일 시점에 확정되는 함수일 경우, constexpr + noexcept 를 사용하자




	// 이건 제미나이 도움 받아 만든 거다. 타입을 추론해서 리턴하는 게 아니라, 
	// 타입은 일단 아무 인자나 받고, 이걸 명시적으로 remove_reference<T>&& 로 강제로 rValue로 형변환하는 듯 싶다
	template<typename T>
	constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept
	{
		return static_cast<typename remove_reference<T>::type&&>(arg);
	}
}











#include <stdio.h>
namespace StdExplain
{
	struct TempStruct
	{
		TempStruct(int* inPtr): ptr(inPtr) {}

		// 복사 생성자. lValue 로 복사될 때 호출됨
		TempStruct(const TempStruct& other)
		{
			printf("복사 생성\n");
			
			ptr = new int(*other.ptr);
		}
		
		// 이동 생성자. rValue 로 이동되어 데이터를 받을 때 호출됨
		TempStruct(TempStruct&& other) noexcept
		{
			printf("이동 생성\n");

			ptr = other.ptr;
			other.ptr = nullptr;
		}

		int* ptr;
	};


	template<typename T>
	inline void normalValFunc(T intVal) {}

	template<typename T>
	inline void rightValFunc(T&& rightInt)
	{
		// 인자를 rightValue 로 받았지만, 함수 내부에 rightInt 라는 변수명으로 사용하는 순간, 
		// lValue 가 되어, rightValue 의 이동이 아니라, lValue의 복사가 이뤄짐 ( 이 함수에서 rightInt 가 이동이 아닌 복사 가 됨 )
		normalValFunc(rightInt);

		// 반면 아래처럼 함수 내부에 forward로 래핑하여 전달시, 원래의 rValue 대로 정의되어, 함수 내부에서도 lValue의 복사가 아닌 rValue의 이동이 이뤄짐
		normalValFunc(Std::forward<T>(rightInt));
	}


	template<typename T>
	inline void leftValFunc(T& leftInt)
	{
		// leftVal 로 받음
		normalValFunc(leftInt);


		printf("%d \n", *leftInt.ptr);

		// leftVal 로 받은 걸 forward로 rightVal 로 만들어 전달
		normalValFunc(Std::forward<T>(leftInt));


		// 아래 함수 호출시 nullptr 참조로 크래시
		//printf("%d \n", *leftInt.ptr); 
	}






	inline void test()
	{
		TempStruct lValue(new int(1));

		printf("RightValFunc 호출 : \n");
		rightValFunc(TempStruct(new int(2)));

		printf("\nLeftValFunc 호출 : \n");
		leftValFunc(lValue);
		// 여기서 두번째 forward일시 &(복사) 가 아닌 &&(이동) 이 되는 부분은 이해 못했다


		printf("\nRighttValFunc 에 leftValFunc을 넣어 호출: \n");
		TempStruct lValue2(new int(3));
		rightValFunc(lValue2);


		printf("\nRighttValFunc 에 leftValFunc을 std::move로 넣어 호출: \n");
		TempStruct lValue3(new int(3));
		rightValFunc(Std::move(lValue3));

	}




}


