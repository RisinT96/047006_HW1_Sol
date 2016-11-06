#pragma once

#ifdef _WIN32
#include <intrin.h>
//  Windows
#define cpuid(info, x)    __cpuidex(info, x, 0)

#else

//  GCC Intrinsics
#include <cpuid.h>
void cpuid(int info[4], int InfoType) {
	__cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}

#endif



class Detector
{
public:
	Detector();
	bool isSSE() { return (HW_SSE); };
	bool isAVX() { return (HW_AVX && avxSupported); };
	bool isAVX2() { return (HW_AVX2 && avxSupported); };
	bool isAVX512() { return (HW_AVX512CD && HW_AVX512DQ && HW_AVX512PF && HW_AVX512VBMI && HW_AVX512VL && HW_AVX512BW && HW_AVX512F && avxSupported_512); };
	bool isFMA() { return (HW_FMA3 || HW_FMA4); };
	bool isFMA512() { return (HW_AVX512IFMA); };
	
	~Detector();

private:

	//  SIMD: 128-bit
	bool HW_SSE;

	//  SIMD: 256-bit
	bool HW_AVX;
	bool HW_FMA3;
	bool HW_FMA4;
	bool HW_AVX2;

	//  SIMD: 512-bit
	bool HW_AVX512F;    //  AVX512 Foundation
	bool HW_AVX512CD;   //  AVX512 Conflict Detection
	bool HW_AVX512PF;   //  AVX512 Prefetch
	bool HW_AVX512ER;   //  AVX512 Exponential + Reciprocal
	bool HW_AVX512VL;   //  AVX512 Vector Length Extensions
	bool HW_AVX512BW;   //  AVX512 Byte + Word
	bool HW_AVX512DQ;   //  AVX512 Doubleword + Quadword
	bool HW_AVX512IFMA; //  AVX512 Integer 52-bit Fused Multiply-Add
	bool HW_AVX512VBMI; //  AVX512 Vector Byte Manipulation Instructions

	bool avxSupported = false;
	bool avxSupported_512 = false;
};

