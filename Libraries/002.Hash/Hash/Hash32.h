#pragma once

typedef unsigned long uint32;

class Hash32
{
public:
	static uint32 hash(const void* data, size_t size);
};

