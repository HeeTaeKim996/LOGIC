#pragma once
#include "SizeType.h"

/*
	대칭키는 수리개념이 중요하기에 코드로만 보면 짧아서 모아놓음
	수리 증명은 $(SolutionDir)study\ 에
*/

class AssymetricTest
{
public:
	inline static void test()
	{
		//DiffieHellman();
		//RSA();
		//EIGammal();
		Rabin();
	
		
		//ECDH();
		//ECEIGammal();
		//ECDSA();
	}

private:
	static void DiffieHellman();
	static void RSA();
	static void EIGammal();
	static void Rabin();
	
	static void ECDH();
	static void ECEIGammal();
	static void ECDSA();


};