#include "Hash.h"
#include "Hash32.h"


Hash* Hash::instance()
{
	static Hash* _instance = new Hash();
	return _instance;
}

Hash::Hash()
{

}

sizeType Hash::getHash32(const void* data, size_t size)
{
	return Hash32::hash(data, size);
}