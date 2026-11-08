#pragma once


#include "SizeType.h"

class LittleEndian
{
public:
	static uint32 loadTo32(const uint8* p);
	static void storeFrom32(uint8* p, uint32 v);

	

};


class BigEndian
{
public:
	static void copyTo16(uint16* dst, const uint8* src, int n);
	static void copyFrom16(uint8* dst, const uint16* src, int n);

	static uint64 loadTo64(const uint8* p);
	static void storeFrom64(uint8* p, uint64 v);
};