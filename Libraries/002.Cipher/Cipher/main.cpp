
#include <iostream>

#include "../private/DES.h"
#include "../private/GalosField.h"
#include "../private/AES.h"
#include "../private/IDEA.h"
#include "../private/RC4.h"
#include "../private/RC5.h"

int main()
{
	DES::test();
	//AES::test();
	//IDEA::test();
	//RC4::test();
	//RC5::test();
}

