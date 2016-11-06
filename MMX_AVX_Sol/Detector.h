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
	bool isAVX512() { return (HW_AVX512F && avxSupported_512); };
	bool isFMA() { return (HW_FMA3 || HW_FMA4); };
	
	~Detector();

private:

	//  SIMD: 128-bit
	bool HW_SSE;

	//  SIMD: 256-bit
	bool HW_AVX;
	bool HW_FMA3;
	bool HW_FMA4;

	//  SIMD: 512-bit
	bool HW_AVX512F;    //  AVX512 Foundation

	bool avxSupported = false;
	bool avxSupported_512 = false;
};

