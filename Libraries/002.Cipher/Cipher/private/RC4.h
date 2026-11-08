#pragma once
#include "SizeType.h"

/*
	코드를 보면 알다시피 너무 간단하다. 초기화 때 sBox를 섞고,
	암복호화는 동일 로직으로 바이트 암복호화 때마다 sBox를 섞고 sBox에서 꺼내 xor

	구조적 문제가 많아 현대에서는 폐기됐다.

	1. 송-수신간 키 동기화가 까다롭다.
	송신은 2번했는데, 1개가 유실되고, 수신은 1번 받는다면, 이후 sBox 스와핑이 어긋나
	키 동기화가 깨진다
	(TCP 위에서 제한적으로 사용됐던 이유)

	2. WEP 
	WEP 는 이 동기화 문제를 해결하기 위해, 송-수신자가 키의 일부를 사전에 공유하고,
	키의 일부는 송신자가 헤더에 암호화 안하고 얹어서 수신자에게 보냄.
	송수신자는 헤더에 실린 키와 사전 공유키를 조합하여 암복호화로 사용
	-> 헤더에 실린 키의 길이가 짧아 전수공격에 취약하고, 사전 공유키는 정보가 유출될
	   확률이 높아 공격에 취약

	3. 스트림 암호의 현대 방식으로의 대체. AES-CTR, chacha20
	chacha20 은 키스트림 위치를 카운터로 계산하여, 패킷 번호만 알면 그 위치로 
	점프해서 복호화가 가능하도록 설계됨
*/


class RC4
{
public:
	RC4(const uint8* key, int keyBytes);
	void crypt(uint8* data, int len);

private:
	void makeKeys(const uint8* key, int keyBytes);

private:
	static void swap(uint8& a, uint8& b);

private:
	uint8 sBox[256];
	int x;
	int y;


public:
	static void test();
};