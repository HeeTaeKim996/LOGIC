#pragma once
#include "SizeType.h"
#include "IBlockCryptor.h"

// ¡Ø $(SolutionDir)study/008.LEA ÂüÁ¶

class LEA : public IBlockCryptor
{

public:
	LEA(const uint8* key, int bitLen);
	virtual void encrypt(uint8* p) override;
	virtual void decrypt(uint8* c) override;
	virtual int getBlockSize() { return 16; }

private:
	static uint32 rotLeft_32(uint32 x, int n);
	static uint32 rotRight_32(uint32 x, int n);

	void expandKey(const uint8* key, int bitLen);

private:
	uint32 subKeys[32][6];
	int roundCount;



public:
	static void test();
};