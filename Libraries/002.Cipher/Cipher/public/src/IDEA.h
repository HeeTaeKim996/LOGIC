#pragma once

#include "src/SizeType.h"



#define M 0x1'00'01		// 2 ^ 16 + 1
#define CEIL 0x1'00'00	// 2 ^ 16

#define WHEN_ENDIAN_MATTERS

class IDEA
{
public:
	IDEA(uint8* key);
	void encrypt(uint8* p) { crypt(p, encKeys); }
	void decrypt(uint8* c) { crypt(c, decKeys); }



private:
	void crypt(uint8* p, const uint16* key);

	static uint16 add(uint16 x, uint16 y);

	static uint16 mul(uint16 x, uint16 y);



	static uint16 extendedEuclidian(uint16 k);
	static void swap(uint16& x, uint16& y);



private:
	void makeEncKeys(uint8* key);
	void makeDecKeys();

private:
	uint16 encKeys[52];
	uint16 decKeys[52];



#ifdef WHEN_ENDIAN_MATTERS
private: // 엔디안 불일치 문제가 있다는 가정하에, 엔디안 정렬하는 함수
	static inline void reinterpretCast_to16(uint16* dst, uint8* src, int n)
	{
		for (int i = 0; i < n; ++i)
		{
			dst[i] = (uint16)((src[i * 2] << 8) | src[i * 2 + 1]);
		}
	}

	static inline void reinterpretCast_to8(uint8* dst, uint16* src, int n)
	{
		for (int i = 0; i < n; ++i)
		{
			dst[i * 2] = (uint8)(src[i] >> 8);
			dst[i * 2 + 1] = (uint8)(src[i] & 0xFF);
		}
	}
#endif

public:
	static void test();
};