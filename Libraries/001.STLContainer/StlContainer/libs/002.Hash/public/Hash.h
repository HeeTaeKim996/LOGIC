#pragma once



typedef unsigned long long uint64;
typedef unsigned long uint32;

class Hash32;
class Hash64;


class __declspec(dllexport) Hash
{
public:
	static Hash* instance()
	{
		static Hash* instance = new Hash();
		return instance;
	}
private:
	Hash();


public:
	~Hash();

public:

	uint32 hash32(const void* data, size_t size);
	uint64 hash64(const void* data, size_t size);



private:
	Hash32* _hash32;
	Hash64* _hash64;
	
};