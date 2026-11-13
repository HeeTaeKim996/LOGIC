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
		ECDSA();
	}

private:
	static void DiffieHellman();
	static void RSA();
	static void EIGammal();
	
	static void ECDH();
	static void ECDSA();
};