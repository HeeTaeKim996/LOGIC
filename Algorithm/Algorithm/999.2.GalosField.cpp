#include "pch.h"
#include "999.2.GalosField.h"



size_t GalosField::mul(size_t a, size_t b)
{
	size_t out = 0;
	while (b)
	{
		if (b & 1)
		{
			out ^= a;
		}
		a <<= 1;
		b >>= 1;
	}

	return out;
}

size_t GalosField::mulWhileMod(size_t a, size_t b, size_t m, 
	size_t limit)
{
	size_t out = 0;
	while (b)
	{
		if (b & 1)
		{
			out ^= a;
		}
		a = (a << 1) ^ ((a & limit) > 0 ? m : 0);
		b >>= 1;
	}

	return out;
}

size_t GalosField::mulWhileMod(size_t a, size_t b, size_t m)
{
	if (m <= 1) return - 1;

	return mulWhileMod(a, b, m, 
		static_cast<size_t>(1) << degree(m) - 1);
}

size_t GalosField::div(size_t a, size_t b)
{
	if (b == 0) return -1;

	size_t quotient = 0;

	while (a && degree(a) >= degree(b))
	{
		size_t temp = b;
		uint8 shift = 0;

		while (degree(temp) < degree(a))
		{
			temp <<= 1;
			++shift;
		}

		a ^= temp;
		quotient |= static_cast<size_t>(1) << shift;
	}

	return quotient;
}

size_t GalosField::res(size_t a, size_t b)
{
	if (b == 0) return -1;

	while (a && degree(a) >= degree(b))
	{
		size_t temp = b;

		while (degree(temp) < degree(a))
		{
			temp <<= 1;
		}

		a ^= temp;
	}

	return a;
}











uint8 GalosField::degree(size_t a)
{
	uint8 degree = 0;

	while ((a >>= 1) > 0) // 전위 판정
	{
		++degree;
	}

	return degree; // 인덱스 0부터
}







void GalosField::test()
{
	size_t a = 0x53;
	size_t b = 0xca;

	size_t m = mul(a, b);
	size_t r = res(m, 0x11B);
	size_t m2 = mulWhileMod(a, b, 0x11B);
	
	printf("%x, %x, %x\n", m, r, m2);

}
