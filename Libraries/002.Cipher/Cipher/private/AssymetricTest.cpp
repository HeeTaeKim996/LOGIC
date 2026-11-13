#include "AssymetricTest.h"




#pragma once
#include <type_traits>
#include <stdio.h>
#include <functional>
#include "src/IM_mod.h"
#include "src/IM_int.h"
#include "ECC.h"
#include <cassert>

void AssymetricTest::DiffieHellman()
{
	// 디피 헬만 테스트
	auto func = [](int64 m, int64 g, int64 a, int64 b)
		{
			// m, g 를 공유. 
			// 서로 g^a, g^b (%m) 를 각자 만든 후 서로에게 송신
			// 서로 받은 값에 각자 ^b, ^a 하여,
			// g^ab = g^ba (%m) 를 키로 사용

			int64 x = mpow(g, a, m);
			int64 y = mpow(g, b, m);

			int64 ab = mpow(x, b, m);
			int64 ba = mpow(y, a, m);

			if (ab == ba)
			{
				printf("디피헬멘 키교환 성공\n");
			}
			else
			{
				printf("디피헬멘 키교환 실패\n");
			}
		};


	int64 m1 = 23;
	int64 g1 = 5;

	int64 a1 = 6;
	int64 b1 = 15;

	func(m1, g1, a1, b1);

	int64 m2 = 4294967087;
	int64 g2 = 5;

	int64 a2 = 1234567891;
	int64 b2 = 987654321;

	func(m2, g2, a2, b2);

}

void AssymetricTest::RSA()
{
	auto func = [](int64 p, int64 q, int64 E, int64 m) -> void
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


			int64 N = p * q;
			int64 L = lcm(p - 1, q - 1);
			int64 D = extendedEuclidian(E, L);

			int64 c = mpow(m, E, N); // 공개키로 평문을 암호화
			int64 d = mpow(c, D, N); // 개인키로 암호를 복호화

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
	int64 p = 61;
	int64 q = 53;
	int64 E = 17;
	int64 m = 1234;
	func(p, q, E, m);
}

void AssymetricTest::EIGammal()
{
	auto func = [](int64 p, int64 g, int64 x, int64 y)
		{
			// 수신자 p, g, x 에 기반해 h를 만든 후, 송신자에게 
			// 공개키인 (h, g, p) 를 전송. (개인키는 x)

			int64 h = mpow(g, x, p);



			// 송신자는 공개키를 받은 후, c1, c2(평문과 결합) 를 만들어, 
			// 수신자에게 전송
			int64 m = 10;
			


			int64 c1 = mpow(g, y, p);
			int64 c2 = m * mpow(h, y, p) % p;

			// 수신자는 c1 에 ^(-x) 하여 t를 만든 후 C2 와 곱하여, 평문 m을 도출 
			// (t 도출 실제 코드는 확장 유클리드로 invC1 구한후, mpow)
			int64 invC1 = extendedEuclidian(c1, p);
			int64 t = mpow(invC1, x, p);
			int64 d5 = t * c2 % p;

			if (m == d5)
			{
				printf("EIGammal 암복호화 성공\n");
			}
			else
			{
				printf("EIGammal 암복호화 실패\n");
			}
		};

	
	int64 p = 23;
	int64 g = 5;

	int64 x = 6;
	int64 y = 8;
	
	func(p, g, x, y);
}



























void AssymetricTest::ECDH()
{
	auto func = [](ECC ecc, Point G,
		int32 n, int32 a, int32 b) -> void
		{
			// 각자 공개키 생성
			Point A = ecc.scalarMul(G, a);
			Point B = ecc.scalarMul(G, b);

			// 공개키 교환 뒤 공유키를 각자 생성
			Point bA = ecc.scalarMul(A, b);
			Point aB = ecc.scalarMul(B, a);

			if (bA == aB && bA.inf == aB.inf)
			{
				printf("ECDH 키 교환 성공\n");
			}
			else
			{
				printf("ECDH 키 교환 실패\n");
			}
		};


	func(ECC(17, 2, 2), Point(5, 1), 19, /*privA*/ 7, /*privB*/ 11);
}

void AssymetricTest::ECEIGammal()
{
	auto func = [](ECC ecc, Point G, int32 n, int32 e, int32 r,
		Point M)-> void
		{
			assert(ecc.isOnCurve(M));
			// M 이 ecc 곡선 위의 점이어야 하는 까다로운 조건이 있기에,
			// ECEIGammal 은 자주 쓰이지 않는다.
			
			// 의외로 ECC 를 직접 활용하여 메세지를 암복화하는 경우는 
			// 흔치 않고, ECC + 대칭키 로 섞어 사용하거나, RSA 를 사용

			// 수신자(개인키 보유) 전송
			Point E = ecc.scalarMul(G, e); // e G


			// 송신자(공개키 공유) 전송
			Point C1 = ecc.scalarMul(G, r); // r G
			Point C2 = ecc.add(M, ecc.scalarMul(E, r)); // M + r e G


			// 수신자 복호화
			Point S = ecc.scalarMul(C1, e); // e r G
			Point D = ecc.add(C2, ecc.neg(S));
			// M + r e G + ( - [ e R G ] )  == M

			if (D == M && D.inf == M.inf)
			{
				printf("ECEIGammal 암복호화 성공\n");
			}
			else
			{
				printf("ECEIGammal 암복호화 실패\n");
			}
		};

	func(ECC(17, 2, 2), Point(5, 1),
		19, 7, 3, Point(10, 6));
}




void AssymetricTest::ECDSA()
{
	auto func = [](ECC ecc, Point G, 
		int64 p, int64 e, int64 m, int64 r) -> void
		{
			
			// 서명 (서명에 사용되는 e, r 는 비공개)
			// ※ e 는 서명자가 영구적으로 들고 있는 비밀키로, E도 상시
			// ※ r 은 서명할 때마다 새로 생성되는 난수로, R도 서명 때 생성
			Point E = ecc.scalarMul(G, e);
			Point R = ecc.scalarMul(G, r);

			int64 Rx = R.x % p;
			int64 inv_r = extendedEuclidian(r, p);
			
			int64 s = mmul(
				inv_r, madd(m, mmul(Rx, e, p), p), p);

			
			// 검증자에게 전달 
			// (m(메세지), 서명(E, s, Rx) 이 전달)
			// ( 원점 G, 모듈려 p는 사전 공유 )
			int64 inv_s = extendedEuclidian<int64>(s, p);
			int64 u1 = mmul(m, inv_s, p);
			int64 u2 = mmul(Rx, inv_s, p);

			Point X = ecc.add(
				ecc.scalarMul(G, u1),
				ecc.scalarMul(E, u2)
			);


			// X == r G = R 이고(이론), 
			// X.x == R.x == Rx  임이 검증되므로,
			// X == R or X == -R (x가 같아도 y^2 .. 에 의해 y는 반전가능)
			// 왜 X == -R 이어도 검증이 되는지는 이해가 안되지만, 이것도 
			// 검증이라 함
			// 아무튼 이렇게 해서 메세지 m이 유효함을 검증
			if (X.inf == false && X.x % p == Rx)
			{
				printf("ECDSA 키 검증 성공\n");
			}
			else
			{
				printf("ECDSA 키 검증 실패\n");
			}
		};

	func(ECC(17, 2, 2), Point(5, 1), 
		19, 7, 10, 3);
}

