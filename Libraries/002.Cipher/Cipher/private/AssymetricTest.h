#pragma once
#include "SizeType.h"

class AssymetricTest
{
public:
	inline static void test()
	{
		DiffieHellman();
		RSA();
		EIGammal();


		ECDH();
		ECEIGammal();
		ECDSA();
	}

private:
	static void DiffieHellman();
	static void RSA();
	static void EIGammal();
	
	static void ECDH();
	static void ECEIGammal();
	static void ECDSA();
};