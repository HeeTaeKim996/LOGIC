#pragma once

#include "SizeType.h"
#include "IBlockCryptor.h"

// ¡Ø $(ProjectDir)study/003.ExtendedEuclidianAlgorithm, 004.IDEA ÂüÁ¶


#define M 0x1'00'01		// 2 ^ 16 + 1
#define CEIL 0x1'00'00	// 2 ^ 16

class IDEA : public IBlockCryptor
{
public:
	IDEA(const uint8* key);
	virtual void encrypt(uint8* p) override { crypt(p, encKeys); }
	virtual void decrypt(uint8* c) override { crypt(c, decKeys); }
	virtual int getBlockSize() { return 8; }


private:
	void crypt(uint8* p, const uint16* key);

	static uint16 add(uint16 x, uint16 y);

	static uint16 mul(uint16 x, uint16 y);



	static uint16 extendedEuclidian(uint16 k);
	static void swap(uint16& x, uint16& y);



private:
	void makeEncKeys(const uint8* key);
	void makeDecKeys();

private:
	uint16 encKeys[52];
	uint16 decKeys[52];


public:
	static void test();
};