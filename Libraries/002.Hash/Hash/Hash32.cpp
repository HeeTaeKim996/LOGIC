#include "Hash32.h"


uint32 Hash32::hash(const void* data, size_t size)
{
	return *(static_cast<const int*>(data)) + 1;
}
