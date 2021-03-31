#ifndef ZOOMANALYSIS_H_BHKD_
#define ZOOMANALYSIS_H_BHKD_

#pragma pack(push)
#pragma pack(1)

#pragma comment(lib,"ZoomAnalysis.lib")

#ifdef __cplusplus
extern "C" {
#endif

/*
*返回错误参数说明：	 -1:"细化分辨率大于未细化的分辨率";
                     -2:"细化倍数超过范围（即超过20倍）";
                     -3:="起始频率大于终止频率或者设置参数有误";
                 -4，-5:"分析出错";
                     -6:"终止频率超出范围";
                   -100:"设备或测点信息有误";
                   -101:"数据库中没有记录";
*/
short __stdcall ZoomFFT(float data[], long data_len, short startFreq, 
	short stopFreq, short nLines, short SamplingRate, float zoomSpectrum[], 
	long *len, float *f0, float *df);

long __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

#endif 
