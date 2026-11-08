#pragma once
#include "SizeType.h"

class IBlockCryptor
{
public:
	virtual void encrypt(uint8* p) = 0;
	virtual void decrypt(uint8* c) = 0;
	virtual int getBlockSize() = 0;
};