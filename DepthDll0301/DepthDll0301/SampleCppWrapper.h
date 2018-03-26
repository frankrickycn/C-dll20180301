//#include "stdafx.h"
//#include "ipp.h"
#pragma once
#include "DepthDll0301.h"


namespace SampleCppWrapper
{
	extern "C" __declspec(dllexport) void InitialMemory();
	extern "C" __declspec(dllexport) void DataExecute(INT16* dataL, INT16* dataH, int* range, int* lens, int FS, int* powerL, int* powerH,
	int* gainL, int* gainH, int* widthL, int* widthH, int* countall, double thredH, int* depthpointL, int* depthpointH,
	int* maxvL, int* maxvH, double depthlower, double depthupper, double* crr);
	extern "C" __declspec(dllexport) void FreeMemory();

	extern "C" __declspec(dllexport) int Add();

}

