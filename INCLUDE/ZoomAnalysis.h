#ifndef ZOOMANALYSIS_H_BHKD_
#define ZOOMANALYSIS_H_BHKD_

#pragma pack(push)
#pragma pack(1)

#pragma comment(lib,"ZoomAnalysis.lib")

#ifdef __cplusplus
extern "C" {
#endif

/*
*���ش������˵����	 -1:"ϸ���ֱ��ʴ���δϸ���ķֱ���";
                     -2:"ϸ������������Χ��������20����";
                     -3:="��ʼƵ�ʴ�����ֹƵ�ʻ������ò�������";
                 -4��-5:"��������";
                     -6:"��ֹƵ�ʳ�����Χ";
                   -100:"�豸������Ϣ����";
                   -101:"���ݿ���û�м�¼";
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
