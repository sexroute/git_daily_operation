// S_Processor.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "S_Processor.h"
#include "IProcessor.h"
#include "DynPressComputer.h"
#include "CReciprocationAccComputer.h"
#include "LoadComputer.h"
#include "CRotationAccComputer.h"
#include "CRotationAxisDisComputer.h"
#include "CRotationVecComputer.h"
#include "CWindAndPumpVecComputer.h"
#include "CWindAndPumpAccComputer.h"
#include "ChanComputer.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CS_ProcessorApp

BEGIN_MESSAGE_MAP(CS_ProcessorApp, CWinApp)
END_MESSAGE_MAP()

#define HIGH_PASS_FC 2048


// CS_ProcessorApp 构造

CS_ProcessorApp::CS_ProcessorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CS_ProcessorApp 对象

CS_ProcessorApp theApp;


// CS_ProcessorApp 初始化

BOOL CS_ProcessorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

E_TBL_CHANNTYPE GetChannOneType(const int &iChannType_)
{
	switch(iChannType_)
	{
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		return E_TBL_CHANNTYPE_PROC;
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
	case GE_AXISLOCATIONCHANN:
		return E_TBL_CHANNTYPE_VIB;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
		return E_TBL_CHANNTYPE_DYN;
	default:
		return E_TBL_CHANNTYPE_VIB;
	}
}

/// 数据类型
enum DataType
{
	DATA_DISPLACEMENT,             ///< 位移
	DATA_ACCELERATE,               ///< 加速度
	DATA_VELOCITY,                 ///< 速度
	DATA_TEMP,                     ///< 温度
	DATA_PRESS,                    ///< 压力
	DATA_LOAD,                     ///< 应力
	DATA_FLUX,                     ///< 流量
	DATA_IMPACT                    ///< 撞击次数
}; 

/// 有无间隙电压
bool HasGapVolt(S_ChannInfo & arefChannInfo) 
{
	return (arefChannInfo._iSensorType ==GE_SENSORTYPE_VOTEX);
}
/// 是否振动通道(除轴位移通道)
bool IsVib(S_ChannInfo & arefChannInfo) 
{
	return (GetChannOneType(arefChannInfo._iChannType) == E_TBL_CHANNTYPE_VIB);
}
/// 是否轴位移通道
bool IsAxis(S_ChannInfo & arefChannInfo) 
{
	return (arefChannInfo._iChannType == GE_AXIALCHANN);
}
/// 是否动态压力通道
bool IsDynPress(S_ChannInfo & arefChannInfo) 
{
	return (arefChannInfo._iChannType == GE_DYNPRESSCHANN );
}
/// 是否活塞杆通道
bool IsRod(S_ChannInfo & arefChannInfo) 
{
	return (arefChannInfo._iChannType == GE_RODSINKCHANN);
}
/// 是否应力通道
bool IsLoad(S_ChannInfo & arefChannInfo) 
{
	return (arefChannInfo._iChannType == GE_DYNSTRESSCHANN);
}
/// 是否缓变量通道
bool IsProc(S_ChannInfo & arefChannInfo) 
{
	return !(IsVib(arefChannInfo) || IsAxis(arefChannInfo) || IsDynPress(arefChannInfo) || IsRod(arefChannInfo) || IsLoad(arefChannInfo));
}
/// 是否启停车通道
bool IsSUD(S_ChannInfo & arefChannInfo) 
{
	return IsVib(arefChannInfo)&&HasGapVolt(arefChannInfo);
}






Computation::CChanComputer * GetComputer(S_ChannInfo &chan_, float fDC_, float fGap_,PlantType aEnumPlantType, CNiReal32Vector &nivfParam_)
{
	Computation::CChanComputer *  _pComp = NULL;

	nivfParam_.SetSize(0);

	if (IsSUD(chan_))
	{
		_pComp = new Computation::CRotationRadialDisComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
		nivfParam_.Append(chan_._fSensitivity);
		nivfParam_.Append(1);
	}
	else if (IsAxis(chan_))
	{
		_pComp = new Computation::CRotationAxisDisComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
		nivfParam_.Append(chan_._fSensitivity);
		nivfParam_.Append(1);
	}
	else if (chan_._iDataType ==DATA_ACCELERATE && aEnumPlantType== MACH_ROTATION)
	{
		_pComp = new Computation::CRotationAccComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_WIND )
	{
		_pComp = new Computation::CWindAndPumpAccComputer;
		nivfParam_.Append(HIGH_PASS_FC);
	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_PUMP )
	{
		_pComp = new Computation::CWindAndPumpAccComputer;
		nivfParam_.Append(HIGH_PASS_FC);
	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_RECIPROCATION)
	{
		ASSERT(FALSE);
		/*_pComp = new CReciprocationAccComputer;*/
	}

	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType  == MACH_ROTATION)
	{
		_pComp = new Computation::CRotationVecComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType  == MACH_RECIPROCATION)
	{
		ASSERT(FALSE);
/*		_pComp = new CReciprocationVecComputer;*/
	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType == MACH_WIND)
	{
		_pComp = new Computation::CWindAndPumpVecComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType == MACH_PUMP)
	{
		_pComp = new Computation::CWindAndPumpVecComputer;
		nivfParam_.Append(fDC_);
		nivfParam_.Append(fGap_);
	}
	else if (IsDynPress(chan_))
	{
		ASSERT(FALSE);
/*
		Cylinder cld;
		_pComp = new CDynPressComputer;
		_pPlant->_plantInfo.GetCylinderInfo(chan_._iPos, cld);
		nivfParam_.Append(cld._fInPress);
		nivfParam_.Append(cld._fOutPress);
		nivfParam_.Append(cld._fClrVol);
		nivfParam_.Append(cld._fRodLen);
		nivfParam_.Append(cld._fHubRds);
		nivfParam_.Append(cld._fPisArea);
		nivfParam_.Append(cld._fTtlPower);
		nivfParam_.Append(chan_._fRDeg);
		nivfParam_.Append(chan_._fUnitRatio);*/

	}
	else if (IsRod(chan_))
	{

		ASSERT(FALSE);
/*
		Cylinder cld;
		_pComp = new CRodComputer;
		_pPlant->_plantInfo.GetCylinderInfo(chan_._iPos, cld);
		nivfParam_.Append(cld._fPisRodLen);
		nivfParam_.Append(cld._fHubRds);
		nivfParam_.Append(cld._fSensorPos);
		nivfParam_.Append(cld._fPisThick);
		nivfParam_.Append(cld._fCPG);
		nivfParam_.Append(fGap_);
		nivfParam_.Append(cld._fRodLen);
		nivfParam_.Append(chan_._fUnitRatio);*/

	}
	else if (IsLoad(chan_))
	{
		ASSERT(FALSE);
/*		_pComp = new CLoadComputer;*/
	}
	else	// 缓变量测点
	{
		CString str(chan_._cChannID);

		if (chan_._iDataType==DATA_IMPACT)		// 撞击测点
		{
			ASSERT(FALSE);
/*
			_pComp = new CImpactComputer;
			nivfParam_.Append(chan_._fSensitivity);
			nivfParam_.Append(chan_._fSensitivity1);
			nivfParam_.Append(chan_._iFilter);*/

		}
		else
		{
			ASSERT(FALSE);
			/*_pComp = new COtherComputer;*/
		}
	}

	return _pComp;
}





BOOL BHKD::IsFloatValid(float f)
{
	int lnResult = _fpclass(f);
	switch (lnResult)
	{
	case _FPCLASS_SNAN:
	case _FPCLASS_QNAN:
	case _FPCLASS_NINF:
	case _FPCLASS_ND:
	case _FPCLASS_PD:
	case _FPCLASS_PINF:
		return FALSE;
		break;
	default:
		return TRUE;
	}
}

void S_VibCharValue::SetSpecTypeData(float fData_,int iCharType_/* =GE_ALLCHAR */)
{
	if (!IsFloatValid(fData_))
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************特征值错误! %f %s %d******************"),fData_,__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		fData_ = 0;
	}
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		_fCharAll=fData_;
		break;
	case GE_HALFCHAR:
		_fCharHalf=fData_;
		break;
	case GE_ONECHAR:
		_fCharOne=fData_;
		break;
	case GE_TWOCHAR:
		_fCharTwo=fData_;
		break;
	case GE_THREECHAR:
		_fCharThree=fData_;
		break;
	case GE_DCCHAR:
		_fDC=fData_;
		break;
	case GE_GAP:
		_fGap=fData_;
		break;
	case GE_RESCHAR:
		_fCharRes=fData_;
		break;
	case GE_PHASECHARONE:
		_fPhaseOne=fData_;
		break;
	case GE_PHASECHARTWO:
		_fPhaseTwo=fData_;
		break;
	case GE_PHASECHARTHREE:
		_fPhaseThree=fData_;
		break;
	case GE_PHASECHARHALF:
		_fPhaseHalf=fData_;
		break;
	case GE_SAMPLEFREQ:
		_iSmpFreq = (int)fData_;
		break;
	case GE_SAMPLENUM:
		_iSmpNum  = (int)fData_;
		break;
	case GE_REV:
		_iRev= (int)fData_;
		break;

	}
}


BHXZ_API BOOL ComputeVibEigenValue( S_VibCharValue & arefVibCharValue,
								    S_ChannInfo & arefChannInfo,
									float afRev,
									float *  apNivfWave_, 
									int		 anNivfWaveLen_,
								    PlantType aEnumPlantType,
								    CHANNTYPE aEnumChannType)
{
	if (apNivfWave_ == NULL || anNivfWaveLen_ <=0)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	if (arefVibCharValue._iSmpFreq == 0)
	{	
	   ASSERT(FALSE);

	   return FALSE;
	}



	int lnWaveLength = anNivfWaveLen_;

	if (lnWaveLength >=arefVibCharValue._iSmpNum )
	{
		lnWaveLength = arefVibCharValue._iSmpNum ;
	}

	CNiReal32Vector nivfWave(lnWaveLength,apNivfWave_);

	CNiReal32Vector nivfFFT, nivfPar, nivfEV, nivfTmp;


	
/*
	nivfWave.Scale(arefChannInfo._fSensitivity*1, arefChannInfo._fSensitivity1);// y=k*r*x+r*b
*/

	float f0 = 0;	// 中心频率
	float fGap = 0;	// 间隙电压
	float dt = 1.0/arefVibCharValue._iSmpFreq;		// 时域分辨率
	float lfDc = .0;

	//工频
	if (aEnumPlantType == MACH_RECIPROCATION)
	{
		f0 =  CKDFilter::RpmToHz(afRev);
	}
	else
	{
		f0 =  CKDFilter::RpmToHz(afRev);
	}

	//间隙电压
	if (HasGapVolt(arefChannInfo))
	{
		fGap = arefChannInfo._fGapVoltage;
	}
	else
	{
		fGap = 0;
	}

	//直流量
	if (afRev >0)
	{
		lfDc = CKDFilter::RemoveDirectCurrent(afRev,arefVibCharValue._iSmpFreq , nivfWave, nivfTmp);

	}else
	{
		lfDc = CKDFilter::RemoveDirectCurrent(nivfWave, nivfTmp);
	}

	Computation::CChanComputer * lpComputer = GetComputer(arefChannInfo, lfDc, fGap,aEnumPlantType, nivfPar);

	if (NULL != lpComputer)
	{
		//1.预处理
		nivfFFT.Resize(lnWaveLength/2);
	
		CKDFilter::SingleSideFFT(nivfWave, nivfFFT);

		lpComputer->Compute(nivfWave, nivfFFT, dt, f0, nivfPar, nivfEV);

		if (IsVib(arefChannInfo)||IsAxis(arefChannInfo))		// 振动测点
		{

			int lnDataSize = nivfEV.GetSize();

			for (int i = 0;i< lnDataSize;i++)
			{
				float lfData = nivfEV[i];

				arefVibCharValue.SetSpecTypeData(lfData,i);
			}

			return TRUE;
		}
		else if (IsDynPress(arefChannInfo) || IsRod(arefChannInfo) || IsLoad(arefChannInfo))	// 动态测点
		{
			ASSERT(FALSE);
		}
		else	// 缓变量
		{
			ASSERT(FALSE);		
		}
	

		delete lpComputer;

		lpComputer = NULL;
		
	}

	
	return FALSE;
}

BHXZ_API float ComputeAxisEigenValue  (     
									   S_ChannInfo & arefChannInfo,
									   float  *  apNivfWave_, 
									   float afRev,
									   float afFreq,
									   int		 anNivfWaveLen_,
									   PlantType aEnumPlantType,									 
									   CHANNTYPE aEnumChannType)

{
	float f0 = 0;	// 中心频率
	float fGap = 0;	// 间隙电压
	float lfDc = .0;



	if (apNivfWave_ == NULL || anNivfWaveLen_ <=0 || afFreq <= 0)
	{
		ASSERT(FALSE);

		return 0;
	}

	CNiReal32Vector nivfWave(anNivfWaveLen_,apNivfWave_);

	CNiReal32Vector  nivfTmp;

	if (HasGapVolt(arefChannInfo))
	{
		fGap = arefChannInfo._fGapVoltage;
	}
	else
	{
		fGap = 0;
	}

	//直流量
	if (afRev >0)
	{
		lfDc = CKDFilter::RemoveDirectCurrent(afRev,afFreq , nivfWave, nivfTmp);

	}else
	{
		lfDc = CKDFilter::RemoveDirectCurrent(nivfWave, nivfTmp);
	}
	
	float fAxis = (lfDc - fGap) * arefChannInfo._fSensitivity *1;

	return fAxis;

}

BHXZ_API float ComputeFirstEigenValue     (
											S_ChannInfo & arefChannInfo,
											float  *  apNivfWave_, 
											int		 anNivfWaveLen_,
											PlantType aEnumPlantType,									 
											CHANNTYPE aEnumChannType)
{
	if (apNivfWave_ == NULL || anNivfWaveLen_ <=0)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	CNiReal32Vector nivfWav(anNivfWaveLen_,apNivfWave_);

	S_ChannInfo & chan_ = arefChannInfo;

	float ldbVal = 0;

	if (IsSUD(chan_))
	{
		ldbVal = CKDFilter::GetPeak2Peak(nivfWav);
	}
	else  if (IsAxis(chan_))
	{
	    ldbVal =	CKDFilter::GetPeak2Peak(nivfWav);
	}
	else if (chan_._iDataType ==DATA_ACCELERATE && aEnumPlantType== MACH_ROTATION)
	{
		ldbVal =	CKDFilter::GetPeak2Peak(nivfWav);
	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_WIND )
	{
		CSigMath SigMath;
		CNiReal64Vector nivftemp;
		nivftemp.SetSize(nivfWav);
		nivftemp = nivfWav;
		ldbVal =    SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);
	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_PUMP )
	{
		CSigMath SigMath;
		CNiReal64Vector nivftemp;
		nivftemp.SetSize(nivfWav);
		nivftemp = nivfWav;
		ldbVal =    SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);

	}
	else if (chan_._iDataType==DATA_ACCELERATE && aEnumPlantType == MACH_RECIPROCATION)
	{
		ASSERT(FALSE);
		/*_pComp = new CReciprocationAccComputer;*/
	}

	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType  == MACH_ROTATION)
	{
		ldbVal =  CKDFilter::GetRMS(nivfWav);
	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType  == MACH_RECIPROCATION)
	{
		ASSERT(FALSE);
/*		_pComp = new CReciprocationVecComputer;*/
	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType == MACH_WIND)
	{
		ldbVal =  CKDFilter::GetRMS(nivfWav);

	}
	else if (chan_._iDataType==DATA_VELOCITY && aEnumPlantType == MACH_PUMP)
	{
		ldbVal =  CKDFilter::GetRMS(nivfWav);
	}
	else if (IsDynPress(chan_))
	{
		ASSERT(FALSE);


	}
	else if (IsRod(chan_))
	{
		ASSERT(FALSE);
	}
	else if (IsLoad(chan_))
	{
		ASSERT(FALSE);

	}
	else	// 缓变量测点
	{
		CString str(chan_._cChannID);

		if (chan_._iDataType==DATA_IMPACT)		// 撞击测点
		{
			ASSERT(FALSE);

		}
		else
		{
			ASSERT(FALSE);
			
		}
	}

	return ldbVal;
	
}


BHXZ_API BOOL ComputeDynEigenValue( S_DynCharValue & arefVibCharValue,
								   	S_ChannInfo & arefChannInfo,
									float afRev,
									float *  apNivfWave_, 
									int		 anNivfWaveLen_,
								    PlantType aEnumPlantType,
								    CHANNTYPE aEnumChannType)
{

	if (apNivfWave_ == NULL || anNivfWaveLen_ <=0)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	CNiReal32Vector nivfWav(anNivfWaveLen_,apNivfWave_);

	CNiReal32Vector nivfFFT, nivfPar, nivfEV;

	return FALSE;

}

