#include "Detector.h"



Detector::Detector()
{
	int info[4];
	cpuid(info, 0);
	int nIds = info[0];

	cpuid(info, 0x80000000);
	unsigned nExIds = info[0];

	//  Detect Features
	if (nIds >= 0x00000001) {
		cpuid(info, 0x00000001);
		HW_SSE = (info[3] & ((int)1 << 25)) != 0;

		HW_AVX = (info[2] & ((int)1 << 28)) != 0;
		HW_FMA3 = (info[2] & ((int)1 << 12)) != 0;
	}
	if (nIds >= 0x00000007) {
		cpuid(info, 0x00000007);
		HW_AVX512F = (info[1] & ((int)1 << 16)) != 0;
	}
	if (nExIds >= 0x80000001) {
		cpuid(info, 0x80000001);
		HW_FMA4 = (info[2] & ((int)1 << 16)) != 0;
	}

	__cpuid(info, 1);

	bool osUsesXSAVE_XRSTORE = info[2] & (1 << 27) || false;
	bool cpuAVXSuport = info[2] & (1 << 28) || false;

	if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
	{
		unsigned long long xcrFeatureMask = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
		avxSupported = (xcrFeatureMask & 0x6) == 0x6;
		avxSupported_512 = (xcrFeatureMask & 0xe6) == 0xe6;
	}
}


Detector::~Detector()
{
}
