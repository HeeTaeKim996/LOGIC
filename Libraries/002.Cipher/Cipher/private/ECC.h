#pragma once
#include "SizeType.h"



struct Point
{
	Point() = default;
	Point(int32 inX, int32 inY) : x(inX), y(inY) {}
	bool operator ==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}
	static Point Infinity()
	{
		Point O;
		O.inf = true;
		return O;
	};


	int32 x = 0;
	int32 y = 0;
	bool inf = false; // true 이면 무한 원점 0 (x, y 는 무시됨)
};


class ECC
{
	int32 madd(int64 a, int64 b) { return norm(a + b); }
	int32 msub(int64 a, int64 b) { return norm(a - b); }
	int32 mmul(int64 a, int64 b) { return norm(a * b); }

	int32 inv_mul(int32 a);
	int32 norm(int64 a);

	Point add(const Point& A, const Point& B);

private:
	// 곡선 파라미터 : y^2 = x^3 + a*x + b ( mod p )
	int32 p;
	int32 a; // 0 <= a < p
	int32 b;
};