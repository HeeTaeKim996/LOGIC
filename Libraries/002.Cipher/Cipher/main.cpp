
#include <iostream>

#include "src/MemCounter.h"


#include "../private/DES.h"
#include "../private/GalosField.h"
#include "../private/AES.h"
#include "../private/IDEA.h"
#include "../private/RC4.h"
#include "../private/RC5.h"
#include "../private/BlockCrypt.h"


int main()
{
	//DES::test();
	//AES::test();
	//IDEA::test();
	//RC4::test();
	//RC5::test();

	BlockCrypt::test();

}

