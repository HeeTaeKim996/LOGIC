#include "src//BigInt.h"

BigInt::BigInt(half* ptr, size_t size, bool isNeg)
{
	if (ptr == nullptr || size == 0) return;

	vec.reserve(size * sizeof(half) / MASK_LIMB);
	// 그냥 여기까지 대충할당한다. 더 까다롭게 처리하고 싶지 않다.

	word acc = 0; // 누적기 ( BLOCK_SIZE 보다 누적값이 커질 때 push_back )
	int bits = 0; 
	// 시프팅기 
	// ( 새로 acc에 할당할 때 기존 값들이 bits 에 있으니, 새로 입력되는 값을 시프팅)

	for (size_t i = 0; i < size; ++i) 
		// ptr 이 uint8* 이 아니라 half* 이니, ++i 가능
	{
		acc |= (word)ptr[i] << bits;
		bits += (LIMB_SIZE + 2);

		while (bits >= LIMB_SIZE)
		{
			vec.push_back((half)(acc & MASK_LIMB));
			acc >>= LIMB_SIZE;
			bits -= LIMB_SIZE;
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
	printf("%s\n", toString().c_str());
}

string BigInt::toString() const
{
	if (vec.empty()) return "0";

	const half CHUNK = 1'000'000'000; 
	// 10 ^ 9. 9자리씩 뽑기 위해
	vector<half> q = vec;
	vector<half> chunks;

	while (q.empty() == false)
	{
		half rem;
		q = divModSmall(q, CHUNK, rem); 
		// CHUNK 진수(?) 로 절사해서 출력하는 그 방식이네
		chunks.push_back(rem);
	}

	string s = neg ? "-" : "";
	s += std::to_string(chunks.back());

	for (size_t i = chunks.size() - 1; i-- > 0;) // size - 2부터
	{
		string part = std::to_string(chunks[i]);
		s += string(9 - part.size(), '0') + part;
		// 9 자리 중 공백을 0으로 채움
	}
	return s;
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

bool BigInt::operator<(const BigInt& other) const
{
	if (neg != other.neg) return neg;

	int c = cmpMag(vec, other.vec);
	return neg ? (c > 0) : (c < 0);
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







BigInt BigInt::operator*(const BigInt& other) const
{
	BigInt out;
	out.vec = mulMag(vec, other.vec);
	out.neg = out.vec.empty() == false && neg != other.neg;

	return out;
}

BigInt BigInt::operator<<(const size_t bits) const
{
	BigInt out;
	out.vec = shiftLeftMag(vec, bits);
	out.neg = out.vec.empty() == false && neg;
	return out;
}

BigInt BigInt::operator>>(const size_t bits) const
{
	BigInt out;
	out.vec = shiftRightMag(vec, bits);
	out.neg = out.vec.empty() == false && neg;
	return out;
}

int BigInt::cmpMag(const vector<half>& a, const vector<half>& b)
{
	// a 가 크면 1 리턴, b가 크면 -1 리턴, 같으면 0

	if (a.size() != b.size())
	{
		return a.size() < b.size() ? -1 : 1;
	}
	for (size_t i = a.size(); i-- > 0;) 
		// size_t 는 unsigned 이니, (2) 처럼
		// (2) 비교 가 바로 작동하니, 사실상 vec.size() - 1이 할당 !! 
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
		out.push_back((half)(sum & MASK_LIMB));
		carry = sum >> LIMB_SIZE;
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
			d += (sword)1 << LIMB_SIZE; // 이번 자리에서 한블록 빌려옴
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

vector<half> BigInt::mulMag(const vector<half>& a, const vector<half>& b)
{
	if (a.empty() || b.empty()) return {};

	vector<half> out(a.size() + b.size(), 0);

	// 마치 산술로 곱샘을 푸는 것 같다
	for (size_t i = 0; i < a.size(); ++i)
	{
		word carry = 0;
		for (size_t j = 0; j < b.size(); ++j)
		{
			word t = (word)a[i] * b[j] + out[i + j] + carry;
			out[i + j] = (half)(t & MASK_LIMB);
			carry = t >> LIMB_SIZE;
		}

		// 이 때는 아직 out[i + b.size()] == 0 인 상태
		out[i + b.size()] += (half)carry; 
	}

	while (out.empty() == false && out.back() == 0)
	{
		out.pop_back();
	}

	return out;
}

vector<half> BigInt::divModSmall(const vector<half>& a, half d, half& rem)
	// ※ 제수(나누는 수)가 BLOCK_SIZE 미만일 때만 사용가능 (빠르지만 제수 크기 한계)
{
	vector<half> q(a.size(), 0);

	// 이것도 산술로 푸는 나눗샘 같네
	word res = 0;
	for (size_t i = a.size(); i-- > 0;)
	{
		word curr = (res << LIMB_SIZE) | a[i];
		q[i] = (half)(curr / d);
		res = curr % d;
	}
	rem = (half)res;

	while (q.empty() == false && q.back() == 0)
	{
		q.pop_back();
	}

	return q;
}

vector<half> BigInt::shiftLeftMag(const vector<half>& a, size_t bits)
{
	if (a.empty()) return {};
	size_t limbShift = bits / LIMB_SIZE;
	int bitShift = bits % LIMB_SIZE;

	vector<half> out(limbShift, 0); 
	// 아래 LIMB 들은 0 으로 push_back == limbShift
	out.reserve(limbShift + a.size() + 1);

	// 잔차인 bitShift
	word carry = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		word t = ((word)a[i]) << bitShift | carry;
		out.push_back((half)(t & MASK_LIMB));
		carry = t >> LIMB_SIZE;
	}

	if (carry)
	{
		out.push_back((half)carry);
	}
	return out;
}

vector<half> BigInt::shiftRightMag(const vector<half>& a, size_t bits)
{
	size_t limbShift = bits / LIMB_SIZE;
	int bitShift = bits % LIMB_SIZE;
	if (limbShift >= a.size()) return {};

	vector<half> out;
	out.reserve(a.size() - limbShift);
	for (size_t i = limbShift; i < a.size(); ++i)
	{
		word twoLimb = a[i];

		if (i + 1 < a.size()) // 높은 림브의 값을 다 가져와, 2개의 림브로 만든다
		{
			twoLimb |= (word)a[i + 1] << LIMB_SIZE;
		}

		out.push_back((half)((twoLimb >> bitShift) & MASK_LIMB));
		// 오른쪽으로 시프팅 후 적용
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

			(b1 * b2).printNum();
			(b1 * b4).printNum();

			printf("\n\n");
		};

	word n1 = UINT64_MAX;
	func(n1, n1);

	word n2 = 3;
	word n3 = 4;
	func(n2, n3);
}
