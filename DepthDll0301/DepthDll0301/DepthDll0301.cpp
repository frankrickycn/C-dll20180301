// DepthDll0301.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "ipp.h"
#include "DepthDll0301.h"
using namespace std;

//namespace DualFreqDepthDll
//{
	//声明全局常量
	const double c = 1500.0;

	const int fL = 24;
	const int fH = 200;

	const int PING = 100;//以100ping数据为数据初始化处理单元

	const int RANGE_LEN = 15;
	const int RANGE_ARR[15] = { 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 125, 150, 200, 300 };

	const int GAIN_LEN = 10;//0-9档
	const int GAINL_ARR[10] = { -40, -30, -20, -10, 0, 5, 10, 20, 30, 40 };
	const int GAINH_ARR[10] = { -40, -30, -20, -10, 0, 5, 10, 20, 30, 40 };

	const int POWER_LEN = 10;//0-9档
	const int POWERL_ARR[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const int POWERH_ARR[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	const int WIDTH_LEN = 7;//0-6档
	const int WIDTHL_ARR[7] = { 32, 64, 128, 256, 512, 1024, 2048 };
	const int WIDTHH_ARR[7] = { 32, 64, 128, 256, 512, 1024, 2048 };
	
	//Variables* Variables::_variables = NULL;
	INT16 Variables::SigL[84] = { 16,	24,	9, -21, -41, -27, 16	, 54, 50, 0, -59, -74, -26,	52,	94,	58, -32, -105, -96,	0, 94, 109,	35, -66, -111, -64,	33,	101, 85, 0, -83, -96, -31, 58, 97, 56, -29, -89, -74, 0, 72, 84, 27, -50, -84, -49,	25,	76,	66,	0, -54, -58, -17, 29	, 45, 24, -11, -32, -24, 0,	20,	21,	6, -11, -17, -9, 4,	12,	9, 0, -7, -8, -2, 4, 6,	3, -2, -4, -3, 0	, 3, 3,	1, -1 };
	int Variables::pastd[10] = { 1, 1, 1, 1, 1, 1, 2, 16, 16, 16 };
	int Variables::pa[20] = { 0 };
	int Variables::DepthLPointArr[10] = { 0 };

	Variables::Variables()
	{


	}
	Variables::~Variables()
	{
//		cout << 22 << endl;
		ippsFree(taps64f);
		ippsFree(taps32f);
		ippsFree(filtercachearr);
		ippFree(pSpec);
		ippFree(buf);
		ippsFree(taps64fLow);
		ippsFree(taps32fLow);
		ippsFree(filtercachearrLow);
		ippFree(pSpecLow);
		ippFree(bufLow);

//~~~		ippsFree(envelpSrc);
		ippsFree(envelpDst);
//~~~		ippsFree(envelmag);
//~~~		ippsFree(depthStoreH);
//~~~		ippsFree(IntenStoreH);
		ippsFree(DataLowArr);
		ippsFree(DataLowCorrCache64s);
		ippsFree(DataLowCorrCache32s);
		ippsFree(DataLowSumCache16s);

		ippsFree(arr);
		ippsFree(HighCache);
		ippsFree(LowCache);
		ippsFree(DataLow);
		ippsFree(DataHigh);

//		cout << 23333 << endl;

		//delete(_variables);
		//_variables = NULL;

	}
	
	void Variables::Sub()
	{
		cout << "-10" << endl;
		
	}


	//Variables* __stdcall Variables::GetInstance()
	//{
	//	if (Variables::_variables == NULL)
	//		Variables::_variables = new Variables();
	//	return Variables::_variables;
	//}

	void Variables::initialmemory()
	{
							//Variables* P = new Variables;
							//Variables::GetInstance();//Variables::
	ippInit();

	//////预开辟空间大小，=65536*2，800kH采样率、300m深度下，单通道点数为320,000点
	DataLen = 400000;
	//						//INT16 SigL1[84] = { 16,	24,	9, -21, -41, -27, 16	, 54, 50, 0, -59, -74, -26,	52,	94,	58, -32, -105, -96,	0, 94, 109,	35, -66, -111, -64,	33,	101, 85, 0, -83, -96, -31, 58, 97, 56, -29, -89, -74, 0, 72, 84, 27, -50, -84, -49,	25,	76,	66,	0, -54, -58, -17, 29	, 45, 24, -11, -32, -24, 0,	20,	21,	6, -11, -17, -9, 4,	12,	9, 0, -7, -8, -2, 4, 6,	3, -2, -4, -3, 0	, 3, 3,	1, -1 };
	//						//SigL = SigL1;
	//						//SigL = (INT16*)malloc(sizeof(INT16)* 84);
	//						//free(SigL);
	//						//////软件输入参数的一一对应，初始化
	channel1 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channel1, DataLen);
	channel2 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channel2, DataLen);
	channel3 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channel3, DataLen);
	channel4 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channel4, DataLen);
	channelEnv1 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channelEnv1, DataLen);
	channelEnv2 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channelEnv2, DataLen);
	channelEnv3 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channelEnv3, DataLen);
	channelEnv4 = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(channelEnv4, DataLen);
	Range = 0;
	Len = 0;
	fs = 0;
	PowerL = 0;
	PowerH = 0;
	GainL = 0;
	GainH = 0;
	PwGnL = 0;
	PwGnH = 0;
	WidthL = 0;
	WidthH = 0;
	CountAll = 0;
	ThredH = 0;
	DepthpointL = 0;
	DepthpointH = 0;
	WidthLowPoints = 0;
	WidthHighPoints = 0;
	MaxVL = 0;
	MaxVH = 0;
	FlgDepthLimit = 0;
	Depthplower = 0;
	Depthpupper = 0;
	FlgChannelLow = 0;
	FlgChannelHigh = 0;

	//////带通滤波器相关量初始化
	rLowXia = 0;
	rLowShang = 0;
	rHighXia = 0;
	rHighShang = 0;
	order = 64;//带通滤波器阶数
	tapslen = 65;//滤波器系数个�?
	taps64f = ippsMalloc_64f(tapslen*sizeof(double));//带通滤波器系数数组
	ippsZero_64f(taps64f, tapslen);
	taps32f = ippsMalloc_32f(tapslen*sizeof(float));//带通滤波器系数数组
	ippsZero_32f(taps32f, tapslen);
	filtercachearr = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(filtercachearr, DataLen);
	numIters = 0;
	ippsFIRSRGetSize(tapslen, ipp32f, &specSize, &bufSize);
	pSpec = (IppsFIRSpec_32f*)ippsMalloc_8u(specSize);
	buf = ippsMalloc_8u(bufSize);
	ippsZero_8u(buf, bufSize);

	//////低通滤波器相关量初始化
	orderLow = 64;
	tapslenLow = 65;//滤波器系数个�?
	numItersLow = 0;
	taps64fLow = ippsMalloc_64f(tapslenLow*sizeof(double));//带通滤波器系数数组
	ippsZero_64f(taps64fLow, tapslenLow);
	taps32fLow = ippsMalloc_32f(tapslenLow*sizeof(float));//带通滤波器系数数组
	ippsZero_32f(taps32fLow, tapslenLow);
	filtercachearrLow = ippsMalloc_16s(DataLen*sizeof(INT16));
	ippsZero_16s(filtercachearrLow, DataLen);
	ippsFIRSRGetSize(tapslenLow, ipp32f, &specSizeLow, &bufSizeLow);
	pSpecLow = (IppsFIRSpec_32f*)ippsMalloc_8u(specSizeLow);
	bufLow = ippsMalloc_8u(bufSizeLow);
	ippsZero_8u(bufLow, bufSizeLow);


	//////包络相关量初始化
					//envelpSrc = ippsMalloc_32f(DataLen * sizeof(float));//在求包络子函数中放置float型数�?
					//ippsZero_32f(envelpSrc, DataLen);
	envelpDst = ippsMalloc_16sc(DataLen * sizeof(Ipp16sc));//在求包络子函数中放置复数结果数组
	ippsZero_16sc(envelpDst, DataLen);
					//envelmag = ippsMalloc_16s(DataLen * sizeof(INT16));//在求包络子函数中放置float型包络数�?
					//ippsZero_16s(envelmag, DataLen);

	//////发射信号相关量初始化
	SigLLen = 84;//matlab200us下低频长�?

	//////高频数据处理变量初始�?
	NoiseLevH = 0;
	SNRH = 0;
	maxvalueH = 0;
	maxpointH = 0;
	AmpH = 0;
					//sNH = 0;
					//pingSpanH = 5;
					//NH = 5;
					//depthStoreH = ippsMalloc_32s(NH * sizeof(int));
					//ippsZero_32s(depthStoreH, NH);
					//IntenStoreH = ippsMalloc_16s(NH * sizeof(INT16));
					//ippsZero_16s(IntenStoreH, NH);
					//flgPingH = 0;
					//foreseeValueH = 0;
					//xiaxianH = 0;
					//shangxianH = 0;
					//secondH = 0;
					//thirdH = 0;
					//fourthH = 0;
					//flgH2 = 0;
					//flgH3 = 0;
					//flgH4 = 0;
					//maxvalueH2ed = 0;
					//maxpointH2ed = 0;
					//maxvalueH3rd = 0;
					//maxpointH3rd = 0;
					//maxvalueH4th = 0;
					//maxpointH4th = 0;

	//////低频数据处理变量初始�?

	DataLowArr = ippsMalloc_16s(10 * DataLen * sizeof(INT16));
	ippsZero_16s(DataLowArr, 10 * DataLen);
					//DataLowArr[10][D] = { 0 };
					//INT16(*pDataLowArr)[D] = DataLowArr;//定义一�?0行的二维数组的指�?
	DataLowCorrCache64s = ippsMalloc_64s(DataLen * sizeof(INT64));
	ippsZero_64s(DataLowCorrCache64s, DataLen);
	DataLowCorrCache32s = ippsMalloc_32s(DataLen * sizeof(INT32));
	ippsZero_32s(DataLowCorrCache32s, DataLen);
	DataLowSumCache16s = ippsMalloc_16s(DataLen * sizeof(INT16));
	ippsZero_16s(DataLowSumCache16s, DataLen);

	NoiseLevL = 0;
	SNRL = 0;
	maxvalueL = 0;
	maxpointL = 0;
	AmpL = 0;


	//////缓存变量及其他变量初始化
	arr = ippsMalloc_64f(20 * sizeof(double));
	ippsZero_64f(arr, 20);
					//LowerLimt = 0;//水深下限
					//UperLimit = 0;//水深上限
					//cache1 = 0;
					//cache2 = 0;
					//cache3 = 0;
					//cache4 = 0;
					//cache5 = 0;
					//cache6 = 0;
	LowCache = ippsMalloc_16s(DataLen * sizeof(INT16));//用来求解低频水深的缓存数�?
	ippsZero_16s(LowCache, DataLen);
	HighCache = ippsMalloc_16s(DataLen * sizeof(INT16));//用来求解高频水深的缓存数�?
	ippsZero_16s(HighCache, DataLen);
	DataLow = ippsMalloc_16s(DataLen * sizeof(INT16));
	ippsZero_16s(DataLow, DataLen);
	DataHigh = ippsMalloc_16s(DataLen * sizeof(INT16));
	ippsZero_16s(DataHigh, DataLen);
	banpointsLow = 0;
	banpointsHigh = 0;

	return;

	}


	void Variables::dataexecute(INT16* dataL, INT16* dataH, int* range, int* lens, int FS, int* powerL, int* powerH,
	int* gainL, int* gainH, int* widthL, int* widthH, int* countall, double thredH, int* depthpointL, int* depthpointH,
	int* maxvL, int* maxvH, double depthlower, double depthupper, double* crr)
	{

		///20171208--->在关闭自动参数更新的情况下，使用该代码进行参数判定
		int flg = 0;
		if (Range != *range)
			flg++;
		if (fs != FS)
			flg++;
		if (PowerL != *powerL)
			flg++;
		if (PowerH != *powerH)
			flg++;
		if (GainL != *gainL)
			flg++;
		if (GainH != *gainH)
			flg++;
		if (WidthL != *widthL)
			flg++;
		if (WidthH != *widthH)
			flg++;
		if (ThredH != thredH)
			flg++;

		if (flg != 0)
		{
			CountAll = 1;
			*countall = CountAll;
			ippsZero_16s(DataLowArr, 10 * DataLen);
			ippsZero_16s(DataLowSumCache16s, DataLen);
		}

		//传入的量赋值到相应内存中
		Range = *range;
		Len = *lens / 2;//数据长度除以2得到每个通道的数据长度
		fs = FS;
		PowerL = *powerL;
		PowerH = *powerH;
		GainL = *gainL;
		GainH = *gainH;
		WidthL = *widthL;
		WidthH = *widthH;
		CountAll = *countall;
		ThredH = thredH;
		DepthpointL = *depthpointL;
		DepthpointH = *depthpointH;
		MaxVL = *maxvL;
		MaxVH = *maxvH;
		ippsCopy_64f(crr, arr, 10);

		Depthplower = 0; //(int)depthlower * 2 * fs / c;
		Depthpupper = 0; //(int)depthupper * 2 * fs / c;

		//将传入数组分割为四通道数据
		ippsCopy_16s(dataL, channel1, Len);
		ippsCopy_16s(&dataL[Len], channel2, Len);
		ippsCopy_16s(dataH, channel3, Len);
		ippsCopy_16s(&dataH[Len], channel4, Len);
		
		VarsCaculate();

		int errflg = 0;
		errflg = ErrorCheck(crr);//上位机要记得将crr数组重新置零
		if (errflg == -1)
			return;
		
		BandPassFilter(channel1, Len, (double)rLowXia / (double)fs, (double)rLowShang / (double)fs);
		Envel(channel1, channelEnv1, Len);
		BandPassFilter(channel2, Len, (double)rLowXia / (double)fs, (double)rLowShang / (double)fs);
		Envel(channel2, channelEnv2, Len);
		BandPassFilter(channel3, Len, (double)rHighXia / (double)fs, (double)rHighShang / (double)fs);
		Envel(channel3, channelEnv3, Len);
		BandPassFilter(channel4, Len, (double)rHighXia / (double)fs, (double)rHighShang / (double)fs);
		Envel(channel4, channelEnv4, Len);

		HighGetDepthPoint();

		LowGetDepthPoint();
		HostCompUpdate(dataL, dataH, range, lens, powerL, powerH,
		gainL, gainH, widthL, widthH, countall, depthpointL, depthpointH, 
		maxvL, maxvH, crr);
		
		//相关的值清零
		ippsZero_16s(DataLow, Len);
		ippsZero_16s(DataHigh, Len);
		
		ippsZero_16s(channel1, Len);
		ippsZero_16s(channel2, Len);
		ippsZero_16s(channel3, Len);
		ippsZero_16s(channel4, Len);
		ippsZero_16s(channelEnv1, Len);
		ippsZero_16s(channelEnv2, Len);
		ippsZero_16s(channelEnv3, Len);
		ippsZero_16s(channelEnv4, Len);
		
		ippsZero_64f(arr, 10);

		return;

	}
	
	
	
	void Variables::freememory()
	{
	//	delete _variables;
	//	_variables = NULL;
	}

	
	
	void Variables::VarsCaculate()
	{
		//判断是否有深度上下限限制
		if (Depthplower >= 0 && Depthpupper > Depthplower)
			FlgDepthLimit = 1;
		else
			FlgDepthLimit = 0;

		//功率和增益拼接为一个值
		for (int sr = 0; sr < GAIN_LEN; sr++)
		{
			if (GainL <= GAINL_ARR[sr])
			{
				PwGnL = 10 * (PowerL - 1) + sr;
				break;
			}
		}
		for (int ss = 0; ss < GAIN_LEN; ss++)
		{
			if (GainH <= GAINH_ARR[ss])
			{
				PwGnH = 10 * (PowerH - 1) + ss;
				break;
			}
		}



		//双频信号的发射信号点数
		WidthLowPoints = WidthL * fs / 1000 + 1 + 2 * fs / fL + 1 + 5 * fs / fL;//低频信号对应的点数
		WidthHighPoints = WidthH * fs / 1000 + 1 + 13 * fs / fH + 1;//高频信号对应的点数

		banpointsLow = 3 * WidthLowPoints;
		banpointsHigh = 3 * WidthHighPoints;

		int BanPoints = (int)(arr[1] * 2 / c*fs * 1000);
		if (banpointsLow < BanPoints || banpointsHigh < BanPoints)
		{
			banpointsLow = BanPoints;
			banpointsHigh = BanPoints;
		}


		//1.确定滤波上下限;2.确认通道选择
		if (CountAll < 1)
		{
			arr[0] = -1;//计数值错误
			return;
		}

		if (CountAll == 1)
		{
			//设定双频滤波的上下限频率值
			rLowXia = 20; rLowShang = 30;
			if (fs == 250){
				rHighXia = 40; rHighShang = 60;
			}
			else if (fs == 160){
				rHighXia = 30; rHighShang = 50;
			}
			else if (fs == 500 || fs == 800){
				rHighXia = 190; rHighShang = 210;
			}
			else{
				arr[0] = -2;//采样率错误
				return;
			}

			//通道选择
			FlgChannelLow = 0;
			FlgChannelHigh = 0;
			
		}

	}
	
	
	
	int Variables::ErrorCheck(double* crr)
	{
		if(arr[0] != 0)
		{
			ippsCopy_64f(arr, crr, 10);
			ippsZero_64f(arr, 10);
			//*depthpointL = 0;
			//*depthpointH = 0;
			return -1;
		}
		return 0;
	}
	
	
	
	void Variables::BandPassFilter(INT16* data, int len, double rLowFreq, double rHighFreq)
	{
		//int numIters = len + order / 2;
		numIters = len + order / 2;
		ippsCopy_16s(data, filtercachearr, len);
		//滤波系数求解
		ippsFIRGenBandpass_64f(rLowFreq, rHighFreq, taps64f, tapslen, ippWinHamming, ippTrue);
		ippsConvert_64f32f(taps64f, taps32f, tapslen);

		//滤波器获取结构大小和缓存大小
		//int specSize, bufSize;
		//ippsFIRSRGetSize(tapslen, ipp32f, &specSize, &bufSize);

		//initialize the spec structure
		//IppsFIRSpec_32f *pSpec;
		//pSpec = (IppsFIRSpec_32f*)ippsMalloc_8u(specSize);
		ippsFIRSRInit_32f(taps32f, tapslen, ippAlgFFT, pSpec);

		//apply the FIR filter
		//Ipp8u *buf;
		//buf = ippsMalloc_8u(bufSize);
		ippsFIRSR_16s(filtercachearr, filtercachearr, numIters, pSpec, NULL, NULL, buf);
		ippsCopy_16s(&filtercachearr[order / 2], data, len);

		ippsZero_16s(filtercachearr, DataLen);

		////free memory
		//ippFree(pSpec);
		//ippFree(buf);

		return;
	}



	void Variables::Envel(INT16* data, INT16* result, int len)
	{
		//hilbert transform
		IppsHilbertSpec_16s16sc* pSpece;
		ippsHilbertInitAlloc_16s16sc(&pSpece, len, ippAlgHintFast);
		ippsHilbert_16s16sc_Sfs(data, envelpDst, pSpece, 0);

		//求包络
		ippsMagnitude_16sc_Sfs(envelpDst, result, len, 0);

		//防止尾部突起
		ippsZero_16s(&result[len - 2 * WidthLowPoints], 2 * WidthLowPoints);

		//ippsFree(pSpec);
		ippsHilbertFree_16s16sc(pSpece);

		return;

	}

	

	void Variables::LowPassFilter(INT16* data, int len, double rLowFreq)
	{
		numItersLow = len + orderLow / 2;
		ippsFIRGenLowpass_64f(rLowFreq, taps64fLow, tapslenLow, ippWinHamming, ippTrue);

		ippsConvert_64f32f(taps64fLow, taps32fLow, tapslenLow);

		ippsFIRSRInit_32f(taps32fLow, tapslenLow, ippAlgFFT, pSpecLow);

		ippsFIRSR_16s(data, filtercachearrLow, numItersLow, pSpecLow, NULL, NULL, bufLow);
		ippsCopy_16s(&filtercachearrLow[order / 2], data, len);

		ippsZero_16s(filtercachearrLow, DataLen);

		
	}

	
	void Variables::HostCompUpdate(INT16* dataL, INT16* dataH, int* range, int* lens, int* powerL, int* powerH,
	int* gainL, int* gainH, int* widthL, int* widthH, int* countall, int* depthpointL, int* depthpointH,
	int* maxvL, int* maxvH, double* crr)
	{
		//声学数组，深度值更新至上位机内存中
		ippsCopy_16s(DataLow, dataL, Len);//DataLow
		ippsZero_16s(&dataL[Len], Len);
		ippsCopy_16s(DataHigh, dataH, Len);//DataHigh
		ippsZero_16s(&dataH[Len], Len);

		if (DepthpointL>=3*2*160000/c)
			DepthpointL = DepthpointL - 20;//
		else if (DepthpointL>=1*2*160000/c && DepthpointL<3*2*160000/c)
			DepthpointL = DepthpointL - 20;//
		else
			DepthpointL = DepthpointL - 20;//

		if (DepthpointL < DepthpointH)
			DepthpointL = DepthpointH;

		*depthpointL = DepthpointL;//
		*depthpointH = DepthpointH;

		//截取数据长度
		double depthcut = ((*depthpointH) *c / fs / 2 / 1000);
		int depthcut32s = (((int)(depthcut * 2))/10 + 1)*10*2;
		*lens = depthcut32s*fs *1000/ ((int)c);

		//通道选择、量程、功率、增益、脉宽、最大值更新模块
		//ParasUpdate();

		//更新值至上位机的内存空间中
		*range = Range;
		*countall = CountAll;

		*powerL = PowerL;
		*gainL = GainL;
		*widthL = WidthL;
		*maxvL = (int)MaxVL;//NoiseLevL;

		*powerH = PowerH;
		*gainH = GainH;
		*widthH = WidthH;
		*maxvH = (int)MaxVH;//NoiseLevH;

		return;

	}


	
	void Variables::HighGetDepthPoint()
	{
		if (FlgDepthLimit == 0)
		{

			switch (FlgChannelHigh)
			{
			case 0:ippsCopy_16s(channelEnv3, DataHigh, Len); break;
			case 1:ippsCopy_16s(channelEnv4, DataHigh, Len); break;
			default:{ }//通道选取标志位错误
			}	

			//排除孤立高点的干扰
			for (int pl = 0; pl < 2; pl++)
			{
				ippsMaxIndx_16s(&DataHigh[banpointsHigh], Len - banpointsHigh - WidthHighPoints, &maxvalueH, &maxpointH);//maxpoint的值从data+banpoints开始计0
				maxpointH += banpointsHigh;

				int cc = 0;//用来计数
				for (int i = maxpointH - WidthHighPoints; i < maxpointH + WidthHighPoints; i++)
				{
					if (DataHigh[i] >(maxvalueH >> 1))
						cc = cc + 1;
				}

				if (cc < (int)(WidthHighPoints*0.3))
				{
					ippsZero_16s(&DataHigh[maxpointH - (WidthHighPoints >> 1)], 1 * WidthHighPoints);
				}
				else
				{
					break;
				}
			}


			//求噪声等级和最大值
			ippsMean_16s_Sfs(&DataHigh[(int)(0.85*Len)], (int)(0.10*Len), &NoiseLevH, 0);
			ippsMaxIndx_16s(&DataHigh[banpointsHigh], Len - banpointsHigh - WidthHighPoints, &maxvalueH, &maxpointH);//maxpoint的值从data+banpoints开始计0
			maxpointH += banpointsHigh;

			//求信噪比SNR，利用该值判断水质清澈或者浑浊，分别对应不同算法，后续补充
			SNRH = (int)(maxvalueH / (NoiseLevH + 1));//此处加1是为了防止噪声为0的情况
		
		
		
			arr[4] = SNRH;



			if (maxvalueH <= 1000 || SNRH <= 10)
				arr[0] = -4;//无回波或回波幅值很小

			if (SNRH >= 10)
			{
				//找到大于最大值50％以上的第一个点，认为是水底
				int sk;
				for (sk = banpointsHigh + WidthHighPoints; sk < Len; sk++)
				{
					if (DataHigh[sk] >= (maxvalueH * 0.6))
					{
						ippsCopy_16s(&DataHigh[sk - WidthHighPoints], HighCache, 2 * WidthHighPoints);
						break;
					}
				}


				ippsMaxIndx_16s(HighCache, 2 * WidthHighPoints, &maxvalueH, &maxpointH);//maxpoint的值从data+banpoints开始计0
				maxpointH += sk - WidthHighPoints;
				MaxVH = (int)maxvalueH;

				//根据输入的ThredH参数确定阈值并求解深度
				AmpH = (short)(maxvalueH * 0.1);


		//20171205版本
				int j,chch = 0;
				if ((sk - 2 * WidthHighPoints) > banpointsHigh)
					chch = sk - 2 * WidthHighPoints + 1 * WidthHighPoints;//为下面预留出一个WidthHighPoints的间隔
				else
					chch = banpointsHigh + 1 * WidthHighPoints;//为下面预留出一个WidthHighPoints的间隔


				for (j = sk; j > chch; j--)
				{
					if (DataHigh[j] <= (short)(maxvalueH * 0.1))
					{
						DepthpointH = j + 1;
						if (((j - WidthHighPoints) > banpointsHigh) && (arr[0] != -4))
						{
							for (int ks = j - WidthHighPoints; ks < j; ks++)
							{
								if (DataHigh[ks] >= AmpH)
								{
									DepthpointH = ks + 1;
									break;
								}
							}
						}
						break;
					}
					DepthpointH = j + 1;

				}

				//数据处理
				ippsSet_16s(0, &DataHigh[DepthpointH + 1*WidthHighPoints], Len - DepthpointH - 2*WidthHighPoints);

			}
			else
			{
				DepthpointH = 0;
			}














	///////////20171228抽取
			int coef_SD = Len / 2048;
			int outputLen1 = 0;
			int phase1 = 0;
			int outputLen2 = 0;
			int phase2 = 0;
			if (coef_SD != 1)
			{
				//LowPassFilter(DataLow, Len, 0.5/(double)coef_SD);
				LowPassFilter(DataHigh, Len, 0.5/(double)coef_SD);
				//ippsSampleDown_16s(DataLow, Len, DataLow, &outputLen1, coef_SD, &phase1);
				ippsSampleDown_16s(DataHigh, Len, DataHigh, &outputLen2, coef_SD, &phase2);
			}

			ippsCopy_16s(&DataHigh[1], DataHigh, 2049);

	//////////


			//整理数据以便软件端做处理
			ippsThreshold_LTVal_16s_I(DataHigh, 2048, AmpH, 0);

	////////把值划归至1-64之间
			ippsDivC_16s_ISfs(128, DataHigh, 2048, 0);
			//ippsAddC_16s_I(1, DataHigh, 2048);
			ippsThreshold_GT_16s_I(DataHigh, 2048, 64);
			ippsThreshold_LT_16s_I(DataHigh, 2048,  0);
	////////

			ippsSet_16s(64, DataHigh, banpointsHigh/coef_SD/3);//maxvalueH


			ippsZero_16s(HighCache, 2 * WidthHighPoints);

			return;

		}

		else if (FlgDepthLimit == 1)
		{
			//switch (FlgChannelHigh)
			//{
			//case 0:ippsCopy_16s(channelEnv3, DataHigh, Len); break;
			//case 1:ippsCopy_16s(channelEnv4, DataHigh, Len); break;
			//default:{DepthpointH = -1; return; }
			//}

			////排除孤立高点的干扰
			//ippsCopy_16s(&DataHigh[Depthplower], HighCache, Depthpupper - Depthplower);

			////求噪声等级和最大值
			//ippsMean_16s_Sfs(&DataHigh[(int)(0.6*Len)], (int)(0.05*Len), &NoiseLevH, 0);

			//ippsMaxIndx_16s(HighCache, 2 * WidthHighPoints, &maxvalueH, &maxpointH);//maxpoint的值从data+banpoints开始计0
			//maxpointH += Depthplower - WidthHighPoints;
			//MaxVH = (int)maxvalueH;

			////求信噪比SNR，利用该值判断水质清澈或者浑浊，分别对应不同算法，后续补充
			//SNRH = (int)(maxvalueH / (NoiseLevH + 1));//此处加1是为了防止噪声为0的情况

			////根据输入的ThredH参数确定阈值并求解深度
			//AmpH = (short)(maxvalueH * ThredH);//maxvalueH * ThredH
			//for (int j = 0; j < (Depthpupper - Depthplower); j++)
			//{
			//	if (HighCache[j] >= AmpH)
			//	{
			//		DepthpointH = Depthplower + j + 1;
			//		break;
			//	}
			//}




			////整理数据以便软件端做处理
			//ippsThreshold_LTVal_16s_I(DataHigh, Len, AmpH, 0);
			//ippsSet_16s(maxvalueH, DataHigh, banpointsHigh);

			//ippsZero_16s(HighCache, 2 * WidthHighPoints);

			//return;

		}

	}



	void Variables::LowGetDepthPoint()
	{
		int  remainder = (CountAll - 1) % 10;
		if (FlgDepthLimit == 0)//判断是否有上下限限制
		{
			switch (FlgChannelLow)
			{
				case 0:ippsCopy_16s(channel1, DataLow, Len); break;
				case 1:ippsCopy_16s(channel2, DataLow, Len); break;
				default:{ }//通道选取标志位错误
			}

			//求信噪比SNR，利用该值判断水质清澈或者浑浊，分别对应不同算法，后续补充
			ippsMean_16s_Sfs(&channelEnv1[(int)(0.85*Len)], (int)(0.10*Len), &NoiseLevL, 0);
			ippsMaxIndx_16s(&channelEnv1[banpointsLow], Len - banpointsLow - WidthLowPoints, &maxvalueL, &maxpointL);//maxpoint的值从data+banpoints开始计0
			maxpointL += banpointsLow;
			MaxVL = (int)maxvalueL;
			SNRL = (int)(maxvalueL / (NoiseLevL + 1));//此处加1是为了防止噪声为0的情况


			ippsCrossCorr_16s64s(SigL, SigLLen, DataLow, Len, DataLowCorrCache64s, Len, 0);//该步骤可求得所需的相关结果
			ippsDivC_64s_ISfs(65536, DataLowCorrCache64s, Len, 0);//保证1ping的结果可转化为INT16，10ping的加和仍在INT16范围内
			ippsConvert_64s32s_Sfs(DataLowCorrCache64s, DataLowCorrCache32s, Len, ippRndZero,0);
			ippsConvert_32s16s(DataLowCorrCache32s, DataLow, Len);
			Envel(DataLow, DataLow, Len);

			//对未平均前的数据做处理，排除孤立高点的干扰
			for (int pl = 0; pl < 2; pl++)
			{
				ippsMaxIndx_16s(&DataLow[banpointsLow], Len - banpointsLow - WidthLowPoints, &maxvalueL, &maxpointL);//maxpoint的值从data+banpoints开始计0
				maxpointL += banpointsLow;

				int cc = 0;//用来计数
				for (int i = maxpointL - WidthLowPoints; i < maxpointL + WidthLowPoints; i++)
				{
					if (DataLow[i] >(maxvalueL >> 1))
						cc = cc + 1;
				}

				if (cc < (int)(WidthLowPoints*0.3))
				{
					ippsZero_16s(&DataLow[maxpointL - (WidthLowPoints >> 1)], 1 * WidthLowPoints);
				}
				else
				{
					break;
				}
			}

			//找到大于最大值50％以上的第一个点，认为是水底
			int sl;
			for (sl = banpointsLow; sl < Len; sl++)
			{
				if (DataLow[sl] >= (maxvalueL * 0.6))
				{
					ippsMaxIndx_16s(&DataLow[sl], 2 * WidthLowPoints, &maxvalueL, &maxpointL);
					maxpointL += sl;
					DepthLPointArr[remainder] = maxpointL;


					break;
				}
			}


	////////->2017version:处理沙丘时双频有差别
			int addmethod = 1;
			switch (addmethod){
			case 0:{

					   ippsSub_16s_I(&DataLowArr[remainder*DataLen], DataLowSumCache16s, Len);
					   ippsAdd_16s_I(DataLow, DataLowSumCache16s, Len);
					   ippsCopy_16s(DataLow, &DataLowArr[remainder*DataLen], Len);
					   ippsCopy_16s(DataLowSumCache16s, DataLow, Len);
			}
				break;
			case 1:{
					   int  remainderr = (CountAll - 1) % 6;
					   ippsSub_16s_I(&DataLowArr[remainderr*DataLen], DataLowSumCache16s, Len);
					   ippsAdd_16s_I(DataLow, DataLowSumCache16s, Len);
					   ippsCopy_16s(DataLow, &DataLowArr[remainderr*DataLen], Len);
					   ippsCopy_16s(DataLowSumCache16s, DataLow, Len);

					   for (int ldk = 0; ldk < 3;ldk++)
						ippsAdd_16s_I(&DataLowArr[remainderr*DataLen], DataLow, Len);

			}
				break;
			default:{ }
			}


			//arr[0] = (double)DataLowSumCache16s[0];// DataLowArr[remainder*DataLen + 0];
			//arr[1] = (double)DataLowSumCache16s[1];


			//对平均后的数据做处理，排除孤立高点的干扰
			for (int pl = 0; pl < 2; pl++)
			{
				ippsMaxIndx_16s(&DataLow[banpointsLow], Len - banpointsLow - WidthLowPoints, &maxvalueL, &maxpointL);//maxpoint的值从data+banpoints开始计0
				maxpointL += banpointsLow;

				int cc = 0;//用来计数
				for (int i = maxpointL - WidthLowPoints; i < maxpointL + WidthLowPoints; i++)
				{
					if (DataLow[i] >(maxvalueL >> 1))
						cc = cc + 1;
				}

				if (cc < (int)(WidthLowPoints*0.3))
				{
					ippsZero_16s(&DataLow[maxpointL - (WidthLowPoints >> 1)], 1 * WidthLowPoints);
				}
				else
				{
					break;
				}
			}

			//找到大于最大值50％以上的第一个点，认为是水底
			int sk = 0;
			for (sk = banpointsLow; sk < Len; sk++)
			{
				if (DataLow[sk] >= (maxvalueL * 0.9))
				{
					ippsCopy_16s(&DataLow[sk - 2 * WidthLowPoints], LowCache, 4 * WidthLowPoints);
					ippsZero_16s(&DataLow[sk + 2 * WidthLowPoints], Len - sk - 3 * WidthLowPoints);

	//////20180119->低频最大值之前的数值进行处理
					if ((sk-3*WidthLowPoints) > banpointsLow)
						//ippsZero_16s(&DataLow[sk - banpointsLow], banpointsLow);
						ippsDivC_16s_ISfs(10, &DataLow[sk - 3*WidthLowPoints], 3*WidthLowPoints, 0);
					else
						//ippsZero_16s(&DataLow[banpointsLow], sk - banpointsLow);
						ippsDivC_16s_ISfs(10, &DataLow[banpointsLow], sk - banpointsLow, 0);
					//arr[9] = 100;

					break;
				}
			}

			//求噪声等级和最大值
			//ippsMean_16s_Sfs(&DataLow[(int)(0.9*Len)], (int)(0.05*Len), &NoiseLevL, 0);
			//arr[9] = NoiseLevL;
			ippsMaxIndx_16s(LowCache, 4 * WidthLowPoints, &maxvalueL, &maxpointL);//maxpoint的值从data+banpoints开始计0
			maxpointL += sk - 2*WidthLowPoints;
	//////
			ippsCopy_32s(&pastd[9 - remainder], &pa[0], remainder + 1);
			ippsCopy_32s(&pastd[0], &pa[remainder + 1], 9 - remainder);
			int sm = remainder + 1;
			if (CountAll >= 10)
				sm = 10;
			int psum1 = 0;
			int psum = 0;
			int pck[10] = { 0 };
			ippsSum_32s_Sfs(pa, sm, &psum1, 0);
			ippsMul_32s_Sfs(DepthLPointArr, pa, pck, sm, 0);
			ippsSum_32s_Sfs(pck, sm, &psum, 0);
		
			DepthpointL = psum/psum1;
	///////////


	///////////20180223
			int T_l = 0;
			switch (T_l)
			{
				case 0:DepthpointL = maxpointL; break;
				case 1:DepthpointL = (psum + maxpointL * 8) / (psum1 + 8); break;
				default:{ }
			}


	///////////20180302高频之前的回波图像清零
			if (DepthpointL < DepthpointH)
			{
				if ((DepthpointL - 2 * WidthLowPoints)>banpointsLow)
					ippsZero_16s(&DataLow[DepthpointL - 2 * WidthLowPoints], DepthpointH - (DepthpointL - 2 * WidthLowPoints));
				else
					ippsZero_16s(&DataLow[banpointsLow], DepthpointH - banpointsLow);
			}

	//////////		

			//DepthpointL = maxpointL;


			//根据输入的ThredH参数确定阈值并求解深度
			AmpL = (short)(maxvalueL * 0.5);//maxvalueH * ThredH
			//for (int j = maxpointL - sk + 2*WidthLowPoints; j > 0; j--)
			//{
			//	if (LowCache[j] == maxvalueL)
			//	{
			//		DepthpointL = sk - 2 * WidthLowPoints + j + 1;
			//		break;
			//	}
			//}


	///////////20171228
			int coef_SD = Len / 2048;//除数的理论取值为2048，此处多取几个点
			int outputLen1 = 0;
			int phase1 = 0;
			int outputLen2 = 0;
			int phase2 = 0;
			if (coef_SD != 1)
			{
				LowPassFilter(DataLow, Len, 0.5/(double)coef_SD);
				//LowPassFilter(DataHigh, Len, 0.5/(double)coef_SD);
				ippsSampleDown_16s(DataLow, Len, DataLow, &outputLen1, coef_SD, &phase1);
				//ippsSampleDown_16s(DataHigh, Len, DataHigh, &outputLen2, coef_SD, &phase2);
			}


			ippsCopy_16s(&DataLow[1], DataLow, 2049);//数组从1开始取是由于ipp降采样方法导致的

	//////////




			//整理数据以便软件端做处理
			ippsThreshold_LTVal_16s_I(DataLow, 2048, AmpL, 0);

	////////把值划归至1-64之间
			ippsDivC_16s_ISfs(50, DataLow, 2048, 0);
			//ippsAddC_16s_I(1, DataLow, 2048);
			ippsThreshold_GT_16s_I(DataLow, 2048, 64);
			ippsThreshold_LT_16s_I(DataLow, 2048,  0);
	////////

		ippsSet_16s(64, DataLow, banpointsLow/coef_SD);//maxvalueL

		return;
		}

		else if (FlgDepthLimit == 1)
		{
			//switch (FlgChannelLow)
			//{
			//case 0:ippsCopy_16s(channelEnv1, DataLow, Len); break;
			//case 1:ippsCopy_16s(channelEnv2, DataLow, Len); break;
			//default:{DepthpointL = -1; return; }
			//}

			////排除孤立高点的干扰
			//ippsCopy_16s(&DataLow[Depthplower], LowCache, Depthpupper - Depthplower);

			////求噪声等级和最大值
			//ippsMean_16s_Sfs(&DataLow[(int)(0.6*Len)], (int)(0.05*Len), &NoiseLevL, 0);

			//ippsMaxIndx_16s(LowCache, 2 * WidthLowPoints, &maxvalueL, &maxpointL);//maxpoint的值从data+banpoints开始计0
			//maxpointL += Depthplower;
			//MaxVL = (int)maxvalueL;

			////求信噪比SNR，利用该值判断水质清澈或者浑浊，分别对应不同算法，后续补充
			//SNRL = (int)(maxvalueL / (NoiseLevL + 1));//此处加1是为了防止噪声为0的情况

			////根据输入的ThredH参数确定阈值并求解深度
			//AmpL = (short)(maxvalueL * ThredH);//maxvalueH * ThredH
			//for (int j = maxpointL - Depthplower; j > 0; j--)
			//{
			//	if (LowCache[j] <= AmpL)
			//	{
			//		DepthpointL = Depthplower + j + 1;
			//		break;
			//	}
			//}


			////整理数据以便软件端做处理
			//ippsThreshold_LTVal_16s_I(DataLow, Len, AmpL, 0);
			//ippsSet_16s(64, DataLow, banpointsLow);//maxvalueL


			//ippsZero_16s(LowCache, 2 * WidthLowPoints);

			//return;


		}

	}

	//class Func
	//{
	//	//Variables *d = new Variables();//////////////////////////////////?????????????
	//	
	//	Variables d;
	//	Variables *p = Variables::GetInstance();
	//	void fund()
	//	{
	//		d.DataExecute();
	//		p->InitialMemory();
	//		p->DataExecute();
	//		p->FreeMemory();
	//		//p->ss = 0;
	//		//p->ee[0] = 2;
	//	}
	//};


//}










