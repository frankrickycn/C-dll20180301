#include "stdafx.h"
#include "SampleCppWrapper.h"
//#include "DepthDll0301.h"
//using namespace DualFreqDepthDll;

namespace SampleCppWrapper
{
	Variables *_vars = new Variables();
	void InitialMemory()
	{
		_vars->initialmemory();
		//_vars->Sub();
		
	}

	void DataExecute(INT16* dataL, INT16* dataH, int* range, int* lens, int FS, int* powerL, int* powerH,
		int* gainL, int* gainH, int* widthL, int* widthH, int* countall, double thredH, int* depthpointL, int* depthpointH,
		int* maxvL, int* maxvH, double depthlower, double depthupper, double* crr)
	{
		//_vars->Sub();
		_vars->dataexecute(dataL, dataH, range, lens, FS, powerL, powerH,
		gainL, gainH, widthL, widthH, countall, thredH, depthpointL, depthpointH,
		maxvL, maxvH, depthlower, depthupper, crr);
		//_vars->Sub();
	}

	void FreeMemory()
	{
		//_vars->~Variables();
				//delete(_vars);
		delete _vars;
		//_vars = NULL;
		//_vars->Sub();
	}
/*

*/
	int Add()
	{
		return 30;
	}

}