
#include <iostream>

#include "src/MemCounter.h"


#include "../private/DES.h"
#include "../private/AES.h"
#include "../private/IDEA.h"
#include "../private/RC4.h"
#include "../private/RC5.h"
#include "../private/BlockCrypt.h"
#include "../private/ARIA.h"
#include "../private/SEED.h"
#include "../private/HIGHT.h"
#include "../private/LEA.h"
#include "../private/AssymetricTest.h"


int main()
{
	//DES::test();
	//AES::test();
	//IDEA::test();
	//RC4::test();
	//RC5::test();

	//BlockCrypt::test();
	
	//ARIA::test();
	//SEED::test();
	//Hight::test();
	//LEA::test();

	AssymetricTest::test();
	//ECC::test();
}

