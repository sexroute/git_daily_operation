#ifndef _SIG_INTERFACE_EXPORT_DSE_
#define _SIG_INTERFACE_EXPORT_DSE_

#ifdef SIGMATH_EXPORTS
  #define API_DLL_DSE extern "C" __declspec(dllexport)
#else
  #define API_DLL_DSE extern "C" __declspec(dllimport)
  #ifdef _DEBUG
    #pragma comment(lib,"sigmathd.lib")
  #else
    #pragma comment(lib,"sigmath.lib")
  #endif  
#endif


API_DLL_DSE int HanningWin(int ilen_,double *pWinOut_);

API_DLL_DSE int SpectralMeasureMent(double *pwave,int n,double *pout,int type,int iWinType_);

API_DLL_DSE int EnergySpecCorrect(double *pWave_,int iSmpFreq_, int ilen_ ,double fSpecCorrectFreq_, double *fCorretedFreq_, double *fCorretedAmp_, double *fCorrecdPhase_);

API_DLL_DSE int CrossSpecPhase(double *pWave_,int iLen_, double fSpecFreq_, int iSmpFreq_,double * dPhase_);

API_DLL_DSE int SCorbitpurifier(double *pWave_,int iLen_,double f0_, int iFs_, int iHiFcNum_, double fHiFcTh_,int iLoFcNum_, double fLoFcTh_, double * pWaveOutput_, double *pFreq_, double * pAmp_, double * pPhase_);

API_DLL_DSE int FWHT(double * pWave_, int ilen_, double *pWaveOut_);

API_DLL_DSE int DistillSupThNums(const double* pWave_, int ilen_, int iStartIndex_,  int iSectionNum_,  double fTh_, int* pVector_);

API_DLL_DSE int TestbSvm2(double *pTrainningData_, double *pDataLabel_, int ilen_,double fRegC_, long iItmax_, double ftolabs_, double ftorrel_,double fth_, long lCacheSize_);

API_DLL_DSE int SCorbitpurifie_Rp(double *pWaveV_, double *pWaveH_,  int ilen_,double f0_, int iFs_, int iHiFcNum_, double fHiFcTh_,int iLoFcNum_, double fLoFcTh_);

API_DLL_DSE int CheckOrbitShape(double *pWaveV_, double *pWaveH_, int ilen_,double f0_, int iFs_,double fThFactor_);
#endif