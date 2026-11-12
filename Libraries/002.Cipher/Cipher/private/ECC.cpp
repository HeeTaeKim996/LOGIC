#include "ECC.h"
#include <cassert>



int32 ECC::inv_mul(int32 a)
{
	assert(a != 0);
	if (a == 1) return 1;


	int32 r0 = p;
	int32 r1 = a;

	int32 t0 = 0;
	int32 t1 = 1;

	while (r1)
	{
		int32 q = r0 / r1;

		int32 r2 = r0 - q * r1;
		r0 = r1;
		r1 = r2;

		int32 t2 = t0 - q * t1;
		t0 = t1;
		t1 = t2;
	}

	if (t0 < 0) t0 += p;
	return t0;
}

int32 ECC::norm(int64 a)
{
	a %= p; // a 가 음수여도 %= 처리됨

	if (a < 0) a += p;
	return (int32)a;
}

Point ECC::add(const Point& A, const Point& B)
{
	// 1) O + B = B, A + O = A
	if (A.inf) return B;
	if (B.inf) return A;

	int64 x1 = A.x;
	int64 y1 = A.y;

	int64 x2 = B.x;
	int64 y2 = B.y;

	// 위 아래 쌍 -> O 
	if (x1 == x2 && madd(y1, y2) == 0) 
	// x1 == x2 인 경우는 y1 == y2 or y1 == -y2 둘 뿐 (수리정리에 정리)
	// 따라서 madd(y1, y2) == 0 인 경우는 y1 == -y2 로, 기울기가 무한이
	// 되기에, Infinity ( O ) 처리
	// y1 != -y2 라면 y1 == y2 이므로, 아래의 접선 으로 처리됨
	{
		return Point::Infinity();
	}



	int64 inc = A == B ? 
		mmul(
		madd(mmul(3, mmul(x1, x1)), a),
		inv_mul(mmul(2, y1))
		)
		: 
		mmul(msub(y1, y2), inv_mul(msub(x1, x2)));

	int64 x3 = msub(mmul(inc, inc), madd(x1, x2));

	int64 y3 = msub(mmul(inc, msub(x1, x3)), y1);

	return Point(x3, y3);
}
