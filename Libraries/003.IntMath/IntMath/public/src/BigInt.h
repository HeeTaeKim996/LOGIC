#pragma once
#include <vector>
#include <cstdint>

using namespace std;

#if UINTPTR_MAX == UINT64_MAX
using word = uint64_t;
using sword = int64_t;
using half = uint32_t;
#define MASK_BLOCK 0x3F'FF'FF'FF
#define BLOCK_SIZE 30
#else
// 일단은 무시. 나중에 완성되면 여기도 처리
#endif


class BigInt
{
public:
	BigInt() = default;
	BigInt(half* ptr, size_t size, bool isNeg = false);

	void printNum();

public:
	BigInt operator - () const;

	BigInt operator + (const BigInt& other) const;
	BigInt operator - (const BigInt& other) const
	{ return *this + (-other); }


private:
	static int cmpMag(const vector<half>& a, const vector<half>& b);
	static vector<half> addMag(const vector<half>& a,
		const vector<half>& b);
	static vector<half> subMag(const vector<half>& a, 
		const vector<half>& b);

private:
	vector<half> vec;
	bool neg = false;


public:
	static void test();
};