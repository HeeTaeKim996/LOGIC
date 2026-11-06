#pragma once


using uint8 = unsigned char;

// ※ 개념 정리는 $(ProjectDir)study\003.AES 에 위치

/* ■ 행우선과 열우선
	MIX, INVMIX 는 state 매트릭스와 연산할 때 
	왼쪽 ( MIX * S, INV_MIX * S) 에 위치하니 행우선 매트릭스로
	AES 에서 설계되었다.

	반면 state 매트릭스는 연산시 오른쪽 에 위치하니, 열우선 매트릭스
	로 가정되어 코드가 쓰였다
	( for(int c = 0; c < 4; ++c) for(int r = 0; r < 4; ++r)
	  처럼 r 보다 c를 먼저 써야 함 )
*/


class AES
{
private:
	static const uint8 SBOX[256];
	static const uint8 INV_SBOX[256];
	static const uint8 MIX[4][4];
	static const uint8 INV_MIX[4][4];

public:
	AES(const uint8* key, int keyBytes);
	void encrypt(uint8* p);
	void decrypt(uint8* c);

private:
	static void subBytes(uint8* state);
	static void invSubBytes(uint8* state);

	static uint8 gmul(uint8 a, uint8 b);
	static void mixColumns(uint8* state);
	static void invMixColumns(uint8* state);

	static void shiftRows(uint8* state);
	static void invShiftRows(uint8* state);

	void addRoundKey(uint8* state, int round);

private:
	void keyExpansion(const uint8* key, int keyBytes);


public:
	static void test();



private:
	uint8 roundKeys[15][16];
	int numberOfRounds;
};

