#include "AssymetricTest.h"




#pragma once
#include <type_traits>
#include <stdio.h>
#include <functional>
#include "src/IM_mod.h"
#include "src/IM_int.h"
//#include "ECC.h"
#include <cassert>
#include "src/IM_BigInt.h"
#include "src/IM_ECC.h"



#define HEAVY_256_TEST 
// ※ ECC 계열은 조심. 두번째 func들을 256 비트 연산하는데 각각 최소 1분 걸렸음


void AssymetricTest::DiffieHellman()
{
	// 디피 헬만 테스트
	auto func = [](BigInt m, BigInt g, BigInt a, BigInt b)
		{
			// m, g 를 공유. 
			// 서로 g^a, g^b (%m) 를 각자 만든 후 서로에게 송신
			// 서로 받은 값에 각자 ^b, ^a 하여,
			// g^ab = g^ba (%m) 를 키로 사용

			BigInt x = mpow(g, a, m);
			BigInt y = mpow(g, b, m);

			BigInt ab = mpow(x, b, m);
			BigInt ba = mpow(y, a, m);

			if (ab == ba)
			{
				printf("디피헬멘 키교환 성공\n");
			}
			else
			{
				printf("디피헬멘 키교환 실패\n");
			}
		};


	BigInt m1 = 23;
	BigInt g1 = 5;

	BigInt a1 = 6;
	BigInt b1 = 15;

	func(m1, g1, a1, b1);


#ifdef HEAVY_256_TEST
	// 255비트 소수 (2^255 - 19, Curve25519 의 소수)
	BigInt m4("57896044618658097711785492504343953926634992332820282019728792003956564819949");
	BigInt g4("5");
	BigInt a4("90942894222941581070058735694432465663348344332098107489693037779484723616546");
	BigInt b4("71563446777022297856526126342750658392501306254664949883333486863006233104021");

	func(m4, g4, a4, b4);
#endif
}

void AssymetricTest::RSA()
{
	auto func = [](BigInt p, BigInt q, BigInt E, BigInt m) -> void
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


			BigInt N = p * q;
			BigInt L = lcm(p - 1, q - 1);
			BigInt D = extendedEuclidian(E, L);

			BigInt c = mpow(m, E, N); // 공개키로 평문을 암호화
			BigInt d = mpow(c, D, N); // 개인키로 암호를 복호화

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
	BigInt p = 61;
	BigInt q = 53;
	BigInt E = 17;
	BigInt m = 1234;
	func(p, q, E, m);


#ifdef HEAVY_256_TEST
	BigInt p2("72701006939494238243595020906218961312501607072826208109892606521075242172797");
	BigInt q2("104618889389031091741697867029934545270541378915302818511401062929801834355683");
	BigInt E2("65537");   // 2^16 + 1, L 과 서로소임을 확인함
	BigInt m2("46705349425275964047091750287422271149780248886835835895293305932645477301988");
	func(p2, q2, E2, m2);
#endif
}

void AssymetricTest::EIGammal()
{
	auto func = [](BigInt p, BigInt g, BigInt x, BigInt y)
		{
			// 수신자 p, g, x 에 기반해 h를 만든 후, 송신자에게 
			// 공개키인 (h, g, p) 를 전송. (개인키는 x)

			BigInt h = mpow(g, x, p);



			// 송신자는 공개키를 받은 후, c1, c2(평문과 결합) 를 만들어, 
			// 수신자에게 전송
			BigInt m = 10;
			


			BigInt c1 = mpow(g, y, p);
			BigInt c2 = m * mpow(h, y, p) % p;

			// 수신자는 c1 에 ^(-x) 하여 t를 만든 후 C2 와 곱하여, 평문 m을 도출 
			// (t 도출 실제 코드는 확장 유클리드로 invC1 구한후, mpow)
			BigInt invC1 = extendedEuclidian(c1, p);
			BigInt t = mpow(invC1, x, p);
			BigInt d5 = t * c2 % p;

			if (m == d5)
			{
				printf("EIGammal 암복호화 성공\n");
			}
			else
			{
				printf("EIGammal 암복호화 실패\n");
			}
		};

	
	BigInt p = 23;
	BigInt g = 5;

	BigInt x = 6;
	BigInt y = 8;
	
	func(p, g, x, y);


#ifdef HEAVY_256_TEST
	BigInt p2("115792089237316195423570985008687907853269984665640564039457584007913129639747");
	BigInt g2("5");
	BigInt x2("54402719445738542648830185211217922203494088715465318631540275427069301805885");
	BigInt y2("42716311098472971311653471837678667036500068065288240843043180787528881192220");
	func(p2, g2, x2, y2);
#endif
}

void AssymetricTest::Rabin()
{
	auto func = [](BigInt p, BigInt q, BigInt m) -> void
		{
			// 수신자 (개인키 보유자) 는 N 을 만들어 송신자에게 전송
			BigInt N = p * q;
			
			// 송신자는 m ^ 2 % N 으로 c를 만들어 수신자에게 회신
			BigInt c = mmul(m, m, N); // ※ 메세지의 '표시' 는 12 의 배수로 한다





			// 수신자는 rp, rq, u, v 를 도출후, y0~y3 을 도출하여,
			// y0~y3 을 검사해서 표식 이 있는지 확인
			BigInt rep = (p + BigInt(1)) / BigInt(4);
			BigInt rp = mpow(c % p, rep, p);
			
			rep = (q + BigInt(1)) / BigInt(4);
			BigInt rq = mpow(c % q, rep, q);


			BigInt u = extendedEuclidian(p, q);
			BigInt v = extendedEuclidian(q, p);
			

			static int dirs[4][2] =
			{ {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };

			vector<BigInt> ys;
			for (int i = 0; i < 4; ++i)
			{
				BigInt y = (BigInt(dirs[i][0]) * rq * u * p
					+ BigInt(dirs[i][1]) * rp * v * q) % N;
				if (y < 0) y += N;

				ys.push_back(y);
			}

			for (int i = 0; i < 4; ++i)
			{
				// 전수 조사. 표식 이 있다면 해당 y 가 m
				if (ys[i] % BigInt(12) == BigInt(0))
				{
					printf("Rabin 복호화 성공\n");
					return;
				}
			}

			printf("Rabin 복호화 실패\n");
		};


	func(BigInt(7), BigInt(19), BigInt(120));

#ifdef HEAVY_256_TEST
	BigInt p2("68303089742191328715788819840977517352752076767741384200728718098178231561919");
	BigInt q2("92621432031255631263177377311948007137785978295086095448908175338779207791019");
	BigInt m2("4253835037253463382025236508368317342810529334434205117296090875509874046324607643133314027916496324945555624297315668681841228112758481332967504724729620");
	func(p2, q2, m2);
#endif
}



























void AssymetricTest::ECDH()
{
	auto func = [](ECC ecc, Point G,
		BigInt n, BigInt a, BigInt b) -> void
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


#ifdef HEAVY_256_TEST
	// 계산하는데 1분 걸리더라. 조심.
	string p2 = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
	string ca2 = "0";
	string cb2 = "7";
	string Gx2 = "55066263022277343669578718895168534326250603453777594175500187360389116729240";
	string Gy2 = "32670510020758816978083085130507043184471273380659243275938904335757337482424";
	string n2 = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
	string privA2 = "65610368078285743686530282514354855437166089147984718752267841475098511612777";
	string privB2 = "98451540608157669946738156664573520625309719434949018445507184326091007808504";

	func(ECC(p2, ca2, cb2), Point(Gx2, Gy2), n2, privA2, privB2);
#endif
}

void AssymetricTest::ECEIGammal()
{
	auto func = [](ECC ecc, Point G, BigInt n, BigInt e, BigInt r,
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



#ifdef HEAVY_256_TEST
	string p2 = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
	string ca2 = "0";
	string cb2 = "7";
	string Gx2 = "55066263022277343669578718895168534326250603453777594175500187360389116729240";
	string Gy2 = "32670510020758816978083085130507043184471273380659243275938904335757337482424";
	string n2 = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
	string e2 = "64079920806756270664663670307314760341123342645464479887056290712984156574479";
	string r2 = "74264468267481795513795638651832904600743061130363080607858082521394889165253";
	string Mx2 = "31881392106941887333678275854453803986392374053530684366417262024340950537203";
	string My2 = "96580774553925525813305908843296821559250139675613714592546612238647448074340";

	func(ECC(p2, ca2, cb2), Point(Gx2, Gy2), n2, e2, r2, Point(Mx2, My2));
#endif
}




void AssymetricTest::ECDSA()
{
	auto func = [](ECC ecc, Point G, 
		BigInt p, BigInt e, BigInt m, BigInt r) -> void
		{
			
			// 서명 (서명에 사용되는 e, r 는 비공개)
			// ※ e 는 서명자가 영구적으로 들고 있는 비밀키로, E도 상시
			// ※ r 은 서명할 때마다 새로 생성되는 난수로, R도 서명 때 생성
			Point E = ecc.scalarMul(G, e);
			Point R = ecc.scalarMul(G, r);

			BigInt Rx = R.x % p;
			BigInt inv_r = extendedEuclidian(r, p);
			
			BigInt s = mmul(
				inv_r, madd(m, mmul(Rx, e, p), p), p);

			
			// 검증자에게 전달 
			// (m(메세지), 서명(E, s, Rx) 이 전달)
			// ( 원점 G, 모듈려 p는 사전 공유 )
			BigInt inv_s = extendedEuclidian(s, p);
			BigInt u1 = mmul(m, inv_s, p);
			BigInt u2 = mmul(Rx, inv_s, p);

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
		BigInt(19), BigInt(7), BigInt(10), BigInt(3));


#ifdef HEAVY_256_TEST
	string p2 = "115792089237316195423570985008687907853269984665640564039457584007908834671663";
	string ca2 = "0";
	string cb2 = "7";
	string Gx2 = "55066263022277343669578718895168534326250603453777594175500187360389116729240";
	string Gy2 = "32670510020758816978083085130507043184471273380659243275938904335757337482424";
	string n2 = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
	string e2 = "79835748785261120864012942577833320715585386880370569440160598109098789671830";
	string m2 = "59457825462496181976452791601234300875549676596185728411045724927123710946639";
	string r2 = "85059200315112121403676589033912233601446069219626980267999507455413383352205";

	func(ECC(p2, ca2, cb2), Point(Gx2, Gy2), n2, e2, m2, r2);
#endif
}