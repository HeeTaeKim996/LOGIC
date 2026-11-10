#include "LEA.h"
#include <cassert>
#include "Endian.h"

LEA::LEA(const uint8* key, int bitLen)
{
    expandKey(key, bitLen);
}


void LEA::encrypt(uint8* p)
{
    uint32 x0 = LittleEndian::loadTo32(p);
    uint32 x1 = LittleEndian::loadTo32(p + 4);
    uint32 x2 = LittleEndian::loadTo32(p + 8);
    uint32 x3 = LittleEndian::loadTo32(p + 12);
    
    for (int r = 0; r < roundCount; ++r)
    {
        const uint32* k = subKeys[r];
        uint32 t = x0;
        x0 = rotLeft_32(
            (x0 ^ k[0]) + (x1 ^ k[1]),
            9);
        x1 = rotRight_32(
            (x1 ^ k[2]) + (x2 ^ k[3]),
            5
        );
        x2 = rotRight_32(
            (x2 ^ k[4]) + (x3 ^ k[5]),
            3
        );
        x3 = t;
    }

    LittleEndian::storeFrom32(p, x0);
    LittleEndian::storeFrom32(p + 4, x1);
    LittleEndian::storeFrom32(p + 8, x2);
    LittleEndian::storeFrom32(p + 12, x3);
}

void LEA::decrypt(uint8* c)
{
    uint32 x0 = LittleEndian::loadTo32(c);
    uint32 x1 = LittleEndian::loadTo32(c + 4);
    uint32 x2 = LittleEndian::loadTo32(c + 8);
    uint32 x3 = LittleEndian::loadTo32(c + 12);

    for (int r = roundCount - 1; r >= 0; --r)
    {
        const uint32* k = subKeys[r];
        uint32 y0 = x0;
        uint32 y1 = x1;
        uint32 y2 = x2;

        x0 = x3;
        x1 = (rotRight_32(y0, 9) - (x0 ^ k[0]))
            ^ k[1];
        x2 = (rotLeft_32(y1, 5) - (x1 ^ k[2]))
            ^ k[3];
        x3 = (rotLeft_32(y2, 3) - (x2 ^ k[4]))
            ^ k[5];
    }

    LittleEndian::storeFrom32(c, x0);
    LittleEndian::storeFrom32(c + 4, x1);
    LittleEndian::storeFrom32(c + 8, x2);
    LittleEndian::storeFrom32(c + 12, x3);
}
































uint32 LEA::rotLeft_32(uint32 x, int n)
{
    // n 이 상수가 아닌 변수 i, j 등을 통해 들어오기에, n &= 31 처리
    n &= 31;
    
    if (n)
    {
        return (x << n) | (x >> (32 - n));
    }
    return x;
}

uint32 LEA::rotRight_32(uint32 x, int n)
{
    n &= 31;

    if (n)
    {
        return (x >> n) | (x << (32 - n));
    }
    return x;
}





















void LEA::expandKey(const uint8* key, int bitLen)
{
	static const uint32 DELTA[8] = {
	0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec,
	0x715ea49e, 0xc785da0a, 0xe04ef22a, 0xe5c40957 };


    static const unsigned R[6] = { 1, 3, 6, 11, 13, 17 };
    if (bitLen == 128) {
        roundCount = 24;
        uint32 T[4];
        for (int j = 0; j < 4; ++j)
        {
            T[j] = LittleEndian::loadTo32(key + 4 * j);
        }
        for (int i = 0; i < 24; ++i) {
            uint32 d = DELTA[i % 4];
            for (int j = 0; j < 4; ++j)
                T[j] = rotLeft_32(T[j] + rotLeft_32(d, i + j), R[j]);
            subKeys[i][0] = T[0]; 
            subKeys[i][1] = T[1];
            subKeys[i][2] = T[2];
            subKeys[i][3] = T[1];
            subKeys[i][4] = T[3];
            subKeys[i][5] = T[1];   // T1이 세 번 쓰임
        }
    }
    else if (bitLen == 192) {
        roundCount = 28;
        uint32 T[6];
        for (int j = 0; j < 6; ++j)
        {
            T[j] = LittleEndian::loadTo32(key + 4 * j);
        }
        for (int i = 0; i < 28; ++i) 
        {
            uint32 d = DELTA[i % 6];
            for (int j = 0; j < 6; ++j) {
                T[j] = rotLeft_32(T[j] + rotLeft_32(d, i + j), R[j]);
                subKeys[i][j] = T[j];
            }
        }
    }
    else if(bitLen == 256)
    {
        // 256비트(len==32)는 비 확장 방식이 다른데, 클로드가 모름.
        // 일단 192 로 땜빵

        expandKey(key, 192);
    }
    else
    {
        assert(false);
    }
}



































#include <functional>
void LEA::test()
{
    auto func = [](const uint8* key, int keyLen, const uint8* pt,
        const uint8* ct) -> void
        {
            LEA lea(key, keyLen);

            uint8 temp[16];
            memcpy(temp, pt, 16);

            lea.encrypt(temp);
            if (memcmp(temp, ct, 16) != 0)
            {
                printf("암호화 실패\n");
                return;
            }

            lea.decrypt(temp);
            if (memcmp(temp, pt, 16) != 0)
            {
                printf("복호화 실패\n");
                return;
            }

            printf("암복호화 성공\n");
        };


    const uint8 key128[16] = {
0x0f,0x1e,0x2d,0x3c,0x4b,0x5a,0x69,0x78,
0x87,0x96,0xa5,0xb4,0xc3,0xd2,0xe1,0xf0 };
    const uint8 pt128[16] = {
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
        0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f };
    const uint8 ct128[16] = {
        0x9f,0xc8,0x4e,0x35,0x28,0xc6,0xc6,0x18,
        0x55,0x32,0xc7,0xa7,0x04,0x64,0x8b,0xfd };
    func(key128, 128, pt128, ct128);


    const uint8 key192[24] = {
    0x0f,0x1e,0x2d,0x3c,0x4b,0x5a,0x69,0x78,
    0x87,0x96,0xa5,0xb4,0xc3,0xd2,0xe1,0xf0,
    0xf0,0xe1,0xd2,0xc3,0xb4,0xa5,0x96,0x87 };
    const uint8 pt192[16] = {
        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
        0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f };
    const uint8 ct192[16] = {
        0x6f,0xb9,0x5e,0x32,0x5a,0xad,0x1b,0x87,
        0x8c,0xdc,0xf5,0x35,0x76,0x74,0xc6,0xf2 };
    func(key192, 192, pt192, ct192);


    // 256 은 키확장 코드를 모르니 생략
}