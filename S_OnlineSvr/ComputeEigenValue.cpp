#include "stdafx.h"
#include "vibdata.h"
#include "additions/DebugHelper.h"
#include "ComputeEigenValue.h"
#include "../S_Processor/IProcessor.h"


bool ConvertDTWaveToDouble(double *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_)
{
	const double fPrec=10.0;
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=(pWave_[loop]/fPrec);
	return true;
}

bool ConvertDTWaveToFloat(float *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_)
{
	const float fPrec=10.0;
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=(float)(pWave_[loop]/fPrec);
	return true;
}

PlantType CovertToPlantType(MACHINETYPE aEnumMachineType)
{
	PlantType lEnumType = MACH_OTHER;

  switch (aEnumMachineType)
  {
   case 	  GE_MACHINETYPE_COMPR:      ///离心压缩机compressor
   case	      GE_MACHINETYPE_FAN:        ///风机fan
   case 	  GE_MACHINETYPE_TURB:       ///汽轮机turbine
   case       GE_MACHINETYPE_COMPR1:     ///轴流式压缩机或轴流+离心压缩机
   case       GE_MACHINETYPE_SMOKESTEAM:  ///烟汽轮机
			  lEnumType =   MACH_ROTATION;
			  break;

   case	      GE_MACHINETYPE_PUMP:       ///离心泵pump
   case		  GE_MACHINETYPE_KEYPUMP:   ///关键机泵
			  lEnumType =   MACH_PUMP;
			  break;

   case		  GE_MACHINETYPE_WINDPEQ:
			  lEnumType =   MACH_WIND;
			  break;

   case   	  GE_MACHINETYPE_RC:         ///往复式压缩机 reciprocating compressor
   case       GE_MACHINETYPE_DISEL:
   case       GE_MACHINETYPE_ICE:
			  lEnumType =   MACH_RECIPROCATION;
			  break;

   case	      GE_MACHINETYPE_OTHERS:     ///其他
			  lEnumType =   MACH_OTHER;
			  break;


  }

  return lEnumType;
}

float EigenValueComputer::ComputeFirstEigenValue     (      S_ChannInfo & arefChannInfo,
															DATATYPE_WAVE  *  apNivfWave_, 
															int		 anNivfWaveLen_,
															MACHINETYPE aEnumPlantType,									 
															CHANNTYPE aEnumChannType)
{
	if (anNivfWaveLen_<=0)
	{
		return FALSE;
	}

	std::vector<float> lofWaveDataArray(anNivfWaveLen_);

	float * lpWaveTableBuffer = &lofWaveDataArray.front();

	ConvertDTWaveToFloat(lpWaveTableBuffer,apNivfWave_,anNivfWaveLen_);

	PlantType lEnumType = CovertToPlantType(aEnumPlantType);

	float lfRet =  ::ComputeFirstEigenValue( arefChannInfo,
											lpWaveTableBuffer,
											anNivfWaveLen_,
											lEnumType,
											aEnumChannType);

	return lfRet;
}


BOOL EigenValueComputer::ComputeDynEigenValue(      S_DynCharValue & arefVibCharValue,
													S_ChannInfo & arefChannInfo,
													float afRev,
													DATATYPE_WAVE  *  apNivfWave_, 
													int		 anNivfWaveLen_,
													MACHINETYPE aEnumPlantType,
													CHANNTYPE aEnumChannType)
{




	return FALSE;
}

float EigenValueComputer::ComputeAxisEigenValue  (     
													S_ChannInfo & arefChannInfo,
													DATATYPE_WAVE  *  apNivfWave_, 
													float afRev,
													float afFreq,
													int		 anNivfWaveLen_,
													MACHINETYPE aEnumPlantType,									 
													CHANNTYPE aEnumChannType)
{
	if (anNivfWaveLen_<=0)
	{
		return FALSE;
	}

	PlantType lEnumType = CovertToPlantType(aEnumPlantType);

	std::vector<float> lofWaveDataArray(anNivfWaveLen_);

	float * lpWaveTableBuffer = &lofWaveDataArray.front();

	ConvertDTWaveToFloat(lpWaveTableBuffer,apNivfWave_,anNivfWaveLen_);

	float lfValue = ::ComputeAxisEigenValue(arefChannInfo,
										    lpWaveTableBuffer,
											afRev,
											afFreq,
											anNivfWaveLen_,
											lEnumType,
											aEnumChannType);

	return lfValue;

}


BOOL EigenValueComputer::ComputeVibEigenValue(  S_VibCharValue & arefVibCharValue,
												S_ChannInfo & arefChannInfo,
												float afRev,
												DATATYPE_WAVE  *  apNivfWave_, 
												int		 anNivfWaveLen_,
												MACHINETYPE aEnumPlantType,
												CHANNTYPE aEnumChannType)
{

	if (anNivfWaveLen_<=0)
	{
		return FALSE;
	}

	PlantType lEnumType = CovertToPlantType(aEnumPlantType);

	std::vector<float> lofWaveDataArray(anNivfWaveLen_);

	float * lpWaveTableBuffer = &lofWaveDataArray.front();

	ConvertDTWaveToFloat(lpWaveTableBuffer,apNivfWave_,anNivfWaveLen_);

	BOOL lbRet =  ::ComputeVibEigenValue(arefVibCharValue,
											arefChannInfo,
											afRev,
											lpWaveTableBuffer,
											anNivfWaveLen_,
											lEnumType,
											aEnumChannType);
	return lbRet;

}