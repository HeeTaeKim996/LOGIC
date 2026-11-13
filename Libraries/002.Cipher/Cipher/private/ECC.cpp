#include "ECC.h"
#include <cassert>







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
	if (x1 == x2 && add(y1, y2) == 0)
		// x1 == x2 인 경우는 y1 == y2 or y1 == -y2 둘 뿐 (수리정리에 정리)
		// 따라서 madd(y1, y2) == 0 인 경우는 y1 == -y2 로, 기울기가 무한이
		// 되기에, Infinity ( O ) 처리
		// y1 != -y2 라면 y1 == y2 이므로, 아래의 접선 으로 처리됨
	{
		return Point::Infinity();
	}



	int64 inc = A == B ?
		mul(
			add(mul(3, mul(x1, x1)), a),
			inv_mul(mul(2, y1))
		)
		:
		mul(sub(y1, y2), inv_mul(sub(x1, x2)));

	int64 x3 = sub(mul(inc, inc), add(x1, x2));

	int64 y3 = sub(mul(inc, sub(x1, x3)), y1);

	return Point(x3, y3);
}

Point ECC::scalarMulNaively(const Point& A, int32 k)
{
	Point out = Point::Infinity();

	while (k-- > 0)
	{
		out = add(out, A);
	}

	return out;
}

Point ECC::scalarMul(Point A, int k)
{
	Point out = Point::Infinity();

	while (k)
	{
		if (k & 1)
		{
			out = add(out, A);
		}

		A = add(A, A);
		k >>= 1;
	}

	return out;
}

bool ECC::isOnCurve(const Point& A)
{
	if (A.inf) return true;

	int64 x = A.x;
	int64 y = A.y;

	return mul(y, y) ==
		(add(add(mul(x, mul(x, x)), mul(a, x)), b));
}

Point ECC::neg(const Point& A)
{
	if (A.inf) return A;

	return Point(A.x, sub(0, A.y));
}































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







void ECC::test()
{
	if(0)
	{
		ECC ecc(17, 0, 7);

		ecc.add(Point(1, 12), Point(6, 11)).print();
		ecc.add(Point(5, 8), Point(5, 9)).print();
		ecc.add(Point(8, 0), Point(8, 0)).print();
		ecc.add(Point::Infinity(), Point(5, 8)).print();
		ecc.add(Point(5, 8), Point::Infinity()).print();
	}

	if(1)
	{
		ECC ecc(17, -3, 5);

		ecc.scalarMulNaively(Point(5, 8), 12).print();
		ecc.scalarMulNaively(Point(5, 8), 13).print();


		ecc.scalarMul(Point(5, 8), 12).print();
		ecc.scalarMul(Point(5, 8), 13).print();
	}

}