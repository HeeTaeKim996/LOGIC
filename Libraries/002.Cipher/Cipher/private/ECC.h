#pragma once
#include "SizeType.h"
#include <stdio.h>


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


	void print()
	{
		printf("P(%d,%d)\n", x, y);
	}

	int32 x = 0;
	int32 y = 0;
	bool inf = false; // true 이면 무한 원점 0 (x, y 는 무시됨)
};


class ECC
{
public:
	ECC(int32 inP, int32 inA, int32 inB) : p(inP), a(inA), b(inB) {}

public:
	Point add(const Point& A, const Point& B);
	Point scalarMulNaively(const Point& A, int32 k);
	Point scalarMul(Point A, int k);
	bool isOnCurve(const Point& A);
	Point neg(const Point& A);


private:
	int32 add(int64 a, int64 b) { return norm(a + b); }
	int32 sub(int64 a, int64 b) { return norm(a - b); }
	int32 mul(int64 a, int64 b) { return norm(a * b); }

	int32 inv_mul(int32 a);
	int32 norm(int64 a);



private:
	// 곡선 파라미터 : y^2 = x^3 + a*x + b ( mod p )
	int32 p;
	int32 a; // 0 <= a < p
	int32 b;


public:
	static void test();
};