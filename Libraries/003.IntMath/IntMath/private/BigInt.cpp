#include "src//BigInt.h"
#include <cassert>

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

BigInt::BigInt(half num)
{
	if (num == 0) return;

	vec.push_back(num & MASK_LIMB);
	if (num >> LIMB_SIZE)
	{
		vec.push_back(num >> LIMB_SIZE);
	}
}

BigInt::BigInt(const string& s)
{
	bool isNeg = (s[0] == '-');
	neg = isNeg;
	
	BigInt r;
	BigInt ten(10);
	for (size_t i = isNeg ? 1 : 0; i < s.size(); ++i)
	{
		r = r * ten + BigInt((half)s[i] - '0');
	}
	vec = r.vec;
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

BigInt BigInt::operator/(const BigInt& b) const
{
	assert(b.vec.empty() == false);

	BigInt q;
	BigInt r;

	divModBig(b, q, r);
	return q;
}

BigInt BigInt::operator%(const BigInt& b) const
{
		assert(b.vec.empty() == false);

	BigInt q;
	BigInt r;

	divModBig(b, q, r);
	return r;
}

BigInt BigInt::operator&(const BigInt& other) const
{
	assert(neg == false && other.neg == false); // 양수 전용

	BigInt out;

	size_t n = min(vec.size(), other.vec.size());
	out.vec.resize(n);

	for (size_t i = 0; i < n; ++i)
	{
		out.vec[i] = vec[i] & other.vec[i];
	}

	while (out.vec.empty() == false && out.vec.back() == 0)
	{
		out.vec.pop_back();
	}

	return out;
}

BigInt BigInt::operator|(const BigInt& other) const
{
	assert(neg == false && other.neg == false); // 양수 전용
	
	BigInt out;

	size_t n = max(vec.size(), other.vec.size());
	out.vec.resize(n);

	for (size_t i = 0; i < n; ++i)
	{
		half a = (i < vec.size()) ? vec[i] : 0;
		half b = (i < other.vec.size()) ? other.vec[i] : 0;
		out.vec[i] = a | b;
	}

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
		q = divModSmallMag(q, CHUNK, rem);
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

size_t BigInt::bitLength() const
{
	if (vec.empty()) return 0;

	size_t lastLimbBits = 0;
	half temp = vec[vec.size() - 1];
	while(temp)
	{
		lastLimbBits++;
		temp >>= 1;
	}

	return (vec.size() - 1) * LIMB_SIZE + lastLimbBits;
}




void BigInt::divModBig(const BigInt& B, BigInt& Q, BigInt& R) const
{
	divModBigMag(*this, B, Q.vec, R.vec);

	Q.neg = Q.vec.empty() == false && (neg != B.neg);
	R.neg = R.vec.empty() == false && neg;
}

void BigInt::divModSmall(const BigInt& b, BigInt& q, BigInt& r) const
{
	assert(b.vec.size() == 1);

	half res;
	q.vec = divModSmallMag(vec, b.vec[0], res);

	r.vec.clear();
	if (res)
	{
		r.vec.push_back(res);
	}
	

	q.neg = q.vec.empty() == false && neg != b.neg;
	r.neg = r.vec.empty() == false && neg;
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

vector<half> BigInt::divModSmallMag(const vector<half>& a, half d, half& rem)
	// ※ 제수(나누는 수)가 BLOCK_SIZE 미만일 때만 사용가능 (빠르지만 제수 크기 한계)
{
	vector<half> q(a.size(), 0);

	// 이것도 산술로 푸는 나눗샘 같다
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






















/*
		CPU 나눗샘 연산에서 리스토링 알고리즘을 모방했다

		■ 리스토링 알고리즘 (하드웨어)
		제수 B 와 피제수 A라 할 때,
		R - A 의 연속된 비트를 만든다. ※ R은 제수 B와 같은 비트수의 0으로 시작

		R-A << 1 하고,
		R 부분이 >= B 라면, R -= B, A 의 LSB 에 1 입력
		(R 부분이 < B 라면 아무것도 안함)

		위 과정을 A의 비트수만큼 반복
		반복후, R 부문에는 잔차가, A 부문에는 몫이 배치됨

		■ 아래 코드
		R 부문이 res 에 대응,

		A 부문은 2개로 나뉘어지는데,
		R에 시프팅으로 비트를 공급하는 A 의 역할은 a 가,
		LSB 에 1 이 찍히는 A의 역할은 quot가 담당한다



		※
		번외로 CPU는 리스토링 알고리즘이 아닌 논리스토링 알고리즘을 사용한다.
		반면 이를 소프트웨어로 구현할 때는, 아래 설명에서 나온 -=B, +=B 비용이
		R - B 비교 비용보다 비싸기 때문에, 의미가 없기에, 리스토링 알고리즘을 사용


		■ 논리스토링 ( 설명만 남김. 여기 알고리즘에서 안씀 )

		제수 B 와 피제수 A 라 할 때,
		R - A 의 연속된 비트를 만든다. ※ R은 제수 B와 같은 비트수의 0 으로 시작

		R-A << 1 하고,
		1) A부분
		R부분이 < 0 이라면, 생략
		R부분이 >= 0 이라면, A의 LSB에 1
		2) R부분
		R부분이 < 0 이라면, R += B
		R부분이 >= 0 이라면, -= B

		이를 A 의 비트수만큼 반복

		2) 의 의미를 풀어보면, R-A << 1에서, R << 1부분은 R *= 2 를 의미.
		따라서 R 이 -B 이라 했다면, -2B 을 만드는 것.
		이후 += B 를 하니 -B 가 유지됨
		반면 R-A << 1 에서 R쪽으로 A에서 넘어오는 부분이 있는데, 이건 넘어오고 있는
		피제수 를 의미. 이를 a 라 부른다면,
		2) 의 반복은 -M + a 구조를 유지하는 것. 여기서 M 은 -2M + M 으로 유지되고,
		a 는 시프팅 의 의미로서 점차 커지는 구조로 유지됨.

		R 부분이  < 0 이라면, a < B 를 의미하므로, +=B 하며 기존 상태를 유지

		R 부분이 >= 0 이라면, a >= B 를 의미하므로, -= B 하여, 해당 비트 기준
		'나누기' 가 적용 ( -B + (a - B) )되어, A 의 LSB 에 1이 입력됨.

		회전이 끝난 후 R 자리에는 -B + a 구조가 유지됐으므로, += B 처리.
		R 자리는 a(최종 나머지) 가,
		A 자리에는 몫 이 배치됨
	*/
void BigInt::divModBigMag(const BigInt& A, const BigInt& B, 
	vector<half>& quot, vector<half>& res)
{
	const vector<half>& a = A.vec;
	const vector<half>& b = B.vec;

	quot.clear();
	res.clear();


	if (cmpMag(a, b) < 0) // a < b 이면 몫 = 0, 나머지 = a
	{
		res = a;
		return;
	}

	size_t n = A.bitLength();

	quot.assign(a.size(), 0);

	for (size_t i = n; i-- > 0;)
	{
		res = shiftLeftMag(res, 1);
		if ((a[i / LIMB_SIZE] >> (i % LIMB_SIZE)) & 1)
		{
			if (res.empty())
			{
				res.push_back(1);
			}
			else
			{
				res[0] |= 1;
			}
		}


		if (cmpMag(res, b) >= 0)
		{
			res = subMag(res, b);
			quot[i / LIMB_SIZE] |= (half)1 << (i % LIMB_SIZE);
		}
	}

	while (quot.empty() == false && quot.back() == 0)
	{
		quot.pop_back();
	}
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
	auto eq = [](const BigInt& got, const char* expect)
		{ if (got.toString() != expect) 
			printf("FAIL: got %s, expect %s\n", got.toString().c_str(), 
				expect
		); 
		};

	BigInt m("18446744073709551615");   // 2^64 - 1
	eq(m + m, "36893488147419103230");
	eq(m * m, "340282366920938463426481119284349108225");
	eq(BigInt("5") - BigInt("17"), "-12");
	eq(BigInt("100") / BigInt("7"), "14");   
	eq(BigInt("100") % BigInt("7"), "2");
	eq(m / BigInt("3"), "6148914691236517205");
	eq(BigInt("-17") / BigInt("5"), "-3");   
	eq(BigInt("-17") % BigInt("5"), "-2");   // C++ 규칙
	BigInt a("115792089237316195423570985008687907853269984665640564039457584007913129639935");
	BigInt b("340282366920938463463374607431768223801");
	eq(a / b, "340282366920938463463374607431768199111");   
	eq(a % b, "152399024");




	// 256비트 나눗셈 검증: a / b, a % b, 그리고 q*b + r == a
	auto chk = [](const char* sa, const char* sb, const char* sq, const char* sr)
		{
			BigInt a(sa), b(sb);
			BigInt q = a / b, r = a % b;
			if (q.toString() != sq) printf("FAIL div: got %s, expect %s\n", q.toString().c_str(), sq);
			if (r.toString() != sr) printf("FAIL mod: got %s, expect %s\n", r.toString().c_str(), sr);
			if (q * b + r != a)     printf("FAIL identity (q*b+r != a)\n");
		};

	chk("115792089237316195423570985008687907853269984665640564039457584007913129639935", "340282366920938463463374607431768223801", "340282366920938463463374607431768199111", "152399024");
	chk("115792089237316195423570985008687907853269984665640564039457584007913129639935", "57896044618658097711785492504343953926634992332820282019728792003956564819975", "1", "57896044618658097711785492504343953926634992332820282019728792003956564819960");
	chk("71563446777022297856526126342750658392501306254664949883333486863006233104021", "41890673666460448982159197293325984121", "1708338408372725718657518656537802641710", "458576924190192092429474546920817111");
	chk("90942894222941581070058735694432465663348344332098107489693037779484723616546", "267257146016241686964920093290467695825", "340282366920938463463374607431768211456", "54510315125507421606219084379035245346");
	chk("-100720434724302814903610305981132438410696627821163241311670897279490076600696", "24197857203266734864793317670504947441", "-4162369993269712151472098540768050461185", "-4078602233972714460526385605841023111");
	chk("100720434724302814903610305981132438410696627821163241311670897279490076600696", "-1361129467683753853853498429727072845923", "-73997688769239326174540292765690557302", "839493251872625660895176611557028020950");
	chk("57896044618658097711785492504343953926975274699741220483192166611388333031424", "340282366920938463463374607431768211456", "170141183460469231731687303715884105729", "0");



	printf("done\n");
}
