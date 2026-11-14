#pragma once
#include "IM_BigInt.h"


// if consexpr 을 쓰는 이유는, static_assert 는 실행 시점이 아닌 
// 컴파일 시점에 실행되기에, BigInt를 인자로 쓴다면 컴파일 시점에 
// static_assert 가 실행되어 에러가 난다. if constexpr 는 실행시점이 아닌
// 컴파일 시점에 실행되기에, BigInt를 쓸 때 static_assert 가 발동하지 않음
#define CHECK_INT(T) do{												\
		if constexpr(is_same_v<T, BigInt> == false)						\
		{																\
			static_assert(std::is_integral_v<T>, "T must be Integer");	\
		}																\
	} while(0)

#define CHECK_SIGN_INT(T) do{											\
		if constexpr(is_same_v<T, BigInt> == false)						\
		{																\
			static_assert(std::is_integral_v<T>, "T must be Integer");	\
			static_assert(std::is_signed_v<T>, "T must be Signed");		\
		}																\
	} while(0)