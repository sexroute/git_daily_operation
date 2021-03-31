#include "stdafx.h"
#include "transactdaqdata.h"
#include "SpeedUpDownDataSave.h"
#include "zipimpl.h"
#include "NetOffDataSave.h"
#include <MiddleWare.h>
#include <json/json.h>
#include "additions/ReaderWriterlock.h"
#include "additions/DebugHelper.h"

namespace FileTime
{
	inline FILETIME Now()
	{
		FILETIME loTime;

		::GetSystemTimeAsFileTime(&loTime);

		return loTime;
	}


	inline int Diff(FILETIME & aoFileTime1, FILETIME & aoFileTime2)
	{
		ULARGE_INTEGER ulTime1;
		ulTime1.LowPart  = aoFileTime1.dwLowDateTime;
		ulTime1.HighPart = aoFileTime1.dwHighDateTime;

		ULARGE_INTEGER ulTime2;
		ulTime2.LowPart  = aoFileTime2.dwLowDateTime;
		ulTime2.HighPart = aoFileTime2.dwHighDateTime;

		return (ulTime1.QuadPart - ulTime2.QuadPart) / 10000LL;	
	}
}

CTransactDaqData::CTransactDaqData(void)
{
	_dtUpdateChannInfo=CBHDateTime::GetCurrentTime();
	_bUpdateChannInfo=false;
}

CTransactDaqData::~CTransactDaqData(void)
{
}
/**发送指定机组的振动测点实时所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID   FIELD_SERVICE_PLANTID
*            振动测点通频特征值数据          FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*            采样频率                        FIELD_SERVICE_SAMPLE_FREQ
*            采样点数                        FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速                      FIELD_SERVICE_REV
*            机组的转速2                     FIELD_SERVICE_REV2
*            机组的转速3					 FIELD_SERVICE_REV3
*            机组的转速4					 FIELD_SERVICE_REV4
*            机组的转速5					 FIELD_SERVICE_REV5
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
 *            测点个数                 FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
#define     SERVICE_SERVICE_ON_RECEIVEALLVALUE        24000
*/
LONG g_StartFrom_Zero_Vib = 0;
LONG g_StartFrom_Zero_Dyn = 0;
LONG g_StartFrom_Zero_Proc = 0;

int CTransactDaqData::ReceiveVibValueForGap(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
    string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	

    //获取快变学习参数信息,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			
			if(pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_VIBCHANN)>0)
			{
				if(channInfo._chAlarmParam._bCharFast)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,
										(char *)(channInfo._chAlarmParam._pThLearnParamF),
										sizeof(CThresholdLearnParam)*D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SERVICE_LEN,D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,1);
				}
				else
				{
					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,-1);
				}
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
				{
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				}

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_LEN,-1);
			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}


int CTransactDaqData::ReceiveVibValue(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
    string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	int iJudgeAlarm = pFixComm->GetLong(FIELD_SERVICE_JUDGE_ALARM);

	int iEventID = 0, iAlarmStatus = 0;
	
	if (iStartIndex == 0)
	{
		InterlockedExchange(&g_StartFrom_Zero_Vib,1);
	}
	
	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24000接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);
		zdlTraceLine(lstrLogData);
		
		lstrLogData.Format(_T("24000接口，数采参数不正确，sCompany:%s,sFactory:%s,sPlant :%s, %s,%d"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(), __FILE__,__LINE__);
		zdlTraceLine(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);
	bool bEnd=false,bNewAssit=true;

	int lnUpdateFlag = 0;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
    {    
		
		if (g_StartFrom_Zero_Vib)
		{
			bEnd=true;
		}
		
	}
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2006,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		timeData.SetMillisecond(iMicroSecond);
	}

	int nChannNum = pFixComm->GetLong(FIELD_SERVICE_FC_CHANN_BUFFSIZE);
	vector<int> vChannNo;

	if(nChannNum > 0)
	{
		vChannNo.resize(nChannNum);
		pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),nChannNum*sizeof(int));
	}

    //判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;
	bool bFlag=false;//判断是否解压缩正确

	std::vector<S_OldVibCharValue> loOldVibCharValue;
	loOldVibCharValue.resize(iLen);
	
	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);

	S_OldVibCharValue *pOldVibCharValue = &loOldVibCharValue.front();
	S_VibCharValue    *pVibCharValue = &loVibCharValue.front();


	int iRealAllNum=0;
	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldVibCharValue,iLen*sizeof(S_OldVibCharValue));
		pPlantInfo->ChangeVibCharValueToNew(pVibCharValue,pOldVibCharValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
	}
	else
	{
		iRealAllNum=iRealNum;

		if (iRealAllNum<=G_SAMPLE_LIMITATION)
		{			
			CString lstrLogData;
			lstrLogData.Format(_T("	24000接口，数采参数不正确，\
									FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
									FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
									FIELD_SERVICE_SAMPLE_NUM :%d,，中间件调整为%d, %s,%d "),
									iLen,
									iRealAllNum,
									iSmpNum, 
									iRealNum*iLen,
									__FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			iRealAllNum = iRealNum*iLen;
		}

		if (iRealAllNum  <=G_SAMPLE_LIMITATION)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("	24000接口，数采参数不正确，中间件不处理，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								  iLen,
								  iRealAllNum,
								  iSmpNum, __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

		for (int i =0;i<iLen;i++)
		{
			S_VibCharValue  * lpVibCharValue = &pVibCharValue[i];


			if (!IsVibCharValid(lpVibCharValue))
			{
				CString lstrCharValue;
				lstrCharValue.Format("_fCharAll:%f _fCharHalf:%f _fCharOne:%f _fCharTwo:%f _fCharThree:%f _fCharRes:%f _fDC:%f _fGap:%f _fPhaseHalf:%f _fPhaseOne:%f _fPhaseTwo:%f _fPhaseThree:%f _iSmpFreq:%f _iSmpNum:%f _iRev:%f ",
									  lpVibCharValue->_fCharAll,lpVibCharValue->_fCharHalf,lpVibCharValue->_fCharOne,lpVibCharValue->_fCharTwo,
									  (float)lpVibCharValue->_fCharThree,(float)lpVibCharValue->_fCharRes,(float)lpVibCharValue->_fDC,(float)lpVibCharValue->_fGap,
									  (float)lpVibCharValue->_fPhaseHalf,(float)lpVibCharValue->_fPhaseOne,(float)lpVibCharValue->_fPhaseTwo,(float)lpVibCharValue->_fPhaseThree,
									  (float)lpVibCharValue->_iSmpFreq,(float)lpVibCharValue->_iSmpNum,(float)lpVibCharValue->_iRev);
				CString lstrLogData;
				lstrLogData.Format(_T("24000接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d 全部特征值:%s %s %d \r\n"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),lpVibCharValue->_fCharAll, lpVibCharValue->_iSmpNum,lpVibCharValue->_iRev,lstrCharValue, __FILE__,__LINE__);
				CHZLogManage::Error(lstrLogData);
				continue;
				zdlTraceLine(lstrLogData);

				//DATATYPE_WAVE *pByteBuf=new DATATYPE_WAVE[iRealAllNum];
				//pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

				//delete[] pByteBuf;
				//pFixComm->CreateAnswer(pFixComm->GetFunc());
				//pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
				//pFixComm->Write(pFixComm->GetAci());

				//return -1;
			}
		}


		#if 0
		{
			CString sLog;
			sLog.Format("VibCharValue(ac=%f,1c=%f,2c=%f,3c=%f,hc=%f,dc=%f,gap=%f,rc=%f,1p=%f,2p=%f,3p=%f,hp=%f,rev=%d,SmpFreq=%d,SmpNum=%d)",
			pVibCharValue->GetSpecTypeData(GE_ALLCHAR),
			pVibCharValue->GetSpecTypeData(GE_ONECHAR),
			pVibCharValue->GetSpecTypeData(GE_TWOCHAR),
			pVibCharValue->GetSpecTypeData(GE_THREECHAR),
			pVibCharValue->GetSpecTypeData(GE_HALFCHAR),
			pVibCharValue->GetSpecTypeData(GE_DCCHAR),
			pVibCharValue->GetSpecTypeData(GE_GAP),
			pVibCharValue->GetSpecTypeData(GE_RESCHAR),
			pVibCharValue->GetSpecTypeData(GE_PHASECHARONE),
			pVibCharValue->GetSpecTypeData(GE_PHASECHARTWO),
			pVibCharValue->GetSpecTypeData(GE_PHASECHARTHREE),
			pVibCharValue->GetSpecTypeData(GE_PHASECHARHALF),
			pVibCharValue->_iRev,
			pVibCharValue->_iSmpFreq,
			pVibCharValue->_iSmpNum
			);
			
			BHTRACE_DEBUG(sLog);
		}
		#endif

	}

	ASSERT(pVibCharValue->_iSmpNum!=0);

	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;

	CBHDateTime loTimeUpdate = CBHDateTime::GetCurrentTimeEx();

	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
	if (iAllBytes>0)
	{
		CZipImpl zipImpl;
		
		std::vector<BYTE> loZipByte(iAllBytes);
		pZipByte = &loZipByte.front();

		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
		if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
		{
			ASSERT(FALSE);
			bFlag=true;
		}else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

		//patch for precision problem
		lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

		if(lnIsPatchedForPrecision>0)
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
			pByteBuf = pByteBuf_precision;

		}

		
		bFlag=true;
	}
	if (bFlag)
	{

		//1.得到自定参数的长度
		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24000接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
			}
		}

		_DECLARE_PERF_MEASURE_TIME();

		pPlantInfo->CalcVibDataGrade(pVibCharValue,iLen,iStartIndex);

		//更新实时数据的缓存
		lnUpdateFlag = pPlantInfo->UpdateVibData(pByteBuf,pVibCharValue,iRealAllNum,iLen,timeData,iStartIndex,lfScaleCoef,
			lnIsPatchedForPrecision,
			loPMin,
			loPMax,bEnd);
		

		if (lbShouldComputCustomParam)
		{
			_BEGIN_PERF_MEASURE_TIME();
			//计算自定义参数信息
			pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_VIBCHANN,FALSE);

			_END_PERF_MEASURE_TIME("pPlantInfo->CalcCustomParamData_New");

		}else
		{
			_BEGIN_PERF_MEASURE_TIME();

			pPlantInfo->TransferCustomParamData(iStartIndex, 
												iLen,
												loCustomBuffer, 
												GE_VIBCHANN,FALSE);

			_END_PERF_MEASURE_TIME("pAlarmPlantInfo->TransferCustomParamData");

		}

		if (bEnd)
		{
			pPlantInfo->SyncVibData();
		}

		////更新报警数据保存处理中的数据
		if (pAlarmPlantInfo!=NULL)
		{
			if(!pAlarmPlantInfo->NewAlarmAddLog())
			{
				pAlarmPlantInfo->UpdateVibData( pByteBuf,
					pVibCharValue,
					iRealAllNum,
					iLen,
					timeData,
					iStartIndex,
					lfScaleCoef,
					lnIsPatchedForPrecision,
					loPMin,
					loPMax,
					bEnd);


				if (lbShouldComputCustomParam)
				{
					_BEGIN_PERF_MEASURE_TIME();

					//计算自定义参数信息
					pAlarmPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_VIBCHANN,TRUE);

					_END_PERF_MEASURE_TIME("pAlarmPlantInfo->CalcCustomParamData_New");
				}else
				{
					_BEGIN_PERF_MEASURE_TIME();

					//计算自定义参数信息
					pAlarmPlantInfo->TransferCustomParamData(iStartIndex, 
						iLen,
						loCustomBuffer,
						GE_VIBCHANN,
						TRUE);

					_END_PERF_MEASURE_TIME("pAlarmPlantInfo->TransferCustomParamData");
				}

				if(iJudgeAlarm == 1 && bEnd)
				{
					try
					{
						pAlarmPlantInfo->SetVibFastChangeChann(vChannNo);
						pAlarmPlantInfo->JudgeVibFastAlarm(pDB,iAlarmStatus,iEventID,vChannNo);

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
			else
			{
				std::vector<S_CustomParamData> loCustomParamBuffer(iLen);		
				S_CustomParamData *pfCustomParom= &loCustomParamBuffer.front();

				if (lbShouldComputCustomParam)
				{
					//计算自定义参数信息
					pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pVibCharValue,pByteBuf,pfCustomParom,GE_VIBCHANN,FALSE);

				}else
				{

					pPlantInfo->TransferCustomParamData(iStartIndex, 
						iLen,
						loCustomBuffer,
						pfCustomParom,
						GE_VIBCHANN,FALSE);

				}


				pPlantInfo->SaveVibFastTrend(pDB,pByteBuf,pVibCharValue,pfCustomParom,iRealAllNum,iLen,timeData,iStartIndex,0,1,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax,bEnd);

			}
			


		}
	}

	updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_VIB_TIME,
		CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));

    //获取快变学习参数信息,并返回到发送数据的数据采集器
	if(lnUpdateFlag < 0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
	}
	else
	{
		S_ChannInfo channInfo;
		pFixComm->CreateAnswerMul(pFixComm->GetFunc());
		if (iLen>0)
		{
			for (int index=0;index<iLen;++index)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);

				if(pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_VIBCHANN)>0)
				{
					if(channInfo._chAlarmParam._bCharFast)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,
							(char *)(channInfo._chAlarmParam._pThLearnParamF),
							sizeof(CThresholdLearnParam)*D_BHKD_ALARM_CHARTYPE_NUM);

						pFixComm->SetItem(FIELD_SERVICE_LEN,D_BHKD_ALARM_CHARTYPE_NUM);

						pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,1);
					}
					else
					{
						pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,-1);
					}
					if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
					{
						pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
					}

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_LEN,-1);
				}
				pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
				pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,iAlarmStatus);
				if(iJudgeAlarm == 1 && bEnd)
				{
					pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,(int)vChannNo.size());
					pFixComm->SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),vChannNo.size()*sizeof(int));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,0);
				}

				pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
		else
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
	}
	return iLen;
}

/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			 分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            是否这次更新数据完成    FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001
*/
int CTransactDaqData::ReceiveDynValueForGap(ICommInterFace *pFixComm,IDBInterFace  *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	//获取动态测点的间隙电压数组,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			int lnRet = 0;
			
			try
			{
				lnRet = pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_RODSINKCHANN);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			if(lnRet > 0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
				{
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				}

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}

		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,pAlarmPlantInfo->_iEventID);
		pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,pAlarmPlantInfo->GetPlantAlarmState());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}


int CTransactDaqData::ReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);


	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	int iJudgeAlarm=pFixComm->GetLong(FIELD_SERVICE_JUDGE_ALARM);

	int iEventID = 0,iAlarmStatus = 0;

	if (0 == iStartIndex)
	{
		InterlockedExchange(&g_StartFrom_Zero_Dyn,1);
	}

	if (iRealNum<=128)
	{
	//	iRealNum = iRealNum*iLen;
	}

	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24001接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);
	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END) == 1)
	{	
		if (g_StartFrom_Zero_Dyn)
		{
			bEnd=true;
		}		
	}
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2006,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		timeData.SetMillisecond(iMicroSecond);
	}

	int nChannNum = pFixComm->GetLong(FIELD_SERVICE_FC_CHANN_BUFFSIZE);
	vector<int> vChannNo;

	if(nChannNum > 0)
	{
		vChannNo.resize(nChannNum);
		pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),nChannNum*sizeof(int));
	}
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;
	bool bFlag=false;//判断是否解压缩正确

	std::vector<S_OldDynCharValue> loOldDynCharValue;
	loOldDynCharValue.resize(iLen);
	
	std::vector<S_DynCharValue> loDynCharValue;
	loDynCharValue.resize(iLen);

	S_OldDynCharValue *pOldDynCharValue = &loOldDynCharValue.front();
	S_DynCharValue    *pDynCharValue = &loDynCharValue.front();
	int iRealAllNum=0;
	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;

		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldDynCharValue,iLen*sizeof(S_OldDynCharValue));
			pPlantInfo->ChangeDynCharValueToNew(pDynCharValue,pOldDynCharValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
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
	else
	{
		iRealAllNum=iRealNum;

		if (iRealAllNum<=G_SAMPLE_LIMITATION)
		{			
			CString lstrLogData;
			lstrLogData.Format(_T("	24001接口，数采参数不正确，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d,，中间件调整为%d, %s,%d "),
								  iLen,
								  iRealAllNum,
								  iSmpNum, 
								  iRealNum*iLen,
								  __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			iRealAllNum = iRealNum*iLen;
		}

		if (iRealAllNum  <=G_SAMPLE_LIMITATION)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("	24001接口，数采参数不正确，中间件不处理，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								  iLen,
								  iRealAllNum,
								  iSmpNum, __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		for (int i =0;i<iLen;i++)
		{
			S_DynCharValue   * lpVibCharValue = &pDynCharValue[i];
			if (!IsDynCharValid(lpVibCharValue))
			{
				CString lstrLogData;
				lstrLogData.Format(		_T("24001接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d  测点索引:%d %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lpVibCharValue->_fCharValue0,
					lpVibCharValue->_iSmpNum,
					lpVibCharValue->_iRev,
					iStartIndex+i,
					__FILE__,__LINE__);
				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
				return -1;
			}
		}

	}

	if (iRealAllNum  <=256)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24001接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;
	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);

	CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTimeEx();

	if (iAllBytes>0)
	{
		try
		{
			CZipImpl zipImpl;
			std::vector<BYTE> loZipByte(iAllBytes);
			pZipByte = &loZipByte.front();
			
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
			if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
				bFlag=true;
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
	else
	{
		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

			//patch for precision problem
			lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

			if(lnIsPatchedForPrecision>0)
			{
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
				pByteBuf = pByteBuf_precision;

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
		bFlag=true;
	}

	if (bFlag)
	{
		//1.得到自定参数的长度
		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24000接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
			}
		}

		_DECLARE_PERF_MEASURE_TIME();
		//更新实时数据的缓存
		try
		{
			pPlantInfo->UpdateDynData( pByteBuf,
				pDynCharValue,
				iRealAllNum,
				iLen,
				timeData,
				iStartIndex,
				lfScaleCoef,
				lnIsPatchedForPrecision,
				loPMin,
				loPMax,
				bEnd);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}
		//计算自定义参数信息
		try
		{
			if (lbShouldComputCustomParam)
			{
				pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_RODSINKCHANN);
			}else
			{
				pPlantInfo->TransferCustomParamData(iStartIndex, iLen,loCustomBuffer,GE_RODSINKCHANN);
			}
			

			if (bEnd)
			{
				pPlantInfo->SyncDynData();
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

		//更新报警数据保存处理中的数据
		if (pAlarmPlantInfo!=NULL)
		{
			if(!pAlarmPlantInfo->NewAlarmAddLog())
			{
				try
				{
					pAlarmPlantInfo->UpdateDynData( pByteBuf,
						pDynCharValue,
						iRealAllNum,
						iLen,
						timeData,
						iStartIndex,
						lfScaleCoef,
						lnIsPatchedForPrecision,
						loPMin,
						loPMax,
						bEnd);
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				if(iJudgeAlarm == 1 && bEnd)
				{
					try
					{
						pAlarmPlantInfo->SetDynFastChangeChann(vChannNo);
						pAlarmPlantInfo->JudgeDynFastAlarm(pDB,iAlarmStatus,iEventID,vChannNo);
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


				//计算自定义参数信息
				try
				{
					if (lbShouldComputCustomParam)
					{
						pAlarmPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_RODSINKCHANN,TRUE);
					}else
					{
						pAlarmPlantInfo->TransferCustomParamData(iStartIndex,iLen,loCustomBuffer,GE_RODSINKCHANN,TRUE);
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
			}
			else
			{
				vector<S_CustomParamData> loCustomParam;
				loCustomParam.resize(iLen);

				S_CustomParamData *pfCustomParam = &loCustomParam.front();

				if (lbShouldComputCustomParam)
				{
					//计算自定义参数信息

					pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pDynCharValue,pByteBuf,pfCustomParam,GE_RODSINKCHANN,FALSE);

				}else
				{

					pPlantInfo->TransferCustomParamData(iStartIndex, 
						iLen,
						loCustomBuffer,
						pfCustomParam,
						GE_RODSINKCHANN,FALSE);

				}

				pPlantInfo->SaveDynFastTrend(pDB,pByteBuf,pDynCharValue,pfCustomParam,iRealAllNum,iLen,timeData,iStartIndex,0,1,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax,bEnd);

			}
			

		}
	}

	try
	{
		updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_DYN_TIME,
			CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	//获取动态测点的间隙电压数组,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,iAlarmStatus);
			if(iJudgeAlarm == 1 && bEnd)
			{

				pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,(int)vChannNo.size());
				pFixComm->SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),vChannNo.size()*sizeof(int));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,0);
			}
			int lnRet = 0;

			try
			{
				lnRet = pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_RODSINKCHANN);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			if(lnRet > 0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			//CString strTmp;strTmp.Format("动态测点%s,间隙电压：%.2f",channInfo._cChannNo,channInfo._fGapVoltage);
			//CHZLogManage::Debug(strTmp,"CTransactDaqData","ReceiveDynValue");
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,pAlarmPlantInfo->_iEventID);
		pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,pAlarmPlantInfo->GetPlantAlarmState());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}


/**发送转速到中间件模块
*输入参数字段码：
*                公司ID      FIELD_SYSTEM_COMPANY
*	    		 分厂名      FIELD_SYSTEM_FACTORY
*                机组ID      FIELD_SERVICE_PLANTID
*                机组的转速  FIELD_SERVICE_REV
*                机组的转速2 FIELD_SERVICE_REV2
*                机组的转速3     FIELD_SERVICE_REV3
*                机组的转速4     FIELD_SERVICE_REV4
*                机组的转速5     FIELD_SERVICE_REV5
*返回值的字段码：无
#define     SERVICE_SERVICE_ON_RECEIVEREV             24002
*/
int CTransactDaqData::ReceiveRev(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	pPlantInfo->UpdateRealRev(pfRev);
	//更新报警数据保存处理中的数据
	if (pAlarmPlantInfo!=NULL)
		pAlarmPlantInfo->UpdateRealRev(pfRev);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->Write(pFixComm->GetAci());
	return (int)pfRev[0];
}
/**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*                分厂名   FIELD_SYSTEM_FACTORY
*                机组ID      FIELD_SERVICE_PLANTID
*                起始索引          FIELD_SERVICE_ON_SEND_STARTID
*                测点类型          FIELD_SERVICE_CHANNTYPE
*                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
*                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
#define     SERVICE_SERVICE_ON_RECEIVEPROC            24003
*/
int CTransactDaqData::ReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	int iType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iReturnType=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_CHANNTYPE);

	int lnProcJudgedPlantStatus = pFixComm->GetLong(FIELD_SYSTEM_STATUS);

	int iJudgeAlarm = pFixComm->GetLong(FIELD_SERVICE_JUDGE_ALARM);
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2006,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		timeData.SetMillisecond(iMicroSecond);
	}

	int nChannNum = pFixComm->GetLong(FIELD_SERVICE_FC_CHANN_BUFFSIZE);
	vector<int> vChannNo;

	if(nChannNum > 0)
	{
		vChannNo.resize(nChannNum);
		pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),nChannNum*sizeof(int));
	}

	int iAlarmStatus = 0, iEventID = 0;

	if (0>= iProcChannNum )
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24003 接口，数采参数不正确，FIELD_SERVICE_ON_SEND_PROCCHANNNUM:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	//float *pfData=new float[iProcChannNum];
	std::vector<float> loData;
	loData.resize(iProcChannNum);
	float *pfData = &loData.front();
	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)pfData,iProcChannNum*sizeof(float));

	#if 0
	{
		HZLOG_DEBUG(STR("StartIndex=%d, ProcChannNum", iStartIndex, iProcChannNum));
		HZLOG_ARRAY("ProcValue", pfData, iProcChannNum);
	}
	#endif

	switch (iType)
	{
	case GE_ALLPROC:
		pPlantInfo->UpdateProcData(pfData,iProcChannNum,timeData,iStartIndex);
		pAlarmPlantInfo->UpdateProcData(pfData,iProcChannNum,timeData,iStartIndex);
		pPlantInfo->SetProcJudgeRunStatus(lnProcJudgedPlantStatus,iProcChannNum,iStartIndex);
		pAlarmPlantInfo->SetProcJudgeRunStatus(lnProcJudgedPlantStatus,iProcChannNum,iStartIndex);
		if(iJudgeAlarm == 1)
		{
			pAlarmPlantInfo->SetProcFastChangeChann(vChannNo);
			pAlarmPlantInfo->JudgeProcFastAlarm(pDB,iAlarmStatus,iEventID,vChannNo);
		}
		break;
	}
	//KD_DeletePoint(pfData);
	loData.clear();

	updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_PROC_TIME,
	CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	switch(iReturnType)
	{
	case GE_ALLPROC:
		iProcChannNum=pPlantInfo->GetProcChannNum();
		loData.resize(iProcChannNum);
		pfData = &loData.front();
		//pfData=new float[iProcChannNum];
		pPlantInfo->GetProcData(pfData,iProcChannNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACE,(char *)pfData,iProcChannNum*GD_FLOAT_SIZE);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,iProcChannNum);
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
		pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,iAlarmStatus);
		if(iJudgeAlarm == 1)
		{
			pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,(int)vChannNo.size());
			pFixComm->SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),vChannNo.size()*sizeof(int));
		}
		else
		{
			pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,0);
		}
		//KD_DeletePoint(pfData);
		break;
	}
	pFixComm->Write(pFixComm->GetAci());
	return iProcChannNum;
}

/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID
*返回值的字段码：无
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24004
*/
int CTransactDaqData::ReceiveChannState(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pAlarmPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	bool lbSudStatus = pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS);
	int lnSudType    = pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);

	pAlarmPlantInfo->SetSudStatus(lbSudStatus, (E_SUD_TYPE)lnSudType);

	if (isLocalJudgeAlarm())
	{

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		if (pAlarmPlantInfo->_iSyncID >= 0)
		{
			pAlarmPlantInfo->_iSyncID = -2;
		}

		return -2;
	}

	CString sLog;

	int iVibNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iProcNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
    int iAxisPairNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_AXISCHANNNUM);
	int iDynNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_DYNCHANNNUM);
	S_AlarmState *pState=NULL;
	if(iVibNum>0)
	{
		try {
			pState=new S_AlarmState[iVibNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}

		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLVIBSTATE,(char *)pState,iVibNum*sizeof(S_AlarmState));
		}
		catch (...) {
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		}

		pAlarmPlantInfo->SetVibChannState(pState,iVibNum);
		KD_DeletePoint(pState);
	}
	if(iProcNum>0)
	{
		try {
			pState=new S_AlarmState[iProcNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCSTATE,(char *)pState,iProcNum*sizeof(S_AlarmState));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetProcChannState(pState,iProcNum);
	    KD_DeletePoint(pState);
	}

	if (iAxisPairNum>0)
	{
		try
		{
			pState=new S_AlarmState[iAxisPairNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}


		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE,(char *)pState,iAxisPairNum*sizeof(S_AlarmState));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetAxisLocationState(pState,iAxisPairNum);
		KD_DeletePoint(pState);
	}

	if (iDynNum>0)
	{
		try {
			pState=new S_AlarmState[iDynNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		try 
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLDYNSTATE,(char *)pState,iDynNum*sizeof(S_AlarmState));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetDynChannState(pState,iDynNum);
		KD_DeletePoint(pState);
	}

	pAlarmPlantInfo->_iSyncID = pFixComm->GetLong(FIELD_SERVICE_ALARMID);

	KD_DeletePoint(pState);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
	pFixComm->Write(pFixComm->GetAci());
	return iVibNum;
}

/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID
*返回值的字段码：无
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL      24027
*/
int CTransactDaqData::ReceiveChannStateWithAlarmLevel(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pAlarmPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	bool lbSudStatus = pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS);
	int lnSudType    = pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);

	pAlarmPlantInfo->SetSudStatus(lbSudStatus, (E_SUD_TYPE)lnSudType);

	if (isLocalJudgeAlarm())
	{

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		if (pAlarmPlantInfo->_iSyncID >= 0)
		{
			pAlarmPlantInfo->_iSyncID = -2;
		}

		return -2;
	}

	CString sLog;

	int iVibNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iProcNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	int iAxisPairNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_AXISCHANNNUM);
	int iDynNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_DYNCHANNNUM);
	S_AlarmStateWithLevel *pState=NULL;
	if(iVibNum>0)
	{
		try {
			pState=new S_AlarmStateWithLevel[iVibNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}

		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLVIBSTATE,(char *)pState,iVibNum*sizeof(S_AlarmStateWithLevel));
		}
		catch (...) {
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		}

		pAlarmPlantInfo->SetVibChannState(pState,iVibNum);
		KD_DeletePoint(pState);
	}
	if(iProcNum>0)
	{
		try {
			pState=new S_AlarmStateWithLevel[iProcNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCSTATE,(char *)pState,iProcNum*sizeof(S_AlarmStateWithLevel));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetProcChannState(pState,iProcNum);
		KD_DeletePoint(pState);
	}

	if (iAxisPairNum>0)
	{
		try
		{
			pState=new S_AlarmStateWithLevel[iAxisPairNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}


		try{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE,(char *)pState,iAxisPairNum*sizeof(S_AlarmStateWithLevel));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetAxisLocationState(pState,iAxisPairNum);
		KD_DeletePoint(pState);
	}

	if (iDynNum>0)
	{
		try {
			pState=new S_AlarmStateWithLevel[iDynNum];
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		try 
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLDYNSTATE,(char *)pState,iDynNum*sizeof(S_AlarmStateWithLevel));
		}
		catch (...) 
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		pAlarmPlantInfo->SetDynChannState(pState,iDynNum);
		KD_DeletePoint(pState);
	}

	pAlarmPlantInfo->_iSyncID = pFixComm->GetLong(FIELD_SERVICE_ALARMID);

	KD_DeletePoint(pState);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
	pFixComm->Write(pFixComm->GetAci());
	return iVibNum;
}
/**发送指定指定设备的启停车数据到中间件
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			  分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             起始索引        FIELD_SERVICE_ON_SEND_STARTID
*             启停车标志位    FIELD_SERVICE_SUD_STATUS
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
*             指定机组的转速  FIELD_SERVICE_REV
*             指定机组的转速2 FIELD_SERVICE_REV2
*             特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据        FIELD_SERVICE_ON_SEND_VIBWAVE
*             一组波形点数     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*             采样频率        FIELD_SERVICE_SAMPLE_FREQ
*             采样点数        FIELD_SERVICE_SAMPLE_NUM
*            对应的时间       FIELD_SERVICE_TIME
*            对应时间的微秒   FIELD_SERVICE_MICROSECOND
*             启停车类型     FIELD_SERVICE_SUD_TYPE
*             启停车标志位    FIELD_SERVICE_SUD_STATUS
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_REVSUDDATA            24005
*/
int CTransactDaqData::ReciveSudData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	int iLength=-1;
	bool bSudStatus=(pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS))==1?true:false;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	CAlarmPlantInfo *pAlarmPlantInfo = (CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	CRealPlantInfo *pRealPlantInfo   = (CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pSudPlantInfo==NULL|| pAlarmPlantInfo == NULL || pRealPlantInfo == NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
   pSudPlantInfo->SetSudFinishStatus(!bSudStatus); 
	if (bSudStatus)
	{
        iLength=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
		int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
		int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
		int iRealNums=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
		int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
		float pfRev[5];
		pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
		pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
		pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
		pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
		pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
		bool bEnd=(pFixComm->GetLong(FIELD_SERVICE_SEND_END))==1?true:false;

		//得到启停车相关参数
		CBHDateTime timeData=CBHDateTime::GetCurrentTime();
		timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
		int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
		int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
		long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
		int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
		float lfScaleCoef = pFixComm->GetLong(FIELD_SERVICE_WAVE_COEF);

        bool bNewAssit=true;
		int iRealAllNum=0;
		//判断是否为新版本的发送助手
		if (iSmpFreq>0 && iSmpNum>0)
			bNewAssit=false;
		else
			bNewAssit=true;
		
		std::vector<S_OldVibCharValue> loOldVibCharValue;
		loOldVibCharValue.resize(iLength);
		
		std::vector<S_VibCharValue> loVibCharValue;
		loVibCharValue.resize(iLength);

		S_OldVibCharValue *pOldVibValue = &loOldVibCharValue.front();
		S_VibCharValue    *pVibValue  = &loVibCharValue.front();

		CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTime();
		if (bNewAssit==false)
		{
			iRealAllNum=iSmpNum*iLength;
		    pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pOldVibValue,iLength*sizeof(S_OldVibCharValue));
		    pSudPlantInfo->ChangeVibCharValueToNew(pVibValue,pOldVibValue,pfRev,iSmpNum,iSmpFreq,iRealNums,iLength,iStartIndex);
		}
		else
		{
			iRealAllNum=iRealNums;
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pVibValue,iLength*sizeof(S_VibCharValue));
		}
		//DATATYPE_WAVE *pBuf=new DATATYPE_WAVE[iRealAllNum];
		std::vector<DATATYPE_WAVE> loBuf;
		loBuf.resize(iRealAllNum);
		DATATYPE_WAVE *pBuf = &loBuf.front();

		std::vector<DATATYPE_WAVE> loByteBuf_precision;
		loByteBuf_precision.resize(iRealAllNum);
		DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
		memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

		std::vector<float> loPMin;
		std::vector<float> loPMax;

		loPMin.resize(iLength);
		loPMax.resize(iLength);


		float * lfPrecisionMin = &loPMin.front();
		float * lfPrecisonMax = &loPMax.front();

		int lnIsPatchedForPrecision = 0;

		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

		//patch for precision problem
		lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

		if(lnIsPatchedForPrecision>0)
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLength*sizeof(float));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLength*sizeof(float));	
			pBuf = pByteBuf_precision;

		}

		
	//	writefile(pBuf,iSmpNum*iLength,iRealNums*iLength);
		if (timeData<CBHDateTime(2000,1,1,1,1,1))
			timeData=CBHDateTime::GetCurrentTime();
		if (iRealAllNum>1)
		{          
			pSudPlantInfo->UpdateVibData(pBuf,
										 pVibValue,
										 iRealAllNum,
										 iLength,
										 loUpdateTime,
										 iStartIndex,
										 lfScaleCoef,
										 lnIsPatchedForPrecision,
										 loPMin,
										 loPMax);

			

			if (bEnd)
			{
				pSudPlantInfo->SyncVibData();

			}
			pSudPlantInfo->SetDataTime(timeData,iMicroSecond);
			pSudPlantInfo->SetDataStatu(pDB,true,bEnd,iEventID,(E_SUD_TYPE)iType);
			pAlarmPlantInfo ->SetSudStatus(true, (E_SUD_TYPE)iType);
			pRealPlantInfo  ->SetSudStatus(true, (E_SUD_TYPE)iType);
		}
		else
			CHZLogManage::Info("Online:采样点数为0，不更新和保存启停车数据","CTransactDaqData","RevSudData");
	}
	else
	{
		pSudPlantInfo->SetDataStatu(pDB,false,false,-1,GE_SUD_UNKNOWN);
		pAlarmPlantInfo ->SetSudStatus(false, (E_SUD_TYPE)GE_SUD_UNKNOWN);
		pRealPlantInfo  ->SetSudStatus(false, (E_SUD_TYPE)GE_SUD_UNKNOWN);
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	pFixComm->Write(pFixComm->GetAci());
	return iLength;
}
/**通知中间件服务器，需要保存数据
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*返回值字段码：无
#define     SERVICE_SERVICE_ON_SAVEDATA              24006
*/
int CTransactDaqData::SrvSaveData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	
	static FILETIME g_loBegin;

    if(CheckDBConnEx(pDB)<0)
	{	
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-5);
		pFixComm->Write(pFixComm->GetAci());

		return -5;
	}

	

	if (!TryEnterCriticalSection(g_pSavingCS))
	{
		FILETIME loEnd = FileTime::Now();

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pFixComm->SetItem(FIELD_SERVICE_TIME, FileTime::Diff(loEnd, g_loBegin));
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	BEGIN_ERROR_HANDLE;

	g_loBegin = FileTime::Now();
	
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);

	g_bIsDiskFull = (bool)pFixComm->GetLong(FIELD_SERVICE_DISK_FULL);

	UpdateTrendDataSaveParam();

	int nCount=-1;
    bool bNoFactory=sFactory.empty();
    int iCompanyNum=0;
	if (sCompany.empty())
	{
		iCompanyNum=g_pCompanyPoolReal->GetInfoNum();
		for (int loop=0;loop<iCompanyNum;++loop)
		{
			g_pCompanyPoolReal->GetSpecInfoPool(loop,sCompany);
			nCount+=SaveSpecCompanyData(sCompany,pDB);
		}
	}
    else
	{
		if (sFactory.empty())
          nCount=SaveSpecCompanyData(sCompany,pDB);
		else
          nCount=SaveSpecFactoryData(sCompany,sFactory,pDB);
	}

	FILETIME loEnd = FileTime::Now();

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
	pFixComm->SetItem(FIELD_SERVICE_TIME, FileTime::Diff(loEnd, g_loBegin));
	pFixComm->Write(pFixComm->GetAci());
	
	END_ERROR_HANDLE;

	LeaveCriticalSection(g_pSavingCS);

	return 1;
}

int CTransactDaqData::SaveSpecCompanyData(string sCompany_,IDBInterFace *pDB_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolReal->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL)
		return 0;
	int iFactory=pFactoryInfoPool->GetInfoNum();
	string sFactory="";
	for (int loop=0;loop<iFactory;++loop)
	{
		if(pFactoryInfoPool->GetSpecInfoPool(loop,sFactory)!=NULL)
			nCount+=SaveSpecFactoryData(sCompany_,sFactory,pDB_);
	}
	return nCount;
}
int CTransactDaqData::SaveSpecFactoryData(string sCompany_,string sFactory_,IDBInterFace *pDB_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolReal->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
	CPlantInfoPool *pPlantInfoPool=pFactoryInfoPool->GetSpecInfoPool(sFactory_);
	if (pPlantInfoPool==NULL) return 0;
	CRealPlantInfo *pPlantInfo=NULL;
	int iPlantNum=pPlantInfoPool->GetInfoNum();
	string sPlant="";
	for (int loop=0;loop<iPlantNum;++loop)
	{
	//	CString strTmp;//strTmp.Format("%s,%s",sTmp1.c_str(),sTmp2.c_str());AfxMessageBox(strTmp);
		pPlantInfo=(CRealPlantInfo *)pPlantInfoPool->GetSpecInfoPool(loop,sPlant);
	//	strTmp.Format("%s,%d,%d,%s",pPlantInfo->GetPlantID().c_str(),loop,iPlantNum,pPlantInfo->GetTableName().c_str());AfxMessageBox(strTmp);
		if (pPlantInfo!=NULL)
		{
			pPlantInfo->SaveTrendData(pDB_);
			Sleep(5);
			++nCount;
		}
	}
	return nCount;
}
/**通知中间件服务器，需要判断报警
  *输入参数字段码：
  *              公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
  *返回值字段码：无
  #define     SERVICE_SERVICE_ON_JUDGEALARM            24007
  */
int CTransactDaqData::SrvJudgeAlarm(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	if(CheckDBConnEx(pDB)<0)
	{
		return -5;
	}
	UpdateJudgeAlarmTime();

	CBHDateTime dtCurr=CBHDateTime::GetCurrentTime();
	COleDateTimeSpan dtSpan;
	_lockUpdateChannTime.BeginRead();
	dtSpan=dtCurr-_dtUpdateChannInfo;
	_lockUpdateChannTime.EndRead();

	UpdateAlarmParam();

	if (dtSpan.GetTotalMinutes()>4)
	{
		_lockUpdateChannTime.BeginWrite();
		_bUpdateChannInfo=true;
		_dtUpdateChannInfo=dtCurr;
		_lockUpdateChannTime.EndWrite();
	}
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int nCount=-1;
	bool bNoFactory=sFactory.empty();
	int iCompanyNum=0;
	if (sCompany.empty())
	{
		iCompanyNum=g_pCompanyPoolAlarm->GetInfoNum();
		for (int loop=0;loop<iCompanyNum;++loop)
		{
			g_pCompanyPoolAlarm->GetSpecInfoPool(loop,sCompany);
	//		CString strTmp;strTmp.Format("%s",sCompany.c_str());	AfxMessageBox(strTmp);
			nCount+=JudgeSpecCompanyAlarm(sCompany,pDB);
		}
	}
	else
	{
		if (sFactory.empty())
			nCount=JudgeSpecCompanyAlarm(sCompany,pDB);
		else
			nCount=JudgeSpecFactoryAlarm(sCompany,sFactory,pDB);
	}
	_lockUpdateChannTime.BeginWrite();
	_bUpdateChannInfo=false;
	_lockUpdateChannTime.EndWrite();
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**申请阈值ID，如果数组长度小于1表示失败
*输入参数字段码
*           公司ID          FIELD_SYSTEM_COMPANY
*			分厂名          FIELD_SYSTEM_FACTORY
*           机组ID：        FIELD_SERVICE_PLANTID
*           通道个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*           阈值ID数组           FIELD_SERVICE_THRESHOLD_ID
*           数组长度（通道个数） FIELD_SERVICE_CHANN_NUM
#define     SERVICE_SERVICE_ON_REQTHESHOLDID       24011
*/
int CTransactDaqData::ReqThresholdID(ICommInterFace *pFixComm,CCustomDB *pDB)
{
    string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	int iFirstID=pPlantInfo->ReqThID(iChannNum);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (iFirstID>0)
	{
       int *pID=new int[iChannNum];
	   for (int loop=0;loop<iChannNum;++loop)
	     pID[loop]=iFirstID+loop;
	   pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_ID,(char*)pID,sizeof(int)*iChannNum);
	   KD_DeletePoint(pID);
	}
	else
		iChannNum=0;
	pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum);
	pFixComm->Write(pFixComm->GetAci());
	return iChannNum;
}

/**申请指定公司、分厂、设备的快变报警事件ID
  *输入参数字段码：
  *             公司ID         FIELD_SYSTEM_COMPANY
  *			    分厂名         FIELD_SYSTEM_FACTORY
  *             机组ID：       FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *             快变报警事件ID:   FIELD_SYSTEM_EVENT_ID
  #define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012
  */
int CTransactDaqData::ReqFastAlarmEventID(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	int iEventID=-2;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo!=NULL)
	{
	  iEventID=pPlantInfo->ReqAlarmEventID();
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
	pFixComm->Write(pFixComm->GetAci());
	return iEventID;
}

/**更新指定公司、分厂、设备的振动测点快变报警状态
*输入参数字段码：
*             公司ID            FIELD_SYSTEM_COMPANY
*			  分厂名            FIELD_SYSTEM_FACTORY
*             机组ID：          FIELD_SERVICE_PLANTID
*             通道索引          FIELD_SERVICE_ON_SEND_STARTID
*             振动的报警状态    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*             振动通道个数      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*返回值的字段码：
*             成功标识            FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS     24013
*/
int CTransactDaqData::UpdateFastAlarmStatus(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	int nStatus=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo!=NULL)
	{

	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,nStatus);
	pFixComm->Write(pFixComm->GetAci());
	return nStatus;
}

/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE        24021
*/
LONG g_netoff_StartFrom_Zero_Vib = 0;
LONG g_netoff_StartFrom_Zero_Dyn = 0;
LONG g_netoff_StartFrom_Zero_Proc = 0;
int CTransactDaqData::NetOffReceiveAllValue(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	int iLen=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	
	CNetOffPlantInfo *lpNetOffPlantInfo=(CNetOffPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_NETOFF);
	CRealPlantInfo *lpPlantRealInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	
	if (lpNetOffPlantInfo==NULL || lpPlantRealInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
/*		ASSERT(FALSE);*/
		return -1;
	}
	
	//设置指向实时类的设备指针

	if (NULL == lpNetOffPlantInfo->GetRealPlantInfo())
	{
		lpPlantRealInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

		if (lpNetOffPlantInfo==NULL)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lpNetOffPlantInfo->SetIDPointer(lpPlantRealInfo);
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);

	if (iStartIndex == 0)
	{
		InterlockedExchange(&g_netoff_StartFrom_Zero_Vib,1);
	}

	iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	if (iLen <=0)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24021接口，接收断网数据，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,%s,%d"),iLen,__FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);
	S_VibCharValue *pVibCharValue = &loVibCharValue.front();
	ZeroMemory(pVibCharValue,sizeof(S_VibCharValue)*iLen);

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

	bool bExistOnlineChann = false;

	for (int i =0;i<iLen;i++)
	{
		S_VibCharValue * lpCharVal = pVibCharValue+i;

		int iChannIndex = iStartIndex + i;

		S_ChannInfo& loChannInfo = lpPlantRealInfo->GetSpecVibChannInfo(iChannIndex);

		if (loChannInfo._iDataSource == GE_DATA_SOURCE_ONLINE)
		{
			if (lpCharVal->_iSmpNum < 1)
			{
				CString lsErrLog;

				lsErrLog.Format("SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE(%d)测点%s采样点数不正确(%d)", 
					SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE,
					loChannInfo._cChannID, lpCharVal->_iSmpNum);

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, lsErrLog);
				pFixComm->Write(pFixComm->GetAci());

				BHLOG_ERROR(lsErrLog);

				return -1;
			}

			bExistOnlineChann = true;
		}
	}

	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	if (bExistOnlineChann)
	{
		if (iRealNum <=0)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("24021接口，接收断网数据,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,%s,%d"),iRealNum,__FILE__,__LINE__);
			CHZLogManage::Error(lstrLogData);

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-2);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}

	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	CBHDateTime loUpdateTime = timeData;
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMilliSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMilliSecond > 0)
	{
		timeData.SetMillisecond(iMilliSecond);
	}
	
	bool bEnd=false;
	
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
	{
		if (g_netoff_StartFrom_Zero_Vib)
		{
			bEnd=true;
		}		
	}
	
	bool bFlag=false;//判断是否解压缩正确
	

	int iRealAllNum=iRealNum;

	if (bExistOnlineChann)
	{
		if (iRealAllNum<=G_SAMPLE_LIMITATION)
		{			
			CString lstrLogData;
			lstrLogData.Format(_T("	24021接口，数采参数不正确，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  中间件调整为%d, %s,%d "),
								  iLen,
								  iRealAllNum,									
								  iRealNum*iLen,
								  __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			iRealAllNum = iRealNum*iLen;
		}

		if (iRealAllNum  <=G_SAMPLE_LIMITATION)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("	24021接口，数采参数不正确，中间件不处理，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  %s,%d"),
								  iLen,
								  iRealAllNum,
								  __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	//DATATYPE_WAVE *pByteBuf=new DATATYPE_WAVE[iRealAllNum];
	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;

	if (bExistOnlineChann)
	{
		iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);

		if (iAllBytes>0)
		{
			CZipImpl zipImpl;
			vector<BYTE> loZipByte;
			loZipByte.resize(iAllBytes);
			pZipByte = &loZipByte.front();
			memset(pZipByte,0,iAllBytes);
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
			if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
				bFlag=true;
		}
		else
		{
			
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
			//patch for precision problem
			lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

			if(lnIsPatchedForPrecision>0)
			{
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
				pByteBuf = pByteBuf_precision;

			}

		
			bFlag=true;
		}
	}
	else
	{
		bFlag = true;
	}

	lpPlantRealInfo->CalcVibDataGrade(pVibCharValue,iLen,iStartIndex);

	if (bFlag)
	{
		BOOL lbLockGot = FALSE;

		try
		{
			lbLockGot = lpNetOffPlantInfo->m_oWriteToDbLock.AcquireWriterLock();

			//更新断网数据的缓存
			int lnUpdatedCount = lpNetOffPlantInfo->UpdateVibData(  pByteBuf,
																	pVibCharValue,
																	iRealAllNum,
																	iLen,
																	loUpdateTime,
																	iStartIndex,
																	lfScaleCoef,
																	lnIsPatchedForPrecision,
																	loPMin,
																	loPMax,
																	bEnd);
			ASSERT(lnUpdatedCount == iLen);

	
			if (bExistOnlineChann)
			{
				//计算自定义参数信息
				lpNetOffPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_VIBCHANN,FALSE);
			}


			if (bEnd)
			{
				lpNetOffPlantInfo->SyncVibData();
			}
			
			//设置断网保存数据的时间
			lpNetOffPlantInfo->SetVibDataTime(timeData);
			
			//设置保存数据状态
			lpNetOffPlantInfo->SetVibDataStatu(pDB,bEnd);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			ASSERT(FALSE);
		}

		if (lbLockGot)
		{
			lpNetOffPlantInfo->m_oWriteToDbLock.ReleaseWriterLock();
		}
		

	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

/**发送指定机组的动态测点断网后保存的所有特征值波形数据到中间件模块
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			 分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                 FIELD_SERVICE_TIME
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE        24022
*/
int CTransactDaqData::NetOffReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iLen=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	
	CNetOffPlantInfo *lpNetOffPlantInfo=(CNetOffPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_NETOFF);

	if (lpNetOffPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CRealPlantInfo * lpRealPlantInfo = lpNetOffPlantInfo->GetRealPlantInfo();

	if (NULL == lpRealPlantInfo)
	{
		lpRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

		if (lpNetOffPlantInfo==NULL)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lpNetOffPlantInfo->SetIDPointer(lpRealPlantInfo);
	}


	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);
	
	if (0 == iStartIndex)
	{
		InterlockedExchange(&g_netoff_StartFrom_Zero_Dyn,1);
	}

	iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	if (iRealNum <=0 || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24022接口，接收断网数据，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,%s,%d"),iLen,iRealNum,__FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMilliSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMilliSecond > 0)
	{
		timeData.SetMillisecond(iMilliSecond);
	}

	bool bEnd=false;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END) == 1)
	{
		if (g_netoff_StartFrom_Zero_Dyn)
		{
			bEnd=true;
		}		
	}
	bool bFlag=false;//判断是否解压缩正确
	
	//S_DynCharValue *pDynCharValue=new S_DynCharValue[iLen];
	std::vector<S_DynCharValue> loDynCharValue;
	loDynCharValue.resize(iLen);
	S_DynCharValue *pDynCharValue = &loDynCharValue.front();
	ZeroMemory(pDynCharValue,sizeof(S_DynCharValue)*iLen);

	int iRealAllNum=0;
	iRealAllNum=iRealNum;

	if (iRealAllNum<=G_SAMPLE_LIMITATION)
	{			
		CString lstrLogData;
		lstrLogData.Format(_T("	24022接口，数采参数不正确，\
							  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
							  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
							  中间件调整为%d, %s,%d "),
									iLen,
									iRealAllNum,									
									iRealNum*iLen,
									__FILE__,__LINE__);
		zdlTraceLine(lstrLogData);
		CHZLogManage::Error(lstrLogData);
		iRealAllNum = iRealNum*iLen;
	}

	if (iRealAllNum  <=G_SAMPLE_LIMITATION)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("	24022接口，数采参数不正确，中间件不处理，\
							  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
							  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
							  %s,%d"),
							  iLen,
							  iRealAllNum,
							  __FILE__,__LINE__);
		zdlTraceLine(lstrLogData);
		CHZLogManage::Error(lstrLogData);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));

	for (int i =0;i<iLen;i++)
	{
		S_DynCharValue  * lpVibCharValue = &pDynCharValue[i];
		if (!IsDynCharValid(lpVibCharValue))
		{
			CString lstrLogData;
			lstrLogData.Format(_T("24022接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d %s %d\r\n"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),lpVibCharValue->_fCharValue0, lpVibCharValue->_iSmpNum,lpVibCharValue->_iRev, __FILE__,__LINE__);
			CHZLogManage::Error(lstrLogData);
			zdlTraceLine(lstrLogData);
			return -1;
		}
	}

	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;
	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
	CBHDateTime loUpDateTime = CBHDateTime::GetCurrentTime();
	if (iAllBytes>0)
		{
			CZipImpl zipImpl;
			std::vector<BYTE> loZipByte(iAllBytes);
			pZipByte = &loZipByte.front();
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
			if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
				bFlag=true;
		}
		else
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
			//patch for precision problem
			lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

			if(lnIsPatchedForPrecision>0)
			{
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
				pByteBuf = pByteBuf_precision;

			}
			
			bFlag=true;
		}
	
		if (bFlag)
		{
			//更新断网数据的缓存

			lpNetOffPlantInfo->UpdateDynData(	pByteBuf,
												pDynCharValue,
												iRealAllNum,
												iLen,
												loUpDateTime,
												iStartIndex,
												lfScaleCoef,
												lnIsPatchedForPrecision,
												loPMin,
												loPMax,
												bEnd);
			//计算自定义参数信息
			lpNetOffPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_RODSINKCHANN,FALSE);

			if (bEnd)
			{
				lpNetOffPlantInfo->SyncDynData();
			}

			//设置断网保存数据的时间
			lpNetOffPlantInfo->SetDynDataTime(timeData);
			//设置保存数据状态
 			lpNetOffPlantInfo->SetDynDataStatu(pDB,bEnd);

		}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

/**发送断网后保存的ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID            FIELD_SYSTEM_COMPANY
*                分厂名            FIELD_SYSTEM_FACTORY
*                机组ID            FIELD_SERVICE_PLANTID
*                起始索引          FIELD_SERVICE_ON_SEND_STARTID
*                测点类型          FIELD_SERVICE_CHANNTYPE
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*                时间值            FIELD_SERVICE_TIME
*返回值的字段码：
*                测点个数          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEPROC            24023
*/
int CTransactDaqData::NetOffReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	int iProcChannNum=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CNetOffPlantInfo *lpNetoffPlantInfo=(CNetOffPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_NETOFF);
	if (lpNetoffPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iProcChannNum);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	
	CRealPlantInfo * lpRealPlantInfo = lpNetoffPlantInfo->GetRealPlantInfo();

	if (NULL == lpRealPlantInfo)
	{
		lpRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

		if (lpNetoffPlantInfo==NULL)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lpNetoffPlantInfo->SetIDPointer(lpRealPlantInfo);
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	
	if (iProcChannNum <= 0)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24023接口，过程量断网数据保存，测点数为0,%s,%d"),__FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iProcChannNum);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	
	int iType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMilliSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMilliSecond > 0)
	{
		timeData.SetMillisecond(iMilliSecond);
	}

	//float *pfData=new float[iProcChannNum];
	std::vector<float> loData;
	loData.resize(iProcChannNum);
	float *pfData = &loData.front();
	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)pfData,iProcChannNum*sizeof(float));

	switch (iType)
	{
	case GE_ALLPROC:
		{
			//更新断网数据的缓存
			lpNetoffPlantInfo->UpdateProcData(pfData,iProcChannNum,timeData,iStartIndex);
			//设置断网保存数据的时间
			lpNetoffPlantInfo->SetProcDataTime(timeData);
			//设置保存数据状态
			lpNetoffPlantInfo->SetProcDataStatu(pDB,true);
		}
		break;
	}
	

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iProcChannNum);
	pFixComm->Write(pFixComm->GetAci());
	return iProcChannNum;
}

/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE         24024
*/
int CTransactDaqData::NetOffReceiveAllWirelessValue(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	int iLen=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CNetOffPlantInfo *lpNetOffPlantInfo=(CNetOffPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_NETOFF);
	CRealPlantInfo *lpPlantRealInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if (lpNetOffPlantInfo==NULL || lpPlantRealInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		/*		ASSERT(FALSE);*/
		return -1;
	}

	//设置指向实时类的设备指针

	if (NULL == lpNetOffPlantInfo->GetRealPlantInfo())
	{
		lpPlantRealInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

		if (lpNetOffPlantInfo==NULL)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lpNetOffPlantInfo->SetIDPointer(lpPlantRealInfo);
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);

	if (iStartIndex == 0)
	{
		InterlockedExchange(&g_netoff_StartFrom_Zero_Vib,1);
	}

	iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	if (iLen <=0)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24024接口，接收断网数据，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,%s,%d"),iLen,__FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);
	S_VibCharValue *pVibCharValue = &loVibCharValue.front();
	ZeroMemory(pVibCharValue,sizeof(S_VibCharValue)*iLen);

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	CBHDateTime loUpdateTime = timeData;
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMilliSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMilliSecond > 0)
	{
		timeData.SetMillisecond(iMilliSecond);
	}

	bool bEnd=false;

	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
	{
		if (g_netoff_StartFrom_Zero_Vib)
		{
			bEnd=true;
		}		
	}

	bool bFlag = true;

	lpPlantRealInfo->CalcVibDataGrade(pVibCharValue,iLen,iStartIndex);

	float lfScaleCoef = 0;
	int lnIsPatchedForPrecision = 0;
	vector<float> loPMin(iLen,0);
	vector<float> loPMax(iLen,0);

	if (bFlag)
	{
		BOOL lbLockGot = FALSE;

		try
		{
			lbLockGot = lpNetOffPlantInfo->m_oWriteToDbLock.AcquireWriterLock();

			//更新断网数据的缓存
			int lnUpdatedCount = lpNetOffPlantInfo->UpdateVibData(  NULL,
				pVibCharValue,
				0,
				iLen,
				loUpdateTime,
				iStartIndex,
				lfScaleCoef,
				lnIsPatchedForPrecision,
				loPMin,
				loPMax,
				bEnd);
			ASSERT(lnUpdatedCount == iLen);

			if (bEnd)
			{
				lpNetOffPlantInfo->SyncVibData();
			}

			//设置断网保存数据的时间
			lpNetOffPlantInfo->SetVibDataTime(timeData);

			//设置保存数据状态
			lpNetOffPlantInfo->SetVibDataStatu(pDB,bEnd);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			ASSERT(FALSE);
		}

		if (lbLockGot)
		{
			lpNetOffPlantInfo->m_oWriteToDbLock.ReleaseWriterLock();
		}


	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

int CTransactDaqData::JudgeSpecCompanyAlarm(string sCompany_,IDBInterFace *pDB_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
	int iFactory=pFactoryInfoPool->GetInfoNum();
	string sFactory="";
	for (int loop=0;loop<iFactory;++loop)
	{
		if(pFactoryInfoPool->GetSpecInfoPool(loop,sFactory)!=NULL)
			nCount+=JudgeSpecFactoryAlarm(sCompany_,sFactory,pDB_);
	}
	return nCount;
}

int CTransactDaqData::JudgeSpecFactoryAlarm(string sCompany_,string sFactory_,IDBInterFace *pDB_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL)
		return 0;
	CPlantInfoPool *pPlantInfoPool=pFactoryInfoPool->GetSpecInfoPool(sFactory_);
	if (pPlantInfoPool==NULL)
		return 0;

	_lockUpdateChannTime.BeginRead();
	bool bUpdate=_bUpdateChannInfo;
	_lockUpdateChannTime.EndRead();

	CFactoryInfoPool *pFactoryInfoPoolReal=NULL;
	CPlantInfoPool *pPlantInfoPoolReal=NULL;
	CRealPlantInfo  *pPlantInfoReal=NULL;
	if(bUpdate)
	{
		pFactoryInfoPoolReal=g_pCompanyPoolReal->GetSpecInfoPool(sCompany_);
		pPlantInfoPoolReal=pFactoryInfoPoolReal->GetSpecInfoPool(sFactory_);
		if(pFactoryInfoPoolReal == NULL || pPlantInfoPoolReal == NULL)
		{
			return 0;
		}
	}
	CAlarmPlantInfo *pPlantInfo=NULL;
	int iPlantNum=pPlantInfoPool->GetInfoNum();
	string sPlant="";
	
	for (int loop=0;loop<iPlantNum;++loop)
	{
		pPlantInfo=(CAlarmPlantInfo *)pPlantInfoPool->GetSpecInfoPool(loop,sPlant);
		if (pPlantInfo!=NULL)
		{

			pPlantInfo->JudgeAlarm(pDB_);

			++nCount;
			
			if(bUpdate)
			{//更新通道信息
				pPlantInfoReal=(CRealPlantInfo*)pPlantInfoPoolReal->GetSpecInfoPool(loop,sPlant);
				if (pPlantInfoReal!=NULL)
				{
					UpdateSpecPlantChannInfo(pPlantInfoReal,pPlantInfo,pDB_);
				}
			}
		}
	}
	return nCount;
}

/**发送ALLPROC过程量到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*                机组ID            FIELD_SERVICE_PLANTID
*                数据的时间              FIELD_SERVICE_TIME
*返回值的字段码：
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*                数据的时间              FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA        24051
*/
int CTransactDaqData::GetSpecPlantProcData(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM,-1);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	dtData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	CBHDateTime dtNewData = CBHDateTime::GetCurrentTime();
	dtNewData = pPlantInfo->GetRealProcDataDateTime();

	if ((dtNewData == g_dtInitTime) || dtData == dtNewData)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}

	int iLen=pPlantInfo->GetProcChannNum();

	float * pfData=new float[iLen];

	iLen=pPlantInfo->GetProcData(pfData,iLen);

	int lnProcJudgedStatus = pPlantInfo->GetProcJudgeRunStatus();

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (iLen>0)
	{
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char *)pfData,iLen*sizeof(float));
		pFixComm->SetItem(FIELD_SERVICE_TIME,dtNewData);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnProcJudgedStatus);
	}
		//返回值-1表示没有找到设备，-2表示数据没有更新,-3表示参数有误
	pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM,iLen);
	pFixComm->Write(pFixComm->GetAci());
	if(pfData!=NULL) delete [] pfData;
	return iLen;

}

/**发送指定机组的报警状态，为报警状态的结构体
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*                机组ID   FIELD_SERVICE_PLANTID
*返回值的字段码：
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID			 
#define     SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE       24052
*/
int CTransactDaqData::GetSpecPlantAllState(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	int iLen=pPlantInfo->GetVibChannNum();
	S_AlarmState *pState=new S_AlarmState[iLen];
	int iStateLen=iLen;
	iLen=pPlantInfo->GetVibChannState(pState,iLen);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
//	if (iLen>0)
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLVIBSTATE,(char*)pState,iLen*sizeof(S_AlarmState));
	}
	iLen=pPlantInfo->GetProcChannNum();
	if(iLen>iStateLen)
	{
		delete [] pState;
		pState=new S_AlarmState[iLen];
		iStateLen=iLen;
	}
	pPlantInfo->GetProcChannState(pState,iLen);
//	if (iLen>0)
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCSTATE,(char*)pState,iLen*sizeof(S_AlarmState));
	}

	iLen=pPlantInfo->GetAxisPairNum();
	if(iLen>iStateLen)
	{
		delete [] pState;
		pState=new S_AlarmState[iLen];
		iStateLen=iLen;
	}
	pPlantInfo->GetAxisLocationState(pState,iLen);
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE,(char*)pState,iLen*sizeof(S_AlarmState));
	}

	iLen=pPlantInfo->GetDynChannNum();
	if(iLen>iStateLen)
	{
		delete [] pState;
		pState=new S_AlarmState[iLen];
		iStateLen=iLen;
	}
	pPlantInfo->GetDynChannState(pState,iLen);
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_DYNCHANNNUM,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLDYNSTATE,(char*)pState,iLen*sizeof(S_AlarmState));
	}

	pFixComm->SetItem(FIELD_SERVICE_ALARMID, pPlantInfo->_iEventID);

	int lnSudType = -1;
	bool lbSudStatus = false;

	lnSudType = pPlantInfo->GetSudStatus(lbSudStatus);

	pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS, lbSudStatus);
	pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE, lnSudType);

	delete [] pState;
	//返回值-1表示没有找到设备，-2表示数据没有更新,-3表示参数有误
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

/**根据指定设备得到指定索引和长度的振动波形数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            振动测点特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数            FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数            FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            数据的时间              FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA     24050
*/
int CTransactDaqData::GetSpecPlantLengthVibData(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-1);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	dtData=pFixComm->GetTime(FIELD_SERVICE_TIME);

	CBHDateTime dtNewData=pPlantInfo->GetRealVibDataDateTime();

	if ((dtNewData == g_dtInitTime) || (dtData == dtNewData))
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	int lnVibJudgedRunStatus = pPlantInfo->GetVibJudgeRunStatus();

	if(iStartIndex < 0 || iLen <=0 || (iStartIndex + iLen) > pPlantInfo->GetVibChannNum())
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-3);
		pFixComm->Write(pFixComm->GetAci());

		ASSERT(FALSE);
		BHLOG_ERROR(STR("测点起始索引错误 Start=%d, Len=%d, Total=%d(公司:%s|分厂:%s|设备:%s)", 
			iStartIndex,
			iLen,
			pPlantInfo->GetVibChannNum(),
			sCompany.c_str(),
			sFactory.c_str(),
			sPlant.c_str()));

		return -3;
	}


	bool lbExistOnline = false;

	int nCount=0,iRealAllNum=0,nAllSmpNums = 0;
	vector<int> lSmpNums;
	int lnBufferSize = iLen*GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
	vector<char> loBuffer(lnBufferSize);
	for (int nloop = iStartIndex; nloop < iStartIndex+iLen; ++nloop)
	{

		//if (nAllSmpNums<D_SENDDATA_MAXNUMS)
		{
			//一次发送的总的波形点数不能超过100K，即46080个点
			S_ChannInfo loChannInfo = pPlantInfo->GetSpecVibChannInfo(nloop);

			string sChannID = loChannInfo._cChannID;

			if (sChannID.empty())
			{
				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-3);
				pFixComm->Write(pFixComm->GetAci());

				ASSERT(FALSE);
				BHLOG_ERROR(STR("测点%d值空(公司:%s|分厂:%s|设备:%s)", 
					nloop,
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str()));

				return -3;
			}

			string sUnit="";
			S_VibCharValue vibCharValue;
			pPlantInfo->GetSpecChannRadialAllChar(sChannID, vibCharValue,sUnit);

			if (GE_DATA_SOURCE_ONLINE == loChannInfo._iDataSource)
			{
				lbExistOnline = true;

				if (vibCharValue._iSmpNum < 1)
				{
					/*pFixComm->CreateAnswer(pFixComm->GetFunc());
					pFixComm->Write(pFixComm->GetAci());

					ASSERT(FALSE);
					BHLOG_ERROR(STR("测点%d采样点数(%d)不正确(公司:%s|分厂:%s|设备:%s)", 
						nloop,
						vibCharValue._iSmpNum,
						sCompany.c_str(),
						sFactory.c_str(),
						sPlant.c_str()));

					return -3;*/
				}

				lSmpNums.push_back(vibCharValue._iSmpNum);
				nAllSmpNums += vibCharValue._iSmpNum;
			}
			else if(GE_DATA_SOURCE_WIRELESS == loChannInfo._iDataSource)
			{
				ASSERT(0 == vibCharValue._iSmpNum);
				lSmpNums.push_back(vibCharValue._iSmpNum);
				nAllSmpNums += vibCharValue._iSmpNum;
			}
			else
			{
				lSmpNums.push_back(vibCharValue._iSmpNum);
				nAllSmpNums += vibCharValue._iSmpNum;
			}

			pPlantInfo->GetSpecRadialVibChannCustomData( nloop,
														(float *)(&loBuffer.front()+(nloop-iStartIndex)*GD_CUSTOMPARAM_BUFFSIZE*sizeof(float)));

			nCount++;
			//CString sTemp;sTemp.Format("Vib:iStartIndex:%d,nAllSmpNums:%d,nCount:%d",iStartIndex,nAllSmpNums,nCount);BHTRACE_DEBUG(sTemp);
		}
	}
	//得到实际发送的通道数，和实际发送的总的总的波形点数
	iLen=nCount;
	iRealAllNum = nAllSmpNums;
	if (iLen < 0)
	{
		return -1;
	}
	DATATYPE_WAVE *pWave=new DATATYPE_WAVE[nAllSmpNums];
	memset(pWave,0,iRealAllNum*sizeof(DATATYPE_WAVE));
	S_VibCharValue *pVibCharValue=new S_VibCharValue[iLen];

	DATATYPE_WAVE *pRawWave= NULL;
	vector<DATATYPE_WAVE> loRawWave(nAllSmpNums,0);
	pRawWave = &loRawWave.front();
	

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	

	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;
	vector<float> lvMaxValue(iLen);
	vector<float> lvMinValue(iLen);
	iLen=pPlantInfo->GetAllVibData(pWave,pVibCharValue,iRealAllNum,lSmpNums,iLen,pRawWave,lfScaleCoef,lnPrecisionEnabled,lvMaxValue,lvMinValue,iStartIndex);

	if (iLen>0)
	{
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,iStartIndex);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM,iRealAllNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pWave,iRealAllNum*sizeof(DATATYPE_WAVE));

		//writefile(pWave,iRealAllNum,777);
		pFixComm->SetItem(FIELD_SERVICE_TIME,dtNewData);

		pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)&loBuffer.front(),lnBufferSize);
		pFixComm->SetItem( FIELD_SERVICE_CHANN_BUFFSIZE,lnBufferSize);

		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnVibJudgedRunStatus);

		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);

		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pRawWave,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)&lvMinValue.front(),iLen*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)&lvMaxValue.front(),iLen*sizeof(float));
		}
		
		
	}
	//返回值-1表示没有找到设备，-2表示数据没有更新,-3表示参数有误
	pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,iLen);
	pFixComm->Write(pFixComm->GetAci());
	if(pVibCharValue!=NULL) delete [] pVibCharValue;
	if(pWave!=NULL) delete [] pWave;

	return iLen;
}

/**根据指定设备指定索引和长度的动态数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*		     分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA     24054
*/
int CTransactDaqData::GetSpecPlantLengthDynData(ICommInterFace *pFixComm,CCustomDB * pDB)
{

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-1);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	dtData=pFixComm->GetTime(FIELD_SERVICE_TIME);


	CBHDateTime dtNewData=CBHDateTime::GetCurrentTime();
	dtNewData=pPlantInfo->GetRealDynDataDateTime();

	if ((dtNewData == g_dtInitTime) || (dtData == dtNewData))
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-2);
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	if(iStartIndex < 0 || iLen <=0 || (iStartIndex + iLen) > pPlantInfo->GetDynChannNum())
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-3);
		pFixComm->Write(pFixComm->GetAci());

		ASSERT(FALSE);
		BHLOG_ERROR(STR("测点起始索引错误 Start=%d, Len=%d, Total=%d(公司:%s|分厂:%s|设备:%s)", 
			iStartIndex,
			iLen,
			pPlantInfo->GetDynChannNum(),
			sCompany.c_str(),
			sFactory.c_str(),
			sPlant.c_str()));

		return -3;
	}

	
    int nCount=0,iRealAllNum=0,nAllSmpNums = 0;
	int lnBufferSize = iLen*GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
	vector<char> loBuffer(lnBufferSize);

	vector<int> lSmpNums;
    for (int nloop = 0; nloop < iLen; ++nloop)
    {
		//if (nAllSmpNums<D_SENDDATA_MAXNUMS)
		{
			//一次发送的总的波形点数不能超过100K，即46080个点
			string sChannID = pPlantInfo->GetSpecDynChannInfo(iStartIndex + nloop)._cChannID;
			if (sChannID.empty())
			{
				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-3);
				pFixComm->Write(pFixComm->GetAci());

				ASSERT(FALSE);
				BHLOG_ERROR(STR("测点%d值空(公司:%s|分厂:%s|设备:%s)", 
					iStartIndex + nloop,
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str()));

				return -3;
			}

			S_DynCharValue dynCharValue;
			pPlantInfo->GetSpecDynChannCharValue(sChannID, dynCharValue);
			if (dynCharValue._iSmpNum < 1)
			{
				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->Write(pFixComm->GetAci());

				ASSERT(FALSE);
				BHLOG_ERROR(STR("测点%s采样点数(%d)不正确(公司:%s|分厂:%s|设备:%s)", 
					sChannID.c_str(),
					dynCharValue._iSmpNum,
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str()));

				return -3;
			}

			lSmpNums.push_back(dynCharValue._iSmpNum);
			nAllSmpNums += dynCharValue._iSmpNum;
			pPlantInfo->GetSpecDynChannCustomData(iStartIndex + nloop,(float *)(&loBuffer.front()+ nloop * GD_CUSTOMPARAM_BUFFSIZE*sizeof(float)));

			nCount++;
			//CString sTemp;sTemp.Format("Dyn::iStartIndex:%d,nAllSmpNums:%d,nCount:%d",iStartIndex,nAllSmpNums,nCount);BHTRACE_DEBUG(sTemp);
		}
	}
	//得到实际发送的通道数，和实际发送的总的总的波形点数
	iLen=nCount;
    iRealAllNum = nAllSmpNums;
	if (iLen<0 || iRealAllNum <0)
	{
		return -1;
	}

	DATATYPE_WAVE *pWave=new DATATYPE_WAVE[iRealAllNum];
	memset(pWave,0,iRealAllNum*sizeof(DATATYPE_WAVE));
	S_DynCharValue *pDynCharValue=new S_DynCharValue[iLen];

	DATATYPE_WAVE *pRawWave= NULL;
	vector<DATATYPE_WAVE> loRawWave(nAllSmpNums,0);
	pRawWave = &loRawWave.front();

	pFixComm->CreateAnswer(pFixComm->GetFunc());


	float lfScaleCoef = 0;
	int lnPrecisionEnabled = 0;
	vector<float> lvMaxValue(iLen);
	vector<float> lvMinValue(iLen);
 
	iLen = pPlantInfo->GetAllDynData(pWave,pDynCharValue,iRealAllNum,lSmpNums,iLen,pRawWave,lfScaleCoef,lnPrecisionEnabled,lvMaxValue,lvMinValue,iStartIndex);

	if (iLen>0)
	{
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,iStartIndex);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pWave,iRealAllNum*sizeof(DATATYPE_WAVE));
		//writefile(pWave,iRealAllNum,888);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM,iRealAllNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)&loBuffer.front(),lnBufferSize);
		pFixComm->SetItem( FIELD_SERVICE_CHANN_BUFFSIZE,lnBufferSize);
		pFixComm->SetItem(FIELD_SERVICE_TIME,dtNewData);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pRawWave,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)&lvMinValue.front(),iLen*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)&lvMaxValue.front(),iLen*sizeof(float));
		}

	}
	//CString sTemp;sTemp.Format("Dyn24057接口:iStartIndex%d,ilen:%d,iRealAllNum:%d",iStartIndex,iLen,iRealAllNum);BHTRACE_DEBUG(sTemp);
	//返回值-1表示没有找到设备，-2表示数据没有更新,-3表示参数有误
	pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,iLen);
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pDynCharValue);
	KD_DeletePoint(pWave);
	return iLen;
}



/** 接收数硬件状态
  *输入参数字段码：(n)
  *            数采卡索引             FIELD_SERVICE_DAQ_CARD_INDEX
  *            数采卡数量             FIELD_SERVICE_DAQ_CARD_NUM
  *            数采卡逻辑名称         FIELD_SERVICE_DAQ_CARD_NAME
  *            使用AI通道(CSV)        FIELD_SERVICE_DAQ_USED_AI_CHANNEL
  *            使用AI通道数           FIELD_SERVICE_AI_CHANN_NUM
  *            使用CI通道(CSV)        FIELD_SERVICE_DAQ_USED_CI_CHANNEL
  *            使用CI通道数           FIELD_SERVICE_CI_CHANN_NUM
  *            工作状况(CSV)          FIELD_SERVICE_DAQ_CARD_STATUS
  *返回值的字段码：
  *            接收状态      FIELD_SYSTEM_STATUS   (0正常， 其它 异常)

#define     SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS              24030
  */
int CTransactDaqData::RcvDAQHWStatus(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_HW_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_WARN(STR("无法打开文件%s, 尝试重建%s\n", statusFile));
		nRet = -1;
		verifyDAQHWStautsFile();
	}


	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_WARN(STR("无法解析文件%s, 尝试重建%s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		nRet = -2;
		verifyDAQHWStautsFile();
	}

	CBHDateTime current = CBHDateTime::GetCurrentTime();

	std::string sFileModifiedTime =
		root.get(FILE_MODIFIED_TIME, "").asString();
	CBHDateTime tFileModifiedTime;
	if(!tFileModifiedTime.ParseDateTime(sFileModifiedTime.c_str())){
		HZLOG_WARN(STR("无法读取文件%s上次更新时间, 尝试重建",
			statusFile));

		verifyDAQHWStautsFile();
		return -3;
	}


	/* 接收来自数采的状态 */
	int     rcvCardCount  = pFixComm->GetLong(FIELD_SERVICE_DAQ_CARD_NUM);
	int     rcvCardIndex  = pFixComm->GetLong(FIELD_SERVICE_DAQ_CARD_INDEX);
	CString rcvCardName   = pFixComm->Get(FIELD_SERVICE_DAQ_CARD_NAME);

	CString rcvAIChannels = pFixComm->Get(FIELD_SERVICE_DAQ_USED_AI_CHANNEL);
	int     rcvAIChannNum = pFixComm->GetLong(FIELD_SERVICE_AI_CHANN_NUM);
	CString rcvCIChannels = pFixComm->Get(FIELD_SERVICE_DAQ_USED_CI_CHANNEL);
	int     rcvCIChannNum = pFixComm->GetLong(FIELD_SERVICE_CI_CHANN_NUM);

	CString     rcvCardStatus = pFixComm->Get(FIELD_SERVICE_DAQ_CARD_STATUS);

	/* 容错处理*/
	if(rcvCardCount < 0 ){
		HZLOG_WARN(STR("数采卡数目错误: %d", rcvCardCount));
		return SetFieldWithStatus(pFixComm, FIELD_SYSTEM_STATUS, -5);
	}

	if(rcvCardIndex < 0 || rcvCardIndex >= rcvCardCount)	{
		HZLOG_WARN(STR("数采卡索引错误: index=%d, count=%d", rcvCardIndex, rcvCardCount));
		return SetFieldWithStatus(pFixComm, FIELD_SYSTEM_STATUS, -6);
	}

	/* 更新状态 */
	root[FILE_MODIFIED_TIME]      = current.Format(gc_cTimeFormat).GetString();

  	root[DAQ_LAST_UPDATE_TIME]    = current.Format(gc_cTimeFormat).GetString();
	root[DAQ_HW_STATUS_CARD_COUNT] = rcvCardCount;

	std::string cardId  = getStringFormattedAs("card%d", rcvCardIndex);

	Json::Value  card;
	card[DAQ_HW_STATUS_CARD_INDEX]        = rcvCardIndex;
	card[DAQ_HW_STATUS_CARD_NAME]         = rcvCardName.GetString();

	card[DAQ_HW_STATUS_USED_AI_CHANNELS]  = rcvAIChannels.GetString();
	card[DAQ_HW_STATUS_USED_AI_CHANN_NUM] = rcvAIChannNum;
	card[DAQ_HW_STATUS_USED_CI_CHANNELS]  = rcvCIChannels.GetString();
	card[DAQ_HW_STATUS_USED_CI_CHANN_NUM] = rcvCIChannNum;

	card[DAQ_HW_STATUS_STATUS]            = rcvCardStatus.GetString();

	root[DAQ_HW_STATUS_CARD_INFO][cardId] = card;

	/* 写入状态文件 */
	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	//HZLOG_INFO(STR("\n数采硬件状态文件更新=\n%s\n", convertedStream.c_str()));

	outStatusStream << convertedStream;
	outStatusStream.close();

	return SetFieldWithStatus(pFixComm, FIELD_SYSTEM_STATUS, nRet);
}

/** 接收数采软件状态(n)
  *输入参数字段码：
  *            版本号                                  FIELD_SERVICE_DAQ_SW_VERSION
  *            采集模式                                FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            启动时间                                FIELD_SERVICE_TIME
  *            接收间隙电压状况(bool: 正常 true)       FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            最近一次断网数据发送开始时间            FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            最近一次断网数据发送结杏时间            FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  *返回值的字段码：
  *            接收状态       FIELD_SYSTEM_STATUS    (0正常， 其它 异常)
#define     SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS              24031
  */
int CTransactDaqData::RcvDAQSWStatus(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_SW_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_WARN(STR("无法打开文件%s, 尝试重建%s\n", statusFile));
		nRet = -1;
		verifyDAQSWStautsFile();
	}


	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_WARN(STR("无法解析文件%s, 尝试重建%s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		nRet = -2;
		verifyDAQSWStautsFile();
	}

	CBHDateTime current = CBHDateTime::GetCurrentTime();

	std::string sFileModifiedTime =
		root.get(FILE_MODIFIED_TIME, "").asString();
	CBHDateTime tFileModifiedTime;
	if(!tFileModifiedTime.ParseDateTime(sFileModifiedTime.c_str())){
		HZLOG_WARN(STR("无法读取文件%s上次更新时间, 尝试重建",
			statusFile));

		verifyDAQHWStautsFile();
		return -3;
	}

	/* 接收来自数采的状态 */
	CString      rcvSWVersion = pFixComm->Get(FIELD_SERVICE_DAQ_SW_VERSION);
	int          rcvDAQType   = pFixComm->GetLong(FIELD_SERVICE_DAQ_ACQUIRE_TYPE);
	CBHDateTime rcvStartTime = pFixComm->GetTime(FIELD_SERVICE_TIME);
	int          rcvGapStatus = pFixComm->GetLong(FIELD_SERVICE_DAQ_GAP_RCV_STATUS);

	CBHDateTime rcvLatestOfflineBegin = pFixComm->GetTime(FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME);
    CBHDateTime rcvLatestOfflineEnd   = pFixComm->GetTime(FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME);

	/* 更新状态 */
	root[FILE_MODIFIED_TIME]      = current.Format(gc_cTimeFormat).GetString();
  	root[DAQ_LAST_UPDATE_TIME]    = current.Format(gc_cTimeFormat).GetString();

	root[DAQ_SW_STATUS_VERSION]        = rcvSWVersion.GetString();
	root[DAQ_SW_STATUS_ACQUIRE_TYPE]   = rcvDAQType;
	root[DAQ_SW_STATUS_APP_START_TIME] = rcvStartTime.Format(gc_cTimeFormat).GetString();
	root[DAQ_SW_STATUS_RCV_GAP_STATUS] = rcvGapStatus;

	root[DAQ_SW_STATUS_LATEST_OFFLINE_SEND_BEGIN] =
	                              rcvLatestOfflineBegin.Format(gc_cTimeFormat).GetString();
	root[DAQ_SW_STATUS_LATEST_OFFLINE_SEND_END]   =
	                              rcvLatestOfflineEnd.Format(gc_cTimeFormat).GetString();

	/* 写入状态文件 */
	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	//HZLOG_INFO(STR("\n数采软件状态文件更新=\n%s\n", convertedStream.c_str()));

	outStatusStream << convertedStream;
	outStatusStream.close();

	return SetFieldWithStatus(pFixComm, FIELD_SYSTEM_STATUS, nRet);
}

int CTransactDaqData::UpdateSpecPlantChannInfo(CRealPlantInfo *pPlantInfo_,CAlarmPlantInfo * pAlarmPlantInfo_,IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	if ((NULL==pPlantInfo_)||(NULL==pAlarmPlantInfo_))
		return -3;
	int nCount=-1;
	S_ChannInfo channInfo;
	pDB_->Cmd("select * from [%s] ",gc_cChannTable);
	pDB_->Cmd(" where [%s]='%s' and [%s]='%s' ",gc_cPlantNo,pAlarmPlantInfo_->GetPlantID().c_str(),gc_cCompany,pAlarmPlantInfo_->GetCompanyName().c_str());
	while(pDB_->More())
	{
		channInfo.GetChannInfo(pDB_->getPDB());
		pPlantInfo_->UpdateSpecChannInfo(channInfo._cChannID,channInfo);
		pAlarmPlantInfo_->UpdateSpecChannInfo(channInfo._cChannID,channInfo);
		++nCount;
	}
	return nCount;
}

/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *            报警确认记录是否保存在数据表中 FIELD_SYSTEM_EVENT_TYPE 1:不保存 0：保存 默认0
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *			   #define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */

int CTransactDaqData::Srv_Alarm_Confirm( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int lnRet = 0;

	if (pDB && pFixComm)
	{
		string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
		string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
		string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

		CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,
																				sFactory,
																				sPlant,
																				PLANTINFO_ALARM);
		if (pPlantInfo==NULL)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnRet = pPlantInfo->AlarmConfirm(*pFixComm,*pDB);


		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SERVICE_DATA_CODE,lnRet);
		pFixComm->Write(pFixComm->GetAci());

	}
	return lnRet;
}

/**启停车过程发送指定机组的振动测点实时所有振动特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
  *            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
  * #define     SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE        24100
  */

int CTransactDaqData::ReceiveSudVibValue( ICommInterFace * pFixComm,IDBInterFace *pDB )
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	CSudPlantInfo *pPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	
	
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	if (iStartIndex == 0)
	{
		InterlockedExchange(&g_StartFrom_Zero_Vib,1);
	}

	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24100接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);
		zdlTraceLine(lstrLogData);

		lstrLogData.Format(_T("24100接口，数采参数不正确，sCompany:%s,sFactory:%s,sPlant :%s, %s,%d"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(), __FILE__,__LINE__);
		zdlTraceLine(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
	{    
		if (g_StartFrom_Zero_Vib)
		{
			bEnd=true;
		}
	}
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;
	bool bFlag=false;//判断是否解压缩正确

	std::vector<S_OldVibCharValue> loOldVibCharValue;
	loOldVibCharValue.resize(iLen);

	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);

	S_OldVibCharValue *pOldVibCharValue = &loOldVibCharValue.front();
	S_VibCharValue *pVibCharValue = &loVibCharValue.front();

	int iRealAllNum=0;
	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldVibCharValue,iLen*sizeof(S_OldVibCharValue));
		pPlantInfo->ChangeVibCharValueToNew(pVibCharValue,pOldVibCharValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
	}
	else
	{
		iRealAllNum=iRealNum;

		if (iRealAllNum<=G_SAMPLE_LIMITATION)
		{			
			CString lstrLogData;
			lstrLogData.Format(_T("	24100接口，数采参数不正确，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d,，中间件调整为%d, %s,%d "),
								  iLen,
								  iRealAllNum,
								  iSmpNum, 
								  iRealNum*iLen,
								  __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			iRealAllNum = iRealNum*iLen;
		}

		if (iRealAllNum  <=G_SAMPLE_LIMITATION)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("	24100接口，数采参数不正确，中间件不处理，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								  iLen,
								  iRealAllNum,
								  iSmpNum, __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

		for (int i =0;i<iLen;i++)
		{
			S_VibCharValue  * lpVibCharValue = &pVibCharValue[i];

			if (!IsVibCharValid(lpVibCharValue))
			{
				CString lstrCharValue;
				lstrCharValue.Format("_fCharAll:%f _fCharHalf:%f _fCharOne:%f _fCharTwo:%f _fCharThree:%f _fCharRes:%f _fDC:%f _fGap:%f _fPhaseHalf:%f _fPhaseOne:%f _fPhaseTwo:%f _fPhaseThree:%f _iSmpFreq:%f _iSmpNum:%f _iRev:%f ",
					lpVibCharValue->_fCharAll,lpVibCharValue->_fCharHalf,lpVibCharValue->_fCharOne,lpVibCharValue->_fCharTwo,
					(float)lpVibCharValue->_fCharThree,(float)lpVibCharValue->_fCharRes,(float)lpVibCharValue->_fDC,(float)lpVibCharValue->_fGap,
					(float)lpVibCharValue->_fPhaseHalf,(float)lpVibCharValue->_fPhaseOne,(float)lpVibCharValue->_fPhaseTwo,(float)lpVibCharValue->_fPhaseThree,
					(float)lpVibCharValue->_iSmpFreq,(float)lpVibCharValue->_iSmpNum,(float)lpVibCharValue->_iRev);
				CString lstrLogData;
				lstrLogData.Format(_T("24100接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d 全部特征值:%s %s %d \r\n"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),lpVibCharValue->_fCharAll, lpVibCharValue->_iSmpNum,lpVibCharValue->_iRev,lstrCharValue, __FILE__,__LINE__);
				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);

				DATATYPE_WAVE *pByteBuf=new DATATYPE_WAVE[iRealAllNum];
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

				delete[] pByteBuf;
				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
				pFixComm->Write(pFixComm->GetAci());

				return -1;
			}
		}

	}


	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTime();
	}

	int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	

	ASSERT(pVibCharValue->_iSmpNum!=0);

	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);

	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();

	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;


	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;

	CBHDateTime loTimeUpdate = CBHDateTime::GetCurrentTime();

	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
	if (iAllBytes>0)
	{
		CZipImpl zipImpl;

		std::vector<BYTE> loZipByte(iAllBytes);
		pZipByte = &loZipByte.front();
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
		if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
		{
			ASSERT(FALSE);
			bFlag=true;
		}else
		{
			ASSERT(FALSE);
		}

	}
	else
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
		//patch for precision problem
		lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

		if(lnIsPatchedForPrecision>0)
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
			pByteBuf = pByteBuf_precision;

		}

		
		bFlag=true;
	}
	if (bFlag)
	{

		//1.得到自定参数的长度
		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24100接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
			}
		}

		_DECLARE_PERF_MEASURE_TIME();

		//更新实时数据的缓存

		pPlantInfo->UpdateVibData(pByteBuf,pVibCharValue,iRealAllNum,iLen,loTimeUpdate,iStartIndex,lfScaleCoef,
			lnIsPatchedForPrecision,
			loPMin,
			loPMax,bEnd);

		if (lbShouldComputCustomParam)
		{
			_BEGIN_PERF_MEASURE_TIME();
			//计算自定义参数信息
			pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_VIBCHANN,FALSE);

			_END_PERF_MEASURE_TIME("pPlantInfo->CalcCustomParamData_New");

		}else
		{
			_BEGIN_PERF_MEASURE_TIME();

			pPlantInfo->TransferCustomParamData(iStartIndex, 
				iLen,
				loCustomBuffer, 
				GE_VIBCHANN,FALSE);

			_END_PERF_MEASURE_TIME("pAlarmPlantInfo->TransferCustomParamData");

		}
		pPlantInfo->SetDataTime(timeData,iMicroSecond);
		pPlantInfo->SetDataStatu(pDB,true,false,iEventID,(E_SUD_TYPE)iType);
		pRealPlantInfo->SetDataStatuEx(pDB,true,iEventID,(E_SUD_TYPE)iType);
		pRealPlantInfo->SetDataTime(timeData,iMicroSecond);
		if (bEnd)
		{
			pPlantInfo->SyncVibData();		
			pPlantInfo->SaveSudTrend(pDB,SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE);
		}

	}

	updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_VIB_TIME,
		CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));

	//获取快变学习参数信息,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);

			if(pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_VIBCHANN)>0)
			{
				if(channInfo._chAlarmParam._bCharFast)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,
						(char *)(channInfo._chAlarmParam._pThLearnParamF),
						sizeof(CThresholdLearnParam)*D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SERVICE_LEN,D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,1);
				}
				else
				{
					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,-1);
				}
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
				{
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				}

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_LEN,-1);
			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

int CTransactDaqData::ReceiveSudDynValue( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	CSudPlantInfo *pPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);


	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	if (0 == iStartIndex)
	{
		InterlockedExchange(&g_StartFrom_Zero_Dyn,1);
	}

	if (iRealNum<=128)
	{
		//	iRealNum = iRealNum*iLen;
	}

	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24101接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END) == 1)
	{	
		if (g_StartFrom_Zero_Dyn)
		{
			bEnd=true;
		}		
	}
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;
	bool bFlag=false;//判断是否解压缩正确
	
	std::vector<S_OldDynCharValue> loOldDynCharValue;
	loOldDynCharValue.resize(iLen);

	std::vector<S_DynCharValue> loDynCharValue;
	loDynCharValue.resize(iLen);

	S_OldDynCharValue *pOldDynCharValue = &loOldDynCharValue.front();
	S_DynCharValue    *pDynCharValue    = &loDynCharValue.front();

	int iRealAllNum=0;
	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;

		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldDynCharValue,iLen*sizeof(S_OldDynCharValue));
			pPlantInfo->ChangeDynCharValueToNew(pDynCharValue,pOldDynCharValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
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
	else
	{
		iRealAllNum=iRealNum;

		if (iRealAllNum<=G_SAMPLE_LIMITATION)
		{			
			CString lstrLogData;
			lstrLogData.Format(_T("	24101接口，数采参数不正确，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d,，中间件调整为%d, %s,%d "),
								  iLen,
								  iRealAllNum,
								  iSmpNum, 
								  iRealNum*iLen,
								  __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			iRealAllNum = iRealNum*iLen;
		}

		if (iRealAllNum  <=G_SAMPLE_LIMITATION)
		{
			CString lstrLogData;
			lstrLogData.Format(_T("	24101接口，数采参数不正确，中间件不处理，\
								  FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								  FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								  FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								  iLen,
								  iRealAllNum,
								  iSmpNum, __FILE__,__LINE__);
			zdlTraceLine(lstrLogData);
			CHZLogManage::Error(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		for (int i =0;i<iLen;i++)
		{
			S_DynCharValue   * lpVibCharValue = &pDynCharValue[i];
			if (!IsDynCharValid(lpVibCharValue))
			{
				CString lstrLogData;
				lstrLogData.Format(		_T("24101接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d  测点索引:%d %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lpVibCharValue->_fCharValue0,
					lpVibCharValue->_iSmpNum,
					lpVibCharValue->_iRev,
					iStartIndex+i,
					__FILE__,__LINE__);
				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
				return -1;
			}
		}

	}

	if (iRealAllNum  <=256)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24101接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;
	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);

	CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTime();

	if (iAllBytes>0)
	{
		try
		{
			CZipImpl zipImpl;
			std::vector<BYTE> loZipByte;
			loZipByte.resize(iAllBytes);
			pZipByte = &loZipByte.front();
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
			if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
				bFlag=true;
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
	else
	{
		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
			//patch for precision problem
			lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

			if(lnIsPatchedForPrecision>0)
			{
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
				pByteBuf = pByteBuf_precision;

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
		bFlag=true;
	}

	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTime();
	}

	int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);

	if (bFlag)
	{
		//1.得到自定参数的长度
		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24101接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				CHZLogManage::Error(lstrLogData);
				zdlTraceLine(lstrLogData);
			}
		}

		_DECLARE_PERF_MEASURE_TIME();
		//更新实时数据的缓存
		try
		{

			pPlantInfo->UpdateDynData( pByteBuf,
				pDynCharValue,
				iRealAllNum,
				iLen,
				loUpdateTime,
				iStartIndex,
				lfScaleCoef,
				lnIsPatchedForPrecision,
				loPMin,
				loPMax,
				bEnd);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);		
		}
		//计算自定义参数信息
		try
		{
			if (lbShouldComputCustomParam)
			{
				pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, GE_RODSINKCHANN);
			}else
			{
				pPlantInfo->TransferCustomParamData(iStartIndex, iLen,loCustomBuffer,GE_RODSINKCHANN);
			}

			pPlantInfo->SetDataTime(timeData,iMicroSecond);
			pPlantInfo->SetDataStatu(pDB,true,false,iEventID,(E_SUD_TYPE)iType);
			pRealPlantInfo->SetDataStatuEx(pDB,true,iEventID,(E_SUD_TYPE)iType);

			if (bEnd)
			{
				pPlantInfo->SyncDynData();
				pPlantInfo->SaveSudTrend(pDB,SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE);
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
	}

	try
	{
		updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_DYN_TIME,
			CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	//获取动态测点的间隙电压数组,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			int lnRet = 0;

			try
			{
				lnRet = pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_RODSINKCHANN);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			if(lnRet > 0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			//CString strTmp;strTmp.Format("动态测点%s,间隙电压：%.2f",channInfo._cChannNo,channInfo._fGapVoltage);
			//CHZLogManage::Debug(strTmp,"CTransactDaqData","ReceiveDynValue");
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

int CTransactDaqData::ReceiveSudProcValue( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	CSudPlantInfo *pPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	int iType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iReturnType=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_CHANNTYPE);

	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTime();
	}

	int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);


	if (0>= iProcChannNum )
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24003 接口，数采参数不正确，FIELD_SERVICE_ON_SEND_PROCCHANNNUM:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<float> loData;
	loData.resize(iProcChannNum);
	float *pfData =  &loData.front();

	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)pfData,iProcChannNum*sizeof(float));


	switch (iType)
	{
	case GE_ALLPROC:
		pPlantInfo->UpdateProcData(pfData,iProcChannNum,timeData,iStartIndex);
		break;
	}

	pPlantInfo->SetDataTime(timeData,iMicroSecond);
	pPlantInfo->SetDataStatu(pDB,true,false,iEventID,(E_SUD_TYPE)iSudType);
	pRealPlantInfo->SetDataStatuEx(pDB,true,iEventID,(E_SUD_TYPE)iSudType);
	pPlantInfo->SaveSudTrend(pDB,SERVICE_SERVICE_ON_SUD_RECEIVEPROC);

	loData.clear();

	updateDAQSWStautsFile(DAQ_SW_STATUS_RCV_PROC_TIME,
							CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat));

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	switch(iReturnType)
	{
	case GE_ALLPROC:
		iProcChannNum=pPlantInfo->GetProcChannNum();
		loData.resize(iProcChannNum);
		pfData = &loData.front();
		pPlantInfo->GetProcData(pfData,iProcChannNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACE,(char *)pfData,iProcChannNum*GD_FLOAT_SIZE);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,iProcChannNum);
		break;
	}
	pFixComm->Write(pFixComm->GetAci());
	return iProcChannNum;
}

int CTransactDaqData::ReceiveVibFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	CAlarmPlantInfo *pAlarmPlantInfo = (CAlarmPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo==NULL || pAlarmPlantInfo == NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	int iAlarmStatus = pFixComm->GetLong(FIELD_SERVICE_ALARM_STATUS);
	int iEventID    = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	if (iStartIndex == 0)
	{
		InterlockedExchange(&g_netoff_StartFrom_Zero_Vib,1);
	}

	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24104接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		BHLOG_ERROR(lstrLogData);

		lstrLogData.Format(_T("24104接口，数采参数不正确，sCompany:%s,sFactory:%s,sPlant :%s, %s,%d"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(), __FILE__,__LINE__);
		BHLOG_ERROR(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
	{    
		if (g_netoff_StartFrom_Zero_Vib)
		{
			bEnd=true;
		}
	}
	bool bFlag=false;//判断是否解压缩正确

	std::vector<S_VibCharValue> loNewCharBuffer;
	loNewCharBuffer.resize(iLen);

	S_VibCharValue *pVibCharValue=&loNewCharBuffer.front();

	int iRealAllNum=0;
	
	iRealAllNum=iRealNum;

	if (iRealAllNum  <=G_SAMPLE_LIMITATION)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("	24104接口，数采参数不正确，中间件不处理，\
								FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								iLen,
								iRealAllNum,
								iSmpNum, __FILE__,__LINE__);

		BHLOG_ERROR(lstrLogData);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

	for (int i =0;i<iLen;i++)
	{
		S_VibCharValue  * lpVibCharValue = &pVibCharValue[i];

		if (!IsVibCharValid(lpVibCharValue))
		{
			CString lstrCharValue;
			lstrCharValue.Format("_fCharAll:%f _fCharHalf:%f _fCharOne:%f _fCharTwo:%f _fCharThree:%f _fCharRes:%f _fDC:%f _fGap:%f _fPhaseHalf:%f _fPhaseOne:%f _fPhaseTwo:%f _fPhaseThree:%f _iSmpFreq:%f _iSmpNum:%f _iRev:%f ",
				lpVibCharValue->_fCharAll,lpVibCharValue->_fCharHalf,lpVibCharValue->_fCharOne,lpVibCharValue->_fCharTwo,
				(float)lpVibCharValue->_fCharThree,(float)lpVibCharValue->_fCharRes,(float)lpVibCharValue->_fDC,(float)lpVibCharValue->_fGap,
				(float)lpVibCharValue->_fPhaseHalf,(float)lpVibCharValue->_fPhaseOne,(float)lpVibCharValue->_fPhaseTwo,(float)lpVibCharValue->_fPhaseThree,
				(float)lpVibCharValue->_iSmpFreq,(float)lpVibCharValue->_iSmpNum,(float)lpVibCharValue->_iRev);
			CString lstrLogData;
			lstrLogData.Format(_T("24104接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d 全部特征值:%s %s %d \r\n"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),lpVibCharValue->_fCharAll, lpVibCharValue->_iSmpNum,lpVibCharValue->_iRev,lstrCharValue, __FILE__,__LINE__);
			BHLOG_ERROR(lstrLogData);
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}

	CBHDateTime timeData=CBHDateTime::GetCurrentTimeEx();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}

	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	timeData.SetMillisecond(iMicroSecond);

	std::vector<DATATYPE_WAVE> loByteBuffer(iRealAllNum);
	DATATYPE_WAVE *pByteBuf=&loByteBuffer.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	ASSERT(pVibCharValue->_iSmpNum!=0);

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;

	CBHDateTime loTimeUpdate = CBHDateTime::GetCurrentTimeEx();


	//得到报警类型数组
	std::vector<unsigned int>louiType;
	louiType.resize(iLen);
	unsigned int *puiType=&louiType.front();
	memset(puiType,0,iLen*sizeof(int));
	pFixComm->GetItemBuf(FIELD_SERVICE_ALARM_TYPE, (char *)puiType, iLen*sizeof(int));
	//得到自定义参数模板ID和自定参数BUFF数据
	string sTempletID="";
	sTempletID=pFixComm->GetItem(FIELD_SYSTEM_TEMPID);


	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
	if (iAllBytes>0)
	{
		CZipImpl zipImpl;
		std::vector<BYTE> loZipBuffer(iAllBytes);
		pZipByte= &loZipBuffer.front();
		
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
		if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
		{
			ASSERT(FALSE);
			bFlag=true;
		}else
		{
			ASSERT(FALSE);
		}
		

	}
	else
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

		//patch for precision problem
		lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

		if(lnIsPatchedForPrecision>0)
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
			pByteBuf = pByteBuf_precision;

		}

		
		bFlag=true;
	}
	if (bFlag)
	{

		std::vector<S_CustomParamData> loCustomPromBuffer(iLen);		
		S_CustomParamData *pfCustomParom= &loCustomPromBuffer.front();
	

		//1.得到自定参数的长度
		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24101接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				BHLOG_ERROR(lstrLogData);				
			}
		}

		try
		{
			if (lbShouldComputCustomParam)
			{
				//计算自定义参数信息
				pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pVibCharValue,pByteBuf,pfCustomParom,GE_VIBCHANN,FALSE);

			}else
			{

 				pPlantInfo->TransferCustomParamData(iStartIndex, 
 					iLen,
 					loCustomBuffer,
 					pfCustomParom,
 					GE_VIBCHANN,FALSE);

			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);
			
		}
		
		//pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)pfCustomParom,iLen*sizeof(S_CustomParamData));
		pPlantInfo->CalcVibDataGrade(pVibCharValue,iLen,iStartIndex);

		try
		{
			pPlantInfo->SaveVibFastTrend(pDB,pByteBuf,pVibCharValue,pfCustomParom,iRealAllNum,iLen,timeData,iStartIndex,iEventID,iAlarmStatus,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax,bEnd);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);
		}	

		
		if(pAlarmPlantInfo->VibAlarmAddLogTimeExists(timeData))
		{
			try
			{

				int iStatus = 0;

				if(iEventID<= 0)
				{
					iEventID = pAlarmPlantInfo->QueryVibAlarmEventIDByTime(pDB,timeData);
				}

				if(iEventID >= 0)
				{

					if(iStartIndex == 0)
					{
						iStatus=pAlarmPlantInfo->SaveVibFAlarmData(iStartIndex,iLen,pVibCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,timeData,iMicroSecond,iRealNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
						if(iStatus >= 0)
						{
							pAlarmPlantInfo->SaveVibFastDataFromTrendData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLen);
						}
					}
					else
					{
						iStatus = pAlarmPlantInfo->InsertVibAlarmRecordFromTrendData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLen);
						if(iStatus >= 0)
						{
							pAlarmPlantInfo->SaveVibFAlarmData(iStartIndex,iLen,pVibCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,timeData,iMicroSecond,iRealNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
						}
					}

					if(iStatus >= 0)
					{
						pAlarmPlantInfo->UpdateAlarmSyncFlag(pDB,iEventID,timeData);
					}


				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				BHLOG_FATAL(lstrErrorLog);
			}

			pAlarmPlantInfo->DescVibFastChangeAlarmAddLogTime(timeData);

		}
		

	}

	
	//获取快变学习参数信息,并返回到发送数据的数据采集器

	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);

			if(pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_VIBCHANN)>0)
			{
				if(channInfo._chAlarmParam._bCharFast)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,
						(char *)(channInfo._chAlarmParam._pThLearnParamF),
						sizeof(CThresholdLearnParam)*D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SERVICE_LEN,D_BHKD_ALARM_CHARTYPE_NUM);

					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,1);
				}
				else 
				{
					pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,-1);
				}
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
				{
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				}

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_LEN,-1);
			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	else
	{
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

int CTransactDaqData::ReceiveDynFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	
	CAlarmPlantInfo *pAlarmPlantInfo = (CAlarmPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo==NULL || pAlarmPlantInfo == NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	int iAlarmStatus = pFixComm->GetLong(FIELD_SERVICE_ALARM_STATUS);
	int iEventID    = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	if (0 == iStartIndex)
	{
		InterlockedExchange(&g_netoff_StartFrom_Zero_Dyn,1);
	}

	if (0>= iRealNum || 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24105接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,iRealNum,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END) == 1)
	{	
		if (g_netoff_StartFrom_Zero_Dyn)
		{
			bEnd=true;
		}		
	}
	
	bool bFlag=false;//判断是否解压缩正确

	std::vector<S_DynCharValue> loNewCharBuffer;
	loNewCharBuffer.resize(iLen);

	S_DynCharValue *pDynCharValue=&loNewCharBuffer.front();

	int iRealAllNum=0;
	
	iRealAllNum=iRealNum;

	if (iRealAllNum  <=G_SAMPLE_LIMITATION)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("	24105接口，数采参数不正确，中间件不处理，\
								FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,\
								FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,\
								FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),
								iLen,
								iRealAllNum,
								iSmpNum, __FILE__,__LINE__);
		BHLOG_FATAL(lstrLogData);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	try
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}

	for (int i =0;i<iLen;i++)
	{
		S_DynCharValue   * lpVibCharValue = &pDynCharValue[i];
		if (!IsDynCharValid(lpVibCharValue))
		{
			CString lstrLogData;
			lstrLogData.Format(		_T("24105接口，特征值不正确，%s %s %s 通频:%f 采样点:%d 转速:%d  测点索引:%d %s %d\r\n"),
				sCompany.c_str(),
				sFactory.c_str(),
				sPlant.c_str(),
				lpVibCharValue->_fCharValue0,
				lpVibCharValue->_iSmpNum,
				lpVibCharValue->_iRev,
				iStartIndex+i,
				__FILE__,__LINE__);
			BHLOG_FATAL(lstrLogData);
			return -1;
		}
	}
	vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;
	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);

	CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTimeEx();

	if (iAllBytes>0)
	{
		try
		{
			CZipImpl zipImpl;
			std::vector<BYTE> loZipBuffer(iAllBytes);
			pZipByte= &loZipBuffer.front();
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
			if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
				bFlag=true;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);
		}
	}
	else
	{
		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
			//patch for precision problem
			lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

			if(lnIsPatchedForPrecision>0)
			{
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
				pByteBuf = pByteBuf_precision;

			}
			
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);
		}
		bFlag=true;
	}

	CBHDateTime timeData=CBHDateTime::GetCurrentTimeEx();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}

	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	timeData.SetMillisecond(iMicroSecond);

	//得到报警类型数组
	std::vector<unsigned int> louiType;
	louiType.resize(iLen);
	unsigned int *puiType = &louiType.front();
	memset(puiType,0,iLen*sizeof(int));

	pFixComm->GetItemBuf(FIELD_SERVICE_ALARM_TYPE, (char *)puiType, iLen*sizeof(int));
	//得到自定义参数模板ID和自定参数BUFF数据
	string sTempletID="";
	sTempletID=pFixComm->GetItem(FIELD_SYSTEM_TEMPID);

	if (bFlag)
	{
		vector<S_CustomParamData> loCustomParam;
		loCustomParam.resize(iLen);

		S_CustomParamData *pfCustomParom = &loCustomParam.front();

		int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		BOOL lbShouldComputCustomParam = TRUE;

		std::vector<char> loCustomBuffer(lnBufferSize);

		//2.判断是否需要自己计算自定义特征值
		if (lnBufferSize>0)
		{
			int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if (lnChannCountCompute == iLen)
			{		
				lbShouldComputCustomParam = FALSE;
				pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


			}else
			{
				ASSERT(FALSE);
				CString lstrLogData;
				lstrLogData.Format(_T("24105接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
					sCompany.c_str(),
					sFactory.c_str(),
					sPlant.c_str(),
					lnChannCountCompute, 
					iLen,
					__FILE__,
					__LINE__);

				BHLOG_FATAL(lstrLogData);
			}
		}
		
		try
		{
			if (lbShouldComputCustomParam)
			{
				//计算自定义参数信息

				pPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pDynCharValue,pByteBuf,pfCustomParom,GE_RODSINKCHANN,FALSE);

			}else
			{

				pPlantInfo->TransferCustomParamData(iStartIndex, 
					iLen,
					loCustomBuffer,
					pfCustomParom,
					GE_RODSINKCHANN,FALSE);

			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);
		}

		try
		{

			pPlantInfo->SaveDynFastTrend(pDB,pByteBuf,pDynCharValue,pfCustomParom,iRealAllNum,iLen,timeData,iStartIndex,iEventID,iAlarmStatus,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax,bEnd);
		
		
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			BHLOG_FATAL(lstrErrorLog);		
		}

		
		if(pAlarmPlantInfo->DynAlarmAddLogTimeExists(timeData))
		{
			try
			{

				int iStatus = 0;

				if(iEventID <= 0)
				{
					iEventID = pAlarmPlantInfo->QueryDynAlarmEventIDByTime(pDB,timeData);
				}
				if(iEventID >= 0)
				{
					if(iStartIndex == 0)
					{
						iStatus=pAlarmPlantInfo->SaveDynAlarmData(iStartIndex,iLen,pDynCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,timeData,iMicroSecond,iRealAllNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);

						if(iStatus >= 0)
						{
							pAlarmPlantInfo->SaveDynFastDataFromTrendData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLen);
						}
					}
					else
					{
						iStatus = pAlarmPlantInfo->InsertDynAlarmRecordFromTrendData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLen);
						if(iStatus >= 0)
						{
							pAlarmPlantInfo->SaveDynAlarmData(iStartIndex,iLen,pDynCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,timeData,iMicroSecond,iRealAllNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
						}
					}

					if(iStatus >= 0)
					{
						pAlarmPlantInfo->UpdateAlarmSyncFlag(pDB,iEventID,timeData);
					}
				}
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				BHLOG_FATAL(lstrErrorLog);		
			}

			pAlarmPlantInfo->DescDynFastChangeAlarmAddLogTime(timeData);
			
		}
	
		
	}


	//获取动态测点的间隙电压数组,并返回到发送数据的数据采集器
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			int lnRet = 0;

			try
			{
				lnRet = pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_RODSINKCHANN);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				BHLOG_FATAL(lstrErrorLog);		
			}

			if(lnRet > 0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iLen;
}

int CTransactDaqData::ReceiveProcFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	int iType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iReturnType=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_CHANNTYPE);
	int iAlarmStatus = pFixComm->GetLong(FIELD_SERVICE_ALARM_STATUS);
	int iEventID    = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTimeEx();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
	{
		timeData=CBHDateTime::GetCurrentTimeEx();
	}

	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	timeData.SetMillisecond(iMicroSecond);

	if (0>= iProcChannNum )
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24106 接口，数采参数不正确，FIELD_SERVICE_ON_SEND_PROCCHANNNUM:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	vector<float> loData;
	loData.resize(iProcChannNum);
	
	float *pfData = &loData.front();

	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)pfData,iProcChannNum*sizeof(float));


	switch (iType)
	{
	case GE_ALLPROC:
		pPlantInfo->SaveProcFastTrend(pDB,pfData,iProcChannNum,timeData,iEventID,iAlarmStatus,iStartIndex);
		break;
	}


	loData.clear();

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	switch(iReturnType)
	{
	case GE_ALLPROC:
		iProcChannNum=pPlantInfo->GetProcChannNum();
		loData.resize(iProcChannNum);
		pfData=&loData.front();
		pFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACE,(char *)pfData,iProcChannNum*GD_FLOAT_SIZE);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,iProcChannNum);
		break;
	}
	pFixComm->Write(pFixComm->GetAci());
	return iProcChannNum;
}

/**发送ALLPROC过程量或轴位移快变报警数据到中间件模块
	*输入参数字段码：
	*           公司ID            FIELD_SYSTEM_COMPANY
	*			分厂名            FIELD_SYSTEM_FACTORY
	*           机组ID            FIELD_SERVICE_PLANTID
	*           起始索引          FIELD_SERVICE_ON_SEND_STARTID
	*           测点类型          FIELD_SERVICE_CHANNTYPE
	*           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
	*           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*           报警事件ID        FIELD_SYSTEM_EVENT_ID
	*           对应的时间        FIELD_SERVICE_TIME
	*           转速              FIELD_SERVICE_REV
	*返回值的字段码：
	*           成功标识          FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_RECPROCFASTALARMDATA           24107
	*/
int CTransactDaqData::ReceiveProcFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	if (NULL==pPlantInfo)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStatus = -2;

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);
	int iType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iReturnType=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_CHANNTYPE);

	//得到启停车相关参数
	if (((iProcChannNum+iStartIndex)> pPlantInfo->_vProcChann.size())||(iProcChannNum<1)||(iStartIndex<0))
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24107 接口，数采参数不正确，FIELD_SERVICE_ON_SEND_PROCCHANNNUM:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	std::vector<float> vCharValue(iProcChannNum);

	std::vector<int> vAlarmType(iProcChannNum);
	
	CBHDateTime dtTime = CBHDateTime::GetCurrentTime();

	dtTime = pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		dtTime.SetMillisecond(iMicroSecond);
	}

	int iEventID = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	int iRev = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_REV);

	
	
	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)&vCharValue.front(),iProcChannNum*sizeof(float));

	pFixComm->GetItemBuf(FIELD_SERVICE_ALARM_TYPE,(char*)&vAlarmType.front(),iProcChannNum*sizeof(int));

	int nCount=0;
	int iloop=0;

	
	iProcChannNum = iProcChannNum + iStartIndex;

	nCount=0;

	for (iloop=iStartIndex;iloop<iProcChannNum;++iloop)
	{

		iStatus = pPlantInfo->SaveProcAlarmData(pDB, 
				                                vAlarmType[nCount], 
												iEventID,
												dtTime, 
												iMicroSecond,
												pPlantInfo->_vProcChann[iloop]->GetChannInfo()._cChannNo, 
												pPlantInfo->_vProcChann[iloop]->GetChannInfo()._iChannType, 
												vCharValue[nCount], 
												iRev);

		if(iStatus <= 0)
		{
			break;
		}
		
		nCount++;
	}


	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return nCount;

}

/**发送振动快变报警数据到中间件保存
  *输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             快变报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速   FIELD_SERVICE_REV
	*             指定机组的转速2  FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
	*             指定机组的转速4  FIELD_SERVICE_REV4
	*             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*            自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*            间隙电压          FIELD_SERVICE_TREND_GAP  
	*            成功标识      FIELD_SYSTEM_STATUS
 
	#define     SERVICE_SERVICE_ON_RECVIBFASTALARMDATA        24108 */
int CTransactDaqData::ReceiveVibFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-2;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (NULL==pPlantInfo || NULL == pRealPlantInfo)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	if (iLen < 1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("测点数目不正确:%d", iLen));
		pFixComm->Write(pFixComm->GetAci());

		return -3;
	}

	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	long  iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int   iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	CBHDateTime dtTime=CBHDateTime::GetCurrentTime();
	dtTime=pFixComm->GetTime(FIELD_SERVICE_TIME);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	if(iMicroSecond > 0)
	{
		dtTime.SetMillisecond(iMicroSecond);
	}

	//判断是否为新版本的发送助手

	if (iRealNum < 1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealNum));
		pFixComm->Write(pFixComm->GetAci());

		return -4;
	}

	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);
	S_VibCharValue *pVibValue=&loVibCharValue.front();

	try
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibValue,iLen*sizeof(S_VibCharValue));
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}
	
	//得到波形数据
	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealNum*sizeof(DATATYPE_WAVE));
    pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	//patch for precision problem
	lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

	if(lnIsPatchedForPrecision>0)
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealNum*sizeof(DATATYPE_WAVE));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
		pByteBuf = pByteBuf_precision;
	}

	//得到报警类型数组
	std::vector<unsigned int>louiType;
	louiType.resize(iLen);
	unsigned int *puiType=&louiType.front();
	memset(puiType,0,iLen*sizeof(int));
	pFixComm->GetItemBuf(FIELD_SERVICE_ALARM_TYPE, (char *)puiType, iLen*sizeof(int));
	//得到自定义参数模板ID和自定参数BUFF数据
	string sTempletID="";
	sTempletID=pFixComm->GetItem(FIELD_SYSTEM_TEMPID);

	vector<S_CustomParamData> loCustomParam;
	loCustomParam.resize(iLen);
	S_CustomParamData *pfCustomParom = &loCustomParam.front();
	memset(pfCustomParom,0,iLen*sizeof(S_CustomParamData));


	int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
	BOOL lbShouldComputCustomParam = TRUE;

	std::vector<char> loCustomBuffer(lnBufferSize);

	//2.判断是否需要自己计算自定义特征值
	if (lnBufferSize>0)
	{
		int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

		if (lnChannCountCompute == iLen)
		{		
			lbShouldComputCustomParam = FALSE;
			pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


		}else
		{
			ASSERT(FALSE);
			CString lstrLogData;
			lstrLogData.Format(_T("24108接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
				sCompany.c_str(),
				sFactory.c_str(),
				sPlant.c_str(),
				lnChannCountCompute, 
				iLen,
				__FILE__,
				__LINE__);

			BHLOG_FATAL(lstrLogData);
		}
	}

	try
	{
		if (lbShouldComputCustomParam)
		{
			//计算自定义参数信息

			pRealPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pVibValue,pByteBuf,pfCustomParom,GE_VIBCHANN,FALSE);

		}else
		{

			pRealPlantInfo->TransferCustomParamData(iStartIndex, 
				iLen,
				loCustomBuffer,
				pfCustomParom,
				GE_VIBCHANN,FALSE);

		}

	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);	
	}

	

	try
	{
		pPlantInfo->CalcVibDataGrade(pVibValue,iLen,iStartIndex);

		if(iStartIndex == 0)
		{
			iStatus=pPlantInfo->SaveVibFAlarmData(iStartIndex,iLen,pVibValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dtTime,iMicroSecond,iRealNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
			if(iStatus >= 0)
			{
				pPlantInfo->SaveVibFastDataFromTrendData(pDB,iEventID,dtTime,iMicroSecond,iStartIndex,iLen);
			}
		}
		else
		{
			iStatus = pPlantInfo->InsertVibAlarmRecordFromTrendData(pDB,iEventID,dtTime,iMicroSecond,iStartIndex,iLen);
			if(iStatus >= 0)
			{
				pPlantInfo->SaveVibFAlarmData(iStartIndex,iLen,pVibValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dtTime,iMicroSecond,iRealNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
			}
		}
		
		
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}

	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);

			if(pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_VIBCHANN)>0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
				{
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				}

			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());;
	return iStatus;
}

/**发送动态快变报警数据到中间件保存
	*输入参数字段码：
	*             公司ID              FIELD_SYSTEM_COMPANY
	*			  分厂名              FIELD_SYSTEM_FACTORY
	*             机组ID：            FIELD_SERVICE_PLANTID
	*             起始索引            FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组        FIELD_SERVICE_ALARM_TYPE
	*             动态报警事件ID      FIELD_SYSTEM_EVENT_ID
	*             特征值数据		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据            FIELD_SERVICE_ON_SEND_VIBWAVE
	*             总的波形点数        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID    FIELD_SYSTEM_TEMPID
	*             对应的时间          FIELD_SERVICE_TIME
	*             对应时间的微秒      FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	*             间隙电压          FIELD_SERVICE_TREND_GAP  

	#define     SERVICE_SERVICE_ON_RECDYNFASTALARMDATA          24109*/
int CTransactDaqData::ReceiveDynFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-2;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (NULL==pPlantInfo || NULL == pRealPlantInfo)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	BOOL bNewAssit = FALSE;

	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	if (iLen<1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("测点数目不正确:%d", iLen));
		pFixComm->Write(pFixComm->GetAci());

		return -3;
	}

	int iRealAllNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	if (iRealAllNum<128)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealAllNum));
		pFixComm->Write(pFixComm->GetAci());

		return -4;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	//S_DynCharValue *pDynCharValue=new S_DynCharValue[iLen];
	std::vector<S_DynCharValue> loDynCharValue;
	loDynCharValue.resize(iLen);
	S_DynCharValue *pDynCharValue = &loDynCharValue.front();

	try
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}
	

	long  iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int   iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	CBHDateTime dtTime=CBHDateTime::GetCurrentTime();
	dtTime=pFixComm->GetTime(FIELD_SERVICE_TIME);

	if(iMicroSecond > 0)
	{
		dtTime.SetMillisecond(iMicroSecond);
	}

	for (int i=0;i<iLen;i++)
	{
		if (!IsDynCharValid(&pDynCharValue[i]))
		{
			delete[] pDynCharValue;

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-5);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("测点(%d,%d)特征值不正确", iStartIndex, iLen));
			pFixComm->Write(pFixComm->GetAci());

			return -5;
		}
	}
	if (iRealAllNum<=0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-6);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("总波形点数不正确:%d", iRealAllNum));
		pFixComm->Write(pFixComm->GetAci());

		return -6;
	}
	//得到波形数据
	
	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLen);
	loPMax.resize(iLen);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;

	//patch for precision problem
	lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

	if(lnIsPatchedForPrecision>0)
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLen*sizeof(float));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLen*sizeof(float));	
		pByteBuf = pByteBuf_precision;

	}

	//得到报警类型数组
	std::vector<unsigned int> louiType;
	louiType.resize(iLen);
	unsigned int *puiType = &louiType.front();
	memset(puiType,0,iLen*sizeof(int));

	pFixComm->GetItemBuf(FIELD_SERVICE_ALARM_TYPE, (char *)puiType, iLen*sizeof(int));
	//得到自定义参数模板ID和自定参数BUFF数据
	string sTempletID="";
	sTempletID=pFixComm->GetItem(FIELD_SYSTEM_TEMPID);

	std::vector<S_CustomParamData> loCustomParam;
	loCustomParam.resize(iLen);
	S_CustomParamData *pfCustomParom = &loCustomParam.front();
	memset(pfCustomParom,0,iLen*sizeof(S_CustomParamData));


	int lnBufferSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
	BOOL lbShouldComputCustomParam = TRUE;

	std::vector<char> loCustomBuffer(lnBufferSize);

	//2.判断是否需要自己计算自定义特征值
	if (lnBufferSize>0)
	{
		int lnChannCountCompute = lnBufferSize/(GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

		if (lnChannCountCompute == iLen)
		{		
			lbShouldComputCustomParam = FALSE;
			pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,&loCustomBuffer.front(),lnBufferSize);				


		}else
		{
			ASSERT(FALSE);
			CString lstrLogData;
			lstrLogData.Format(_T("24109接口,自定义特征值长度不正确 %12s %12s %12s | 传送计算得到的:%4d 测点长度:%4d, %s %d\r\n"),
				sCompany.c_str(),
				sFactory.c_str(),
				sPlant.c_str(),
				lnChannCountCompute, 
				iLen,
				__FILE__,
				__LINE__);

			BHLOG_FATAL(lstrLogData);
		}
	}

	try
	{
		if (lbShouldComputCustomParam)
		{
			//计算自定义参数信息

			pRealPlantInfo->CalcCustomParamData_New(iStartIndex, iLen, pDynCharValue,pByteBuf,pfCustomParom,GE_RODSINKCHANN,FALSE);

		}else
		{

			pRealPlantInfo->TransferCustomParamData(iStartIndex, 
				iLen,
				loCustomBuffer,
				pfCustomParom,
				GE_RODSINKCHANN,FALSE);

		}

	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}

	//保存数据
	try
	{

		if(iStartIndex == 0)
		{
			iStatus=pPlantInfo->SaveDynAlarmData(iStartIndex,iLen,pDynCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dtTime,iMicroSecond,iRealAllNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);

			if(iStatus >= 0)
			{
				pPlantInfo->SaveDynFastDataFromTrendData(pDB,iEventID,dtTime,iMicroSecond,iStartIndex,iLen);
			}
		}
		else
		{
			iStatus = pPlantInfo->InsertDynAlarmRecordFromTrendData(pDB,iEventID,dtTime,iMicroSecond,iStartIndex,iLen);
			if(iStatus >= 0)
			{
				pPlantInfo->SaveDynAlarmData(iStartIndex,iLen,pDynCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dtTime,iMicroSecond,iRealAllNum,pDB,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
			}
		}
		
		
		
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		BHLOG_FATAL(lstrErrorLog);
	}
	
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iLen>0)
	{
		for (int index=0;index<iLen;++index)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);
			int lnRet = 0;

			try
			{
				lnRet = pPlantInfo->GetSpecTypeChannInfo(channInfo,index+iStartIndex,GE_RODSINKCHANN);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			if(lnRet > 0)
			{
				if (channInfo._iSensorType == GE_SENSORTYPE_VOTEX)
					pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,channInfo._fGapVoltage);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

int CTransactDaqData::ReceiveVibWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);

	int lnVibJudgedPlantStatus = pFixComm->GetLong(FIELD_SYSTEM_STATUS);


	if ( 0>= iLen)
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24115接口，数采参数不正确，FIELD_SERVICE_ON_SEND_VIBCHANNNUM:%d,FIELD_SERVICE_ON_SEND_WAVEPOINTNUM:%d,FIELD_SERVICE_SAMPLE_NUM :%d, %s,%d"),iLen,0,iSmpNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);
		zdlTraceLine(lstrLogData);

		lstrLogData.Format(_T("24115接口，数采参数不正确，sCompany:%s,sFactory:%s,sPlant :%s, %s,%d"),sCompany.c_str(),sFactory.c_str(),sPlant.c_str(), __FILE__,__LINE__);
		zdlTraceLine(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	bool bEnd=false,bNewAssit=true;
	//判断这台设备所有振动通道这次数据更新完成
	if (pFixComm->GetLong(FIELD_SERVICE_SEND_END)==1)
	{    
		bEnd=true;
	}
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		timeData.SetMillisecond(iMicroSecond);
	}

	S_VibCharValue *pVibCharValue=new S_VibCharValue[iLen];
	ZeroMemory(pVibCharValue,sizeof(S_VibCharValue)*iLen);

	float lfScaleCoef = 0;
	int lnIsPatchedForPrecision = 0;
	vector<float> loPMin(iLen,0);
	vector<float> loPMax(iLen,0);

	int lnUpdateFlag = 0;

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibCharValue,iLen*sizeof(S_VibCharValue));

	pPlantInfo->CalcVibDataGrade(pVibCharValue,iLen,iStartIndex);

	//更新实时数据的缓存
	lnUpdateFlag = pPlantInfo->UpdateVibData(NULL,pVibCharValue,0,iLen,timeData,iStartIndex,lfScaleCoef,
		lnIsPatchedForPrecision,
		loPMin,
		loPMax,bEnd);

	pPlantInfo->SetVibJudgeRunStatus(lnVibJudgedPlantStatus);
	

	int nChannNum = pFixComm->GetLong(FIELD_SERVICE_FC_CHANN_BUFFSIZE);
	vector<int> vChannNo;

	if(nChannNum > 0)
	{
		vChannNo.resize(nChannNum);
		pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),nChannNum*sizeof(int));
	}
	int iJudgeAlarm = pFixComm->GetLong(FIELD_SERVICE_JUDGE_ALARM);


	if (bEnd)
	{
		pPlantInfo->SyncVibData();
	}

	int iAlarmStatus = 0;
	int iEventID = 0;

	////更新报警数据保存处理中的数据
	if (pAlarmPlantInfo!=NULL)
	{
		pAlarmPlantInfo->UpdateVibData( NULL,
			pVibCharValue,
			0,
			iLen,
			timeData,
			iStartIndex,
			lfScaleCoef,
			lnIsPatchedForPrecision,
			loPMin,
			loPMax,
			bEnd);

		pAlarmPlantInfo->SetVibJudgeRunStatus(lnVibJudgedPlantStatus);

		if(iJudgeAlarm == 1 && bEnd)
		{
			try
			{
				pAlarmPlantInfo->SetVibFastChangeChann(vChannNo);
				pAlarmPlantInfo->JudgeVibFastAlarm(pDB,iAlarmStatus,iEventID,vChannNo);

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
	KD_DeletePoint(pVibCharValue);


	//获取快变学习参数信息,并返回到发送数据的数据采集器
	if(lnUpdateFlag < 0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
	}
	else
	{
		//获取快变学习参数信息,并返回到发送数据的数据采集器
		S_ChannInfo channInfo;
		pFixComm->CreateAnswerMul(pFixComm->GetFunc());
		if (iLen>0)
		{
			for (int index=0;index<iLen;++index)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,index+iStartIndex);

				pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
				pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,iAlarmStatus);
				if(iJudgeAlarm == 1 && bEnd)
				{
					pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,(int)vChannNo.size());
					pFixComm->SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)&vChannNo.front(),vChannNo.size()*sizeof(int));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_FC_CHANN_BUFFSIZE,0);
				}

				pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
		else
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iLen);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
	}
	return iLen;
}


/**设置无线测点状态
*输入参数：  
*         公司名         FIELD_SYSTEM_COMPANY
*         分厂名         FIELD_SYSTEM_FACTORY
*         机组名         FIELD_SERVICE_PLANTID
*         测点类型       FIELD_SERVICE_CHANNTYPE
*         起始索引       FIELD_SERVICE_ON_SEND_STARTID
*         测点个数       FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*         测点状态       FIELD_SYSTEM_WIRELESS_CHANN_STATUS
*返回值的字段码：
*         返回状态       FIELD_SYSTEM_STATUS //测点个数
*         索引           FIELD_SERVICE_ON_SEND_STARTID
#define   SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS 20046
*/
int CTransactDaqData::SetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);

	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);

	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if (NULL == pPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	int lnStartIndex = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);

	int lnChannNum  = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	CHANNTYPE lenChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	S_CHANN_STATUS * lpChannStatus = new S_CHANN_STATUS[lnChannNum];

	ZeroMemory(lpChannStatus,sizeof(S_CHANN_STATUS)*lnChannNum);

	pFixComm->GetItemBuf(FIELD_SYSTEM_WIRELESS_CHANN_STATUS,(char*)lpChannStatus,sizeof(S_CHANN_STATUS)*lnChannNum);

	int lnCount = 0;

	switch(lenChannType)
	{
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
		lnCount = pPlantInfo->UpdateVibChannStatus(lpChannStatus,lnChannNum,lnStartIndex);
		break;
	case GE_TEMPCHANN:
		lnCount = pPlantInfo->UpdateProcChannStstus(lpChannStatus,lnChannNum,lnStartIndex);
		break;
	default:
		break;
	}
	KD_DeletePoint(lpChannStatus);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());


	if (lnChannNum > 0)
	{
		for (int lnChannIndex = 0; lnChannIndex < lnChannNum; lnChannIndex++)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,lnChannIndex+lnStartIndex);

			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnCount);

			pFixComm->Write(pFixComm->GetAci());

			pFixComm->CreateNextAnswer();
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID,lnStartIndex+lnChannNum);

		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnCount);

		pFixComm->Write(pFixComm->GetAci());

		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return lnCount;
}

/**启停后处理数据，发送指定机组的启停车振动测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_VIB_DATA                         24210
	*/

int CTransactDaqData::ReceiveSudVibData( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int iLength=-1;
	bool bSudStatus=(pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS))==1?true:false;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pSudPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	
	string sTemplete;
	iLength=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNums=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);
	bool bEnd=(pFixComm->GetLong(FIELD_SERVICE_SEND_END))==1?true:false;

	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	bool bNewAssit=true;
	int iRealAllNum=0;
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;

	std::vector<S_OldVibCharValue> loOldVibCharValue;
	loOldVibCharValue.resize(iLength);

	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLength);

	S_OldVibCharValue *pOldVibValue = &loOldVibCharValue.front();
	S_VibCharValue    *pVibValue  = &loVibCharValue.front();

	CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTime();
	if (bNewAssit==false)
	{
		iRealAllNum=iSmpNum*iLength;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pOldVibValue,iLength*sizeof(S_OldVibCharValue));
		pSudPlantInfo->ChangeVibCharValueToNew(pVibValue,pOldVibValue,pfRev,iSmpNum,iSmpFreq,iRealNums,iLength,iStartIndex);
	}
	else
	{
		iRealAllNum=iRealNums;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pVibValue,iLength*sizeof(S_VibCharValue));
	}
	//DATATYPE_WAVE *pBuf=new DATATYPE_WAVE[iRealAllNum];
	std::vector<DATATYPE_WAVE> loBuf;
	loBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pBuf = &loBuf.front();

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLength);
	loPMax.resize(iLength);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;


	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

	//patch for precision problem
	lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

	if(lnIsPatchedForPrecision>0)
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLength*sizeof(float));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLength*sizeof(float));	
		pBuf = pByteBuf_precision;

	}

	
	std::vector<S_CustomParamData> loParamBuffer(iLength);
	S_CustomParamData* lpParam = &loParamBuffer[0];
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)lpParam,iLength*sizeof(S_CustomParamData));

	//	writefile(pBuf,iSmpNum*iLength,iRealNums*iLength);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
		timeData=CBHDateTime::GetCurrentTime();
	if (iRealAllNum>1)
	{
		pSudPlantInfo->SaveSudVibData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLength,pVibValue,pBuf,iRealAllNum,lpParam,sTemplete,iSmpNum,iSmpFreq,pfRev,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
	}
	else
		CHZLogManage::Info("Online:采样点数为0，不更新和保存启停车数据","CTransactDaqData","RevSudData");

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	pFixComm->Write(pFixComm->GetAci());
	return iLength;
}

/**启停后处理数据，发送指定机组的启停车动态测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             动态测点个数         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_DYN_DATA                         24211
	*/


int CTransactDaqData::ReceiveSudDynData( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int iLength=-1;
	bool bSudStatus=(pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS))==1?true:false;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pSudPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	string sTemplete;
	iLength=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	int iRealNums=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	//得到启停车相关参数
	CBHDateTime timeData=CBHDateTime::GetCurrentTime();
	timeData=pFixComm->GetTime(FIELD_SERVICE_TIME);
	int iSudType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	float lfScaleCoef = pFixComm->GetDouble(FIELD_SERVICE_WAVE_COEF);

	bool bNewAssit=true;
	int iRealAllNum=0;
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;

	std::vector<S_OldDynCharValue> loOldVibCharValue;
	loOldVibCharValue.resize(iLength);

	std::vector<S_DynCharValue> loVibCharValue;
	loVibCharValue.resize(iLength);

	S_OldDynCharValue *pOldDynValue = &loOldVibCharValue.front();
	S_DynCharValue    *pDynValue  = &loVibCharValue.front();

	CBHDateTime loUpdateTime = CBHDateTime::GetCurrentTime();
	if (bNewAssit==false)
	{
		iRealAllNum=iSmpNum*iLength;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pOldDynValue,iLength*sizeof(S_OldVibCharValue));
		pSudPlantInfo->ChangeDynCharValueToNew(pDynValue,pOldDynValue,pfRev,iSmpNum,iSmpFreq,iRealNums,iLength,iStartIndex);
	}
	else
	{
		iRealAllNum=iRealNums;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)pDynValue,iLength*sizeof(S_VibCharValue));
	}
	//DATATYPE_WAVE *pBuf=new DATATYPE_WAVE[iRealAllNum];
	std::vector<DATATYPE_WAVE> loBuf;
	loBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pBuf = &loBuf.front();

	std::vector<DATATYPE_WAVE> loByteBuf_precision;
	loByteBuf_precision.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf_precision = &loByteBuf_precision.front();
	memset(pByteBuf_precision,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	std::vector<float> loPMin;
	std::vector<float> loPMax;

	loPMin.resize(iLength);
	loPMax.resize(iLength);


	float * lfPrecisionMin = &loPMin.front();
	float * lfPrecisonMax = &loPMax.front();

	int lnIsPatchedForPrecision = 0;


	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
	//patch for precision problem
	lnIsPatchedForPrecision = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO);

	if(lnIsPatchedForPrecision>0)
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE2 ,(char *)pByteBuf_precision,iRealAllNum*sizeof(DATATYPE_WAVE));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MIN,(char*)lfPrecisionMin,iLength*sizeof(float));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ORGINAL_MAX,(char*)lfPrecisonMax,iLength*sizeof(float));	
		pBuf = pByteBuf_precision;

	}

	

	std::vector<S_CustomParamData> loParamBuffer(iLength);
	S_CustomParamData* lpParam = &loParamBuffer[0];
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)lpParam,iLength*sizeof(S_CustomParamData));

	//	writefile(pBuf,iSmpNum*iLength,iRealNums*iLength);
	if (timeData<CBHDateTime(2000,1,1,1,1,1))
		timeData=CBHDateTime::GetCurrentTime();
	if (iRealAllNum>1)
	{
		pSudPlantInfo->SaveSudDynData(pDB,iEventID,timeData,iMicroSecond,iStartIndex,iLength,pDynValue,pBuf,iRealAllNum,lpParam,sTemplete,iSmpNum,iSmpFreq,pfRev,lfScaleCoef,lnIsPatchedForPrecision,loPMin,loPMax);
	}
	else
		CHZLogManage::Info("Online:采样点数为0，不更新和保存启停车数据","CTransactDaqData","RevSudData");

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	pFixComm->Write(pFixComm->GetAci());
	return iLength;
}


/**启停后处理数据，发送指定机组的启停车过程量测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID               FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             过程量个数           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             报警事件ID           FIELD_SYSTEM_EVENT_ID
	*             报警类型             FIELD_SERVICE_SUD_TYPE
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             过程量特征值         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_PROC_DATA                        24212
	*/

int CTransactDaqData::ReceiveSudProcData( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int iLength=-1;
	bool bSudStatus=(pFixComm->GetLong(FIELD_SERVICE_SUD_STATUS))==1?true:false;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_SUD);
	if (pSudPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}


	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iProcChannNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_PROCCHANNNUM);

	float pfRev[5];

	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	//得到启停车相关参数
	if (((iProcChannNum+iStartIndex)> pSudPlantInfo->_vProcChann.size())||(iProcChannNum<1)||(iStartIndex<0))
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24212 接口，数采参数不正确，SERVICE_SERVICE_ON_SUD_PROC_DATA:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
		CHZLogManage::Error(lstrLogData);

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	vector<float> vCharValue(iProcChannNum);

	CBHDateTime dtTime = CBHDateTime::GetCurrentTime();

	dtTime = pFixComm->GetTime(FIELD_SERVICE_TIME);
	int iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		SYSTEMTIME st;

		if(dtTime.GetAsSystemTime(st))
		{
			if (st.wMilliseconds != iMicroSecond)
			{
				dtTime.SetMillisecond(iMicroSecond);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	int iEventID = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	float * lpCharVal = &vCharValue.front();

	//接受传递的实时过程量数据
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char*)lpCharVal,iProcChannNum*sizeof(float));

	int nCount=0;
	int iloop=0;

	int iStatus = pSudPlantInfo->SaveSudProcData(pDB, 
		iEventID,
		dtTime, iMicroSecond,
		lpCharVal,
		iStartIndex, iProcChannNum,
		pfRev);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, iStatus);
	pFixComm->Write(pFixComm->GetAci());

	return nCount;
}