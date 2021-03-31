#include "StdAfx.h"
#include "BHChannValueJudgeValidHelper.h"
string CBHChannValueJudgeValidHelper::g_sAbnormalDataErrorMessage = "";

 void  CBHChannValueJudgeValidHelper:: AbnormalData_SetLastError(const CString & error)
{
	g_sAbnormalDataErrorMessage = error;
}

 string CBHChannValueJudgeValidHelper:: AbnormalData_GetLastError()
{
	return g_sAbnormalDataErrorMessage;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsOverflow(double value)
{
	if (_isnan(value) || !_finite (value))
		return true;

	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsOverflow(int value)
{
	if (value < AD_MIX_INT || value > AD_MAX_INT)
		return true;

	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsUnResonable(float value)
{
	return (value < 0);
}

 bool CBHChannValueJudgeValidHelper::AbnormalData_IsUnResonable(
									   const map<int, UnResonableDataPredict> &mapCharPredict, int charIndex, float charVal)
{
	map<int, UnResonableDataPredict>::const_iterator it = mapCharPredict.find(charIndex);
	if (it == mapCharPredict.end())
		return false;

	return (*it->second)(charVal);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsAmpUnResonable(float value)
{
	return (value < 0);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsPhaseUnResonable(float value)
{
	return (value < 0 || value > 360);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsKurtosisUnResonable(float value)
{
	return (value < 0);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsSkewnessUnResonable(float value)
{
	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsgSEUnResonable(float value)
{
	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsDCUnResonable(float value)
{
	return (value < -24 || value > -2);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsGapUnResonable(float value)
{
	return (value < -24 || value > -2);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsEfficiencyUnResonable(float value)
{
	return (value < 0 || value > 100);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsPreesureRatioUnResonable(float value)
{
	return (value < 1);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsImpactUnResonable(float value)
{
	return (value < 0 || value > 16);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsTempUnResonable(float value)
{
	return (value < -273);
}

  bool  CBHChannValueJudgeValidHelper:: AbnormalData_IsAngleUnResonable(float value)
{
	return (value < 0 || value > 360);
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsCharOverflow(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsCharOverflow(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return false;
}

 bool CBHChannValueJudgeValidHelper:: AbnormalData_IsCharResonable(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return true;
}


bool CBHChannValueJudgeValidHelper::AbnormalData_IsCharResonable(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	ASSERT( NULL != pChann && NULL != pPlant);

	switch(pChann->_iChannType)
	{
	case GE_IMPACTCHANN:
		if (AbnormalData_IsImpactUnResonable(procChar)) {
			ABNORMAL_ERROR(STR("�쳣����[������] : ײ�������쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				procChar,
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return false;
		}
		break;
	case GE_TEMPCHANN:
		if (pChann->_iLocationNo == TEMP_INVAL_LOCATION_TYPE)
		{
			if (procChar > 55 || procChar < 2)
			{
				return false;
			}
		}
		else
		{
			if (procChar > 120 || procChar < 20)
			{
				return false;
			}
		}
		//if (AbnormalData_IsTempUnResonable(procChar)) {
		//	ABNORMAL_ERROR(STR("�쳣����[������] : �¶��쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
		//		procChar,
		//		pChann->_cChannID, pChann->_cChannNo,
		//		pPlant->GetCompanyName().c_str(),
		//		pPlant->GetFactoryName().c_str(),
		//		pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		//	return false;
		//}
		break;
	default:
		break;
	}

	return true;
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	S_VibCharValue * pVibChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, UnResonableDataPredict>::const_iterator it = mapCharPredict.begin();
		it != mapCharPredict.end(); ++it)
	{
		charIndex = it->first;
		//S_CharInfo &charInfo  = it->second;

		charVal = pVibChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("�쳣����[Խ��] : �쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				//charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("�쳣����[������] :�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				//charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}


	return GE_DATA_VALUE_NO_ERROR;
}


 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	S_DynCharValue * pDynChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, UnResonableDataPredict>::const_iterator it = mapCharPredict.begin();
		it != mapCharPredict.end(); ++it)
	{
		charIndex = it->first;
		//S_CharInfo &charInfo  = it->second;

		charVal = pDynChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("�쳣����[Խ��] : �쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				//charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("�쳣����[������] : �쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				//charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}

	return GE_DATA_VALUE_NO_ERROR;
}


 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	const map<int, string> & mapCharDesc,
	S_DynCharValue * pDynChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, UnResonableDataPredict>::const_iterator it = mapCharPredict.begin();
		it != mapCharPredict.end(); ++it)
	{
		charIndex = it->first;
		//S_CharInfo &charInfo  = it->second;

		string sCharName = getStringFormattedAs("����ֵ%d", charIndex);
		map<int, string>::const_iterator ir = mapCharDesc.find(charIndex);
		if(ir != mapCharDesc.end())
			sCharName = ir->second;

		charVal = pDynChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("�쳣����[Խ��] : %s�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				sCharName.c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("�쳣����[������] : %s�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
				charVal,
				sCharName.c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}


	return GE_DATA_VALUE_NO_ERROR;
}


 int  CBHChannValueJudgeValidHelper::AbnormalData_RotaryPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	ASSERT(NULL != pVibChar && NULL != pChann && NULL != pPlant);

	if(AbnormalData_IsOverflow(pVibChar->_fCharAll)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : ͨƵ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharAll,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if (AbnormalData_IsAmpUnResonable(pVibChar->_fCharAll)) {
		ABNORMAL_ERROR(STR("�쳣����[������] : ͨƵ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharAll,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharHalf)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 1/2��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if (AbnormalData_IsAmpUnResonable(pVibChar->_fCharHalf)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 1/2��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}


	if(AbnormalData_IsOverflow(pVibChar->_fCharOne)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 1��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharOne)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 1��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharTwo)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 2��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharTwo)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 2��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharThree)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 3��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharThree)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 3��Ƶ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharRes)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : �����ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharRes,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharRes)){
		ABNORMAL_ERROR(STR("�쳣����[������] : �����ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fCharRes,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fDC)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : ֱ�����쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fDC,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsDCUnResonable(pVibChar->_fDC)){
		ABNORMAL_ERROR(STR("�쳣����[������] : ֱ�����쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fDC,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fGap)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : ��϶��ѹ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fGap,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsGapUnResonable(pVibChar->_fGap)){
		ABNORMAL_ERROR(STR("�쳣����[������] : ��϶��ѹ��ֵ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fGap,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseHalf)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 1/2��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseHalf)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 1/2��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseOne)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 1��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseOne)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 1��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseTwo)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 2��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseTwo)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 2��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseThree)){
		ABNORMAL_ERROR(STR("�쳣����[Խ��] : 3��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseThree)){
		ABNORMAL_ERROR(STR("�쳣����[������] : 3��Ƶ��λ�쳣(%f),���(%s/%s) ��˾(%s) �ֳ�(%s) �豸(%s/%s)",
			pVibChar->_fPhaseThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	return GE_DATA_VALUE_NO_ERROR;
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_ReciprocalPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	//UNRESONABLE_PREDICT_INIT(mapCharPredict);
	int lnNormalData = GE_DATA_VALUE_NO_ERROR;

	float lfData = .0; 

	switch(pChann->_iInfoType)
	{
	case 100:
		/* ������е���ٶȲ��
		{100,0,"��ֵ",0,0,"m/s2"},
		{100,1,"�Ͷ�",0,0," "},
		{100,2,"���",0,0," "},
		*/
		if(pChann->_iSensorType == GE_SENSORTYPE_ACCELERATE){
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsKurtosisUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsSkewnessUnResonable);
			if (pVibChar->_fCharAll < 0)//��ֵ
			{
				lnNormalData = -1;//
				break;
			}
			//else if (pVibChar->_fCharHalf > 20)
			//{
			//	lnNormalData = -1;
			//	break;
			//}
			//else if(pVibChar->_fCharOne > 20)
			//{
			//	lnNormalData = -1;
			//	break;
			//}
		}
		break;
	case 101:
		/* ������е�ٶȲ��
		{101,0,"��Чֵ",0,0,"mm/s"},
		{101,1,"�Ͷ�",0,0," "},
		{101,2,"���",0,0," "},
		*/
		//if(pChann->_iSensorType == GE_SENSORTYPE_SPEED)
		{
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsKurtosisUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsSkewnessUnResonable);
			if (pVibChar->_fCharAll < 0)//��ֵ
			{
				lnNormalData = -1;//
				break;
			}
			//else if (pVibChar->_fCharHalf > 20)
			//{
			//	lnNormalData = -1;
			//	break;
			//}
			//else if(pVibChar->_fCharOne > 20)
			//{
			//	lnNormalData = -1;
			//	break;
			//}
		}
		break;
	default:
		break;
	}

	return lnNormalData;
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_WindElectroPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	/* �Ƿ����ò��ɲ���������ת����ж�*/
	if(pChann->_iInfoType != GE_MI_WINDPUMPCHARTYPE_ACC
		&& pChann->_iInfoType != GE_MI_WINDPUMPCHARTYPE_VEL)
	{
		return AbnormalData_RotaryPlant_IsCharValid(pVibChar, pChann, pPlant);
	}

	/*
	0,"���ٶȷ�ֵ"
	1,"���ٶȸ�Ƶ"
	2,"���ٶȵ�Ƶ"
	3,"���ٶȲ����Ͷ�"
	4,"�ٶ���Чֵ"
	5,"ͨƵgSEֵ"
	*/
	UNRESONABLE_PREDICT_INIT(mapCharPredict);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 3, AbnormalData_IsKurtosisUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 4, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 5, AbnormalData_IsgSEUnResonable);

	return AbnormalData_IsCharValid(mapCharPredict, pVibChar, pChann, pPlant);
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	const S_PlantInfo & plantInfo = pPlant->GetPlantInfo();

	if(plantInfo._iMachineType == GE_MACHINETYPE_RC || plantInfo._iMachineType == GE_MACHINETYPE_ICE || plantInfo._iMachineType == GE_MACHINETYPE_DISEL)
		return AbnormalData_ReciprocalPlant_IsCharValid(pVibChar, pChann, pPlant);
	else if (plantInfo._iMachineType == GE_MACHINETYPE_WINDPEQ)
		return AbnormalData_WindElectroPlant_IsCharValid(pVibChar, pChann, pPlant);

	return AbnormalData_RotaryPlant_IsCharValid(pVibChar, pChann, pPlant);
}
 int  CBHChannValueJudgeValidHelper::AbnormalData_IsDynStressCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	/*
	��ֵ������ѹ����>0
	��ֵ������������������
	��ת�ǣ�0~360
	*/
	map<int, string> mapCharDesc;
	mapCharDesc.insert(make_pair(0, "��ֵ������ѹ��"));
	mapCharDesc.insert(make_pair(1, "��ֵ����������"));
	mapCharDesc.insert(make_pair(2, "��ת��"));

	UNRESONABLE_PREDICT_INIT(mapCharPredict);

	UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsAngleUnResonable);

	return AbnormalData_IsCharValid(mapCharPredict, mapCharDesc, pDynChar, pChann, pPlant);
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_ReciprocalPlant_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	//UNRESONABLE_PREDICT_INIT(mapCharPredict);
	int lnNormalData = GE_DATA_VALUE_NO_ERROR;

	switch(pChann->_iInfoType)
	{
	case 111:
		/* ������̬ѹ��
		{111,0,"ƽ��ѹ��",0,0,"KPa"},
		{111,1,"��еЧ��",0,0,"%"},
		{111,2,"�����ݻ�Ч��",0,0,"%"},
		{111,3,"�����ݻ�Ч��",0,0,"%"},
		{111,4,"����ָʾ����",0,0,"KW"},
		{111,5,"ѹ��ָ��",0,0," "},
		{111,6,"����ϵ��",0,0," "},
		{111,7,"ʵ��ѹ����",0,0," "},
		{111,8,"�Ͷ�",0,0," "},
		{111,9,"���",0,0," "},
		*/
		if(pChann->_iChannType == GE_DYNPRESSCHANN){
			if (pDynChar->_fCharValue0 > 30)//��ֵ
			{
				lnNormalData = -1;//
				break;
			}
			else if (pDynChar->_fCharValue1 > 20)
			{
				lnNormalData = -1;
				break;
			}
			else if(pDynChar->_fCharValue2 > 20)
			{
				lnNormalData = -1;
				break;
			}
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 9, AbnormalData_IsSkewnessUnResonable);
		}
		break;
	case 112:
		/* ������λ�ò��
		{112,0,"������/ƫ����",0,0,"um"},
		{112,1,"��-��ֵ",0,0,"um"},
		{112,2,"��ֵ",0,0,"um"},
		{112,3,"����-����ʣ���϶",0,0,"um"},
		{112,4,"һ��Ƶ��ֵ",0,0,"um"},
		{112,5,"����Ƶ��ֵ",0,0,"um"},
		*/
		if(pChann->_iChannType == GE_RODSINKCHANN){
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 3, AbnormalData_IsUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 4, AbnormalData_IsDCUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 5, AbnormalData_IsGapUnResonable);
			if (pDynChar->_fCharValue1 < 20 || pDynChar->_fCharValue1 > 500)//��ֵ
			{
				lnNormalData = -1;//
				break;
			}
			if(pChann->_iMeassureDirection == MEASURE_DIRECTION_HORIZONTAL)
			{
				if (pDynChar->_fCharValue6 > -13 && pDynChar->_fCharValue6 > -7)
				{
					lnNormalData = -1;
				}
			}
			else
			{
				if (pDynChar->_fCharValue8 > -13 && pDynChar->_fCharValue8 > -7)
				{
					lnNormalData = -1;
				}
			}
		}
		break;
	default:
		if(pChann->_iChannType == GE_DYNSTRESSCHANN){
			return AbnormalData_IsDynStressCharValid(pDynChar, pChann, pPlant);
		}
		break;
	}

	return lnNormalData;//AbnormalData_IsCharValid(mapCharPredict, pDynChar, pChann, pPlant);
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	const S_PlantInfo & plantInfo = pPlant->GetPlantInfo();

	if(plantInfo._iMachineType == GE_MACHINETYPE_RC || plantInfo._iMachineType == GE_MACHINETYPE_ICE ||plantInfo._iMachineType == GE_MACHINETYPE_DISEL)
		return AbnormalData_ReciprocalPlant_IsCharValid(pDynChar, pChann, pPlant);

	return GE_DATA_VALUE_NO_ERROR;
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	if(AbnormalData_IsCharOverflow(procChar, pChann, pPlant))
		return GE_DATA_BOUNDS_EXCEEDED;

	if(!AbnormalData_IsCharResonable(procChar, pChann, pPlant))
		return GE_DATA_VALUE_UNRESONABLE;

	return GE_DATA_VALUE_NO_ERROR;
}

 int  CBHChannValueJudgeValidHelper::AbnormalData_IsRevValid(
	float * pfLastRev, float * pfCurrRev, int nRevNum,
	CPlantInfoBase  *  pPlant)
{
	ASSERT(pPlant != NULL);

	for(int i = 0; i < nRevNum; i++){
		if(AbnormalData_IsOverflow(pfCurrRev[i])){
			ABNORMAL_ERROR(STR("�쳣����[Խ��] : ת��%dԽ��(%f),��˾[%s]�ֳ�[%s]�豸[%s/%s]",
				i,
				pfCurrRev[i],
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));

			return GE_DATA_BOUNDS_EXCEEDED;
		}

		if(fabs(pfCurrRev[i] - pfLastRev[i]) > 1000){
			ABNORMAL_ERROR(STR("�쳣����[����] : ת��%d��������(%f-->%f),��˾[%s]�ֳ�[%s]�豸[%s/%s]",
				i,
				pfLastRev[i], pfCurrRev[i],
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));

			return GE_DATA_TREND_FLUCTUATING;
		}
	}

	return GE_DATA_VALUE_NO_ERROR;
}