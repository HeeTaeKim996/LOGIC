#pragma once
#include "SizeType.h"



// RFC 4269 (SEED) :  https://www.rfc-editor.org/info/rfc4269/

/*
	■ 정리
	DES 보다도 오히려 더 fiestel 구조에 충실하다. 
	DES 에는 IP, FP 등 라운딩 밖에서 하는 permutate 라도 있었지만, SEED 는 이것도 없다.
	fiestel 의 기본 골격에 충실하다 
	( 확장 DES 도 없다 한다. IP-FP 가 암호적으로 아무 의미가 없단 게 밝혀졌기에 )

	F 함수도 너무 단순한데 코드만 길 뿐이다. 더하기, XOR, 시프팅 3개만으로 구성된다.
	마찬가지로 키확장 단계도 위 3연산자를 기반으로 연산되는 더러운 코드만 있다

	정리하면, 코드를 볼 필요 없다. fiestel 구조에 너무 충실한 알고리즘이다
*/


#define MASK_8 0xFF
#define MASK_32 0xFF'FF'FF'FF

class SEED
{
private:
	static const uint8 S0[256];
	static const uint8 S1[256];


public:
	SEED(const uint8* key);
	void encrypt(uint8* p) { crypt(p, false); }
	void decrypt(uint8* c) { crypt(c, true); }
	

private:
	void crypt(uint8* m, bool isDec);
	uint32 G(uint32 x);
	uint64 F(const uint32 subKey[2], uint64 right);

	void expandKeys(const uint8* key);

private:
	uint32 subKeys[16][2];


public:
	static void test();
};