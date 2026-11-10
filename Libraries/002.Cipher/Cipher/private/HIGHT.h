#pragma once
#include "SizeType.h"
#include "IBlockCryptor.h"


// ¡Ø $(SolutionDir)study/007.HIGHT ÂüÁ¶

class HIGHT : public IBlockCryptor
{

public:
	HIGHT(const uint8* key);
	virtual void encrypt(uint8* p) override;
	virtual void decrypt(uint8* c) override;
	virtual int getBlockSize() { return 8; }

private:
	static uint8 F0(uint8 x);
	static uint8 F1(uint8 x);
	static uint8 rotLeft_8(uint8 x, int n);

	void expandKeys(const uint8* key);

private:
	uint8 whiteningKeys[8];
	uint8 subKeys[32][4];



public:
	static void test();
};