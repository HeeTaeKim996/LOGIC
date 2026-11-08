#pragma once

// ※ 개념 정리는 $(ProjectDir)study\002.GalosField 에 위치

using uint8 = unsigned char;

class GalosField
{
public:
	static size_t add(size_t a, size_t b) { return a ^ b; }
	static size_t sub(size_t a, size_t b) { return a ^ b; }
	
	static size_t mul(size_t a, size_t b);
	static size_t mulWhileMod(size_t a, size_t b, size_t m, 
		size_t limit);
	static size_t mulWhileMod(size_t a, size_t b, size_t m);
	
	static size_t div(size_t a, size_t b);
	static size_t res(size_t a, size_t b);

private:
	static uint8 degree(size_t a);


public:
	static void test();
};

