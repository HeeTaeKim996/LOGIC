#include "AssymetricTest.h"




#pragma once
#include <type_traits>
#include <stdio.h>
#include <functional>
#include "src/IM_mod.h"
#include "src/IM_int.h"


void AssymetricTest::test()
{
	DiffieHellman();

	RSA();
	
	EIGammal();
}

void AssymetricTest::DiffieHellman()
{
	// 디피 헬만 테스트
	auto func = [](uint64 m, uint64 g, uint64 a, uint64 b)
		{
			// m, g 를 공유. 
			// 서로 g^a, g^b (%m) 를 각자 만든 후 서로에게 송신
			// 서로 받은 값에 각자 ^b, ^a 하여,
			// g^ab = g^ba (%m) 를 키로 사용

			uint64 x = mpow(g, a, m);
			uint64 y = mpow(g, b, m);

			uint64 ab = mpow(x, b, m);
			uint64 ba = mpow(y, a, m);

			if (ab == ba)
			{
				printf("디피헬멘 키교환 성공\n");
			}
			else
			{
				printf("디피헬멘 키교환 실패\n");
			}
		};


	uint64 m1 = 23;
	uint64 g1 = 5;

	uint64 a1 = 6;
	uint64 b1 = 15;

	func(m1, g1, a1, b1);

	uint64 m2 = 4294967087;
	uint64 g2 = 5;

	uint64 a2 = 1234567891;
	uint64 b2 = 987654321;

	func(m2, g2, a2, b2);

}

void AssymetricTest::RSA()
{
	auto func = [](uint64 p, uint64 q, uint64 E, uint64 m) -> void
		{
			// ■ 개인키 사용자(생성자)의 키생성 과정
			// 
			// 1) p, q 를 난수로 생성후, p, q가 소수인지 판별.
			//    소수가 아니라면, 1)부터 반복
			// 2) p, q 로부터 L = lcm(p, q) 를 생성
			// 3) 관용적으로 사용되는 E(2^16 + 1 == 33567) 와 L 이
			//    서로소인지 판별, 서로소가 아니라면, 1) 부터 반복
			// 4) 확장 유클리드를 사용하여, E * D % L 을 만족하는 D 도출
			//
			// ※ 실전에서는 위처럼 키들을 만든다 한다.
			//    현 예시는 사전에 확인된 소수인 p, q와, 
			//    사전에 L 과 서로소인 게 확인된 E를 
			//    변수로 받아 처리했다


			uint64 N = p * q;
			uint64 L = lcm(p - 1, q - 1);
			uint64 D = extendedEuclidian(E, L);

			uint64 c = mpow(m, E, N); // 공개키로 평문을 암호화
			uint64 d = mpow(c, D, N); // 개인키로 암호를 복호화

			if (d == m)
			{
				printf("RSA 암복호화 성공\n");
			}
			else
			{
				printf("RSA 암복호화 실패\n");
			}
		};

	// RSA 테스트
	uint64 p = 61;
	uint64 q = 53;
	uint64 E = 17;
	uint64 m = 1234;
	func(p, q, E, m);
}

void AssymetricTest::EIGammal()
{
	auto func = [](uint64 p, uint64 g, uint64 x, uint64 y)
		{
			// 수신자 p, g, x 에 기반해 h를 만든 후, 송신자에게 
			// 공개키인 (h, g, p) 를 전송. (개인키는 x)

			uint64 h = mpow(g, x, p);



			// 송신자는 공개키를 받은 후, c1, c2(평문과 결합) 를 만들어, 
			// 수신자에게 전송
			uint64 m = 10;
			


			uint64 c1 = mpow(g, y, p);
			uint64 c2 = m * mpow(h, y, p) % p;

			// 수신자는 c1 에 ^(-x) 하여 t를 만든 후 C2 와 곱하여, 평문 m을 도출 
			// (t 도출 실제 코드는 확장 유클리드로 invC1 구한후, mpow)
			uint64 invC1 = extendedEuclidian(c1, p);
			uint64 t = mpow(invC1, x, p);
			uint64 d5 = t * c2 % p;

			if (m == d5)
			{
				printf("EIGammal 암복호화 성공\n");
			}
			else
			{
				printf("EIGammal 암복호화 실패\n");
			}
		};

	
	uint64 p = 23;
	uint64 g = 5;

	uint64 x = 6;
	uint64 y = 8;
	
	func(p, g, x, y);
}

