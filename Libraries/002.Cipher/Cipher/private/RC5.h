#pragma once

#include "SizeType.h"
#include "IBlockCryptor.h"

/*
	□ 워드, 라운드, 키길이의 가변성
	RC5 알고리즘 자체는 간단하고, 차별점도 없다.
	차별점이라고는 워드, 라운드, 키길이를 변수로 두어 관리한다는데,
	사실상 워드는 32비트 고정이라 보면 되고(하드웨어적으로 변수로 처리하기 어려움)
	라운드, 키길이가 가변이라 이해하면 된다. 
	구현은 워드 32비트 / 라운드 12회 / 키길이 128비트 (표준) 를 전제로 구현함

	※ P,C 길이는 64비트
	
	□ 기본 연산
	로테이션, 더하기, ^ 만으로 구현
 
	□ 키 생성
	로테이션, 더하기 를 기존 서브키 + 키 + 사전 정의 난수 P, Q 와 반복하여, 
	2 * (r + 1) 개의 서브키 생성. (서브키수 * 3 만큼 섞기를 반복)
	※ 코드 보면 쉽게 파악 가능

	□ 암호화
	64비트 블록을 두개의 블록 A, B 로 양분한 후,
	xor - 시프팅(LEFT) - 더하기 를 A - B - A - B 순서대로 반복
	A 연산 때에는 B를 연산자로 사용, B 연산 때는 A를 연산자로 사용

	□ 복호화
	암호화와 구조가 대쳉되게 역순으로 처리. 
	(연산도 역순, B - A 순서도 역으로 B - A - B - A..)

	
*/



class RC5 : public IBlockCryptor
{
private:

	static const uint32 P = 0xB7E15163;
	static const uint32 Q = 0x9E3779B9;

public:
	RC5(const uint8* key);
	virtual void encrypt(uint8* p) override;
	virtual void decrypt(uint8* c) override;
	virtual int getBlockSize() { return 8; }

private:
	void makeKeys(const uint8* key);


private:
	static uint32 rotLeft(uint32 x, uint32 n);
	static uint32 rotRight(uint32 x, uint32 n);

private:
	uint32 S[26]; // S 의 워드수 = 2(r + 1) = 26


public:
	static void test();
};