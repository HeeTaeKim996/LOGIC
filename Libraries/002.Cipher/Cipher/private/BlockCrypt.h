#pragma once
#include "IBlockCryptor.h"
#include <string>
#include "use/use_vector.h"

using namespace std;

enum BlockMode
{
	ECB,
	CBC,
	CFB,
	OFB,
	CTR,
	ECB_CTS,
	CBC_CTS
};

class BlockCrypt
{
public:
		~BlockCrypt();

private:
	void resetMembers();

public:
	void initCryptor(IBlockCryptor* inCryptor);
	string encrypt(string p);
	string decrypt(string c);



private:
	string encrypt_ECB(string& p, int blockSize);
	string decrypt_ECB(string& c, int blockSize);

	string encrypt_CBC(string& p, int blockSize);
	string decrypt_CBC(string& c, int blockSize);

	string encrypt_CFB(string& p, int blockSize);
	string decrypt_CFB(string& c, int blockSize);

	string encrypt_OFB(string& p, int blockSize);
	string decrypt_OFB(string& c, int blockSize);
	void addStreamKey_OFB(size_t needBytes, int blockSize);

	string encrypt_CTR(string& p, int blockSize);
	string decrypt_CTR(string& p, int blockSize);
	void addStreamKey_CTR(size_t needBytes, size_t doneBlock, int blockSize);
	void addCounter_CTR(uint8* block, int blockSize, size_t add);


	string encrypt_ECB_CTS(string& p, int blockSize);
	string decrypt_ECB_CTS(string& c, int blockSize);

	static void pad(string& s, size_t blockSize);
	static void unpad(string& s, size_t blockSize);
	
	static void XOR(uint8* dst, uint8* x, int n);

private:
	IBlockCryptor* blockCryptor = nullptr;
	BlockMode blockMode = BlockMode::ECB_CTS;

	// ※ 초기값 ( 블록이 128 비트 이하로 가정 )
	uint8 IV[16] = { 
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
	Vector<uint8> keyStream;


public:
	static void test();
};