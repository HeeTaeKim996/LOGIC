#pragma once
#include <type_traits>
#include "IM_integer_interfaces.h"


/*
	■ if constexpr
	if consexpr 을 쓰는 이유는, static_assert 는 실행 시점이 아닌
	컴파일 시점에 실행되기에, BigInt를 인자로 쓴다면 컴파일 시점에
	static_assert 가 실행되어 에러가 난다. if constexpr 는 실행시점이 아닌
	컴파일 시점에 실행되기에, BigInt를 쓸 때 static_assert 가 발동하지 않음

	■ is_same_as_v<T, ClassName> : T 클래스가 ClassName 과 같은 지 체크
	  is_base_of_v<ClassName, T> : T 클래스가 ClassName 을 상속했는지 체크
*/



#define CHECK_INT(T) do{												\
		if constexpr(std::is_base_of_v<IInteger, T> == false)				\
		{																\
			static_assert(std::is_integral_v<T>, "T must be Integer");	\
		}																\
	} while(0)

#define CHECK_SIGN_INT(T) do{											\
		if constexpr(std::is_base_of_v<IInteger, T> == false)				\
		{																\
			static_assert(std::is_integral_v<T>, "T must be Integer");	\
			static_assert(std::is_signed_v<T>, "T must be Signed");		\
		}																\
	} while(0)