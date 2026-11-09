#pragma once
#include "SizeType.h"
#include "IBlockCryptor.h"


// study/006.ARIA ÂüÁ¶

class ARIA : public IBlockCryptor
{
private:
	static const uint8 S1[256];
	static const uint8 S2[256];
	static const uint8 INV_S1[256];
	static const uint8 INV_S2[256];

public:
	ARIA(const uint8* key, int keyBits);
	virtual void encrypt(uint8* p) override
	{ crypt(p, encKeys); }
	virtual void decrypt(uint8* c) override
	{ crypt(c, decKeys);}
	virtual int getBlockSize() { return 16; }

private:
	void crypt(uint8* m, uint8 keys[17][16]);
	void makeDecKeys();
	void makeEncKeys(const uint8* key, int keyBits);

private:
	static void substType1(uint8* state);
	static void substType2(uint8* state);

	static void diffLayer(uint8* state);

	static void xorBlock(uint8* a, const uint8* b);

private:
	static uint8 rotLeft_8(uint8 v, int n);
	static void rotRight_128(uint8* out, const uint8* in, int n);
	static void rotLeft_128(uint8* out, const uint8* in, int n);


private:
	uint8 roundCount;
	uint8 encKeys[17][16];
	uint8 decKeys[17][16];



public:
	static void test();
};