#include "stdafx.h"
#include "plantinfo.h"
#include "ZoomAnalysis.h"
#include "math.h"
#include "additions/DebugHelper.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/CTimeOutReadWriteLock.h>
#include "MiddleWare.h"
#include "../ServerStatus/IServerStatusExport.h"
extern LY::IServerStatus * g_pServerStatus ;

HashTableSeedTable BufferedBaseChannSigMathBasedOnSpecChann::m_oSeedTable;

HashTableSeedTable::HashTableSeedTable()
{
	DWORD ldwHigh =0;

	DWORD ldwLow =0 ;

	DWORD ldwSeed = 0x00100001;

	DWORD ldwPos1 = 0;

	DWORD ldwPos2 = 0;

	DWORD i =0;

	for(ldwPos1 = 0; ldwPos1 < 0x100; ldwPos1++)
	{ 
		for(ldwPos2 = ldwPos1, i = 0; i < 5; i++, ldwPos2 += 0x100)
		{ 
			ldwSeed = (ldwSeed * 125 + 3) % 0x2AAAAB;
			ldwHigh= (ldwSeed & 0xFFFF) << 0x10;
			ldwSeed = (ldwSeed * 125 + 3) % 0x2AAAAB;
			ldwLow= (ldwSeed & 0xFFFF);
			this->SeedTable[ldwPos2] = (ldwHigh| ldwLow); 
		} 
	} 

}

BufferedBaseChannSigMathBasedOnSpecChann::BufferedBaseChannSigMathBasedOnSpecChann()
{

}

int BufferedBaseChannSigMathBasedOnSpecChann::gSEAnalysis(const double* pwave_, 
														  const int& ilen_, 
														  const int& iFs_, 
														  const int& iCutoffFreq_, 
														  double & fgSEValue_, 
														  double *pEnvWave_ /* = NULL */, 
														  unsigned int iPrecision_ /* = 2 */)
{
	//计算key
	CHAR lpKey[9] = {0};
	memcpy(lpKey,&iCutoffFreq_,sizeof(int));//iCutoffFreq_
	memcpy(&lpKey[2],&iPrecision_,sizeof(int));//iPrecision_
	int lnKey = BufferedBaseChannSigMathBasedOnSpecChann::Hash(lpKey,8);

	std::map<int,ValueType>::iterator loItBuffer = this->m_oMapBuffer.find(lnKey);

	std::map<int,double>::iterator   loItDobleBuffer = this->m_oMapDoublebuffer.find(lnKey);

	if (loItBuffer!= this->m_oMapBuffer.end() && loItDobleBuffer!=this->m_oMapDoublebuffer.end())
	{
		fgSEValue_ = loItDobleBuffer->second;

		double * lpBuffer = &loItBuffer->second.front();

		int lnCopyLen = ilen_;

		if (lnCopyLen> loItBuffer->second.size())
		{
			lnCopyLen = loItBuffer->second.size();
		}		

		memcpy(pEnvWave_,lpBuffer,lnCopyLen);

		return 1;

	}else
	{
		try
		{
			int lnRet = this->m_oSigMath.gSEAnalysis(	pwave_,
				ilen_,
				iFs_,
				iCutoffFreq_,
				fgSEValue_,
				pEnvWave_,
				iPrecision_);

			if (lnRet>=0)
			{
				std::vector<double> & lRefBuffer = this->m_oMapBuffer[lnKey];
				lRefBuffer.resize(ilen_);
				double * lpBuffer =& lRefBuffer.front();
				memcpy(lpBuffer,pEnvWave_,lRefBuffer.size());

				this->m_oMapDoublebuffer[lnKey] = fgSEValue_;
			}

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}


		return -1;


	}

}

int BufferedBaseChannSigMathBasedOnSpecChann::Hash(char * apChar,int anLen,int anMask)
{
	if (apChar == NULL)
	{
		return 0;
	}

	DWORD ldwParam = 0x7FED7FED;
	DWORD ldwSeed = 0xEEEEEEEE;

	int ch =0;

	int lnIndex = 0;

	while(lnIndex < anLen)
	{ 
		/*ch = toupper(apChar[lnIndex]);*/
		ldwParam = BufferedBaseChannSigMathBasedOnSpecChann::m_oSeedTable.SeedTable[(anMask<< 8) + ch] ^ (ldwParam + ldwSeed);
		ldwSeed = ch + ldwParam + ldwSeed + (ldwSeed << 5) + 3; 
		lnIndex++;
	}

	return ldwParam; 
}

int BufferedBaseChannSigMathBasedOnSpecChann::SpectralMeasureMent( const double *pwave, 
																  int n,
																  vector<double>& vOut_, 
																  E_SpectrumType type, 
																  E_WINOW_TYPE iWinType_ /* = WINDOW_HANNING */, 
																  unsigned int iPrecision_ /* = 2 */)
{
	//计算key
	int lnCharSize = sizeof(E_SpectrumType)+sizeof(E_WINOW_TYPE)+sizeof(int);
	std::vector<CHAR> loBuffer;
	loBuffer.resize(lnCharSize);

	CHAR * lpKey = & loBuffer.front();

	memcpy(lpKey,&type,sizeof(E_SpectrumType));//type
	memcpy(&lpKey[sizeof(E_SpectrumType)],&iWinType_,sizeof(E_WINOW_TYPE));//iWinType_
	memcpy(&lpKey[sizeof(E_SpectrumType)+sizeof(E_WINOW_TYPE)],&iPrecision_,sizeof(int));//iWinType_
	int lnKey = BufferedBaseChannSigMathBasedOnSpecChann::Hash(lpKey,lnCharSize);

	std::map<int,ValueType>::iterator loItBuffer = this->m_oMapBuffer.find(lnKey);

	if (loItBuffer!= this->m_oMapBuffer.end())
	{
		double * lpBuffer = &loItBuffer->second.front();
		if (vOut_.size()<loItBuffer->second.size())		{
			vOut_.resize(loItBuffer->second.size());
		}
		memcpy(&vOut_.front(),lpBuffer,loItBuffer->second.size());
		return 1;

	}else
	{
		try
		{
			int lnRet = this->m_oSigMath.SpectralMeasureMent(	pwave, 
				n,
				vOut_, 
				type, 
				iWinType_ /* = WINDOW_HANNING */, 
				iPrecision_ /* = 2 */);

			if (lnRet>=0)
			{
				std::vector<double> & lRefBuffer = this->m_oMapBuffer[lnKey];
				lRefBuffer.resize(n/2);
				double * lpBuffer =& lRefBuffer.front();
				memcpy(lpBuffer,&vOut_.front(),lRefBuffer.size());

			}

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}


		return -1;


	}

}

int BufferedBaseChannSigMathBasedOnSpecChann::SpectralMeasureMent(const double *pwave, 
																  int n,
																  double *pout, 
																  E_SpectrumType type, 
																  E_WINOW_TYPE iWinType_ /* = WINDOW_HANNING */, 
																  unsigned int iPrecision_ /* = 2 */)
{
	//计算key
	int lnCharSize = sizeof(E_SpectrumType)+sizeof(E_WINOW_TYPE)+sizeof(int);
	std::vector<CHAR> loBuffer;
	loBuffer.resize(lnCharSize);

	CHAR * lpKey = & loBuffer.front();

	memcpy(lpKey,&type,sizeof(E_SpectrumType));//type
	memcpy(&lpKey[sizeof(E_SpectrumType)],&iWinType_,sizeof(E_WINOW_TYPE));//iWinType_
	memcpy(&lpKey[sizeof(E_SpectrumType)+sizeof(E_WINOW_TYPE)],&iPrecision_,sizeof(int));//iWinType_
	int lnKey = BufferedBaseChannSigMathBasedOnSpecChann::Hash(lpKey,lnCharSize);

	std::map<int,ValueType>::iterator loItBuffer = this->m_oMapBuffer.find(lnKey);

	if (loItBuffer!= this->m_oMapBuffer.end())
	{
		double * lpBuffer = &loItBuffer->second.front();

		int lnBufferCopyed = n/2;

		if (lnBufferCopyed>loItBuffer->second.size())
		{
			lnBufferCopyed = loItBuffer->second.size();
		}

		memcpy(pout,lpBuffer,lnBufferCopyed);

		return 1;

	}else
	{
		try
		{
			int lnRet = this->m_oSigMath.SpectralMeasureMent(	pwave, 
				n,
				pout, 
				type, 
				iWinType_ /* = WINDOW_HANNING */, 
				iPrecision_ /* = 2 */);

			if (lnRet>=0)
			{
				std::vector<double> & lRefBuffer = this->m_oMapBuffer[lnKey];
				lRefBuffer.resize(n/2);
				double * lpBuffer =& lRefBuffer.front();
				memcpy(lpBuffer,pout,lRefBuffer.size());

			}

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}


		return -1;


	}
}

int BufferedBaseChannSigMathBasedOnSpecChann::SpectrumIntegrate(double * pSpectrum_, 
																const double& df_,
																const int& ilen_, 
																const double& fSensitivity_/* = 1.0 */,
																E_SpectrumType eSpectrumType_ /* = E_SpectrumType_PEAK */, 
																const int& iStartidx_ /* = 2 */)
{
	//计算key
	int lnCharSize = sizeof(double)+sizeof(E_SpectrumType)+sizeof(iStartidx_);
	std::vector<CHAR> loBuffer;
	loBuffer.resize(lnCharSize);

	CHAR * lpKey = & loBuffer.front();

	memcpy(lpKey,&fSensitivity_,sizeof(double));//fSensitivity_
	memcpy(&lpKey[sizeof(double)],&eSpectrumType_,sizeof(E_SpectrumType));//eSpectrumType_
	memcpy(&lpKey[sizeof(double)+sizeof(E_SpectrumType)],&iStartidx_,sizeof(int));//iStartidx_
	int lnKey = BufferedBaseChannSigMathBasedOnSpecChann::Hash(lpKey,lnCharSize);

	std::map<int,ValueType>::iterator loItBuffer = this->m_oMapBuffer.find(lnKey);

	if (loItBuffer!= this->m_oMapBuffer.end() )
	{
		double * lpBuffer = &loItBuffer->second.front();

		int lnBufferCopyed =ilen_;

		if (lnBufferCopyed>loItBuffer->second.size())
		{
			lnBufferCopyed = loItBuffer->second.size();
		}

		memcpy(pSpectrum_,lpBuffer,lnBufferCopyed);

		return 1;

	}else
	{
		try
		{
			int lnRet = this->m_oSigMath.SpectrumIntegrate(pSpectrum_, 
				df_,
				ilen_, 
				fSensitivity_/* = 1.0 */,
				eSpectrumType_ /* = E_SpectrumType_PEAK */, 
				iStartidx_ /* = 2 */);

			if (lnRet>0)
			{
				std::vector<double> & lRefBuffer = this->m_oMapBuffer[lnKey];
				lRefBuffer.resize(ilen_);
				double * lpBuffer =& lRefBuffer.front();
				memcpy(lpBuffer,pSpectrum_,lRefBuffer.size());

			}

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);	
		}


		return -1;


	}
}

static __time64_t COleDateTimeToI64(const CBHDateTime &dtTime_)
{
	SYSTEMTIME sysTime;

	if(!dtTime_.GetAsSystemTime(sysTime))
	{
		return (__time64_t)0L;
	}

	CTime tTime(sysTime);

	return tTime.GetTime();
}

/*
InterlockedXX64无法在Windows XP上使用，故直接使用lock cmpxchg8b实现

CMPXCHAGE8B 见 《Intel 64 and IA-32 Architectures Software Developer’s Manual》 7.3.1.2

DST, ECX:EBX, EDX:EAX

伪代码:
	if DST == EDX:EAX:
		DST = ECX:EBX
	else:
		EDX:EAX = DST

#define USE_INTERLOCKED_API
*/

__int64 __stdcall InterlockedRead64(__int64 volatile * DST)
{
 #ifdef USE_INTERLOCKED_API

	return InterlockedExchangeAdd64(DST, 0);

#else
	__asm
	{
		push edi
		push ebx
		mov edi, DST
		xor edx, edx
		xor eax, eax
		lock cmpxchg8b [edi]
		pop ebx
		pop edi
	}
#endif
}

void __stdcall InterlockedWrite64(__int64 volatile * DST, __int64 VAL)
{
 #ifdef USE_INTERLOCKED_API

	(void)InterlockedExchange64(DST, VAL);

#else
	__asm
	{
		push edi
		push ebx
		mov edi, DST
		lea ecx, [VAL]
		mov ebx, dword ptr [ecx]
		mov ecx, dword ptr [ecx +4]
		mov eax, [edi]
		mov edx, [edi+4]
RETRY:
		lock cmpxchg8b [edi]
		jnz RETRY
		pop ebx
		pop edi
	}
#endif
}

CPlantInfoOnLine::CPlantInfoOnLine(void)
{
	_pLockVibData=new CTimeOutReadWriteLock();
	_pLockDataUpdateStatues=new CTimeOutReadWriteLock();
	_pLockProcData = _pLockVibData;
	_pLockAxisLocationData = _pLockVibData;
	_pLockDynData = _pLockVibData;

	CHZLogManage::Info("construction","CPlantInfoOnLine","CPlantInfoOnLine");
	_bProcDataUpdate=false;
	_bDynDataUpdate=false;
	_bVibDataUpdate=false;
	_bVibChannInfoUpdate=false;
	_bDynChannInfoUpdate=false;
	_vAxisChannValue.clear();
	_iRealSmpNum=1;
	_iRealSmpFreq=1;
	_iDynRealSmpNum=1;
	_iDynRealSmpFreq=1;
	_pWave          = NULL;
	_pWaveDyn       =NULL;
	_iType=GE_SUD_START;
	_bSudStatus = false;
	for (int loop = 0; loop < 5; ++loop){
		_pfLastRev[loop] =-1.0;
		_pfRealRev[loop] =-1.0;
	}

	this->MaxRealRev(0);

	//临时在线，启停车用到的相关参数
	_Timesud =CBHDateTime::GetCurrentTime();
	_iMicroSecond=0;
	_iCurrEventID=-1;
	_iEventID = -1;

	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
}

CPlantInfoOnLine::~CPlantInfoOnLine(void)
{
	Destory();
}

void CPlantInfoOnLine::Destory()
{

	V_PCHANNINFO::iterator it=_vVibChann.begin();
	CVibChannParam *pVibChann=NULL;
	for (;it!=_vVibChann.end();++it)
	{
		pVibChann=(CVibChannParam*)(*it);
		delete pVibChann;
		pVibChann=NULL;
	}


	{
		CProcChannParam *pProcChann=NULL;
		it=_vProcChann.begin();
		for(;it!=_vProcChann.end();++it)
		{
			pProcChann=(CProcChannParam*)(*it);
			delete pProcChann;
			pProcChann=NULL;
		}
	}
	{
		CAxisChannParam *pAxisChann=NULL;
		V_PAXISCHANNINFO::iterator itAxis=_vAxisChann.begin();
		for (;itAxis!=_vAxisChann.end();++itAxis)
		{
			pAxisChann=(CAxisChannParam*)(*itAxis);
			delete pAxisChann;
			pAxisChann=NULL;
		}
	}
	_vProcChann.clear();
	_vVibChann.clear();
	_vAxisChann.clear();
	_vAxisChannValue.clear();
	_mapProcChannIndex.clear();
	_mapVibChannIndex.clear();

	try
	{

		KD_DeleteSinglePoint(_pLockDataUpdateStatues);
		KD_DeleteSinglePoint(_pLockVibData);

		// 		KD_DeleteSinglePoint(_pLockProcData);	
		// 		KD_DeleteSinglePoint(_pLockAxisLocationData);
		// 		KD_DeleteSinglePoint(_pLockDynData);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

}

void  CPlantInfoOnLine::SetSudStatus(bool bSud_, E_SUD_TYPE iType_)
{
	_pLockVibData->BeginWrite();
	_bSudStatus = bSud_;
	_iType = iType_;
	_pLockVibData->EndWrite();
}

int CPlantInfoOnLine::GetSudStatus(bool & bSud_ )
{
	int iType = -1;
	_pLockVibData->BeginRead();
	iType = _iType;
	bSud_ = _bSudStatus;
	_pLockVibData->EndRead();
	return iType;
}

int CPlantInfoOnLine::ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,S_OldVibCharValue *pOldVibCharValue_,float *pfRev_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//转化特征值数据
		_pLockVibData->BeginWrite();
		try
		{
			for(int loop=iStartIndex_;loop<(iStartIndex_ + iLen_);++loop)
			{
				float fRev=.0;
				int iRevType=((CVibChannParam*)_vVibChann[loop])->GetChannInfo()._iRevType;;
				switch(iRevType)
				{
				case 1:
					fRev = pfRev_[0];
					break;
				case 2:
					fRev = pfRev_[1];
					break;
				case 3:
					fRev = pfRev_[2];
					break;
				case 4:
					fRev = pfRev_[3];
					break;
				case 5:
					fRev = pfRev_[4];
					break;
				default:
					fRev = pfRev_[0];
				}
				pVibCharValue_[nCount]._fCharAll=pOldVibCharValue_[nCount]._fCharAll;
				pVibCharValue_[nCount]._fCharHalf=pOldVibCharValue_[nCount]._fCharHalf;
				pVibCharValue_[nCount]._fCharOne=pOldVibCharValue_[nCount]._fCharOne;
				pVibCharValue_[nCount]._fCharTwo=pOldVibCharValue_[nCount]._fCharTwo;
				pVibCharValue_[nCount]._fCharThree=pOldVibCharValue_[nCount]._fCharThree;
				pVibCharValue_[nCount]._fCharRes=pOldVibCharValue_[nCount]._fCharRes;
				pVibCharValue_[nCount]._fDC=pOldVibCharValue_[nCount]._fDC;
				pVibCharValue_[nCount]._fGap=pOldVibCharValue_[nCount]._fGap;
				pVibCharValue_[nCount]._fPhaseHalf=pOldVibCharValue_[nCount]._fPhaseHalf;
				pVibCharValue_[nCount]._fPhaseOne=pOldVibCharValue_[nCount]._fPhaseOne;
				pVibCharValue_[nCount]._fPhaseTwo=pOldVibCharValue_[nCount]._fPhaseTwo;
				pVibCharValue_[nCount]._fPhaseThree=pOldVibCharValue_[nCount]._fPhaseThree;
				pVibCharValue_[nCount]._iSmpFreq=iSmpFreq_;
				pVibCharValue_[nCount]._iSmpNum=iSmpNum_;
				ASSERT( pVibCharValue_[nCount]._iSmpNum!=0);
				pVibCharValue_[nCount]._iRev=(int)fRev;
				//CString sTemp;sTemp.Format("Vib:_fCharAll,%f,_fCharOne:%f,_fCharTwo:%f",pVibCharValue_[nCount]._fCharAll,pVibCharValue_[nCount]._fCharOne,pVibCharValue_[nCount]._fCharTwo);BHTRACE_DEBUG(sTemp);
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndWrite();
	}
	return nCount;
}

int CPlantInfoOnLine::UpdateVibData(DATATYPE_WAVE *pWave_,
									S_VibCharValue *pVibCharValue_,
									int iAllRealWavePointNum_,
									int iLen_,
									CBHDateTime & aoDataTime,
									int iStartIndex_,
									const float& fScaleCoef_,
									int anPrecisionEnabled,
									std::vector<float>& afPreciousMin ,
									std::vector<float>&  afPreciousMax,
									bool bEnd_/* =false */)
{
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
	{
		ASSERT(FALSE);
		nCount=-1;
	}
	else
	{

		if (bEnd_)
		{
			SetVibDataUpdateStatus(aoDataTime);
			SetRealAllVibWaveNum(_iVibAllWaveNum,true);
			_iVibAllWaveNum=0;
		}

		//进行数据更新
		_iVibAllWaveNum=_iVibAllWaveNum+iAllRealWavePointNum_;
		int iRealSmpFreq=pVibCharValue_[nCount]._iSmpFreq > _sPlantInfo._iSmpFreq? _sPlantInfo._iSmpFreq:pVibCharValue_[nCount]._iSmpFreq;
		int iRealSmpNum=pVibCharValue_[nCount]._iSmpNum> _sPlantInfo._iSmpNum? _sPlantInfo._iSmpNum:pVibCharValue_[nCount]._iSmpNum;
		SetRealSmpInfo(iRealSmpNum, iRealSmpFreq, true);
		long iRealPointNum=0;
		float pfRev[5]={.0};
		GetRealRev(pfRev, 5);

		CWaveHead sWaveHead;
		sWaveHead.SetLength(4);

		_pLockVibData->BeginWrite();

		try
		{
			if(iStartIndex_ == 0)
			{
				for(int loop = 0; loop < _vVibChann.size(); ++loop)
				{
					((CVibChannParam*)_vVibChann[loop])->SetChannUpdate(false);
				}
			}
			for(int loop=iStartIndex_;loop<(iStartIndex_ + iLen_);++loop)
			{
				int iRevType=((CVibChannParam*)_vVibChann[loop])->GetChannInfo()._iRevType;;
				switch(iRevType)
				{
				case 1:
					pfRev[0] = pVibCharValue_[nCount]._iRev;
					break;
				case 2:
					pfRev[1] = pVibCharValue_[nCount]._iRev;
					break;
				case 3:
					pfRev[2] = pVibCharValue_[nCount]._iRev;
					break;
				case 4:
					pfRev[3] = pVibCharValue_[nCount]._iRev;
					break;
				case 5:
					pfRev[4] = pVibCharValue_[nCount]._iRev;
					break;
				default:
					pfRev[0] = pVibCharValue_[nCount]._iRev;
				}
				
				if ( 0 == loop)
				{
					this->MaxRealRev(0);
				}

				if(iRevType > 0 && iRevType <= 5)
				{
					_pfRealRev[iRevType - 1] = pVibCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<_pfRealRev[iRevType - 1])
					{
						m_fMaxRealRev = _pfRealRev[iRevType - 1];
					}
				}
				else 
				{
					_pfRealRev[0] = pVibCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<	_pfRealRev[0])
					{
						m_fMaxRealRev =_pfRealRev[0];
					}
				}

				((CVibChannParam*)_vVibChann[loop])->SetLastUpdateTime(CBHDateTime::GetCurrentTime());

				((CVibChannParam*)_vVibChann[loop])->SetChannValue(&(pVibCharValue_[nCount]));

				((CVibChannParam*)_vVibChann[loop])->SetChannUpdate(true);

				//if(Datasource::HasOnline((unsigned int)this->_sPlantInfo._iDataSource))
				{
					if ((iRealPointNum+pVibCharValue_[nCount]._iSmpNum) <= iAllRealWavePointNum_)
					{
						if(pWave_)
						{
							((CVibChannParam*)_vVibChann[loop])->SetChannWave(&(pWave_[iRealPointNum]),pVibCharValue_[nCount]._iSmpNum);
						}
						else
						{
							((CVibChannParam*)_vVibChann[loop])->SetChannWave(NULL,0);
						}
						
					}else
					{
						CString lstrErrorLog;
						lstrErrorLog.Format("Error:24005接口 %s|%s|%s 一次更新的累计的采样点数iRealPointNum(%d) >=  数采传送过来的总采样点数 iAllRealNum_(%d)",
							this->_sCompany.c_str(),
							this->_sFactory.c_str(),
							this->_sSetID.c_str(),
							iRealPointNum,
							iAllRealWavePointNum_);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

				}

				iRealPointNum+=pVibCharValue_[nCount]._iSmpNum;

				if(IsFloatZero(fScaleCoef_))
				{
					sWaveHead.m_pParam[0] = 10.0;
				}
				else
				{
					sWaveHead.m_pParam[0] = fScaleCoef_;
				}

				if(anPrecisionEnabled)
				{
					sWaveHead.m_pParam[1] = 1;
					sWaveHead.m_pParam[2] = afPreciousMin[nCount];
					sWaveHead.m_pParam[3] = afPreciousMax[nCount];
				}
				else
				{
					sWaveHead.m_pParam[1] = 0;

				}

				((CVibChannParam*)_vVibChann[loop])->WaveHead(sWaveHead);

				nCount++;

			}

			this->MaxRealRevLast(this->MaxRealRev());
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		
		_pLockVibData->EndWrite();
		//更新转速
		// UpdateRealRev(pfRev);
	}
	return nCount;
}

void CPlantInfoOnLine::SyncVibData()
{
	try
	{

		for (int i =0;i<_vVibChann.size();i++)
		{
			((CVibChannParamAlarm*)_vVibChann[i])->SyncData();
		}

		LY::PLANTINFO_STATUS loStatus;
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		CBHDateTime loCurrentTime = CBHDateTime::GetCurrentTimeEx();
		loStatus.m_strVibUpdateTime = loCurrentTime.Format();
		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}
}


void CPlantInfoOnLine::SyncDynData()
{
	try
	{
		for (int i =0;i<_vDynChann.size();i++)
		{
			((CVibChannParamAlarm*)_vDynChann[i])->SyncData();
		}

		LY::PLANTINFO_STATUS loStatus;
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		CBHDateTime loCurrentTime = CBHDateTime::GetCurrentTimeEx();
		loStatus.m_strDynUpdateTime = loCurrentTime.Format();
		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}
}

int CPlantInfoOnLine::ChangeDynCharValueToNew(S_DynCharValue *pDynCharValue_,S_OldDynCharValue *pOldDynCharValue_,float *pfRev_,const int &iSmpNum_, const int &iSmpFreq_, const int iRealNum_,const int &iLen_,const int &iStartIndex_)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//转化特征值数据
		_pLockDynData->BeginWrite();
		try
		{
			for(int loop=iStartIndex_; loop< (iStartIndex_ + iLen_); ++loop)
			{
				float fRev=.0;
				int iRevType=((CDynChannParam*)_vDynChann[loop])->GetChannInfo()._iRevType;
				switch(iRevType)
				{
				case 1:
					fRev = pfRev_[0];
					break;
				case 2:
					fRev = pfRev_[1];
					break;
				case 3:
					fRev = pfRev_[2];
					break;
				case 4:
					fRev = pfRev_[3];
					break;
				case 5:
					fRev = pfRev_[4];
					break;
				default:
					fRev = pfRev_[0];
				}
				pDynCharValue_[nCount]._fCharValue0=pOldDynCharValue_[nCount]._fCharValue0;
				pDynCharValue_[nCount]._fCharValue1=pOldDynCharValue_[nCount]._fCharValue1;
				pDynCharValue_[nCount]._fCharValue2=pOldDynCharValue_[nCount]._fCharValue2;
				pDynCharValue_[nCount]._fCharValue3=pOldDynCharValue_[nCount]._fCharValue3;
				pDynCharValue_[nCount]._fCharValue4=pOldDynCharValue_[nCount]._fCharValue4;
				pDynCharValue_[nCount]._fCharValue5=pOldDynCharValue_[nCount]._fCharValue5;
				pDynCharValue_[nCount]._fCharValue6=pOldDynCharValue_[nCount]._fCharValue6;
				pDynCharValue_[nCount]._fCharValue7=pOldDynCharValue_[nCount]._fCharValue7;
				pDynCharValue_[nCount]._fCharValue8=pOldDynCharValue_[nCount]._fCharValue8;
				pDynCharValue_[nCount]._fCharValue9=pOldDynCharValue_[nCount]._fCharValue9;
				pDynCharValue_[nCount]._fCharValue10=pOldDynCharValue_[nCount]._fCharValue10;
				pDynCharValue_[nCount]._fCharValue11=pOldDynCharValue_[nCount]._fCharValue11;
				pDynCharValue_[nCount]._iSmpFreq=iSmpFreq_;
				pDynCharValue_[nCount]._iSmpNum=iSmpNum_;
				pDynCharValue_[nCount]._iRev=(int)fRev;
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockDynData->EndWrite();
	}
	return nCount;
}

int CPlantInfoOnLine::UpdateDynData(DATATYPE_WAVE *pWave_,
									S_DynCharValue *pDynCharValue_,
									int iAllRealNum_,
									const int &iLen_,
									CBHDateTime & arefUpdateTime,
									const int &iStartIndex_, 
									const float& fScaleCoef_,
									int anPrecisionEnabled,
									std::vector<float>& afPreciousMin ,
									std::vector<float>&  afPreciousMax,
									const bool &bEnd_/* =false */)
{
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	//CString strInfo;strInfo.Format(" %d,%d,%d",iLen_,iStartIndex_,_sPlantInfo._iDynChannNum);BHTRACE_DEBUG(strInfo);
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		if (bEnd_)
		{
			SetDynDataUpdateStatus(arefUpdateTime);
			SetRealAllDynWaveNum(_iDynAllWaveNum,true);
			_iDynAllWaveNum=0;
		}
		//进行数据更新
		_iDynAllWaveNum=_iDynAllWaveNum+iAllRealNum_;
		int iRealSmpFreq = pDynCharValue_[nCount]._iSmpFreq > _sPlantInfo._iSmpFreq ? _sPlantInfo._iSmpFreq : pDynCharValue_[nCount]._iSmpFreq;
		int iRealSmpNum = pDynCharValue_[nCount]._iSmpNum> _sPlantInfo._iSmpNum? _sPlantInfo._iSmpNum : pDynCharValue_[nCount]._iSmpNum;
		SetDynRealSmpInfo(iRealSmpNum, iRealSmpFreq, true);
		long iRealPointNum=0;

		float pfRev[5]={.0};
		GetRealRev(pfRev, 5);

		CWaveHead sWaveHead;
		sWaveHead.SetLength(4);

		_pLockDynData->BeginWrite();
		try
		{
			if(iStartIndex_ == 0)
			{
				for(int loop = 0; loop < _vDynChann.size(); ++loop)
				{
					((CDynChannParam*)_vDynChann[loop])->SetChannUpdate(false);
				}
			}

			for(int loop=iStartIndex_; loop< (iStartIndex_ + iLen_); ++loop)
			{
				((CDynChannParam*)_vDynChann[loop])->SetLastUpdateTime(CBHDateTime::GetCurrentTime());

				((CDynChannParam*)_vDynChann[loop])->SetChannUpdate(true);

				int iRevType=((CDynChannParam*)_vDynChann[loop])->GetChannInfo()._iRevType;;
				switch(iRevType)
				{
				case 1:
					pfRev[0] = pDynCharValue_[nCount]._iRev;
					break;
				case 2:
					pfRev[1] = pDynCharValue_[nCount]._iRev;
					break;
				case 3:
					pfRev[2] = pDynCharValue_[nCount]._iRev;
					break;
				case 4:
					pfRev[3] = pDynCharValue_[nCount]._iRev;
					break;
				case 5:
					pfRev[4] = pDynCharValue_[nCount]._iRev;
					break;
				default:
					pfRev[0] = pDynCharValue_[nCount]._iRev;
				}

				if ( 0 == loop)
				{
					this->MaxRealRev(0);
				}

				if(iRevType > 0 && iRevType <= 5)
				{
					_pfRealRev[iRevType - 1] = pDynCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<	_pfRealRev[iRevType - 1])
					{
						m_fMaxRealRev =_pfRealRev[iRevType - 1];
					}
				}
				else {
					_pfRealRev[0] = pDynCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<	_pfRealRev[0])
					{
						m_fMaxRealRev =_pfRealRev[0];
					}
				}

				((CDynChannParam*)_vDynChann[loop])->SetChannValue(&(pDynCharValue_[nCount]));

				if ((iRealPointNum+pDynCharValue_[nCount]._iSmpNum) <= iAllRealNum_)
				{
					((CDynChannParam*)_vDynChann[loop])->SetChannWave(&(pWave_[iRealPointNum]),pDynCharValue_[nCount]._iSmpNum);
				}else
				{
					CString lstrErrorLog;
					lstrErrorLog.Format("Error:24001接口 %s|%s|%s 一次更新的累计的采样点数iRealPointNum(%d) >=  数采传送过来的总采样点数 iAllRealNum_(%d)",
						this->_sCompany.c_str(),
						this->_sFactory.c_str(),
						this->_sSetID.c_str(),
						iRealPointNum,
						iAllRealNum_);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				iRealPointNum+=pDynCharValue_[nCount]._iSmpNum;

				if(IsFloatZero(fScaleCoef_))
				{
					sWaveHead.m_pParam[0] = 10.0;
				}
				else
				{
					sWaveHead.m_pParam[0] = fScaleCoef_;
				}

				if(anPrecisionEnabled)
				{
					sWaveHead.m_pParam[1] = 1;
					sWaveHead.m_pParam[2] = afPreciousMin[nCount];
					sWaveHead.m_pParam[3] = afPreciousMax[nCount];
				}
				else
				{
					sWaveHead.m_pParam[1] = 0;

				}
				((CDynChannParam*)_vDynChann[loop])->WaveHead(sWaveHead);


				nCount++;

				if (bEnd_)
				{
					((CDynChannParam*)_vDynChann[loop])->SyncData();
				}
			}
			this->MaxRealRevLast(this->MaxRealRev());
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockDynData->EndWrite();

		//更新转速
		//UpdateRealRev(pfRev);
	}
	return nCount;
}

int CPlantInfoOnLine::UpdateProcData(float *pfData_,int iLen_,const CBHDateTime& tDateTime,int iStartIndex_)
{
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	int nCount=0;
	int iloop=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iProcChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		SetProcDataUpdateStatus(tDateTime);
		CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();
		LY::PLANTINFO_STATUS loStatus;
		_pLockDataUpdateStatues->BeginRead();
		dtTmp=_timeProcDataUpdate;
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;
		CBHDateTime loCurrentTime = CBHDateTime::GetCurrentTimeEx();
		loStatus.m_strProcUpdateTime = loCurrentTime.Format();

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		_pLockDataUpdateStatues->EndRead();

		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}

		iLen_=iLen_+iStartIndex_;
		nCount=0;
		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=iStartIndex_;iloop<iLen_;++iloop)
			{
				((CProcChannParam*)_vProcChann[iloop])->SetData(pfData_[nCount],tDateTime);

				((CProcChannParam*)_vProcChann[iloop])->SetLastUpdateTime(dtTmp);

				((CProcChannParam*)_vProcChann[iloop])->SetChannUpdate(true);
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}
	return nCount;
}

int CPlantInfoOnLine::UpdateProcSUDStatus(int anProcJudgedPlantStatus,int anLen_,int anStartIndex_)
{
	int nCount=0;
	int iloop=0;
	if (((anLen_+anStartIndex_)> _sPlantInfo._iProcChannNum)||(anLen_<1)||(anStartIndex_<0))
		nCount=-1;
	else
	{
		anLen_=anLen_+anStartIndex_;
		nCount=0;
		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=anStartIndex_;iloop<anLen_;++iloop)
			{
				((CProcChannParam*)_vProcChann[iloop])->SetSUDStatus(anProcJudgedPlantStatus);
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}
	return nCount;
}

int CPlantInfoOnLine::GetProcSUDStatus()
{
	int lnSpeedCount = 0;

	int lnStopCount = 0;

	bool lbJudgeSUDStatus = false;

	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			bool lbUpdateDataOverTime = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->IsChannUpdateDataOverTime();
			//如果更新数据时间超时则认为无效，不参与开停车判断
			if (lbUpdateDataOverTime)
			{
				continue;
			}

			int lnProcSUDStatus = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->GetSUDStatus();

			//0表示不参与判断开停车，1表示开车，2表示停车
			if (lnProcSUDStatus == 0)
			{    
				continue;
			}

			lbJudgeSUDStatus = true;

			if (lnProcSUDStatus == 1)
			{
				++lnSpeedCount;
			}
			else if(lnProcSUDStatus == 2)
			{
				++lnStopCount;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	if (!lbJudgeSUDStatus)
	{
		return 0;
	}

	return lnStopCount<lnSpeedCount?1:2;
}

bool CPlantInfoOnLine::IsAllChannUpdate()
{
	bool lbChannUpdate = false;

	_pLockVibData->BeginRead();

	try
	{
		for (int lnVibChannIndex = 0; lnVibChannIndex < _sPlantInfo._iVibChannNum; lnVibChannIndex++)
		{
			lbChannUpdate = ((CVibChannParam*)_vVibChann[lnVibChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				break;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();

	_pLockDynData->BeginRead();

	try
	{
		for (int lnDynChannIndex = 0; lnDynChannIndex < _sPlantInfo._iDynChannNum; lnDynChannIndex++)
		{
			lbChannUpdate = ((CDynChannParam*)_vDynChann[lnDynChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				break;
			}
		}

	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockDynData->EndRead();


	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			lbChannUpdate = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				break;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	return lbChannUpdate;
}


bool CPlantInfoOnLine::IsHasChannNotUpdate()
{
	bool lbChannUpdate = false;

	int lnUpdateChannCount = 0;

	_pLockVibData->BeginRead();

	try
	{
		for (int lnVibChannIndex = 0; lnVibChannIndex < _sPlantInfo._iVibChannNum; lnVibChannIndex++)
		{
			lbChannUpdate = ((CVibChannParam*)_vVibChann[lnVibChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				continue;
			}

			lnUpdateChannCount++;
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();

	_pLockDynData->BeginRead();

	try
	{
		for (int lnDynChannIndex = 0; lnDynChannIndex < _sPlantInfo._iDynChannNum; lnDynChannIndex++)
		{
			lbChannUpdate = ((CDynChannParam*)_vDynChann[lnDynChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				continue;
			}

			lnUpdateChannCount++;
		}

	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockDynData->EndRead();


	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			lbChannUpdate = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->GetChannUpdate();

			if (!lbChannUpdate)
			{
				continue;
			}

			lnUpdateChannCount++;
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	int lnChannCount = _sPlantInfo._iVibChannNum + _sPlantInfo._iDynChannNum + _sPlantInfo._iProcChannNum;

	if (lnUpdateChannCount > 0 && lnUpdateChannCount < lnChannCount)
	{
		return true;
	}

	return false;
}

bool CPlantInfoOnLine::IsPlantUpdateOverTime()
{
	CBHDateTime loDateTimeLastUpdate,loLeastUpdateTime;

	bool lbInit = true;

	_pLockVibData->BeginRead();

	try
	{
		for (int lnVibChannIndex = 0; lnVibChannIndex < _sPlantInfo._iVibChannNum; lnVibChannIndex++)
		{
			loDateTimeLastUpdate = ((CVibChannParam*)_vVibChann[lnVibChannIndex])->GetLastUpdateTime();

			if(lbInit)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
				lbInit = false;
			}

			if (loDateTimeLastUpdate < loLeastUpdateTime)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();

	_pLockDynData->BeginRead();

	try
	{
		for (int lnDynChannIndex = 0; lnDynChannIndex < _sPlantInfo._iDynChannNum; lnDynChannIndex++)
		{
			loDateTimeLastUpdate = ((CDynChannParam*)_vDynChann[lnDynChannIndex])->GetLastUpdateTime();

			if(lbInit)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
				lbInit = false;
			}

			if (loDateTimeLastUpdate < loLeastUpdateTime)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
			}
		}

	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockDynData->EndRead();

	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			loDateTimeLastUpdate = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->GetLastUpdateTime();

			CString lstr = loDateTimeLastUpdate.Format();

			if(lbInit)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
				lbInit = false;
			}

			if (loDateTimeLastUpdate < loLeastUpdateTime)
			{
				loLeastUpdateTime = loDateTimeLastUpdate;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	CBHDateTime loCurrrentTime = CBHDateTime::GetCurrentTime();

	COleDateTimeSpan loTimeSpan = loCurrrentTime - loLeastUpdateTime;
	
	if (loTimeSpan.GetTotalMinutes() < g_UpdatePlantDataOverTime)
	{
		return false;
	}

	return true;
}

void CPlantInfoOnLine::UpdateRealRev(const float *pfRev_)
{
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());

	float fRev[5];

	_pLockVibData->BeginRead();

	try
	{
		GetRealRev(fRev,5);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();


	_pLockVibData->BeginWrite();
	/* 保存上次转速*/
	try
	{
		::memcpy(_pfLastRev, fRev, sizeof(fRev));
		_tLastRevTime = CBHDateTime::GetCurrentTime();

		this->m_fMaxRealRev = _pfRealRev[0];

		for (int nloop=0;nloop<5;++nloop)
		{
			_pfRealRev[nloop] = pfRev_[nloop];

			if (this->m_fMaxRealRev<_pfRealRev[nloop])
			{
				this->m_fMaxRealRev = _pfRealRev[nloop];
			}
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	//CString strInfo; strInfo.Format("%.2f,%.2f,%.2f,%.2f,%.2f", _pfRealRev[0], _pfRealRev[1], _pfRealRev[2],_pfRealRev[3],_pfRealRev[4]);BHTRACE_DEBUG(strInfo);
	//CHZLogManage::Debug(strInfo, "Online", "CPlantInfoOnLine::UpdateRealRev");
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::AddCustomParamInfo(CCustomDB *pDB_)
{
	int index=0;
	vector<CCustomParamInfo> vCustomParamInfo;
	//添加振动自定义参数信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		//获取自定义参数数据
		if (strcmp(pVibChann->GetChannInfo()._cTempID,"") != 0)
		{
			SwichDbName(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cCustom_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cTempletID,pVibChann->GetChannInfo()._cTempID,gc_cPlantNo,this->_sPlantInfo._cPlantNo);
			while (pDB_->More())
			{
				CCustomParamInfo paramInfo;
				paramInfo.GetCustomParamInfo(pDB_);
				vCustomParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Vib自定义参数数据条数 index :%d,%d",index,vCustomParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pVibChann->SetCustomParamInfo(0,index,&vCustomParamInfo[0]);
			vCustomParamInfo.clear();
		}
	}
	//添加动态自定义参数信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		//获取自定义参数数据
		if (strcmp(pDynChann->GetChannInfo()._cTempID, "") != 0)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cCustom_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cTempletID,pDynChann->GetChannInfo()._cTempID,gc_cPlantNo,this->_sPlantInfo._cPlantNo);
			while (pDB_->More())
			{
				CCustomParamInfo paramInfo;
				paramInfo.GetCustomParamInfo(pDB_);
				vCustomParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Dyn自定义参数数据条数 index :%d,%d",index,vCustomParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pDynChann->SetCustomParamInfo(0,index,&vCustomParamInfo[0]);
			vCustomParamInfo.clear();
		}
	}
}

void CPlantInfoOnLine::AddChannLocationsInfo(CCustomDB *pDB_)
{
	int index=0;
	//添加振动测点位置信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		CChannLocations locationsInfo;
		//获取位置数据
		if (pVibChann->GetChannInfo()._iLocationIdx>-1)
		{
			SwichDbName(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pVibChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pVibChann->SetChannLocationsInfo(locationsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Vib测点位置数据条数 index :%d",index);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannLocationsInfo");
			}
		}
		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;
		CString lstrSqlGetBear;
		CString lstrTemp;
		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d ",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			// 			pDB_->Cmd("SELECT * FROM [%s]",gc_cBearing_Table);
			// 			pDB_->Cmd("WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{
				pVibChann->SetBearingParamInfo(bearParamInfro);
			}
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pVibChann->SetBearingParamInfo(bearParamInfro);
			}
		}

	}
	//添加动态测点位置信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		//获取位置数据
		CChannLocations locationsInfo;
		if (pDynChann->GetChannInfo()._iLocationIdx>-1)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pDynChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pDynChann->SetChannLocationsInfo(locationsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Dyn测点位置数据条数 index :%d",index);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannLocationsInfo");
			}
		}
		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;

		CString lstrSqlGetBear;
		CString lstrTemp;

		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{			
				pDynChann->SetBearingParamInfo(bearParamInfro);
			}

			// 			pDB_->Cmd("SELECT * FROM [%s]",gc_cBearing_Table);
			// 			pDB_->Cmd("WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pDynChann->SetBearingParamInfo(bearParamInfro);
			}
		}
	}

	//添加过程量测点位置信息
	V_PCHANNINFO::iterator itProc;
	CProcChannParam *pProcChann=NULL;
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{
		index=0;
		pProcChann=(CProcChannParam*)(*itProc);
		//获取位置数据
		CChannLocations locationsInfo;
		if (pProcChann->GetChannInfo()._iLocationIdx>-1)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pProcChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pProcChann->SetChannLocationsInfo(locationsInfo);
				index++;
			}
		}

		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;

		CString lstrSqlGetBear;
		CString lstrTemp;

		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{			
				pProcChann->SetBearingParamInfo(bearParamInfro);
			}
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pProcChann->SetBearingParamInfo(bearParamInfro);
			}
		}
	}
}

void CPlantInfoOnLine::AddChannSectionsInfo(CCustomDB *pDB_)
{
	int index=0;
	//添加振动测点位置信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		//获取部位数据
		CChannSections sectionsInfo;
		/*if (pVibChann->GetChannInfo()._iCylinderNo >1000)*///大于1000为部位号
		{
			SwichDbName(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pVibChann->GetChannInfo()._iCylinderNo);
			if (pDB_->More())
			{
				sectionsInfo.GetChannSections(pDB_);
				pVibChann->SetChannSectionsInfo(sectionsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Vib测点部位数据条数 index :%d,_iGearBoxIDX:%d",index,sectionsInfo._iGearBoxIDX);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannSectionsInfo");
			}
		}
		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		CString lstrSql = pDB_->GetSql();
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pVibChann->SetGearBoxParamInfo(gearBoxInfo);
			//CString sTemp;sTemp.Format("振动测点齿轮箱参数:iGearNum12:%d,iGearNum21:%d,iGearNum22:%d,iGearNum31:%d",gearBoxInfo._iGearNumZ12,gearBoxInfo._iGearNumZ21,gearBoxInfo._iGearNumZ22,gearBoxInfo._iGearNumZ31);
			//CHZLogManage::Info(sTemp,"CPlantInfoOnLine","AddChannSectionsInfo");
		}
	}
	//添加动态测点位置信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		CChannSections sectionsInfo;
		//获取位置数据
		/*if (pDynChann->GetChannInfo()._iCylinderNo>1000)*/
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pDynChann->GetChannInfo()._iCylinderNo);
			if (pDB_->More())
			{
				sectionsInfo.GetChannSections(pDB_);
				pDynChann->SetChannSectionsInfo(sectionsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Dyn测点部位数据条数 index :%d,_iGearBoxIDX:%d",index,sectionsInfo._iGearBoxIDX);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannSectionsInfo");
			}
		}
		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pDynChann->SetGearBoxParamInfo(gearBoxInfo);
			//CString sTemp;sTemp.Format("动态测点齿轮箱参数:iGearNum12:%d,iGearNum21:%d,iGearNum22:%d,iGearNum31:%d",gearBoxInfo._iGearNumZ12,gearBoxInfo._iGearNumZ21,gearBoxInfo._iGearNumZ22,gearBoxInfo._iGearNumZ31);
			//CHZLogManage::Info(sTemp,"CPlantInfoOnLine","AddChannSectionsInfo");
		}
	}

	//添加过程量测点部位信息
	V_PCHANNINFO::iterator itProc;
	CProcChannParam *pProcChann=NULL;
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{
		index=0;
		pProcChann=(CProcChannParam*)(*itProc);
		CChannSections sectionsInfo;

		pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pProcChann->GetChannInfo()._iCylinderNo);
		if (pDB_->More())
		{
			sectionsInfo.GetChannSections(pDB_);
			pProcChann->SetChannSectionsInfo(sectionsInfo);
			index++;
		}

		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pProcChann->SetGearBoxParamInfo(gearBoxInfo);

		}
	}
}

void CPlantInfoOnLine::AddVibChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int index=0;
	S_ChannInfo sChannInfo;
	CVibChannParam *pVibParam=NULL;
	CString strLog="";
	//设置数据处理连接

	_vVibChann.clear();
	_mapVibChannIndex.clear();
	_vAxisChann.clear();
	_vAxisChannValue.clear();
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d ",
		gc_cCompany,
		_sCompany.c_str(),
		gc_cPlantNo,
		_sPlantInfo._cPlantNo,
		gc_cChannType,GE_VIBCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;
	CString lstrQuery = pDB_->GetSql();

	zdlTraceLine(pDB_->GetSql());

	CHZLogManage::Info(pDB_->GetSql(),"CPlantInfo","AddVibChannInfo");

	strLog.Format("%s-%s-%s 添加径向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfo","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CVibChannParam;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND %s =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);
	pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

	strLog.Format("%s-%s-%s 添加轴向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CPlantInfo","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CVibChannParam;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR && 
		_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL )
	{
		//得到轴心位置通道信息
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfo","AddVibChannInfo");
		CAxisChannParam *axisChannParam=NULL;
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannParam=new CAxisChannParam;
			axisChannParam->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannParam->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannParam);
			_vAxisChannValue.push_back(axisData);
		}
	}
	strLog.Format("%s-%s-%s 添加振动通道信息,振动数目%d,轴心位置数目%d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_vVibChann.size(),_vAxisChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfo","AddVibChannInfo");
}
void CPlantInfoOnLine::AddDynChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int index=0;
	S_ChannInfo sChannInfo;
	CDynChannParam *pDynParam=NULL;
	CString strLog="";
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	//振动通道
	pDB_->Cmd(" SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d)",gc_cCompany,_sCompany.c_str(), gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	CHZLogManage::Debug(pDB_->GetSql(),"CPlantInfoOnLine","AddDynChannInfo");

	while (pDB_->More())
	{
		pDynParam=new CDynChannParam;
		pDynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pDynParam->SetChannInfo(sChannInfo);
		_vDynChann.push_back(pDynParam);
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}

	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{
		//得到轴心位置通道信息
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s'  AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfo","AddDynChannInfo");
		CAxisChannParam *axisChannParam=NULL;
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannParam=new CAxisChannParam;
			axisChannParam->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannParam->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannParam);
			_vAxisChannValue.push_back(axisData);
		}
	}

	strLog.Format("%s-%s-%s/%s 添加动态通道信息 %d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_sPlantInfo._cPlantNo,_vDynChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfoOnLine","AddDynChannInfo");
}

void CPlantInfoOnLine::AddProcChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	CProcChannParam *procParam=NULL;
	int index=0;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	S_ChannInfo sChannInfo;
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CProcChannParam;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
}

void CPlantInfoOnLine::AddChannInfoEnd(CCustomDB * pDB_)
{
	_sPlantInfo._iVibChannNum = _vVibChann.size();
	_sPlantInfo._iProcChannNum= _vProcChann.size();
	_sPlantInfo._iDynChannNum = _vDynChann.size();
	_iAxesPairNum=_vAxisChann.size();
	V_PCHANNINFO::iterator it;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{

		if (((*it)->GetChannInfo())._iChannType==GE_VIBCHANN)
			++_iRadialVibChannNum;
		else
			++_iAxialVibChannNum;
	}
	return;
}


void CPlantInfoOnLine::SetVibDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	_pLockDataUpdateStatues->BeginWrite();
	_timeVibDataUpdate= tDatetime_;
	_bVibDataUpdate   = true;
	_bVibChannInfoUpdate=true;
	_pLockDataUpdateStatues->EndWrite();
}

void CPlantInfoOnLine::SetDynDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	_pLockDataUpdateStatues->BeginWrite();
	try
	{
		_timeDynDataUpdate= tDatetime_;
		_bDynDataUpdate   = true;
		_bDynChannInfoUpdate=true;
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}

	_pLockDataUpdateStatues->EndWrite();
}

void CPlantInfoOnLine::SetProcDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	_pLockDataUpdateStatues->BeginWrite();
	_bProcDataUpdate   = true;
	_timeProcDataUpdate= tDatetime_;
	_pLockDataUpdateStatues->EndWrite();
}

CBHDateTime CPlantInfoOnLine::GetRealProcDataDateTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTimeEx();
	_pLockDataUpdateStatues->BeginRead();
	dt=_timeProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	return dt;
}

CBHDateTime CPlantInfoOnLine::GetRealVibDataDateTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTimeEx();
	_pLockDataUpdateStatues->BeginRead();
	dt=_timeVibDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	return dt;
}

CBHDateTime CPlantInfoOnLine::GetRealDynDataDateTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTimeEx();
	_pLockDataUpdateStatues->BeginRead();
	dt=_timeDynDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	return dt;
}

int CPlantInfoOnLine::GetRealRev(float *pfRev_, int iLen_)
{
	if ((iLen_ <1) || (iLen_ >5))
		iLen_ = 1;
	float pRev[5] = {0};
	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	for (int nloop=0;nloop < iLen_;++nloop)
	{
		pfRev_[nloop]= _pfRealRev[nloop];
	}
	END_ERROR_HANDLE
	_pLockVibData->EndRead();

	GetRealRevFromChannel(pRev,5);
	for(int nloop = 0; nloop <iLen_; ++nloop)
	{
		if(pfRev_[nloop] < pRev[nloop])
		{
			pfRev_[nloop] = pRev[nloop];
		}
	}

	return iLen_;
}

int CPlantInfoOnLine::GetLastRev(float *pfRev_, int iLen_)
{
	if ((iLen_ <1) || (iLen_ >5))
		iLen_ = 1;
	_pLockVibData->BeginRead();
	for (int nloop=0;nloop < iLen_;++nloop)
		pfRev_[nloop]= _pfLastRev[nloop];
	_pLockVibData->EndRead();
	return iLen_;
}

CBHDateTime CPlantInfoOnLine::GetLastRevTime()
{
	CBHDateTime tLastRevTime;

	_pLockVibData->BeginRead();
	tLastRevTime = _tLastRevTime;
	_pLockVibData->EndRead();

	return tLastRevTime;
}

float CPlantInfoOnLine::GetSpecTypeRealRev(const int &iRevType_,const bool &bSafe_)
{
	float fRev=0;
	int iRevType = iRevType_;
	if ((iRevType_<1)||(iRevType_>5))
		iRevType = 1;
	--iRevType;
	if (bSafe_)
	{
		_pLockVibData->BeginRead();
		fRev=_pfRealRev[iRevType];
		_pLockVibData->EndRead();
	}
	else
		fRev=_pfRealRev[iRevType];
	return fRev;
}

void CPlantInfoOnLine::GetRealSmpInfo(int &iSmpNum_,int &iSmpFreq_,bool bSafe_)
{
	CString strTmp;
	if (bSafe_)
	{
		_pLockVibData->BeginRead();
		iSmpNum_=_iRealSmpNum;
		iSmpFreq_=_iRealSmpFreq;
		_pLockVibData->EndRead();
	}
	else
	{
		iSmpNum_=_iRealSmpNum;
		iSmpFreq_=_iRealSmpFreq;
	}
}

void CPlantInfoOnLine::GetDynRealSmpInfo(int &iSmpNum_,int &iSmpFreq_,bool bSafe_)
{
	if (bSafe_)
	{
		_pLockDynData->BeginRead();
		iSmpNum_ = _iDynRealSmpNum;
		iSmpFreq_= _iDynRealSmpFreq;
		_pLockDynData->EndRead();
	}
	else
	{
		iSmpNum_ = _iDynRealSmpNum;
		iSmpFreq_= _iDynRealSmpFreq;
	}
}

void CPlantInfoOnLine::GetRealAllVibWaveNum(int &iRealAllNum_,bool bSafe_/* =true */)
{
	if (bSafe_)
	{
		_pLockVibData->BeginRead();
		iRealAllNum_=_iVibRealAllNum;
		_pLockVibData->EndRead();
	}
	else
	{
		iRealAllNum_=_iVibRealAllNum;
	}
}

void CPlantInfoOnLine::GetRealAllDynWaveNum(int &iRealAllNum_,bool bSafe_/* =true */)
{
	if (bSafe_)
	{
		_pLockDynData->BeginRead();
		iRealAllNum_=_iDynRealAllNum;
		_pLockDynData->EndRead();
	}
	else
	{
		iRealAllNum_=_iDynRealAllNum;
	}
}

void CPlantInfoOnLine::SetRealSmpInfo(const int &iSmpNum_,const int &iSmpFreq_,bool bSafe_/* =true */)
{
	CString strTmp;
	if (bSafe_)
	{
		_pLockVibData->BeginWrite();
		_iRealSmpNum  = iSmpNum_;
		_iRealSmpFreq = iSmpFreq_;
		_pLockVibData->EndWrite();
	}
	else
	{
		_iRealSmpNum  = iSmpNum_;
		_iRealSmpFreq = iSmpFreq_;
	}
}

void CPlantInfoOnLine::SetDynRealSmpInfo(const int &iSmpNum_,const int &iSmpFreq_,bool bSafe_/* =true */)
{
	if (bSafe_)
	{
		_pLockDynData->BeginWrite();
		try
		{
			_iDynRealSmpNum = iSmpNum_;
			_iDynRealSmpFreq = iSmpFreq_;
		}
		catch (...)
		{
			//log here
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

		}

		_pLockDynData->EndWrite();
	}
	else
	{
		_iDynRealSmpNum = iSmpNum_;
		_iDynRealSmpFreq = iSmpFreq_;
	}
}

void CPlantInfoOnLine::SetRealAllVibWaveNum(const int &iRealAllNum_,bool bSafe_/* =true */)
{
	if (bSafe_)
	{
		_pLockVibData->BeginWrite();
		_iVibRealAllNum=iRealAllNum_;
		_pLockVibData->EndWrite();
	}
	else
	{
		_iVibRealAllNum=iRealAllNum_;
	}
}

void CPlantInfoOnLine::SetRealAllDynWaveNum(const int &iRealAllNum_,bool bSafe_/* =true */)
{
	if (bSafe_)
	{
		_pLockDynData->BeginWrite();
		try
		{
			_iDynRealAllNum=iRealAllNum_;
		}
		catch (...)
		{
			//log here
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

		}

		_pLockDynData->EndWrite();
	}
	else
	{
		_iDynRealAllNum=iRealAllNum_;
	}
}

void CPlantInfoOnLine::SetVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDVibTrendSecond=iCurrIDVibTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetVibTrendHourCurrID(const int &iCurrIDVibTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDVibTrendHour=iCurrIDVibTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetVibTrendDayCurrID(const int &iCurrIDVibTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDVibTrendDay=iCurrIDVibTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDVibTrendMonth=iCurrIDVibTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDProcTrendSecond=iCurrIDProcTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetProcTrendHourCurrID(const int &iCurrIDProcTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDProcTrendHour=iCurrIDProcTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetProcTrendDayCurrID(const int &iCurrIDProcTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDProcTrendDay=iCurrIDProcTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDProcTrendMonth=iCurrIDProcTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDDynTrendSecond=iCurrIDDynTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetDynTrendHourCurrID(const int &iCurrIDDynTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDDynTrendHour=iCurrIDDynTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetDynTrendDayCurrID(const int &iCurrIDDynTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDDynTrendDay=iCurrIDDynTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDDynTrendMonth=iCurrIDDynTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendSecond_=_iCurrIDVibTrendSecond;
		if (_iCurrIDVibTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDVibTrendSecond = 0;
			}
			else
			{
				++_iCurrIDVibTrendSecond;
			}
		}
		else
		{
			++_iCurrIDVibTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendHour_=_iCurrIDVibTrendHour;
		if (_iCurrIDVibTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDVibTrendHour = 0;
			}
			else
			{
				++_iCurrIDVibTrendHour;
			}
		}
		else
		{
			++_iCurrIDVibTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendDay_=_iCurrIDVibTrendDay;
		if (_iCurrIDVibTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDVibTrendDay = 0;
			}
			else
			{
				++_iCurrIDVibTrendDay;
			}
		}
		else
		{
			++_iCurrIDVibTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendMonth_=_iCurrIDVibTrendMonth;
		++_iCurrIDVibTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendSecond_=_iCurrIDProcTrendSecond;
		if (_iCurrIDProcTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDProcTrendSecond = 0;
			}
			else
			{
				++_iCurrIDProcTrendSecond;
			}
		}
		else
		{
			++_iCurrIDProcTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendHour_=_iCurrIDProcTrendHour;
		if (_iCurrIDProcTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDProcTrendHour = 0;
			}	
			else
			{
				++_iCurrIDProcTrendHour;
			}
		}
		else
		{
			++_iCurrIDProcTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendDay_=_iCurrIDProcTrendDay;
		if (_iCurrIDProcTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDProcTrendDay = 0;
			}
			else
			{
				++_iCurrIDProcTrendDay;
			}
		}
		else
		{
			++_iCurrIDProcTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendMonth_=_iCurrIDProcTrendMonth;
		++_iCurrIDProcTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendSecond_=_iCurrIDDynTrendSecond;
		if (_iCurrIDDynTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDDynTrendSecond = 0;
			}
			else
			{
				++_iCurrIDDynTrendSecond;
			}
		}
		else
		{
			++_iCurrIDDynTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendHour_=_iCurrIDDynTrendHour;
		if (_iCurrIDDynTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDDynTrendHour = 0;
			}
			else
			{
				++_iCurrIDDynTrendHour;
			}
		}
		else
		{
			++_iCurrIDDynTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendDay_=_iCurrIDDynTrendDay;
		if (_iCurrIDDynTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDDynTrendDay = 0;
			}
			else
			{
				++_iCurrIDDynTrendDay;
			}
		}
		else
		{
			++_iCurrIDDynTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendMonth_=_iCurrIDDynTrendMonth;
		++_iCurrIDDynTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}


void CPlantInfoOnLine::SetNegVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDVibTrendSecond=iCurrIDVibTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegVibTrendHourCurrID(const int &iCurrIDVibTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDVibTrendHour=iCurrIDVibTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegVibTrendDayCurrID(const int &iCurrIDVibTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDVibTrendDay=iCurrIDVibTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDVibTrendMonth=iCurrIDVibTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDProcTrendSecond=iCurrIDProcTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegProcTrendHourCurrID(const int &iCurrIDProcTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDProcTrendHour=iCurrIDProcTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegProcTrendDayCurrID(const int &iCurrIDProcTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDProcTrendDay=iCurrIDProcTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDProcTrendMonth=iCurrIDProcTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDDynTrendSecond=iCurrIDDynTrendSecond_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegDynTrendHourCurrID(const int &iCurrIDDynTrendHour_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDDynTrendHour=iCurrIDDynTrendHour_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegDynTrendDayCurrID(const int &iCurrIDDynTrendDay_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDDynTrendDay=iCurrIDDynTrendDay_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetNegDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDDynTrendMonth=iCurrIDDynTrendMonth_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendSecond_=_iNegCurrIDVibTrendSecond;
		--_iNegCurrIDVibTrendSecond;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegVibTrendHourCurrID(int &iCurrIDVibTrendHour_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendHour_=_iNegCurrIDVibTrendHour;
		--_iNegCurrIDVibTrendHour;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegVibTrendDayCurrID(int &iCurrIDVibTrendDay_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendDay_=_iNegCurrIDVibTrendDay;
		--_iNegCurrIDVibTrendDay;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendMonth_=_iNegCurrIDVibTrendMonth;
		--_iNegCurrIDVibTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendSecond_=_iNegCurrIDProcTrendSecond;
		--_iNegCurrIDProcTrendSecond;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetNegProcTrendHourCurrID(int &iCurrIDProcTrendHour_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendHour_=_iNegCurrIDProcTrendHour;
		--_iNegCurrIDProcTrendHour;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetNegProcTrendDayCurrID(int &iCurrIDProcTrendDay_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendDay_=_iNegCurrIDProcTrendDay;
		
		--_iNegCurrIDProcTrendDay;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetNegProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendMonth_=_iNegCurrIDProcTrendMonth;
		--_iNegCurrIDProcTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CPlantInfoOnLine::GetNegDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendSecond_=_iNegCurrIDDynTrendSecond;
		--_iNegCurrIDDynTrendSecond;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetNegDynTrendHourCurrID(int &iCurrIDDynTrendHour_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendHour_=_iNegCurrIDDynTrendHour;
		--_iNegCurrIDDynTrendHour;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetNegDynTrendDayCurrID(int &iCurrIDDynTrendDay_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendDay_=_iNegCurrIDDynTrendDay;
		
		--_iNegCurrIDDynTrendDay;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetNegDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendMonth_=_iNegCurrIDDynTrendMonth;
		--_iNegCurrIDDynTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetVibTrendFastCurrID(int& iCurrIDVibTrendFast_)
{
	_pLockVibData->BeginWrite();
	try
	{
		
		iCurrIDVibTrendFast_=_iCurrIDVibTrendFast;

		if (iCurrIDVibTrendFast_>=G_D_TREND_FASTRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDVibTrendFast = 0;
			}
			else
			{
				++_iCurrIDVibTrendFast;
			}
		}
		else
		{
			++_iCurrIDVibTrendFast;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}
void CPlantInfoOnLine::GetDynTrendFastCurrID(int& iCurrIDDynTrendFast_)
{
	_pLockDynData->BeginWrite();
	try
	{
		
		iCurrIDDynTrendFast_=_iCurrIDDynTrendFast;

		if (iCurrIDDynTrendFast_>=G_D_TREND_FASTRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDDynTrendFast = 0;
			}
			else
			{
				++_iCurrIDDynTrendFast;
			}
		}
		else
		{
			++_iCurrIDDynTrendFast;
		}

	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}
void CPlantInfoOnLine::GetProcTrendFastCurrID(int& iCurrIDProcTrendFast_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendFast_=_iCurrIDProcTrendFast;

		if (iCurrIDProcTrendFast_>=G_D_TREND_FASTRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_iCurrIDProcTrendFast = 0;
			}
			else
			{
				++_iCurrIDProcTrendFast;
			}
		}
		else
		{
			++_iCurrIDProcTrendFast;
		}

	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}
void CPlantInfoOnLine::SetVibTrendFastCurrID(const int &iCurrIDVibTrendFast_)
{
	_pLockVibData->BeginWrite();
	_iCurrIDVibTrendFast=iCurrIDVibTrendFast_;
	_pLockVibData->EndWrite();
}
void CPlantInfoOnLine::SetDynTrendFastCurrID(const int &iCurrIDDynTrendFast_)
{
	_pLockDynData->BeginWrite();
	_iCurrIDDynTrendFast=iCurrIDDynTrendFast_;
	_pLockDynData->EndWrite();
}
void CPlantInfoOnLine::SetProcTrendFastCurrID(const int &iCurrIDProcTrendFast_)
{
	_pLockProcData->BeginWrite();
	_iCurrIDProcTrendFast=iCurrIDProcTrendFast_;
	_pLockProcData->EndWrite();
}


void CPlantInfoOnLine::GetNegVibTrendFastCurrID(int &iCurrIDVibTrendFast_)
{
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendFast_=_iNegCurrIDVibTrendFast;
		--_iNegCurrIDVibTrendFast;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::GetNegDynTrendFastCurrID(int &iCurrIDDynTrendFast_)
{
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendFast_=_iNegCurrIDDynTrendFast;
		--_iNegCurrIDDynTrendFast;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CPlantInfoOnLine::GetNegProcTrendFastCurrID(int &iCurrIDProcTrendFast_)
{
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendFast_=_iNegCurrIDProcTrendFast;
		--_iNegCurrIDProcTrendFast;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}


void CPlantInfoOnLine::SetNegVibTrendFastCurrID(const int &iCurrIDVibTrendFast_)
{
	_pLockVibData->BeginWrite();
	_iNegCurrIDVibTrendFast=iCurrIDVibTrendFast_;
	_pLockVibData->EndWrite();
}


void CPlantInfoOnLine::SetNegDynTrendFastCurrID(const int &iCurrIDDynTrendFast_)
{
	_pLockDynData->BeginWrite();
	_iNegCurrIDDynTrendFast=iCurrIDDynTrendFast_;
	_pLockDynData->EndWrite();
}


void CPlantInfoOnLine::SetNegProcTrendFastCurrID(const int &iCurrIDProcTrendFast_)
{
	_pLockProcData->BeginWrite();
	_iNegCurrIDProcTrendFast=iCurrIDProcTrendFast_;
	_pLockProcData->EndWrite();
}

int CPlantInfoOnLine::GetSpecTypeChannInfo(S_ChannInfo * pSchannInfo_,int iType_)
{
	int nCount=0;
	V_PCHANNINFO::iterator itVib;
	V_PCHANNINFO::iterator itProc;
	S_ChannInfo channInfo;
	switch(iType_)
	{
	case GE_ALLPROC:
		for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
		{
			pSchannInfo_[nCount]=(*itProc)->GetChannInfo();
			nCount++;
		}
		break;
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
		for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
		{
			channInfo=(*itVib)->GetChannInfo();
			if (channInfo._iChannType==iType_)
			{
				pSchannInfo_[nCount]=channInfo;
				++nCount;
			}
		}
		break;
	default:
		for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
		{
			channInfo=(*itProc)->GetChannInfo();
			if (channInfo._iChannType==iType_)
			{
				pSchannInfo_[nCount]=channInfo;
				nCount++;
			}
		}
	}
	return nCount;
}

int CPlantInfoOnLine::GetSpecTypeChannInfo(S_ChannInfo &sChannInfo_,int iIndex_/* =0 */,int iType_/* =GE_ALLPROC */)
{
	int iRtn=-1;
	if (iIndex_<0)
		return -1;

	switch(iType_)
	{
	case GE_VIBCHANN://径向振动
		if(iIndex_< _sPlantInfo._iVibChannNum)
		{

			sChannInfo_=_vVibChann[iIndex_]->GetChannInfo();
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动
		if(iIndex_<_iAxialVibChannNum)
		{
			sChannInfo_=_vVibChann[iIndex_+_iRadialVibChannNum]->GetChannInfo();
			iRtn=1;
		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量
		if(iIndex_<_sPlantInfo._iDynChannNum)
		{
			sChannInfo_=_vDynChann[iIndex_]->GetChannInfo();
			iRtn=1;
		}
		break;
	case GE_ALLPROC:
	default:
		if (iIndex_< _sPlantInfo._iProcChannNum)
		{
			sChannInfo_=_vProcChann[iIndex_]->GetChannInfo();
			iRtn=1;
		}
	}
	return iRtn;
}

int CPlantInfoOnLine::GetSpecVibChannStartEndIndex(int index_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return GetICESpecVibChannStartEndIndex(index_,iStartIndex_,iEndIndex_,abRealTime);
	}

	int index=0;
	S_VibCharValue vib;
	if(index_ > -1)
	{
		_pLockVibData->BeginRead();
		try
		{
			if (abRealTime)
			{
				vib = ((CVibChannParam*)_vVibChann[index_])->GetRealCharValueEx();
			}else
			{
				vib = ((CVibChannParam*)_vVibChann[index_])->GetCharValue();
			}

		}
		catch (...)
		{
			//log here
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

		}

		_pLockVibData->EndRead();
		iStartIndex_ = (int)vib.GetSpecTypeData(GE_PHASECHARTHREE);
		iEndIndex_ = (int)vib.GetSpecTypeData(GE_PHASECHARHALF);
		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			if ((iEndIndex_ - iStartIndex_ )> ((iSmpFreq*60/iRev)+10) || (iEndIndex_ - iStartIndex_) < ((iSmpFreq*60/iRev)-10))
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
				iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;
			}
		}
	}
	return  index_;
}

int CPlantInfoOnLine::GetSpecVibChannStartEndIndex(string sChann_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetSpecVibChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_,abRealTime);

}

int CPlantInfoOnLine::GetICESpecVibChannStartEndIndex(int index_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int index=0;
	S_VibCharValue vib;
	if(index_ > -1)
	{
		_pLockVibData->BeginRead();
		try
		{
			if (abRealTime)
			{
				vib = ((CVibChannParam*)_vVibChann[index_])->GetRealCharValueEx();
			}else
			{
				vib = ((CVibChannParam*)_vVibChann[index_])->GetCharValue();
			}

		}
		catch (...)
		{
			//log here
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

		}

		_pLockVibData->EndRead();

		CString lstrChann  = this->GetSpecVibChannInfo(index_)._cChannNo;

		int lnStartIndexSpec = GE_THREECHAR;
		int lnEndIndexSpec = GE_HALFCHAR;

		if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;

		}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
		{
			lnStartIndexSpec = GE_PHASECHARTWO;
			lnEndIndexSpec = GE_PHASECHARTHREE;

		}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;
		}

		iStartIndex_ = (int)vib.GetSpecTypeData(lnStartIndexSpec);
		iEndIndex_ = (int)vib.GetSpecTypeData(lnEndIndexSpec);
		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;

		if ((iEndIndex_-iStartIndex_)<= 256)
		{
			iStartIndex_ = 0;
			iEndIndex_ = iSmpNum;
		}

	}
	return  index_;
}

int CPlantInfoOnLine::GetICESpecVibChannStartEndIndex(string sChann_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetICESpecVibChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_,abRealTime);

}

int CPlantInfoOnLine::GetSpecDynChannStartEndIndex(int index_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return GetICESpecDynChannStartEndIndex(index_,iStartIndex_,iEndIndex_,abRealTime);
	}
	int index=0;
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		_pLockDynData->BeginRead();
		if (abRealTime)
		{
			dyn = ((CDynChannParam*)_vDynChann[index_])->GetCharValue();

		}else
		{
			dyn = ((CDynChannParam*)_vDynChann[index_])->GetRealCharValueEx();

		}
		_pLockDynData->EndRead();
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			if (iEndIndex_ - iStartIndex_ > (iSmpFreq*60/iRev)+10 || iEndIndex_ - iStartIndex_ < (iSmpFreq*60/iRev)-10)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
				iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;
			}
		}
	}
	return index_;
}

int CPlantInfoOnLine::GetSpecDynChannStartEndIndex(string sChann_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetSpecDynChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_,abRealTime);
}

int CPlantInfoOnLine::GetICESpecDynChannStartEndIndex(int index_,
												  int &iStartIndex_,
												  int &iEndIndex_,
												  bool abRealTime /*= false*/)
{
	int index=0;
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		_pLockDynData->BeginRead();
		if (abRealTime)
		{
			dyn = ((CDynChannParam*)_vDynChann[index_])->GetCharValue();

		}else
		{
			dyn = ((CDynChannParam*)_vDynChann[index_])->GetRealCharValueEx();

		}
		_pLockDynData->EndRead();
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			if(iStartIndex_ > iEndIndex_)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
			}
			iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;

		}
	}
	return index_;
}

int CPlantInfoOnLine::GetICESpecDynChannStartEndIndex(string sChann_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetICESpecDynChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_,abRealTime);
}


int CPlantInfoOnLine::GetSpecVibChannStartEndIndex(int index_,
												   const S_VibCharValue& VibCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return GetICESpecVibChannStartEndIndex(index_,VibCharValue_,iStartIndex_,iEndIndex_,abRealTime);
	}

	int index=0;
	S_VibCharValue vib;
	if(index_ > -1)
	{
		vib = VibCharValue_;
		iStartIndex_ = (int)vib.GetSpecTypeData(GE_PHASECHARTHREE);
		iEndIndex_ = (int)vib.GetSpecTypeData(GE_PHASECHARHALF);
		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			
			if ((iEndIndex_ - iStartIndex_ )> ((iSmpFreq*60/iRev)+10) || (iEndIndex_ - iStartIndex_) < ((iSmpFreq*60/iRev)-10))
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
				iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;
			}
			
		}
	}
	return  index_;
}

int CPlantInfoOnLine::GetSpecVibChannStartEndIndex(string sChann_,
												   const S_VibCharValue& VibCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetSpecVibChannStartEndIndex(iRtn, VibCharValue_,iStartIndex_,iEndIndex_,abRealTime);

}

int CPlantInfoOnLine::GetICESpecVibChannStartEndIndex(int index_,
												   const S_VibCharValue& VibCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int index=0;
	S_VibCharValue vib;
	if(index_ > -1)
	{
		vib = VibCharValue_;
		
		CString lstrChann  = this->GetSpecVibChannInfo(index_)._cChannNo;
		int lnStartIndexSpec = GE_THREECHAR;
		int lnEndIndexSpec = GE_HALFCHAR;

		if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;

		}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
		{
			lnStartIndexSpec = GE_PHASECHARTWO;
			lnEndIndexSpec = GE_PHASECHARTHREE;

		}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;
		}

		
		iStartIndex_ = (int)vib.GetSpecTypeData(lnStartIndexSpec);
		iEndIndex_ = (int)vib.GetSpecTypeData(lnEndIndexSpec);
		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;

		
		if ((iEndIndex_-iStartIndex_)<= 256)
		{
			iStartIndex_ = 0;
			iEndIndex_ = iSmpNum;
		}

/*
		if (iRev >0)
		{
			if ((iEndIndex_ - iStartIndex_ )> ((iSmpFreq*60/iRev)+10) || (iEndIndex_ - iStartIndex_) < ((iSmpFreq*60/iRev)-10))
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
				iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;
			}
		}*/
	}
	return  index_;
}

int CPlantInfoOnLine::GetICESpecVibChannStartEndIndex(string sChann_,
												   const S_VibCharValue& VibCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime/* = false*/)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetICESpecVibChannStartEndIndex(iRtn, VibCharValue_,iStartIndex_,iEndIndex_,abRealTime);

}

int CPlantInfoOnLine::GetSpecDynChannStartEndIndex(int index_,
												   const S_DynCharValue& DynCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return this->GetICESpecDynChannStartEndIndex(index_,DynCharValue_,iStartIndex_,iEndIndex_,abRealTime);
	}
	int index=0;
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		dyn = DynCharValue_;
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			if (iEndIndex_ - iStartIndex_ > (iSmpFreq*60/iRev)+10 || iEndIndex_ - iStartIndex_ < (iSmpFreq*60/iRev)-10)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
				iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;
			}
		}
	}
	return index_;
}

int CPlantInfoOnLine::GetSpecDynChannStartEndIndex(string sChann_,
												   const S_DynCharValue& DynCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetSpecDynChannStartEndIndex(iRtn,DynCharValue_, iStartIndex_,iEndIndex_,abRealTime);
}


int CPlantInfoOnLine::GetICESpecDynChannStartEndIndex(int index_,
												   const S_DynCharValue& DynCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	int index=0;
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		dyn = DynCharValue_;
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
		if (iRev >0)
		{
			if(iStartIndex_ > iEndIndex_)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
			}
			iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;

		}
	}
	return index_;
}

int CPlantInfoOnLine::GetICESpecDynChannStartEndIndex(string sChann_,
												   const S_DynCharValue& DynCharValue_,
												   int &iStartIndex_,
												   int &iEndIndex_,
												   bool abRealTime /*= false*/)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetICESpecDynChannStartEndIndex(iRtn,DynCharValue_, iStartIndex_,iEndIndex_,abRealTime);
}

double CPlantInfoOnLine::_CalcEneryValue(		
	double * arefPSpectrumValueArray,
	int anSpectrumArrayLength
	)
{
	if (!arefPSpectrumValueArray)
	{
		ASSERT(FALSE);
		return 0;
	}

	double ldbValue = 0;

	//能量值
	for (int nLoop=0;nLoop<anSpectrumArrayLength;nLoop++)
	{
		ldbValue+=pow((double)(arefPSpectrumValueArray[nLoop]),2);
	}

	return ldbValue;
}

double  CPlantInfoOnLine::_FindMaxValue(
										double * arefPSpectrumValueArray,
										int anSpectrumArrayLength,
										int & anIndex
										)
{
	if (!arefPSpectrumValueArray)
	{
		ASSERT(FALSE);
		return 0;
	}

	double ldblRet = 0.0;

	for (int i =0;i<anSpectrumArrayLength;i++)
	{
		if (ldblRet<arefPSpectrumValueArray[i])
		{
			ldblRet = arefPSpectrumValueArray[i];

			anIndex = i;
		}
	}

	return ldblRet;
}

//[TBD]

double CPlantInfoOnLine::_FindSpectrumData(
	double afFreq,
	double * arefPSpectrumValueArray,
	int anSpectrumArrayLength,
	double afDf									
	)
{
	if (afDf <=0 || arefPSpectrumValueArray == NULL || anSpectrumArrayLength <=0 || afFreq<=0)
	{
		ASSERT(FALSE);
		return .0;
	}

	int idx = afFreq/afDf;

	///在idx左右两个，总共5个值，找出一个最大值
	if (idx >= anSpectrumArrayLength || idx < 0)
	{
		return .0;
	}

	int lnLeftIdx = 0;

	int lnRightIdx = 0;

	//得到左右两边的边界

	lnLeftIdx = idx - 2;

	if (lnLeftIdx < 0)
	{

		lnLeftIdx = 0;
	}

	lnRightIdx = idx +2;

	if (lnRightIdx > anSpectrumArrayLength)
	{

		lnRightIdx = idx;
	} 

	double lfMax = 0;

	for (int j =lnLeftIdx;j <= lnRightIdx;j++)
	{
		if (lfMax < arefPSpectrumValueArray[j])
		{
			lfMax = arefPSpectrumValueArray[j];
		}
	}

	return lfMax;
}

int CPlantInfoOnLine::_FindSpectrumData(
										std::map<int,float> & arefFeatureData,
										float * apCalcResultArray,
										int anResultArrayLength,
										double * arefPSpectrumValueArray,
										int anSpectrumArrayLength,
										double afDf
										)
{
	float * fBuf = apCalcResultArray;

	double df = afDf;

	double * pSpectrum = arefPSpectrumValueArray;

	int lnSpectrumSize = anSpectrumArrayLength;

	for ( std::map<int,float>::iterator it = arefFeatureData.begin(); it != arefFeatureData.end(); ++it)
	{
		int idx = it->second/df;

		///在idx左右两个，总共5个值，找出一个最大值
		if (idx >= lnSpectrumSize || idx < 0)
		{
			continue;
		}

		int lnLeftIdx = 0;

		int lnRightIdx = 0;

		//得到左右两边的边界

		lnLeftIdx = idx - 2;

		if (lnLeftIdx < 0)
		{

			lnLeftIdx = 0;
		}

		lnRightIdx = idx +2;

		if (lnRightIdx > lnSpectrumSize)
		{

			lnRightIdx = idx;
		} 

		float lfMax = 0;

		for (int j =lnLeftIdx;j <= lnRightIdx;j++)
		{
			if (lfMax < pSpectrum[j])
			{
				lfMax = pSpectrum[j];
			}
		}

		float fValue = lfMax; 

		if (it->first > anResultArrayLength)
		{
			ASSERT(FALSE);

			continue;
		}

		fBuf[it->first] = fValue;
	}

	return 1;
}

int CPlantInfoOnLine::_CalcCustomSpectrumEnvelopeWindPump(
	const S_VibCharValue & aoVibCharVal,
	CVibChannParam * apVib, 
	float * apCalcResultArray,
	int anResultArrayLength,
	double * apWave,
	float afWorkFreq,
	double adblDf
	)
{

	float * fBuf = apCalcResultArray;
	S_VibCharValue vibCharValue = aoVibCharVal;
	CVibChannParam * pVib = apVib;
	double * pWave = apWave;
	double df = adblDf;
	int fSampleFreq  = vibCharValue._iSmpFreq ;
	int iSmpNum = vibCharValue._iSmpNum;


	//4.压入特征值
	map<int ,float> mapIdxFreq;

	CBearingParamInfo bearingInfo = pVib->GetBearingParamInfo();
	if (bearingInfo._iBearingIDX > 0 || vibCharValue._iRev > 1)
	{
		S_BearingDefectFreq defectFreq;
		CalcBearingDefectFreq(bearingInfo, vibCharValue._iRev,defectFreq);
		mapIdxFreq.insert(make_pair(GE_ENVSPECCF1,defectFreq.fInnerFreq));			///内圈特征频率				m/s2
		mapIdxFreq.insert(make_pair(GE_ENVSPECCF2,defectFreq.fOuterFreq));			///外圈特征频率				m/s2
		mapIdxFreq.insert(make_pair(GE_ENVSPECCF3,defectFreq.fCageFreq));			///保持架特征频率			m/s2
		mapIdxFreq.insert(make_pair(GE_ENVSPECCF4,defectFreq.fRollingFreq));		///滚动体特征频率			m/s2	
	}

	CGearBoxParamInfo gearboxInfo = pVib->GetGearBoxParamInfo();
	CChannLocations chanLocation =   pVib->GetChannLocationsInfo();

	if (gearboxInfo._iGearBoxLevel > 0 || gearboxInfo._iGearNumGalaxy > 0 || chanLocation._iShaftNumber > -1)
	{
		S_GearBoxCharFreq   gearboxCharFreq;

		ZeroMemory(&gearboxCharFreq,sizeof(S_GearBoxCharFreq));

		CalcGearboxCharFreq(gearboxInfo, vibCharValue._iRev,gearboxCharFreq);
		mapIdxFreq.insert(make_pair(GE_USER_AENVFREQ2,gearboxCharFreq.fEnvFreq2));	///输出轴转频				m/s2
		mapIdxFreq.insert(make_pair(GE_GEARBOXSHAFTCF1,gearboxCharFreq.fShaftCF1));	///齿轮箱I轴转频			m/s2
		mapIdxFreq.insert(make_pair(GE_GEARBOXSHAFTCF2,gearboxCharFreq.fShaftCF2));	///齿轮箱I轴转频			m/s2
		mapIdxFreq.insert(make_pair(GE_GEARBOXSHAFTCF3,gearboxCharFreq.fShaftCF3));	///齿轮箱I轴转频			m/s2
		mapIdxFreq.insert(make_pair(GE_GEARBOXSHAFTCF4,gearboxCharFreq.fShaftCF4));	///齿轮箱I轴转频			m/s2

	}

	if (mapIdxFreq.size() < 1)
	{
		return -1;
	}

	std::vector<double> loVectorSpectrum;

	std::vector<double> loWaveVectorEnvelop;

	int lnSpectrumSize = iSmpNum/2+1;

	if (lnSpectrumSize < mapIdxFreq.size())
	{
		return -1;
	}

	loWaveVectorEnvelop.resize(iSmpNum);

	loVectorSpectrum.resize(lnSpectrumSize,0.0);				

	double *pSpectrum = &(loVectorSpectrum.front());//包络谱存放数据缓存

	double * lpSpectrumWavePoint = &(loWaveVectorEnvelop.front());//包络波形存放数据的缓存

	CSigMath math;

	double ldblgSE = .0;

	int iCutoffFreq = 0;

	if (fSampleFreq > 10240)
	{
		iCutoffFreq = 5000;
	}else
	{
		iCutoffFreq = fSampleFreq/3;
	}

	BOOL lbRet = 0;

	try
	{
		lbRet = math.gSEAnalysis(pWave, iSmpNum,fSampleFreq,iCutoffFreq, ldblgSE,lpSpectrumWavePoint);

		if (lbRet < 0)
		{
			return -1;
		}

	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		return -1;
	}

	try
	{
		lbRet = math.SpectralMeasureMent(lpSpectrumWavePoint,iSmpNum,pSpectrum,E_SpectrumType_Peak_Peak) ;

		if (lbRet < 0)
		{

			return -1;
		}
	}
	catch (...)
	{

#ifdef _DEBUG
		//writefile(pWave,iSmpNum,101);
#endif // _DEBUG
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		return -1;		
	}

	try
	{
		lbRet = this->_FindSpectrumData(mapIdxFreq,fBuf,anResultArrayLength,pSpectrum,lnSpectrumSize,df);

		if (lbRet == 0)
		{
			return -1;
		}
	}
	catch (...)
	{

#ifdef _DEBUG
		//writefile(pSpectrum,lnSpectrumSize,102);
#endif // _DEBUG

		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		return -1;	
	}

	return 1;
}


int CPlantInfoOnLine::_CalcCustomSpectrumAmpliferWindPump(
	const S_VibCharValue & aoVibCharVal,
	CVibChannParam * apVib, 
	float * apCalcResultArray,
	int anResultArrayLength,
	double * apWave,
	float afWorkFreq,
	double adblDf
	)
{

	float * fBuf = apCalcResultArray;

	S_VibCharValue vibCharValue = aoVibCharVal;

	CVibChannParam * pVib = apVib;

	double * pWave = apWave;

	double df = adblDf;

	float fWorkFreq  = afWorkFreq ;

	int iSmpNum = vibCharValue._iSmpNum;

	int lnSpectrumSize = iSmpNum/2+1;


	//4.压入特征值
	map<int ,float> mapIdxFreq;

	CGearBoxParamInfo gearboxInfo = pVib->GetGearBoxParamInfo();

	CChannLocations chanLocation =   pVib->GetChannLocationsInfo();

	if (gearboxInfo._iGearBoxLevel > 0 || gearboxInfo._iGearNumGalaxy > 0 || gearboxInfo._iGearBoxLevel > 0 || chanLocation._iShaftNumber > -1)
	{
		S_GearBoxCharFreq   gearboxCharFreq;

		ZeroMemory(&gearboxCharFreq,sizeof(S_GearBoxCharFreq));

		CalcGearboxCharFreq(gearboxInfo, vibCharValue._iRev,gearboxCharFreq);

		mapIdxFreq.insert(make_pair(GE_USER_AENVFREQ1,gearboxCharFreq.fEnvFreq1));		///啮合频率				m/s2	

	}

	if(mapIdxFreq.size() < 1)
	{
		return -1;
	}

	std::vector<double> loVectorSpectrum;

	if (lnSpectrumSize < mapIdxFreq.size())
	{
		return -1;
	}

	loVectorSpectrum.resize(lnSpectrumSize,-1.0);				

	double *pSpectrum = &loVectorSpectrum.front();

	CSigMath math;

	try
	{
		if( math.SpectralMeasureMent(pWave, iSmpNum,pSpectrum, E_SpectrumType_PEAK) < 0)
		{
			return -1;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		return -1;
	}

	try
	{
		if (this->_FindSpectrumData(mapIdxFreq,fBuf,anResultArrayLength,pSpectrum,lnSpectrumSize,df) < 0)
		{
			return -2;
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		return -2;
	}
	return 1;
}

int CPlantInfoOnLine::_CalcSpeedSpectrumPowerWindPump(
	const S_VibCharValue & aoVibCharVal,
	CVibChannParam * apVib, 
	float * apCalcResultArray,
	int anResultArrayLength,
	double * apWave,
	float afWorkFreq,
	double adblDf
	)
{

	float * fBuf = apCalcResultArray;

	S_VibCharValue vibCharValue = aoVibCharVal;

	CVibChannParam * pVib = apVib;

	double * pWave = apWave;

	double df = adblDf;

	float fWorkFreq  = afWorkFreq ;

	int iSmpNum = vibCharValue._iSmpNum; //实际的采样点数

	int lnSpectrumSize = iSmpNum/2;	 //频谱点数

	//4.压入特征值
	map<int ,float> mapIdxFreq;

	mapIdxFreq.insert(make_pair(GE_VF_HALF,fWorkFreq*0.5));
	mapIdxFreq.insert(make_pair(GE_VF_FIVE,fWorkFreq*5));
	mapIdxFreq.insert(make_pair(GE_VF_ONEX,fWorkFreq*1));
	mapIdxFreq.insert(make_pair(GE_VF_TWO,fWorkFreq*2));
	mapIdxFreq.insert(make_pair(GE_VF_THREE,fWorkFreq*3));
	mapIdxFreq.insert(make_pair(GE_VF_FOUR,fWorkFreq*4));
	mapIdxFreq.insert(make_pair(GE_VF_BLADE,fWorkFreq*pVib->GetChannLocationsInfo()._iShaftNumber));

	std::vector<double> loVectorSpectrum; //频谱计算后的缓存

	if (lnSpectrumSize < mapIdxFreq.size())
	{
		return -1;
	}

	loVectorSpectrum.resize(lnSpectrumSize,.0);	

	double *pSpectrum = &(loVectorSpectrum.front());

	CSigMath math;

	try
	{
		if( math.SpectralMeasureMent(pWave, iSmpNum,pSpectrum, E_SpectrumType_POWER) < 0)
		{
			return -1;
		}


		if( math.SpectrumIntegrate(pSpectrum, df, lnSpectrumSize,0.001,E_SpectrumType_POWER) < 0)
		{
			return -2;
		}


		if (this->_FindSpectrumData(mapIdxFreq,fBuf,anResultArrayLength,pSpectrum,lnSpectrumSize,df) < 0)
		{
			return -3;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	return 1; 
}

int  CPlantInfoOnLine::CalcCustomParamDataWindPump(int iStartIndex_,int iLen_,CHANNTYPE channType)
{

	/// 风电机泵自定义特征值类型
	/*
	enum E_KEYPUMPWIND_USERCHARTYPE
	{
	GE_USER_AFREQ1=0, ///速度功率谱0.5X倍频  mm/s
	GE_USER_AFREQ2, ///速度功率谱5X倍频      mm/s
	GE_VF_ONEX, ///速度功率谱1X倍频          mm/s
	GE_VF_TWO, ///速度功率谱2X倍频           mm/s
	GE_VF_THREE, ///速度功率谱3X倍频         mm/s
	GE_VF_FOUR, ///速度功率谱4X倍频          mm/s
	GE_VF_BLADE, ///速度功率谱叶片通过频率   mm/s
	GE_USER_AENVFREQ1, ///啮合频率           m/s2
	GE_USER_AENVFREQ2, ///输出轴转频         m/s2
	GE_ENVSPECCF1, ///内圈特征频率           m/s2
	GE_ENVSPECCF2, ///外圈特征频率           m/s2
	GE_ENVSPECCF3, ///保持架特征频率         m/s2
	GE_ENVSPECCF4, ///滚动体特征频率         m/s2
	GE_GEARBOXSHAFTCF1, ///齿轮箱I轴转频     m/s2
	GE_GEARBOXSHAFTCF2, ///齿轮箱I轴转频     m/s2
	GE_GEARBOXSHAFTCF3, ///齿轮箱I轴转频     m/s2
	GE_GEARBOXSHAFTCF4 ///齿轮箱I轴转频      m/s2
	};

	1、这个17个特征值单位：目前写死，你需要传递给客户端。

	2、以下7个。通过速度功率谱计算出来。原始信号是加速度信号，注意单位换算和频谱类型设置
	GE_USER_AFREQ1=0, ///速度功率谱0.5X倍频  mm/s
	GE_USER_AFREQ2, ///速度功率谱5X倍频      mm/s
	GE_VF_ONEX, ///速度功率谱1X倍频          mm/s
	GE_VF_TWO, ///速度功率谱2X倍频           mm/s
	GE_VF_THREE, ///速度功率谱3X倍频         mm/s
	GE_VF_FOUR, ///速度功率谱4X倍频          mm/s
	GE_VF_BLADE, ///速度功率谱叶片通过频率   mm/s

	3、以下九个。计算原始信号的包络谱，需要注意的是不同采样频率情况下，对截止频率设置。如果是采样频率小于1000Hz，则截止频率就设置为采样频率的1/4
	GE_ENVSPECCF1, ///内圈特征频率           m/s2
	GE_ENVSPECCF2, ///外圈特征频率           m/s2
	GE_ENVSPECCF3, ///保持架特征频率         m/s2
	GE_ENVSPECCF4, ///滚动体特征频率         m/s2
	GE_USER_AENVFREQ2, ///输出轴转频         m/s2
	GE_GEARBOXSHAFTCF1, ///齿轮箱I轴转频     m/s2
	GE_GEARBOXSHAFTCF2, ///齿轮箱II轴转频     m/s2
	GE_GEARBOXSHAFTCF3, ///齿轮箱III轴转频     m/s2
	GE_GEARBOXSHAFTCF4 ///齿轮箱IV轴转频      m/s2

	4、以下一个。加速度信号的幅值谱
	GE_USER_AENVFREQ1, ///啮合频率           m/s2

	第一步，计算速度功率谱，然后把需要从速度功率谱中提取值的频率信息作为一个向量，循环从速度功率谱提取各自值

	第二步 计算包络谱，然后把需要从速度功率谱中提取值的频率信息作为一个向量，循环从速度功率谱提取各自值

	第三步 计算加速度幅值谱，然后把需要从速度功率谱中提取值的频率信息作为一个向量，循环从速度功率谱提取各自值	
	*/	

	int loop = 0, loopCustom =0,iRtn=0;

	int lnCustromParamListSize = 0;

	CCustomParamInfo paramInfo;

	float fBuf[GD_CUSTOMPARAM_BUFFSIZE] ={.0};

	switch(channType)
	{

	case GE_VIBCHANN://径向振动

		if((iStartIndex_ + iLen_)<= _sPlantInfo._iVibChannNum)
		{

			for (loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParam * pVib = NULL;
				S_VibCharValue vibCharValue;

				int lnVibChannIndex = iStartIndex_ + loop;

				if (lnVibChannIndex >= _vVibChann.size())
				{
					break;
				}

				pVib = (CVibChannParam*)_vVibChann[lnVibChannIndex];

				_pLockVibData->BeginRead();
				vibCharValue = pVib->GetCharValue();
				_pLockVibData->EndRead();

				if (1 >  vibCharValue._iSmpNum )
				{
					continue;
				}

				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;

				//2.自定义参数列表大小
				lnCustromParamListSize = pVib->GetCustomParamSize();
				lnCustromParamListSize = 17;

				float fWorkFreq = vibCharValue._iRev/60.0;

				double df = 0;

				if (vibCharValue._iSmpFreq* vibCharValue._iSmpNum >0)
				{
					df = (double)vibCharValue._iSmpFreq/(double)vibCharValue._iSmpNum;
				}

				if (0 == df)
				{
					return -1;
				}

				int iSmpNum = vibCharValue._iSmpNum;
				int lnSpectrumSize = iSmpNum/2+1;

				std::vector<double> loVectorWave;
				loVectorWave.resize(this->GetMiddleWareWavePointNumMax(),-1.0);
				double *pWave =  &loVectorWave.front();


				//3.得到波形
				_pLockVibData->BeginRead();
				const DATATYPE_WAVE * lpWaveData =  pVib->GetCurrRealWavePoint();	
				for (int i =0;i< iSmpNum;i++)
				{
					pWave[i] = (double)lpWaveData[i] /10;
				}
				_pLockVibData->EndRead();


				//4.计算速度功率谱				
				BOOL lbCalRet = -1;

				try
				{
					lbCalRet = this->_CalcSpeedSpectrumPowerWindPump(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);

					ASSERT(lbCalRet >0);	
				}
				catch (...)
				{
					//writefile(pWave,iSmpNum,99);		
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
					ASSERT(FALSE);
				}

				if (lbCalRet < 0)
				{
				}

				//5.计算包络谱
				try
				{
					lbCalRet = this->_CalcCustomSpectrumEnvelopeWindPump(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);

				}
				catch (...)
				{
					//writefile(pWave,iSmpNum,103);
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
					ASSERT(FALSE);
				}

				if (lbCalRet < 0)
				{

				}

				//6.计算幅值谱
				try
				{
					lbCalRet = this->_CalcCustomSpectrumAmpliferWindPump(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);

					ASSERT(lbCalRet >=0);
				}
				catch (...)
				{
					//writefile(pWave,iSmpNum,104);
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
					ASSERT(FALSE);
				}

				if (lbCalRet < 0)
				{
					//	return -1;
				}

				_pLockVibData->BeginWrite();
				memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
				//writefile(&(fBuf[0]),GD_CUSTOMPARAM_BUFFSIZE,1010);
				_pLockVibData->EndWrite();		

			}
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动

		/*
		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
		for (loop = 0; loop < iLen_; ++loop)
		{
		CVibChannParam * pVib = NULL;
		S_VibCharValue vibCharValue;

		int lnVibChannIndex = iStartIndex_+loop+_iRadialVibChannNum;

		if (lnVibChannIndex > _vVibChann.size())
		{
		break;
		}

		pVib = (CVibChannParam*)_vVibChann[lnVibChannIndex];

		//1.计算轴承和齿轮特征频率
		CBearingParamInfo bearingParamInfo=pVib->GetBearingParamInfo();
		CGearBoxParamInfo gearBoxParamInfo=pVib->GetGearBoxParamInfo();
		CChannLocations chanLocation=pVib->GetChannLocationsInfo();


		S_BearingDefectFreq defectFreq;
		S_GearBoxCharFreq gearboxCharFreq;

		//2.自定义参数列表大小
		lnCustromParamListSize = pVib->GetCustomParamSize();
		lnCustromParamListSize = 17;

		_pLockVibData->BeginRead();
		vibCharValue = pVib->GetCharValue();
		_pLockVibData->EndRead();

		float fWorkFreq = vibCharValue._iRev/60.0;

		double df = 0;

		if (vibCharValue._iSmpFreq !=0)
		{
		df = 1.0/vibCharValue._iSmpFreq;
		}

		int iSmpNum = vibCharValue._iSmpNum;

		int lnSpectrumSize = iSmpNum/2+1;

		std::vector<double> loVectorWave;

		loVectorWave.resize(iSmpNum,-1.0);

		double *pWave =  &loVectorWave.front();

		//3.得到波形
		_pLockVibData->BeginRead();

		const DATATYPE_WAVE * lpWaveData =  pVib->GetCurrWavePoint();				

		for (int i =0;i< iSmpNum;i++)
		{
		pWave[i] = lpWaveData[i] /10;
		}

		_pLockVibData->EndRead();


		//4.计算速度功率谱				
		BOOL lbCalRet = this->_CalcCustomSpectrumPower(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);				
		ASSERT(lbCalRet >0);	

		//5.计算包络谱
		lbCalRet = this->_CalcCustomSpectrumEnvelope(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);
		ASSERT(lbCalRet >0);

		//6.计算幅值谱
		lbCalRet = this->_CalcCustomSpectrumAmplifer(vibCharValue,pVib,fBuf,GD_CUSTOMPARAM_BUFFSIZE,pWave,fWorkFreq,df);
		ASSERT(lbCalRet >0);

		_pLockVibData->BeginWrite();
		memcpy(pVib->GetCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		_pLockVibData->EndWrite();	

		}
		iRtn=1;
		}*/

		break;
	default:
		iRtn = -1;
	}
	return -1;
}

int CPlantInfoOnLine::TransferCustomParamData(int iStartIndex_,
											  int iLen_,
											  std::vector<char> & arefBuffer,
											  CHANNTYPE channType  /*= GE_VIBCHANN*/,
											  BOOL abCalledFromAlarm /*= FALSE*/)
{
	S_PlantInfo  loPlantInfo = this->GetPlantInfo();

	int lnSize = 0;


	switch(channType)
	{
	case GE_VIBCHANN://径向振动

		if((iStartIndex_ + iLen_)<= loPlantInfo._iVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;
			int loop = 0;

			for (loop = 0; loop < iLen_; ++loop)
			{

				//得到给定测点的自定义模板的长度

				_pLockVibData->BeginRead();
				try
				{

					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+ loop];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);						
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}


				_pLockVibData->BeginWrite();

				try
				{
					if (abCalledFromAlarm)
					{
						memcpy(pVib->GetCustomParamData(), (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					}else
					{
						memcpy(pVib->GetRealCustomParamData(), (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();

			}

		}
		break;
	case GE_AXIALCHANN://轴向振动

		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
			for (int loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParam * pVib = NULL;
				S_VibCharValue vibCharValue;

				_pLockVibData->BeginRead();

				try
				{
					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);				
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}

				_pLockVibData->BeginWrite();

				try
				{
					if (abCalledFromAlarm)
					{
						memcpy(pVib->GetCustomParamData(), 
							(&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), 
							GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					}else
					{
						memcpy(pVib->GetRealCustomParamData(),
							(&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), 
							GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					}
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();
			}

		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量

		if((iStartIndex_ + iLen_)<= _sPlantInfo._iDynChannNum)
		{
			CDynChannParam * pDyn = NULL;
			S_DynCharValue dynCharValue;
			for (int loop = 0; loop < iLen_; ++loop)
			{
				_pLockDynData->BeginRead();

				try
				{
					pDyn = (CDynChannParam*)_vDynChann[iStartIndex_+ loop];


				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}

				_pLockDynData->EndRead();

				lnSize = pDyn->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}

				_pLockDynData->BeginWrite();

				if (abCalledFromAlarm)
				{
					memcpy(pDyn->GetCustomParamData(), (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

				}else
				{
					memcpy(pDyn->GetRealCustomParamData(), (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
				}
				_pLockDynData->EndWrite();

			}		
		}
		break;
	default:
		ASSERT(FALSE);
	}


	return 0;
}


int CPlantInfoOnLine::TransferCustomParamData(int iStartIndex_,
											  int iLen_,
											  std::vector<char> & arefBuffer,
											  S_CustomParamData* pCustomParamData_,
											  CHANNTYPE channType  /*= GE_VIBCHANN*/,
											  BOOL abCalledFromAlarm /*= FALSE*/)
{
	S_PlantInfo  loPlantInfo = this->GetPlantInfo();

	int lnSize = 0;


	switch(channType)
	{
	case GE_VIBCHANN://径向振动

		if((iStartIndex_ + iLen_)<= loPlantInfo._iVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;
			int loop = 0;

			for (loop = 0; loop < iLen_; ++loop)
			{

				//得到给定测点的自定义模板的长度

				_pLockVibData->BeginRead();
				try
				{

					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+ loop];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);						
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}


				try
				{
					memcpy(&pCustomParamData_[loop], (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


			}

		}
		break;
	case GE_AXIALCHANN://轴向振动

		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
			for (int loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParam * pVib = NULL;
				S_VibCharValue vibCharValue;

				_pLockVibData->BeginRead();

				try
				{
					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);				
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}

				try
				{
					
					memcpy(&pCustomParamData_[loop], 
						(&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), 
						GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

			}

		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量

		if((iStartIndex_ + iLen_)<= _sPlantInfo._iDynChannNum)
		{
			CDynChannParam * pDyn = NULL;
			S_DynCharValue dynCharValue;
			for (int loop = 0; loop < iLen_; ++loop)
			{
				_pLockDynData->BeginRead();

				try
				{
					pDyn = (CDynChannParam*)_vDynChann[iStartIndex_+ loop];


				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}

				_pLockDynData->EndRead();

				lnSize = pDyn->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}

				
				memcpy(&pCustomParamData_[loop], (&(arefBuffer[0])+(loop*sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE)), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			}		
		}
		break;
	default:
		ASSERT(FALSE);
	}


	return 0;
}


int CPlantInfoOnLine::CalcCustomParamData_New(int iStartIndex_,
											  int iLen_,
											  CHANNTYPE channType,
											  BOOL abCalledFromAlarm/* = FALSE*/)
{
	CSigMath loSigMath;

	BufferedBaseChannSigMathBasedOnSpecChann loBufferedSigMath;


	int loop = 0, loopCustom =0,iRtn=0;
	CCustomParamInfo paramInfo;
	float fBuf[GD_CUSTOMPARAM_BUFFSIZE] ={.0};
	int lnSize = 0;

	S_PlantInfo  loPlantInfo = this->GetPlantInfo();

	switch(channType)
	{
	case GE_VIBCHANN://径向振动

		if((iStartIndex_ + iLen_)<= loPlantInfo._iVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;


			for (loop = 0; loop < iLen_; ++loop)
			{

				//得到给定测点的自定义模板的长度

				_pLockVibData->BeginRead();
				try
				{

					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+ loop];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);						
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}


				int iPeriodStartIndex=0,iPeriodEndIndex=0;

				_pLockVibData->BeginRead();
				try
				{
					if (abCalledFromAlarm)
					{
						vibCharValue = pVib->GetCharValueEx();
						((CAlarmPlantInfo *)this)->GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);
					}else
					{
						vibCharValue = pVib->GetRealCharValueEx();
						GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex,true);
					}
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);			
				}
				_pLockVibData->EndRead();


				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;

				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(vibCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(vibCharValue._iSmpNum);

				double df = 0;

				if (vibCharValue._iSmpFreq* vibCharValue._iSmpNum >0)
				{
					df = (double)vibCharValue._iSmpFreq/(double)vibCharValue._iSmpNum;
				}

				if (df <0)
				{
					continue;
				}

				int lnSampleNum = vibCharValue._iSmpNum;

				int lnMiddleWareSum = pVib->GetWaveNum();

				if (lnSampleNum > lnMiddleWareSum)
				{
					lnSampleNum = lnMiddleWareSum;
					//log here
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d****************** : 数采配置的采样点数:%d 大于中间件组态配置的采样点数:%d，忽略多余点数"),
						__FILE__,
						__LINE__,vibCharValue._iSmpNum,
						lnMiddleWareSum);

					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);

				}

				if (abCalledFromAlarm)
				{

					ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pVib->GetCurrWavePoint(),
						lnSampleNum,
						pVib->WaveHead());

				}else
				{
					ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pVib->GetCurrRealWavePoint(),
						lnSampleNum,
						pVib->WaveHead());
				}

#ifdef _DEBUG
				writefile(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),vibCharValue._iSmpNum,222);
#endif // _DEBUG


				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = 
						loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}



				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					//得到通道的一个自定义参数模板
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					this->_pLockVibData->BeginWrite();

					try
					{

						if (abCalledFromAlarm)
						{
							CalcSingleCustomParamData_New(  paramInfo,
								*pVib,													 
								pVib->GetCurrWavePoint(),
								vibCharValue._iSmpNum, 
								vibCharValue._iSmpFreq ,
								iPeriodStartIndex,
								iPeriodEndIndex,
								vibCharValue._iRev,
								fBuf,
								aRefBufferedSigMath,
								loCalcBuffer,
								df);

						}else
						{
							CalcSingleCustomParamData_New(  paramInfo,
								*pVib,													 
								pVib->GetCurrRealWavePoint(),
								vibCharValue._iSmpNum, 
								vibCharValue._iSmpFreq ,
								iPeriodStartIndex,
								iPeriodEndIndex,
								vibCharValue._iRev,
								fBuf,
								aRefBufferedSigMath,
								loCalcBuffer,
								df);
						}					


					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					this->_pLockVibData->EndWrite();

					_pLockVibData->BeginWrite();

					try
					{
						if (abCalledFromAlarm)
						{
							memcpy(pVib->GetCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

						}else
						{
							memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
						}
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动
		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
			for (loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParam * pVib = NULL;
				S_VibCharValue vibCharValue;

				_pLockVibData->BeginRead();
				try
				{
					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);				
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}


				int iPeriodStartIndex=0,iPeriodEndIndex=0;
				_pLockVibData->BeginRead();
				try
				{
					if (abCalledFromAlarm)
					{
						vibCharValue = pVib->GetCharValueEx();
						((CAlarmPlantInfo *)this)->GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);
					}else
					{
						vibCharValue = pVib->GetRealCharValueEx();
						GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex,true);
					}
				
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndRead();

				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;
				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(vibCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(vibCharValue._iSmpNum);


				double df = 0;

				if (vibCharValue._iSmpFreq* vibCharValue._iSmpNum >0)
				{
					df = (double)vibCharValue._iSmpFreq/(double)vibCharValue._iSmpNum;
				}

				if (df <0)
				{
					continue;
				}

				if (abCalledFromAlarm)
				{
					ConvertDTWaveToDouble(	&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pVib->GetCurrWavePoint(),
						vibCharValue._iSmpNum,
						pVib->WaveHead());
				}else
				{
					ConvertDTWaveToDouble(	&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pVib->GetCurrRealWavePoint(),
						vibCharValue._iSmpNum,
						pVib->WaveHead());
				}

				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}


				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}
					_pLockVibData->BeginRead();

					if (abCalledFromAlarm)
					{
						vibCharValue = pVib->GetCharValueEx();

					}else
					{
						vibCharValue = pVib->GetRealCharValueEx();

					}
					_pLockVibData->EndRead();

					_pLockVibData->BeginWrite();
					try
					{
						if (abCalledFromAlarm)
						{
							CalcSingleCustomParamData_New(  paramInfo,
								*pVib,													 
								pVib->GetCurrWavePoint(),
								vibCharValue._iSmpNum, 
								vibCharValue._iSmpFreq ,
								iPeriodStartIndex,
								iPeriodEndIndex,
								vibCharValue._iRev,
								fBuf,
								aRefBufferedSigMath,
								loCalcBuffer,
								df);	
						}else
						{
							CalcSingleCustomParamData_New(  paramInfo,
								*pVib,													 
								pVib->GetCurrRealWavePoint(),
								vibCharValue._iSmpNum, 
								vibCharValue._iSmpFreq ,
								iPeriodStartIndex,
								iPeriodEndIndex,
								vibCharValue._iRev,
								fBuf,
								aRefBufferedSigMath,
								loCalcBuffer,
								df);	
						}

					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();

					_pLockVibData->BeginWrite();
					try
					{
						if (abCalledFromAlarm)
						{
							memcpy(pVib->GetCustomParamData(), 
								&(fBuf[0]), 
								GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

						}else
						{
							memcpy(pVib->GetRealCustomParamData(),
								&(fBuf[0]), 
								GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

						}
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量
		if((iStartIndex_ + iLen_)<= _sPlantInfo._iDynChannNum)
		{
			CDynChannParam * pDyn = NULL;
			S_DynCharValue dynCharValue;
			for (loop = 0; loop < iLen_; ++loop)
			{


				_pLockDynData->BeginRead();
				try
				{
					pDyn = (CDynChannParam*)_vDynChann[iStartIndex_+ loop];


				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}

				_pLockDynData->EndRead();

				lnSize = pDyn->GetCustomParamSize();

				if (lnSize <=0)
				{
					continue;
				}

				int iPeriodStartIndex=0,iPeriodEndIndex=0;

				_pLockDynData->BeginRead();
				try
				{
					if (abCalledFromAlarm)
					{
						dynCharValue = pDyn->GetCharValueEx();
						((CAlarmPlantInfo *) this)->GetSpecDynChannStartEndIndex(pDyn->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					}else
					{
						dynCharValue = pDyn->GetRealCharValueEx();
						GetSpecDynChannStartEndIndex(pDyn->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex,true);
					}

					
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}
				_pLockDynData->EndRead();

				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;

				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(dynCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(dynCharValue._iSmpNum);



				double df = 0;

				if (dynCharValue._iSmpFreq* dynCharValue._iSmpNum >0)
				{
					df = (double)dynCharValue._iSmpFreq/(double)dynCharValue._iSmpNum;
				}

				if (df <= 0)
				{
					continue;
				}

				if (abCalledFromAlarm)
				{
					ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pDyn->GetCurrWavePoint(),
						dynCharValue._iSmpNum,
						pDyn->WaveHead());

				}else
				{
					ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
						pDyn->GetCurrRealWavePoint(),
						dynCharValue._iSmpNum,
						pDyn->WaveHead());
				}


				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}

				lnSize = pDyn->GetCustomParamSize();

				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					paramInfo =pDyn->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					_pLockDynData->BeginRead();

					if (abCalledFromAlarm)
					{
						dynCharValue = pDyn->GetCharValueEx();					

					}else
					{
						dynCharValue = pDyn->GetRealCharValueEx();					
					}

					_pLockDynData->EndRead();

					this->_pLockDynData->BeginWrite();

					if (abCalledFromAlarm)
					{
						CalcSingleCustomParamData_New(paramInfo,
							*pDyn,													 
							pDyn->GetCurrWavePoint(),
							dynCharValue._iSmpNum, 
							dynCharValue._iSmpFreq ,
							iPeriodStartIndex,
							iPeriodEndIndex,
							dynCharValue._iRev,
							fBuf,
							aRefBufferedSigMath,
							loCalcBuffer,
							df);
					}else
					{
						CalcSingleCustomParamData_New(paramInfo,
							*pDyn,													 
							pDyn->GetCurrRealWavePoint(),
							dynCharValue._iSmpNum, 
							dynCharValue._iSmpFreq ,
							iPeriodStartIndex,
							iPeriodEndIndex,
							dynCharValue._iRev,
							fBuf,
							aRefBufferedSigMath,
							loCalcBuffer,
							df);
					}

					this->_pLockDynData->EndWrite();

					_pLockDynData->BeginWrite();

					if (abCalledFromAlarm)
					{
						memcpy(pDyn->GetCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					}else
					{
						memcpy(pDyn->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					_pLockDynData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	default:
		iRtn = -1;
	}
#ifdef _DEBUG
	writefile(&(fBuf[0]),GD_CUSTOMPARAM_BUFFSIZE,2020);
#endif // _DEBUG

	return iRtn;
}

int CPlantInfoOnLine::CalcCustomParamData_New( int iStartIndex_,
											  int iLen_,
											  void *pCharValue_,
										      DATATYPE_WAVE *pWave_,
											  S_CustomParamData* pCustomParamData_,
											  CHANNTYPE channType /*= GE_VIBCHANN*/,
											  BOOL abCalledFromAlarm /*= FALSE*/ )
{
	CSigMath loSigMath;

	BufferedBaseChannSigMathBasedOnSpecChann loBufferedSigMath;


	int loop = 0, loopCustom =0,iRtn=0;
	CCustomParamInfo paramInfo;
	float fBuf[GD_CUSTOMPARAM_BUFFSIZE] ={.0};
	int lnSize = 0;

	int iPointWaveNum = 0;
	S_VibCharValue *pVibCharValue;
	S_DynCharValue *pDynCharValue;

	S_PlantInfo  loPlantInfo = this->GetPlantInfo();

	switch(channType)
	{
	case GE_VIBCHANN://径向振动

		 pVibCharValue = (S_VibCharValue*)pCharValue_;
		 iPointWaveNum = 0;

		if((iStartIndex_ + iLen_)<= loPlantInfo._iVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;

			
			for (loop = 0; loop < iLen_; ++loop)
			{

				//得到给定测点的自定义模板的长度

				vibCharValue = pVibCharValue[loop];

				_pLockVibData->BeginRead();
				try
				{

					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+ loop];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);						
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					iPointWaveNum += vibCharValue._iSmpNum;
					continue;
				}


				int iPeriodStartIndex=0,iPeriodEndIndex=0;

				_pLockVibData->BeginRead();
				try
				{
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,vibCharValue,iPeriodStartIndex,iPeriodEndIndex,true);

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);			
				}
				_pLockVibData->EndRead();


				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;

				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(vibCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(vibCharValue._iSmpNum);

				double df = 0;

				if (vibCharValue._iSmpFreq* vibCharValue._iSmpNum >0)
				{
					df = (double)vibCharValue._iSmpFreq/(double)vibCharValue._iSmpNum;
				}

				if (df <0)
				{
					iPointWaveNum += vibCharValue._iSmpNum;
					continue;
				}

				int lnSampleNum = vibCharValue._iSmpNum;
				
				ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
					&pWave_[iPointWaveNum],
					lnSampleNum,
					pVib->WaveHead());



#ifdef _DEBUG
				writefile(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),vibCharValue._iSmpNum,222);
#endif // _DEBUG


				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = 
						loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}



				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					//得到通道的一个自定义参数模板
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					this->_pLockVibData->BeginWrite();

					try
					{

						
						CalcSingleCustomParamData_New(  paramInfo,
							*pVib,												 
							&pWave_[iPointWaveNum],
							vibCharValue._iSmpNum, 
							vibCharValue._iSmpFreq,
							iPeriodStartIndex,
							iPeriodEndIndex,
							vibCharValue._iRev,
							fBuf,
							aRefBufferedSigMath,
							loCalcBuffer,
							df);					


					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					this->_pLockVibData->EndWrite();

					try
					{
						
						memcpy(&pCustomParamData_[loop], &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

				}
				iPointWaveNum += lnSampleNum;
			}
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动
		pVibCharValue = (S_VibCharValue*)pCharValue_;

		iPointWaveNum = 0;

		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;

			for (loop = 0; loop < iLen_; ++loop)
			{
				
				vibCharValue = pVibCharValue[loop];

				_pLockVibData->BeginRead();
				try
				{
					pVib = (CVibChannParam*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);				
				}

				_pLockVibData->EndRead();

				lnSize = pVib->GetCustomParamSize();

				if (lnSize <=0)
				{
					iPointWaveNum += vibCharValue._iSmpNum;
					continue;
				}


				int iPeriodStartIndex=0,iPeriodEndIndex=0;
				_pLockVibData->BeginRead();
				try
				{
					
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,vibCharValue,iPeriodStartIndex,iPeriodEndIndex,true);

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndRead();

				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;
				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(vibCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(vibCharValue._iSmpNum);


				double df = 0;

				if (vibCharValue._iSmpFreq* vibCharValue._iSmpNum >0)
				{
					df = (double)vibCharValue._iSmpFreq/(double)vibCharValue._iSmpNum;
				}

				if (df <0)
				{
					iPointWaveNum += vibCharValue._iSmpNum;
					continue;
				}
				
				ConvertDTWaveToDouble(	&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
					&pWave_[iPointWaveNum],
					vibCharValue._iSmpNum,
					pVib->WaveHead());
	

				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}


				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					_pLockVibData->BeginWrite();
					try
					{
						
						CalcSingleCustomParamData_New(  paramInfo,
							*pVib,													 
							&pWave_[iPointWaveNum],
							vibCharValue._iSmpNum, 
							vibCharValue._iSmpFreq ,
							iPeriodStartIndex,
							iPeriodEndIndex,
							vibCharValue._iRev,
							fBuf,
							aRefBufferedSigMath,
							loCalcBuffer,
							df);	

					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();

					try
					{
						
						memcpy(&pCustomParamData_[loop],
							&(fBuf[0]), 
							GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

				}
				iPointWaveNum += vibCharValue._iSmpNum;
			}
			iRtn=1;
		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量
		pDynCharValue = (S_DynCharValue*)pCharValue_;
		iPointWaveNum = 0;
		if((iStartIndex_ + iLen_)<= _sPlantInfo._iDynChannNum)
		{
			CDynChannParam * pDyn = NULL;
			S_DynCharValue dynCharValue;

			for (loop = 0; loop < iLen_; ++loop)
			{
				dynCharValue = pDynCharValue[loop];

				_pLockDynData->BeginRead();
				try
				{
					pDyn = (CDynChannParam*)_vDynChann[iStartIndex_+ loop];


				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}

				_pLockDynData->EndRead();

				lnSize = pDyn->GetCustomParamSize();

				if (lnSize <=0)
				{
					iPointWaveNum += dynCharValue._iSmpNum;
					continue;
				}

				int iPeriodStartIndex=0,iPeriodEndIndex=0;

				_pLockDynData->BeginRead();
				try
				{
					GetSpecDynChannStartEndIndex(pDyn->GetChannInfo()._cChannID,dynCharValue,iPeriodStartIndex,iPeriodEndIndex,true);
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);					
				}
				_pLockDynData->EndRead();

				BufferedBaseChannSigMathBasedOnSpecChann aRefBufferedSigMath;

				BufferForCustomCalc loCalcBuffer;

				//频谱计算用的临时缓存
				loCalcBuffer.m_ovMapDoubleWaveBufferSpectrum.resize(dynCharValue._iSmpNum/2);

				//波形short到double转换后的double缓存
				loCalcBuffer.m_ovDoubleWaveBuffer.resize(dynCharValue._iSmpNum);



				double df = 0;

				if (dynCharValue._iSmpFreq* dynCharValue._iSmpNum >0)
				{
					df = (double)dynCharValue._iSmpFreq/(double)dynCharValue._iSmpNum;
				}

				if (df <= 0)
				{
					iPointWaveNum += dynCharValue._iSmpNum;
					continue;
				}

				
				ConvertDTWaveToDouble(&loCalcBuffer.m_ovDoubleWaveBuffer.front(),
					&pWave_[iPointWaveNum],
					dynCharValue._iSmpNum,
					pDyn->WaveHead());


				int lnBufferSize = loCalcBuffer.m_ovDoubleWaveBuffer.size();

				for (int i =0;i<lnBufferSize;i++)
				{
					loCalcBuffer.m_ovDoubleWaveBuffer[i] = loCalcBuffer.m_ovDoubleWaveBuffer[i]*paramInfo._fPREPROC_K + paramInfo._fPREPROC_B;
				}

				lnSize = pDyn->GetCustomParamSize();

				for( loopCustom = 0; loopCustom < lnSize; ++loopCustom)
				{
					paramInfo =pDyn->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					this->_pLockDynData->BeginWrite();

					try
					{
						CalcSingleCustomParamData_New(paramInfo,
							*pDyn,													 
							&pWave_[iPointWaveNum],
							dynCharValue._iSmpNum, 
							dynCharValue._iSmpFreq ,
							iPeriodStartIndex,
							iPeriodEndIndex,
							dynCharValue._iRev,
							fBuf,
							aRefBufferedSigMath,
							loCalcBuffer,
							df);

					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Error(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);	
					}

					this->_pLockDynData->EndWrite();

					memcpy(&pCustomParamData_[loop], &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
				}

				iPointWaveNum += dynCharValue._iSmpNum;
			}
			iRtn=1;
		}
		break;
	default:
		iRtn = -1;
	}
#ifdef _DEBUG
	writefile(&(fBuf[0]),GD_CUSTOMPARAM_BUFFSIZE,2020);
#endif // _DEBUG

	return iRtn;
}
int CPlantInfoOnLine::CalcCustomParamData(int iStartIndex_,int iLen_,CHANNTYPE channType)
{

	if((GE_MACHINETYPE_WINDPEQ == _sPlantInfo._iMachineType ) ||
		(GE_MACHINETYPE_KEYPUMP == _sPlantInfo._iMachineType ) ||
		(GE_MACHINETYPE_PUMP == _sPlantInfo._iMachineType ) )
	{
		BOOL lbRet = -1;

		try
		{
			lbRet = CalcCustomParamDataWindPump(iStartIndex_,iLen_,channType);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			lbRet = -1;					
		}

		return lbRet;		
	}
	int loop = 0, loopCustom =0,iRtn=0;
	CCustomParamInfo paramInfo;
	float fBuf[GD_CUSTOMPARAM_BUFFSIZE] ={.0};
	int iSize = 0;
	switch(channType)
	{
	case GE_VIBCHANN://径向振动
		if((iStartIndex_ + iLen_)<= _sPlantInfo._iVibChannNum)
		{
			CVibChannParam * pVib = NULL;
			S_VibCharValue vibCharValue;
			for (loop = 0; loop < iLen_; ++loop)
			{
				pVib = (CVibChannParam*)_vVibChann[iStartIndex_+ loop];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pVib->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pVib->GetGearBoxParamInfo();
				CChannLocations chanLocation=pVib->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pVib->GetCustomParamSize();

				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					_pLockVibData->BeginRead();
					try
					{
						vibCharValue = pVib->GetCharValue();
					}
					catch (...)
					{
						//log here
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

					}

					_pLockVibData->EndRead();

					CalcBearingDefectFreq(bearingParamInfo,vibCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,vibCharValue._iRev,gearboxCharFreq);

					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					this->_pLockVibData->BeginRead();

					//{
					//	vector<float> fWaveData;
					//	fWaveData.resize(vibCharValue._iSmpNum);

					//	ConvertDTWaveToFloat(&fWaveData[0], pVib->GetCurrWavePoint(), vibCharValue._iSmpNum);

					//	CString lsTitle;
					//	lsTitle.Format("W_%s", GetRealVibDataDateTime().Format("%Y%m%d%H%M%S"));
					//	writefile(&fWaveData[0], vibCharValue._iSmpNum, lsTitle);
					//}


					try
					{
						CalcSingleCustomParamData(paramInfo,
							defectFreq,
							gearboxCharFreq,
							pVib->GetCurrRealWavePoint(),
							vibCharValue._iSmpNum, 
							vibCharValue._iSmpFreq ,
							iPeriodStartIndex,
							iPeriodEndIndex,
							vibCharValue._iRev,
							&(fBuf[0]),
							pVib->WaveHead());
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					this->_pLockVibData->EndRead();

					_pLockVibData->BeginWrite();
					try
					{
						memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();

					//{
					//	CString lsTitle;
					//	lsTitle.Format("Cs_%s", GetRealVibDataDateTime().Format("%Y%m%d%H%M%S"));
					//	writefile(fBuf, GD_CUSTOMPARAM_BUFFSIZE, lsTitle);
					//}

				}
			}
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动
		if((iStartIndex_ + iLen_)<= _iAxialVibChannNum)
		{
			for (loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParam * pVib = NULL;
				S_VibCharValue vibCharValue;
				pVib = (CVibChannParam*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pVib->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pVib->GetGearBoxParamInfo();
				CChannLocations chanLocation=pVib->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pVib->GetCustomParamSize();
				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}
					_pLockVibData->BeginRead();
					try
					{
						vibCharValue = pVib->GetCharValue();
					}
					catch (...)
					{
						//log here
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

					}

					_pLockVibData->EndRead();

					CalcBearingDefectFreq(bearingParamInfo,vibCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,vibCharValue._iRev,gearboxCharFreq);
					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					_pLockVibData->BeginRead();
					try
					{
						CalcSingleCustomParamData(paramInfo,defectFreq,gearboxCharFreq, pVib->GetCurrRealWavePoint(),vibCharValue._iSmpNum, vibCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,vibCharValue._iRev,&(fBuf[0]),pVib->WaveHead());
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndRead();

					_pLockVibData->BeginWrite();
					try
					{
						memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量
		if((iStartIndex_ + iLen_)<= _sPlantInfo._iDynChannNum)
		{
			CDynChannParam * pDyn = NULL;
			S_DynCharValue dynCharValue;
			for (loop = 0; loop < iLen_; ++loop)
			{
				pDyn = (CDynChannParam*)_vDynChann[iStartIndex_+ loop];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pDyn->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pDyn->GetGearBoxParamInfo();
				CChannLocations chanLocation=pDyn->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pDyn->GetCustomParamSize();
				//CString sTemp;sTemp.Format("pDyn->GetCustomParamSize():%d",iSize);BHTRACE_DEBUG(sTemp);
				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo =pDyn->GetSpecIndexCustomParamInfo(loopCustom);

					if (paramInfo._iID < 0)
					{
						ASSERT(FALSE);
						continue;
					}

					_pLockDynData->BeginRead();
					try
					{
						dynCharValue = pDyn->GetCharValue();
					}
					catch (...)
					{
						//log here
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

					}					
					_pLockDynData->EndRead();
					CalcBearingDefectFreq(bearingParamInfo,dynCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,dynCharValue._iRev,gearboxCharFreq);
					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecDynChannStartEndIndex(pDyn->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					this->_pLockDynData->BeginRead();
					try
					{
						CalcSingleCustomParamData(paramInfo,defectFreq,gearboxCharFreq, pDyn->GetCurrRealWavePoint(),dynCharValue._iSmpNum, dynCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,dynCharValue._iRev,&(fBuf[0]),pDyn->WaveHead());
					}
					catch (...)
					{
						//log here
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

					}

					this->_pLockDynData->EndRead();
					//CString sTemp;sTemp.Format("pDyn:_iSmpNum:%d,_iSmpFreq:%d,iPeriodStartIndex:%d,iPeriodEndIndex:%d,_iRev:%d",dynCharValue._iSmpNum, dynCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,dynCharValue._iRev);BHTRACE_DEBUG(sTemp);


					_pLockDynData->BeginWrite();
					try
					{
						memcpy(pDyn->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						//log here
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

					}

					_pLockDynData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	default:
		iRtn = -1;
	}
	return iRtn;
}

int CPlantInfoOnLine::CalcSingleCustomParamData_New(CCustomParamInfo& arefParamInfo_,
													CChannInfoBase & arefChanninfo,
													const DATATYPE_WAVE *pWaveData_,
													int& ilen_,
													int iSmpFreq_,
													int iStartIndex_,
													int iEndIndex_,
													int iRev_,
													float* apfCustomParamData_,
													BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath,
													BufferForCustomCalc & aRefBufferObj, //计算使用的缓存对象
													double adbldf)
{
	int lnSegNum = arefParamInfo_._iSegNum;
	int lnSegStartNo = arefParamInfo_._iSegStartNo;
	float lfStartValue =  arefParamInfo_._fDataStartValue;
	float lfEndValue = arefParamInfo_._fDataEndValue;
	E_CUSTOMPARAM_CHARTYPE lnCharTye = arefParamInfo_._eCharType;
	int lnUnitType=arefParamInfo_._eUnitType;
	int lnSigAnaType=arefParamInfo_._eSigAnaType;
	if (arefParamInfo_._iSegStartNo >= GD_CUSTOMPARAM_BUFFSIZE || lnSegNum <=0 || lnSegStartNo <0)
	{
		return -1;
	}

	std::vector<float> loDataBuffer;
	loDataBuffer.resize(lnSegNum);

	float *lpfCustomParamValue= &loDataBuffer.front();

	if (ilen_<2 || iSmpFreq_ <=0 || (lfStartValue >=lfEndValue) || adbldf <=0)
	{
		ASSERT(FALSE);

		return -1;
	}


	std::vector<double> & lRefWaveBuffer = aRefBufferObj.m_ovDoubleWaveBuffer;
	CSigMath loSigMath;

	//信号的预处理方式
	switch(arefParamInfo_._iPreProcType)
	{

		//积分
	case E_CUSTOMPARAM_PREPROC_INT:
		//没有计算过积分
		if (aRefBufferObj.m_ovMapIntegrateDoubleWaveBuffer.size() == 0)
		{
			ASSERT(FALSE);
		}
		break;

		//微分
	case E_CUSTOMPARAM_PREPROC_DIFF:
		if (aRefBufferObj.m_ovMapDiffDoubleWaveBuffer.size() == 0)
		{
			ASSERT(FALSE);
		}		
		break;

		//不处理
	case E_CUSTOMPARAM_PREPROC_NONE:
		lRefWaveBuffer = aRefBufferObj.m_ovDoubleWaveBuffer;
		break;
	default:

		lRefWaveBuffer = aRefBufferObj.m_ovDoubleWaveBuffer;
	}

	switch (arefParamInfo_._eDataType)
	{

		//时域波形
	case  E_CUSTOMPARAM_TIMEDOMAIN:

		switch(lnSigAnaType)
		{
			//计算包络波形
		case E_CUSTOMPARAM_ENVELOPEWAVE:
			{
				/*				_DECLARE_PERF_MEASURE_TIME();*/

				/*				_BEGIN_PERF_MEASURE_TIME();*/

				if (aRefBufferObj.m_ovMapDoubleEnvelopeWaveBuffer.size() == 0)
				{
					aRefBufferObj.m_ovMapDoubleEnvelopeWaveBuffer.resize(ilen_);
				}

				lRefWaveBuffer = aRefBufferObj.m_ovMapDoubleEnvelopeWaveBuffer;
				double ldbGseValue = 0;
				double *pfWaveData= &(lRefWaveBuffer.front());
				double ldbBandEndFreq = arefParamInfo_._fBandEndValue;
				if (arefParamInfo_._fBandEndValue > iSmpFreq_/3)
				{
					ldbBandEndFreq = iSmpFreq_/3;
				}

				if (arefParamInfo_._fBandEndValue < 1000)
				{
					arefParamInfo_._fBandEndValue = 1000;
				}

				if (ilen_%2!=0)
				{
					ilen_ = ilen_ -1;
				}

				if (ilen_ <=0)
				{
					return -1;
				}

				int lnRet = aRefBufferedSigMath.gSEAnalysis(	pfWaveData,
					ilen_,
					iSmpFreq_,
					ldbBandEndFreq,
					ldbGseValue,
					&(aRefBufferObj.m_ovMapDoubleEnvelopeWaveBuffer.front()));

				/*				_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_ENVELOPEWAVE");*/

				if (lnRet <0)
				{
					ASSERT(FALSE);

					return -1;
				}

			}
			break;
		case E_CUSTOMPARAM_TIMEWAVE:
			break;
		default:
			ASSERT(FALSE);
			break;

		}

		//根据时域数据计算自定义相关参数
		CalcWaveCustomParamValue_New(	pWaveData_,
			ilen_,
			iSmpFreq_,
			iStartIndex_,
			iEndIndex_,
			arefParamInfo_._eUnitType, 
			arefParamInfo_._eSigAnaType,
			lnSegNum,
			lnSegStartNo,
			lnCharTye,
			lfStartValue,
			lfEndValue,
			lpfCustomParamValue,
			aRefBufferedSigMath,
			lRefWaveBuffer);
		break;

		//频域
	case E_CUSTOMPARAM_FREQDOMAIN:

		{

			//根据频域数据计算自定义相关参数

			double *pfFreq= &(aRefBufferObj.m_ovMapDoubleWaveBufferSpectrum.front());
			ZeroMemory(pfFreq,aRefBufferObj.m_ovMapDoubleWaveBufferSpectrum.size()*sizeof(double));

			double *pfWaveData= &(lRefWaveBuffer.front());

			if (ilen_%2 != 0)
			{
				ilen_ = ilen_-1;
			}

			try
			{
				/*				_DECLARE_PERF_MEASURE_TIME();*/

				switch(lnSigAnaType)
				{
				case E_CUSTOMPARAM_PEAKSPEC:
					{
						//得到峰值谱
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						if (aRefBufferedSigMath.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_PEAK)<0)
						{
							ASSERT(FALSE);
							return -1;
						}
						/*						_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_PEAKSPEC");*/

					}
					break;
				case E_CUSTOMPARAM_PEAKPEAKSPEC:
					{
						//得到峰峰值谱
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						if (aRefBufferedSigMath.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_Peak_Peak)<0)
						{
							ASSERT(FALSE);
							return -1;
						}
						/*						_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_PEAKPEAKSPEC");*/

					}
					break;
				case E_CUSTOMPARAM_POWERSPEC_INTEGRAL:
					{
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						//得到功率谱
						if(aRefBufferedSigMath.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_POWER)<0)
						{
							ASSERT(FALSE);
							return -1;
						}

						if( aRefBufferedSigMath.SpectrumIntegrate(pfFreq, adbldf, ilen_/2,0.001,E_SpectrumType_POWER) < 0)
						{
							return -2;
						}

						/*						_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_POWERSPEC");*/
					}
					break;
				case E_CUSTOMPARAM_POWERSPEC:
					{
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						//得到功率谱
						if(aRefBufferedSigMath.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_POWER)<0)
						{
							/*ASSERT(FALSE);*/
							return -1;
						}
						/*						_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_POWERSPEC");*/
					}
					break;

				case E_CUSTOMPARAM_WRAPPERSPEC:
					{
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						double ldbGseValue = 0;
						std::vector<double> ldbEnvolopBuffer;
						ldbEnvolopBuffer.resize(ilen_);
						double ldbBandEndFreq = arefParamInfo_._fBandEndValue;
						if (arefParamInfo_._fBandEndValue > iSmpFreq_/3)
						{
							ldbBandEndFreq = iSmpFreq_/3;
						}

						if (arefParamInfo_._fBandEndValue < 1000)
						{
							arefParamInfo_._fBandEndValue = 1000;
						}


						int lnRet = aRefBufferedSigMath.gSEAnalysis(pfWaveData,
							ilen_,
							iSmpFreq_,
							ldbBandEndFreq,
							ldbGseValue,
							&ldbEnvolopBuffer.front());

						if (lnRet>=0)
						{
							if (aRefBufferedSigMath.SpectralMeasureMent(&ldbEnvolopBuffer.front(),ilen_,pfFreq,E_SpectrumType_Peak_Peak)<0)
							{
								ASSERT(FALSE);
								return -1;
							}

						}else
						{
/*
							ASSERT(FALSE);
							writefile(pfWaveData,ilen_,111);*/

							return -1;
						}
						/*						_END_PERF_MEASURE_TIME("E_CUSTOMPARAM_WRAPPERSPEC");*/
					}
					break;
				default:
					{
						ASSERT(FALSE);
						/*						_BEGIN_PERF_MEASURE_TIME();*/
						//算峰值谱
						if (aRefBufferedSigMath.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_PEAK)<0)
						{
							ASSERT(FALSE);
							return -1;
						}
						/*						_END_PERF_MEASURE_TIME("default");*/

					}
				}

				/*				_BEGIN_PERF_MEASURE_TIME();*/

				CalcFreqCustomParamValue_New(	
					pfFreq,
					ilen_,
					iRev_,
					lnUnitType,
					lnSigAnaType,
					lnSegNum,
					lnSegStartNo,
					lnCharTye,
					lfStartValue,
					lfEndValue,
					lpfCustomParamValue,
					adbldf,
					aRefBufferedSigMath);


			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Error(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

		}
		break;

	}	
	int lnDataCopyCount = lnSegNum;
	if ((lnSegStartNo + lnSegNum)>GD_CUSTOMPARAM_BUFFSIZE)
	{
		lnDataCopyCount = GD_CUSTOMPARAM_BUFFSIZE - lnSegStartNo;

		if (lnDataCopyCount<0)
		{
			return -1;
		}
	}
	memcpy(apfCustomParamData_+lnSegStartNo,lpfCustomParamValue,lnDataCopyCount*sizeof(float));

	return lnDataCopyCount;
}

int CPlantInfoOnLine::CalcSingleCustomParamData(CCustomParamInfo& paramInfo_,
												S_BearingDefectFreq& defectFreq_,
												S_GearBoxCharFreq& gearboxCharFreq_,
												const DATATYPE_WAVE *pWaveData_,
												int& ilen_,
												int iSmpFreq_,
												int iStartIndex_,
												int iEndIndex_,
												int iRev_,
												float *pfCustomParamData_,
												CWaveHead& sWaveHead_)
{
	int iSegNum = paramInfo_._iSegNum;
	if (iSegNum<=0 || ilen_ <=0)
	{
		return 0;
	}
	int iSegStartNo = paramInfo_._iSegStartNo;
	float fStartValue =  paramInfo_._fDataStartValue;
	float fEndValue = paramInfo_._fDataEndValue;
	E_CUSTOMPARAM_CHARTYPE iCharTye = paramInfo_._eCharType;
	int iUnitType=paramInfo_._eUnitType;
	int iSigAnaType=paramInfo_._eSigAnaType;
	float *pfCustomParamValue=new float[iSegNum];
	memset(pfCustomParamValue,0,iSegNum*sizeof(float));
	if (paramInfo_._eDataType == E_CUSTOMPARAM_TIMEDOMAIN)
	{
		//根据时域数据计算自定义相关参数
		CalcWaveCustomParamValue(pWaveData_,
			ilen_,
			iSmpFreq_,
			iStartIndex_,
			iEndIndex_,
			paramInfo_._eUnitType, 
			paramInfo_._eSigAnaType,
			iSegNum,
			iSegStartNo,
			iCharTye,
			fStartValue,
			fEndValue,
			pfCustomParamValue,
			sWaveHead_);
	}
	else
	{
		//根据频域数据计算自定义相关参数
		/*
		CSigMath calcValue;
		double *pfFreq=new double [ilen_/2];
		memset(pfFreq,0,(ilen_/2)*sizeof(double));
		double *pfWaveData=new double[ilen_];
		memset(pfWaveData,0,ilen_*sizeof(double));
		ConvertDTWaveToDouble(pfWaveData,pWaveData_,ilen_);
		switch(iSigAnaType)
		{
		case E_CUSTOMPARAM_PEAKSPEC:
		{
		//得到峰值谱
		calcValue.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_PEAK);
		}
		break;
		case E_CUSTOMPARAM_PEAKPEAKSPEC:
		{
		//得到峰峰值谱
		calcValue.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_Peak_Peak);
		for (int i=0;i<ilen_/2;i++)
		pfFreq[i]=2*pfFreq[i];
		}
		break;
		case E_CUSTOMPARAM_POWERSPEC:
		{
		//得到功率谱
		calcValue.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_POWER);
		}
		break;
		//[TBD]包络谱待加
		default:
		{
		//算峰值谱
		calcValue.SpectralMeasureMent(pfWaveData,ilen_,pfFreq,E_SpectrumType_PEAK);
		}
		}
		CalcFreqCustomParamValue(pfFreq,ilen_,iRev_,iUnitType,iSigAnaType,iSegNum,iSegStartNo,iCharTye,fStartValue,fEndValue,defectFreq_,gearboxCharFreq_,pfCustomParamValue);
		KD_DeletePoint(pfWaveData);
		KD_DeletePoint(pfFreq);*/

	}
	memcpy(&(pfCustomParamData_[iSegStartNo]),pfCustomParamValue, iSegNum*sizeof(float));
	KD_DeletePoint(pfCustomParamValue);
	//CString sTemp;sTemp.Format("paramInfo_._cTempletID:%s,pfCustomParamValue[0]:%f,pfCustomParamValue[1]:%f,pfCustomParamValue[2]:%f,pfCustomParamValue[3]:%f",paramInfo_._cTempletID,pfCustomParamValue[0],pfCustomParamValue[1],pfCustomParamValue[2],pfCustomParamValue[3]);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CPlantInfoOnLine","CalcSingleCustomParamData");
	return iSegNum;
}


/*
可用的实现CSigMath::FeatureValue
*/
inline float myCalcPeakPeakValue(float * apfWaveData_, int aiWaveLen_)
{
	ASSERT(aiWaveLen_ > 0);
	if (aiWaveLen_ < 1)	{
		return 0;
	}

	ASSERT(apfWaveData_ != NULL);
	if(NULL == apfWaveData_) {
		return 0;
	}

	float fPeakPeakVal = 0;


	float fmax = 0;
	float fmin = 0;


	fmax = fmin = apfWaveData_[0];

	for (int i = 0; i < aiWaveLen_; i++) {
		if (fmax < apfWaveData_[i])	{
			fmax = apfWaveData_[i];
		}

		if (fmin > apfWaveData_[i])	{
			fmin = apfWaveData_[i];
		}
	}


	fPeakPeakVal = (fmax - fmin);

#if 0
	/* FeatureValue 计算出的差异太大, 暂不使用 */
	CSigMath loSigMath;

	vector<double> lvWaveData;
	lvWaveData.resize(aiWaveLen_);

	for (int i = 0; i < aiWaveLen_; i++)
	{
		lvWaveData[i] = static_cast<double>(apfWaveData_[i]);
	}

	int lnWaveDataLen = aiWaveLen_;
	float fPeakPeakVal1 = loSigMath.FeatureValue(&lvWaveData[0], 
		lnWaveDataLen, 
		E_FeatureValueType_PeakPeak_True);

	ASSERT(lnWaveDataLen == aiWaveLen_);
	ASSERT(::fabs(fPeakPeakVal1 - fPeakPeakVal) < 0.01);
#endif


	return fPeakPeakVal;
}

void CPlantInfoOnLine::CalcWaveCustomParamValue(const DATATYPE_WAVE *pfWave_,
												int& ilen_,
												int iSmpFreq_,
												int iStartIndex_,
												int iEndIndex_,
												int iUnitType_,
												int iSigAnaType_,
												int iSegNum_,
												int iSegStartNo_,
												int iCharTye_,
												float fStartValue_,
												float fEndValue_,
												float *fCustomParamValue_,
												CWaveHead& sWaveHead_)
{

	int iStarLoop=0,iEndLoop=0,iPeriodNum=0;
	float dtDelta=.0,fValue=.0;

	if (NULL == pfWave_)
	{
		return;
	}

	if (iSmpFreq_ * ilen_*iSegNum_ <=0)
	{
		ASSERT(FALSE);
		return;
	}

	switch(iUnitType_)
	{
	case E_CUSTOMPARAM_TIME:
		{
			//将得到的时间转化为波形点数
			dtDelta=1/iSmpFreq_;
			dtDelta=dtDelta*ilen_;
			iStarLoop=int(fStartValue_/(dtDelta)*ilen_);
			iEndLoop=int(fEndValue_/(dtDelta)*ilen_);
		}
		break;
	case E_CUSTOMPARAM_PERIOD:
		{
			//将得到的周期点数转化为波形点数
			iPeriodNum=iEndIndex_-iStartIndex_;
			iStarLoop=int(fStartValue_*iPeriodNum);
			iEndLoop=int(fEndValue_*iPeriodNum);
		}
		break;
	case E_CUSTOMPARAM_POINT:
		{
			//得到波形点数
			iStarLoop=(int)fStartValue_;
			iEndLoop=(int)fEndValue_;
		}
		break;
	default:
		{
			iStarLoop=(int)fStartValue_;
			iEndLoop=(int)fEndValue_;
		}
	}

	//根据波形点数计算相关参数
	int index=0;
	if (iEndLoop>ilen_ || iStarLoop<0)
	{	
		return;
	}

	int iWaveLen=(iEndLoop-iStarLoop)/iSegNum_;

	if (iWaveLen <= 0)
	{
		ASSERT(FALSE);
		return;
	}

	int lnWavePtMax = this->GetMiddleWareWavePointNumMax();

	//float fBufRef[GD_CUSTOMPARAM_BUFFSIZE] = {.0};

	DATATYPE_WAVE *pfWave =new DATATYPE_WAVE[iWaveLen];
	memset(pfWave,0,iWaveLen*sizeof(DATATYPE_WAVE));
	float *pfWaveData=new float[iWaveLen];
	memset(pfWaveData,0,iWaveLen*sizeof(float));

	for (int i=0;i<iSegNum_;i++)
	{
		for (int nLoop=0;nLoop<iWaveLen;nLoop++)
		{
			pfWave[nLoop]=pfWave_[nLoop+iStarLoop+index*iWaveLen];
		}

		//计算后获得的自定义参数特征值
		ConvertDTWaveToFloat(pfWaveData,pfWave,iWaveLen,sWaveHead_);

		CSigMath calcValue;

		try
		{
#if 0
			fValue=calcValue.CalcPeak_PeakValue(pfWaveData,iWaveLen);
#else
			fValue = myCalcPeakPeakValue(pfWaveData,iWaveLen);
#endif
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		float fSum=.0;

		//[TBD]
		switch(iCharTye_)
		{
			//峰值
		case  E_CUSTOMPARAM_PEAKVALUE:
			fCustomParamValue_[i]=fValue/2;
			break;

			//峰峰值
		case  E_CUSTOMPARAM_PEAKPEAKVALUE:			
			fCustomParamValue_[i]=fValue;
			break;

			//有效值
		case E_CUSTOMPARAM_RMSVALUE:
			for (int nLoop=0;nLoop<iWaveLen;nLoop++)
			{
				fSum+=pow(pfWaveData[nLoop],2);
			}

			fValue=sqrt(fSum/iWaveLen);
			fCustomParamValue_[i]=fValue;
			break;
			//能量值
		case E_CUSTOMPARAM_ENERGYVALUE:
			ASSERT(FALSE);
			break;

			//最大值
		case  E_CUSTOMPARAM_MAXVALUE:
			fValue = pfWaveData[0];

			for (int nLoop=0;nLoop<iWaveLen;nLoop++)
			{
				if (fValue<pfWaveData[nLoop])
				{
					fValue=pfWaveData[nLoop];
				}

			}
			break;

			//真峰值
		case E_CUSTOMPARAM_TRUE_PEAKVALUE:
			ASSERT(FALSE);
			break;

			//真峰峰值
		case E_CUSTOMPARAM_TRUE_PEAKPEAKVALUE:
			ASSERT(FALSE);
			break;

		default:
			ASSERT(FALSE);

		}
		index++;
	}

	//{
	//	CString lsTitle;
	//	lsTitle.Format("Cm_%s", GetRealVibDataDateTime().Format("%Y%m%d%H%M%S"));
	//	writefile(&fBufRef[0], GD_CUSTOMPARAM_BUFFSIZE, lsTitle);
	//}



	//CBHDateTime timeCurr = CBHDateTime::GetCurrentTime();

	//CString lsTitle;

	//lsTitle.Format("W_%s", timeCurr.Format("%Y%m%d%H%M%S"));
	//writefile(fCustomParamValue_, GD_CUSTOMPARAM_BUFFSIZE, lsTitle);

	//lsTitle.Format("C_%s", timeCurr.Format("%Y%m%d%H%M%S"));
	//writefile(fCustomParamValue_, iSegNum_, lsTitle);

	KD_DeletePoint(pfWave);
	KD_DeletePoint(pfWaveData);

}
/**根据频域数据计算自定义相关参数
/*@param pfWave_					[in]	频域频谱数据                                                  
/*@param ilen_						[in]	波形数据长度                                              
/*@param iRev_						[in]	转速                                                 
/*@param iUnitType_					[in]	自定义参数模板的开始值、结束值的单位类型                                     
/*@param iSigAnaType_				[in]	自定义参数模板的信号分析类型                                    
/*@param iSegNum_					[in]	自定义参数模板的段内自定义参数个数，以及对于频谱横轴的分段个数                        
/*@param iSegStartNo_				[in]	自定义参数模板的自定义参数段开始索引   
/*@param iCharTye_					[in]	自定义参数模板的自定义参数特征值类型  
/*@param fStartValue_				[in]	自定义参数模板的自定义参数开始值起始-无量纲  
/*@param fEndValue_					[in]	自定义参数模板的自定义参数开始值结束-无量纲  
/*@param defectFreq_				[in]	自定义参数对应测点的所在轴承缺陷频率  
/*@param gearboxCharFreq_			[in]	自定义参数对应测点的所在齿轮箱缺陷频率  
/*@param fCustomParamValue_		[in/out]	计算后的自定义参数模板的自定义参数值  
/*@param adbldf						[in]	频谱横轴的频率间隔  
*/
void CPlantInfoOnLine::CalcFreqCustomParamValue_New(double *apfFreq_,
													int& ilen_,
													int iRev_,
													int iUnitType_,
													int iSigAnaType_,
													int iSegNum_,
													int iSegStartNo_,
													int iCharTye_,
													float fStartValue_,
													float fEndValue_,
													/*
													S_BearingDefectFreq& defectFreq_,
													S_GearBoxCharFreq& gearboxCharFreq_,*/

													float *apfCustomParamValue_,
													double adbldf,
													BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath)
{
	int iStarLoop=0,iEndLoop=0,iWaveLen=0;
	switch(iUnitType_)
	{
	case E_CUSTOMPARAM_FREQ:
		{
			iStarLoop=(int)ceil(fStartValue_/adbldf);
			iEndLoop=(int)floor(fEndValue_/adbldf);
			TRACE(_T("E_CUSTOMPARAM_FREQ :start %d  end %d | freq start %d end %d\r\n"),(int)fStartValue_,(int)fEndValue_,iStarLoop,iEndLoop);

			//起始索引之间没有值，或者仅有一个值
			if (iStarLoop > iEndLoop)
			{
				if (iEndLoop>=(ilen_/2))
				{
					iEndLoop = (ilen_/2) -1;
				}

				if (iStarLoop>=(ilen_/2))
				{
					iStarLoop = (ilen_/2) -1;
				}

				double ldblStart = fStartValue_;
				double ldblEnd = fEndValue_;
				double ldblSemi = abs(ldblStart-ldblEnd)/(double)2+ldblStart;

				double ldblOemSart = iEndLoop*adbldf;
				double ldblOemEnd  = iStarLoop*adbldf;
				double ldblSemi2 = abs(ldblOemSart-ldblOemEnd)/(double)2+ldblOemSart;

				if (ldblSemi>ldblSemi2)
				{
					iEndLoop = iStarLoop; 
				}else
				{
					iStarLoop = iEndLoop;
				}
			}
		}
		break;
	case E_CUSTOMPARAM_ORDER:
		{
			//阶次转化为频率点数
			iStarLoop=(int)ceil(fStartValue_*((double)iRev_/60.0)/adbldf);
			iEndLoop=(int)floor(fEndValue_*((double)iRev_/60.0)/adbldf);
			TRACE(_T("E_CUSTOMPARAM_ORDER :start %d  end %d | freq start %d end %d\r\n"),(int)fStartValue_,(int)fEndValue_,iStarLoop,iEndLoop);
			
			//起始索引之间没有值，或者仅有一个值
			if (iStarLoop > iEndLoop)
			{
				if (iEndLoop>=(ilen_/2))
				{
					iEndLoop = (ilen_/2) -1;
				}

				if (iStarLoop>=(ilen_/2))
				{
					iStarLoop = (ilen_/2) -1;
				}

				double ldblStart = fStartValue_*((double)iRev_/60.0);
				double ldblEnd = fEndValue_*((double)iRev_/60.0);
				double ldblSemi = abs(ldblStart-ldblEnd)/(double)2+ldblStart;

				double ldblOemSart = iEndLoop*adbldf;
				double ldblOemEnd  = iStarLoop*adbldf;
				double ldblSemi2 = (abs(ldblOemSart-ldblOemEnd)/(double)2+ldblOemSart);

				if (ldblSemi>ldblSemi2)
				{
					iEndLoop = iStarLoop; 
				}else
				{
					iStarLoop = iEndLoop;
				}
			}
		}
		break;
	default:
		{
			ASSERT(FALSE);
			iStarLoop=0;
			iEndLoop=ilen_ -1;
		}
	}

	if (ilen_<=4)
	{
		ASSERT(FALSE);
		return;
	}

	if (iEndLoop>=(ilen_/2))
	{
		iEndLoop = (ilen_/2) -1;
	}

	if (iStarLoop> iEndLoop)
	{
		ASSERT(FALSE);
		return;
	}

	//根据波形点数计算相关参数
	iWaveLen=(iEndLoop-iStarLoop+1)/iSegNum_;

	if (iWaveLen <=0)
	{
		ASSERT(FALSE);
		return ;
	}

	std::vector<double> loDfFreq;
	loDfFreq.resize(iWaveLen);

	double *lpfFreq = &(loDfFreq.front());
	CSigMath loCSigMath;

	for (int i=0;i<iSegNum_;i++)
	{
		int lnDataIndex = iStarLoop+i*iWaveLen;
		if ((lnDataIndex+iWaveLen-1)>iEndLoop)
		{
			ASSERT(FALSE);
			break;
		}
		memcpy(lpfFreq,&(apfFreq_[lnDataIndex]),sizeof(double)*iWaveLen);
		float fValue=.0;

		switch(iCharTye_)
		{
		case  E_CUSTOMPARAM_PEAKVALUE:
			fValue = loCSigMath.FeatureValue(lpfFreq,iWaveLen,E_FeatureValueType_Peak_Direct);
			TRACE(_T("E_CUSTOMPARAM_PEAKVALUE :%f\r\n"),fValue);
			break;	
		case  E_CUSTOMPARAM_PEAKPEAKVALUE:
			fValue = loCSigMath.FeatureValue(lpfFreq,iWaveLen,E_FeatureValueType_PeakPeak_Direct);
			break;	
		case  E_CUSTOMPARAM_RMSVALUE:
			fValue = loCSigMath.FeatureValue(lpfFreq,iWaveLen,E_FeatureValueType_RMS);
			TRACE(_T("E_CUSTOMPARAM_RMSVALUE :%f\r\n"),fValue);
			break;	
		case  E_CUSTOMPARAM_ENERGYVALUE:
			fValue =  this->_CalcEneryValue(lpfFreq,iWaveLen);
			TRACE(_T("E_CUSTOMPARAM_ENERGYVALUE :%f\r\n"),fValue);
			break;	
		case  E_CUSTOMPARAM_MAXVALUE:
			{
				int lnIndex = 0;
				fValue = this->_FindMaxValue(lpfFreq,iWaveLen,lnIndex) ;
				TRACE(_T("E_CUSTOMPARAM_MAXVALUE :%f\r\n"),fValue);
			}
			break;	
		case  E_CUSTOMPARAM_TRUE_PEAKVALUE:
			break;	
		case  E_CUSTOMPARAM_TRUE_PEAKPEAKVALUE:
			break;
		default:
			{
				int lnIndex = 0;
				fValue = this->_FindMaxValue(lpfFreq,iWaveLen,lnIndex) ;
			}
			break;			
		}
		apfCustomParamValue_[i]=fValue;
	}
}

void CPlantInfoOnLine::CalcWaveCustomParamValue_New(	const DATATYPE_WAVE *apfWave_,
													int& ilen_,
													int iSmpFreq_,
													int iStartIndex_,
													int iEndIndex_,
													int iUnitType_,
													int iSigAnaType_,
													int iSegNum_,
													int iSegStartNo_,
													int iCharTye_,
													float fStartValue_,
													float fEndValue_,
													float *fCustomParamValue_,
													BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath,
													std::vector<double> & aRefDoubleWaveBuffer)
{

	int iStarWavePointIndex=0,iEndWavePointIndex=0,iPeriodNum=0;
	float dtDelta=.0,fValue=.0;

	if (NULL == apfWave_)
	{
		return;
	}

	if ((iSmpFreq_ <=0) || (ilen_<=0) || (iSegNum_ <=0))
	{
		ASSERT(FALSE);
		return;
	}

	switch(iUnitType_)
	{
	case E_CUSTOMPARAM_TIME:
		{
			//将得到的时间转化为波形点数 []
			dtDelta=1.0/iSmpFreq_;
			iStarWavePointIndex=int(fStartValue_/(dtDelta)*ilen_);
			iEndWavePointIndex=int(fEndValue_/(dtDelta)*ilen_);
		}
		break;
	case E_CUSTOMPARAM_PERIOD:
		{
			//将得到的周期点数转化为波形点数
			iPeriodNum=iEndIndex_-iStartIndex_;
			iStarWavePointIndex=int(fStartValue_*iPeriodNum);
			iEndWavePointIndex=int(fEndValue_*iPeriodNum);
		}
		break;
	case E_CUSTOMPARAM_POINT:
		{
			//得到波形点数
			iStarWavePointIndex=(int)fStartValue_;
			iEndWavePointIndex=(int)fEndValue_;
		}
		break;
	default:
		{
			iStarWavePointIndex=(int)fStartValue_;
			iEndWavePointIndex=(int)fEndValue_;
		}
	}

	//根据波形点数计算相关参数
	int index=0;
	if (iEndWavePointIndex>ilen_ || iStarWavePointIndex<0 || iEndWavePointIndex <= iStarWavePointIndex)
	{
		//ASSERT(FALSE);
		return;
	}

	int iWaveLen=(iEndWavePointIndex-iStarWavePointIndex)/iSegNum_;

	if (iWaveLen <= 0)
	{
		ASSERT(FALSE);
		return;
	}

	int lnWavePtMax = this->GetMiddleWareWavePointNumMax();

	DATATYPE_WAVE *pfWave =new DATATYPE_WAVE[iWaveLen];
	memset(pfWave,0,iWaveLen*sizeof(DATATYPE_WAVE));
	double *pfWaveData=new double[iWaveLen];
	memset(pfWaveData,0,iWaveLen*sizeof(double));

	for (int i=0;i<iSegNum_;i++)
	{
		for (int nLoop=0;nLoop<iWaveLen;nLoop++)
		{
			pfWaveData[nLoop]=aRefDoubleWaveBuffer[nLoop+iStarWavePointIndex+index*iWaveLen];
		}

		//计算后获得的自定义参数特征值
		CSigMath calcValue;

		try
		{
			float fSum=.0;

			//[TBD]
			switch(iCharTye_)
			{
				//峰值
			case  E_CUSTOMPARAM_PEAKVALUE:
				fCustomParamValue_[i] = calcValue.FeatureValue(pfWaveData,iWaveLen,E_FeatureValueType_Peak_Direct);
				break;

				//峰峰值
			case  E_CUSTOMPARAM_PEAKPEAKVALUE:			
				fCustomParamValue_[i] = calcValue.FeatureValue(pfWaveData,iWaveLen,E_FeatureValueType_PeakPeak_Direct);
				break;

				//有效值
			case E_CUSTOMPARAM_RMSVALUE:
				for (int nLoop=0;nLoop<iWaveLen;nLoop++)
				{
					fSum+=pow((double)pfWaveData[nLoop],2);
				}

				fValue=sqrt(fSum/iWaveLen);
				fCustomParamValue_[i]=fValue;
				break;

				//能量值
			case E_CUSTOMPARAM_ENERGYVALUE:
				fValue = 0;
				for (int nLoop=0;nLoop<iWaveLen;nLoop++)
				{
					fValue+=pow(pfWaveData[nLoop],2);
				}
				break;

				//最大值
			case  E_CUSTOMPARAM_MAXVALUE:
				fValue = 0;
				for (int nLoop=0;nLoop<iWaveLen;nLoop++)
				{
					if (fValue<pfWaveData[nLoop])
					{
						fValue=pfWaveData[nLoop];
					}
				}
				break;

				//真峰值
			case E_CUSTOMPARAM_TRUE_PEAKVALUE:
				fCustomParamValue_[i] = calcValue.FeatureValue(pfWaveData,iWaveLen,E_FeatureValueType_Peak_Statistic);
				break;

				//真峰峰值
			case E_CUSTOMPARAM_TRUE_PEAKPEAKVALUE:
				fCustomParamValue_[i] = calcValue.FeatureValue(pfWaveData,iWaveLen,E_FeatureValueType_PeakPeak_Statistic);
				break;

			default:
				fCustomParamValue_[i] = 0;
				ASSERT(FALSE);

			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Error(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		index++;
	}
	KD_DeletePoint(pfWave);
	KD_DeletePoint(pfWaveData);
}

void CPlantInfoOnLine::CalcFreqCustomParamValue(double *pfFreq_,
												int& ilen_,
												int iRev_,
												int iUnitType_,
												int iSigAnaType_,
												int iSegNum_,
												int iSegStartNo_,
												int iCharTye_,
												float fStartValue_,
												float fEndValue_,
												/*
												S_BearingDefectFreq& defectFreq_,
												S_GearBoxCharFreq& gearboxCharFreq_,*/

												float *fCustomParamValue_)
{
	int iStarLoop=0,iEndLoop=0,iWaveLen=0;
	switch(iUnitType_)
	{
	case E_CUSTOMPARAM_FREQ:
		{
			iStarLoop=(int)fStartValue_;
			iEndLoop=(int)fEndValue_;
		}
		break;
	case E_CUSTOMPARAM_ORDER:
		{
			//阶次转化为频率点数
			iStarLoop=(int)fStartValue_*(iRev_/60);
			iEndLoop=(int)fEndValue_*(iRev_/60);
		}
		break;
	default:
		{
			iStarLoop=(int)fStartValue_;
			iEndLoop=(int)fEndValue_;
		}
	}
	//根据波形点数计算相关参数
	int index=0;
	if (iEndLoop>ilen_ || iStarLoop<0 || ilen_ <=0 || iEndLoop <= iStarLoop)
	{
		ASSERT(FALSE);
		return;
	}

	iWaveLen=(iEndLoop-iStarLoop)/iSegNum_;

	if (iWaveLen <=0)
	{
		ASSERT(FALSE);
		return ;
	}

	double *pfFreq =new double[iWaveLen];

	memset(pfFreq,0,iWaveLen*sizeof(double));

	for (int i=0;i<iSegNum_;i++)
	{
		for (int nLoop=0;nLoop<iWaveLen;nLoop++)
		{
			pfFreq[nLoop]=pfFreq_[nLoop+iStarLoop+index*iWaveLen];
		}
		float fValue=.0;
		//writefile(pfFreq,iWaveLen,115);
		//计算后获得的自定义参数特征值
		switch(_sPlantInfo._iMachineType)
		{
		case GE_MACHINETYPE_COMPR:
			{
				if (iCharTye_==E_CUSTOMPARAM_MAXVALUE)
				{
					//最大值
					for (int nLoop=0;nLoop<iWaveLen;nLoop++)
					{
						if (fValue<pfFreq[nLoop])
						{
							fValue=pfFreq[nLoop];
						}
						else
							fValue=fValue;
					}
				}
				else
				{
					//能量值
					for (int nLoop=0;nLoop<iWaveLen;nLoop++)
					{
						fValue+=pow(pfFreq[nLoop],2);
					}
				}
			}
			break;
		case GE_MACHINETYPE_KEYPUMP:
		case GE_MACHINETYPE_WINDPEQ:
			{
				//关键机泵和机电设备自定义特征趋势
				int iWorkFreq=iRev_/60;
				switch(iSegStartNo_)
				{
				case GE_VF_HALF:
					iWorkFreq=0.5*iWorkFreq;
					break;
				case GE_VF_FIVE:
					iWorkFreq=5*iWorkFreq;
					break;
				case GE_VF_ONEX:
					iWorkFreq=iWorkFreq;
					break;
				case GE_VF_TWO:
					iWorkFreq=2*iWorkFreq;
					break;
				case GE_VF_THREE:
					iWorkFreq=3*iWorkFreq;
					break;
				case GE_VF_FOUR:
					iWorkFreq=4*iWorkFreq;
					break;
				case GE_VF_BLADE:
					iWorkFreq=(_sPlantInfo._iCylinderNum)*iWorkFreq;
					break;
					/*
					case GE_USER_AENVFREQ1:
					iWorkFreq=gearboxCharFreq_.fEnvFreq1;
					break;
					case GE_USER_AENVFREQ2:
					iWorkFreq=gearboxCharFreq_.fEnvFreq2;
					break;
					case GE_ENVSPECCF1:
					iWorkFreq=abs(defectFreq_.fInnerFreq);//防止计算的缺陷频率为负数时，中间件出现异常
					break;
					case GE_ENVSPECCF2:
					iWorkFreq=abs(defectFreq_.fOuterFreq);
					break;
					case GE_ENVSPECCF3:
					iWorkFreq=abs(defectFreq_.fCageFreq);
					break;
					case GE_ENVSPECCF4:
					iWorkFreq=abs(defectFreq_.fRollingFreq);
					break;
					case GE_GEARBOXSHAFTCF1:
					iWorkFreq=gearboxCharFreq_.fShaftCF1;
					break;
					case GE_GEARBOXSHAFTCF2:
					iWorkFreq=gearboxCharFreq_.fShaftCF2;
					break;
					case GE_GEARBOXSHAFTCF3:
					iWorkFreq=gearboxCharFreq_.fShaftCF3;
					break;
					case GE_GEARBOXSHAFTCF4:
					iWorkFreq=gearboxCharFreq_.fShaftCF4;
					break;
					*/
				default:
					iWorkFreq=0.5*iWorkFreq;
				}
				//避免频率大于了频谱总的点数，造成的中间件异常
				if (iWorkFreq>=0 && abs(iWorkFreq)<iWaveLen)
				{
					fValue=pfFreq[iWorkFreq];
					//CString sTemp;sTemp.Format("iWaveLen:%d,iSegStartNo_:%d,iRev_:%d,iWorkFreq:%d,pfFreq[iWorkFreq]:%f",iWaveLen,iSegStartNo_,iRev_,iWorkFreq,pfFreq[iWorkFreq]);BHTRACE_DEBUG(sTemp);
				}
				else
					fValue=.1;
			}
			break;
		default:
			{
				if (iCharTye_==E_CUSTOMPARAM_MAXVALUE)
				{
					//最大值
					for (int nLoop=0;nLoop<iWaveLen;nLoop++)
					{
						if (fValue<pfFreq[nLoop])
						{
							fValue=pfFreq[nLoop];
						}
						else
							fValue=fValue;
					}
				}
				else
				{
					//能量值
					for (int nLoop=0;nLoop<iWaveLen;nLoop++)
					{
						fValue+=pow(pfFreq[nLoop],2);
					}
				}
			}
		}
		fCustomParamValue_[i]=fValue;
		index++;
	}
	KD_DeletePoint(pfFreq);
}

int  CPlantInfoOnLine::CalcBearingDefectFreq(CBearingParamInfo &bearingParamInfo_,int iWorkRev_,S_BearingDefectFreq& defectFreq_)
{
	int iElements_Num=0,iBearingType=0;
	float fRollingDiam=.0,fContactAngle=.0,fPitchDiam=.0;
	fRollingDiam=bearingParamInfo_._fElementDiameter;
	fContactAngle=bearingParamInfo_._fContactAngle;
	fPitchDiam=bearingParamInfo_._fPitchDiameter;
	iElements_Num=bearingParamInfo_._iElementsNum;
	iBearingType=bearingParamInfo_._iBearingType;

	S_BearingPar bearingpar;
	if (iBearingType==GE_ROLLING_BEARING && iElements_Num>0)
	{
		bearingpar.fRollingDiam=fRollingDiam;
		bearingpar.fContackAngle=fContactAngle;
		bearingpar.nNumofRolling=iElements_Num;
		bearingpar.fPitchDiam=fPitchDiam;
		//计算轴承的缺陷特征频率
		if (iWorkRev_>0)
		{
			CSigMath calcDefectFreq;
			try
			{
				calcDefectFreq.CalcBearingDefectFreq(bearingpar,iWorkRev_,defectFreq_);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

		}
	}
	return iElements_Num;
}

int CPlantInfoOnLine::CalcGearboxCharFreq(CGearBoxParamInfo &gearBoxParamInfo_,int iInputRev_,S_GearBoxCharFreq& gearboxCharFreq_)
{
	int iGearLevel=0,iGearType=0,iDriverType=0,iPlanetNum=0,iGearNumSun=0,iGearNumPlanet=0,iGearNumGalaxy=0;
	int iGearNum12=1,iGearNum21=1,iGearNum22=1,iGearNum31=1,iGearNum32=1,iGearNum41=1,iGearNum42=1,iGearNum51=1,iGearNum52=1,iGearNum61=1;
	iGearLevel=gearBoxParamInfo_._iGearBoxLevel;
	iGearType=gearBoxParamInfo_._iGearBoxType;
	iDriverType=gearBoxParamInfo_._iDriverType;
	//行星齿轮参数
	iPlanetNum=gearBoxParamInfo_._iPlanetNum;
	iGearNumSun=gearBoxParamInfo_._iGearNumSun;
	iGearNumPlanet=gearBoxParamInfo_._iGearNumPlanet;
	iGearNumGalaxy=gearBoxParamInfo_._iGearNumGalaxy;
	//各级常规齿轮齿数
	iGearNum12=gearBoxParamInfo_._iGearNumZ12;
	iGearNum21=gearBoxParamInfo_._iGearNumZ22;
	iGearNum22=gearBoxParamInfo_._iGearNumZ31;
	iGearNum32=gearBoxParamInfo_._iGearNumZ32;
	iGearNum41=gearBoxParamInfo_._iGearNumZ41;
	iGearNum42=gearBoxParamInfo_._iGearNumZ42;
	iGearNum51=gearBoxParamInfo_._iGearNumZ51;
	iGearNum52=gearBoxParamInfo_._iGearNumZ52;
	iGearNum61=gearBoxParamInfo_._iGearNumZ61;



	//得到轴号
	//int iShaftNo=0;
	//[TBD]
	//iShaftNo=gearBoxParamInfo_._iGearBoxLevel+1;
	if (iInputRev_>0)
	{
		//计算齿轮箱的特征频率
		float fInputFreq=(float)iInputRev_/60.0;
		if (iGearType==0)
		{
			//常规齿轮
			if(gearBoxParamInfo_._iGearBoxLevel > 0)
			{

				if (0 >= iGearNum12*iGearNum21 )
				{
					ASSERT(FALSE);
					return -1;
				}
				gearboxCharFreq_.fShaftCF1=fInputFreq;
				gearboxCharFreq_.fEnvFreq1=iGearNum12*(gearboxCharFreq_.fShaftCF1);
				gearboxCharFreq_.fShaftCF2 = gearboxCharFreq_.fShaftCF1* iGearNum12/iGearNum21;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF2;
			}
			if(gearBoxParamInfo_._iGearBoxLevel > 1)
			{
				if (0 >= iGearNum22*iGearNum31)
				{
					ASSERT(FALSE);
					return -1;
				}

				gearboxCharFreq_.fShaftCF3 = gearboxCharFreq_.fShaftCF2* iGearNum22/iGearNum31;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF3;
			}
			if(gearBoxParamInfo_._iGearBoxLevel > 2)
			{
				if (0 >= iGearNum32*iGearNum41)
				{
					ASSERT(FALSE);
					return -1;
				}
				gearboxCharFreq_.fShaftCF4 = gearboxCharFreq_.fShaftCF3* iGearNum32/iGearNum41;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF4;
			}
		}
		else
		{   //行星齿轮
			if (0 >= gearBoxParamInfo_._iGearNumSun* gearBoxParamInfo_._iGearNumGalaxy * gearBoxParamInfo_._iGearNumPlanet )
			{
				ASSERT(FALSE);

				return -1;
			}

			gearboxCharFreq_.fShaftCF1= fInputFreq;
			///啮合频率
			gearboxCharFreq_.fEnvFreq1= fInputFreq *(float)(gearBoxParamInfo_._iGearNumSun);
			///输出轴频率
			gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF1;
			if(gearBoxParamInfo_._iGearBoxLevel > 0)
			{
				if (0 >= iGearNum12*iGearNum21)
				{
					ASSERT(FALSE);
					return -1;
				}
				gearboxCharFreq_.fShaftCF2 = gearboxCharFreq_.fShaftCF1 * iGearNum12/iGearNum21;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF2;
			}
			if(gearBoxParamInfo_._iGearBoxLevel > 1)
			{
				if (0 >= iGearNum22*iGearNum31)
				{
					ASSERT(FALSE);
					return -1;
				}

				gearboxCharFreq_.fShaftCF3 = gearboxCharFreq_.fShaftCF2* iGearNum22/iGearNum31;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF3;
			}
			if(gearBoxParamInfo_._iGearBoxLevel > 2)
			{
				if (0 >= iGearNum32*iGearNum41)
				{
					ASSERT(FALSE);
					return -1;
				}

				gearboxCharFreq_.fShaftCF4 = gearboxCharFreq_.fShaftCF3* iGearNum32/iGearNum41;
				gearboxCharFreq_.fEnvFreq2 = gearboxCharFreq_.fShaftCF4;
			}
		}
	}
	return iGearLevel;
}

float CPlantInfoOnLine::MaxRealRev() const
{
	return m_fMaxRealRev;
}

void CPlantInfoOnLine::MaxRealRev( float val )
{
	m_fMaxRealRev = val;
}

void CPlantInfoOnLine::SetDataStatuEx( IDBInterFace * pDB_, 
									 bool bFlag_, 
									 long iEventID_, 
									 E_SUD_TYPE iSudType_ )
{
	SetSudStatus( bFlag_, iSudType_);
	_pLockVibData->BeginWrite();
	_iCurrEventID=iEventID_;
	_pLockVibData->EndWrite();
}

void CPlantInfoOnLine::SetDataTime( CBHDateTime dataTime_, int iMicroSecond_ )
{
	_pLockVibData->BeginWrite();
	_Timesud =dataTime_;
	_iMicroSecond=iMicroSecond_;
	_pLockVibData->EndWrite();
}

int CPlantInfoOnLine::UpdateVibChannStatus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex)
{
	if (NULL == apChannStatus)
	{
		return -1;
	}

	if (((anLen+anStartIndex)> _sPlantInfo._iVibChannNum)||(anLen<1)||(anStartIndex<0))
	{
		return -1;
	}
	
	int lnCount=0;

	int lnChannRealLen = anLen+anStartIndex;

	_pLockVibData->BeginWrite();
	
	try
	{
		for (int lnChanIndex = anStartIndex; lnChanIndex < lnChannRealLen; ++lnChanIndex)
		{
			int lnStatusIndex = lnChanIndex-anStartIndex;

			((CVibChannParam*)_vVibChann[lnChanIndex])->ChannOnline(apChannStatus[lnStatusIndex]._nStatus_Online);

			((CVibChannParam*)_vVibChann[lnChanIndex])->ChannNormal(apChannStatus[lnStatusIndex]._nStatus_Normal);

			((CVibChannParam*)_vVibChann[lnChanIndex])->ChannDataUpdate(apChannStatus[lnStatusIndex]._nStatus_DataUpdate);

			((CVibChannParam*)_vVibChann[lnChanIndex])->BatteryPower(apChannStatus[lnStatusIndex]._nStatus_BatteryPower);
			
			lnCount++;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndWrite();

	return lnCount;
}

int CPlantInfoOnLine::UpdateProcChannStstus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex)
{
	if (NULL == apChannStatus)
	{
		return -1;
	}

	if (((anLen+anStartIndex)> _sPlantInfo._iProcChannNum)||(anLen<1)||(anStartIndex<0))
	{
		return -1;
	}

	int lnCount=0;

	int lnChannRealLen = anLen+anStartIndex;

	_pLockProcData->BeginWrite();

	try
	{
		for (int lnChanIndex = anStartIndex; lnChanIndex < lnChannRealLen; ++lnChanIndex)
		{
			int lnStatusIndex = lnChanIndex-anStartIndex;

			((CProcChannParam*)_vProcChann[lnChanIndex])->ChannOnline(apChannStatus[lnStatusIndex]._nStatus_Online);

			((CProcChannParam*)_vProcChann[lnChanIndex])->ChannNormal(apChannStatus[lnStatusIndex]._nStatus_Normal);

			((CProcChannParam*)_vProcChann[lnChanIndex])->ChannDataUpdate(apChannStatus[lnStatusIndex]._nStatus_DataUpdate);

			((CProcChannParam*)_vProcChann[lnChanIndex])->BatteryPower(apChannStatus[lnStatusIndex]._nStatus_BatteryPower);

			lnCount++;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndWrite();

	return lnCount;
}

/*------------------------------------*/
CRealPlantInfo::CRealPlantInfo()
{
	//	CPlantInfoOnLine::CPlantInfoOnLine();
	CHZLogManage::Info("construction","CRealPlantInfo","CRealPlantInfo");
	_bProcDataUpdateHour = false;
	_bProcDataUpdateDay  = false;
	_bProcDataUpdateMonth= false;

	_bVibDataUpdate      = false;
	_bVibDataUpdateHour  = false;
	_bVibDataUpdateDay   = false;
	_bVibDataUpdateMonth = false;

	_bDynDataUpdate      = false;
	_bDynDataUpdateHour  = false;
	_bDynDataUpdateDay   = false;
	_bDynDataUpdateMonth = false;

	_pLockSaveDataStatus = new CTimeOutReadWriteLock();
	_bSaveDataStatus     = false;
	_bProcDataUpdateSend = false;
	_bVibDataUpdateSend  = false;
	_bDynDataUpdateSend  = false;

	_vVibWaveField.clear();
	_vDynWaveField.clear();

	this->m_nProcJudgedRunStatus = 0;

	this->m_nVibJudgedRunStatus = 0;

	this->_tVibLastSaveTime = COleDateTime(2000,1,1,0,0,0);

	this->_tDynLastSaveTime = COleDateTime(2000,1,1,0,0,0);

	this->_tProcLastSaveTime = COleDateTime(2000,1,1,0,0,0);

	this->_iVibSecTblSaveCount = 0;
	this->_iVibHourTblSaveCount = 0;
	this->_iVibDayTblSaveCount = 0;
	this->_iVibMonthTblSaveCount = 0;

	this->_iDynSecTblSaveCount = 0;
	this->_iDynHourTblSaveCount = 0;
	this->_iDynDayTblSaveCount = 0;
	this->_iDynMonthTblSaveCount = 0;

	this->_iVibSecTblRecordNum = D_TREND_SECONDRANGE;
	this->_iVibHourTblRecordNum = D_TREND_HOURRANGE;
	this->_iVibDayTblRecordNum = D_TREND_DAYRANGE;
	this->_iVibMonthTblRecordNum = D_TREND_DAYRANGE;

	this->_iDynSecTblRecordNum = D_TREND_SECONDRANGE;
	this->_iDynHourTblRecordNum = D_TREND_HOURRANGE;
	this->_iDynDayTblRecordNum = D_TREND_DAYRANGE;
	this->_iDynMonthTblRecordNum = D_TREND_DAYRANGE;
	
}
CRealPlantInfo::~CRealPlantInfo()
{
	Destory();
}

void CRealPlantInfo::Destory()
{
	KD_DeletePoint( _pWave);
	KD_DeletePoint( _pWaveDyn);
	KD_DeletePoint( _pWave_real);
	KD_DeletePoint( _pWaveDyn_real);
	_vVibWaveField.clear();
	_vDynWaveField.clear();
	delete _pLockSaveDataStatus;
}
void CRealPlantInfo::AddChannInfoEnd(IDBInterFace * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的
	//组成数据保存时，插入的SQL语句
	//振动数据
	/*
	int iSmpNum = _sPlantInfo._iSmpNum;
	*/
	int iSmpNum = _sPlantInfo._iSmpNum;

	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave			= new DATATYPE_WAVE[iNum];
	_pWave_real      = new DATATYPE_WAVE[iNum];


	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real = new  DATATYPE_WAVE[iNum];

	CString strInfo;
	strInfo.Format("vibNum: %d,dynNum: %d, SampleNum: %d,%d", _sPlantInfo._iVibChannNum,_sPlantInfo._iDynChannNum, iSmpNum, iNum);
	CHZLogManage::Info(strInfo, "Online", "CRealPlantInfo::AddChannInfoEnd");

	//构造振动通道数据表单名字
	V_PCHANNINFO::iterator itVib;
	_sVibFieldSel= "";
	_sVibFieldIns= "";
	CString strTmp="",strTmp1="";
	CVibChannParam *pVibChann=NULL;
	int index=0;
	_vVibWaveField.clear();
	_vDynWaveField.clear();
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		pVibChann=(CVibChannParam*)(*itVib);
		_sVibFieldIns.append(pVibChann->GetCharValueField());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-3).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-4).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-5).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-6).c_str());
		_sVibFieldIns.append(",");
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-1).c_str());
		strTmp1 = strTmp;
		_sVibFieldSel+=strTmp;
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-7).c_str());
		strTmp1 += strTmp;
		strTmp1 = strTmp1.Left(strTmp1.GetLength()-1);
		_sVibFieldSel+=strTmp;
		pVibChann->SetWaveMemoryPoint(&(_pWave[index*iSmpNum]),iSmpNum);
		pVibChann->SetWaveMemoryPoint_real(&(_pWave_real[index*iSmpNum]),iSmpNum);

		_vVibWaveField.push_back( strTmp1 );
		++index;
	}
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sVibFieldIns += strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sVibFieldSel += strTmp;
	///动态测点，构造动态数据表单
	V_PCHANNINFO::iterator itDyn;
	_sDynFieldIns= "";
	_sDynFieldSel= "";
	CDynChannParam *pDynChann=NULL;
	index = 0;
	for (itDyn = _vDynChann.begin(); itDyn != _vDynChann.end(); ++itDyn)
	{
		pDynChann = (CDynChannParam*)(*itDyn);
		_sDynFieldIns.append(pDynChann->GetCharValueField());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-3).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-4).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-5).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-6).c_str());
		_sDynFieldIns.append(",");
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-1).c_str());
		_sDynFieldSel+=strTmp;
		strTmp1 = strTmp;
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-7).c_str());
		_sDynFieldSel+=strTmp;
		strTmp1 += strTmp;
		strTmp1 = strTmp1.Left(strTmp1.GetLength()-1);
		pDynChann->SetWaveMemoryPoint(&(_pWaveDyn[index*iSmpNum]),iSmpNum);
		pDynChann->SetWaveMemoryPoint_real(&(_pWaveDyn_real[index*iSmpNum]),iSmpNum);
	
		_vDynWaveField.push_back(strTmp1);

		++index;
	}
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sDynFieldSel+=strTmp;


	//过程量数据，构造过程量数据表单
	V_PCHANNINFO::iterator it;
	_sProcField=" ";
	S_ChannInfo channInfo;
	for (it=_vProcChann.begin();it!=_vProcChann.end();++it)
	{
		channInfo=(*it)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);

	TrendInitID(pDb_);
}



void CRealPlantInfo::AddChannInfoEnd(CCustomDB * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的
	//组成数据保存时，插入的SQL语句
	//振动数据
	/*
	int iSmpNum = _sPlantInfo._iSmpNum;
	*/
	int iSmpNum = _sPlantInfo._iSmpNum;

	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave			= new DATATYPE_WAVE[iNum];
	_pWave_real      = new DATATYPE_WAVE[iNum];


	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real = new  DATATYPE_WAVE[iNum];

	CString strInfo;
	strInfo.Format("vibNum: %d,dynNum: %d, SampleNum: %d,%d", _sPlantInfo._iVibChannNum,_sPlantInfo._iDynChannNum, iSmpNum, iNum);
	CHZLogManage::Info(strInfo, "Online", "CRealPlantInfo::AddChannInfoEnd");

	//构造振动通道数据表单名字
	V_PCHANNINFO::iterator itVib;
	_sVibFieldSel= "";
	_sVibFieldIns= "";
	CString strTmp="",strTmp1="";
	CVibChannParam *pVibChann=NULL;
	int index=0;
	_vVibWaveField.clear();
	_vDynWaveField.clear();
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		pVibChann=(CVibChannParam*)(*itVib);
		_sVibFieldIns.append(pVibChann->GetCharValueField());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-3).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-4).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-5).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-6).c_str());
		_sVibFieldIns.append(",");
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-1).c_str());
		_sVibFieldSel+=strTmp;
		strTmp1 = strTmp;
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-7).c_str());
		_sVibFieldSel+=strTmp;
		strTmp1+=strTmp;
		strTmp1 = strTmp1.Left(strTmp1.GetLength()-1);
		pVibChann->SetWaveMemoryPoint(&(_pWave[index*iSmpNum]),iSmpNum);
		pVibChann->SetWaveMemoryPoint_real(&(_pWave_real[index*iSmpNum]),iSmpNum);
		_vVibWaveField.push_back(strTmp1);
		++index;
	}
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sVibFieldIns += strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sVibFieldSel += strTmp;
	///动态测点，构造动态数据表单
	V_PCHANNINFO::iterator itDyn;
	_sDynFieldIns= "";
	_sDynFieldSel= "";
	CDynChannParam *pDynChann=NULL;
	index = 0;
	for (itDyn = _vDynChann.begin(); itDyn != _vDynChann.end(); ++itDyn)
	{
		pDynChann = (CDynChannParam*)(*itDyn);
		_sDynFieldIns.append(pDynChann->GetCharValueField());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-3).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-4).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-5).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-6).c_str());
		_sDynFieldIns.append(",");
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-1).c_str());
		_sDynFieldSel+=strTmp;
		strTmp1 = strTmp;
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-7).c_str());
		_sDynFieldSel+=strTmp;
		strTmp1+=strTmp;
		strTmp1 = strTmp1.Left(strTmp1.GetLength()-1);

		pDynChann->SetWaveMemoryPoint(&(_pWaveDyn[index*iSmpNum]),iSmpNum);
		pDynChann->SetWaveMemoryPoint_real(&(_pWaveDyn_real[index*iSmpNum]),iSmpNum);
		
		_vDynWaveField.push_back(strTmp1);

		++index;
	}
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sDynFieldSel+=strTmp;


	//过程量数据，构造过程量数据表单
	V_PCHANNINFO::iterator it;
	_sProcField=" ";
	S_ChannInfo channInfo;
	for (it=_vProcChann.begin();it!=_vProcChann.end();++it)
	{
		channInfo=(*it)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);

	TrendInitID(pDb_);
}


int CRealPlantInfo::GetSpecChannProcData(int index_,float &fData_,string &sUnit_,int iChannType_)
{
	if ((index_>-1)&&(index_<_sPlantInfo._iProcChannNum))
	{
		S_ChannInfo channInfo=_vProcChann[index_]->GetChannInfo();
		if (iChannType_!=GE_ALLPROC)
		{
			if (channInfo._iChannType!=iChannType_)
				return -1;
		}
		_pLockProcData->BeginRead();
		fData_=((CProcChannParam*)_vProcChann[index_])->GetData();
		_pLockProcData->EndRead();
		sUnit_=channInfo._cUnit;
		return index_;
	}
	return -1;
}

int CRealPlantInfo::GetSpecChannProcData(string sChann_,float &fData_,string &sUnit_,int iChannType_)
{
	return GetSpecChannProcData(GetSpecProcChannIndex(sChann_),fData_,sUnit_);
}

int CRealPlantInfo::GetSpecChannAxisData(string sChann_,float &fData_,string &sUnit_,int iCharType_)
{
	int index=-1;
	V_PCHANNINFO::iterator it=_vVibChann.begin();
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginRead();
			fData_=((CVibChannParam*)(*it))->GetCharValueEx().GetSpecTypeData(iCharType_);
			_pLockVibData->EndRead();
			sUnit_=channInfo._cUnit;
			return index;
		}
	}
	return -1;
}

int CRealPlantInfo::GetSpecChannAxisData(int index_,float &fData_,string &sUnit_,int iCharType_)
{
	if((index_>-1)&&(index_<_iAxialVibChannNum))
	{
		_pLockVibData->BeginRead();
		fData_=((CVibChannParam*)_vVibChann[_iRadialVibChannNum+index_])->GetCharValueEx().GetSpecTypeData(iCharType_);
		_pLockVibData->EndRead();
		sUnit_=_vVibChann[_iRadialVibChannNum+index_]->GetChannInfo()._cUnit;
	}
	else
		index_=-1;
	return index_;
}

string CRealPlantInfo::GetSpecIndexAxisVibChannID(int index_)
{
	string sID="";
	if((index_>-1)&&(index_<_iAxialVibChannNum))
		sID=_vVibChann[_iRadialVibChannNum+index_]->GetChannInfo()._cChannID;
	return sID;
}

string CRealPlantInfo::GetSpecIndexRadialVibChannID(int index_)
{
	string sID="";
	//	if((index_>-1)&&(index_<_iRadialVibChannNum))//2007.4.10修改，by Marble
	if((index_>-1)&&(index_<_sPlantInfo._iVibChannNum))
		sID=_vVibChann[index_]->GetChannInfo()._cChannID;
	return sID;
}

string CRealPlantInfo::GetSpecIndexProcChannID(int index_)
{
	string sID="";
	if ((index_>-1)&&(index_<_sPlantInfo._iProcChannNum))
		sID=_vProcChann[index_]->GetChannInfo()._cChannID;
	return sID;
}
int CRealPlantInfo::GetSpecChannRadialData(string sChann_,float &fData_,string &sUnit_,int iCharType_)
{
	int index=-1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginWrite();
			((CVibChannParam*)(*it))->ComputeEigenValue(GetPlantInfo()._iMachineType,index);
			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			fData_=((CVibChannParam*)(*it))->GetCharValueEx().GetSpecTypeData(iCharType_);
			_pLockVibData->EndRead();
			sUnit_=channInfo._cUnit;
			return index;
		}
	}
	return -1;
}

int CRealPlantInfo::GetSpecRadialVibChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	int index=-1;
	if (iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginRead();
			memcpy(pfCustom_,&(((CVibChannParam*)(*it))->GetCustomParamData()[iStartIndex_]),sizeof(float)*iLen_);
			_pLockVibData->EndRead();
			return index;
		}
	}
	return -1;
}

int CRealPlantInfo::GetSpecRadialVibChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	if (iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;

	CVibChannParam * lpChannParam = (CVibChannParam *)(_vVibChann[index_]);
	_pLockVibData->BeginRead();
	memcpy(pfCustom_,&(lpChannParam->GetCustomParamData()[iStartIndex_]),sizeof(float)*iLen_);
	_pLockVibData->EndRead();
	return 1;
}

int CRealPlantInfo::GetSpecChannRadialAllChar(string sChann_,S_VibCharValue &sVibData_,string &sUnit_)
{
	int index=-1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			CVibChannParam * lpVibChannParam = (CVibChannParam*)(*it);	

			_pLockVibData->BeginWrite();
			lpVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,index);
			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();

			sVibData_=((CVibChannParam*)(*it))->GetCharValueEx();

			_pLockVibData->EndRead();

			sUnit_=channInfo._cUnit;

			return index;
		}
	}
	return -1;
}

int CRealPlantInfo::GetSpecChannRadialData(int index_,float &fData_,string &sUnit_,int iCharType_)
{
	//	if((index_>-1)&&(index_<_iRadialVibChannNum))//2007.4.10修改，by Marble
	if((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
	{
		_pLockVibData->BeginWrite();
		((CVibChannParam*)_vVibChann[index_])->ComputeEigenValue(GetPlantInfo()._iMachineType,index_);
		_pLockVibData->EndWrite();

		_pLockVibData->BeginRead();
		fData_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx().GetSpecTypeData(iCharType_);
		_pLockVibData->EndRead();
		sUnit_=_vVibChann[index_]->GetChannInfo()._cUnit;
	}
	else
		index_=-1;
	return index_;
}

int CRealPlantInfo::GetSpecChannRadialAllChar(int index_,S_VibCharValue &sVibData_,string &sUnit_)
{
	//	if((index_>-1)&&(index_<_iRadialVibChannNum))//2007.4.10修改，by Marble
	if((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
	{
		_pLockVibData->BeginWrite();
		((CVibChannParam*)_vVibChann[index_])->ComputeEigenValue(GetPlantInfo()._iMachineType,index_);
		_pLockVibData->EndWrite();

		_pLockVibData->BeginRead();		
		sVibData_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx();
		_pLockVibData->EndRead();
		sUnit_=_vVibChann[index_]->GetChannInfo()._cUnit;
	}
	else
		index_=-1;
	return index_;
}

int CRealPlantInfo::GetSpecChannRadialData(string sChann_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_/* =GE_ALLCHAR */)
{
	return GetSpecChannRadialData(GetSpecVibChannIndex(sChann_),fAmp_,fPhase_,fRev_,sUnit_,iCharType_);
}

int CRealPlantInfo::GetSpecChannRadialData(int index_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_/* =GE_ALLCHAR */)
{
	//
	//	if((index_<0)||(index_>(_iRadialVibChannNum-1)))//临时改动，为了让客户端可以看到轴位移通道波形2007.4.10
	if((index_<0)||(index_>( _sPlantInfo._iVibChannNum-1)))
		return -1;
	int iCharType=0;
	int iPhaseType=0;
	switch(iCharType_)
	{
	case GE_ALLCHAR://因为目前没有计算通频相位，所以用一倍频替代
		iCharType=GE_ALLCHAR;
		iPhaseType=GE_PHASECHARONE;
		break;
	case GE_ONECHAR:
	case GE_PHASECHARONE:
		iCharType=GE_ONECHAR;
		iPhaseType=GE_PHASECHARONE;
		break;
	case GE_TWOCHAR:
	case GE_PHASECHARTWO:
		iCharType=GE_TWOCHAR;
		iPhaseType=GE_PHASECHARTWO;
		break;
	case GE_THREECHAR:
	case GE_PHASECHARTHREE:
		iCharType=GE_THREECHAR;
		iPhaseType=GE_PHASECHARTHREE;
		break;
	case GE_HALFCHAR:
	case GE_PHASECHARHALF:
		iCharType=GE_HALFCHAR;
		iPhaseType=GE_PHASECHARHALF;
		break;
	default://缺省情况下，就是用一倍频的幅值和相位
		iCharType=GE_ONECHAR;
		iPhaseType=GE_PHASECHARONE;
		break;
	}
	S_ChannInfo channInfo=_vVibChann[index_]->GetChannInfo();
	sUnit_=channInfo._cUnit;

	fRev_ = GetSpecTypeRealRev(channInfo._iRevType);
	_pLockVibData->BeginRead();
	fAmp_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx().GetSpecTypeData(iCharType);
	fPhase_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx().GetSpecTypeData(iPhaseType);
	_pLockVibData->EndRead();

	return index_;
}

float CRealPlantInfo::GetSpecChannRev(string strChann_,float &fRealRev_)
{
	float fRatingRev=.0;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for(it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		channInfo=(*it)->GetChannInfo();
		if(strcmp(strChann_.c_str(),channInfo._cChannID)==0)
		{
			fRealRev_ = GetSpecTypeRealRev( channInfo._iRevType, true);
			int iRevType=channInfo._iRevType;
			if ((iRevType< 1) || ( iRevType > 5))
				iRevType=1;
			iRevType--;
			fRatingRev=_sPlantInfo._piRev[iRevType];
			return fRatingRev;
		}
	}
	for (it= _vDynChann.begin(); it!= _vDynChann.end(); ++it)
	{
		channInfo=(*it)->GetChannInfo();
		if(strcmp(strChann_.c_str(),channInfo._cChannID)==0)
		{
			fRealRev_ = GetSpecTypeRealRev( channInfo._iRevType, true);
			int iRevType=channInfo._iRevType;
			if ((iRevType< 1) || ( iRevType > 5))
				iRevType=1;
			iRevType--;
			fRatingRev=_sPlantInfo._piRev[iRevType];
			return fRatingRev;
		}
	}
	for (it= _vProcChann.begin(); it!= _vProcChann.end(); ++it)
	{
		channInfo=(*it)->GetChannInfo();
		if(strcmp(strChann_.c_str(),channInfo._cChannID)==0)
		{
			int iRevType=channInfo._iRevType;
			if ((iRevType< 1) || ( iRevType > 5))
				iRevType=1;
			iRevType--;
			_pLockVibData->BeginRead();
			fRealRev_ = _pfRealRev[iRevType];
			_pLockVibData->EndRead();
			fRatingRev=_sPlantInfo._piRev[iRevType];
			return fRatingRev;
		}
	}
	CString sTemp;sTemp.Format("fRealRev_:%f",fRealRev_);
	return -1;
}

int CRealPlantInfo::GetSpecChannAndCharTypeVibData(int index_,float &fData_,string &sUnit_,int iCharType_)
{
	if((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
		return -1;
	_pLockVibData->BeginRead();
	fData_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx().GetSpecTypeData(iCharType_);
	_pLockVibData->EndRead();
	sUnit_=_vVibChann[index_]->GetChannInfo()._cUnit;
	return index_;
}

int CRealPlantInfo::GetSpecChannAndCharTypeVibData(string sChann_,float &fData_,string &sUnit_,int iCharType_)
{
	int index=-1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginRead();
			fData_=((CVibChannParam*)(*it))->GetCharValueEx().GetSpecTypeData(iCharType_);
			_pLockVibData->EndRead();
			sUnit_=channInfo._cUnit;
			return index;
		}
	}
	return -1;
}

/*
* param[in] sChann_   测点名称
* param[out] pfData_  波形数据Buffer
* param[in/out] ilen_ 请求点数/波形大小
* - 根据测点名得到测点索引
* - 通过测点索引对应的测点信息获取实际采样点数
* - 按请求点数和实际点数中较小的取波形
* - 请求点数置为取得波形点数
*/
int CRealPlantInfo::GetSpecChannVibWave(string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_)
{
	int iRealNum = 1, iSmpFreq = 1;
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1)
	{
		_HZLOG_ERROR(STR("测点%s索引为-1", sChann_.c_str()));
		ilen_ = 0;
		return -1;
	}

	CWaveHead lsWaveHead;

	iRealNum=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
	//CString sTemp;sTemp.Format("ilen_:%d,iRealNum:%d",ilen_,iRealNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CRealPlantInfo","GetSpecChannVibWave");

	// min 取实际点数和要求点数较小的
	iRealNum=(ilen_> iRealNum) ? iRealNum:ilen_;

	if(iRealNum < 1){
		HZLOG_DEBUG(STR("CalcNum=%d, RequestNum=%d", iRealNum, ilen_));
		ilen_ = 0;
		return -1;
	}

	_pLockVibData->BeginRead();
	lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();
	ConvertDTWaveToFloat(pfWave_,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),iRealNum,lsWaveHead);
	
	_pLockVibData->EndRead();

	ConvertFloatToDTWave(pfData_,pfWave_,ilen_,lsWaveHead);
	GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,fMaxValue_,fMinValue_);


	ilen_=iRealNum;
	return index;
}

int CRealPlantInfo::GetSpecChannVibWave( string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_ )
{
	int iRealNum = 1, iSmpFreq = 1;
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1)
	{
		_HZLOG_ERROR(STR("测点%s索引为-1", sChann_.c_str()));
		ilen_ = 0;
		return -1;
	}

	CWaveHead lsWaveHead;

	iRealNum=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
	//CString sTemp;sTemp.Format("ilen_:%d,iRealNum:%d",ilen_,iRealNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CRealPlantInfo","GetSpecChannVibWave");

	// min 取实际点数和要求点数较小的
	iRealNum=(ilen_> iRealNum) ? iRealNum:ilen_;

	if(iRealNum < 1){
		HZLOG_DEBUG(STR("CalcNum=%d, RequestNum=%d", iRealNum, ilen_));
		ilen_ = 0;
		return -1;
	}

	_pLockVibData->BeginRead();
	lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();
	
	memcpy(pfRawData_,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),ilen_*sizeof(DATATYPE_WAVE));
	
	_pLockVibData->EndRead();

	GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,fMaxValue_,fMinValue_);

	if(nPrecisionEnabled_  == 1)
	{
		for(int loop = 0; loop < ilen_; ++loop)
		{
			pfData_[loop] = (((pfRawData_[loop] -SHRT_MIN)*1.0)/((SHRT_MAX-SHRT_MIN)*1.0)*(fMaxValue_-fMinValue_)+fMinValue_)*fScaleCoef_;
		}
	}


	ilen_=iRealNum;
	return index;
}

int CRealPlantInfo::GetSpecDynChannWave( const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_)
{
	int iRtn=-1;
	CWaveHead lsWaveHead;
	iRtn=GetSpecDynChannIndex(sChannID_);
	if(iRtn>-1)
	{
		int iSmpFreq = 1;
		int iSmpNums = 1;
		CDynChannParam *pDynChann=(CDynChannParam*)(_vDynChann[iRtn]);
		iSmpNums=pDynChann->GetCharValue()._iSmpNum;
		//CString strInfo;strInfo.Format("%d,%d,%d",iRtn,iSmpNums,ilen_);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannWave");
		ilen_=ilen_ > iSmpNums ? iSmpNums :ilen_;
		_pLockDynData->BeginRead();
		lsWaveHead = pDynChann->WaveHead();
		ConvertDTWaveToFloat(pfWave_,pDynChann->GetCurrRealWavePoint(),ilen_,lsWaveHead);
		
		_pLockDynData->EndRead();
		ConvertFloatToDTWave(pfData_,pfWave_,ilen_,lsWaveHead);

		GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,fMaxValue_,fMinValue_);

	}
	return iRtn;
}

int CRealPlantInfo::GetSpecDynChannWave( const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_ )
{
	int iRtn=-1;
	CWaveHead lsWaveHead;
	iRtn=GetSpecDynChannIndex(sChannID_);
	if(iRtn>-1)
	{
		int iSmpFreq = 1;
		int iSmpNums = 1;
		CDynChannParam *pDynChann=(CDynChannParam*)(_vDynChann[iRtn]);
		iSmpNums=pDynChann->GetCharValue()._iSmpNum;
		//CString strInfo;strInfo.Format("%d,%d,%d",iRtn,iSmpNums,ilen_);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannWave");
		ilen_=ilen_ > iSmpNums ? iSmpNums :ilen_;
		_pLockDynData->BeginRead();
		lsWaveHead = pDynChann->WaveHead();
		
		memcpy(pfRawData_,pDynChann->GetCurrWavePoint(),ilen_*sizeof(DATATYPE_WAVE));
		
		_pLockDynData->EndRead();

		GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,fMaxValue_,fMinValue_);

		if(nPrecisionEnabled_  == 1)
		{
			for(int loop = 0; loop < ilen_; ++loop)
			{
				pfData_[loop] = (((pfRawData_[loop] -SHRT_MIN)*1.0)/((SHRT_MAX-SHRT_MIN)*1.0)*(fMaxValue_-fMinValue_)+fMinValue_)*fScaleCoef_;
			}
		}

	}
	return iRtn;
}
int CRealPlantInfo::GetSpecDynChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	int index=-1;
	if ( iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		++index;
		if (strcmp((*it)->GetChannInfo()._cChannID,sChann_.c_str())==0)
		{
			_pLockDynData->BeginRead();
			memcpy(pfCustom_,&(((CDynChannParam*)(*it))->GetCustomParamData()[iStartIndex_]), sizeof(float)*iLen_);
			_pLockDynData->EndRead();
			return index;
		}
	}
	return -1;
}

int CRealPlantInfo::GetSpecDynChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	if ( index_< 0 || (!(index_< _vDynChann.size())) ||  iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;

	CDynChannParam * lpChannParam = (CDynChannParam *)(_vDynChann[index_]);
	_pLockDynData->BeginRead();
	memcpy(pfCustom_,&(lpChannParam->GetCustomParamData()[iStartIndex_]), sizeof(float)*iLen_);
	_pLockDynData->EndRead();
	return 1;
}

int CRealPlantInfo::GetSpecChannVibWaveAndFreq(string sChann_,
											   DATATYPE_WAVE *pfData_,
											   int &iPtNum_,
											   DATATYPE_WAVE *pfFreqData_,
											   int &iLines_,
											   float &fDf_,
											   float& fScaleCoef_,
											   int& nPrecisionEnabled_,
											   float *pfWave_,
											   float * pfFreq_,
											   BOOL abSkipFreq)
{
	_DECLARE_PERF_MEASURE_TIME();
	int iSensorType=0;
	//CString strInfo;strInfo.Format("侧点名%s",sChann_.c_str());
	//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecChannVibWaveAndFreq");
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1)
		return -1;
	//strInfo.Format("输入点数%d",iPtNum_);
	//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecChannVibWaveAndFreq");
	iSensorType=_vVibChann[index]->GetChannInfo()._iSensorType;
	float *pfWave=NULL;
	float *pfSpec=NULL;
	//得到客户指定分析线数需要波形数据个数
	int iLinesPt=iLines_*2.56;

	{
		_DECLARE_PERF_MEASURE_TIME();
		_BEGIN_PERF_MEASURE_TIME();
		int iSmpNums = 1;
		int iSmpFreq = 1;
		int iRtn = -1;
		iSmpNums=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
		iSmpFreq=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpFreq;
		//iPtNum_=iPtNum_>iSmpNums ? iSmpNums:iPtNum_;
		iPtNum_ = iSmpNums;

		if (iLines_ == 0)
		{
			iLines_ =(int)iSmpNums/2.56;
		}
		else
		{
			iLines_=iLines_<2?16:iLines_;
		}

		if (iLines_ < 199)
		{
			iLines_ = 200;
		}
		
		float fStartFreq=0;
		CWaveHead lsWaveHead;
		float lfMaxValue = 0;
		float lfMinValue = 0;

		pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfSpec=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];//加10是为了分配的内存不够
		//得到客户需要的波形数据和谱分析需要的波形数据
		_pLockVibData->BeginRead();
		lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();

		ConvertDTWaveToFloat(pfWave,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),iSmpNums,lsWaveHead);
		
		_pLockVibData->EndRead();

		GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,lfMaxValue,lfMinValue);

		memcpy(pfWave_,pfWave,iPtNum_*sizeof(float));

		ConvertFloatToDTWave(pfData_,pfWave,iSmpNums,lsWaveHead);

		_END_PERF_MEASURE_TIME("ConvertDTWaveToFloat");
		_BEGIN_PERF_MEASURE_TIME();

		if(!abSkipFreq)
		{
			iRtn = ZoomSpectrumAnalysis(pfWave,iSmpNums,iSmpFreq,pfSpec,iLines_,fDf_,fStartFreq);
		}
		
		_END_PERF_MEASURE_TIME("ZoomSpectrumAnalysis");
		_BEGIN_PERF_MEASURE_TIME();
		if (iRtn<0)
			iLines_=0;
		else
		{
			if(iRtn==2)
			{//正常峰值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2);
					ScaleWave(pfFreq_,pfSpec,iLines_,2);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1);
					ScaleWave(pfFreq_,pfSpec,iLines_,1);
				}
			}
			else
			{//细化谱,细化谱为有效值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2.828);
					ScaleWave(pfFreq_,pfSpec,iLines_,2.828);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1.414);
					ScaleWave(pfFreq_,pfSpec,iLines_,1.414);
				}
			}
		}
		_END_PERF_MEASURE_TIME("ConvertFloatToDTWave");
	}
	if(NULL!=pfWave) delete [] pfWave;
	if(NULL!=pfSpec) delete [] pfSpec;
	return index;
}

int CRealPlantInfo::GetSpecChannVibWaveAndCepstrum(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,double *pfFreqData_,int &iCepsNums_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_)
{
	int iSensorType=0;
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1)
		return -1;
	float *pfWave=NULL;
	//得到客户指定分析线数需要波形数据个数
	{
		int iSmpNums = 1;
		int iSmpFreq = 1;
		int iRtn;
		iSmpNums=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
		iSmpFreq=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpFreq;
		iPtNum_=iPtNum_>iSmpNums ? iSmpNums:iPtNum_;
		fDf_ = (float)1.0/iSmpFreq;
		fDf_=1000.0*fDf_;//转化为倒谱的横坐标
		CWaveHead lsWaveHead;
		//得到客户需要的波形数据和谱分析需要的波形数据
		_pLockVibData->BeginRead();
		lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();
		ConvertDTWaveToFloat(pfWave_,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),iSmpNums,lsWaveHead);
		_pLockVibData->EndRead();

		ConvertFloatToDTWave(pfData_,pfWave_,iSmpNums,lsWaveHead);


		iRtn = CepstrumAnalysis(pfData_,iSmpNums,pfFreqData_,iCepsNums_,lsWaveHead);
		//writefile(pfData_,iSmpNums,105);
		//writefile(pfFreqData_,iCepsNums_,106);
		KD_DeletePoint(pfWave);
		if (iRtn<0)
			return -1;
	}
	return index;
}

int CRealPlantInfo::GetSpecChannVibFreq(string sChann_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float * pfFreq_,float& fScaleCoef_,int& nPrecisionEnabled_)
{
	int iSensorType=0;
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1){
		return -1;
	}
	iSensorType=_vVibChann[index]->GetChannInfo()._iSensorType;
	float *pfWave=NULL;
	float *pfSpec=NULL;
	/*2007-4-10注释掉，为了添加细化分析
	//得到客户指定分析线数需要波形数据个数
	int iLinesPt=iLines_*2.56;
	int iSmpFreq;
	_pLockVibData->BeginRead();
	//判断实际数据是否满足谱分析的波形数据个数
	iLinesPt=iLinesPt>_iRealSmpNum?_iRealSmpNum:iLinesPt;
	iSmpFreq=_iRealSmpFreq;
	_pLockVibData->EndRead();
	pfWave=new float[iLinesPt];
	pfSpec=new float[iLinesPt];
	//得到谱分析需要的波形数据
	_pLockVibData->BeginRead();
	ConvertDTWaveToFloat(pfWave,_vVibChann[index].GetCurrWavePoint(),iLinesPt);
	_pLockVibData->EndRead();
	//	CString sTmp;sTmp.Format("%d,%d,%d",_iRealPointNum,ag_iPtNum,iLinesPt);AfxMessageBox(sTmp);
	iLines_=iLinesPt/2.56;
	if(iLines_>1)
	{
	fDf_=(float)iSmpFreq/iLinesPt;
	iLines_+=1;
	if(iSensorType==GE_SENSORTYPE_VOTEX){//峰峰值谱
	SpectralMeasureMent(pfWave,iLinesPt,pfSpec,EA_PEAK_PEAK_SPEC);
	}
	else{//峰值谱
	SpectralMeasureMent(pfWave,iLinesPt,pfSpec,EA_PEAK_SPEC);
	}
	ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_);
	}
	*/
	{
		int iSmpNums =0;
		int iSmpFreq =0;
		int iRtn;
		float fStartFreq=0;
		CWaveHead lsWaveHead;
		float lfMaxValue = 0;
		float lfMinValue = 0;
		iLines_=iLines_<2?16:iLines_;
		iSmpNums=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
		iSmpFreq=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpFreq;
		pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfSpec=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];//加10是为了分配的内存不够
		//得到谱分析需要的波形数据
		_pLockVibData->BeginRead();
		lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();
		ConvertDTWaveToFloat(pfWave,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),iSmpNums,lsWaveHead);
		_pLockVibData->EndRead();

		GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,lfMaxValue,lfMinValue);

		iRtn = ZoomSpectrumAnalysis(pfWave,iSmpNums,iSmpFreq,pfSpec,iLines_,fDf_,fStartFreq);
		if (iRtn<0)
			iLines_=0;
		else
		{
			if(iRtn==2)
			{//正常峰值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)
				{//峰峰值谱
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2);
					ScaleWave(pfFreq_,pfSpec,iLines_,2);
				}
				else
				{//峰值谱
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1);
					ScaleWave(pfFreq_,pfSpec,iLines_,1);
				}
			}
			else
			{//细化谱,细化谱为有效值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)
				{//峰峰值谱
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2.828);
					ScaleWave(pfFreq_,pfSpec,iLines_,2.828);
				}
				else
				{//峰值谱
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1.414);
					ScaleWave(pfFreq_,pfSpec,iLines_,1.414);
				}
			}
		}
	}
	if(NULL!=pfWave) delete [] pfWave;
	if(NULL!=pfSpec) delete [] pfSpec;
	return index;
}


int CRealPlantInfo::GetSpecChannZoomSpec(string sChann_,DATATYPE_WAVE *pfZoomSpec_,long &iLines_,float &fStartFreq_,int iStopFreq_,float &fDf_,DATATYPE_WAVE *pfWave_,int &iPtNum_,float &fScaleCoef_,int& nPrecisionEnabled_,float *pfWaveData_,float *pfFreqData_)
{
	int nErrorCode=0;
	int iSmpFreq=0;
	int iSmpNum=0;
	float fRealDf=.0;

	CWaveHead lsWaveHead;

	float lfMaxValue = 0;
	float lfMinValue = 0;

	int index=GetSpecVibChannIndex(sChann_);
	if (index<0)
	{
		return -1;
	}
	iSmpNum=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
	iSmpFreq=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpFreq;
	fRealDf=(float)iSmpFreq/iSmpNum;
	float *pWave=new float[iSmpNum];
	float *pfZoomSpec=new float[iSmpNum];
	if(!((fStartFreq_>0)&&(iStopFreq_>fStartFreq_)&&((iSmpFreq/2)>iStopFreq_)&&(iLines_>0)))
	{//起始频率必须小于终止频率，采样频率的一半不能大于终止频率
		nErrorCode=-3;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}
	fDf_=(iStopFreq_-fStartFreq_)/iLines_;
	//要求最小要细化2倍
	if(!((2*fDf_)<fRealDf))
	{
		nErrorCode=-1;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}
	//细化分析不能超过20倍
	if((20*fDf_)<fRealDf)
	{
		nErrorCode=-2;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}
	int iSensorType=0;

	if (index==-1){
		nErrorCode=-100;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}

	iSensorType=_vVibChann[index]->GetChannInfo()._iSensorType;
	iPtNum_=iPtNum_>iSmpNum?iSmpNum:iPtNum_;

	
	_pLockVibData->BeginRead();
	lsWaveHead = ((CVibChannParam*)_vVibChann[index])->WaveHead();
	ConvertDTWaveToFloat(pfWaveData_,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),iSmpNum,lsWaveHead);
	_pLockVibData->EndRead();

	ConvertFloatToDTWave(pfWave_,pfWaveData_,iSmpNum,lsWaveHead);

	GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,lfMaxValue,lfMinValue);

	long iRealLines=iLines_;
	float iRealStartFreq=0;

	//ZoomFFT(float data[], long data_len, short startFreq, short stopFreq, short nLines, short SamplingRate, float zoomSpectrum[], long *len, float *f0, float *df);
	try
	{
#ifndef __NO_ZOOM_FFT
		nErrorCode=ZoomFFT(pWave,iSmpNum,fStartFreq_,iStopFreq_,iLines_,iSmpFreq,pfZoomSpec,&iRealLines,&iRealStartFreq,&fDf_);
		if (iRealLines >3)
		{
			pfZoomSpec[iRealLines-1] = 0;
			pfZoomSpec[iRealLines-2] = 0;
		}
#endif
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		nErrorCode = -1;
	}

	if (GE_SENSORTYPE_VOTEX==_vVibChann[index]->GetChannInfo()._iSensorType)
	{
		ConvertFloatToDTWave(pfZoomSpec_,pfZoomSpec,iRealLines,lsWaveHead,2.828);
		ScaleWave(pfFreqData_,pfZoomSpec,iRealLines,2.828);
	}
	else
	{
		ConvertFloatToDTWave(pfZoomSpec_,pfZoomSpec,iRealLines,lsWaveHead,1.414);
		ScaleWave(pfFreqData_,pfZoomSpec,iRealLines,1.414);
	}

	/*	for (int nloop=0;nloop<iRealLines;++nloop)
	{
	pfZoomSpec_[nloop]=2*pfZoomSpec_[nloop];
	}*/
	fStartFreq_=iRealStartFreq;
	iLines_=iRealLines;

CLEAN_GETSPECCHANNZOOMSPEC:
	delete [] pWave;
	delete [] pfZoomSpec;
	return nErrorCode;
}

void CRealPlantInfo::SetVibDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	CPlantInfoOnLine::SetVibDataUpdateStatus(tDatetime_);
	_pLockDataUpdateStatues->BeginWrite();
	_bVibDataUpdateHour  = true;
	_bVibDataUpdateDay   = true;
	_bVibDataUpdateMonth = true;
	_bVibDataUpdateSend  = true;
	_pLockDataUpdateStatues->EndWrite();
}

void CRealPlantInfo::SetProcDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	CPlantInfoOnLine::SetProcDataUpdateStatus(tDatetime_);
	_pLockDataUpdateStatues->BeginWrite();
	_bProcDataUpdateHour=true;
	_bProcDataUpdateDay=true;
	_bProcDataUpdateMonth=true;
	_bProcDataUpdateSend=true;
	_pLockDataUpdateStatues->EndWrite();
}

void CRealPlantInfo::SetDynDataUpdateStatus(const CBHDateTime &tDatetime_)
{
	CPlantInfoOnLine::SetDynDataUpdateStatus(tDatetime_);
	_pLockDataUpdateStatues->BeginWrite();
	_bDynDataUpdateHour = true;
	_bDynDataUpdateMonth= true;
	_bDynDataUpdateDay  = true;
	_bDynDataUpdateSend = true;
	_pLockDataUpdateStatues->EndWrite();
}
void CRealPlantInfo::TrendInitID(IDBInterFace * pDb_)
{
	CString lstrDBName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();

	BOOL lbRet = TRUE;
		
		SwitchDbNameEx(pDb_,lstrDBName);

	CString strLog="";
	strLog.Format("%s-%s-%s 初始化各种ID",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CRealPlantInfo","TrendInitID");

	//振动数据
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendSecond=0;
		iCurrIDVibTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDVibTrendSecond>G_D_TREND_SECONDRANGE)
		{	
			BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:1 %s:%d %d",__FILE__,__LINE__,iCurrIDVibTrendSecond));
			SetVibTrendSecondCurrID(0);
		}
		else
		{	
			SetVibTrendSecondCurrID(iCurrIDVibTrendSecond);
			BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:2 %s:%d %d",__FILE__,__LINE__,iCurrIDVibTrendSecond));
		}
	}
	else
	{	
		SetVibTrendSecondCurrID(0);
		BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:3 %s:%d %d",__FILE__,__LINE__,0));
	}

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendHour=0;
		iCurrIDVibTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDVibTrendHour>G_D_TREND_HOURRANGE)
			SetVibTrendHourCurrID(0);
		else
			SetVibTrendHourCurrID(iCurrIDVibTrendHour);
	}
	else
		SetVibTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDVibTrendDay=0;
		iCurrIDVibTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDVibTrendDay>G_D_TREND_DAYRANGE)
			SetVibTrendDayCurrID(0);
		else
			SetVibTrendDayCurrID(iCurrIDVibTrendDay);
	}
	else
		SetVibTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendMonth=0;
		iCurrIDVibTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
	}
	else
		SetVibTrendMonthCurrID(0);

	//动态测点数据
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendSecond=0;
		iCurrIDDynTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDDynTrendSecond>G_D_TREND_SECONDRANGE)
			SetDynTrendSecondCurrID(0);
		else
			SetDynTrendSecondCurrID(iCurrIDDynTrendSecond);
	}
	else
		SetDynTrendSecondCurrID(0);

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendHour=0;
		iCurrIDDynTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDDynTrendHour>G_D_TREND_HOURRANGE)
			SetDynTrendHourCurrID(0);
		else
			SetDynTrendHourCurrID(iCurrIDDynTrendHour);
	}
	else
		SetDynTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDDynTrendDay=0;
		iCurrIDDynTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDDynTrendDay>G_D_TREND_DAYRANGE)
			SetDynTrendDayCurrID(0);
		else
			SetDynTrendDayCurrID(iCurrIDDynTrendDay);
	}
	else
		SetDynTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendMonth=0;
		iCurrIDDynTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
	}
	else
		SetDynTrendMonthCurrID(0);

	///过程量
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendSecond=0;
		iCurrIDProcTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDProcTrendSecond>G_D_TREND_SECONDRANGE)
			SetProcTrendSecondCurrID(0);
		else
			SetProcTrendSecondCurrID(iCurrIDProcTrendSecond);
	}
	else
		SetProcTrendSecondCurrID(0);

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendHour=0;
		iCurrIDProcTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDProcTrendHour>G_D_TREND_HOURRANGE)
			SetProcTrendHourCurrID(0);
		else
			SetProcTrendHourCurrID(iCurrIDProcTrendHour);
	}
	else
		SetProcTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDProcTrendDay=0;
		iCurrIDProcTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDProcTrendDay>G_D_TREND_DAYRANGE)
			SetProcTrendDayCurrID(0);
		else
			SetProcTrendDayCurrID(iCurrIDProcTrendDay);
	}
	else
		SetProcTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendMonth=0;
		iCurrIDProcTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetProcTrendMonthCurrID(iCurrIDProcTrendMonth);
	}
	else
		SetProcTrendMonthCurrID(0);

	//初始化振动快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendFast=0;
		iCurrIDVibTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetVibTrendFastCurrID(iCurrIDVibTrendFast);
	}
	else
		SetVibTrendFastCurrID(0);

	//初始化动态快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendFast=0;
		iCurrIDDynTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetDynTrendFastCurrID(iCurrIDDynTrendFast);
	}
	else
		SetDynTrendFastCurrID(0);

	//初始化过程快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendFast=0;
		iCurrIDProcTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetProcTrendFastCurrID(iCurrIDProcTrendFast);
	}
	else
		SetProcTrendFastCurrID(0);


	_tLastRevTime       = g_dtInitTime;

	_timeProcDataUpdate = g_dtInitTime;
	_timeVibDataUpdate  = g_dtInitTime;
	_timeDynDataUpdate  = g_dtInitTime;

	__time64_t llInitTime = COleDateTimeToI64(g_dtInitTime);

	_timeSaveVibSecond  = llInitTime;
	_timeSaveVibHour    = llInitTime;
	_timeSaveVibDay     = llInitTime;
	_timeSaveVibMonth   = llInitTime;


	_timeSaveDynSecond  = llInitTime;
	_timeSaveDynHour    = llInitTime;
	_timeSaveDynDay     = llInitTime;
	_timeSaveDynMonth   = llInitTime;

	_timeSaveProcSecond = llInitTime;
	_timeSaveProcHour   = llInitTime;
	_timeSaveProcDay    = llInitTime;
	_timeSaveProcMonth  = llInitTime;

	SetNegVibTrendSecondCurrID(0);
	SetNegVibTrendHourCurrID(0);
	SetNegVibTrendDayCurrID(0);
	SetNegVibTrendMonthCurrID(0);

	SetNegDynTrendSecondCurrID(0);
	SetNegDynTrendHourCurrID(0);
	SetNegDynTrendDayCurrID(0);
	SetNegDynTrendMonthCurrID(0);

	SetNegProcTrendSecondCurrID(0);
	SetNegProcTrendHourCurrID(0);
	SetNegProcTrendDayCurrID(0);
	SetNegProcTrendMonthCurrID(0);

	SetNegVibTrendFastCurrID(0);
	SetNegDynTrendFastCurrID(0);
	SetNegProcTrendFastCurrID(0);

	/*
	strLog.Format("%s-%s-%s 初始化各种ID完成",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CRealPlantInfo","TrendInitID");
	*/
}


void CRealPlantInfo::TrendInitID(CCustomDB * pDb_)
{
	CString lstrDBName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();

	BOOL lbRet = SwichDbName(pDb_,lstrDBName);

	CString strLog="";
	strLog.Format("%s-%s-%s 初始化各种ID",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CRealPlantInfo","TrendInitID");

	//振动数据
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendSecond=0;
		iCurrIDVibTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDVibTrendSecond>G_D_TREND_SECONDRANGE)
		{	
			BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:1 %s:%d %d",__FILE__,__LINE__,iCurrIDVibTrendSecond));
			SetVibTrendSecondCurrID(0);
		}
		else
		{	
			SetVibTrendSecondCurrID(iCurrIDVibTrendSecond);
			BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:2 %s:%d %d",__FILE__,__LINE__,iCurrIDVibTrendSecond));
		}
	}
	else
	{	
		SetVibTrendSecondCurrID(0);
		BHLOG_DEBUG(STR("iCurrIDVibTrendSecond IID:3 %s:%d %d",__FILE__,__LINE__,0));
	}

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendHour=0;
		iCurrIDVibTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDVibTrendHour>G_D_TREND_HOURRANGE)
			SetVibTrendHourCurrID(0);
		else
			SetVibTrendHourCurrID(iCurrIDVibTrendHour);
	}
	else
		SetVibTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDVibTrendDay=0;
		iCurrIDVibTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDVibTrendDay>G_D_TREND_DAYRANGE)
			SetVibTrendDayCurrID(0);
		else
			SetVibTrendDayCurrID(iCurrIDVibTrendDay);
	}
	else
		SetVibTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendMonth=0;
		iCurrIDVibTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
	}
	else
		SetVibTrendMonthCurrID(0);

	//动态测点数据
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendSecond=0;
		iCurrIDDynTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDDynTrendSecond>G_D_TREND_SECONDRANGE)
			SetDynTrendSecondCurrID(0);
		else
			SetDynTrendSecondCurrID(iCurrIDDynTrendSecond);
	}
	else
		SetDynTrendSecondCurrID(0);

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendHour=0;
		iCurrIDDynTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDDynTrendHour>G_D_TREND_HOURRANGE)
			SetDynTrendHourCurrID(0);
		else
			SetDynTrendHourCurrID(iCurrIDDynTrendHour);
	}
	else
		SetDynTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDDynTrendDay=0;
		iCurrIDDynTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDDynTrendDay>G_D_TREND_DAYRANGE)
			SetDynTrendDayCurrID(0);
		else
			SetDynTrendDayCurrID(iCurrIDDynTrendDay);
	}
	else
		SetDynTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendMonth=0;
		iCurrIDDynTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
	}
	else
		SetDynTrendMonthCurrID(0);

	///过程量
	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendSecTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendSecond=0;
		iCurrIDProcTrendSecond=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDProcTrendSecond>G_D_TREND_SECONDRANGE)
			SetProcTrendSecondCurrID(0);
		else
			SetProcTrendSecondCurrID(iCurrIDProcTrendSecond);
	}
	else
		SetProcTrendSecondCurrID(0);

	//初始化小时趋势表中最新的记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendHourTblName.c_str(),gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendHour=0;
		iCurrIDProcTrendHour=pDb_->GetLong(gc_cID)+1;
		if (iCurrIDProcTrendHour>G_D_TREND_HOURRANGE)
			SetProcTrendHourCurrID(0);
		else
			SetProcTrendHourCurrID(iCurrIDProcTrendHour);
	}
	else
		SetProcTrendHourCurrID(0);

	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendDayTblName.c_str(),gc_cTime);
	if (pDb_->More())
	{
		int iCurrIDProcTrendDay=0;
		iCurrIDProcTrendDay=(pDb_->GetLong(gc_cID))+1;
		if (iCurrIDProcTrendDay>G_D_TREND_DAYRANGE)
			SetProcTrendDayCurrID(0);
		else
			SetProcTrendDayCurrID(iCurrIDProcTrendDay);
	}
	else
		SetProcTrendDayCurrID(0);

	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	//月表按ID值大小初始化，避免ID冲突
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendMonthTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendMonth=0;
		iCurrIDProcTrendMonth=(pDb_->GetLong(gc_cID))+1;
		SetProcTrendMonthCurrID(iCurrIDProcTrendMonth);
	}
	else
		SetProcTrendMonthCurrID(0);

	//初始化振动快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sVibTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDVibTrendFast=0;
		iCurrIDVibTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetVibTrendFastCurrID(iCurrIDVibTrendFast);
	}
	else
		SetVibTrendFastCurrID(0);

	//初始化动态快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sDynTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDDynTrendFast=0;
		iCurrIDDynTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetDynTrendFastCurrID(iCurrIDDynTrendFast);
	}
	else
		SetDynTrendFastCurrID(0);

	//初始化过程快变趋势表中最新记录ID值
	pDb_->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_sProcTrendFastTblName.c_str(), gc_cTime);
	if(pDb_->More())
	{
		int iCurrIDProcTrendFast=0;
		iCurrIDProcTrendFast=(pDb_->GetLong(gc_cID))+1;
		SetProcTrendFastCurrID(iCurrIDProcTrendFast);
	}
	else
		SetProcTrendFastCurrID(0);


	_tLastRevTime       = g_dtInitTime;

	_timeProcDataUpdate = g_dtInitTime;
	_timeVibDataUpdate  = g_dtInitTime;
	_timeDynDataUpdate  = g_dtInitTime;

	__time64_t llInitTime = COleDateTimeToI64(g_dtInitTime);

	_timeSaveVibSecond  = llInitTime;
	_timeSaveVibHour    = llInitTime;
	_timeSaveVibDay     = llInitTime;
	_timeSaveVibMonth   = llInitTime;


	_timeSaveDynSecond  = llInitTime;
	_timeSaveDynHour    = llInitTime;
	_timeSaveDynDay     = llInitTime;
	_timeSaveDynMonth   = llInitTime;

	_timeSaveProcSecond = llInitTime;
	_timeSaveProcHour   = llInitTime;
	_timeSaveProcDay    = llInitTime;
	_timeSaveProcMonth  = llInitTime;
	SetNegVibTrendSecondCurrID(0);
	SetNegVibTrendHourCurrID(0);
	SetNegVibTrendDayCurrID(0);
	SetNegVibTrendMonthCurrID(0);

	SetNegDynTrendSecondCurrID(0);
	SetNegDynTrendHourCurrID(0);
	SetNegDynTrendDayCurrID(0);
	SetNegDynTrendMonthCurrID(0);

	SetNegProcTrendSecondCurrID(0);
	SetNegProcTrendHourCurrID(0);
	SetNegProcTrendDayCurrID(0);
	SetNegProcTrendMonthCurrID(0);

	SetNegVibTrendFastCurrID(0);
	SetNegDynTrendFastCurrID(0);
	SetNegProcTrendFastCurrID(0);

	/*
	strLog.Format("%s-%s-%s 初始化各种ID完成",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CRealPlantInfo","TrendInitID");
	*/
}

int CRealPlantInfo::SaveTrendData(IDBInterFace * pDb)
{
	int iRtn=-1;

	_pLockVibData->BeginWrite();
	try
	{
		_timeCurr     = CBHDateTime::GetCurrentTimeEx();
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndWrite();

	bool bFlag=false;

	_pLockSaveDataStatus->BeginWrite();

	if (_bSaveDataStatus)
	{
		bFlag=false;
	}
	else
	{
		bFlag=true;
		_bSaveDataStatus=true;
	}

	_pLockSaveDataStatus->EndWrite();

	if (bFlag)
	{
		if (G_TEMP_ONLINE)
		{
			std::vector<float> loRev(5);

			this->GetRealRev(&loRev.front(),loRev.size());

			BOOL lbRevStart = FALSE;

			for (int i =0;i<loRev.size();i++)
			{
				if (loRev[i] > 0)
				{
					lbRevStart = TRUE;
					break;
				}
			}

			if (lbRevStart || g_SaveDataOnStop)
			{
				SaveSecondTrend(pDb);
				SaveHourTrend(pDb);
				SaveDayTrend(pDb);
			}

			if (!lbRevStart)
			{
				if (g_SaveMonthDataOnStop)
				{
					iRtn=SaveMonthTrend(pDb);
				}
			}else
			{
				iRtn=SaveMonthTrend(pDb);
			}			

		}else
		{
			if (!g_SaveDataOnStop)
			{
				bool lbRun = this->GetVibJudgeRunStatus() || this->GetProcJudgeRunStatus();
				BOOL lbIsPlantRunning = ::IsPlantRunning(*this,lbRun);
				if (lbIsPlantRunning)
				{
					iRtn=SaveSecondTrend(pDb);
					SaveHourTrend(pDb);
					SaveDayTrend(pDb);
				}

			}else
			{

				iRtn=SaveSecondTrend(pDb);
				SaveHourTrend(pDb);
				SaveDayTrend(pDb);
			}

			if (!g_SaveMonthDataOnStop)
			{
				bool lbRun = this->GetVibJudgeRunStatus() || this->GetProcJudgeRunStatus();
				BOOL lbIsPlantRunning = ::IsPlantRunning(*this,lbRun);
				if (lbIsPlantRunning)
				{
					SaveMonthTrend(pDb);
				}
				
			}else
			{
				SaveMonthTrend(pDb);

			}		
			
		}

		_pLockSaveDataStatus->BeginWrite();
		_bSaveDataStatus=false;
		_pLockSaveDataStatus->EndWrite();
	}
	return iRtn;
}

bool CRealPlantInfo::IsDateTimeDifferent(
	CBHDateTime Last_,
	CBHDateTime Curr_)
{
	COleDateTimeSpan timeSpan = Curr_ - Last_;
	int lnTotalSeconds = timeSpan.GetTotalSeconds();
	
	return (lnTotalSeconds >= 1);
}


int CRealPlantInfo::SaveSecondTrend(IDBInterFace * pDb_)
{
	int iRtn=0;

	bool         bUpdate = false;
	CBHDateTime tUpdate = CBHDateTime::GetCurrentTime();
	
	///振动测点数据
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bVibDataUpdate;
	tUpdate=_timeVibDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	
	if (bUpdate)
	{		
		if (IsDateTimeDifferent(
				CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveVibSecond)),
				tUpdate))
		{
			_pLockDataUpdateStatues->BeginWrite();
			_bVibDataUpdate=false;
			_pLockDataUpdateStatues->EndWrite();

			if(this->VibSecTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CVibChannParam *pVibChannParam=NULL;

				_pLockVibData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					pVibChannParam=(CVibChannParam*)(*it);
					if(pVibChannParam)
					{
						pVibChannParam->SecondTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
				_pLockVibData->EndWrite();
						
			}

			int iCurrIDVibTrendSecond=0;
			GetVibTrendSecondCurrID(iCurrIDVibTrendSecond);
			
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDVibTrendSecond = this->ResetTrendTableCurID(&loDb,_sVibTrendSecTblName,iCurrIDVibTrendSecond);
			
			
			SaveVibRecord(&loDb,_sVibTrendSecTblName,iCurrIDVibTrendSecond,tUpdate);

			InterlockedWrite64(&_timeSaveVibSecond, COleDateTimeToI64(tUpdate));
		}

		iRtn += 1;
	}
	///动态测点数据
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bDynDataUpdate;
	tUpdate=_timeDynDataUpdate;
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		if (IsDateTimeDifferent(
				CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveDynSecond)),
				tUpdate))
		{
			_pLockDataUpdateStatues->BeginWrite();
			_bDynDataUpdate = false;
			_pLockDataUpdateStatues->EndWrite();

			if(this->DynSecTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CDynChannParam *pDynChannParam=NULL;

				_pLockDynData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
				{
					pDynChannParam=(CDynChannParam*)(*it);
					if(pDynChannParam)
					{
						pDynChannParam->SecondTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
			   _pLockDynData->EndWrite();

			}

			int iCurrIDDynTrendSecond=0;
			GetDynTrendSecondCurrID(iCurrIDDynTrendSecond);

			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDDynTrendSecond = this->ResetTrendTableCurID(&loDb,_sDynTrendSecTblName,iCurrIDDynTrendSecond);
			
			
			SaveDynRecord(&loDb, _sDynTrendSecTblName, iCurrIDDynTrendSecond, tUpdate);

			InterlockedWrite64(&_timeSaveDynSecond, COleDateTimeToI64(tUpdate));

			iRtn+=1;
		}
	}

	///过程测点数据
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdate;
	tUpdate=_timeProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{		 
		if (IsDateTimeDifferent(
				CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveProcSecond)),
				tUpdate))
		{
			_pLockDataUpdateStatues->BeginWrite();
			_bProcDataUpdate=false;
			_pLockDataUpdateStatues->EndWrite();

			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			int iCurrIDProcTrendSecond=0;
			GetProcTrendSecondCurrID(iCurrIDProcTrendSecond);

			iCurrIDProcTrendSecond = this->ResetTrendTableCurID(&loDb,_sProcTrendSecTblName,iCurrIDProcTrendSecond);

			SaveProcRecord(&loDb,_sProcTrendSecTblName,iCurrIDProcTrendSecond, tUpdate);

			InterlockedWrite64(&_timeSaveProcSecond, COleDateTimeToI64(tUpdate));

			iRtn+=1;
		}

	}

	return iRtn;
}

int CRealPlantInfo::SaveHourTrend(IDBInterFace * pDb_)
{
	int iRtn=-1;
	
	bool bUpdate=false;
	CBHDateTime tUpdate = CBHDateTime::GetCurrentTime();
	
	COleDateTimeSpan timeSpan;

	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bVibDataUpdateHour;
	tUpdate=_timeVibDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveVibHour));
		if((timeSpan.GetTotalSeconds())>D_TREND_HOURINTERVAL)
		{
			
			if(this->VibHourTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CVibChannParam *pVibChannParam=NULL;
				
				_pLockVibData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					pVibChannParam=(CVibChannParam*)(*it);
					if(pVibChannParam)
					{
						pVibChannParam->HourTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
				_pLockVibData->EndWrite();

			}
			
			int iCurrIDVibTrendHour=0;
			GetVibTrendHourCurrID(iCurrIDVibTrendHour);
			
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());
			
			iCurrIDVibTrendHour = this->ResetTrendTableCurID(&loDb,_sVibTrendHourTblName,iCurrIDVibTrendHour);
			
			SaveVibRecord(&loDb,_sVibTrendHourTblName,iCurrIDVibTrendHour,tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bVibDataUpdateHour=false;
			_pLockDataUpdateStatues->EndWrite();

			InterlockedWrite64(&_timeSaveVibHour, COleDateTimeToI64(tUpdate));
			
			iRtn=1;
		}
	}
	///动态测点
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bDynDataUpdateHour;
	tUpdate=_timeDynDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveDynHour));
		if((timeSpan.GetTotalSeconds())>D_TREND_HOURINTERVAL)
		{
			if(this->DynHourTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CDynChannParam *pDynChannParam=NULL;

				_pLockDynData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
				{
					pDynChannParam=(CDynChannParam*)(*it);
					if(pDynChannParam)
					{
						pDynChannParam->HourTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
				_pLockDynData->EndWrite();

			}

			int iCurrIDDynTrendHour=0;
			GetDynTrendHourCurrID(iCurrIDDynTrendHour);
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDDynTrendHour = this->ResetTrendTableCurID(&loDb,_sDynTrendHourTblName,iCurrIDDynTrendHour);
			
			SaveDynRecord(&loDb, _sDynTrendHourTblName, iCurrIDDynTrendHour, tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bDynDataUpdateHour = false;
			_pLockDataUpdateStatues->EndWrite();

			InterlockedWrite64(&_timeSaveDynHour, COleDateTimeToI64(tUpdate));
			iRtn=1;
		}
	}

	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdateHour;
	tUpdate=_timeProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveProcHour));
		if((timeSpan.GetTotalSeconds())>D_TREND_HOURINTERVAL)
		{
			int iCurrIDProcTrendHour=0;
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			GetProcTrendHourCurrID(iCurrIDProcTrendHour);

			iCurrIDProcTrendHour = this->ResetTrendTableCurID(&loDb,_sProcTrendHourTblName,iCurrIDProcTrendHour);

			SaveProcRecord(&loDb,_sProcTrendHourTblName,iCurrIDProcTrendHour, tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bProcDataUpdateHour=false;
			_pLockDataUpdateStatues->EndWrite();

			InterlockedWrite64(&_timeSaveProcHour, COleDateTimeToI64(tUpdate));
			++iRtn;
		}
	}
	return iRtn;
}

int CRealPlantInfo::SaveDayTrend(IDBInterFace * pDb_)
{
	int iRtn=-1;

	bool         bUpdate = false;
	CBHDateTime tUpdate = CBHDateTime::GetCurrentTime();
	
	COleDateTimeSpan timeSpan;
	
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bVibDataUpdateDay;
	tUpdate=_timeVibDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if ((bUpdate))
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveVibDay));
		if((timeSpan.GetTotalSeconds())>D_TREND_DAYINTERVAL)
		{
			
			if(this->VibDayTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CVibChannParam *pVibChannParam=NULL;

				_pLockVibData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					pVibChannParam=(CVibChannParam*)(*it);
					if(pVibChannParam)
					{
						pVibChannParam->DayTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
				_pLockVibData->EndWrite();

			}

			int iCurrIDVibTrendDay=0;
			GetVibTrendDayCurrID(iCurrIDVibTrendDay);

			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDVibTrendDay = this->ResetTrendTableCurID(&loDb,_sVibTrendDayTblName,iCurrIDVibTrendDay);

			SaveVibRecord(&loDb,_sVibTrendDayTblName,iCurrIDVibTrendDay,tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bVibDataUpdateDay=false;
			_pLockDataUpdateStatues->EndWrite();

			InterlockedWrite64(&_timeSaveVibDay, COleDateTimeToI64(tUpdate));
			iRtn=1;
		}
	}
	///dynimatic channel
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bDynDataUpdateDay;
	tUpdate=_timeDynDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if ((bUpdate))
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveDynDay));
		if((timeSpan.GetTotalSeconds())>D_TREND_DAYINTERVAL)
		{
			if(this->DynDayTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CDynChannParam *pDynChannParam=NULL;
				
				_pLockDynData->BeginWrite();
				BEGIN_ERROR_HANDLE
				for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
				{
					pDynChannParam=(CDynChannParam*)(*it);
					if(pDynChannParam)
					{
						pDynChannParam->DayTblLastValue(G_F_INIT_VALUE);
					}
				}
				END_ERROR_HANDLE
				_pLockDynData->EndWrite();

			}

			int iCurrIDDynTrendDay=0;
			GetDynTrendDayCurrID(iCurrIDDynTrendDay);

			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDDynTrendDay = this->ResetTrendTableCurID(&loDb,_sDynTrendDayTblName,iCurrIDDynTrendDay);
			SaveDynRecord(&loDb, _sDynTrendDayTblName, iCurrIDDynTrendDay, tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bDynDataUpdateDay=false;
			_pLockDataUpdateStatues->EndWrite();

			
			InterlockedWrite64(&_timeSaveDynDay, COleDateTimeToI64(tUpdate));

			iRtn=1;
		}
	}

	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdateDay;
	tUpdate=_timeProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveProcDay));
		if((timeSpan.GetTotalSeconds())>D_TREND_DAYINTERVAL)
		{
			int iCurrIDProcTrendDay=0;

			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			GetProcTrendDayCurrID(iCurrIDProcTrendDay);

			iCurrIDProcTrendDay = this->ResetTrendTableCurID(&loDb,_sProcTrendDayTblName,iCurrIDProcTrendDay);
			
			SaveProcRecord(&loDb,_sProcTrendDayTblName,iCurrIDProcTrendDay, tUpdate);
			_pLockDataUpdateStatues->BeginWrite();
			_bProcDataUpdateDay=false;
			_pLockDataUpdateStatues->EndWrite();

			InterlockedWrite64(&_timeSaveProcDay, COleDateTimeToI64(tUpdate));
			iRtn+=1;
		}
	}
	return iRtn;
}

int CRealPlantInfo::SaveMonthTrend(IDBInterFace * pDb_)
{
	int iRtn=-1;

	bool         bUpdate = false;
	CBHDateTime tUpdate = CBHDateTime::GetCurrentTime();

	COleDateTimeSpan timeSpan;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bVibDataUpdateMonth;
	tUpdate=_timeVibDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveVibMonth));
		if((timeSpan.GetTotalMinutes())>D_TREND_MONTHINTERVAL)
		{
			
			if(this->VibMonthTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CVibChannParam *pVibChannParam=NULL;

				_pLockVibData->BeginWrite();
				BEGIN_ERROR_HANDLE;
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					pVibChannParam=(CVibChannParam*)(*it);
					if(pVibChannParam)
					{
						pVibChannParam->MonthTblLastValue(G_F_INIT_VALUE);
					} 
				}
				END_ERROR_HANDLE;
				_pLockVibData->EndWrite();

			}

			int iCurrIDVibTrendMonth=0;
			GetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDVibTrendMonth = this->ResetTrendTableCurID(pDb_,_sVibTrendMonthTblName,iCurrIDVibTrendMonth);
			
			SaveVibRecord(&loDb,_sVibTrendMonthTblName,iCurrIDVibTrendMonth,tUpdate,true);

			_pLockDataUpdateStatues->BeginWrite();
			_bVibDataUpdateMonth=false;
			_pLockDataUpdateStatues->EndWrite();
			InterlockedWrite64(&_timeSaveVibMonth, COleDateTimeToI64(tUpdate));
			iRtn=1;
		}
	}
	///dynamic channel
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bDynDataUpdateMonth;
	tUpdate=_timeDynDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveDynMonth));
		if((timeSpan.GetTotalMinutes()) > D_TREND_MONTHINTERVAL)
		{
			if(this->DynMonthTblSaveTimeReached())
			{
				V_PCHANNINFO::iterator it;
				CDynChannParam *pDynChannParam=NULL;

				_pLockDynData->BeginWrite();
				BEGIN_ERROR_HANDLE;
				for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
				{
					pDynChannParam=(CDynChannParam*)(*it);
					if(pDynChannParam)
					{
						pDynChannParam->MonthTblLastValue(G_F_INIT_VALUE);
					} 
				}
				END_ERROR_HANDLE;
				_pLockDynData->EndWrite();

			}

			int iCurrIDDynTrendMonth=0;
			GetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			iCurrIDDynTrendMonth = this->ResetTrendTableCurID(pDb_,_sDynTrendMonthTblName,iCurrIDDynTrendMonth);

			SaveDynRecord( &loDb,_sDynTrendMonthTblName, iCurrIDDynTrendMonth,tUpdate,true);
			
			_pLockDataUpdateStatues->BeginWrite();
			_bDynDataUpdateMonth = false;
			_pLockDataUpdateStatues->EndWrite();
			InterlockedWrite64(&_timeSaveDynMonth, COleDateTimeToI64(tUpdate));
			iRtn=1;
		}
	}

	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdateMonth;
	tUpdate=_timeProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		timeSpan = tUpdate - CBHDateTime((__time64_t)InterlockedRead64(&_timeSaveProcMonth));
		if((timeSpan.GetTotalMinutes())>D_TREND_MONTHINTERVAL)
		{
			int iCurrIDProcTrendMonth=0;
			COleSqlServerDB loDb;
			loDb.setPDB(pDb_->getPDB());

			GetProcTrendMonthCurrID(iCurrIDProcTrendMonth);

			iCurrIDProcTrendMonth = this->ResetTrendTableCurID(pDb_,_sProcTrendMonthTblName,iCurrIDProcTrendMonth);

			SaveProcRecord(&loDb,_sProcTrendMonthTblName,iCurrIDProcTrendMonth, tUpdate);
			
			_pLockDataUpdateStatues->BeginWrite();
			_bProcDataUpdateMonth=false;
			_pLockDataUpdateStatues->EndWrite();
			++iRtn;
			InterlockedWrite64(&_timeSaveProcMonth, COleDateTimeToI64(tUpdate));
		}
	}
	return iRtn;
}

void CRealPlantInfo::SaveVibRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_, const CBHDateTime &tDatetime_, bool bMonth_)
{

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR || 
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}
	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_VibCharValue vibValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev( pfRev, 5);
	GetRealSmpInfo(iSmpNum, iSmpFreq);

	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	CBHDateTime ltUpdateTime = tDatetime_;

	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*	int iSmpNumMax = GetPlantInfo()._iSmpNum;*/

	int iSmpNumMax = iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{
		iSaveWaveNum=iSmpNumMax;
	}

	ASSERT(iSaveWaveNum >0);

	int lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

	iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

	BOOL lbShoudUpdate = TRUE;
	BOOL lbErrorOccoured  = FALSE;

	V_PCHANNINFO::iterator it;
	CVibChannParam *pVibChannParam=NULL;

	BOOL lbMonth = bMonth_;

	int lnUpdateMonthTable = CConfigHelper::GetMonthTableUpdateFlag();

	if(g_bIsDiskFull && ((!lbMonth)||(lbMonth && lnUpdateMonthTable > 0)))
	{
		int lnTotalWavePtNum = 0;

		_pLockVibData->BeginRead();
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			pVibChannParam=(CVibChannParam*)(*it);	

			lnTotalWavePtNum += pVibChannParam->GetCharValue()._iSmpNum;
		}
		_pLockVibData->EndRead();
		MakeSureVibSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,0);
	}

	if(g_bIsDiskFull &&  lnUpdateMonthTable> 0)
	{
		lbMonth = false;
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT %s,%s FROM [%s] ",gc_cTime,_sVibFieldSel.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
		zdlTraceLine("**************************** Being Checked!***********************\r\n%s",pDB_->GetSql());
		if(pDB_->More(TRUE))
		{
			CBHDateTime loDateTime = pDB_->GetTime(gc_cTime);

			COleSqlServerDB loDB;
			loDB.setPDB(pDB_->getPDB());
			SwitchDbNameEx(&loDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

			this->RemoveSpecTime(&loDB,sTableName_);
			if(!SaveSpecTimeData(&loDB,sTableName_,loDateTime,iCurID_))
			{
				if (!g_bIsShouldSaveEmptyValue)
				{
					for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
					{
						pVibChannParam=(CVibChannParam*)(*it);	

						int lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;
						//特征值全部为0，且配置了不保存空特征值则跳过
						if (pVibChannParam->IsEigenValueEmpty() && lnDataSource != GE_DATA_SOURCE_WIRELESS)
						{
							lbShoudUpdate = FALSE;
							break;
						}
					}
				}
				if(!lbShoudUpdate)
				{
					return;
				}

				std::vector<int> lvShouldSaveWave;

				{
					zdlTraceLine("**************************** Checked Succeed!***********************\r\n%s",pDB_->GetSql());
					BYTE  *pBuf=new BYTE[iAllBytes];
					float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];

					int lnCharIndex = -1;

					for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
					{
						lnCharIndex++;

						pVibChannParam=(CVibChannParam*)(*it);	

						int iDataSource = pVibChannParam->GetChannInfo()._iDataSource;

						bool lbShouldSave = TRUE;

						_pLockVibData->BeginWrite();
						pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

						//特征值全部为0，且配置了不保存空特征值则跳过
						if (!g_bIsShouldSaveEmptyValue)
						{
							if (pVibChannParam->IsEigenValueEmpty() && iDataSource != GE_DATA_SOURCE_WIRELESS)
							{
								lbShouldSave = FALSE;
							}
						}
						int iRealSmpNum=0;

						BEGIN_ERROR_HANDLE;

						iRealSmpNum = pVibChannParam->GetCharValue()._iSmpNum;

						END_ERROR_HANDLE;

						if(iSaveWaveNum>iRealSmpNum)
						{
							iSaveWaveNum=iRealSmpNum;
						}

						iSaveWaveNum = iRealSmpNum;

						if(iSaveWaveNum>iSmpNumMax)
						{
							iSaveWaveNum=iSmpNumMax;
						}

						iSmpNum=iSaveWaveNum;
						int lnHeadSize = 0;
						int lnTotalSize = 0;

						BEGIN_ERROR_HANDLE;

						ASSERT(iSaveWaveNum > 0);

						((int)pVibChannParam->GetCharValue()._iSmpNum) = iSaveWaveNum;

						CWaveHead lsHead = pVibChannParam->WaveHead();

						lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

						memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);

						vibValue=pVibChannParam->GetCharValueEx();
						END_ERROR_HANDLE;

						_pLockVibData->EndWrite();

						if (!lbShouldSave)
						{
							lvShouldSaveWave.push_back(0);

							continue;
						}

						bool lbRet = true;
						if(ShouldSaveVibWaveData(pDB_,sTableName_,pVibChannParam,vibValue))
						{
							lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
							lbRet =pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
							lvShouldSaveWave.push_back(1);
						}
						else
						{
							lvShouldSaveWave.push_back(0);
						}
						
						if (!lbRet)
						{
							lbErrorOccoured = TRUE;
						}
					}
					KD_DeletePoint(pBuf);
					KD_DeletePoint(pfCustomData);
				}
				index=0;
				int lnCharIndex = -1;
				int lnDataSource = GE_DATA_SOURCE_ONLINE;
				int lnLoop = -1;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					lnCharIndex++;
					pVibChannParam=(CVibChannParam*)(*it);

					lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;

					BOOL lbShouldSave = TRUE;

					_pLockVibData->BeginWrite();
					pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

					if (!g_bIsShouldSaveEmptyValue)
					{
						if (pVibChannParam->IsEigenValueEmpty() && lnDataSource != GE_DATA_SOURCE_WIRELESS)
						{
							lbShouldSave = FALSE;
						}
					}

					BEGIN_ERROR_HANDLE;
					vibValue=pVibChannParam->GetCharValueEx();
					ltUpdateTime = this->_timeVibDataUpdate;
					END_ERROR_HANDLE;

					_pLockVibData->EndWrite();

					if (!lbShouldSave)
					{
						continue;
					}

					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibValue.GetSpecTypeData(GE_ALLCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibValue.GetSpecTypeData(GE_ONECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibValue.GetSpecTypeData(GE_TWOCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibValue.GetSpecTypeData(GE_THREECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibValue.GetSpecTypeData(GE_HALFCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibValue.GetSpecTypeData(GE_RESCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARONE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTWO));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARHALF));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibValue.GetSpecTypeData(GE_DCCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibValue.GetSpecTypeData(GE_GAP));
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);
					if(g_SaveUseOptimizeMode && 
						lvShouldSaveWave.size() > lnCharIndex &&
						lvShouldSaveWave[lnCharIndex] == 0)
					{
						pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
						pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());
					}

					iSaveWaveNum = vibValue._iSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}

					iSmpNum=iSaveWaveNum;

					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),iSmpNum);
					ASSERT(iSmpNum != 0);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);

				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,ltUpdateTime.Format(DATE_TIME_FORMAT));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				pDB_->Cmd(" [%s]=%.2f ",gc_cRev5,pfRev[4]);
				pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
				pDB_->More();
				return;
			}
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sVibFieldIns.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	index=0;
	int lnCharIndex = -1;
	for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
	{
		lnCharIndex++;
		pVibChannParam=(CVibChannParam*)(*it);

		int lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;

		BOOL lbShouldSave = TRUE;
		_pLockVibData->BeginWrite();
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

		//特征值全部为0，且配置了不保存空特征值则跳过
		if (!g_bIsShouldSaveEmptyValue)
		{			
			if (pVibChannParam->IsEigenValueEmpty() && lnDataSource != GE_DATA_SOURCE_WIRELESS)
			{
				lbShouldSave = FALSE;
			}
		}

		BEGIN_ERROR_HANDLE;
		
		vibValue=pVibChannParam->GetCharValueEx();
		ltUpdateTime = this->_timeVibDataUpdate;
		
		END_ERROR_HANDLE;

		_pLockVibData->EndWrite();

		if (!lbShouldSave)
		{
			continue;
		}

		index ++;

		iSaveWaveNum = vibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}

		if (iSaveWaveNum != 0)
		{
			iSmpNum=iSaveWaveNum;
		}		

		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ALLCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ONECHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_TWOCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_THREECHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_HALFCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_RESCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_DCCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_GAP));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARONE));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTWO));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARHALF));
		pDB_->Cmd("%d,",vibValue._iSmpFreq);
		pDB_->Cmd("%d,",iSaveWaveNum);
		ASSERT(iSmpNum != 0);
		pDB_->Cmd("%d,",vibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}

	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);

	for (int nloop=0;nloop<5;++nloop)
	{	pDB_->Cmd("%.2f,",pfRev[nloop]);
	}

	pDB_->Cmd("'%s')",ltUpdateTime.Format(DATE_TIME_FORMAT));

	if(!pDB_->Exec())
	{
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveVibRecord");
		lbErrorOccoured = TRUE;
		return;
	}

	pDB_->Cmd("SELECT %s FROM [%s]",_sVibFieldSel.c_str(),sTableName_.c_str());
	pDB_->Cmd(" WHERE %s=%d",gc_cID,iCurID_);

	if(pDB_->More(TRUE))
	{
		index=0;
		int lnCharIndex = -1;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		int iRealSmpNum= 0;
		for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
		{
			int lnTotalSize = 0;

			try
			{
				lnCharIndex++;
				pVibChannParam=(CVibChannParam*)(*it);

				int lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;

				BOOL lbShouldSave = TRUE;
				_pLockVibData->BeginWrite();
				BEGIN_ERROR_HANDLE;
				
				pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

				//特征值全部为0，且配置了不保存空特征值则跳过
				if (!g_bIsShouldSaveEmptyValue)
				{
					if (pVibChannParam->IsEigenValueEmpty() && lnDataSource != GE_DATA_SOURCE_WIRELESS)
					{
						lbShouldSave = FALSE;
					}
				}
				
				END_ERROR_HANDLE;
				iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;

				iSaveWaveNum = iRealSmpNum;

				if(iSaveWaveNum>iSmpNumMax)
				{
					iSaveWaveNum=iSmpNumMax;
				}

				

				BEGIN_ERROR_HANDLE;
				

				CWaveHead lsHead = pVibChannParam->WaveHead();
				lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

				memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
				vibValue = pVibChannParam->GetCharValueEx();
				END_ERROR_HANDLE;

				_pLockVibData->EndWrite();

				if (!lbShouldSave)
				{
					continue;
				}

			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}



			bool lbRet = true;
			if(this->ShouldSaveVibWaveData(pDB_,sTableName_,pVibChannParam,vibValue))
			{
				lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
				lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			}
			
		}
		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
	}
	else
	{
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveVibRecord");
	}

	if (!lbErrorOccoured)
	{
		{
			LY::PLANTINFO_STATUS loStatus;
			this->_pLockVibData->BeginRead();
			loStatus.m_strGroup = this->_sGroup.c_str();
			loStatus.m_strCompany = this->_sCompany.c_str();
			loStatus.m_strFactory = this->_sFactory.c_str();
			loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
			loStatus.m_strSet = this->_sPlantInfo._cSetID;

			loStatus.m_nVibChannCount = this->_vVibChann.size();
			loStatus.m_nDynChannCount = this->_vDynChann.size();
			loStatus.m_nProcChannCount = this->_vProcChann.size();

			CBHDateTime loCurrentTime;	
			loCurrentTime = loCurrentTime.GetCurrentTimeEx();
			loStatus.m_strVibSaveTime = loCurrentTime.Format();
			this->_pLockVibData->EndRead();
			if (NULL!= g_pServerStatus)
			{
				g_pServerStatus->UpdateStatistics(loStatus);

			}
		}
	}
	return;
}

void CRealPlantInfo::SaveDynRecord(IDBInterFace * pDB_,const string &sTableName_,const int &iCurID_, const CBHDateTime &tDatetime_, const bool &bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;

	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}

	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_DynCharValue vibValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev(pfRev,5);
	GetDynRealSmpInfo(iSmpNum, iSmpFreq);
	
	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	CBHDateTime ltDateTime = tDatetime_;

	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*int iSmpNumMax = GetPlantInfo()._iSmpNum;*/

	int iSmpNumMax =iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{	
		iSaveWaveNum=iSmpNumMax;
	}

	iAllBytes=this->GetMiddleWareWavePointNumMax()*sizeof(DATATYPE_WAVE);

	BOOL lbShoudUpDate = TRUE;

	V_PCHANNINFO::iterator it;
	CDynChannParam *pVibChannParam=NULL;
	BOOL lbErrorOccoured = FALSE;

	BOOL lbMonth = bMonth_;

	int lnUpdateMonthTable = CConfigHelper::GetMonthTableUpdateFlag();

	if(g_bIsDiskFull && ((!lbMonth)||(lbMonth && lnUpdateMonthTable > 0)))
	{
		int lnTotalWavePtNum = 0;

		_pLockDynData->BeginRead();
		for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
		{
			pVibChannParam =(CDynChannParam*)(*it);	

			lnTotalWavePtNum += pVibChannParam->GetCharValue()._iSmpNum;
		}
		_pLockDynData->EndRead();

		MakeSureDynSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,0);
	}

	if(g_bIsDiskFull &&  lnUpdateMonthTable> 0)
	{
		lbMonth = false;
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT %s,%s FROM [%s] ",gc_cTime,_sDynFieldSel.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
		if(pDB_->More(TRUE))
		{
			CBHDateTime loDateTime = pDB_->GetTime(gc_cTime);

			COleSqlServerDB loDB;
			loDB.setPDB(pDB_->getPDB());
			SwitchDbNameEx(&loDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

			this->RemoveSpecTime(&loDB,sTableName_);
			if(!SaveSpecTimeData(&loDB,sTableName_,loDateTime,iCurID_))
			{
				if (!g_bIsShouldSaveEmptyValue)
				{
					for (it=_vDynChann.begin(); it!= _vDynChann.end(); ++it)
					{
						pVibChannParam=(CDynChannParam*)(*it);
						if (pVibChannParam->IsEigenValueEmpty())
						{
							lbShoudUpDate = FALSE;
							break;
						}
					}
				}

				if(!lbShoudUpDate)
				{
					return;
				}

				std::vector<int> lvShouldSaveWave;

				{
					BYTE  *pBuf=new BYTE[iAllBytes];
					float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
					int lnCharIndex = -1;
					for (it=_vDynChann.begin(); it!= _vDynChann.end(); ++it)
					{
						pVibChannParam=(CDynChannParam*)(*it);
						lnCharIndex++;

						BOOL lbShouldSave = TRUE;
						_pLockDynData->BeginWrite();

						pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
						//配置了不保存全部为0的特征值，且特征值全部为0
						if (!g_bIsShouldSaveEmptyValue)
						{
							if (pVibChannParam->IsEigenValueEmpty())
							{
								lbShouldSave = FALSE;
							}
						}

						int iRealSmpNum= 0;
						int lnTotalSize = 0;

						BEGIN_ERROR_HANDLE;
						iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
						iSaveWaveNum = iRealSmpNum;
						if(iSaveWaveNum>iSmpNumMax)
						{
							iSaveWaveNum=iSmpNumMax;
						}
						((int)(pVibChannParam->GetCharValue()._iSmpNum)) = iSaveWaveNum;

						CWaveHead lsHead = pVibChannParam->WaveHead();
						lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

						memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
						vibValue=pVibChannParam->GetCharValueEx();
						END_ERROR_HANDLE;

						_pLockDynData->EndWrite();

						if (!lbShouldSave)
						{
							lvShouldSaveWave.push_back(0);
							continue;
						}

						BOOL lbSetBufferRet = FALSE;

						if(this->ShouldSaveDynWaveData(pDB_,sTableName_,pVibChannParam,vibValue))
						{
							lbSetBufferRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
							lbSetBufferRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
							lvShouldSaveWave.push_back(1);
						}
						else
						{
							lvShouldSaveWave.push_back(0);
						}

						if (!lbSetBufferRet)
						{
							lbErrorOccoured = TRUE;
						}

						nloop++;
					}
					KD_DeletePoint(pBuf);
					KD_DeletePoint(pfCustomData);
				}
				index=0;


				int lnCharIndex = -1;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (it= _vDynChann.begin(); it!= _vDynChann.end(); ++it)
				{
					lnCharIndex++;
					pVibChannParam = (CDynChannParam*)(*it);

					BOOL lbShouldSave = TRUE;
					_pLockDynData->BeginWrite();
					BEGIN_ERROR_HANDLE;

					pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

					if (!g_bIsShouldSaveEmptyValue)
					{
						if (pVibChannParam->IsEigenValueEmpty())
						{
							lbShouldSave = FALSE;
						}
					}
					vibValue=pVibChannParam->GetCharValueEx();
					ltDateTime = this->_timeDynDataUpdate;				
					END_ERROR_HANDLE;
					_pLockDynData->EndWrite();

					if (!lbShouldSave)
					{
						continue;
					}

					iSaveWaveNum = vibValue._iSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(0).c_str(),vibValue.GetSpecTypeData(0));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(1).c_str(),vibValue.GetSpecTypeData(1));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(2).c_str(),vibValue.GetSpecTypeData(2));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(3).c_str(),vibValue.GetSpecTypeData(3));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(4).c_str(),vibValue.GetSpecTypeData(4));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(5).c_str(),vibValue.GetSpecTypeData(5));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(6).c_str(),vibValue.GetSpecTypeData(6));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(7).c_str(),vibValue.GetSpecTypeData(7));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(8).c_str(),vibValue.GetSpecTypeData(8));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(9).c_str(),vibValue.GetSpecTypeData(9));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(10).c_str(),vibValue.GetSpecTypeData(10));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(11).c_str(),vibValue.GetSpecTypeData(11));
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),iSaveWaveNum);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);
					if(g_SaveUseOptimizeMode &&
						lvShouldSaveWave.size() > lnCharIndex &&
						lvShouldSaveWave[lnCharIndex] == 0)
					{
						pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
						pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());
					}
					//_pDB->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cTempID);
				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,ltDateTime.Format(DATE_TIME_FORMAT));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				pDB_->Cmd(" [%s]=%.2f ",gc_cRev5,pfRev[4]);
				pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
				pDB_->More();
				return;
			}
			
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sDynFieldIns.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	index=0;
	int lnCharIndex =-1;
	for ( it= _vDynChann.begin(); it!= _vDynChann.end(); it++)
	{
		lnCharIndex++;
		pVibChannParam = (CDynChannParam*)(*it);
		
		BOOL lbShouldSave = TRUE;
		
		_pLockDynData->BeginWrite();
		BEGIN_ERROR_HANDLE;
		
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
		if (!g_bIsShouldSaveEmptyValue)
		{
			if (pVibChannParam->IsEigenValueEmpty())
			{
				lbShouldSave = FALSE;
			}
		}
		vibValue = pVibChannParam->GetCharValueEx();
		ltDateTime = this->_timeDynDataUpdate;
		
		END_ERROR_HANDLE;
		_pLockDynData->EndWrite();

		if (!lbShouldSave)
		{
			continue;
		}

		iSaveWaveNum = vibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}

		iSmpNum=iSaveWaveNum;
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(0));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(1));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(2));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(3));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(4));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(5));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(6));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(7));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(8));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(9));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(10));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(11));
		pDB_->Cmd("%d,",vibValue._iSmpFreq);
		pDB_->Cmd("%d,",iSmpNum);
		pDB_->Cmd("%d,",vibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}
	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);
	for (int nloop=0;nloop<5;++nloop)
		pDB_->Cmd("%.2f,",pfRev[nloop]);
	pDB_->Cmd("'%s')",ltDateTime.Format(DATE_TIME_FORMAT));
	zdlTraceLine(pDB_->GetSql());
	if(!pDB_->Exec())
	{
		lbErrorOccoured = TRUE;
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveDynRecord");
		return;
	}

	pDB_->Cmd("SELECT %s FROM [%s]", _sDynFieldSel.c_str(), sTableName_.c_str());
	pDB_->Cmd(" WHERE %s=%d", gc_cID, iCurID_);
	if(pDB_->More(TRUE))
	{
		index=0;
		int lnCharIndex = -1;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
		{
			lnCharIndex++;
			pVibChannParam=(CDynChannParam*)(*it);
			
			BOOL lbShouldSave = TRUE;
			_pLockDynData->BeginWrite();			
			pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
			
			if (!g_bIsShouldSaveEmptyValue)
			{
				if (pVibChannParam->IsEigenValueEmpty())
				{
					lbShouldSave = FALSE;
				}
			}
			int iRealSmpNum= 0;

			int lnTotalSize = 0;

			BEGIN_ERROR_HANDLE;			
			iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
			iSaveWaveNum = iRealSmpNum;
			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}

			CWaveHead lsHead = pVibChannParam->WaveHead();
			lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

			memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);			
			vibValue = pVibChannParam->GetCharValueEx();
			END_ERROR_HANDLE;

			_pLockDynData->EndWrite();

			if (!lbShouldSave)
			{
				continue;
			}

			BOOL lbSetBufferRet = FALSE;

			if(this->ShouldSaveDynWaveData(pDB_,sTableName_,pVibChannParam,vibValue))
			{
				lbSetBufferRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
				lbSetBufferRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			}
			
			if (!lbSetBufferRet)
			{
				lbErrorOccoured = TRUE;
			}
		}
		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
	}
	else
	{
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveDynRecord");
	}

	if (!lbErrorOccoured)
	{
		{
			LY::PLANTINFO_STATUS loStatus;
			this->_pLockDynData->BeginRead();
			loStatus.m_strGroup = this->_sGroup.c_str();
			loStatus.m_strCompany = this->_sCompany.c_str();
			loStatus.m_strFactory = this->_sFactory.c_str();
			loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
			loStatus.m_strSet = this->_sPlantInfo._cSetID;

			loStatus.m_nVibChannCount = this->_vVibChann.size();
			loStatus.m_nDynChannCount = this->_vDynChann.size();
			loStatus.m_nProcChannCount = this->_vProcChann.size();

			CBHDateTime loCurrentTime;	
			loCurrentTime = loCurrentTime.GetCurrentTimeEx();
			loStatus.m_strDynSaveTime = loCurrentTime.Format();
			this->_pLockDynData->EndRead();
			if (NULL!= g_pServerStatus)
			{
				g_pServerStatus->UpdateStatistics(loStatus);

			}
		}
	}
	return;
}

void CRealPlantInfo::SaveProcRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_, const CBHDateTime &tDatetime_, bool bMonth_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	float pfRev[5];
	float fData=.0;
	string sTimeCurr;
	GetRealRev( pfRev, 5);

	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	CBHDateTime ltDateTime = tDatetime_;

	V_PCHANNINFO::iterator result;
	CProcChannParam *pProcChann=NULL;

	BOOL lbMonth = bMonth_;

	if(g_bIsDiskFull && CConfigHelper::GetMonthTableUpdateFlag() > 0)
	{
		lbMonth = false;
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT %s FROM [%s]", gc_cTime,sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
		if(pDB_->More())
		{
			CBHDateTime loDateTime = pDB_->GetTime(gc_cTime);

			COleSqlServerDB loDB;

			loDB.setPDB(pDB_->getPDB());
			SwitchDbNameEx(&loDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

			this->RemoveSpecTime(&loDB,sTableName_);
			if(!SaveSpecTimeData(&loDB,sTableName_,loDateTime,iCurID_))
			{
				pDB_->Cmd("UPDATE [%s] SET  ",sTableName_.c_str());
				for (result=_vProcChann.begin();result!=_vProcChann.end();result++)
				{
					pProcChann=(CProcChannParam*)(*result);
					_pLockProcData->BeginRead();
					try
					{
						fData=pProcChann->GetData();
						ltDateTime = this->_timeProcDataUpdate;
					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockProcData->EndRead();
					pDB_->Cmd(" [%s]=%.2f,",(*result)->GetChannInfo()._cChannNo,fData);
				}
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
				pDB_->Cmd(" [%s]='%s' ",gc_cTime,ltDateTime.Format(DATE_TIME_FORMAT));
				pDB_->Cmd("  WHERE [%s]=%d",gc_cID,iCurID_);
				//		TRACE("proc trend save:%s\n ",_pDB->GetSql());
				if(pDB_->Exec())
				{
					{
						LY::PLANTINFO_STATUS loStatus;
						this->_pLockProcData->BeginRead();
						loStatus.m_strGroup = this->_sGroup.c_str();
						loStatus.m_strCompany = this->_sCompany.c_str();
						loStatus.m_strFactory = this->_sFactory.c_str();
						loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
						loStatus.m_strSet = this->_sPlantInfo._cSetID;

						loStatus.m_nVibChannCount = this->_vVibChann.size();
						loStatus.m_nDynChannCount = this->_vDynChann.size();
						loStatus.m_nProcChannCount = this->_vProcChann.size();

						CBHDateTime loCurrentTime;	
						loCurrentTime = loCurrentTime.GetCurrentTimeEx();
						loStatus.m_strProcSaveTime = loCurrentTime.Format();
						this->_pLockProcData->EndRead();
						if (NULL!= g_pServerStatus)
						{
							g_pServerStatus->UpdateStatistics(loStatus);

						}
					}

					return;
				}
			}
			
		}
	}
	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sProcField.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	for (result=_vProcChann.begin();result!=_vProcChann.end();result++)
	{
		_pLockProcData->BeginRead();
		try
		{
			fData=((CProcChannParam*)(*result))->GetData();
			ltDateTime = this->_timeProcDataUpdate;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndRead();
		pDB_->Cmd(" %.2f,",fData);
	}
	pDB_->Cmd("'%s',",ltDateTime.Format(DATE_TIME_FORMAT));
	pDB_->Cmd(" %.2f,",pfRev[0]);
	pDB_->Cmd(" %.2f,",pfRev[1]);
	pDB_->Cmd(" %.2f,",pfRev[2]);
	pDB_->Cmd(" %.2f,",pfRev[3]);
	pDB_->Cmd(" %.2f)",pfRev[4]);
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
	}
	else
	{
		{
			LY::PLANTINFO_STATUS loStatus;
			this->_pLockProcData->BeginRead();
			loStatus.m_strGroup = this->_sGroup.c_str();
			loStatus.m_strCompany = this->_sCompany.c_str();
			loStatus.m_strFactory = this->_sFactory.c_str();
			loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
			loStatus.m_strSet = this->_sPlantInfo._cSetID;

			loStatus.m_nVibChannCount = this->_vVibChann.size();
			loStatus.m_nDynChannCount = this->_vDynChann.size();
			loStatus.m_nProcChannCount = this->_vProcChann.size();

			CBHDateTime loCurrentTime;	
			loCurrentTime = loCurrentTime.GetCurrentTimeEx();
			loStatus.m_strProcSaveTime = loCurrentTime.Format();
			this->_pLockProcData->EndRead();
			if (NULL!= g_pServerStatus)
			{
				g_pServerStatus->UpdateStatistics(loStatus);

			}
		}
	}
}

bool CRealPlantInfo::GetSpecChannAxisLocationData(string sLocation_,float &fX_,float &fY_)
{
	return GetSpecIndexAxisLocationData(GetSpecAxisLocationIndex(sLocation_),fX_,fY_);
}

bool CRealPlantInfo::GetSpecChannAxisLocationData(string sChannHor_,string sChannVer_,float &fX_,float &fY_)
{
	return GetSpecIndexAxisLocationData(GetSpecAxisLocationIndex(sChannHor_,sChannVer_),fX_,fY_);
}

bool CRealPlantInfo::GetSpecIndexAxisLocationData(int index_,float &fX_,float &fY_)
{
	string sUnit="";
	if ((index_<0)||(index_>(_vAxisChannValue.size()-1)))
		return false;
	S_VibCharValue vibValueVer,vibValueHor;
	CAxisChannInfo channInfo;
	_vAxisChann[index_]->GetChannInfo(channInfo);

	S_ChannInfo loInfoH;
	loInfoH = this->GetSpecVibChannNoInfo(channInfo._cHorChann);

	S_ChannInfo loInfoV;
	loInfoV = this->GetSpecVibChannNoInfo(channInfo._cVerChann);


	if(GetSpecChannRadialAllChar(loInfoH._cChannID,vibValueHor,sUnit)<0)
		return false;
	if(GetSpecChannRadialAllChar(loInfoV._cChannID,vibValueVer,sUnit)<0)
		return false;

	/*
	if(GetSpecChannRadialAllChar(channInfo._cHorChann,vibValueHor,sUnit)<0)
	return false;
	if(GetSpecChannRadialAllChar(channInfo._cVerChann,vibValueVer,sUnit)<0)
	return false;

	int index=GetSpecVibChannIndex(channInfo._cHorChann);*/
	int index=GetSpecVibChannIndex(loInfoH._cChannID);
	if (index<0)
	{
		return false;
	}
	float fSensitivityHor=_vVibChann[index]->GetChannInfo()._fSensitivity;
	float fVerGapV =vibValueVer._fGap;
	/*index=GetSpecVibChannIndex(channInfo._cVerChann);*/
	index=GetSpecVibChannIndex(loInfoV._cChannID);
	if (index<0)
	{
		return false;
	}
	float fSensitivityVer=_vVibChann[index]->GetChannInfo()._fSensitivity;
	float fHorGapV=vibValueHor._fGap;
	bool bFlag = false;
	_pLockAxisLocationData->BeginWrite();
	try
	{
		_vAxisChannValue[index_]._fHorDC=vibValueHor._fDC/10.0;
		_vAxisChannValue[index_]._fHorGap=fHorGapV;
		_vAxisChannValue[index_]._fVerDC=vibValueVer._fDC/10.0;
		_vAxisChannValue[index_]._fVerGap=fVerGapV;
		bFlag=_vAxisChannValue[index_].CalcAxisLocation(channInfo._iLocationType,channInfo._iRotateDirection,channInfo._fAxisDia,fSensitivityVer,fSensitivityHor);
		fX_=_vAxisChannValue[index_]._fCoorX;
		fY_=_vAxisChannValue[index_]._fCoorY;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockAxisLocationData->EndWrite();
	return bFlag;
}

bool CRealPlantInfo::GetSpecDynChannAxisLocationData(string sLocation_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_)
{
	return GetSpecDynIndexAxisLocationData(GetSpecAxisLocationIndex(sLocation_),iWaveNum_,pHorWave_,pVerWave_,pHorAxesWave_,pVerAxesWave_);
}

bool CRealPlantInfo::GetSpecDynChannAxisLocationData(string sChannHor_,string sChannVer_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_)
{
	return GetSpecDynIndexAxisLocationData(GetSpecDynAxisLocationIndex(sChannHor_,sChannVer_),iWaveNum_,pHorWave_,pVerWave_,pHorAxesWave_,pVerAxesWave_);
}

bool CRealPlantInfo::GetSpecDynChannAxisLocationData( string sLocation_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_ )
{
	return GetSpecDynIndexAxisLocationData(GetSpecAxisLocationIndex(sLocation_),iWaveNum_,pHorWave_,pVerWave_,pHorAxesWave_,pVerAxesWave_);
}

bool CRealPlantInfo::GetSpecDynChannAxisLocationData( string sChannHor_,string sChannVer_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_ )
{
	return GetSpecDynIndexAxisLocationData(GetSpecDynAxisLocationIndex(sChannHor_,sChannVer_),iWaveNum_,pHorWave_,pVerWave_,pHorAxesWave_,pVerAxesWave_);
}
//计算动态测点活塞杆轴心位置数据
bool CRealPlantInfo::GetSpecDynIndexAxisLocationData(int index_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_)
{
	if ((index_<0)||(index_>(_vAxisChannValue.size()-1)))
		return false;
	CAxisChannInfo channInfo;
	_vAxisChann[index_]->GetChannInfo(channInfo);
	int iChannIndex=GetSpecDynChannIndexNo(channInfo._cHorChann);
	if (iChannIndex<0)
	{
		return false;
	}
	float fSensitivityHor=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	float fHorGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	iChannIndex=GetSpecDynChannIndexNo(channInfo._cVerChann);
	if (iChannIndex<0)
	{
		return false;
	}
	float fSensitivityVer=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	float fVerGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	pVerWave=new DATATYPE_WAVE[iWaveNum_];
	pHorWave=new DATATYPE_WAVE[iWaveNum_];
	fSensitivityHor==0?8:fSensitivityHor;
	fSensitivityVer==0?8:fSensitivityVer;
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		pVerWave[iRtn]=pVerWave_[iRtn]+(fVerGapV/fSensitivityVer)*1000*10.0;
		pHorWave[iRtn]=pHorWave_[iRtn]+(fHorGapV/fSensitivityHor)*1000*10.0;
	}
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		_pLockAxisLocationData->BeginWrite();
		try
		{
			_vAxisChannValue[index_]._fHorDC = pHorWave[iRtn]*fSensitivityHor/(1000*10.0);
			_vAxisChannValue[index_]._fHorGap = fHorGapV;
			_vAxisChannValue[index_]._fVerDC = pVerWave[iRtn]*fSensitivityVer/(1000*10.0);
			_vAxisChannValue[index_]._fVerGap = fVerGapV;
			_vAxisChannValue[index_].CalcAxisLocation(channInfo._iLocationType,channInfo._iRotateDirection,channInfo._fAxisDia,fSensitivityHor,fSensitivityVer);
			pHorAxesWave_[iRtn]=_vAxisChannValue[index_]._fCoorX;
			pVerAxesWave_[iRtn]=_vAxisChannValue[index_]._fCoorY;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAxisLocationData->EndWrite();
	}
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	return true;
}

bool CRealPlantInfo::GetSpecDynIndexAxisLocationData( int index_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_ )
{
	if ((index_<0)||(index_>(_vAxisChannValue.size()-1)))
		return false;
	CAxisChannInfo channInfo;
	_vAxisChann[index_]->GetChannInfo(channInfo);
	int iChannIndex=GetSpecDynChannIndexNo(channInfo._cHorChann);
	if (iChannIndex<0)
	{
		return false;
	}
	float fSensitivityHor=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	float fHorGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	iChannIndex=GetSpecDynChannIndexNo(channInfo._cVerChann);
	if (iChannIndex<0)
	{
		return false;
	}
	float fSensitivityVer=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	float fVerGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	pVerWave=new DATATYPE_WAVE[iWaveNum_];
	pHorWave=new DATATYPE_WAVE[iWaveNum_];
	fSensitivityHor==0?8:fSensitivityHor;
	fSensitivityVer==0?8:fSensitivityVer;
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		pVerWave[iRtn]=pVerWave_[iRtn]+(fVerGapV/fSensitivityVer)*1000*10.0;
		pHorWave[iRtn]=pHorWave_[iRtn]+(fHorGapV/fSensitivityHor)*1000*10.0;
	}
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		_pLockAxisLocationData->BeginWrite();
		try
		{
			_vAxisChannValue[index_]._fHorDC = pHorWave[iRtn]*fSensitivityHor/(1000*10.0);
			_vAxisChannValue[index_]._fHorGap = fHorGapV;
			_vAxisChannValue[index_]._fVerDC = pVerWave[iRtn]*fSensitivityVer/(1000*10.0);
			_vAxisChannValue[index_]._fVerGap = fVerGapV;
			_vAxisChannValue[index_].CalcAxisLocation(channInfo._iLocationType,channInfo._iRotateDirection,channInfo._fAxisDia,fSensitivityHor,fSensitivityVer);
			pHorAxesWave_[iRtn]=_vAxisChannValue[index_]._fCoorX;
			pVerAxesWave_[iRtn]=_vAxisChannValue[index_]._fCoorY;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAxisLocationData->EndWrite();
	}
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	return true;
}	
int CRealPlantInfo::GetProcData(float *pfData_,int iNum_,int iStartIndex_/* =0 */)
{
	int reallen=(iStartIndex_+iNum_) > _sPlantInfo._iProcChannNum ?(_sPlantInfo._iProcChannNum-iStartIndex_):iNum_;
	if ((reallen<1)||(iStartIndex_<0)){
		return -1;
	}
	bool bUpdate;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdateSend;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		_pLockProcData->BeginRead();
		try
		{
			for(int loop=0;loop<reallen;++loop)
				pfData_[loop]=((CProcChannParam*)_vProcChann[loop+iStartIndex_])->GetData();
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndRead();
	}
	else
		return -2;
	return reallen;
}

int CRealPlantInfo::GetAllVibData(DATATYPE_WAVE *pWave_,S_VibCharValue * pVibCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_/* =0 */)
{
	int nCount=0;
	CWaveHead lsWaveHead;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-3;
	else
	{
		bool bUpdate=false;
		_pLockDataUpdateStatues->BeginRead();
		bUpdate=_bVibDataUpdateSend;
		_pLockDataUpdateStatues->EndRead();
		if (bUpdate)
		{
			nCount=0;
			CVibChannParam *pVibChann=NULL;
			int iPerSmpNum=0;
			_pLockVibData->BeginRead();
			try
			{
				for(int loop=0;loop<iLen_;++loop)
				{
					pVibChann=(CVibChannParam*)_vVibChann[iStartIndex_+loop];
					pVibCharValue_[nCount]=pVibChann->GetCharValueEx();
					
					if (lSmpNums_[loop] > 0)
					{
						string sChannID = GetSpecVibChannInfo(iStartIndex_ + loop)._cChannID;
						GetSpecChannVibWave(sChannID,&(pWave_[iPerSmpNum]), lSmpNums_[loop],&(pRawWave_[iPerSmpNum]),fScaleCoef_,nPrecisionEnabled_,fMaxValue[loop],fMinValue_[loop]);
					}
					
					iPerSmpNum+=lSmpNums_[loop];
					++nCount;
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		else
			nCount=-2;
	}
	return nCount;
}

float CRealPlantInfo::GetVibDataWaveCoef(int iLen_,int iStartIndex_/*=0*/ )
{
	float lfCoef = 0.0;

	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		lfCoef = 0.0;
	else
	{
		bool bUpdate=false;
		_pLockDataUpdateStatues->BeginRead();
		bUpdate=_bVibDataUpdateSend;
		_pLockDataUpdateStatues->EndRead();
		if (bUpdate)
		{
			nCount=0;
			CVibChannParam *pVibChann=NULL;
			int iPerSmpNum=0;
			_pLockVibData->BeginRead();
			try
			{
				for(int loop=0;loop<iLen_;++loop)
				{
					pVibChann=(CVibChannParam*)_vVibChann[iStartIndex_+loop];
					CWaveHead lsWaveHead = pVibChann->WaveHead();

					if(lsWaveHead.m_nMagicValue == WAVE_HEAD_MAGIC_NUMBER && lsWaveHead.m_pParam.size() > 0)
					{
						lfCoef = lsWaveHead.m_pParam[0];
						
					}
					else
					{
						lfCoef = 0.0;
					}

					
					++nCount;
					break;
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		else
			nCount=-2;
	}
	return lfCoef;
}
int CRealPlantInfo::GetAllDynData(DATATYPE_WAVE *pWave_,S_DynCharValue * pDynCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_/* =0 */)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-3;
	else
	{
		bool bUpdate=false;
		_pLockDataUpdateStatues->BeginRead();
		bUpdate=_bDynDataUpdateSend;
		_pLockDataUpdateStatues->EndRead();
		if (bUpdate)
		{
			nCount=0;
			CDynChannParam *pDynChann=NULL;
			int iPerSmpNum=0;
			_pLockVibData->BeginRead();
			try
			{
				for(int loop=0;loop<iLen_;++loop)
				{
					pDynChann=(CDynChannParam*)_vDynChann[iStartIndex_+loop];
					pDynCharValue_[nCount]=pDynChann->GetCharValueEx();
					string sChannID = GetSpecDynChannInfo(iStartIndex_ + loop)._cChannID;
					GetSpecDynChannWave(sChannID,&(pWave_[iPerSmpNum]), lSmpNums_[loop],&(pRawWave_[iPerSmpNum]), fScaleCoef_,nPrecisionEnabled_,fMaxValue[loop],fMinValue_[loop]);
					iPerSmpNum+=lSmpNums_[loop];
					++nCount;
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		else
			nCount=-2;
	}
	return nCount;
}

float CRealPlantInfo::GetDynDataWaveCoef( int iLen_,int iStartIndex_/*=0*/ )
{
	int nCount=0;

	float lfCoef = 0.0;

	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		lfCoef=0.0;
	else
	{
		bool bUpdate=false;
		_pLockDataUpdateStatues->BeginRead();
		bUpdate=_bDynDataUpdateSend;
		_pLockDataUpdateStatues->EndRead();
		if (bUpdate)
		{
			nCount=0;
			CDynChannParam *pDynChann=NULL;
			int iPerSmpNum=0;
			_pLockVibData->BeginRead();
			try
			{
				for(int loop=0;loop<iLen_;++loop)
				{
					pDynChann=(CDynChannParam*)_vDynChann[iStartIndex_+loop];
					
					CWaveHead lsWaveHead = pDynChann->WaveHead();
					if(lsWaveHead.m_nMagicValue == WAVE_HEAD_MAGIC_NUMBER && lsWaveHead.m_pParam.size() > 0)
					{
						lfCoef = lsWaveHead.m_pParam[0];
					}
					else
					{
						lfCoef = 0.0;
					}
					
					++nCount;
					break;
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		else
			nCount=-2;
	}
	return lfCoef;
}
int CRealPlantInfo::GetSpecDynChannCharValue(const string &sChannID_,S_DynCharValue &sDynCharValue_)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChannID_);
	if(iRtn > -1)
	{
		_pLockDynData->BeginWrite();
		((CDynChannParam*)_vDynChann[iRtn])->ComputeEigenValue(GetPlantInfo()._iMachineType,iRtn);
		_pLockDynData->EndWrite();

		_pLockDynData->BeginRead();
		sDynCharValue_=((CDynChannParam*)_vDynChann[iRtn])->GetCharValue();
		_pLockDynData->EndRead();
	}
	return iRtn;
}

int CRealPlantInfo::GetSpecDynChannWaveAndFreq(const string &sChannID_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_,float *pfFreq_)
{
	int iSensorType=0;
	int index=GetSpecDynChannIndex(sChannID_);
	if (index<0)
		return -1;
	iSensorType=_vDynChann[index]->GetChannInfo()._iSensorType;
	float *pfWave=NULL;
	float *pfSpec=NULL;
	//得到客户指定分析线数需要波形数据个数

	int iLinesPt=iLines_*2.56;
	{
		int iSmpFreq = 1;
		int iSmpNums = 1;
		int iRtn;
		CDynChannParam *pDynChann=(CDynChannParam*)(_vDynChann[index]);
		_pLockDynData->BeginRead();
		iSmpNums=pDynChann->GetCharValue()._iSmpNum;
		iSmpFreq=pDynChann->GetCharValue()._iSmpFreq;
		_pLockDynData->EndRead();
		iPtNum_ = iPtNum_ > iSmpNums ? iSmpNums: iPtNum_;
		iLines_=iLines_<2?400:iLines_;
		float fStartFreq=0;
		pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfSpec=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];//先去掉10,确保运行稳定(加10是为了分配的内存不够 pfSpec=new float[(iLines_*1.28+10)])

		CWaveHead lsWaveHead;
		float lfMaxValue = 0;
		float lfMinValue = 0;
		//得到客户需要的波形数据和谱分析需要的波形数据
		_pLockDynData->BeginRead();
		try
		{
			lsWaveHead = pDynChann->WaveHead();
			ConvertDTWaveToFloat(pfWave_,pDynChann->GetCurrWavePoint(),iSmpNums,lsWaveHead);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}
		_pLockDynData->EndRead();

		ConvertFloatToDTWave(pfData_,pfWave_,iSmpNums,lsWaveHead);

		GetScaleCoefAndPrecisionFlag(lsWaveHead,fScaleCoef_,nPrecisionEnabled_,lfMaxValue,lfMinValue);
		//ConvertFloatToDTWave( pfData_, pfWave, iPtNum_);
		iRtn = ZoomSpectrumAnalysis(pfWave,iSmpNums,iSmpFreq,pfSpec,iLines_,fDf_,fStartFreq);
		if (iRtn<0)
			iLines_=0;
		else
		{
			if(iRtn==2)
			{//正常峰值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
					//;
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2);
					ScaleWave(pfFreq_,pfSpec,iLines_,2);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1);
					ScaleWave(pfFreq_,pfSpec,iLines_,1);
				}
			}
			else
			{//细化谱,细化谱为有效值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2.828);
					ScaleWave(pfFreq_,pfSpec,iLines_,2.828);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1.414);
					ScaleWave(pfFreq_,pfSpec,iLines_,1.414);
				}
			}
		}
	}
	if(NULL!=pfWave) delete [] pfWave;
	if(NULL!=pfSpec) delete [] pfSpec;
	return index;
}


void CPlantInfoOnLine::AddCustomParamInfo(IDBInterFace *pDB_)
{
	int index=0;
	vector<CCustomParamInfo> vCustomParamInfo;
	//添加振动自定义参数信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		//获取自定义参数数据
		if (strcmp(pVibChann->GetChannInfo()._cTempID,"") != 0)
		{
			SwitchDbNameEx(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cCustom_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cTempletID,pVibChann->GetChannInfo()._cTempID,gc_cPlantNo,this->_sPlantInfo._cPlantNo);
			while (pDB_->More())
			{
				CCustomParamInfo paramInfo;
				paramInfo.GetCustomParamInfo(pDB_);
				vCustomParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Vib自定义参数数据条数 index :%d,%d",index,vCustomParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pVibChann->SetCustomParamInfo(0,index,&vCustomParamInfo[0]);
			vCustomParamInfo.clear();
		}
	}
	//添加动态自定义参数信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		//获取自定义参数数据
		if (strcmp(pDynChann->GetChannInfo()._cTempID, "") != 0)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cCustom_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cTempletID,pDynChann->GetChannInfo()._cTempID,gc_cPlantNo,this->_sPlantInfo._cPlantNo);
			while (pDB_->More())
			{
				CCustomParamInfo paramInfo;
				paramInfo.GetCustomParamInfo(pDB_);
				vCustomParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Dyn自定义参数数据条数 index :%d,%d",index,vCustomParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pDynChann->SetCustomParamInfo(0,index,&vCustomParamInfo[0]);
			vCustomParamInfo.clear();
		}
	}
}

void CPlantInfoOnLine::AddChannLocationsInfo(IDBInterFace *pDB_)
{
	int index=0;
	//添加振动测点位置信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		CChannLocations locationsInfo;
		//获取位置数据
		if (pVibChann->GetChannInfo()._iLocationIdx>-1)
		{
			SwitchDbNameEx(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pVibChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pVibChann->SetChannLocationsInfo(locationsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Vib测点位置数据条数 index :%d",index);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannLocationsInfo");
			}
		}
		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;
		CString lstrSqlGetBear;
		CString lstrTemp;
		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d ",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			// 			pDB_->Cmd("SELECT * FROM [%s]",gc_cBearing_Table);
			// 			pDB_->Cmd("WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{
				pVibChann->SetBearingParamInfo(bearParamInfro);
			}
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pVibChann->SetBearingParamInfo(bearParamInfro);
			}
		}

	}
	//添加动态测点位置信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		//获取位置数据
		CChannLocations locationsInfo;
		if (pDynChann->GetChannInfo()._iLocationIdx>-1)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pDynChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pDynChann->SetChannLocationsInfo(locationsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Dyn测点位置数据条数 index :%d",index);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannLocationsInfo");
			}
		}
		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;

		CString lstrSqlGetBear;
		CString lstrTemp;

		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{			
				pDynChann->SetBearingParamInfo(bearParamInfro);
			}

			// 			pDB_->Cmd("SELECT * FROM [%s]",gc_cBearing_Table);
			// 			pDB_->Cmd("WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pDynChann->SetBearingParamInfo(bearParamInfro);
			}
		}
	}

	//添加过程量测点位置信息
	V_PCHANNINFO::iterator itProc;
	CProcChannParam *pProcChann=NULL;
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{
		index=0;
		pProcChann=(CProcChannParam*)(*itProc);
		//获取位置数据
		CChannLocations locationsInfo;
		if (pProcChann->GetChannInfo()._iLocationIdx>-1)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cLocation_IDX,pProcChann->GetChannInfo()._iLocationIdx);
			if (pDB_->More())
			{
				locationsInfo.GetChannLocations(pDB_);
				pProcChann->SetChannLocationsInfo(locationsInfo);
				index++;
			}
		}

		//获取轴承参数信息
		CBearingParamInfo bearParamInfro;
		int iBearingTable=locationsInfo._iBearingTblType;

		CString lstrSqlGetBear;
		CString lstrTemp;

		if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
			{			
				pProcChann->SetBearingParamInfo(bearParamInfro);
			}
		}
		else
		{
			lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
			lstrSqlGetBear += lstrTemp;

			lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
			lstrSqlGetBear += lstrTemp;

			pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,_sPlantInfo._cCompany,gc_cFactory_Name,_sPlantInfo._cFactory,gc_cSetID,_sPlantInfo._cSetID,gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

			if (pDB_->More())
			{
				bearParamInfro._GetBearingParamInfo(pDB_);

				pProcChann->SetBearingParamInfo(bearParamInfro);
			}
		}
	}
}

void CPlantInfoOnLine::AddChannSectionsInfo(IDBInterFace *pDB_)
{
	int index=0;
	//添加振动测点位置信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParam*)(*itVib);
		//获取部位数据
		CChannSections sectionsInfo;
		/*if (pVibChann->GetChannInfo()._iCylinderNo >1000)*///大于1000为部位号
		{
			SwitchDbNameEx(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pVibChann->GetChannInfo()._iCylinderNo);
			if (pDB_->More())
			{
				sectionsInfo.GetChannSections(pDB_);
				pVibChann->SetChannSectionsInfo(sectionsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Vib测点部位数据条数 index :%d,_iGearBoxIDX:%d",index,sectionsInfo._iGearBoxIDX);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannSectionsInfo");
			}
		}
		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		CString lstrSql = pDB_->GetSql();
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pVibChann->SetGearBoxParamInfo(gearBoxInfo);
			//CString sTemp;sTemp.Format("振动测点齿轮箱参数:iGearNum12:%d,iGearNum21:%d,iGearNum22:%d,iGearNum31:%d",gearBoxInfo._iGearNumZ12,gearBoxInfo._iGearNumZ21,gearBoxInfo._iGearNumZ22,gearBoxInfo._iGearNumZ31);
			//CHZLogManage::Info(sTemp,"CPlantInfoOnLine","AddChannSectionsInfo");
		}
	}
	//添加动态测点位置信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParam *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParam*)(*itDyn);
		CChannSections sectionsInfo;
		//获取位置数据
		/*if (pDynChann->GetChannInfo()._iCylinderNo>1000)*/
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pDynChann->GetChannInfo()._iCylinderNo);
			if (pDB_->More())
			{
				sectionsInfo.GetChannSections(pDB_);
				pDynChann->SetChannSectionsInfo(sectionsInfo);
				index++;
				//CString strLog;strLog.Format("初始化获取Dyn测点部位数据条数 index :%d,_iGearBoxIDX:%d",index,sectionsInfo._iGearBoxIDX);
				//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddChannSectionsInfo");
			}
		}
		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pDynChann->SetGearBoxParamInfo(gearBoxInfo);
			//CString sTemp;sTemp.Format("动态测点齿轮箱参数:iGearNum12:%d,iGearNum21:%d,iGearNum22:%d,iGearNum31:%d",gearBoxInfo._iGearNumZ12,gearBoxInfo._iGearNumZ21,gearBoxInfo._iGearNumZ22,gearBoxInfo._iGearNumZ31);
			//CHZLogManage::Info(sTemp,"CPlantInfoOnLine","AddChannSectionsInfo");
		}
	}

	//添加过程量测点部位信息
	V_PCHANNINFO::iterator itProc;
	CProcChannParam *pProcChann=NULL;
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{
		index=0;
		pProcChann=(CProcChannParam*)(*itProc);
		CChannSections sectionsInfo;

		pDB_->Cmd("SELECT * from [%s] ",gc_cSection_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cSection_IDX,pProcChann->GetChannInfo()._iCylinderNo);
		if (pDB_->More())
		{
			sectionsInfo.GetChannSections(pDB_);
			pProcChann->SetChannSectionsInfo(sectionsInfo);
			index++;
		}

		//获取齿轮箱参数信息
		CGearBoxParamInfo gearBoxInfo;
		pDB_->Cmd("SELECT * FROM [%s]",gc_cGear_Box_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cGearBox_IDX,sectionsInfo._iGearBoxIDX);
		if (pDB_->More())
		{
			gearBoxInfo.GetGearBoxParamInfo(pDB_);
			pProcChann->SetGearBoxParamInfo(gearBoxInfo);

		}
	}
}

void CPlantInfoOnLine::AddVibChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int index=0;
	S_ChannInfo sChannInfo;
	CVibChannParam *pVibParam=NULL;
	CString strLog="";
	//设置数据处理连接

	_vVibChann.clear();
	_mapVibChannIndex.clear();
	_vAxisChann.clear();
	_vAxisChannValue.clear();
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d ",
		gc_cCompany,
		_sCompany.c_str(),
		gc_cPlantNo,
		_sPlantInfo._cPlantNo,
		gc_cChannType,GE_VIBCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;
	CString lstrQuery = pDB_->GetSql();

	zdlTraceLine(pDB_->GetSql());

	CHZLogManage::Info(pDB_->GetSql(),"CPlantInfo","AddVibChannInfo");

	strLog.Format("%s-%s-%s 添加径向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfo","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CVibChannParam;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND %s =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);
	pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

	strLog.Format("%s-%s-%s 添加轴向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CPlantInfo","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CVibChannParam;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if (_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL)
	{
		//得到轴心位置通道信息
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfo","AddVibChannInfo");
		CAxisChannParam *axisChannParam=NULL;
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannParam=new CAxisChannParam;
			axisChannParam->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannParam->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannParam);
			_vAxisChannValue.push_back(axisData);
		}
	}
	strLog.Format("%s-%s-%s 添加振动通道信息,振动数目%d,轴心位置数目%d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_vVibChann.size(),_vAxisChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfo","AddVibChannInfo");
}
void CPlantInfoOnLine::AddDynChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int index=0;
	S_ChannInfo sChannInfo;
	CDynChannParam *pDynParam=NULL;
	CString strLog="";
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	//振动通道
	pDB_->Cmd(" SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d)",gc_cCompany,_sCompany.c_str(), gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	CHZLogManage::Debug(pDB_->GetSql(),"CPlantInfoOnLine","AddDynChannInfo");

	while (pDB_->More())
	{
		pDynParam=new CDynChannParam;
		pDynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pDynParam->SetChannInfo(sChannInfo);
		_vDynChann.push_back(pDynParam);
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{
		//得到轴心位置通道信息
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s'  AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfo","AddDynChannInfo");
		CAxisChannParam *axisChannParam=NULL;
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannParam=new CAxisChannParam;
			axisChannParam->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannParam->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannParam);
			_vAxisChannValue.push_back(axisData);
		}
	}
	strLog.Format("%s-%s-%s/%s 添加动态通道信息 %d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_sPlantInfo._cPlantNo,_vDynChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfoOnLine","AddDynChannInfo");
}

void CPlantInfoOnLine::AddProcChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CProcChannParam *procParam=NULL;
	int index=0;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	S_ChannInfo sChannInfo;
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CProcChannParam;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
}

void CPlantInfoOnLine::AddChannInfoEnd(IDBInterFace * pDB_)
{
	_sPlantInfo._iVibChannNum = _vVibChann.size();
	_sPlantInfo._iProcChannNum= _vProcChann.size();
	_sPlantInfo._iDynChannNum = _vDynChann.size();
	_iAxesPairNum=_vAxisChann.size();
	V_PCHANNINFO::iterator it;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{

		if (((*it)->GetChannInfo())._iChannType==GE_VIBCHANN)
			++_iRadialVibChannNum;
		else
			++_iAxialVibChannNum;
	}
	return;
}

COleDateTime CPlantInfoOnLine::TimeLastUpdateTime() const
{
	return m_oTimeLastUpdateTime;
}

void CPlantInfoOnLine::TimeLastUpdateTime( COleDateTime val )
{
	m_oTimeLastUpdateTime = val;
}

float CPlantInfoOnLine::MaxRealRevLast() const
{
	long lnData = .0;
	InterlockedExchange(&lnData,this->m_lMaxRealRevLast);
	float lfData = int_as_float(lnData);
	TRACE(_T("****Rev:%f\r\n"),lfData);
	return lfData;
}

void CPlantInfoOnLine::MaxRealRevLast( float val )
{
	long lnData = float_as_int(val);
	InterlockedExchange(&this->m_lMaxRealRevLast,lnData);	

	
}

BOOL CPlantInfoOnLine::GetVibAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vVibChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockVibData->BeginRead();

	BEGIN_ERROR_HANDLE

	CVibChannParam * lpVib = (CVibChannParam*)this->_vVibChann.at(anChannIndex);

	if(lpVib)
	{
		aChannAlarmParam._bRegular = lpVib->_channInfo._chAlarmParam._bRegular;
		aChannAlarmParam._iRegJudgeType = lpVib->_channInfo._chAlarmParam._iRegJudgeType;
		aChannAlarmParam._fAlarmHH = lpVib->_channInfo._chAlarmParam._fAlarmHH;
		aChannAlarmParam._fAlarmHL = lpVib->_channInfo._chAlarmParam._fAlarmHL;
		aChannAlarmParam._fAlarmLH = lpVib->_channInfo._chAlarmParam._fAlarmLH;
		aChannAlarmParam._fAlarmLL = lpVib->_channInfo._chAlarmParam._fAlarmLL;
	}

	END_ERROR_HANDLE

	this->_pLockVibData->EndRead();

	return TRUE;
}

BOOL CPlantInfoOnLine::GetDynAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vDynChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockDynData->BeginRead();

	BEGIN_ERROR_HANDLE

	CDynChannParam * lpDyn = (CDynChannParam*)this->_vDynChann.at(anChannIndex);

	if(lpDyn)
	{
		aChannAlarmParam._bRegular = lpDyn->_channInfo._chAlarmParam._bRegular;
		aChannAlarmParam._iRegJudgeType = lpDyn->_channInfo._chAlarmParam._iRegJudgeType;
		aChannAlarmParam._fAlarmHH = lpDyn->_channInfo._chAlarmParam._fAlarmHH;
		aChannAlarmParam._fAlarmHL = lpDyn->_channInfo._chAlarmParam._fAlarmHL;
		aChannAlarmParam._fAlarmLH = lpDyn->_channInfo._chAlarmParam._fAlarmLH;
		aChannAlarmParam._fAlarmLL = lpDyn->_channInfo._chAlarmParam._fAlarmLL;
	}

	END_ERROR_HANDLE

	this->_pLockDynData->EndRead();

	return TRUE;
}

BOOL CPlantInfoOnLine::GetProcAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vProcChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockProcData->BeginRead();

	BEGIN_ERROR_HANDLE

	CProcChannParam * lpProc = (CProcChannParam*)this->_vProcChann.at(anChannIndex);

	if(lpProc)
	{
		aChannAlarmParam._bRegular = lpProc->_channInfo._chAlarmParam._bRegular;
		aChannAlarmParam._iRegJudgeType = lpProc->_channInfo._chAlarmParam._iRegJudgeType;
		aChannAlarmParam._fAlarmHH = lpProc->_channInfo._chAlarmParam._fAlarmHH;
		aChannAlarmParam._fAlarmHL = lpProc->_channInfo._chAlarmParam._fAlarmHL;
		aChannAlarmParam._fAlarmLH = lpProc->_channInfo._chAlarmParam._fAlarmLH;
		aChannAlarmParam._fAlarmLL = lpProc->_channInfo._chAlarmParam._fAlarmLL;
	}

	END_ERROR_HANDLE

	this->_pLockProcData->EndRead();

	return TRUE;
}

BOOL CPlantInfoOnLine::GetVibGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vVibChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockVibData->BeginRead();

	BEGIN_ERROR_HANDLE

		CVibChannParam * lpVib = (CVibChannParam*)this->_vVibChann.at(anChannIndex);

	if(lpVib)
	{
		aChannAlarmParam._fAlarm1 = lpVib->_channInfo._chAlarmParam._fAlarm1;
		aChannAlarmParam._fAlarm2 = lpVib->_channInfo._chAlarmParam._fAlarm2;
		aChannAlarmParam._fAlarm3 = lpVib->_channInfo._chAlarmParam._fAlarm3;
	}

	END_ERROR_HANDLE

		this->_pLockVibData->EndRead();

	return TRUE;
}

BOOL CPlantInfoOnLine::GetDynGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vDynChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockDynData->BeginRead();

	BEGIN_ERROR_HANDLE

		CDynChannParam * lpDyn = (CDynChannParam*)this->_vDynChann.at(anChannIndex);

	if(lpDyn)
	{
		aChannAlarmParam._fAlarm1 = lpDyn->_channInfo._chAlarmParam._fAlarm1;
		aChannAlarmParam._fAlarm2 = lpDyn->_channInfo._chAlarmParam._fAlarm2;
		aChannAlarmParam._fAlarm3 = lpDyn->_channInfo._chAlarmParam._fAlarm3;
	}

	END_ERROR_HANDLE

		this->_pLockDynData->EndRead();

	return TRUE;
}

BOOL CPlantInfoOnLine::GetProcGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex < 0 || anChannIndex >= this->_vProcChann.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	this->_pLockProcData->BeginRead();

	BEGIN_ERROR_HANDLE

		CProcChannParam * lpProc = (CProcChannParam*)this->_vProcChann.at(anChannIndex);

	if(lpProc)
	{
		aChannAlarmParam._fAlarm1 = lpProc->_channInfo._chAlarmParam._fAlarm1;
		aChannAlarmParam._fAlarm2 = lpProc->_channInfo._chAlarmParam._fAlarm2;
		aChannAlarmParam._fAlarm3 = lpProc->_channInfo._chAlarmParam._fAlarm3;
	}

	END_ERROR_HANDLE

		this->_pLockProcData->EndRead();

	return TRUE;
}


int CPlantInfoOnLine::CalcVibDataGrade( S_VibCharValue *pVibCharValue_,int iLen_,int iStartIndex_ )
{

	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
	{
		return 1;
	}
	CChannAlarmParam lcChannAlarmParam;

	if(this->GetPlantInfo()._iMachineType != GE_MACHINETYPE_KEYPUMP
		&& this->GetPlantInfo()._iMachineType != GE_MACHINETYPE_PUMP)
	{
		return 1;
	}

	for(int loop=iStartIndex_;loop<(iStartIndex_ + iLen_);++loop)
	{
		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE
			lcChannAlarmParam = ((CVibChannParam*)_vVibChann[loop])->GetChannInfo()._chAlarmParam;
		END_ERROR_HANDLE
			_pLockVibData->EndRead();

		int lnUseLevel = 0;

		if(lcChannAlarmParam._fAlarm1 < 0.2 && lcChannAlarmParam._fAlarm2 < 0.2 && lcChannAlarmParam._fAlarm3 < 0.2)
		{
			continue;
		}

		if( lcChannAlarmParam._fAlarm1 > 0.2)
		{
			lnUseLevel = 1;
		}

		if( lcChannAlarmParam._fAlarm2 > 0.2 && lcChannAlarmParam._fAlarm2 > lcChannAlarmParam._fAlarm1)
		{
			lnUseLevel = 2;
		}

		if(lcChannAlarmParam._fAlarm3 > 0.2 && lcChannAlarmParam._fAlarm3 > lcChannAlarmParam._fAlarm2 && lcChannAlarmParam._fAlarm2 > lcChannAlarmParam._fAlarm1)
		{
			lnUseLevel = 3;
		}

		if(lnUseLevel <= 0)
		{
			continue;
		}

		float lfCharValue = pVibCharValue_[loop - iStartIndex_]._fCharAll;

		switch(lnUseLevel)
		{
		case 1:
			if(lfCharValue > lcChannAlarmParam._fAlarm1)
			{
				pVibCharValue_->_fCharRes = 2;
			}
			else
			{
				pVibCharValue_->_fCharRes = 1;
			}
			break;
		case 2:
			if(lfCharValue > lcChannAlarmParam._fAlarm2)
			{
				pVibCharValue_->_fCharRes = 3;
			}
			else if(lfCharValue > lcChannAlarmParam._fAlarm1 && lfCharValue <= lcChannAlarmParam._fAlarm2)
			{
				pVibCharValue_->_fCharRes = 2;
			}
			else
			{
				pVibCharValue_->_fCharRes = 1;
			}
			break;
		case 3:
			if(lfCharValue > lcChannAlarmParam._fAlarm3)
			{
				pVibCharValue_->_fCharRes = 4;
			}
			else if(lfCharValue > lcChannAlarmParam._fAlarm2 && lfCharValue <= lcChannAlarmParam._fAlarm3)
			{
				pVibCharValue_->_fCharRes = 3;
			}
			else if(lfCharValue > lcChannAlarmParam._fAlarm1 && lfCharValue <= lcChannAlarmParam._fAlarm2)
			{
				pVibCharValue_->_fCharRes = 2;
			}
			else
			{
				pVibCharValue_->_fCharRes = 1;
			}
			break;
		default:
			break;
		}
	}

	return 1;
}

int CPlantInfoOnLine::GetRealRevFromChannel( float *pfRev_, int iLen_ )
{
	if(iLen_ != 5)
	{
		return -1;
	}
	for(int n = 0; n < iLen_; n++)
	{
		pfRev_[n] = -1;
	}

	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(this->_vVibChann.size() > 0)
	{
		for(int n = 0; n < _vVibChann.size(); ++n)
		{
			CVibChannParam *pChannParam = (CVibChannParam*)_vVibChann[n];
			if(!pChannParam || !pChannParam->GetChannUpdate())
			{
				continue;
			}

			int lnRev = pChannParam->GetCharValueEx()._iRev;
			switch(pChannParam->_channInfo._iRevType)
			{
			case 1:
				if(pfRev_[0] < lnRev)
				{
					pfRev_[0] = lnRev;
				}
				break;
			case 2:
				if(pfRev_[1] < lnRev)
				{
					pfRev_[1] = lnRev;
				}
				break;
			case 3:
				if(pfRev_[2] < lnRev)
				{
					pfRev_[2] = lnRev;
				}
				break;
			case 4:
				if(pfRev_[3] < lnRev)
				{
					pfRev_[3] = lnRev;
				}
				break;
			case 5:
				if(pfRev_[4] < lnRev)
				{
					pfRev_[4] = lnRev;
				}
				break;
			default:
				if(pfRev_[0] < lnRev)
				{
					pfRev_[0] = lnRev;
				}
				break;
			}
		}
	}
	END_ERROR_HANDLE
	_pLockVibData->EndRead();

	_pLockDynData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(this->_vDynChann.size() > 0)
	{
		for(int n = 0; n < _vDynChann.size(); ++n)
		{
			CDynChannParam *pChannParam = (CDynChannParam*)_vDynChann[n];
			if(!pChannParam || !pChannParam->GetChannUpdate())
			{
				continue;
			}
			
			int lnRev = pChannParam->GetCharValueEx()._iRev;
			switch(pChannParam->_channInfo._iRevType)
			{
			case 1:
				if(pfRev_[0] < lnRev)
				{
					pfRev_[0] = lnRev;
				}
				break;
			case 2:
				if(pfRev_[1] < lnRev)
				{
					pfRev_[1] = lnRev;
				}
				break;
			case 3:
				if(pfRev_[2] < lnRev)
				{
					pfRev_[2] = lnRev;
				}
				break;
			case 4:
				if(pfRev_[3] < lnRev)
				{
					pfRev_[3] = lnRev;
				}
				break;
			case 5:
				if(pfRev_[4] < lnRev)
				{
					pfRev_[4] = lnRev;
				}
				break;
			default:
				if(pfRev_[0] < lnRev)
				{
					pfRev_[0] = lnRev;
				}
				break;
			}
		}
	}
	END_ERROR_HANDLE
	_pLockDynData->EndRead();

	return 1;
}
int CRealPlantInfo::GetSpecChannIndex(string sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_/* =GE_ALLCHAR */)
{
	int iCharType=0;
	int iPhaseType=0;
	switch(iCharType_)
	{
	case GE_ALLCHAR://因为目前没有计算通频相位，所以用一倍频替代
		iCharType=GE_ALLCHAR;
		iPhaseType=GE_PHASECHARONE;
		break;
	case GE_ONECHAR:
	case GE_PHASECHARONE:
		iCharType=GE_ONECHAR;
		iPhaseType=GE_PHASECHARONE;
		break;
	case GE_TWOCHAR:
	case GE_PHASECHARTWO:
		iCharType=GE_TWOCHAR;
		iPhaseType=GE_PHASECHARTWO;
		break;
	case GE_THREECHAR:
	case GE_PHASECHARTHREE:
		iCharType=GE_THREECHAR;
		iPhaseType=GE_PHASECHARTHREE;
		break;
	case GE_HALFCHAR:
	case GE_PHASECHARHALF:
		iCharType=GE_HALFCHAR;
		iPhaseType=GE_PHASECHARHALF;
		break;
	default://缺省情况下，就是用一倍频的幅值和相位
		iCharType=GE_ONECHAR;
		iPhaseType=GE_PHASECHARONE;
	}
	int index=GetSpecVibChannIndex(sChannID_);
	if (index>-1)
	{
		CVibChannParam *vibChannParam=(CVibChannParam*)_vVibChann[index];
		S_ChannInfo channInfo=vibChannParam->GetChannInfo();
		_pLockVibData->BeginRead();
		try
		{
			fAmp_=vibChannParam->GetCharValueEx().GetSpecTypeData(iCharType);
			fPhase_=vibChannParam->GetCharValueEx().GetSpecTypeData(iPhaseType);
			iSmpFreq_=vibChannParam->GetCharValue()._iSmpFreq;
			iSmpNum_=vibChannParam->GetCharValue()._iSmpNum;
			int iRev=vibChannParam->GetCharValue()._iRev;
			fRev_ = (float)iRev;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndRead();
	}
	return index;
}


int CRealPlantInfo::GetSpecChannIndex(int index_,string &sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_/* =GE_ALLCHAR */)
{
	if ((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
		return -1;
	S_ChannInfo channInfo=_vVibChann[index_]->GetChannInfo();
	sChannID_=channInfo._cChannID;
	return GetSpecChannIndex(sChannID_,fRev_,fPhase_,fAmp_,iSmpNum_,iSmpFreq_,iCharType_);
}

bool CRealPlantInfo::GetStatusType( E_SUD_TYPE & iType_,int & iEventID_ )
{
	bool bStatus=false;
	_pLockVibData->BeginRead();
	bStatus=_bSudStatus;
	iType_=_iType;
	iEventID_=_iCurrEventID;
	_pLockVibData->EndRead();
	return bStatus;
}

//快变数据保存
int CRealPlantInfo::SaveVibRecord(IDBInterFace * pDB_, 
									string sTableName_, 
									int iCurID_, 
									DATATYPE_WAVE *pWave_, 
									S_VibCharValue *pVibCharValue_, 
									S_CustomParamData *pfCustomParom_, 
									int iAllRealNum_, 
									int iLen_, 
									CBHDateTime & aoDataTime,
									int iEventID_,
									int iStartIndex_,
									const float& fScaleCoef_,
									int anPrecisionEnabled,
									std::vector<float>& afPreciousMin ,
									std::vector<float>&  afPreciousMax,
									bool bSaveWave_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	int nloop=0;
	int iNums=iLen_+iStartIndex_;

	//得到采样频率，采样点数和转速
	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	int   pnRevFlag[5] = {0,0,0,0,0};

	iSmpFreq=pVibCharValue_[nCount]._iSmpFreq;
	iSmpNum=pVibCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=_vVibChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[0] = 1;
			break;
		case 2:
			pfRev[1] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[1] = 1;
			break;
		case 3:
			pfRev[2] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[2] = 1;
			break;
		case 4:
			pfRev[3] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[3] = 1;
			break;
		case 5:
			pfRev[4] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[4] = 1;
			break;
		default:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
			pnRevFlag[0] = 1;
		}
		++nCount;
	}
	S_VibCharValue vibCharValue;
	CVibChannParam *pVibChannParam;
	CString strSql;//构造SQL语句。

	if(g_bIsDiskFull && iStartIndex_ + iLen_ == _sPlantInfo._iVibChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iLen_; n++)
			{
				lnTotalWavePtNum += pVibCharValue_[n]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < iStartIndex_; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = %d",lstrSelField,sTableName_.c_str(),gc_cID,iCurID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iLen_; n++)
			{
				lnTotalWavePtNum += pVibCharValue_[n]._iSmpNum;
			}
		}

		MakeSureVibSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,iStartIndex_);
	}

	

	//构造查询这条趋势记录是否已经存在

	CBHDateTime loDateTime;

	if(bSaveWave_)
	{
		strSql.Format("select  %s from [%s] where [%s]=%d ",gc_cTime,sTableName_.c_str(),gc_cID,iCurID_);
		pDB_->Cmd(strSql);
		if(pDB_->More())
		{
			loDateTime = pDB_->GetTime(gc_cTime);

			this->RemoveSpecTime(pDB_,sTableName_);
			if(!SaveSpecTimeData(pDB_,sTableName_,loDateTime,iCurID_))
			{
				string sChann;
				int index=0;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (nloop=0;nloop<iLen_;++nloop)
				{
					vibCharValue=pVibCharValue_[nloop];
					pVibChannParam=(CVibChannParam*)(_vVibChann[nloop + iStartIndex_]);
					sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ALLCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ONECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_TWOCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_THREECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_HALFCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_RESCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARONE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTWO));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTHREE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARHALF));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_DCCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibCharValue.GetSpecTypeData(GE_GAP));
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibCharValue._iSmpFreq);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibCharValue._iSmpNum);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibCharValue._iRev);
					pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,TimeConvertString(aoDataTime));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				if(pnRevFlag[0] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				}
				if(pnRevFlag[1] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				}
				if(pnRevFlag[2] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				}
				if(pnRevFlag[3] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				}
				if(pnRevFlag[4] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
				}
				pDB_->Cmd(" [%s]=%d, ",gc_cEventID,iEventID_);
				pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (!(pDB_->Exec()))
				{
					iRtn= -2;
					goto QUIT_SAVEFTREND;
				}

				pDB_->Cmd("SELECT 1");
				for (nloop=iStartIndex_;nloop<iNums;++nloop)
				{	
					pDB_->Cmd(",%s",_vVibWaveField[nloop]);
				}
				pDB_->Cmd(" FROM [%s] ",sTableName_.c_str());
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (pDB_->More(TRUE))
				{
					string sChann;
					int iRealNum=0;
					CWaveHead lsHead;
					lsHead.SetLength(4);
					for (nloop=0;nloop<iLen_;++nloop)
					{
						sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
						pVibChannParam = (CVibChannParam *)(_vVibChann[nloop + iStartIndex_]);

						vector<BYTE> lvWave;

						if(!IsFloatZero(fScaleCoef_))
						{
							lsHead.m_pParam[0] = fScaleCoef_;
						}
						if(anPrecisionEnabled)
						{
							lsHead.m_pParam[1] = 1;
							lsHead.m_pParam[2] = afPreciousMin[nloop];
							lsHead.m_pParam[3] = afPreciousMax[nloop];
						}
						else
						{
							lsHead.m_pParam[1] = 0;
						}

						int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pVibCharValue_[nloop]._iSmpNum,lsHead,lvWave);

						if(iRealNum + pVibCharValue_[nloop]._iSmpNum <= iAllRealNum_)
						{
							if(!(pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
							{
								BHLOG_ERROR(STR("快变数据保存，保存波形失败"));
							}
						}

						pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
						iRealNum+=pVibCharValue_[nloop]._iSmpNum;
						iRtn=nloop+1;
					}
				}
				return 1;
			}
			
		}
		//插入记录
		pDB_->ClearSql();
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s ",sTableName_.c_str(),gc_cID);

		for(nloop = iStartIndex_; nloop < iNums; ++nloop)
		{
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" VALUES (%d",iCurID_);

		for (nloop=0;nloop<iLen_;++nloop)
		{
			vibCharValue=pVibCharValue_[nloop];
			pDB_->Cmd(",%.2f",vibCharValue._fCharAll);
			pDB_->Cmd(",%.2f",vibCharValue._fCharOne);
			pDB_->Cmd(",%.2f",vibCharValue._fCharTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fCharThree);
			pDB_->Cmd(",%.2f",vibCharValue._fCharHalf);
			pDB_->Cmd(",%.2f",vibCharValue._fCharRes);
			pDB_->Cmd(",%.2f",vibCharValue._fDC);
			pDB_->Cmd(",%.2f",vibCharValue._fGap);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseOne);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseThree);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseHalf);
			pDB_->Cmd(",%d",vibCharValue._iSmpFreq);
			pDB_->Cmd(",%d",vibCharValue._iSmpNum);
			pDB_->Cmd(",%d",vibCharValue._iRev);
			pDB_->Cmd(",'%s'",_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(",%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",TimeConvertString(aoDataTime));
		pDB_->Cmd("%d,",iEventID_);
		pDB_->Cmd("%d)",0);

		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFTREND;
		}
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{	
			pDB_->Cmd(",%s",_vVibWaveField[nloop]);
		}
		pDB_->Cmd(" FROM [%s] ",sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (pDB_->More(TRUE))
		{
			string sChann;
			int iRealNum=0;
			CWaveHead lsHead;
			lsHead.SetLength(4);

			for (nloop=0;nloop<iLen_;++nloop)
			{
				sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
				pVibChannParam = (CVibChannParam *)(_vVibChann[nloop + iStartIndex_]);

				vector<BYTE> lvWave;

				if(!IsFloatZero(fScaleCoef_))
				{
					lsHead.m_pParam[0] = fScaleCoef_;
				}
				if(anPrecisionEnabled)
				{
					lsHead.m_pParam[1] = 1;
					lsHead.m_pParam[2] = afPreciousMin[nloop];
					lsHead.m_pParam[3] = afPreciousMax[nloop];
				}
				else
				{
					lsHead.m_pParam[1] = 0;
				}

				int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pVibCharValue_[nloop]._iSmpNum,lsHead,lvWave);


				if(iRealNum + pVibCharValue_[nloop]._iSmpNum <= iAllRealNum_)
				{
					if(!(pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
					{
						BHLOG_ERROR(STR("快变数据保存，保存波形失败"));
					}
				}

				pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
				iRealNum+=pVibCharValue_[nloop]._iSmpNum;
				iRtn=nloop+1;
			}
		}
	}
	else
	{
		strSql.Format("select  %s from [%s] where [%s]=%d ",gc_cTime,sTableName_.c_str(),gc_cID,iCurID_);
		pDB_->Cmd(strSql);
		if(pDB_->More())
		{
			loDateTime = pDB_->GetTime(gc_cTime);
			this->RemoveSpecTime(pDB_,sTableName_);
			if(!SaveSpecTimeData(pDB_,sTableName_,loDateTime,iCurID_))
			{
				string sChann;
				int index=0;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (nloop=0;nloop<iLen_;++nloop)
				{
					vibCharValue=pVibCharValue_[nloop];
					pVibChannParam=(CVibChannParam*)(_vVibChann[nloop + iStartIndex_]);
					sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ALLCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ONECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_TWOCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_THREECHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_HALFCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_RESCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARONE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTWO));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTHREE));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARHALF));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_DCCHAR));
					pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibCharValue.GetSpecTypeData(GE_GAP));
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibCharValue._iSmpFreq);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibCharValue._iSmpNum);
					pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibCharValue._iRev);
					pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
					pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
					pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());

				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,TimeConvertString(aoDataTime));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				if(pnRevFlag[0] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				}
				if(pnRevFlag[1] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				}
				if(pnRevFlag[2] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				}
				if(pnRevFlag[3] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				}
				if(pnRevFlag[4] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
				}	
				pDB_->Cmd(" [%s]=%d, ",gc_cEventID,iEventID_);
				pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (!(pDB_->Exec()))
				{
					iRtn= -2;
					goto QUIT_SAVEFTREND;
				}
			}

			return 1;

		}
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s ",sTableName_.c_str(),gc_cID);

		for(nloop = iStartIndex_; nloop < iNums; ++nloop)
		{
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);


		pDB_->Cmd(" VALUES (%d",iCurID_);

		for (nloop=0;nloop<iLen_;++nloop)
		{
			vibCharValue=pVibCharValue_[nloop];
			pDB_->Cmd(",%.2f",vibCharValue._fCharAll);
			pDB_->Cmd(",%.2f",vibCharValue._fCharOne);
			pDB_->Cmd(",%.2f",vibCharValue._fCharTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fCharThree);
			pDB_->Cmd(",%.2f",vibCharValue._fCharHalf);
			pDB_->Cmd(",%.2f",vibCharValue._fCharRes);
			pDB_->Cmd(",%.2f",vibCharValue._fDC);
			pDB_->Cmd(",%.2f",vibCharValue._fGap);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseOne);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseThree);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseHalf);
			pDB_->Cmd(",%d",vibCharValue._iSmpFreq);
			pDB_->Cmd(",%d",vibCharValue._iSmpNum);
			pDB_->Cmd(",%d",vibCharValue._iRev);
			pDB_->Cmd(",'%s'",_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(",%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (int nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",TimeConvertString(aoDataTime));
		pDB_->Cmd("%d,",iEventID_);
		pDB_->Cmd("%d)",0);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFTREND;
		}
		else
		{
			iRtn++;
		}
	}

	{
		LY::PLANTINFO_STATUS loStatus;
		this->_pLockVibData->BeginRead();
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		CBHDateTime loCurrentTime;	
		loCurrentTime = loCurrentTime.GetCurrentTimeEx();
		loStatus.m_strVibSaveTime = loCurrentTime.Format();
		this->_pLockVibData->EndRead();
		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}
	}
	
QUIT_SAVEFTREND:
	return iRtn;
}

int CRealPlantInfo::SaveDynRecord(IDBInterFace * pDB_, 
									string sTableName_, 
									int iCurID_, 
									DATATYPE_WAVE *pWave_, 
									S_DynCharValue *pDynCharValue_, 
									S_CustomParamData *pfCustomParom_, 
									int iAllRealNum_, 
									int iLen_, 
									CBHDateTime & aoDataTime, 
									int iEventID_,
									int iStartIndex_,
									const float& fScaleCoef_,
									int anPrecisionEnabled,
									std::vector<float>& afPreciousMin ,
									std::vector<float>&  afPreciousMax,
									bool bSaveWave_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	int nloop=0;
	int iNums=iLen_+iStartIndex_;

	//得到采样频率，采样点数和转速
	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	int   pnRevFlag[5] = {0,0,0,0,0};

	iSmpFreq=pDynCharValue_[nCount]._iSmpFreq;
	iSmpNum=pDynCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=_vDynChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			pfRev[0] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[0] = 1;
			break;
		case 2:
			pfRev[1] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[1] = 1;
			break;
		case 3:
			pfRev[2] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[2] = 1;
			break;
		case 4:
			pfRev[3] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[3] = 1;
			break;
		case 5:
			pfRev[4] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[4] = 1;
			break;
		default:
			pfRev[0] = pDynCharValue_[nCount]._iRev;
			pnRevFlag[0] = 1;
		}
		++nCount;
	}
	S_DynCharValue dynCharValue;
	CDynChannParam *pDynChannParam;
	CString strSql;//构造SQL语句。

	CBHDateTime loDateTime;

	if(g_bIsDiskFull && iStartIndex_ + iLen_ == _sPlantInfo._iDynChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iLen_; n++)
			{
				lnTotalWavePtNum += pDynCharValue_[n]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < iStartIndex_; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vDynChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = %d",lstrSelField,sTableName_.c_str(),gc_cID,iCurID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vDynChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iLen_; n++)
			{
				lnTotalWavePtNum += pDynCharValue_[n]._iSmpNum;
			}
		}

		MakeSureDynSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,iStartIndex_);
	}


	//构造查询这条趋势记录是否已经存在
	if(bSaveWave_)
	{
		strSql.Format("select %s from [%s] where [%s]=%d ",gc_cTime,sTableName_.c_str(),gc_cID,iCurID_);
		pDB_->Cmd(strSql);
		if(pDB_->More())
		{
			loDateTime = pDB_->GetTime(gc_cTime);
			this->RemoveSpecTime(pDB_,sTableName_);
			if(!SaveSpecTimeData(pDB_,sTableName_,loDateTime,iCurID_))
			{
				string sChann;
				int index=0;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (nloop=0;nloop<iLen_;++nloop)
				{
					dynCharValue=pDynCharValue_[nloop];
					pDynChannParam=(CDynChannParam*)(_vDynChann[nloop + iStartIndex_]);
					sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),dynCharValue.GetSpecTypeData(0));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),dynCharValue.GetSpecTypeData(1));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),dynCharValue.GetSpecTypeData(2));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),dynCharValue.GetSpecTypeData(3));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),dynCharValue.GetSpecTypeData(4));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),dynCharValue.GetSpecTypeData(5));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),dynCharValue.GetSpecTypeData(6));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),dynCharValue.GetSpecTypeData(7));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),dynCharValue.GetSpecTypeData(8));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),dynCharValue.GetSpecTypeData(9));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),dynCharValue.GetSpecTypeData(10));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),dynCharValue.GetSpecTypeData(11));
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),dynCharValue._iSmpFreq);
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),dynCharValue._iSmpNum);
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),dynCharValue._iRev);
					pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,TimeConvertString(aoDataTime));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				if(pnRevFlag[0] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				}
				if(pnRevFlag[1] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				}
				if(pnRevFlag[2] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				}
				if(pnRevFlag[3] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				}
				if(pnRevFlag[4] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
				}
				
				pDB_->Cmd(" [%s]=%d,",gc_cEventID,iEventID_);
				pDB_->Cmd(" [%s]=%d",gc_cSynFlag,0);
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (!(pDB_->Exec()))
				{
					iRtn= -2;
					goto QUIT_SAVEFTREND;
				}

				pDB_->Cmd("SELECT 1");
				for (nloop=iStartIndex_;nloop<iNums;++nloop)
					pDB_->Cmd(",%s",_vDynWaveField[nloop]);
				pDB_->Cmd(" FROM [%s] ",sTableName_.c_str());
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (pDB_->More(TRUE))
				{
					string sChann;
					int iRealNum=0;
					CWaveHead lsHead;
					lsHead.SetLength(4);
					for (nloop=0;nloop<iLen_;++nloop)
					{
						sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
						pDynChannParam = (CDynChannParam *)(_vDynChann[nloop + iStartIndex_]);
						if(iRealNum + pDynCharValue_[nloop]._iSmpNum <= iAllRealNum_)
						{
							vector<BYTE> lvWave;

							if(!IsFloatZero(fScaleCoef_))
							{
								lsHead.m_pParam[0] = fScaleCoef_;
							}
							if(anPrecisionEnabled)
							{
								lsHead.m_pParam[1] = 1;
								lsHead.m_pParam[2] = afPreciousMin[nloop];
								lsHead.m_pParam[3] = afPreciousMax[nloop];
							}
							else
							{
								lsHead.m_pParam[1] = 0;
							}

							int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pDynCharValue_[nloop]._iSmpNum,lsHead,lvWave);

							if(!(pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
							{
								BHLOG_ERROR(STR("快变数据保存，保存波形失败"));
							}
						}

						pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
						iRealNum+=pDynCharValue_[nloop]._iSmpNum;
						iRtn=nloop+1;
					}
				}
				return 1;
			}
			
		}

		pDB_->ClearSql();
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s",sTableName_.c_str(),gc_cID);

		for(nloop = iStartIndex_; nloop < iNums; ++nloop)
		{
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);



		pDB_->Cmd(" VALUES (%d",iCurID_);

		for (nloop=0;nloop<iLen_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue0);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue1);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue2);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue3);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue4);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue5);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue6);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue7);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue8);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue9);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue10);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue11);
			pDB_->Cmd(",%d",dynCharValue._iSmpFreq);
			pDB_->Cmd(",%d",dynCharValue._iSmpNum);
			pDB_->Cmd(",%d",dynCharValue._iRev);
			pDB_->Cmd(",'%s'",_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(",%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",TimeConvertString(aoDataTime));
		pDB_->Cmd("%d,",iEventID_);
		pDB_->Cmd("%d)",0);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFTREND;
		}
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
			pDB_->Cmd(",%s",_vDynWaveField[nloop]);
		pDB_->Cmd(" FROM [%s] ",sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (pDB_->More(TRUE))
		{
			string sChann;
			int iRealNum=0;
			CWaveHead lsHead;
			lsHead.SetLength(4);
			for (nloop=0;nloop<iLen_;++nloop)
			{
				sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
				pDynChannParam = (CDynChannParam *)(_vDynChann[nloop + iStartIndex_]);
				if(iRealNum + pDynCharValue_[nloop]._iSmpNum <= iAllRealNum_)
				{
					vector<BYTE> lvWave;

					if(!IsFloatZero(fScaleCoef_))
					{
						lsHead.m_pParam[0] = fScaleCoef_;
					}
					if(anPrecisionEnabled)
					{
						lsHead.m_pParam[1] = 1;
						lsHead.m_pParam[2] = afPreciousMin[nloop];
						lsHead.m_pParam[3] = afPreciousMax[nloop];
					}
					else
					{
						lsHead.m_pParam[1] = 0;
					}

					int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pDynCharValue_[nloop]._iSmpNum,lsHead,lvWave);

					if(!(pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
					{
						BHLOG_ERROR(STR("快变数据保存，保存波形失败"));
					}
				}

				pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
				iRealNum+=pDynCharValue_[nloop]._iSmpNum;
				iRtn=nloop+1;
			}
		}
	}
	else
	{
		strSql.Format("select top 1 %s from [%s] where [%s]=%d ",gc_cTime,sTableName_.c_str(),gc_cID,iCurID_);
		pDB_->Cmd(strSql);
		if(pDB_->More())
		{
			
			loDateTime = pDB_->GetTime(gc_cTime);
			this->RemoveSpecTime(pDB_,sTableName_);
			if(!SaveSpecTimeData(pDB_,sTableName_,loDateTime,iCurID_))
			{
				string sChann;
				int index=0;
				pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
				for (nloop=0;nloop<iLen_;++nloop)
				{
					dynCharValue=pDynCharValue_[nloop];
					pDynChannParam=(CDynChannParam*)(_vDynChann[nloop + iStartIndex_]);
					sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),dynCharValue.GetSpecTypeData(0));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),dynCharValue.GetSpecTypeData(1));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),dynCharValue.GetSpecTypeData(2));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),dynCharValue.GetSpecTypeData(3));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),dynCharValue.GetSpecTypeData(4));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),dynCharValue.GetSpecTypeData(5));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),dynCharValue.GetSpecTypeData(6));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),dynCharValue.GetSpecTypeData(7));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),dynCharValue.GetSpecTypeData(8));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),dynCharValue.GetSpecTypeData(9));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),dynCharValue.GetSpecTypeData(10));
					pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),dynCharValue.GetSpecTypeData(11));
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),dynCharValue._iSmpFreq);
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),dynCharValue._iSmpNum);
					pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),dynCharValue._iRev);
					pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
					pDB_->Cmd(" [%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-1).c_str());
					pDB_->Cmd(" [%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-7).c_str());

				}
				pDB_->Cmd(" [%s]='%s', ",gc_cTime,TimeConvertString(aoDataTime));
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
				pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
				if(pnRevFlag[0] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
				}
				if(pnRevFlag[1] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
				}
				if(pnRevFlag[2] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
				}
				if(pnRevFlag[3] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
				}
				if(pnRevFlag[4] == 1)
				{
					pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
				}
				pDB_->Cmd(" [%s]=%d,",gc_cEventID,iEventID_);
				pDB_->Cmd(" [%s]=%d",gc_cSynFlag,0);
				pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
				if (!(pDB_->Exec()))
				{
					iRtn= -2;
					goto QUIT_SAVEFTREND;
				}
				return 1;
			}
			
		}
		
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s",sTableName_.c_str(),gc_cID);

		for(nloop = iStartIndex_; nloop < iNums; ++nloop)
		{
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);



		pDB_->Cmd(" VALUES (%d",iCurID_);

		for (nloop=0;nloop<iLen_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue0);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue1);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue2);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue3);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue4);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue5);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue6);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue7);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue8);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue9);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue10);
			pDB_->Cmd(",%.2f",dynCharValue._fCharValue11);
			pDB_->Cmd(",%d",dynCharValue._iSmpFreq);
			pDB_->Cmd(",%d",dynCharValue._iSmpNum);
			pDB_->Cmd(",%d",dynCharValue._iRev);
			pDB_->Cmd(",'%s'",_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(",%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (int nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",TimeConvertString(aoDataTime));
		pDB_->Cmd("%d,",iEventID_);
		pDB_->Cmd("%d)",0);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFTREND;
		}
		else
		{
			iRtn++;
		}
		
	}

	{
		LY::PLANTINFO_STATUS loStatus;
		this->_pLockDynData->BeginRead();
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		CBHDateTime loCurrentTime;	
		loCurrentTime = loCurrentTime.GetCurrentTimeEx();
		loStatus.m_strDynSaveTime = loCurrentTime.Format();
		this->_pLockDynData->EndRead();
		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}
	}

	
QUIT_SAVEFTREND:
	return iRtn;
}

int CRealPlantInfo::SaveProcRecord(IDBInterFace *pDB_, 
									 string sTableName_, 
									 int iCurID_, 
									 float *pfData_, 
									 int iLen_, 
									 const CBHDateTime& aoDateTime_,
									 int iEventID_,
									 int iStartIndex_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	float pfRev[5];
	float fData=.0;
	CString strTimeCurr;
	GetRealRev( pfRev, 5);

	//得到断网数据当前的时间
	CBHDateTime dtCurr = aoDateTime_;
	strTimeCurr =CPlantInfoBase::TimeConvertString(dtCurr);

	int nloop=0;


	CProcChannParam *pProcChann=NULL;
	CBHDateTime loDateTime;
	pDB_->Cmd("SELECT top 1 [%s],[%s] FROM [%s]",gc_cID,gc_cTime,sTableName_.c_str());
	pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
	if(pDB_->More())
	{
		loDateTime = pDB_->GetTime(gc_cTime);
		this->RemoveSpecTime(pDB_,sTableName_);
		if(!this->SaveSpecTimeData(pDB_,sTableName_,loDateTime,iCurID_))
		{
			pDB_->Cmd("UPDATE [%s] SET  ",sTableName_.c_str());
			for(nloop = 0; nloop < iLen_; ++nloop)
			{
				pProcChann=(CProcChannParam*)(_vProcChann[nloop + iStartIndex_]);
				pDB_->Cmd(" [%s]=%.2f,",pProcChann->GetChannInfo()._cChannNo,pfData_[nloop]);
			}
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
			pDB_->Cmd(" [%s]='%s', ",gc_cTime,strTimeCurr);
			pDB_->Cmd(" [%s]=%d,",gc_cEventID,iEventID_);
			pDB_->Cmd(" [%s]=%d",gc_cSynFlag,0);
			pDB_->Cmd("  WHERE [%s]=%d",gc_cID,iCurID_);
			//		TRACE("proc trend save:%s\n ",pDB_->GetSql());
			if(!pDB_->Exec())
				return -1;
		}

		return 1;
		
	}

	
	pDB_->Cmd("INSERT INTO [%s](%s ",sTableName_.c_str(),gc_cID);
	for(nloop = iStartIndex_; nloop < iStartIndex_+iLen_;++nloop)
	{
		pProcChann = (CProcChannParam*)_vProcChann[nloop];
		pDB_->Cmd(",[%s]",pProcChann->GetChannInfo()._cChannNo);
	}
	pDB_->Cmd(",[%s]",gc_cTime);
	pDB_->Cmd(",[%s]",gc_cRev);
	pDB_->Cmd(",[%s]",gc_cRev2);
	pDB_->Cmd(",[%s]",gc_cRev3);
	pDB_->Cmd(",[%s]",gc_cRev4);
	pDB_->Cmd(",[%s]",gc_cRev5);
	pDB_->Cmd(",[%s]",gc_cEventID);
	pDB_->Cmd(",[%s])",gc_cSynFlag);

	pDB_->Cmd(" VALUES (%d,",iCurID_);
	for(nloop = 0; nloop < iLen_; nloop++)
	{
		pDB_->Cmd(" %.2f,",pfData_[nloop]);
	}
	pDB_->Cmd("'%s',",strTimeCurr);
	pDB_->Cmd(" %.2f,",pfRev[0]);
	pDB_->Cmd(" %.2f,",pfRev[1]);
	pDB_->Cmd(" %.2f,",pfRev[2]);
	pDB_->Cmd(" %.2f,",pfRev[3]);
	pDB_->Cmd(" %.2f,",pfRev[4]);
	pDB_->Cmd(" %d,",iEventID_);
	pDB_->Cmd(" %d)",0);
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
		return -1;
	}
	
	{
		LY::PLANTINFO_STATUS loStatus;
		this->_pLockProcData->BeginRead();
		loStatus.m_strGroup = this->_sGroup.c_str();
		loStatus.m_strCompany = this->_sCompany.c_str();
		loStatus.m_strFactory = this->_sFactory.c_str();
		loStatus.m_strPlant = this->_sPlantInfo._cPlantID;
		loStatus.m_strSet = this->_sPlantInfo._cSetID;

		loStatus.m_nVibChannCount = this->_vVibChann.size();
		loStatus.m_nDynChannCount = this->_vDynChann.size();
		loStatus.m_nProcChannCount = this->_vProcChann.size();

		CBHDateTime loCurrentTime;	
		loCurrentTime = loCurrentTime.GetCurrentTimeEx();
		loStatus.m_strProcSaveTime = loCurrentTime.Format();
		this->_pLockProcData->EndRead();
		if (NULL!= g_pServerStatus)
		{
			g_pServerStatus->UpdateStatistics(loStatus);

		}
	}

	return 1;

}

bool CRealPlantInfo::IsRecordExist(IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_ )
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("SELECT top 1 [%s]  FROM [%s] WHERE [%s]='%s'",
		gc_cTime,
		sTable_.c_str(),
		gc_cTime,tDateTime_.Format());
	if(pDB_->More())
	{
		return true;
	}
	return false;
}

int CRealPlantInfo::GetRecordDataID( IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_ )
{
	int iID = -1;

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("SELECT top 1 [%s]  FROM [%s] WHERE [%s]='%s'",
		gc_cID,
		sTable_.c_str(),
		gc_cTime,tDateTime_.Format());
	if(pDB_->More())
	{
		iID = pDB_->GetLong(gc_cID);
	}
	return iID;
}

int CRealPlantInfo::SaveVibFastTrend( IDBInterFace * pDB_, 
									   DATATYPE_WAVE *pWave_, 
									   S_VibCharValue *pVibCharValue_, 
									   S_CustomParamData *pfCustomParom_,
									   int iAllRealNum_, 
									   int iLen_, 
									   CBHDateTime & aoDataTime, 
									   int iStartIndex_,
									   int iEventID_,
									   int iAlarmStatus_,
									   const float& fScaleCoef_,
									   int anPrecisionEnabled,
									   std::vector<float>& afPreciousMin ,
									   std::vector<float>&  afPreciousMax,
									   bool bEnd_/*=false*/ )
{
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
	{
		ASSERT(FALSE);
		return -1;
	}
	/* 快变数据存储逻辑
	1.如果不配置快变数据全部保存g_SaveAllFastData=1,则只保存普通快变且发生报警的数据和后处理数据到月表
	2.iAlarmStatus_>0：快变且报警数据，iEventID_ < 0: 需要永久保存的转速快变数据，记录ID为负
	3.普通快变数据和报警快变数据，如果磁盘空间满了，复写以前记录
	4.需要永久保存的快变数据，磁盘空间满了的情况下，先删除趋势数据，再复写数据
	*/

	UpdateFastDataSaveParam();

	if(iAlarmStatus_ <= 0 && !g_SaveAllFastData)
	{
		return 1;
	}
	
	

	bool bSaveWave = false;

	this->_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	if(g_SaveAllFastDataWave)
	{
		bSaveWave = true;
	}
	else
	{
		if(iAlarmStatus_ > 0)
		{
			bSaveWave = true;
			_tVibLastSaveTime = CBHDateTime::GetCurrentTime();
		}
		else
		{
			CBHDateTime loDatetime = CBHDateTime::GetCurrentTime();
			COleDateTimeSpan lTimespan = loDatetime - _tVibLastSaveTime;

			int nTotalSeconds = lTimespan.GetTotalSeconds();
			if( nTotalSeconds> g_SaveFastDataWaveInterval)
			{
				bSaveWave = true;
				_tVibLastSaveTime = loDatetime;
			}

		}
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
	
	if(iStartIndex_ == 0)
	{
        BEGIN_ERROR_HANDLE
		if(!IsRecordExist(pDB_,_sVibTrendFastTblName,aoDataTime))
		{
			int iCurrIDVibTrendFast=0;

			GetVibTrendFastCurrID(iCurrIDVibTrendFast);
			
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());

			iCurrIDVibTrendFast = this->ResetTrendTableCurID(&loDb,_sVibTrendFastTblName,iCurrIDVibTrendFast);
			
			if(SaveVibRecord(&loDb,_sVibTrendFastTblName,iCurrIDVibTrendFast,pWave_,pVibCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave) >= 0)
			{
				UpdateVibFastDataFromTrendData(&loDb,_sVibTrendFastTblName,iCurrIDVibTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave);
			}
			
			
			
		}
		END_ERROR_HANDLE
	}
	else
	{
		BEGIN_ERROR_HANDLE
		int iCurrIDVibTrendFast=GetRecordDataID(pDB_,_sVibTrendFastTblName,aoDataTime);
		if(iCurrIDVibTrendFast >= 0)
		{
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());
			
			if(SaveVibRecord(&loDb,_sVibTrendFastTblName,iCurrIDVibTrendFast,pWave_,pVibCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave) >= 0)
			{
				UpdateVibFastDataFromTrendData(&loDb,_sVibTrendFastTblName,iCurrIDVibTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave);
			}
			
		}
		else
		{
			GetVibTrendFastCurrID(iCurrIDVibTrendFast);

			iCurrIDVibTrendFast = this->ResetTrendTableCurID(pDB_,_sVibTrendFastTblName,iCurrIDVibTrendFast);

			if(InsertVibFastDataFromTrendData(pDB_,_sVibTrendFastTblName,iCurrIDVibTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave) >= 0)
			{
				SaveVibRecord(pDB_,_sVibTrendFastTblName,iCurrIDVibTrendFast,pWave_,pVibCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave);
			}
		}
		END_ERROR_HANDLE
	}

	return 1;
}

int CRealPlantInfo::SaveDynFastTrend(IDBInterFace * pDB_, 
									   DATATYPE_WAVE *pWave_, 
									   S_DynCharValue *pDynCharValue_, 
									   S_CustomParamData *pfCustomParom_, 
									   int iAllRealNum_, 
									   int iLen_, 
									   CBHDateTime & aoDataTime, 
									   int iStartIndex_,
									   int iEventID_,
									   int iAlarmStatus_,
									   const float& fScaleCoef_,
									   int anPrecisionEnabled,
									   std::vector<float>& afPreciousMin ,
									   std::vector<float>&  afPreciousMax,
									   bool bEnd_/* =false */)
{
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
	{
		ASSERT(FALSE);
		return -1;
	}

	UpdateFastDataSaveParam();

	if(iAlarmStatus_ <= 0 && !g_SaveAllFastData)
	{
		return 1;
	}

	
	bool bSaveWave = false;

	this->_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(g_SaveAllFastDataWave)
	{
		bSaveWave = true;
	}
	else
	{
		if(iAlarmStatus_ > 0)
		{
			bSaveWave = true;
			_tDynLastSaveTime = CBHDateTime::GetCurrentTime();
		}
		else
		{
			CBHDateTime loDatetime = CBHDateTime::GetCurrentTime();
			COleDateTimeSpan lTimespan = loDatetime - _tDynLastSaveTime;
			if(lTimespan.GetTotalSeconds() > g_SaveFastDataWaveInterval)
			{
				bSaveWave = true;
				_tDynLastSaveTime = loDatetime;
			}
			
		}
	}
	END_ERROR_HANDLE
	this->_pLockDynData->EndWrite();

	/* 快变数据存储逻辑
	1.如果不配置快变数据全部保存g_SaveAllFastData=1,则只保存普通快变且发生报警的数据和后处理数据到月表
	2.如果配置快变数据全部保存，根据各个趋势表配置保存相应快变数据
	*/

	if(iStartIndex_ == 0)
	{
		BEGIN_ERROR_HANDLE
		if(!IsRecordExist(pDB_,_sDynTrendFastTblName,aoDataTime))
		{
			int iCurrIDDynTrendFast=0;
			
			GetDynTrendFastCurrID(iCurrIDDynTrendFast);
			
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());

			iCurrIDDynTrendFast = this->ResetTrendTableCurID(&loDb,_sDynTrendFastTblName,iCurrIDDynTrendFast);

			if(SaveDynRecord(&loDb,_sDynTrendFastTblName,iCurrIDDynTrendFast,pWave_,pDynCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave) >= 0)
			{
				UpdateDynFastDataFromTrendData(&loDb,_sDynTrendFastTblName,iCurrIDDynTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave);
			}
			
		}
		END_ERROR_HANDLE

	}
	else
	{
		BEGIN_ERROR_HANDLE
		int iCurrIDDynTrendFast=GetRecordDataID(pDB_,_sDynTrendFastTblName,aoDataTime);
		if(iCurrIDDynTrendFast >= 0)
		{
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());

			if(SaveDynRecord(&loDb,_sDynTrendFastTblName,iCurrIDDynTrendFast,pWave_,pDynCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave) >= 0) 
			{
				UpdateDynFastDataFromTrendData(&loDb,_sDynTrendFastTblName,iCurrIDDynTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave);
			}
			
		}
		else
		{
			GetVibTrendFastCurrID(iCurrIDDynTrendFast);

			iCurrIDDynTrendFast = this->ResetTrendTableCurID(pDB_,_sVibTrendFastTblName,iCurrIDDynTrendFast);

			if(InsertDynFastDataFromTrendData(pDB_,_sDynTrendFastTblName,iCurrIDDynTrendFast,aoDataTime,iStartIndex_,iLen_,bSaveWave) >= 0)
			{
				SaveDynRecord(pDB_,_sDynTrendFastTblName,iCurrIDDynTrendFast,pWave_,pDynCharValue_,pfCustomParom_,iAllRealNum_,iLen_,aoDataTime,iEventID_,iStartIndex_,fScaleCoef_,anPrecisionEnabled,afPreciousMin,afPreciousMax,bSaveWave);
			}
		}
		END_ERROR_HANDLE
	}

	return 1;
}

int CRealPlantInfo::SaveProcFastTrend(IDBInterFace *pDB_,
									  float *pfData_,
									  int iLen_,
									  const CBHDateTime& aoDateTime_,
									  int iEventID_,
									  int iAlarmStatus_,
									  int iStartIndex_/* =0 */)
{
	if (((iLen_+iStartIndex_)> _sPlantInfo._iProcChannNum)||(iLen_<1)||(iStartIndex_<0))
		return -1;
	
	UpdateFastDataSaveParam();

	if(iAlarmStatus_ <= 0 && !g_SaveAllFastData)
	{
		return 1;
	}
	
	/* 快变数据存储逻辑
	1.如果不配置快变数据全部保存g_SaveAllFastData=1,则只保存普通快变且发生报警的数据和后处理数据到月表
	2.如果配置快变数据全部保存，根据各个趋势表配置保存相应快变数据
	*/

	if(iStartIndex_ == 0)
	{
		
		BEGIN_ERROR_HANDLE
		if(!IsRecordExist(pDB_,_sProcTrendFastTblName,aoDateTime_))
		{
			int iCurrIDProcTrendFast=0;

			if(iAlarmStatus_ > 0)
			{
				if(iEventID_ >= 0)
				{
					GetProcTrendFastCurrID(iCurrIDProcTrendFast);
				}
				else
				{
					GetNegProcTrendFastCurrID(iCurrIDProcTrendFast);
					if(iCurrIDProcTrendFast >= 0)
					{
						iCurrIDProcTrendFast = this->InitTableNegCurID(pDB_,_sProcTrendFastTblName);
					}
					if(g_bIsDiskFull)
					{
						if(!this->DeleteProcTrendRecord(pDB_))
						{
							if(!this->DeleteProcTrendFastRecord(pDB_))
							{
								return 1;
							}
						}
					}
				}

			}
			else 
			{
				GetProcTrendFastCurrID(iCurrIDProcTrendFast);
			}
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());

			iCurrIDProcTrendFast = this->ResetTrendTableCurID(&loDb,_sProcTrendFastTblName,iCurrIDProcTrendFast);
			
			if(SaveProcRecord(&loDb,_sProcTrendFastTblName,iCurrIDProcTrendFast,pfData_,iLen_,aoDateTime_,iEventID_,iStartIndex_) >= 0)
			{
				UpdateProcFastDataFromTrendData(&loDb,_sProcTrendFastTblName,iCurrIDProcTrendFast,aoDateTime_,iStartIndex_,iLen_);
			}
			
		}
		END_ERROR_HANDLE

	}
	else
	{
		BEGIN_ERROR_HANDLE
		int iCurrIDProcTrendFast=GetRecordDataID(pDB_,_sProcTrendFastTblName,aoDateTime_);
		if(iCurrIDProcTrendFast >= 0)
		{
			COleSqlServerDB loDb;
			loDb.setPDB(pDB_->getPDB());

			if(SaveProcRecord(&loDb,_sProcTrendFastTblName,iCurrIDProcTrendFast,pfData_,iLen_,aoDateTime_,iEventID_,iStartIndex_) >= 0)
			{

				UpdateProcFastDataFromTrendData(&loDb,_sProcTrendFastTblName,iCurrIDProcTrendFast,aoDateTime_,iStartIndex_,iLen_);
			}
		
		}
		else
		{
			GetProcTrendFastCurrID(iCurrIDProcTrendFast);

			iCurrIDProcTrendFast = this->ResetTrendTableCurID(pDB_,_sProcTrendFastTblName,iCurrIDProcTrendFast);

			if(InsertProcFastDataFromTrendData(pDB_,_sProcTrendFastTblName,iCurrIDProcTrendFast,aoDateTime_,iStartIndex_,iLen_) >= 0)
			{
				SaveProcRecord(pDB_,_sProcTrendFastTblName,iCurrIDProcTrendFast,pfData_,iLen_,aoDateTime_,iEventID_,iStartIndex_);
			}

		}
		END_ERROR_HANDLE
	}

	return 1;
}

int CRealPlantInfo::SetProcJudgeRunStatus( int anProcJudgedRunStatus,int anLen_,int anStartIndex_ )
{
	int nCount=0;
	int iloop=0;
	if (((anLen_+anStartIndex_)> _sPlantInfo._iProcChannNum)||(anLen_<1)||(anStartIndex_<0))
		nCount=-1;
	else
	{
		anLen_=anLen_+anStartIndex_;
		nCount=0;
		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=anStartIndex_;iloop<anLen_;++iloop)
			{
				((CProcChannParam*)_vProcChann[iloop])->SetSUDStatus(anProcJudgedRunStatus);
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}
	return nCount;
}

int CRealPlantInfo::GetProcJudgeRunStatus( )
{
	int lnSpeedCount = 0;

	int lnStopCount = 0;

	bool lbJudgeSUDStatus = false;

	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			int lnProcSUDStatus = ((CProcChannParam*)_vProcChann[lnProcChannIndex])->GetSUDStatus();

			if (lnProcSUDStatus == 0)
			{
				continue;
			}

			lbJudgeSUDStatus = true;

			if (lnProcSUDStatus == 1)
			{
				++lnSpeedCount;
			}
			else if(lnProcSUDStatus == 2)
			{
				++lnStopCount;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	if (!lbJudgeSUDStatus)
	{
		return 0;
	}

	return lnStopCount<lnSpeedCount?1:-1;
}
void CRealPlantInfo::SetVibJudgeRunStatus( int anVibJudgedRunStatus )
{
	_pLockVibData->BeginWrite();
	this->m_nVibJudgedRunStatus = anVibJudgedRunStatus;
	_pLockVibData->EndWrite();
}
int CRealPlantInfo::GetVibJudgeRunStatus( )
{
	int lnVibJudgeRunStatus = 0;
	_pLockVibData->BeginRead();
	lnVibJudgeRunStatus = this->m_nVibJudgedRunStatus;
	_pLockVibData->EndRead();
	return lnVibJudgeRunStatus;
}

int CRealPlantInfo::SaveSpecTime(IDBInterFace * pDB_,const std::pair<CBHDateTime,CBHDateTime>& apDatetime,const CString& sTblName_,int nStatus_)
{

	CString lsTblName = sTblName_;
	lsTblName = lsTblName.MakeUpper();

	string sTable = lsTblName;

	std::pair<CString,int> lpTableInfo;
	lpTableInfo = make_pair(lsTblName,nStatus_);
	
	std::map< pair<CBHDateTime,CBHDateTime>, map<CString,int> >::iterator it;

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	if((it = g_map_SpecSaveTime.find(apDatetime)) != g_map_SpecSaveTime.end())
	{
		it->second.insert(lpTableInfo);
		g_map_SpecSaveTime.insert(make_pair(apDatetime,it->second));
	}
	else
	{
		std::map<CString,int> lmTmp;
		lmTmp.insert(lpTableInfo);
		g_map_SpecSaveTime.insert(make_pair(apDatetime,lmTmp));
	}
	SaveDetainDataTime();
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
	return true;
}
int CRealPlantInfo::RemoveSpecTime(IDBInterFace * pDB_,const string& strTblName_)
{
	CString lstrTblName = strTblName_.c_str();

	lstrTblName = lstrTblName.MakeUpper();

	bool lbShouldRemoveTime = false;

	CBHDateTime ltTime = CBHDateTime::GetCurrentTimeEx();

	std::map< pair<CBHDateTime,CBHDateTime>, map<CString,int> >::iterator it;
	std::map<CString,int>::iterator itTbl;

	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	for( it = g_map_SpecSaveTime.begin();
		it != g_map_SpecSaveTime.end(); it++)
	{
		if(it->second.find(lstrTblName) != it->second.end())
		{
			lbShouldRemoveTime = true;
			break;
		}
	}
	END_ERROR_HANDLE
	_pLockVibData->EndRead();

	if(lbShouldRemoveTime)
	{
		int lnMaxRecord = G_D_TREND_SECONDRANGE;

		if(lstrTblName.Find(_T("SECOND")) >= 0)
		{
			lnMaxRecord = G_D_TREND_SECONDRANGE;
		}
		else if(lstrTblName.Find(_T("HOUR")) >= 0)
		{
			lnMaxRecord = G_D_TREND_HOURRANGE;
		}
		else if(lstrTblName.Find(_T("DAY")) >= 0)
		{
			lnMaxRecord = G_D_TREND_DAYRANGE;
		}
		pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s >= 0 and %s <= %d ORDER BY %s",gc_cTime,lstrTblName,gc_cID,gc_cID,lnMaxRecord,gc_cTime);
		if(pDB_->More())
		{
			ltTime = pDB_->GetTime(gc_cTime);
		}
		else
		{
			return 1;
		}

	}
	else
	{
		return 1;
	}

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE

	bool lbShouldDelete = false;
	for( it = g_map_SpecSaveTime.begin();
		it != g_map_SpecSaveTime.end(); it++)
	{
		pair<CBHDateTime,CBHDateTime> lpDateTime = it->first;

		if((itTbl = it->second.find(lstrTblName)) != it->second.end())
		{
			if(ltTime > lpDateTime.second)
			{
				lbShouldDelete = true;
				break;
			}
		}
	}

	if(it != g_map_SpecSaveTime.end())
	{
		if(lbShouldDelete)
		{
			if(it->second.size() <= 1)
			{
				g_map_SpecSaveTime.erase(it);
			}
			else
			{
				it->second.erase(itTbl);
			}
			SaveDetainDataTime();
		}
	}
		
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
	return 1;
}
int CRealPlantInfo::SaveSpecTimeData(IDBInterFace * pDB_,string sTable_,CBHDateTime tDateTime_,int nCurrID_)
{
	bool lbSave = false;

	CString lsTable = sTable_.c_str();

	lsTable = lsTable.MakeUpper();

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	int lnSaveID = -1;
	for(std::map< pair<CBHDateTime,CBHDateTime>, map<CString,int> >::iterator it = g_map_SpecSaveTime.begin();
		it != g_map_SpecSaveTime.end(); ++it)
	{
		pair<CBHDateTime,CBHDateTime> lpDateTime = it->first;

		if(it->second.find(lsTable) != it->second.end())
		{
			if(tDateTime_ >= lpDateTime.first && tDateTime_ <= lpDateTime.second)
			{

				if(sTable_ == _sVibTrendSecTblName)
				{
					lbSave = true;
					GetNegVibTrendSecondCurrID(lnSaveID);
				}
				else if(sTable_ == _sVibTrendHourTblName)
				{
					lbSave = true;
					GetNegVibTrendHourCurrID(lnSaveID);
				}
				else if(sTable_ == _sVibTrendDayTblName)
				{
					lbSave = true;
					GetNegVibTrendDayCurrID(lnSaveID);
				}
				else if(sTable_ == _sDynTrendSecTblName)
				{
					lbSave = true;
					GetNegDynTrendSecondCurrID(lnSaveID);
				}
				else if(sTable_ == _sDynTrendHourTblName)
				{
					lbSave = true;
					GetNegDynTrendHourCurrID(lnSaveID);
				}
				else if(sTable_ == _sDynTrendDayTblName)
				{
					lbSave = true;
					GetNegDynTrendDayCurrID(lnSaveID);
				}
				else if(sTable_ == _sProcTrendSecTblName)
				{
					lbSave = true;
					GetNegProcTrendSecondCurrID(lnSaveID);
				}
				else if(sTable_ == _sProcTrendHourTblName)
				{
					lbSave = true;
					GetNegProcTrendHourCurrID(lnSaveID);
				}
				else if(sTable_ == _sProcTrendDayTblName)
				{
					lbSave = true;
					GetNegProcTrendDayCurrID(lnSaveID);
				}
				else
				{
					lbSave = false;
				}
				
				if(lbSave)
				{
					if(lnSaveID >= 0)
					{
						lnSaveID = this->InitTableNegCurID(pDB_,sTable_);
					}
					pDB_->Cmd("UPDATE %s SET %s = %d WHERE %s = %d",
						sTable_.c_str(),gc_cID,lnSaveID,gc_cID,nCurrID_);
					if(pDB_->Exec())
					{
						lbSave = true;
						break;
					}
					else
					{
						lbSave = false;
					}
				}
				
			}
		}

	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	return lbSave;
}

int CRealPlantInfo::InitTableNegCurID( IDBInterFace *pDB_,string sTable_ )
{
	int lnID = -1;

	pDB_->Cmd("select top 1 [%s] from [%s] where %s < 0 order by [%s]",gc_cID,sTable_.c_str(),gc_cID,gc_cID);
	if(pDB_->More())
	{
		lnID = pDB_->GetLong(gc_cID) - 1;
	}
	{
		if(sTable_ == _sVibTrendSecTblName)
		{
			this->SetNegVibTrendSecondCurrID(lnID-1);
		}
		else if(sTable_ == _sVibTrendHourTblName)
		{
			this->SetNegVibTrendHourCurrID(lnID-1);
		}
		else if(sTable_ == _sVibTrendDayTblName)
		{
			this->SetNegVibTrendDayCurrID(lnID-1);
		}
		else if(sTable_ == _sVibTrendMonthTblName)
		{
			this->SetNegVibTrendMonthCurrID(lnID-1);
		}
		else if(sTable_ == _sVibTrendFastTblName)
		{
			this->SetNegVibTrendFastCurrID(lnID-1);
		}
		else if(sTable_ == _sDynTrendSecTblName)
		{
			this->SetNegDynTrendSecondCurrID(lnID-1);
		}
		else if(sTable_ == _sDynTrendHourTblName)
		{
			this->SetNegDynTrendHourCurrID(lnID-1);
		}
		else if(sTable_ == _sDynTrendDayTblName)
		{
			this->SetNegDynTrendDayCurrID(lnID-1);
		}
		else if(sTable_ == _sDynTrendMonthTblName)
		{
			this->SetNegDynTrendMonthCurrID(lnID-1);
		}
		else if(sTable_ == _sDynTrendFastTblName)
		{
			this->SetNegDynTrendFastCurrID(lnID-1);
		}
		else if(sTable_ == _sProcTrendSecTblName)
		{
			this->SetNegProcTrendSecondCurrID(lnID-1);
		}
		else if(sTable_ == _sProcTrendHourTblName)
		{
			this->SetNegProcTrendHourCurrID(lnID-1);
		}
		else if(sTable_ == _sProcTrendDayTblName)
		{
			this->SetNegProcTrendDayCurrID(lnID-1);
		}
		else if(sTable_ == _sProcTrendMonthTblName)
		{
			this->SetNegProcTrendMonthCurrID(lnID-1);
		}
		else if(sTable_ == _sProcTrendFastTblName)
		{
			this->SetNegProcTrendFastCurrID(lnID-1);
		}
	}
	return lnID;

}


int CRealPlantInfo::ResetTrendTableCurID(IDBInterFace *pDB_,string sTable_,int nCurrID_)
{
	int lnCurrID = nCurrID_;

	if(g_bIsDiskFull)
	{
		SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

		int lnRecordCount = 0;
		pDB_->Cmd("select count(%s) as RecordCount from %s where %s > 0",gc_cID,sTable_.c_str(),gc_cID);
		if(pDB_->More())
		{
			lnRecordCount = pDB_->GetLong("RecordCount");
		}

		pDB_->Cmd("select top 1 %s from %s where %s >= 0 order by %s desc ",
			gc_cID,sTable_.c_str(),gc_cID,gc_cID);
		if(pDB_->More())
		{
			int lnID = pDB_->GetLong(gc_cID);
			if(nCurrID_ > lnID)
			{
				if(sTable_ == _sVibTrendSecTblName)
				{
					this->SetVibTrendSecondCurrID(0);
					this->VibSecTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sVibTrendHourTblName)
				{
					this->SetVibTrendHourCurrID(0);
					this->VibHourTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sVibTrendDayTblName)
				{
					this->SetVibTrendDayCurrID(0);
					this->VibDayTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sVibTrendMonthTblName)
				{
					if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
					{
						this->SetVibTrendMonthCurrID(0);
						this->VibMonthTblRecordNum(lnRecordCount);
						lnCurrID = 0;
					}
					
				}
				else if(sTable_ == _sDynTrendSecTblName)
				{
					this->SetDynTrendSecondCurrID(0);
					this->DynSecTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sDynTrendHourTblName)
				{
					this->SetDynTrendHourCurrID(0);
					this->DynHourTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sDynTrendDayTblName)
				{
					this->SetDynTrendDayCurrID(0);
					this->DynDayTblRecordNum(lnRecordCount);
					lnCurrID = 0;
				}
				else if(sTable_ == _sDynTrendMonthTblName)
				{
					if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
					{
						this->SetDynTrendMonthCurrID(0);
						this->DynMonthTblRecordNum(lnRecordCount);
						lnCurrID = 0;
					}
					
				}
				else if(sTable_ == _sProcTrendSecTblName)
				{
					this->SetProcTrendSecondCurrID(0);
					lnCurrID = 0;
				}
				else if(sTable_ == _sProcTrendHourTblName)
				{
					this->SetProcTrendHourCurrID(0);
					lnCurrID = 0;
				}
				else if(sTable_ == _sProcTrendDayTblName)
				{
					this->SetProcTrendDayCurrID(0);
					lnCurrID = 0;
				}
				else if(sTable_ == _sProcTrendMonthTblName)
				{
					if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
					{
						this->SetProcTrendMonthCurrID(0);
						lnCurrID = 0;
					}
					
				}
				else if(sTable_ == _sVibTrendFastTblName)
				{
					this->SetVibTrendFastCurrID(0);
					lnCurrID = 0;
				}
				else if(sTable_ == _sDynTrendFastTblName)
				{
					this->SetDynTrendFastCurrID(0);
					lnCurrID = 0;
				}
				else if(sTable_ == _sProcTrendFastTblName)
				{
					this->SetProcTrendFastCurrID(0);
					lnCurrID = 0;
				}
			}
		}
	}
	return lnCurrID;
}

bool CRealPlantInfo::ShouldSaveVibWaveData(IDBInterFace *pDB_,string sTable_,CVibChannParam* pVibChannParam_,const S_VibCharValue& aCharValue_)
{
	if(!g_SaveUseOptimizeMode)
	{
		return true;
	}

	if(g_bIsDiskFull)
	{
		if((sTable_ == _sVibTrendSecTblName && this->VibSecTblRecordNum() < 100) || 
			(sTable_ == _sVibTrendHourTblName && this->VibHourTblRecordNum() < 50)  ||
			(sTable_ == _sVibTrendDayTblName && this->VibDayTblRecordNum() < 20)    ||
			(sTable_ == _sVibTrendMonthTblName && this->VibMonthTblRecordNum() < 10))
		{
			return true;
		}
	}

	if(NULL == pVibChannParam_)
	{
		return false;
	}

	bool lbResult = false;

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	if((sTable_ == _sVibTrendSecTblName && pVibChannParam_->ShouldSaveSecondTblWave(aCharValue_)) || 
	   (sTable_ == _sVibTrendHourTblName && pVibChannParam_->ShouldSaveHourTblWave(aCharValue_))  ||
	   (sTable_ == _sVibTrendDayTblName && pVibChannParam_->ShouldSaveDayTblWave(aCharValue_))    ||
	   (sTable_ == _sVibTrendMonthTblName && pVibChannParam_->ShouldSaveMonthTblWave(aCharValue_)))
	{
		lbResult = true;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
	
	return lbResult;
}

bool CRealPlantInfo::ShouldSaveDynWaveData(IDBInterFace *pDB_,string sTable_,CDynChannParam* pDynChannParam_,const S_DynCharValue& aCharValue_)
{
	if(!g_SaveUseOptimizeMode)
	{
		return true;
	}

	
	if(g_bIsDiskFull)
	{
		if((sTable_ == _sDynTrendSecTblName && this->DynSecTblRecordNum() < 100) || 
			(sTable_ == _sDynTrendHourTblName && this->DynHourTblRecordNum() < 50)  ||
			(sTable_ == _sDynTrendDayTblName && this->DynDayTblRecordNum() < 20)    ||
			(sTable_ == _sDynTrendMonthTblName && this->DynMonthTblRecordNum() < 10))
		{
			return true;
		}	
	}


	if(NULL == pDynChannParam_)
	{
		return false;
	}

	bool lbResult = false;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if((sTable_ == _sDynTrendSecTblName && pDynChannParam_->ShouldSaveSecondTblWave(aCharValue_)) || 
	   (sTable_ == _sDynTrendHourTblName && pDynChannParam_->ShouldSaveHourTblWave(aCharValue_))  ||
	   (sTable_ == _sDynTrendDayTblName && pDynChannParam_->ShouldSaveDayTblWave(aCharValue_))    ||
	   (sTable_ == _sDynTrendMonthTblName && pDynChannParam_->ShouldSaveMonthTblWave(aCharValue_)))
	{
		lbResult = true;
	}

	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::VibSecTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	bool    lbResult = false;

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(this->_iVibSecTblSaveCount > 99)
	{
		this->_iVibSecTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iVibSecTblSaveCount;
	}

	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::VibHourTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	bool    lbResult = false;

	_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE
	
	if(this->_iVibHourTblSaveCount > 29)
	{
		this->_iVibHourTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iVibHourTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::VibDayTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	bool    lbResult = false;

	_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if(this->_iVibDayTblSaveCount > 9)
	{
		this->_iVibDayTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iVibDayTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::VibMonthTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	bool    lbResult = false;

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(this->_iVibMonthTblSaveCount > 1)
	{
		this->_iVibMonthTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iVibMonthTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::DynSecTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	CBHDateTime ltTime = CBHDateTime::GetCurrentTimeEx();
	bool    lbResult = false;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(this->_iDynSecTblSaveCount > 99)
	{
		this->_iDynSecTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iDynSecTblSaveCount;
	}
	END_ERROR_HANDLE

	_pLockDynData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::DynHourTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	CBHDateTime ltTime = CBHDateTime::GetCurrentTimeEx();
	bool    lbResult = false;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(this->_iDynHourTblSaveCount > 29)
	{
		this->_iDynHourTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iDynHourTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::DynDayTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	CBHDateTime ltTime = CBHDateTime::GetCurrentTimeEx();
	bool    lbResult = false;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	if(this->_iDynDayTblSaveCount > 9)
	{
		this->_iDynDayTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iDynDayTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

	return lbResult;
}

bool CRealPlantInfo::DynMonthTblSaveTimeReached()
{
	if(!g_SaveUseOptimizeMode)
	{
		return false;
	}

	CBHDateTime ltTime = CBHDateTime::GetCurrentTimeEx();
	bool    lbResult = false;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE
	if(this->_iDynMonthTblSaveCount > 1)
	{
		this->_iDynMonthTblSaveCount = 1;
		lbResult = true;
	}
	else
	{
		++this->_iDynMonthTblSaveCount;
	}
	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

	return lbResult;
}

BOOL CRealPlantInfo::ChangeVibAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{

	if (anChannIndex<0)
	{
		return FALSE;
	}

	this->_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vVibChann.size())
		{
			CVibChannParam * lpVib = (CVibChannParam*)this->_vVibChann.at(anChannIndex);

			if(lpVib)
			{
				lpVib->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
				lpVib->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
				lpVib->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
				lpVib->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
				lpVib->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
				lpVib->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
			}
		}

		END_ERROR_HANDLE

			this->_pLockVibData->EndWrite();

		return TRUE;
}

int CRealPlantInfo::ChangeDynAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockDynData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if (anChannIndex<this->_vDynChann.size())
	{
		CDynChannParam * lpDyn = (CDynChannParam*)this->_vDynChann.at(anChannIndex);

		if(lpDyn)
		{
			lpDyn->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
			lpDyn->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
			lpDyn->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
			lpDyn->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
			lpDyn->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
			lpDyn->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
		}
	}

	END_ERROR_HANDLE

	this->_pLockDynData->EndWrite();

	return TRUE;
}

int CRealPlantInfo::ChangeProcAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockProcData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if (anChannIndex<this->_vProcChann.size())
	{
		CProcChannParam * lpProc = (CProcChannParam*)this->_vProcChann.at(anChannIndex);

		if(lpProc)
		{
			lpProc->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
			lpProc->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
			lpProc->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
			lpProc->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
			lpProc->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
			lpProc->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
		}
	}

	END_ERROR_HANDLE

	this->_pLockProcData->EndWrite();

	return TRUE;
}

int CRealPlantInfo::SetVibGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{

	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vVibChann.size())
		{
			CVibChannParam * lpVib = (CVibChannParam*)this->_vVibChann.at(anChannIndex);

			if(lpVib)
			{
				lpVib->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpVib->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpVib->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockVibData->EndWrite();

		return TRUE;
}

int CRealPlantInfo::SetDynGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockDynData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vDynChann.size())
		{
			CDynChannParam * lpDyn = (CDynChannParam*)this->_vDynChann.at(anChannIndex);

			if(lpDyn)
			{
				lpDyn->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpDyn->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpDyn->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockDynData->EndWrite();

		return TRUE;
}

int CRealPlantInfo::SetProcGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockProcData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vProcChann.size())
		{
			CProcChannParam * lpProc = (CProcChannParam*)this->_vProcChann.at(anChannIndex);

			if(lpProc)
			{
				lpProc->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpProc->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpProc->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockProcData->EndWrite();

		return TRUE;
}


bool CRealPlantInfo::GetSpecWireLessChannStatus(int anIndex,string &asChannID, int &anChannOnline, int &anChannNormal, int &anDataUpdate,int &anBatteryPower, int anChannType)
{
	if (anIndex < 0)
	{
		return false;
	}

	S_ChannInfo loChannInfo;

	bool lbGetDataSucc = false;

	switch(anChannType)
	{
		case GE_VIBCHANN:
			{
				if (anIndex >= _sPlantInfo._iVibChannNum || anIndex >= _vVibChann.size())
				{
					return false;
				}
			
				loChannInfo = _vVibChann[anIndex]->GetChannInfo();

				if (loChannInfo._iDataSource != GE_DATA_SOURCE_WIRELESS)
				{
					return false;
				}

				asChannID = loChannInfo._cChannID;

				_pLockVibData->BeginRead();

				anChannOnline  = ((CVibChannParam *)_vVibChann[anIndex])->ChannOnline();

				anChannNormal  = ((CVibChannParam *)_vVibChann[anIndex])->ChannNormal();

				anDataUpdate   = ((CVibChannParam *)_vVibChann[anIndex])->ChannDataUpdate();

				anBatteryPower = ((CVibChannParam *)_vVibChann[anIndex])->BatteryPower();

				_pLockVibData->EndRead();

				lbGetDataSucc = true;
			}
			break;
		case GE_TEMPCHANN:
			{
				if (anIndex >= _sPlantInfo._iProcChannNum || anIndex >= _vProcChann.size())
				{
					return false;
				}

				loChannInfo = _vProcChann[anIndex]->GetChannInfo();

				if (loChannInfo._iDataSource != GE_DATA_SOURCE_WIRELESS)
				{
					return false;
				}
				
				asChannID = loChannInfo._cChannID;

				_pLockVibData->BeginRead();

				anChannOnline  = ((CProcChannParam *)_vProcChann[anIndex])->ChannOnline();

				anChannNormal  = ((CProcChannParam *)_vProcChann[anIndex])->ChannNormal();

				anDataUpdate   = ((CProcChannParam *)_vProcChann[anIndex])->ChannDataUpdate();

				anBatteryPower = ((CProcChannParam *)_vProcChann[anIndex])->BatteryPower();

				_pLockVibData->EndRead();

				lbGetDataSucc = true;
			}
			break;
		default:
			break;
	}
		
	return lbGetDataSucc;
}

int CRealPlantInfo::VibSecTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockVibData->BeginRead();
	lnRecordNum = this->_iVibSecTblRecordNum;
	_pLockVibData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::VibSecTblRecordNum( int nRecordNum_ )
{
	_pLockVibData->BeginWrite();
	this->_iVibSecTblRecordNum = nRecordNum_;
	_pLockVibData->EndWrite();
}

int CRealPlantInfo::VibHourTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockVibData->BeginRead();
	lnRecordNum = this->_iVibHourTblRecordNum;
	_pLockVibData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::VibHourTblRecordNum( int nRecordNum_ )
{
	_pLockVibData->BeginWrite();
	this->_iVibHourTblRecordNum = nRecordNum_;
	_pLockVibData->EndWrite();
}

int CRealPlantInfo::VibDayTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockVibData->BeginRead();
	lnRecordNum = this->_iVibDayTblRecordNum;
	_pLockVibData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::VibDayTblRecordNum( int nRecordNum_ )
{
	_pLockVibData->BeginWrite();
	this->_iVibDayTblRecordNum = nRecordNum_;
	_pLockVibData->EndWrite();
}

int CRealPlantInfo::VibMonthTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockVibData->BeginRead();
	lnRecordNum = this->_iVibMonthTblRecordNum;
	_pLockVibData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::VibMonthTblRecordNum( int nRecordNum_ )
{
	_pLockVibData->BeginWrite();
	this->_iVibMonthTblRecordNum = nRecordNum_;
	_pLockVibData->EndWrite();
}

int CRealPlantInfo::DynSecTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockDynData->BeginRead();
	lnRecordNum = this->_iDynSecTblRecordNum;
	_pLockDynData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::DynSecTblRecordNum( int nRecordNum_ )
{
	_pLockDynData->BeginWrite();
	this->_iDynSecTblRecordNum = nRecordNum_;
	_pLockDynData->EndWrite();
}

int CRealPlantInfo::DynHourTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockDynData->BeginRead();
	lnRecordNum = this->_iDynHourTblRecordNum;
	_pLockDynData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::DynHourTblRecordNum( int nRecordNum_ )
{
	_pLockDynData->BeginWrite();
	this->_iDynHourTblRecordNum = nRecordNum_;
	_pLockDynData->EndWrite();
}

int CRealPlantInfo::DynDayTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockDynData->BeginRead();
	lnRecordNum = this->_iDynDayTblRecordNum;
	_pLockDynData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::DynDayTblRecordNum( int nRecordNum_ )
{
	_pLockDynData->BeginWrite();
	this->_iDynDayTblRecordNum = nRecordNum_;
	_pLockDynData->EndWrite();
}

int CRealPlantInfo::DynMonthTblRecordNum()
{
	int lnRecordNum = 0;
	_pLockDynData->BeginRead();
	lnRecordNum = this->_iDynMonthTblRecordNum;
	_pLockDynData->EndRead();
	return lnRecordNum;
}

void CRealPlantInfo::DynMonthTblRecordNum( int nRecordNum_ )
{
	_pLockDynData->BeginWrite();
	this->_iDynMonthTblRecordNum = nRecordNum_;
	_pLockDynData->EndWrite();
}


int CRealPlantInfo::InsertVibFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_ )
{
	/*快变数据缺失前面部分测点数据补齐
	1. 查找快变表中距离当前数据时间最近的一条记录t1
	2. 查找秒表中距离当前数据时间最近的一条记录t2
	3. 取快变表和秒表中查到的记录中(t1,t2)距离当前记录近的记录t3
	4. 如果t3数据与当前数据的时间间隔小于当前记录和实时数据的时间间隔，将t3对应的记录拷贝到快变表
	5. 如果t3数据与当前数据的时间间隔大于当前记录和实时数据的时间间隔，将实时数据存储到快变表
	*/
	int lnVibChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();

	_pLockVibData->BeginRead();
	lnVibChannNum = this->GetVibChannNum();
	_pLockVibData->EndRead();

	if(iStartIndex_ <= 0 || (iLen_ + iStartIndex_ > lnVibChannNum) || iLen_ <=0)
	{
		return 1;
	}

	if(this->InsertVibFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,iStartIndex_,bSaveWave_) > 0)
	{
		return 1;
	}

	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}
	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_VibCharValue vibValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev( pfRev, 5);
	GetRealSmpInfo(iSmpNum, iSmpFreq);


	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*	int iSmpNumMax = GetPlantInfo()._iSmpNum;*/

	int iSmpNumMax = iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{
		iSaveWaveNum=iSmpNumMax;
	}

	ASSERT(iSaveWaveNum >0);

	int lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

	iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

	V_PCHANNINFO::iterator it;
	CVibChannParam *pVibChannParam=NULL;

	//插入记录
	pDB_->ClearSql();
	//不存在这条记录，就插入。
	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sVibTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		pDB_->Cmd("INSERT INTO [%s](%s ",_sVibTrendFastTblName.c_str(),gc_cID);

		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" VALUES (%d,",iCurID_);
		int lnCharIndex = -1;
		for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
		{
			lnCharIndex++;

			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam=(CVibChannParam*)(*it);
			vibValue=pVibChannParam->GetCharValueEx();
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			int lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;

			index ++;

			iSaveWaveNum = vibValue._iSmpNum;

			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}

			if (iSaveWaveNum != 0)
			{
				iSmpNum=iSaveWaveNum;
			}		

			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ALLCHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ONECHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_TWOCHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_THREECHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_HALFCHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_RESCHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_DCCHAR));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_GAP));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARONE));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTWO));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
			pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARHALF));
			pDB_->Cmd("%d,",vibValue._iSmpFreq);
			pDB_->Cmd("%d,",iSaveWaveNum);
			ASSERT(iSmpNum != 0);
			pDB_->Cmd("%d,",vibValue._iRev);
			pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd("%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (int nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",dtTime_.Format(DATE_TIME_FORMAT));
		pDB_->Cmd("%d,",0);
		pDB_->Cmd("%d)",0);

	}
	else
	{
		pDB_->Cmd("UPDATE [%s] SET ",_sVibTrendFastTblName.c_str());

		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			vibValue = pVibChannParam->GetCharValueEx();
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibValue._fCharAll);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibValue._fCharOne);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibValue._fCharTwo);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibValue._fCharThree);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibValue._fCharHalf);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibValue._fCharRes);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibValue._fDC);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibValue._fGap);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibValue._fPhaseOne);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibValue._fPhaseTwo);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibValue._fPhaseThree);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibValue._fPhaseHalf);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibValue._iSmpNum);
			pDB_->Cmd("[%s] = %.2f,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);
			pDB_->Cmd("[%s] = '%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd("[%s] = %d,",gc_cSampleNum,iSmpNum);
		pDB_->Cmd("[%s] = %d,",gc_cSampleFreq,iSmpFreq);
		pDB_->Cmd("[%s] = %d,",gc_cRev,pfRev[0]);
		pDB_->Cmd("[%s] = %d,",gc_cRev2,pfRev[1]);
		pDB_->Cmd("[%s] = %d,",gc_cRev3,pfRev[2]);
		pDB_->Cmd("[%s] = %d,",gc_cRev4,pfRev[3]);
		pDB_->Cmd("[%s] = %d,",gc_cRev5,pfRev[4]);
		pDB_->Cmd("[%s] = '%s',",gc_cTime,dtTime_.Format());
		pDB_->Cmd("[%s] = %d,",gc_cEventID,0);
		pDB_->Cmd("[%s] = %d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s] = %d",gc_cID,iCurID_);

	}
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveFastVibRecord");
		return -1;
	}

	if(bSaveWave_)
	{
		pDB_->Cmd("SELECT %s FROM [%s]",_sVibFieldSel.c_str(),_sVibTrendFastTblName.c_str());
		pDB_->Cmd(" WHERE %s=%d",gc_cID,iCurID_);

		if(pDB_->More(TRUE))
		{
			index=0;
			int lnCharIndex = -1;
			BYTE  *pBuf=new BYTE[iAllBytes];
			float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
			int iRealSmpNum= 0;
			for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
			{
				int lnTotalSize = 0;

				try
				{
					lnCharIndex++;

					_pLockVibData->BeginRead();
					BEGIN_ERROR_HANDLE
					pVibChannParam=(CVibChannParam*)(*it);
					END_ERROR_HANDLE
					_pLockVibData->EndRead();

					int lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;

					_pLockVibData->BeginRead();
					BEGIN_ERROR_HANDLE

					iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;

					iSaveWaveNum = iRealSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}

					CWaveHead lsHead = pVibChannParam->WaveHead();
					lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

					memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
					vibValue = pVibChannParam->GetCharValueEx();
					END_ERROR_HANDLE

					_pLockVibData->EndRead();


				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				bool lbRet = true;
				lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
				lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			}
			KD_DeletePoint(pBuf);
			KD_DeletePoint(pfCustomData);
		}
		else
		{
			CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveVibRecord");
		}
	}
	else
	{
		pDB_->Cmd("UPDATE %s SET %s = %d ",_sVibTrendFastTblName.c_str(),gc_cID,iCurID_);
		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			END_ERROR_HANDLE
			_pLockVibData->EndRead();
			pDB_->Cmd(" ,%s = NULL",pVibChannParam->GetSpecCharTypeField(-1));
			pDB_->Cmd(" ,%s = NULL",pVibChannParam->GetSpecCharTypeField(-7));
		}

		pDB_->Cmd(" WHERE %s = %d",gc_cID,iCurID_);

		pDB_->Exec();
	}
	
	return 1;
}

int CRealPlantInfo::UpdateVibFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_ )
{
	int lnVibChannNum = 0;
	_pLockVibData->BeginRead();
	lnVibChannNum = this->GetVibChannNum();
	_pLockVibData->EndRead();

	if(iStartIndex_ != 0 || (iStartIndex_ == 0 && iLen_ + iStartIndex_ >= lnVibChannNum) || iLen_ <= 0)
	{
		return 1;
	}

	int nloop = 0;

	int lnNum = iStartIndex_ + iLen_;

	if(this->UpdateVibFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,lnNum,bSaveWave_) > 0)
	{
		return 1;
	}

	if(bSaveWave_)
	{
		CVibChannParam *pVibChannParam = NULL;
		S_VibCharValue vibValue;
		int lnDataSource = GE_DATA_SOURCE_ONLINE;

		pDB_->Cmd("UPDATE [%s] SET ",sTable_.c_str());

		for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
		{

			BOOL lbShouldSave = TRUE;

			int lnDataBufferMax = GD_MIDDLEWARE_WAVE_POINT_MAX;

			_pLockVibData->BeginRead();

			BEGIN_ERROR_HANDLE

			lnDataBufferMax = this->GetMiddleWareWavePointNumMax();
			pVibChannParam=(CVibChannParam*)(_vVibChann[nloop]);


			vibValue=pVibChannParam->GetCharValueEx();
			lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;
			END_ERROR_HANDLE;

			_pLockVibData->EndRead();


			if(vibValue._iSmpNum > lnDataBufferMax)
			{
				vibValue._iSmpNum = lnDataBufferMax;
			}

			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibValue.GetSpecTypeData(GE_ALLCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibValue.GetSpecTypeData(GE_ONECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibValue.GetSpecTypeData(GE_TWOCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibValue.GetSpecTypeData(GE_THREECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibValue.GetSpecTypeData(GE_HALFCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibValue.GetSpecTypeData(GE_RESCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARONE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTWO));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARHALF));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibValue.GetSpecTypeData(GE_DCCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibValue.GetSpecTypeData(GE_GAP));
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);

			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (!(pDB_->Exec()))
		{
			return -1;
		}

		pDB_->Cmd("SELECT 1");
		for (nloop=lnNum;nloop<lnVibChannNum;++nloop)
		{	
			pDB_->Cmd(",%s",_vVibWaveField[nloop]);
		}
		pDB_->Cmd(" FROM [%s] ",sTable_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (pDB_->More(TRUE))
		{
			int lnDataBufferMax = GD_MIDDLEWARE_WAVE_POINT_MAX;

			int iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

			vector<BYTE> lvWave(iAllBytes);
			vector<float> lvCustomData(GD_CUSTOMPARAM_BUFFSIZE);

			BYTE  *pBuf=&lvWave.front();
			float *pfCustomData=&lvCustomData.front();

			for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
			{

				bool lbShouldSave = TRUE;
				int iRealSmpNum=0;
				int iDataSource = 0;
				int lnTotalSize = 0;

				_pLockVibData->BeginRead();

				BEGIN_ERROR_HANDLE
				pVibChannParam=(CVibChannParam*)(_vVibChann[nloop]);

				lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

				//特征值全部为0，且配置了不保存空特征值则跳过

				iRealSmpNum = pVibChannParam->GetCharValueEx()._iSmpNum;
				iDataSource = pVibChannParam->GetChannInfo()._iDataSource;

				if(iRealSmpNum > lnDataBufferMax)
				{
					iRealSmpNum = lnDataBufferMax;
				}

				CWaveHead lsHead = pVibChannParam->WaveHead();
				lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iRealSmpNum,lsHead,pBuf);

				//memcpy(pBuf,pVibChannParam->GetCurrWavePoint(),iRealSmpNum*sizeof(DATATYPE_WAVE));

				memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
				END_ERROR_HANDLE;

				_pLockVibData->EndRead();

				bool lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);

				lbRet =pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			}

		}

	}
	else
	{
		CVibChannParam *pVibChannParam = NULL;
		S_VibCharValue vibValue;
		int lnDataSource = GE_DATA_SOURCE_ONLINE;

		pDB_->Cmd("UPDATE [%s] SET ",sTable_.c_str());

		for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
		{

			BOOL lbShouldSave = TRUE;

			_pLockVibData->BeginRead();

			BEGIN_ERROR_HANDLE
			pVibChannParam=(CVibChannParam*)(_vVibChann[nloop]);

			vibValue=pVibChannParam->GetCharValueEx();
			lnDataSource = pVibChannParam->GetChannInfo()._iDataSource;
			END_ERROR_HANDLE

			_pLockVibData->EndRead();

			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibValue.GetSpecTypeData(GE_ALLCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibValue.GetSpecTypeData(GE_ONECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibValue.GetSpecTypeData(GE_TWOCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibValue.GetSpecTypeData(GE_THREECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibValue.GetSpecTypeData(GE_HALFCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibValue.GetSpecTypeData(GE_RESCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARONE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTWO));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARHALF));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibValue.GetSpecTypeData(GE_DCCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibValue.GetSpecTypeData(GE_GAP));
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cTempID);
			pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}

		pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (!(pDB_->Exec()))
		{
			return -1;
		}
	}

	return 1;
}

int CRealPlantInfo::InsertDynFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_ )
{
	int lnDynChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();

	_pLockDynData->BeginRead();
	lnDynChannNum = this->GetDynChannNum();
	_pLockDynData->EndRead();

	if(iStartIndex_ <= 0 || (iLen_ + iStartIndex_ > lnDynChannNum) || iLen_ <=0)
	{
		return 1;
	}

	if(this->InsertDynFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,iStartIndex_,bSaveWave_) > 0)
	{
		return 1;
	}

	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR || 
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}
	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_DynCharValue dynValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev( pfRev, 5);
	GetRealSmpInfo(iSmpNum, iSmpFreq);


	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*	int iSmpNumMax = GetPlantInfo()._iSmpNum;*/

	int iSmpNumMax = iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{
		iSaveWaveNum=iSmpNumMax;
	}

	ASSERT(iSaveWaveNum >0);

	int lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

	iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

	V_PCHANNINFO::iterator it;
	CDynChannParam *pDynChannParam=NULL;

	//插入记录
	pDB_->ClearSql();

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sDynTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s ",_sDynTrendFastTblName.c_str(),gc_cID);

		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" VALUES (%d,",iCurID_);
		int lnCharIndex = -1;
		for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
		{
			lnCharIndex++;
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam=(CDynChannParam*)(*it);
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			int lnDataSource = pDynChannParam->GetChannInfo()._iDataSource;

			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE;
			dynValue=pDynChannParam->GetCharValueEx();
			ltUpdateTime  = this->_timeDynDataUpdate;
			END_ERROR_HANDLE;

			_pLockDynData->EndRead();

			index ++;

			iSaveWaveNum = dynValue._iSmpNum;

			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}

			if (iSaveWaveNum != 0)
			{
				iSmpNum=iSaveWaveNum;
			}		

			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_ALLCHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_ONECHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_TWOCHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_THREECHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_HALFCHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_RESCHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_DCCHAR));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_GAP));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_PHASECHARONE));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_PHASECHARTWO));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_PHASECHARTHREE));
			pDB_->Cmd("%.2f,",dynValue.GetSpecTypeData(GE_PHASECHARHALF));
			pDB_->Cmd("%d,",dynValue._iSmpFreq);
			pDB_->Cmd("%d,",iSaveWaveNum);
			ASSERT(iSmpNum != 0);
			pDB_->Cmd("%d,",dynValue._iRev);
			pDB_->Cmd("'%s',",pDynChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd("%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);

		for (int nloop=0;nloop<5;++nloop)
		{	pDB_->Cmd("%.2f,",pfRev[nloop]);
		}

		pDB_->Cmd("'%s',",dtTime_.Format(DATE_TIME_FORMAT));
		pDB_->Cmd("%d,",0);
		pDB_->Cmd("%d)",0);
	}
	else
	{
		pDB_->Cmd("UPDATE [%s] SET ",_sDynTrendFastTblName.c_str());

		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			dynValue = pDynChannParam->GetCharValueEx();
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(0).c_str(),dynValue._fCharValue0);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(1).c_str(),dynValue._fCharValue1);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(2).c_str(),dynValue._fCharValue2);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(3).c_str(),dynValue._fCharValue3);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(4).c_str(),dynValue._fCharValue4);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(5).c_str(),dynValue._fCharValue5);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(6).c_str(),dynValue._fCharValue6);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(7).c_str(),dynValue._fCharValue7);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(8).c_str(),dynValue._fCharValue8);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(9).c_str(),dynValue._fCharValue9);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(10).c_str(),dynValue._fCharValue10);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(11).c_str(),dynValue._fCharValue11);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),dynValue._iSmpFreq);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),dynValue._iSmpNum);
			pDB_->Cmd("[%s] = %.2f,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),dynValue._iRev);
			pDB_->Cmd("[%s] = '%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd("[%s] = %d,",gc_cSampleNum,iSmpNum);
		pDB_->Cmd("[%s] = %d,",gc_cSampleFreq,iSmpFreq);
		pDB_->Cmd("[%s] = %d,",gc_cRev,pfRev[0]);
		pDB_->Cmd("[%s] = %d,",gc_cRev2,pfRev[1]);
		pDB_->Cmd("[%s] = %d,",gc_cRev3,pfRev[2]);
		pDB_->Cmd("[%s] = %d,",gc_cRev4,pfRev[3]);
		pDB_->Cmd("[%s] = %d,",gc_cRev5,pfRev[4]);
		pDB_->Cmd("[%s] = '%s',",gc_cTime,dtTime_.Format());
		pDB_->Cmd("[%s] = %d,",gc_cEventID,0);
		pDB_->Cmd("[%s] = %d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s] = %d",gc_cID,iCurID_);
	}
	

	if(!pDB_->Exec())
	{
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveFastDynRecord");
		return -1;
	}

	if(bSaveWave_)
	{
		pDB_->Cmd("SELECT %s FROM [%s]",_sDynFieldSel.c_str(),_sDynTrendFastTblName.c_str());
		pDB_->Cmd(" WHERE %s=%d",gc_cID,iCurID_);

		if(pDB_->More(TRUE))
		{
			index=0;
			int lnCharIndex = -1;
			BYTE  *pBuf=new BYTE[iAllBytes];
			float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
			int iRealSmpNum= 0;
			for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
			{
				int lnTotalSize = 0;
				try
				{
					lnCharIndex++;
					

					_pLockDynData->BeginRead();
					BEGIN_ERROR_HANDLE;

					pDynChannParam=(CDynChannParam*)(*it);

					int lnDataSource = pDynChannParam->GetChannInfo()._iDataSource;

					iRealSmpNum=pDynChannParam->GetCharValue()._iSmpNum;

					iSaveWaveNum = iRealSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}

					CWaveHead lsHead = pDynChannParam->WaveHead();
					lnTotalSize = WaveAddHead(pDynChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

					memcpy(pfCustomData,(&pDynChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
					dynValue = pDynChannParam->GetCharValueEx();

					END_ERROR_HANDLE;
					_pLockDynData->EndRead();

				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				bool lbRet = true;

				lbRet = pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
				lbRet = pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			}
			KD_DeletePoint(pBuf);
			KD_DeletePoint(pfCustomData);
		}
		else
		{
			CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveDynRecord");
		}
	}
	else
	{
		pDB_->Cmd("UPDATE %s SET %s = %d ",_sDynTrendFastTblName.c_str(),gc_cID,iCurID_);
		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam *)_vDynChann[nloop];
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(" ,%s = NULL",pDynChannParam->GetSpecCharTypeField(-1));
			pDB_->Cmd(" ,%s = NULL",pDynChannParam->GetSpecCharTypeField(-7));
		}

		pDB_->Cmd(" WHERE %s = %d",gc_cID,iCurID_);

		pDB_->Exec();
	}
	
	return 1;
}

int CRealPlantInfo::UpdateDynFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_ )
{
	int lnDynChannNum = 0;
	_pLockDynData->BeginRead();
	lnDynChannNum = this->GetDynChannNum();
	_pLockDynData->EndRead();

	if(iStartIndex_ != 0 || (iStartIndex_ == 0 && iLen_ + iStartIndex_ >= lnDynChannNum) || iLen_ <= 0 )
	{
		return 1;
	}

	int lnNum = iStartIndex_ + iLen_;

	if(this->UpdateVibFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,lnNum,bSaveWave_) > 0)
	{
		return 1;
	}
	int nloop = 0;
	if(bSaveWave_)
	{
		CDynChannParam *pDynChannParam = NULL;
		S_DynCharValue DynValue;
		int lnDataSource = GE_DATA_SOURCE_ONLINE;

		pDB_->Cmd("UPDATE [%s] SET ",sTable_.c_str());

		for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
		{

			BOOL lbShouldSave = TRUE;

			int lnDataBufferMax  = GD_MIDDLEWARE_WAVE_POINT_MAX;

			_pLockDynData->BeginRead();

			BEGIN_ERROR_HANDLE

			lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

			pDynChannParam=(CDynChannParam*)(_vDynChann[nloop]);

			DynValue=pDynChannParam->GetCharValueEx();
			lnDataSource = pDynChannParam->GetChannInfo()._iDataSource;
			END_ERROR_HANDLE

			_pLockDynData->EndRead();


			if(DynValue._iSmpNum > lnDataBufferMax)
			{
				DynValue._iSmpNum = lnDataBufferMax;
			}

			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),DynValue.GetSpecTypeData(0));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),DynValue.GetSpecTypeData(1));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),DynValue.GetSpecTypeData(2));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),DynValue.GetSpecTypeData(3));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),DynValue.GetSpecTypeData(4));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),DynValue.GetSpecTypeData(5));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),DynValue.GetSpecTypeData(6));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),DynValue.GetSpecTypeData(7));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),DynValue.GetSpecTypeData(8));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),DynValue.GetSpecTypeData(9));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),DynValue.GetSpecTypeData(10));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),DynValue.GetSpecTypeData(11));
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),DynValue._iSmpFreq);

			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),DynValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),DynValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetChannInfo()._cTempID);
		}

		pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (!(pDB_->Exec()))
		{
			return -1;
		}

		pDB_->Cmd("SELECT 1");
		for (nloop=lnNum;nloop<lnDynChannNum;++nloop)
		{	
			pDB_->Cmd(",%s",_vDynWaveField[nloop]);
		}
		pDB_->Cmd(" FROM [%s] ",sTable_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (pDB_->More(TRUE))
		{
			int lnDataBufferMax = GD_MIDDLEWARE_WAVE_POINT_MAX;

			int iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

			vector<BYTE> lvWave(iAllBytes);
			vector<float> lvCustomData(GD_CUSTOMPARAM_BUFFSIZE);

			BYTE  *pBuf=&lvWave.front();
			float *pfCustomData=&lvCustomData.front();

			int lnCharIndex = -1;
			for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
			{

				bool lbShouldSave = TRUE;
				int iRealSmpNum=0;
				int iDataSource = 0;

				int lnTotalSize = 0;

				_pLockDynData->BeginRead();

				BEGIN_ERROR_HANDLE;
				pDynChannParam=(CDynChannParam*)(_vDynChann[nloop]);
				lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

				iRealSmpNum = pDynChannParam->GetCharValueEx()._iSmpNum;
				iDataSource = pDynChannParam->GetChannInfo()._iDataSource;
				

				if(iRealSmpNum > lnDataBufferMax)
				{
					iRealSmpNum = lnDataBufferMax;
				}

				CWaveHead lsHead = pDynChannParam->WaveHead();
				lnTotalSize = WaveAddHead(pDynChannParam->GetCurrWavePoint(),iRealSmpNum,lsHead,pBuf);


				memcpy(pfCustomData,(&pDynChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);

				END_ERROR_HANDLE;
				_pLockDynData->EndRead();

				bool lbRet = pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);

				lbRet =pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			}

		}

	}
	else
	{
		CDynChannParam *pDynChannParam = NULL;
		S_DynCharValue DynValue;
		int lnDataSource = GE_DATA_SOURCE_ONLINE;

		pDB_->Cmd("UPDATE [%s] SET ",sTable_.c_str());

		for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
		{

			BOOL lbShouldSave = TRUE;

			_pLockDynData->BeginRead();

			BEGIN_ERROR_HANDLE;
			pDynChannParam=(CDynChannParam*)(_vDynChann[nloop]);

			DynValue=pDynChannParam->GetCharValueEx();
			lnDataSource = pDynChannParam->GetChannInfo()._iDataSource;
			END_ERROR_HANDLE;

			_pLockDynData->EndRead();

			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),DynValue.GetSpecTypeData(0));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),DynValue.GetSpecTypeData(1));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),DynValue.GetSpecTypeData(2));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),DynValue.GetSpecTypeData(3));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),DynValue.GetSpecTypeData(4));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),DynValue.GetSpecTypeData(5));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),DynValue.GetSpecTypeData(6));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),DynValue.GetSpecTypeData(7));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),DynValue.GetSpecTypeData(8));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),DynValue.GetSpecTypeData(9));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),DynValue.GetSpecTypeData(10));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),DynValue.GetSpecTypeData(11));
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),DynValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),DynValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),DynValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetChannInfo()._cTempID);
			pDB_->Cmd(" [%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}

		pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
		if (!(pDB_->Exec()))
		{
			return -1;
		}
	}

	return 1;
}

int CRealPlantInfo::InsertProcFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_ )
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int lnProcChannNum = 0;
	_pLockProcData->BeginRead();
	lnProcChannNum = this->GetProcChannNum();
	_pLockProcData->EndRead();

	if(iStartIndex_ == 0 || (iLen_ + iStartIndex_ > lnProcChannNum) || iLen_ <= 0)
	{
		return 1;
	}

	float lfData = 0;
	int lnNum = iStartIndex_ + iLen_;

	if(this->InsertProcFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,iStartIndex_) > 0)
	{
		return 1;
	}

	float pfRev[5];
	float fData=.0;
	string sTimeCurr;
	GetRealRev( pfRev, 5);

	CProcChannParam *pProcChann=NULL;

	int nloop = 0;
	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sProcTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		pDB_->Cmd("INSERT INTO [%s](%s ",_sProcTrendFastTblName.c_str(),gc_cID);
		for(nloop = 0; nloop < lnProcChannNum;++nloop)
		{
			_pLockProcData->BeginRead();
			pProcChann = (CProcChannParam*)_vProcChann[nloop];
			_pLockProcData->EndRead();

			pDB_->Cmd(",[%s]",pProcChann->GetChannInfo()._cChannNo);
		}
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" VALUES (%d,",iCurID_);
		for(nloop = 0; nloop < lnProcChannNum; nloop++)
		{
			_pLockProcData->BeginRead();
			try
			{
				pProcChann = (CProcChannParam *)_vProcChann[nloop];

				fData=pProcChann->GetData();
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockProcData->EndRead();

			pDB_->Cmd("%.2f,",fData);
		}
		pDB_->Cmd("'%s',",TimeConvertString(dtTime_));
		pDB_->Cmd(" %.2f,",pfRev[0]);
		pDB_->Cmd(" %.2f,",pfRev[1]);
		pDB_->Cmd(" %.2f,",pfRev[2]);
		pDB_->Cmd(" %.2f,",pfRev[3]);
		pDB_->Cmd(" %.2f,",pfRev[4]);
		pDB_->Cmd(" %d,",0);
		pDB_->Cmd(" %d)",0);
		if(!pDB_->Exec())
		{
			CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
			return -1;
		}
	}
	else
	{
		pDB_->Cmd("UPDATE [%s] SET [%s] = %d, ",_sProcTrendFastTblName.c_str(),gc_cEventID,0);
		for(nloop = 0; nloop < lnProcChannNum; nloop++)
		{
			_pLockProcData->BeginRead();
			try
			{
				pProcChann = (CProcChannParam *)_vProcChann[nloop];

				fData=pProcChann->GetData();
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockProcData->EndRead();

			pDB_->Cmd(" [%s] = %.2f,",fData);
		}
		pDB_->Cmd("[%s] = '%s',",gc_cTime,TimeConvertString(dtTime_));
		pDB_->Cmd("[%s] = %.2f,",gc_cRev,pfRev[0]);
		pDB_->Cmd("[%s] = %.2f,",gc_cRev2,pfRev[1]);
		pDB_->Cmd("[%s] = %.2f,",gc_cRev3,pfRev[2]);
		pDB_->Cmd("[%s] = %.2f,",gc_cRev4,pfRev[3]);
		pDB_->Cmd("[%s] = %.2f,",gc_cRev5,pfRev[4]);
		pDB_->Cmd(" [%s] = %d",gc_cSynFlag,0);
		pDB_->Cmd(" WHERE %s = %d",gc_cID,iCurID_);
		if(!pDB_->Exec())
		{
			CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
			return -1;
		}

	}
	

	return 1;
}

int CRealPlantInfo::UpdateProcFastDataFromTrendData( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_ )
{
	int lnProcChannNum = 0;
	_pLockProcData->BeginRead();
	lnProcChannNum = this->GetProcChannNum();
	_pLockProcData->EndRead();

	if(iStartIndex_ != 0 || (iStartIndex_ == 0 && iLen_ + iStartIndex_ >= lnProcChannNum) || iLen_ <= 0)
	{
		return 1;
	}

	float lfData = 0;
	int lnNum = iStartIndex_ + iLen_;

	if(this->UpdateProcFastDataFromTrendTable(pDB_,sTable_,iCurID_,dtTime_,lnNum) > 0)
	{
		return 1;
	}
	CProcChannParam *pProcChann = NULL;

	pDB_->Cmd("UPDATE [%s] SET  ",sTable_.c_str());
	for(int nloop = lnNum; nloop < lnProcChannNum; ++nloop)
	{
		_pLockProcData->BeginRead();
		BEGIN_ERROR_HANDLE
			pProcChann=(CProcChannParam*)(_vProcChann[nloop]);
		lfData=pProcChann->GetData();
		END_ERROR_HANDLE
			_pLockProcData->EndRead();

		pDB_->Cmd(" [%s]=%.2f,",pProcChann->GetChannInfo()._cChannNo,lfData);
	}

	pDB_->Cmd(" [%s]=%d ",gc_cSynFlag,0);
	pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,iCurID_);
	if (!(pDB_->Exec()))
	{
		return -1;
	}
	return 1;
}

int CRealPlantInfo::InsertVibFastDataFromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_ )
{
	CBHDateTime ltUpdateTime = CBHDateTime(2000,1,1,1,1,1);
	CBHDateTime ltFastTblTimeFormer = ltUpdateTime;
	CBHDateTime ltFastTblTimeLater = ltUpdateTime;
	CBHDateTime ltTrendTblTimeFormer = ltUpdateTime;
	CBHDateTime ltTrendTblTimeLater = ltUpdateTime;

	int lnVibChannNum = 0;
	_pLockVibData->BeginRead();
	ltUpdateTime = this->_timeVibDataUpdate;
	lnVibChannNum = this->GetVibChannNum();
	_pLockVibData->EndRead();

	string lstrTable;
	CBHDateTime ltTime;

	//查找当前时间之前快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sVibTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sVibTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之前秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	double ldTrend = abs(dtTime_ - ltUpdateTime);
	double ldFTblF = abs(dtTime_ - ltFastTblTimeFormer);
	double ldFTblL = abs(dtTime_ - ltFastTblTimeLater);
	double ldTTblF = abs(dtTime_ - ltTrendTblTimeFormer);
	double ldTTblL = abs(dtTime_ - ltTrendTblTimeLater);

	//当前数据时间距实时缓存数据最近
	if(ldTrend < ldFTblF && ldTrend < ldFTblL && ldTrend < ldTTblF && ldTrend < ldTTblL)
	{
		return -1;
	}

	//寻找离当前数据最近时间点的表和记录
	if(ldFTblF > ldFTblL)
	{
		ldFTblF = ldFTblL;
		ltFastTblTimeFormer = ltFastTblTimeLater;
	}

	if(ldTTblF > ldTTblL)
	{
		ldTTblF = ldTTblL;
		ltTrendTblTimeFormer = ltTrendTblTimeLater;
	}

	if(ldFTblF < ldTTblF)
	{
		lstrTable = _sVibTrendFastTblName;
		ltTime = ltFastTblTimeFormer;
	}
	else
	{
		lstrTable = _sVibTrendSecTblName;
		ltTime = ltTrendTblTimeFormer;
	}


	pDB_->ClearSql();
	CVibChannParam *pVibChannParam = NULL;
	int nloop = 0;

	pDB_->Cmd("SELECT [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sVibTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		pDB_->Cmd("INSERT INTO [%s](%s ",_sVibTrendFastTblName.c_str(),gc_cID);

		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
			if(bSaveWave_)
			{
				pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" select %d",iCurID_);
		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam*)_vVibChann[nloop];
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
			if(bSaveWave_)
			{
				pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",'%s'",TimeConvertString(dtTime_));
		pDB_->Cmd(",%d",0);
		pDB_->Cmd(",%d ",0);
		pDB_->Cmd(" from %s where %s = '%s'",lstrTable.c_str(),gc_cTime,TimeConvertString(ltTime));
		if(!pDB_->Exec())
		{
			return -1;
		}
	}
	else
	{
		pDB_->Cmd("UPDATE a SET");

		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam=(CVibChannParam*)(_vVibChann[nloop]);
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str(),pVibChannParam->GetSpecCharTypeField(-3).c_str());

			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str(),pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str(),pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetSpecCharTypeField(-6).c_str());

			if(bSaveWave_)
			{
				pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str(),pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str(),pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}
			else
			{
				pDB_->Cmd(" a.[%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" a.[%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}

		}
		pDB_->Cmd(" a.[%s] = %d,",gc_cSynFlag,0);
		pDB_->Cmd(" a.[%s] = %d,",gc_cEventID,0);
		pDB_->Cmd(" a.[%s]='%s',",gc_cTime,dtTime_.Format());
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cSampleFreq,gc_cSampleFreq);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cSampleNum,gc_cSampleNum);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev,gc_cRev);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev2,gc_cRev2);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev3,gc_cRev3);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev4,gc_cRev4);
		pDB_->Cmd(" a.[%s]=b.[%s]",gc_cRev5,gc_cRev5);

		pDB_->Cmd(" FROM %s a, %s b where a.%s = %d and b.%s = '%s'",
			_sVibTrendFastTblName.c_str(),lstrTable.c_str(),
			gc_cID,iCurID_,gc_cTime,ltTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}
	}
	

	return 1;
}

int CRealPlantInfo::UpdateVibFastDataFromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_ )
{
	int lnVibChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTime = CBHDateTime::GetCurrentTimeEx();
	_pLockVibData->BeginRead();
	lnVibChannNum = this->GetVibChannNum();
	ltUpdateTime = this->_timeVibDataUpdate;
	_pLockVibData->EndRead();

	int nloop = 0;

	int lnNum = iStartIndex_;

	CVibChannParam *pVibChannParam = NULL;
	S_VibCharValue vibValue;

	COleDateTimeSpan ltTimeSpan;

	//快变数据时间点与趋势数据更新时间在10秒内，使用趋势数据
	if(ltUpdateTime >= dtTime_)
	{
		ltTimeSpan = ltUpdateTime - dtTime_;
	}
	else
	{
		ltTimeSpan = dtTime_ - ltUpdateTime;
	}

	if(ltTimeSpan.GetTotalSeconds() <= 5)
	{
		return -1;
	}


	//秒表中距离快变时间点最近的一条记录与快变时间差超过快变时间与趋势更新时间差，使用趋势数据保存
	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s < '%s' ORDER BY %s DESC",
		gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTime = pDB_->GetTime(gc_cTime);
	}
	else
	{
		return -1;
	}

	if(abs(dtTime_ - ltUpdateTime) < abs(dtTime_-ltTrendDataTime))
	{
		return -1;
	}


	pDB_->Cmd("UPDATE a SET");

	for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
	{
		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE
			pVibChannParam=(CVibChannParam*)(_vVibChann[nloop]);
		END_ERROR_HANDLE
			_pLockVibData->EndRead();

		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str(),pVibChannParam->GetSpecCharTypeField(-3).c_str());

		pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str(),pVibChannParam->GetSpecCharTypeField(-4).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str(),pVibChannParam->GetSpecCharTypeField(-5).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetSpecCharTypeField(-6).c_str());

		if(bSaveWave_)
		{
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str(),pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str(),pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}
		else
		{
			pDB_->Cmd(" a.[%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=NULL,",pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}

	}
	pDB_->Cmd(" a.[%s] = %d",gc_cSynFlag,0);
	pDB_->Cmd(" FROM %s a, %s b where a.%s = '%s' and b.%s = '%s'",
		sTable_.c_str(),_sVibTrendSecTblName.c_str(),
		gc_cTime,dtTime_.Format(),gc_cTime,ltTrendDataTime.Format());
	if(pDB_->Exec())
	{
		return 1;
	}

	return -1;
}

int CRealPlantInfo::InsertDynFastDataFromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_ )
{
	CBHDateTime ltUpdateTime = CBHDateTime(2000,1,1,1,1,1);
	CBHDateTime ltFastTblTimeFormer = ltUpdateTime;
	CBHDateTime ltFastTblTimeLater = ltUpdateTime;
	CBHDateTime ltTrendTblTimeFormer = ltUpdateTime;
	CBHDateTime ltTrendTblTimeLater = ltUpdateTime;

	int lnDynChannNum = 0;
	_pLockDynData->BeginRead();
	ltUpdateTime = this->_timeDynDataUpdate;
	lnDynChannNum = this->GetDynChannNum();
	_pLockDynData->EndRead();

	string lstrTable;
	CBHDateTime ltTime;

	//查找当前时间之前快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sDynTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sDynTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之前秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	double ldTrend = abs(dtTime_ - ltUpdateTime);
	double ldFTblF = abs(dtTime_ - ltFastTblTimeFormer);
	double ldFTblL = abs(dtTime_ - ltFastTblTimeLater);
	double ldTTblF = abs(dtTime_ - ltTrendTblTimeFormer);
	double ldTTblL = abs(dtTime_ - ltTrendTblTimeLater);

	//当前数据时间距实时缓存数据最近
	if(ldTrend < ldFTblF && ldTrend < ldFTblL && ldTrend < ldTTblF && ldTrend < ldTTblL)
	{
		return -1;
	}

	//寻找离当前数据最近时间点的表和记录
	if(ldFTblF > ldFTblL)
	{
		ldFTblF = ldFTblL;
		ltFastTblTimeFormer = ltFastTblTimeLater;
	}

	if(ldTTblF > ldTTblL)
	{
		ldTTblF = ldTTblL;
		ltTrendTblTimeFormer = ltTrendTblTimeLater;
	}

	if(ldFTblF < ldTTblF)
	{
		lstrTable = _sDynTrendFastTblName;
		ltTime = ltFastTblTimeFormer;
	}
	else
	{
		lstrTable = _sDynTrendSecTblName;
		ltTime = ltTrendTblTimeFormer;
	}


	pDB_->ClearSql();
	CDynChannParam *pDynChannParam = NULL;
	int nloop = 0;
	
	pDB_->Cmd("SELECT [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sDynTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		pDB_->Cmd("INSERT INTO [%s](%s ",_sDynTrendFastTblName.c_str(),gc_cID);

		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
			if(bSaveWave_)
			{
				pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" select %d",iCurID_);
		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
			if(bSaveWave_)
			{
				pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}
		}

		pDB_->Cmd(",[%s]",gc_cSampleNum);
		pDB_->Cmd(",[%s]",gc_cSampleFreq);
		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",'%s'",TimeConvertString(dtTime_));
		pDB_->Cmd(",%d",0);
		pDB_->Cmd(",%d ",0);
		pDB_->Cmd(" from %s where %s = '%s'",lstrTable.c_str(),gc_cTime,TimeConvertString(ltTime));
		if(!pDB_->Exec())
		{
			return -1;
		}

	}
	else
	{
		pDB_->Cmd("UPDATE a SET");

		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam=(CDynChannParam*)(_vDynChann[nloop]);
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(0).c_str(),pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(1).c_str(),pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(2).c_str(),pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(3).c_str(),pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(4).c_str(),pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(5).c_str(),pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(6).c_str(),pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(7).c_str(),pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(8).c_str(),pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(9).c_str(),pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(10).c_str(),pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(11).c_str(),pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str(),pDynChannParam->GetSpecCharTypeField(-3).c_str());

			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str(),pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str(),pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetSpecCharTypeField(-6).c_str());
			if(bSaveWave_)
			{
				pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str(),pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str(),pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}
			else
			{
				pDB_->Cmd(" a.[%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" a.[%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}

		}
		pDB_->Cmd(" a.[%s] = %d,",gc_cSynFlag,0);
		pDB_->Cmd(" a.[%s] = %d,",gc_cEventID,0);
		pDB_->Cmd(" a.[%s]='%s',",gc_cTime,dtTime_.Format());
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cSampleFreq,gc_cSampleFreq);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cSampleNum,gc_cSampleNum);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev,gc_cRev);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev2,gc_cRev2);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev3,gc_cRev3);
		pDB_->Cmd(" a.[%s]=b.[%s],",gc_cRev4,gc_cRev4);
		pDB_->Cmd(" a.[%s]=b.[%s]",gc_cRev5,gc_cRev5);

		pDB_->Cmd(" FROM %s a, %s b where a.%s = %d and b.%s = '%s'",
			_sDynTrendFastTblName.c_str(),lstrTable.c_str(),
			gc_cID,iCurID_,gc_cTime,ltTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}
	}
	
	return 1;
}

int CRealPlantInfo::UpdateDynFastDataromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_ )
{
	int lnDynChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTime = CBHDateTime::GetCurrentTimeEx();
	_pLockDynData->BeginRead();
	lnDynChannNum = this->GetDynChannNum();
	ltUpdateTime = this->_timeDynDataUpdate;
	_pLockDynData->EndRead();

	int nloop = 0;

	int lnNum = iStartIndex_;

	CDynChannParam *pDynChannParam = NULL;

	COleDateTimeSpan ltTimeSpan;

	//快变数据时间点与趋势数据更新时间在10秒内，使用趋势数据
	if(ltUpdateTime >= dtTime_)
	{
		ltTimeSpan = ltUpdateTime - dtTime_;
	}
	else
	{
		ltTimeSpan = dtTime_ - ltUpdateTime;
	}

	if(ltTimeSpan.GetTotalSeconds() <= 5)
	{
		return -1;
	}

	//秒表中距离快变时间点最近的一条记录与快变时间差超过快变时间与趋势更新时间差，使用趋势数据保存
	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s < '%s' ORDER BY %s DESC",
		gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTime = pDB_->GetTime(gc_cTime);
	}
	else
	{
		return -1;
	}

	if(abs(dtTime_ - ltUpdateTime) < abs(dtTime_-ltTrendDataTime))
	{
		return -1;
	}

	pDB_->Cmd("UPDATE a SET");

	for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
	{
		_pLockDynData->BeginRead();
		BEGIN_ERROR_HANDLE
			pDynChannParam=(CDynChannParam*)(_vDynChann[nloop]);
		END_ERROR_HANDLE
			_pLockDynData->EndRead();

		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(0).c_str(),pDynChannParam->GetSpecCharTypeField(0).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(1).c_str(),pDynChannParam->GetSpecCharTypeField(1).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(2).c_str(),pDynChannParam->GetSpecCharTypeField(2).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(3).c_str(),pDynChannParam->GetSpecCharTypeField(3).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(4).c_str(),pDynChannParam->GetSpecCharTypeField(4).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(5).c_str(),pDynChannParam->GetSpecCharTypeField(5).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(6).c_str(),pDynChannParam->GetSpecCharTypeField(6).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(7).c_str(),pDynChannParam->GetSpecCharTypeField(7).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(8).c_str(),pDynChannParam->GetSpecCharTypeField(8).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(9).c_str(),pDynChannParam->GetSpecCharTypeField(9).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(10).c_str(),pDynChannParam->GetSpecCharTypeField(10).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(11).c_str(),pDynChannParam->GetSpecCharTypeField(11).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str(),pDynChannParam->GetSpecCharTypeField(-3).c_str());

		pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str(),pDynChannParam->GetSpecCharTypeField(-4).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str(),pDynChannParam->GetSpecCharTypeField(-5).c_str());
		pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetSpecCharTypeField(-6).c_str());
		if(bSaveWave_)
		{
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str(),pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str(),pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}
		else
		{
			pDB_->Cmd(" a.[%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=NULL,",pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}

	}
	pDB_->Cmd(" a.[%s] = %d",gc_cSynFlag,0);
	pDB_->Cmd(" FROM %s a, %s b where a.%s = '%s' and b.%s = '%s'",
		sTable_.c_str(),_sDynTrendSecTblName.c_str(),
		gc_cTime,dtTime_.Format(),gc_cTime,ltTrendDataTime.Format());
	if(pDB_->Exec())
	{
		return 1;
	}

	return -1;
}

int CRealPlantInfo::InsertProcFastDataFromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_)
{
	CBHDateTime ltUpdateTime = CBHDateTime(2000,1,1,1,1,1);
	CBHDateTime ltFastTblTimeFormer = ltUpdateTime;
	CBHDateTime ltFastTblTimeLater = ltUpdateTime;
	CBHDateTime ltTrendTblTimeFormer = ltUpdateTime;
	CBHDateTime ltTrendTblTimeLater = ltUpdateTime;

	int lnProcChannNum = 0;
	_pLockProcData->BeginRead();
	ltUpdateTime = this->_timeProcDataUpdate;
	lnProcChannNum = this->GetProcChannNum();
	_pLockProcData->EndRead();

	string lstrTable;
	CBHDateTime ltTime;

	//查找当前时间之前快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sProcTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后快变表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sProcTrendFastTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltFastTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之前秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s <= '%s' order by %s desc",\
		gc_cTime,_sProcTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeFormer = pDB_->GetTime(gc_cTime);
	}

	//查找当前时间之后秒表最近一条记录时间
	pDB_->Cmd("select top 1 %s from %s where %s >= '%s' order by %s asc",\
		gc_cTime,_sProcTrendSecTblName.c_str(),gc_cTime,TimeConvertString(dtTime_),gc_cTime);
	if(pDB_->More())
	{
		ltTrendTblTimeLater = pDB_->GetTime(gc_cTime);
	}

	double ldTrend = abs(dtTime_ - ltUpdateTime);
	double ldFTblF = abs(dtTime_ - ltFastTblTimeFormer);
	double ldFTblL = abs(dtTime_ - ltFastTblTimeLater);
	double ldTTblF = abs(dtTime_ - ltTrendTblTimeFormer);
	double ldTTblL = abs(dtTime_ - ltTrendTblTimeLater);

	//当前数据时间距实时缓存数据最近
	if(ldTrend < ldFTblF && ldTrend < ldFTblL && ldTrend < ldTTblF && ldTrend < ldTTblL)
	{
		return -1;
	}

	//寻找离当前数据最近时间点的表和记录
	if(ldFTblF > ldFTblL)
	{
		ldFTblF = ldFTblL;
		ltFastTblTimeFormer = ltFastTblTimeLater;
	}

	if(ldTTblF > ldTTblL)
	{
		ldTTblF = ldTTblL;
		ltTrendTblTimeFormer = ltTrendTblTimeLater;
	}

	if(ldFTblF < ldTTblF)
	{
		lstrTable = _sProcTrendFastTblName;
		ltTime = ltFastTblTimeFormer;
	}
	else
	{
		lstrTable = _sProcTrendSecTblName;
		ltTime = ltTrendTblTimeFormer;
	}


	pDB_->ClearSql();
	CProcChannParam *pProcChannParam = NULL;
	int nloop = 0;
	
	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d",gc_cID,_sProcTrendFastTblName.c_str(),gc_cID,iCurID_);
	if(!pDB_->More())
	{
		//不存在这条记录，就插入。
		pDB_->Cmd("INSERT INTO [%s](%s ",_sProcTrendFastTblName.c_str(),gc_cID);

		for(nloop = 0; nloop < lnProcChannNum; ++nloop)
		{
			_pLockProcData->BeginRead();
			BEGIN_ERROR_HANDLE
			pProcChannParam = (CProcChannParam*)_vProcChann[nloop];
			END_ERROR_HANDLE
			_pLockProcData->EndRead();

			pDB_->Cmd(",[%s]",pProcChannParam->GetChannInfo()._cChannNo);
		}

		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",[%s]",gc_cTime);
		pDB_->Cmd(",[%s]",gc_cEventID);
		pDB_->Cmd(",[%s])",gc_cSynFlag);

		pDB_->Cmd(" select %d",iCurID_);
		for(nloop = 0; nloop < lnProcChannNum; ++nloop)
		{
			_pLockProcData->BeginRead();
			BEGIN_ERROR_HANDLE
			pProcChannParam = (CProcChannParam*)_vProcChann[nloop];
			END_ERROR_HANDLE
			_pLockProcData->EndRead();

			pDB_->Cmd(",[%s]",pProcChannParam->GetChannInfo()._cChannNo);
		}

		pDB_->Cmd(",[%s]",gc_cRev);
		pDB_->Cmd(",[%s]",gc_cRev2);
		pDB_->Cmd(",[%s]",gc_cRev3);
		pDB_->Cmd(",[%s]",gc_cRev4);
		pDB_->Cmd(",[%s]",gc_cRev5);
		pDB_->Cmd(",'%s'",TimeConvertString(dtTime_));
		pDB_->Cmd(",%d",0);
		pDB_->Cmd(",%d ",0);
		pDB_->Cmd(" from %s where %s = '%s'",lstrTable.c_str(),gc_cTime,TimeConvertString(ltTime));
		if(!pDB_->Exec())
		{
			return -1;
		}
	}
	else
	{
		pDB_->Cmd("UPDATE a SET");

		for(nloop = 0; nloop < lnProcChannNum; ++nloop)
		{
			_pLockProcData->BeginRead();
			BEGIN_ERROR_HANDLE
			pProcChannParam=(CProcChannParam*)(_vProcChann[nloop]);
			END_ERROR_HANDLE
			_pLockProcData->EndRead();

			pDB_->Cmd(" a.[%s]=b.[%s], ",pProcChannParam->GetChannInfo()._cChannNo,pProcChannParam->GetChannInfo()._cChannNo);

		}
		pDB_->Cmd(" a.[%s] = %d,",gc_cSynFlag,0);
		pDB_->Cmd(" a.[%s] = %d,",gc_cEventID,0);
		pDB_->Cmd(" a.[%s] = b.[%s],",gc_cRev,gc_cRev);
		pDB_->Cmd(" a.[%s] = b.[%s],",gc_cRev2,gc_cRev2);
		pDB_->Cmd(" a.[%s] = b.[%s],",gc_cRev3,gc_cRev3);
		pDB_->Cmd(" a.[%s] = b.[%s],",gc_cRev4,gc_cRev4);
		pDB_->Cmd(" a.[%s] = b.[%s] ",gc_cRev5,gc_cRev5);

		pDB_->Cmd(" FROM %s a, %s b where a.%s = %d and b.%s = '%s'",
			_sProcTrendFastTblName.c_str(),lstrTable.c_str(),
			gc_cID,iCurID_,gc_cTime,ltTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}
	}
	

	return 1;
}

int CRealPlantInfo::UpdateProcFastDataFromTrendTable( IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_ )
{
	int lnProcChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTime = CBHDateTime::GetCurrentTimeEx();
	_pLockProcData->BeginRead();
	lnProcChannNum = this->GetProcChannNum();
	ltUpdateTime = this->_timeProcDataUpdate;
	_pLockProcData->EndRead();

	int nloop = 0;

	int lnNum = iStartIndex_;

	CProcChannParam *pProcChannParam = NULL;

	COleDateTimeSpan ltTimeSpan;

	//快变数据时间点与趋势数据更新时间差在10秒内，使用趋势数据
	if(ltUpdateTime >= dtTime_)
	{
		ltTimeSpan = ltUpdateTime - dtTime_;
	}
	else
	{
		ltTimeSpan = dtTime_ - ltUpdateTime;
	}

	if(ltTimeSpan.GetTotalSeconds() <= 5)
	{
		return -1;
	}

	//秒表中距离快变时间点最近的一条记录与快变时间差超过快变时间与趋势更新时间差，使用趋势数据保存
	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s < '%s' ORDER BY %s DESC",
		gc_cTime,_sProcTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTime = pDB_->GetTime(gc_cTime);
	}
	else
	{
		return -1;
	}

	if(abs(dtTime_ - ltUpdateTime) < abs(dtTime_-ltTrendDataTime))
	{
		return -1;
	}

	pDB_->Cmd("UPDATE a SET");

	for(nloop = lnNum; nloop < lnProcChannNum; ++nloop)
	{
		_pLockProcData->BeginRead();
		BEGIN_ERROR_HANDLE
			pProcChannParam=(CProcChannParam*)(_vProcChann[nloop]);
		END_ERROR_HANDLE
			_pLockProcData->EndRead();

		pDB_->Cmd(" a.[%s]=b.[%s], ",pProcChannParam->GetChannInfo()._cChannNo,pProcChannParam->GetChannInfo()._cChannNo);

	}
	pDB_->Cmd(" a.[%s] = %d",gc_cSynFlag,0);
	pDB_->Cmd(" FROM %s a, %s b where a.%s = '%s' and b.%s = '%s'",
		sTable_.c_str(),_sProcTrendSecTblName.c_str(),
		gc_cTime,dtTime_.Format(),gc_cTime,ltTrendDataTime.Format());
	if(pDB_->Exec())
	{
		return 1;
	}

	return -1;
}

int CRealPlantInfo::SetVibAxesLocationParam( int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_ )
{
	if (anHorChannIndex<0 || anVerChannIndex < 0 || anAxisChannIndex < 0)
	{
		return -1;
	}

	this->_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anHorChannIndex<this->_vVibChann.size())
		{
			CVibChannParam * lpVib = (CVibChannParam*)this->_vVibChann.at(anHorChannIndex);

			if(lpVib)
			{
				lpVib->_channInfo._fGapVoltage = aHorChannInfo._fGapVoltage;
				lpVib->_channInfo._fSensitivity = aHorChannInfo._fSensitivity;
			}

			lpVib = (CVibChannParam*)this->_vVibChann.at(anVerChannIndex);
			if(lpVib)
			{
				lpVib->_channInfo._fGapVoltage = aVerChannInfo._fGapVoltage;
				lpVib->_channInfo._fSensitivity = aVerChannInfo._fSensitivity;
			}

			if(anAxisChannIndex < this->_vAxisChann.size())
			{

				CAxisChannInfo lAxisChannInfo = this->_vAxisChann[anAxisChannIndex]->GetChannInfo();
				lAxisChannInfo._fAxisDia = aAxisChanInfo_._fAxisDia;
				lAxisChannInfo._iLocationType = aAxisChanInfo_._iLocationType;
				lAxisChannInfo._iRotateDirection = aAxisChanInfo_._iRotateDirection;

				this->_vAxisChann[anAxisChannIndex]->SetChannInfo(lAxisChannInfo);

			}
		}

		END_ERROR_HANDLE

			this->_pLockVibData->EndWrite();

		return TRUE;
}

int CRealPlantInfo::SetDynAxesLocationParam( int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_)
{
	if (anHorChannIndex<0 || anVerChannIndex < 0 || anAxisChannIndex < 0)
	{
		return -1;
	}

	this->_pLockDynData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anHorChannIndex<this->_vDynChann.size())
		{
			CDynChannParam * lpDyn = (CDynChannParam*)this->_vDynChann.at(anHorChannIndex);

			if(lpDyn)
			{
				lpDyn->_channInfo._fGapVoltage = aHorChannInfo._fGapVoltage;
				lpDyn->_channInfo._fSensitivity = aHorChannInfo._fSensitivity;
			}

			lpDyn = (CDynChannParam*)this->_vDynChann.at(anVerChannIndex);
			if(lpDyn)
			{
				lpDyn->_channInfo._fGapVoltage = aVerChannInfo._fGapVoltage;
				lpDyn->_channInfo._fSensitivity = aVerChannInfo._fSensitivity;
			}

			if(anAxisChannIndex < this->_vAxisChann.size())
			{
				CAxisChannInfo lAxisChannInfo = this->_vAxisChann[anAxisChannIndex]->GetChannInfo();
				lAxisChannInfo._fAxisDia = aAxisChanInfo_._fAxisDia;
				lAxisChannInfo._iLocationType = aAxisChanInfo_._iLocationType;
				lAxisChannInfo._iRotateDirection = aAxisChanInfo_._iRotateDirection;

				this->_vAxisChann[anAxisChannIndex]->SetChannInfo(lAxisChannInfo);
			}
		}

		END_ERROR_HANDLE

			this->_pLockDynData->EndWrite();

		return TRUE;
}