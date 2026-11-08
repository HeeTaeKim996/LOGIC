#pragma once
#include "IBlockCryptor.h"
#include <string>

using namespace std;

enum BlockMode
{
	ECB,
	CBC,
	CFB,
	OFB,
	CTR
};

class BlockCrypt
{
public:
		~BlockCrypt();

public:
	void initCryptor(IBlockCryptor* inCryptor);
	string encrypt(string p);
	string decrypt(string c);


private:
	static void pad(string& s, size_t blockSize);
	static void unpad(string& s, size_t blockSize);

private:
	IBlockCryptor* blockCryptor = nullptr;
	BlockMode blockMode = BlockMode::ECB;


public:
	static void test();
};