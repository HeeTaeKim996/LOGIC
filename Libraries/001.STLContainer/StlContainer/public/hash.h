#pragma once

#include "SizeType.h"

class __declspec(dllexport) Hash
{
public:
	static Hash* instance();
	
private:
	Hash();

public:
	sizeType getHash32(const void* data, size_t size);
	
};


