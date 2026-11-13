#include "src//BigInt.h"

BigInt::BigInt(half* ptr, size_t size, bool isNeg)
{
	if (ptr == nullptr || size == 0) return;

	vec.reserve(size * sizeof(half) / MASK_BLOCK);
	// 그냥 여기까지 대충할당한다. 더 까다롭게 처리하고 싶지 않다.

	word acc = 0; // 누적기 ( BLOCK_SIZE 보다 누적값이 커질 때 push_back )
	int bits = 0; 
	// 시프팅기 
	// ( 새로 acc에 할당할 때 기존 값들이 bits 에 있으니, 새로 입력되는 값을 시프팅)

	for (size_t i = 0; i < size; ++i) 
		// ptr 이 uint8* 이 아니라 half* 이니, ++i 가능
	{
		acc |= (word)ptr[i] << bits;
		bits += (BLOCK_SIZE + 2);

		while (bits >= BLOCK_SIZE)
		{
			vec.push_back((half)(acc & MASK_BLOCK));
			acc >>= BLOCK_SIZE;
			bits -= BLOCK_SIZE;
		}
	}
	if (bits > 0)
	{
		// 잔여 bits 들을 할당
		vec.push_back((half)acc);
	}


	// 잉여 0값들 제거
	while (vec.empty() == false && vec.back() == 0)
	{
		vec.pop_back();
	}

	if (vec.empty() == false)
	{
		neg = isNeg;
	}
}


void BigInt::printNum()
{
	// ※ 아래 코드는 실제 10진수를 표기한 건 아니지만(2비트씩 공간이 있으므로), 
	// 어차피 큰 수 봐도 내가 눈으로 저게 맞는 수인지 확인 못한다. 대충 쓰자

	if (vec.empty() == false && neg)
	{
		printf("-");
	}
	for (size_t i = vec.size(); i-- > 0;)
	{
		printf("%u/", vec[i]);
	}
	printf("\n");
}

BigInt BigInt::operator-() const
{
	BigInt out = *this;
	if (out.vec.empty() == false)
	{
		out.neg = !out.neg;
	}
	return out;
}


BigInt BigInt::operator+(const BigInt& other) const
{
	BigInt out;

	if (neg == other.neg)
	{
		out.vec = addMag(vec, other.vec);
		out.neg = neg;
	}
	else
	{
		int c = cmpMag(vec, other.vec);
		if (c == 0) return out;
		if (c > 0)
		{
			out.vec = subMag(vec, other.vec);
			out.neg = neg;
		}
		else
		{
			out.vec = subMag(other.vec, vec);
			out.neg = other.neg;
		}
		if (out.vec.empty())
		{
			out.neg = false;
		}
	}

	return out;
}







int BigInt::cmpMag(const vector<half>& a, const vector<half>& b)
{
	// a 가 크면 1 리턴, b가 크면 -1 리턴, 같으면 0

	if (a.size() != b.size())
	{
		return a.size() < b.size() ? -1 : 1;
	}
	for (size_t i = a.size(); i-- > 0;) // size_t 는 unsigned 이니, (2) 처럼
	{
		if (a[i] != b[i])
		{
			return a[i] < b[i] ? -1 : 1;
		}
	}
	return 0;
}

vector<half> BigInt::addMag(const vector<half>& a, const vector<half>& b)
{
	size_t n = max(a.size(), b.size());
	vector<half> out;
	out.reserve(n + 1);

	half carry = 0;
	for (size_t i = 0; i < n; ++i)
	{
		word sum = carry;
		if (i < a.size()) sum += a[i];
		if (i < b.size()) sum += b[i];
		out.push_back((half)(sum & MASK_BLOCK));
		carry = sum >> BLOCK_SIZE;
	}
	if (carry)
	{
		out.push_back((half)carry);
	}

	return out;
}

vector<half> BigInt::subMag(const vector<half>& a, const vector<half>& b)
{
	// |a| >= |b| 전제
	vector<half> out;
	out.reserve(a.size());

	sword borrow = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		sword d = (sword)a[i] - borrow;
		if (i < b.size()) d -= (sword)b[i];

		if (d < 0)
		{
			d += (sword)1 << BLOCK_SIZE; // 이번 자리에서 한블록 빌려옴
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		out.push_back((half)d);
	}

	while (out.empty() == false && out.back() == 0)
	{
		out.pop_back();
	}

	return out;
}























































#include <functional>
void BigInt::test()
{



	auto func = [](word n1, word n2) -> void
		{
			BigInt b1(reinterpret_cast<half*>(&n1), 2);
			BigInt b3(reinterpret_cast<half*>(&n1), 2, true);

			BigInt b2(reinterpret_cast<half*>(&n2), 2);
			BigInt b4(reinterpret_cast<half*>(&n2), 2, true);

			(b1 + b2).printNum();
			(b1 - b2).printNum();
			(b3 + b4).printNum();
			(b3 - b4).printNum();
			printf("\n\n");
		};

	word n1 = UINT64_MAX;
	func(n1, n1);

	word n2 = 3;
	word n3 = 4;
	func(n2, n3);
}
