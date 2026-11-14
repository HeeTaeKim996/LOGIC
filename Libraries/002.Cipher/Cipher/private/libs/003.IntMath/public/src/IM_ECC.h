#pragma once
#include "IM_BigInt.h"
#include <stdio.h>


struct __declspec(dllexport) Point
{
	Point() = default;
	Point(BigInt inX, BigInt inY) : x(inX), y(inY) {}
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
		printf("--Point--\n");
		x.printNum();
		y.printNum();
	}

	BigInt x = 0;
	BigInt y = 0;
	bool inf = false; // true 이면 무한 원점 0 (x, y 는 무시됨)
};


class __declspec(dllexport) ECC
{
public:
	ECC(BigInt inP, BigInt inA, BigInt inB) : p(inP), a(inA), b(inB) {}

public:
	Point add(const Point& A, const Point& B);
	Point scalarMulNaively(const Point& A, BigInt k);
	Point scalarMul(Point A, BigInt k);
	bool isOnCurve(const Point& A);
	Point neg(const Point& A);


private:
	BigInt add(BigInt a, BigInt b) { return norm(a + b); }
	BigInt sub(BigInt a, BigInt b) { return norm(a - b); }
	BigInt mul(BigInt a, BigInt b) { return norm(a * b); }

	BigInt inv_mul(BigInt a);
	BigInt norm(BigInt a);



private:
	// 곡선 파라미터 : y^2 = x^3 + a*x + b ( mod p )
	BigInt p;
	BigInt a; // 0 <= a < p
	BigInt b;


public:
	static void test();
};