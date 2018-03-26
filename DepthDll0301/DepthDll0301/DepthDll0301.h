//#include "stdafx.h"
//#include <iostream>

#pragma once
#include "ipp.h"
//using namespace std;
//namespace DualFreqDepthDll
//{
	class Variables//__declspec(dllexport) 
	{
	public:
		Variables();
		~Variables();

		void Sub();


		//static Variables *_variables;
		
	//public:
		//Variables *ptr;
		//__declspec(dllexport) static Variables* __stdcall GetInstance();

	private:
		//////1.预开辟空间大小
		int DataLen;

		//////2.承接DataExecute()中传入的参数们
		#pragma region 软件参数赋值
		INT16 *channel1;
		INT16 *channel2;
		INT16 *channel3;
		INT16 *channel4;
		int Range;
		int Len;
		int fs;
		int PowerL;
		int PowerH;
		int GainL;
		int GainH;
		int WidthL;
		int WidthH;
		int CountAll;
		double ThredH;
		int DepthpointL;
		int DepthpointH;
		int MaxVL;
		int MaxVH;
		int Depthplower;//深度下限点数
		int Depthpupper;//深度上限点数
		double* arr;
		#pragma endregion 
		
		//////3.包络变量、相关变量和其他基本量
		#pragma region 包络变量、相关变量和其他基本量
		INT16 *channelEnv1;
		INT16 *channelEnv2;
		INT16 *channelEnv3;
		INT16 *channelEnv4;

		int PwGnL;
		int PwGnH;
		int FlgChannelLow;//判断低频数据取第一组还是第二组，值为0则取有TVG的通道，为1则取无TVG的通道
		int FlgChannelHigh;//判断高频数据取第一组还是第二组，值为0则取有TVG的通道，为1则取无TVG的通道
		int FlgDepthLimit;//深度上下限设置，值为0则没有限制，为1则有限制
		int WidthLowPoints;
		int WidthHighPoints;

//~~~		float* envelpSrc;//在求包络子函数中放置float型数组
		Ipp16sc *envelpDst;//在求包络子函数中放置复数结果数组
//~~~		INT16 *envelmag;//在求包络子函数中放置int16型包络数组

		//发射信号仿真
		//INT16 SigL[78] = { 16, 27, 11, -26, -53, -36, 21, 73, 69, 0, -82, -105, -31, 59, 100, 59, -31, -95, -81, 0, 81, 95, 31, -59, -100, -59, 31, 95, 81, 0, -81, -95, -31, 59, 100, 59, -31, -95, -81, 0, 81, 95, 31, -53, -82, -44, 21, 58, 44, 0, -36, -39, -11, 20, 30, 16, -8, -21, -16, 0, 13, 14, 4, -7, -11, -6, 3, 8, 6, 0, -5, -5, -2, 3, 4, 2, -1, -3 };
		//INT16 SigL[96] = { 16, 22, 8, -17, -32, -21, 12, 40, 36, 0, -41, -51, -18, 35, 63, 39, -22, -69, -61, 0, 66, 81, 27, -54, -94, -57, 31, 99, 86, 0, -81, -95, -31, 59, 100, 59, -31, -95, -81, 0, 81, 95, 31, -59, -100, -59, 31, 95, 81, 0, -81, -95, -31,	59, 100, 59, -31, -95, -81, 0, 81, 86, 25, -44, -67, -36, 17, 47, 36, 0, -30, -32, -9, 16, 25, 13, -6, -17, -13, 0, 11, 12, 3, -6, -9, -5, 2, 6, 5, 0, -4, -4, -1, 2, 3, 2 };
		static INT16 SigL[84];//  = { 16,	24,	9, -21, -41, -27, 16	, 54, 50, 0, -59, -74, -26,	52,	94,	58, -32, -105, -96,	0, 94, 109,	35, -66, -111, -64,	33,	101, 85, 0, -83, -96, -31, 58, 97, 56, -29, -89, -74, 0, 72, 84, 27, -50, -84, -49,	25,	76,	66,	0, -54, -58, -17, 29	, 45, 24, -11, -32, -24, 0,	20,	21,	6, -11, -17, -9, 4,	12,	9, 0, -7, -8, -2, 4, 6,	3, -2, -4, -3, 0	, 3, 3,	1, -1 };
		int SigLLen;//发射信号长度

//~~~		int LowerLimt;
//~~~		int UperLimit;
		INT16 *HighCache;//用来求解高频水深的缓存数组
		INT16 *LowCache;//用来求解低频水深的缓存数组
		INT16 *DataLow;//低频处理子函数中的操作变量
		INT16 *DataHigh;//高频处理子函数中的操作变量
		int banpointsLow;//低频处理子函数中盲区点数
		int banpointsHigh;//高频处理子函数中盲区点数

		INT16* DataLowArr;//定义一个10*DataLen长度的数组的指针
		INT64* DataLowCorrCache64s;
		INT32* DataLowCorrCache32s;
		INT16* DataLowSumCache16s;

		static int pastd[10];// = { 1, 1, 1, 1, 1, 1, 2, 16, 16, 16 };
		static int pa[20];// = { 0 };
		static int DepthLPointArr[10];// = { 0 };
		#pragma endregion

		//////4.带通滤波器相关量声明，在InitialMemory中赋值
		#pragma region 滤波器相关量
		int rLowXia;
		int rLowShang;
		int rHighXia;
		int rHighShang;
		int order;///
		int tapslen;///
		double* taps64f;///
		float* taps32f;///
		INT16* filtercachearr;///
		int numIters;///
		int specSize;///
		int bufSize;///
		IppsFIRSpec_32f *pSpec;///
		Ipp8u *buf;///
		#pragma endregion

		//////5.低通滤波器相关量声明，在InitialMemory中赋值
		#pragma region 低通滤波器相关量
		int orderLow;///
		int tapslenLow;///
		double* taps64fLow;///
		float* taps32fLow;///
		INT16* filtercachearrLow;///
		int numItersLow;///
		int specSizeLow;///
		int bufSizeLow;///
		IppsFIRSpec_32f *pSpecLow;///
		Ipp8u *bufLow;///
		#pragma endregion

		//////6.高频数据处理变量声明，在InitialMemory中赋值
		#pragma region 高频数据处理变量声明
		INT16 NoiseLevH;//高频噪声值
		int SNRH;//高频信噪比
		INT16 maxvalueH;//高频一次回波最大值
		int maxpointH;//高频一次回波最大值所在点
		INT16 AmpH;//阈值*最大值
//~~~		int sNH;
//~~~		int pingSpanH;
//~~~		int NH;
//~~~		int* depthStoreH;
//~~~		short int* IntenStoreH;
//~~~		int flgPingH;//初始化之后更新depthStoreH和IntenStoreH的标志位
//~~~		int foreseeValueH;
//~~~		int xiaxianH;
//~~~		int shangxianH;
//~~~		int secondH;
//~~~		int thirdH;
//~~~		int fourthH;
//~~~		int flgH2;
//~~~		int flgH3;
//~~~		int flgH4;
//~~~		INT16 maxvalueH2ed;
//~~~		int maxpointH2ed;
//~~~		INT16 maxvalueH3rd;
//~~~		int maxpointH3rd;
//~~~		INT16 maxvalueH4th;
//~~~		int maxpointH4th;
		#pragma endregion

		//////7.低频数据变量声明，在InitialMemory中赋值
		#pragma region 低频数据处理变量声明

		INT16 NoiseLevL;//低频噪声值
		int SNRL;//低频信噪比
		INT16 maxvalueL;//低频一次回波最大值
		int maxpointL;//低频一次回波最大值所在点
		INT16 AmpL;//阈值*最大值
		#pragma endregion

		//////缓存变量及其他变量声明
//~~~		int cache1;
//~~~		int cache2;
//~~~		int cache3;
//~~~		int cache4;
//~~~		int cache5;
//~~~		int cache6;

	
	public:
		 void initialmemory();
		 void dataexecute(INT16* dataL, INT16* dataH, int* range, int* lens, int FS, int* powerL, int* powerH,
	int* gainL, int* gainH, int* widthL, int* widthH, int* countall, double thredH, int* depthpointL, int* depthpointH,
	int* maxvL, int* maxvH, double depthlower, double depthupper, double* crr);
		 void freememory();
		
	
	private:
		void VarsCaculate();
		void BandPassFilter(INT16* data, int len, double rLowFreq, double rHighFreq);
		void Envel(INT16* data, INT16* result, int len);
		void LowPassFilter(INT16* data, int len, double rLowFreq);
		void HostCompUpdate(INT16* dataL, INT16* dataH, int* range, int* lens, int* powerL, int* powerH,
	int* gainL, int* gainH, int* widthL, int* widthH, int* countall, int* depthpointL, int* depthpointH,
	int* maxvL, int* maxvH, double* crr);
		void LowGetDepthPoint();
		void HighGetDepthPoint();
		void ParasUpdate();
		int ErrorCheck(double* crr);

	};



	//class Func
	//{
	//	//Variables *d = new Variables();//////////////////////////////////?????????????

	//	//Variables d;
	//	Variables *p = Variables::GetInstance();
	//public:
	//	void fund()
	//	{
	//		//d.DataExecute();
	//		p->InitialMemory();
	//		p->DataExecute();
	//		p->FreeMemory();
	//		//p->ss = 0;
	//		//p->ee[0] = 2;
	//	}
	//};
//}