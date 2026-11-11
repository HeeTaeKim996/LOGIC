#include "BlockCrypt.h"
#include <stdexcept>
#include "src/MemCounter.h"
#include <cassert>

BlockCrypt::~BlockCrypt()
{
	if (blockCryptor != nullptr)
	{
		TrackDelete(blockCryptor);
	}
}

void BlockCrypt::initCryptor(IBlockCryptor* inCryptor)
{
	if (blockCryptor == inCryptor) return;

	if (blockCryptor != nullptr)
	{
		TrackDelete(blockCryptor);
		resetMembers();
	}
	
	blockCryptor = inCryptor;
}



void BlockCrypt::resetMembers()
{
	keyStream.clear();
}





















string BlockCrypt::encrypt(string p)
{
	if (blockCryptor == nullptr) return p;

	const int blockSize = blockCryptor->getBlockSize();

	assert(blockSize >= 0 &&  blockSize <= 16); 
	// IV 를 블록이 최대 128 로 산정하여 처리함
	
	if (blockMode == BlockMode::ECB)
	{
		return encrypt_ECB(p, blockSize);
	}
	else if (blockMode == BlockMode::CBC)
	{
		return encrypt_CBC(p, blockSize);
	}
	else if (blockMode == BlockMode::CFB)
	{
		return encrypt_CFB(p, blockSize);
	}
	else if (blockMode == BlockMode::OFB)
	{
		return encrypt_OFB(p, blockSize);
	}
	else if (blockMode == BlockMode::CTR)
	{
		return encrypt_CTR(p, blockSize);
	}
	else if (blockMode == BlockMode::ECB_CTS)
	{
		return encrypt_ECB_CTS(p, blockSize);
	}
	else if (blockMode == BlockMode::CBC_CTS)
	{
		return encrypt_CBC_CTS(p, blockSize);
	}



	return p;
}

string BlockCrypt::decrypt(string c)
{
	if (blockCryptor == nullptr) return c;

	const int blockSize = blockCryptor->getBlockSize();
	assert(blockSize >= 0 && blockSize <= 16);


	if (blockMode == BlockMode::ECB)
	{
		return decrypt_ECB(c, blockSize);
	}
	else if (blockMode == BlockMode::CBC)
	{
		return decrypt_CBC(c, blockSize);
	}
	else if (blockMode == BlockMode::CFB)
	{
		return decrypt_CFB(c, blockSize);
	}
	else if (blockMode == BlockMode::OFB)
	{
		return decrypt_OFB(c, blockSize);
	}
	else if (blockMode == BlockMode::CTR)
	{
		return decrypt_CTR(c, blockSize);
	}
	else if (blockMode == BlockMode::ECB_CTS)
	{
		return decrypt_ECB_CTS(c, blockSize);
	}
	else if (blockMode == BlockMode::CBC_CTS)
	{
		return decrypt_CBC_CTS(c, blockSize);
	}

	return c;
}
































string BlockCrypt::encrypt_ECB(string& p, int blockSize)
{
	pad(p, blockSize);

	const size_t blockCount = p.size() / blockSize;

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	for (size_t i = 0; i < blockCount; ++i)
	{
		blockCryptor->encrypt(data + i * blockSize);
	}

	return p;
}

string BlockCrypt::decrypt_ECB(string& c, int blockSize)
{
	if (c.size() % blockSize != 0)
	{
		throw std::runtime_error("bad Length");
	}

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	const size_t blockCount = c.size() / blockSize;

	for (size_t i = 0; i < blockCount; ++i)
	{
		blockCryptor->decrypt(data + i * blockSize);
	}

	unpad(c, blockSize);
	return c;
}















string BlockCrypt::encrypt_CBC(string& p, int blockSize)
{
	pad(p, blockSize);

	const size_t blockCount = p.size() / blockSize;

	Vector<uint8> iv(blockSize);

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	memcpy(&iv[0], &IV[0], blockSize);

	for (size_t i = 0; i < blockCount; ++i)
	{
		uint8* block = data + i * blockSize;

		XOR(block, &iv[0], blockSize);
		blockCryptor->encrypt(block);

		memcpy(&iv[0], block, blockSize);
	}

	return p;
}

string BlockCrypt::decrypt_CBC(string& c, int blockSize)
{
	if (c.size() % blockSize != 0)
	{
		throw std::runtime_error("bad Length");
	}

	const size_t blockCount = c.size() / blockSize;

	Vector<uint8> iv(blockSize);

	memcpy(&iv[0], &IV[0], blockSize);

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	for (size_t i = 0; i < blockCount; ++i)
	{
		uint8* block = data + i * blockSize;

		Vector<uint8> temp(blockSize);
		memcpy(&temp[0], block, blockSize);

		blockCryptor->decrypt(block);
		XOR(block, &iv[0], blockSize);

		memcpy(&iv[0], &temp[0], blockSize);
	}

	unpad(c, blockSize);
	return c;
}










string BlockCrypt::encrypt_CFB(string& p, int blockSize)
{
	Vector<uint8> iv(blockSize);
	memcpy(&iv[0], &IV[0], blockSize);

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	size_t blockCount = p.size() / blockSize;
	size_t res = p.size() - blockSize * blockCount;

	for (size_t i = 0; i < blockCount; ++i)
	{
		uint8* block = data + i * blockSize;

		blockCryptor->encrypt(&iv[0]);
		XOR(block, &iv[0], blockSize);

		memcpy(&iv[0], block, blockSize);
	}

	if (res)
	{
		uint8* resBytes = data + blockCount * blockSize;

		blockCryptor->encrypt(&iv[0]);
		XOR(resBytes, &iv[0], res);
	}

	return p;
}

string BlockCrypt::decrypt_CFB(string& c, int blockSize)
{
	Vector<uint8> iv(blockSize);
	memcpy(&iv[0], &IV[0], blockSize);

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	size_t blockCount = c.size() / blockSize;
	size_t res = c.size() - blockSize * blockCount;

	for (size_t i = 0; i < blockCount; ++i)
	{
		uint8* block = data + i * blockSize;

		Vector<uint8> temp(blockSize);
		memcpy(&temp[0], block, blockSize);

		blockCryptor->encrypt(&iv[0]);
		XOR(block, &iv[0], blockSize);

		memcpy(&iv[0], &temp[0], blockSize);
	}

	if (res)
	{
		uint8* resBytes = data + blockCount * blockSize;

		blockCryptor->encrypt(&iv[0]);
		XOR(resBytes, &iv[0], res);
	}

	return c;
}












string BlockCrypt::encrypt_OFB(string& p, int blockSize)
{
	size_t needSize 
		= (p.size() + blockSize - 1) / blockSize * blockSize;
	// EX. size = 16. blockSize = 8 -> need = (16+8-1) / 16 * 16
	//			= 23 / 16 * 16 = 16
	//	   size = 17 . blockSize = 8 -> need = (17+8-1) / 16 * 16
	//			= 24 / 16 * 16 = 32

	if (keyStream.size() < needSize)
	{
		addStreamKey_OFB(needSize, blockSize);
	}


	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	XOR(data, &keyStream[0], p.size());

	return p;
}

string BlockCrypt::decrypt_OFB(string& c, int blockSize)
{
	size_t needSize
		= (c.size() + blockSize - 1) / blockSize * blockSize;

	if (keyStream.size() < needSize)
	{
		addStreamKey_OFB(needSize, blockSize);
	}

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	XOR(data, &keyStream[0], c.size());
	
	return c;
}

void BlockCrypt::addStreamKey_OFB(size_t needSize, int blockSize)
{
	keyStream.reserve(needSize);

	Vector<uint8> temp(blockSize);
	if (keyStream.empty() == false)
	{
		memcpy(&temp[0], &keyStream[keyStream.size() - blockSize],
			blockSize);
	}
	else
	{
		memcpy(&temp[0], &IV[0], blockSize);
	}

	while (keyStream.size() < needSize)
	{
		blockCryptor->encrypt(&temp[0]);

		keyStream.insert(keyStream.end(),
			temp.begin(), temp.end());
	}
}



















string BlockCrypt::encrypt_CTR(string& p, int blockSize)
{
	size_t needBlock
		= (p.size() + blockSize - 1) / blockSize;
	size_t doneBlock = keyStream.size() / blockSize;

	if (doneBlock < needBlock)
	{
		addStreamKey_CTR(needBlock, doneBlock, blockSize);
	}

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	XOR(data, &keyStream[0], p.size());

	return p;
}

string BlockCrypt::decrypt_CTR(string& c, int blockSize)
{
	size_t needBlock
		= (c.size() + blockSize - 1) / blockSize;
	size_t doneBlock = keyStream.size() / blockSize;

	if (doneBlock < needBlock)
	{
		addStreamKey_CTR(needBlock, doneBlock, blockSize);
	}

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	XOR(data, &keyStream[0], c.size());

	return c;
}

void BlockCrypt::addStreamKey_CTR(size_t needBlock, 
	size_t doneBlock, int blockSize)
{
	for (size_t i = doneBlock; i < needBlock; ++i)
	{
		Vector<uint8> ctr(blockSize);
		memcpy(&ctr[0], &IV[0], blockSize); 
		// IV 를 CTR 초기값으로 봄

		addCounter_CTR(&ctr[0], blockSize, i);

		blockCryptor->encrypt(&ctr[0]);
		keyStream.insert(keyStream.end(),ctr.begin(), ctr.end());
	}
}




void BlockCrypt::addCounter_CTR(uint8* block, int blockSize, size_t add)
{
	// block 을 빅엔디안으로 보고 더하기
	for (int i = blockSize - 1; i >= 0 && add > 0; --i)
	{
		uint64 sum = (uint64)block[i] + (add & 0xFF);
		block[i] = (uint8)sum;
		
		add = (add >> 8) + (sum >> 8); // 더하기의 올림을 반영
	}
}





















string BlockCrypt::encrypt_ECB_CTS(string& p, int blockSize)
{
	size_t blockCount = p.size() / blockSize;

	if (blockCount == 0)
	{
		throw std::runtime_error(
"CTS는 최소 1개의 FULL 블록이 필요라 에러(실제 하드웨어에서도 에러 낸다 함");
	}

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	for (size_t i = 0; i < blockCount; ++i)
	{
		blockCryptor->encrypt(data + i * blockSize);
	}

	int res = p.size() - blockCount * blockSize;
	if (res == 0) return p; 
	// res 가 없을 경우 ECB 와 동일 (패딩이 없는)

	
	uint8* lastFull = data + (blockCount - 1) * blockSize;
	uint8* tail = data + blockCount * blockSize;

	Vector<uint8> combined(blockSize);
	memcpy(&combined[0], tail, res);
	memcpy(&combined[res], lastFull + res, blockSize - res);

	blockCryptor->encrypt(&combined[0]);

	memcpy(tail, &lastFull[0], res);
	memcpy(lastFull, &combined[0], blockSize);

	return p;
}

string BlockCrypt::decrypt_ECB_CTS(string& c, int blockSize)
{
	size_t blockCount = c.size() / blockSize;

	if (blockCount == 0)
	{
		throw std::runtime_error(
"CTS는 최소 1개의 FULL 블록이 필요라 에러(실제 하드웨어에서도 에러 낸다 함");
	}

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	for (size_t i = 0; i < blockCount; ++i)
	{
		blockCryptor->decrypt(data + i * blockSize);
	}

	int res = c.size() - blockCount * blockSize;
	if (res == 0) return c;
	// res 가 없을 경우 ECB 와 동일 (패딩이 없는)

	uint8* lastFull = data + (blockCount - 1) * blockSize;
	uint8* tail = data + blockCount * blockSize;

	Vector<uint8> realLast(blockSize);
	memcpy(&realLast[0], tail, res);
	memcpy(&realLast[res], &lastFull[res], blockSize - res);
	blockCryptor->decrypt(&realLast[0]);

	memcpy(tail, lastFull, res);
	memcpy(lastFull, &realLast[0], blockSize);

	return c;
}




























string BlockCrypt::encrypt_CBC_CTS(string& p, int blockSize)
{
	const size_t blockCount = p.size() / blockSize;

	if (blockCount == 0)
	{
		throw std::runtime_error(
"CTS는 최소 1개의 FULL 블록이 필요라 에러(실제 하드웨어에서도 에러 낸다 함");
	}

	Vector<uint8> iv(blockSize);

	memcpy(&iv[0], &IV[0], blockSize);

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	for (size_t i = 0; i < blockCount; ++i)
	{
		uint8* block = data + i * blockSize;

		XOR(block, &iv[0], blockSize);
		blockCryptor->encrypt(block);

		memcpy(&iv[0], block, blockSize);
	}


	int res = p.size() - blockCount * blockSize;
	if (res == 0) return p;

	uint8* lastFull = data + (blockCount - 1) * blockSize;
	uint8* tail = lastFull + blockSize;

	XOR(tail, lastFull, res);

	Vector<uint8> combined(blockSize);
	memcpy(&combined[0], tail, res);
	memcpy(&combined[res], &lastFull[res], blockSize - res);

	blockCryptor->encrypt(&combined[0]);

	memcpy(tail, lastFull, res);
	memcpy(lastFull, &combined[0], blockSize);

	return p;
}

string BlockCrypt::decrypt_CBC_CTS(string& c, int blockSize)
{
	const size_t blockCount = c.size() / blockSize;

	if (blockCount == 0)
	{
		throw std::runtime_error(
			"CTS는 최소 1개의 FULL 블록이 필요라 에러(실제 하드웨어에서도 에러 낸다 함");
	}

	Vector<uint8> iv(blockSize);
	memcpy(&iv[0], &IV[0], blockSize);

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	for (size_t i = 0; i < blockCount - 1; ++i)
	{
		uint8* block = data + i * blockSize;

		Vector<uint8> temp(blockSize);
		memcpy(&temp[0], block, blockSize);

		blockCryptor->decrypt(block);
		XOR(block, &iv[0], blockSize);

		memcpy(&iv[0], & temp[0], blockSize);
	}

	{ 
		// C[i-2] 값(현 iv)을 XOR 에 사용하기 위한 기록이 필요하며, 
		// encrypt 의 combined 는 C[i-2] 로 XOR 처리하지 않기에,
		// 따로 처리
		uint8* block = data + (blockCount - 1) * blockSize;

		blockCryptor->decrypt(block);
	}

	int res = c.size() - blockCount * blockSize;
	if (res == 0) return c;


	uint8* lastFull = data + (blockCount - 1) * blockSize;
	uint8* tail = lastFull + blockSize;
	

	Vector<uint8> realLast(blockSize);
	memcpy(&realLast[0], tail, res);
	memcpy(&realLast[res], &lastFull[res], blockSize - res);

	XOR(&lastFull[0], tail, res);

	blockCryptor->decrypt(&realLast[0]);
	XOR(&realLast[0], &iv[0], blockSize); // C[i-2] 와 XOR

	memcpy(tail, &lastFull[0], res);
	memcpy(lastFull, &realLast[0], blockSize);

	return c;
}


































void BlockCrypt::pad(string& s, size_t blockSize)
{
	size_t n = blockSize - (s.size() % blockSize);
	// blockSize 는 0 초과이므로, n 의 값으로 0이 올 수 없다
	// (내용이 이미 블록을 꽉 채워도, n = blockSize 로 패딩됨)

	s.append(n, static_cast<char>(n));
	// n 을 n개(1바이트 단위)로 붙인다
}

void BlockCrypt::unpad(string& s, size_t blockSize)
{
	if (s.empty() || s.size() % blockSize != 0)
	{
		throw std::runtime_error("bad length");
	}

	size_t n = static_cast<uint8>(s.back());
	if (n == 0 || n > blockSize || n > s.size())
	{
		throw std::runtime_error("bad padding");
	}

	for (size_t i = s.size() - n; i < s.size(); ++i)
	{
		if (static_cast<uint8>(s[i]) != n)
		{
			throw std::runtime_error("bad padding");
		}
	}

	// 입력된 n값대로, n개를 제거
	s.resize(s.size() - n);
}

void BlockCrypt::XOR(uint8* dst, uint8* x, int n)
{
	for (int i = 0; i < n; ++i)
	{
		dst[i] ^= x[i];
	}
}





























#include "AES.h"
#include "IDEA.h"
#include "LEA.h"
#include <functional>
void BlockCrypt::test()
{
	BlockCrypt blockCrypt;

	uint8 key[32];
	for (int i = 0; i < 32; ++i) key[i] = (uint8)i;


	string pt = "다만 한 가지 짚을 점이 있습니다. 성경 본문은 번역본마다 저작권이 다릅니다. 현재 한국에서 흔히 쓰는 개역개정,"
		"새번역, 공동번역 등은 저작권이 있어서 제가 본문을 그대로 옮겨 드리기는 어렵습니다. 대신 합법적으로 쓸 수 있는 방법이 있습니다.";

	string pt2 = "5추가. 다만 한 가지 짚을 점이 있습니다. 성경 본문은 번역본마다 저작권이 다릅니다. 현재 한국에서 흔히 쓰는 개역개정,"
		"새번역, 공동번역 등은 저작권이 있어서 제가 본문을 그대로 옮겨 드리기는 어렵습니다. 대신 합법적으로 쓸 수 있는 방법이 있습니다.";

	auto test = [&](IBlockCryptor* blockCryptor, string p) -> void
		{
			blockCrypt.initCryptor(blockCryptor);

			string c = blockCrypt.encrypt(p);
			printf("%s\n\n", c.c_str());

			string p2 = blockCrypt.decrypt(c);
			printf("%s\n\n", p2.c_str());
		};

	test(TrackNew<AES>(key, 32), pt);
	test(TrackNew<LEA>(key, 128), pt2);
}