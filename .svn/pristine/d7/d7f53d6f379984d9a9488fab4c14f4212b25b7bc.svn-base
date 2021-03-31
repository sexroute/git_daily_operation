#include "stdafx.h"
#include ".\plantinfoexpertsys.h"

CPlantInfoExpertSys::CPlantInfoExpertSys(void)
{
	_vVibWaveField.clear();
	_vSudWaveField.clear();
	_vDynWaveField.clear();
	_vSudCharValueField.clear();
	_vVibCharValueField.clear();
	_vDynCharValueField.clear();
	_strPlantFactName = "";
}

int CPlantInfoExpertSys::GetSectionCrankNo()
{
	return 0;
}
BOOL CPlantInfoExpertSys::GetCrossHeadChannInfo(std::vector<S_ChannInfo>& arefCrankChanninfos,int anSectionNo)
{
	int lnLoopCount = this->GetVibChannNum();

	int lnLocationIdx = 0;

	//取当前缸十字头位置索引
	for (int i =0;i<lnLoopCount;i++)
	{
		S_ChannInfo loInfo = ((CPlantInfoBase *)this)->GetSpecVibChannInfo(i);

		if (loInfo._iCylinderNo == anSectionNo 
			&& anSectionNo!= this->GetSectionCrankNo() 
			&& loInfo._iLocationNo == CROSSS_HEAD_LOCATION_TYPE)
		{
			lnLocationIdx++;
			arefCrankChanninfos.push_back(loInfo);
		}
	}

	return lnLocationIdx;
}
BOOL CPlantInfoExpertSys::GetCrankChanninfo(std::vector<S_ChannInfo>& arefCrankChanninfos)
{
	int lnLoopCount = this->GetVibChannNum();

	int lnLocationIdx = 0;

	//取当前缸十字头位置索引
	for (int i =0;i<lnLoopCount;i++)
	{
		S_ChannInfo loInfo = ((CPlantInfoBase *)this)->GetSpecVibChannInfo(i);

		if (loInfo._iCylinderNo == this->GetSectionCrankNo())
		{
			arefCrankChanninfos.push_back(loInfo);
			lnLocationIdx ++;
		}
	}

	return lnLocationIdx;
}
int CPlantInfoExpertSys::GetCrossHeadLocationIndex(int anSectionNo)
{
	int lnLoopCount = this->GetVibChannNum();

	int lnLocationIdx = -3;

	//取当前缸十字头位置索引
	for (int i =0;i<lnLoopCount;i++)
	{
		S_ChannInfo loInfo = ((CPlantInfoBase *)this)->GetSpecVibChannInfo(i);

		if (loInfo._iCylinderNo == anSectionNo 
			&& anSectionNo!= this->GetSectionCrankNo() 
			&& loInfo._iLocationNo == CROSSS_HEAD_LOCATION_TYPE)
		{
			lnLocationIdx = loInfo._iLocationIdx;
			break;
		}
	}

	return lnLocationIdx;
}

BOOL CPlantInfoExpertSys::IsValveGasIn(S_ChannInfo & aRefChann)
{
	BOOL lbValveGasIn = TRUE;

	if(aRefChann._iLocationNo == TEMP_OUTVAL_LOCATION_TYPE)
	{
		lbValveGasIn = TRUE;
	}

	return lbValveGasIn;
}

CPlantInfoExpertSys::~CPlantInfoExpertSys(void)
{
	CPlantInfoBase::Destory();
	_vVibWaveField.clear();
	_vSudWaveField.clear();
	_vDynWaveField.clear();
	_vSudCharValueField.clear();
	_vVibCharValueField.clear();
	_vDynCharValueField.clear();
}
void CPlantInfoExpertSys::AddChannInfoEnd()
{
	CPlantInfoBase::AddChannInfoEnd();
	V_PCHANNINFO::iterator it;
	V_PCHANNINFO::iterator itDyn;
	S_ChannInfo sChannInfo;
	CString strTmp;
	string sTmp,sTmp1;
	_vVibCharValueField.clear();
	_vVibWaveField.clear();
	_vSudCharValueField.clear();
	_vSudWaveField.clear();
	_vDynCharValueField.clear();
	_vDynWaveField.clear();
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		sChannInfo = (*it)->GetChannInfo();
		strTmp.Format("ALL_%s,ONE_%s,TWO_%s,THREE_%s,HALF_%s,RES_%s,DC_%s,GAP_%s,ONEPHASE_%s,TWOPHASE_%s,THREEPHASE_%s,HALFPHASE_%s",\
			sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID);
		sTmp = strTmp;
		sTmp1= "WAVE_";
		sTmp1.append(sChannInfo._cChannID);
		_vVibCharValueField.push_back( sTmp );
		_vVibWaveField.push_back( sTmp1 );
		if ( sChannInfo._iChannType == GE_VIBCHANN )
		{
			_vSudCharValueField.push_back( sTmp );
			_vSudWaveField.push_back( sTmp1 );
		}
	}
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		sChannInfo = (*itDyn)->GetChannInfo();
		strTmp.Format("ZERO_%s,ONE_%s,TWO_%s,THREE_%s,FOUR_%s,FIVE_%s,SIX_%s,SEVEN_%s,EIGHT_%s,NINE_%s",\
			sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID,sChannInfo._cChannID);
		sTmp = strTmp;
		sTmp1 = "WAVE_";
		sTmp1.append(sChannInfo._cChannID);
		_vDynCharValueField.push_back(sTmp);
		_vDynWaveField.push_back(sTmp1);
	}
	CHZLogManage::Info("finish", "ExpertSys", "CPlantInfoExpertSys::AddChannInfoEnd");
}

void CPlantInfoExpertSys::SetPlantFactName(CString strPlantFactName_)
{
	_strPlantFactName = strPlantFactName_;
}

CString CPlantInfoExpertSys::GetPlantFactName()
{
	return _strPlantFactName;
}