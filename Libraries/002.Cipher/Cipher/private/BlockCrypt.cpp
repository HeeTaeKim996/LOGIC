#include "BlockCrypt.h"
#include <stdexcept>
#include "src/MemCounter.h"

BlockCrypt::~BlockCrypt()
{
	if (blockCryptor != nullptr)
	{
		TrackDelete(blockCryptor);
	}
}

void BlockCrypt::initCryptor(IBlockCryptor* inCryptor)
{
	if (blockCryptor != nullptr)
	{
		TrackDelete(blockCryptor);
	}
	if (blockCryptor == inCryptor) return;

	blockCryptor = inCryptor;
}

string BlockCrypt::encrypt(string p)
{
	if (blockCryptor == nullptr) return p;

	const int blockSize = blockCryptor->getBlockSize();
	pad(p, blockSize);

	uint8* data = reinterpret_cast<uint8*>(&p[0]);
	const size_t blockCount = p.size() / blockSize;

	if (blockMode == BlockMode::ECB)
	{
		for (size_t i = 0; i < blockCount; ++i)
		{
			blockCryptor->encrypt(data + i * blockSize);
		}
	}

	// TODO : ECB 외에 모드 처리 추가

	return p;
}

string BlockCrypt::decrypt(string c)
{
	if (blockCryptor == nullptr) return c;

	const int blockSize = blockCryptor->getBlockSize();
	if (c.size() % blockSize != 0)
	{
		throw std::runtime_error("bad Length");
	}

	uint8* data = reinterpret_cast<uint8*>(&c[0]);
	const size_t blockCount = c.size() / blockSize;

	if (blockMode == BlockMode::ECB)
	{
		for (size_t i = 0; i < blockCount; ++i)
		{
			blockCryptor->decrypt(data + i * blockSize);
		}
	}
	
	// TODO : ECB 외에 모드 처리 추가


	unpad(c, blockSize);
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





























#include "AES.h"
void BlockCrypt::test()
{
	BlockCrypt blockCrypt;

	uint8 key[32];
	for (int i = 0; i < 32; ++i) key[i] = (uint8)i;

	blockCrypt.initCryptor(TrackNew<AES>(key, 32));

	string pt = "다만 한 가지 짚을 점이 있습니다. 성경 본문은 번역본마다 저작권이 다릅니다. 현재 한국에서 흔히 쓰는 개역개정,"
		"새번역, 공동번역 등은 저작권이 있어서 제가 본문을 그대로 옮겨 드리기는 어렵습니다. 대신 합법적으로 쓸 수 있는 방법이 있습니다.";
	string ct = blockCrypt.encrypt(pt);
	printf("%s\n", ct.c_str());
	string pt2 = blockCrypt.decrypt(ct);
	printf("%s\n", pt2.c_str());
}