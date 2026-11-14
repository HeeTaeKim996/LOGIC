#pragma once
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

#if UINTPTR_MAX == UINT64_MAX
//#if false
using word = uint64_t;
using sword = int64_t;
using half = uint32_t;

// ※ LIMB : 벡터 요소의 단위(비트)
#define MASK_LIMB 0x3F'FF'FF'FF	// 림브(블록) 마스크
#define LIMB_SIZE 30			// 림브(블록) 크기

#define CHUNK_VALUE 1'000'000'000
#define CHUNK_DIGITS 9

#else
using word = uint32_t;
using sword = int32_t;
using half = uint16_t;
#define MASK_LIMB 0x3F'FF
#define LIMB_SIZE 14
#define CHUNK_VALUE 10'000
#define CHUNK_DIGITS 4
#endif



/*
	PYTHON (CPYTHON) 의 정수 타입을 모방함

	더하기, 빼기 는 PYTHON 과 거의 같은 방식이라 하며, 
	(사람이 쓰는 산술 방식을 10진법이 아닌 LIMB 진법으로 구현)

	곱하기 도 LIMB 가 70 개 미만(목표인 256 비트는 9 LIMB)일 때는
	파이썬과 같은 방식이며
	(사람이 쓰는 산술 방식을 LIMB 진법으로 구현)

	나누기(나머지)는 divModeSmallMag 함수(제수단위가 half) 는 파이썬과 같은 방식,
	(사람이 쓰는 산술 방식을 LIMB 진법으로 구현)



	다만 제수단위가 half 를 넘어선 vector<half> 인 경우에 나누기는,
	여기서는 divModBigMag 으로 구현했지만, 
	(하드웨어 나누기의 리스토링 알고리즘을 모방하여 구현)
	
	파이썬의 알고리즘과 다르다. 파이썬은 Knuth D 를 사용한다는데, 구현이 굉장히
	복잡하다 해서, 포기했음
	(다만 속도 때문에 파이썬아 Knuth D 를 사용하는 거지, 정확성에서는 리스토링 모방
	 알고리즘이 더 정확하기 때문에, 테스트를 위해 BigInt를 구현한 나는 knuth D 를
	 구현할 이유가 없다)
*/


class __declspec(dllexport) BigInt
{
public:
	BigInt() = default;
	BigInt(half* ptr, size_t size, bool isNeg = false);
	BigInt(sword num);
	BigInt(const string& s);


public:
	BigInt operator - () const;

	bool operator < (const BigInt& other) const;

	bool operator > (const BigInt& other) const { return other < *this; }

	bool operator <= (const BigInt& other) const
	{
		return (*this > other) == false;
	}

	bool operator >=(const BigInt& other) const
	{
		return (*this < other) == false;
	}

	bool operator == (const BigInt& other) const
	{
		return neg == other.neg && vec == other.vec;
	}
	// vec == 연산자는 주소가 아닌 내용 비교

	bool operator != (const BigInt& other) const
	{
		return (*this == other) == false;
	}


	BigInt operator + (const BigInt& other) const;
	BigInt operator - (const BigInt& other) const
	{
		return *this + (-other);
	}

	BigInt operator * (const BigInt& other) const;

	BigInt operator / (const BigInt& b) const;
	BigInt operator % (const BigInt& b) const;

	BigInt operator & (const BigInt& other) const;
	BigInt operator | (const BigInt& other) const;

	BigInt operator << (const size_t bits) const;
	BigInt operator >> (const size_t bits) const;






	// if(a)
	explicit operator bool() const { return vec.empty() == false; }
	/* □ explicit
		- 암시적 형변환 방지. bool() 로 인해 암시적 형변환으로 bool 이 되는 문제 방지
	   EX)
		BigInt a("123");

		int n = a; (암시적 형변환으로 1이 됨)
		bool b = a; (암시적 형변환으로 true 가 됨)

		explicit 을 쓰면, 위같은 암시 형변환을 방지하고,

		if(a), while(a), if(!a) 등에서만 bool() 이 사용되도록 함
	*/

	BigInt& operator >>= (size_t bits) { return *this = *this >> bits; }
	BigInt& operator %= (const BigInt& other) { return *this = *this % other; }
	BigInt& operator += (const BigInt& other) { return *this = *this + other; }
	BigInt& operator -= (const BigInt& other) { return *this = *this - other; }
	BigInt& operator &=(const BigInt& other) { return *this = *this & other; }
	BigInt& operator |=(const BigInt & other) { return *this = *this | other; }

	BigInt& operator -- ()
	{return *this = *this - BigInt(1);}
	BigInt operator -- (int);

public:
	void printNum();
	string toString() const;
	size_t bitLength() const;
	void divModBig(const BigInt& b, BigInt& q, BigInt& r) const;
	void divModSmall(const BigInt& b, BigInt& q, BigInt& r) const;

private:
	static int cmpMag(const vector<half>& a, const vector<half>& b);
	static vector<half> addMag(const vector<half>& a,
		const vector<half>& b);
	static vector<half> subMag(const vector<half>& a, 
		const vector<half>& b);

	static vector<half> mulMag(const vector<half>& a,
		const vector<half>& b);

	static vector<half> divModSmallMag(const vector<half>& a, half d,
		half& rem);

	static void divModBigMag(const BigInt& a, const BigInt& b,
		vector<half>& q, vector<half>& r);


	static vector<half> shiftLeftMag(const vector<half>& a, size_t bits);
	static vector<half> shiftRightMag(const vector<half>& a, size_t bits);



private:
	vector<half> vec;
	bool neg = false;


public:
	static void test();
};