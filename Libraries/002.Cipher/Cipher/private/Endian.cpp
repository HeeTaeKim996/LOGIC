#include "Endian.h"

/*----------------------------
		  LittleEndian
----------------------------*/
uint32 LittleEndian::loadTo32(const uint8* p)
{
	return (uint32)p[0]
		| (uint32)p[1] << 8
		| (uint32)p[2] << 16
		| (uint32)p[3] << 24;
}

void LittleEndian::storeFrom32(uint8* p, uint32 v)
{
	p[0] = (uint8)v;
	p[1] = (uint8)(v >> 8);
	p[2] = (uint8)(v >> 16);
	p[3] = (uint8)(v >> 24);
}





















/*----------------------------
		   BigEndian
----------------------------*/
void BigEndian::copyTo16(uint16* dst, const uint8* src, int n)
{
	for (int i = 0; i < n; ++i)
	{
		dst[i] = (uint16)((src[i * 2] << 8) |
			src[i * 2 + 1]);
	}
}

void BigEndian::copyFrom16(uint8* dst, const uint16* src, int n)
{
	for (int i = 0; i < n; ++i)
	{
		dst[i * 2] = (uint8)(src[i] >> 8);
		dst[i * 2 + 1] = (uint8)(src[i] & 0xFF);
	}
}

uint64 BigEndian::loadTo64(const uint8* p)
{
	return (uint64)p[7]
		| (uint64)p[6] << 8
		| (uint64)p[5] << 16
		| (uint64)p[4] << 24
		| (uint64)p[3] << 32
		| (uint64)p[2] << 40
		| (uint64)p[1] << 48
		| (uint64)p[0] << 56;
}

void BigEndian::storeFrom64(uint8* p, uint64 v)
{
	p[7] = (uint8)v;
	p[6] = (uint8)(v >> 8);
	p[5] = (uint8)(v >> 16);
	p[4] = (uint8)(v >> 24);
	p[3] = (uint8)(v >> 32);
	p[2] = (uint8)(v >> 40);
	p[1] = (uint8)(v >> 48);
	p[0] = (uint8)(v >> 56);
}




uint32 BigEndian::loadTo32(const uint8* p)
{
	return (uint32)p[3]
		| (uint32)p[2] << 8
		| (uint32)p[1] << 16
		| (uint32)p[0] << 24;
}


void BigEndian::storeFrom32(uint8* p, uint32 v)
{
	p[3] = (uint8)v;
	p[2] = (uint8)(v >> 8);
	p[1] = (uint8)(v >> 16);
	p[0] = (uint8)(v >> 24);
}