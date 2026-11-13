#pragma once
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

#if UINTPTR_MAX == UINT64_MAX
using word = uint64_t;
using sword = int64_t;
using half = uint32_t;

// ※ LIMB : 벡터 요소의 단위(비트)
#define MASK_LIMB 0x3F'FF'FF'FF	// 림브(블록) 마스크
#define LIMB_SIZE 30			// 림브(블록) 크기
#else
// 일단은 무시. 나중에 완성되면 여기도 처리
#endif


// TODO :bitLength 함수 만들고, longDivision 만들어보자
// (knuth D 보다 쉬운 로직이라 함)

class BigInt
{
public:
	BigInt() = default;
	BigInt(half* ptr, size_t size, bool isNeg = false);

	void printNum();
	string toString() const;

public:
	BigInt operator - () const;

	bool operator < (const BigInt& other) const;
	
	bool operator > (const BigInt& other) const { return other < *this; }
	
	bool operator <= (const BigInt& other) const 
	{return (*this > other) == false;}
	
	bool operator >=(const BigInt& other) const
	{return (*this < other) == false;}
	
	bool operator == (const BigInt& other) const
	{return neg == other.neg && vec == other.vec;} 
	// vec == 연산자는 주소가 아닌 내용 비교
	
	bool operator != (const BigInt& other) const
	{return (*this == other) == false;}


	BigInt operator + (const BigInt& other) const;
	BigInt operator - (const BigInt& other) const
	{ return *this + (-other); }
	
	BigInt operator * (const BigInt& other) const;

	BigInt operator << (const size_t bits) const;
	BigInt operator >> (const size_t bits) const;

private:
	static int cmpMag(const vector<half>& a, const vector<half>& b);
	static vector<half> addMag(const vector<half>& a,
		const vector<half>& b);
	static vector<half> subMag(const vector<half>& a, 
		const vector<half>& b);

	static vector<half> mulMag(const vector<half>& a,
		const vector<half>& b);

	static vector<half> divModSmall(const vector<half>& a, half d,
		half& rem);

	static vector<half> shiftLeftMag(const vector<half>& a, size_t bits);
	static vector<half> shiftRightMag(const vector<half>& a, size_t bits);



private:
	vector<half> vec;
	bool neg = false;


public:
	static void test();
};