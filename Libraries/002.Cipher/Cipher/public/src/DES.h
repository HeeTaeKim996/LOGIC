#pragma once



// ※ 개념 정리는 $(ProjectDir)study\001.DES 에 위치



using uint64 = unsigned long long;
using uint32 = unsigned long;
using uint8 = unsigned char;

#define MASK_28 0x0F'FF'FF'FF
#define MASK_56 0x00'FF'FF'FF'FF'FF'FF'FF
#define MASK_32 0xFF'FF'FF'FF
#define MASK_6  0x00'00'00'3F
#define MASK_4  0x00'00'00'0F

class DES
{
	
private: // 알고리즘 고정 값들 
	const static uint8 SHIFTS[16];
	const static uint8 PC1[56];
	const static uint8 PC2[48];
	const static uint8 E[48];
	const static uint32 SBOX[8][4][16];
	const static uint8 P[32];
	const static uint8 IP[64];
	const static uint8 FP[64];
	

	
public:
	DES(uint64 key);
	uint64 encrypt(uint64 p) { return crypt(p, false); }
	uint64 decrypt(uint64 c) { return crypt(c, true); }

private:
	static uint64 permute(uint64 num, const uint8* p, int inBits, 
		int outBits);

private:
	void makeRoundKeys(uint64 key);
	void ROL_28(uint32& shifter, uint32 n);


private:
	uint64 crypt(uint64 block, bool isDec);

private:
	uint64 expandTo48(uint32 num);
	uint32 substitute(uint64 num);
	uint32 permuteAfterSubsitute(uint32 num);


	
public:
	// 테스트
	static void test();
	static void checkWehtherIP_FP_is_fine();

private:
	uint64 roundKeys[16];





private:
	


};

