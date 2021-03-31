#include "stdafx.h"
#include "plantinfo.h"
#include <MiddleWare.h>
#include <additions/DebugHelper.h>
#include <additions/CTimeOutReadWriteLock.h>

CPlantInfoOffLine::CPlantInfoOffLine()
{
	_vVibWaveField.clear();
	_vSudWaveField.clear();
	_vDynWaveField.clear();
	_vSudCharValueField.clear();
	_vVibCharValueField.clear();
	_vDynCharValueField.clear();
	_pLockSudData=new CTimeOutReadWriteLock();
	m_bIsSample = FALSE;
	m_oSetIsSampleTime = CBHDateTime::GetCurrentTime();
}

CPlantInfoOffLine::~CPlantInfoOffLine(void)
{
	Destory();
}

void CPlantInfoOffLine::Destory()
{
	CPlantInfoBase::Destory();
	_vVibWaveField.clear();
	_vSudWaveField.clear();
	_vDynWaveField.clear();
	_vSudCharValueField.clear();
	_vVibCharValueField.clear();
	_vDynCharValueField.clear();
	delete [] _pLockSudData;
}

void CPlantInfoOffLine::AddChannInfoEnd()
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
			sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo);
		sTmp = strTmp;
		sTmp1= "WAVE_";
		sTmp1.append(sChannInfo._cChannNo);
		sTmp1.append(",");
		sTmp1+="CUSTOM_PARAM_";
		sTmp1.append(sChannInfo._cChannNo);
		_vVibCharValueField.push_back( sTmp );
		_vVibWaveField.push_back( sTmp1 );
        if ( sChannInfo._iSensorType==GE_SENSORTYPE_VOTEX )
		{
			_vSudCharValueField.push_back( sTmp );
			_vSudWaveField.push_back( sTmp1 );
		}
	}
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		sChannInfo = (*itDyn)->GetChannInfo();
		strTmp.Format("ZERO_%s,ONE_%s,TWO_%s,THREE_%s,FOUR_%s,FIVE_%s,SIX_%s,SEVEN_%s,EIGHT_%s,NINE_%s,PERIOD_START_INDEX_%s,PERIOD_END_INDEX_%s",\
			sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo,sChannInfo._cChannNo);
		sTmp = strTmp;
		sTmp1 = "WAVE_";
		sTmp1.append(sChannInfo._cChannNo);
		sTmp1.append(",");
		sTmp1+="CUSTOM_PARAM_";
		sTmp1.append(sChannInfo._cChannNo);
		_vDynCharValueField.push_back(sTmp);
		_vDynWaveField.push_back(sTmp1);
	}
}

string CPlantInfoOffLine::GetSudChannAllField(int index_,int iNum_)
{
	string sRtn="";
	if ((index_<0)||((index_+iNum_)>_iSudChannNum)||(iNum_<1))
	{
		return sRtn;
	}
	else
	{
		sRtn.append(_vSudCharValueField[index_]);
		sRtn.append(",WAVE_");
		sRtn.append(_vSudChann[index_]->GetChannInfo()._cChannNo);
		for (int nloop=1;nloop<iNum_;++nloop)
		{
			sRtn.append(",");
			sRtn.append(_vSudCharValueField[nloop+index_]);
			sRtn.append(",WAVE_");
			sRtn.append(_vSudChann[nloop+index_]->GetChannInfo()._cChannNo);
		}
		return sRtn;
	}
}



string CPlantInfoOffLine::GetSudChannCharValueField(int index_,int iNum_)
{
	string sRtn="";
	if ((index_<0)||((index_+iNum_)>_iSudChannNum)||(iNum_<1))
	{
		return sRtn;
	}
	else
	{
		sRtn.append(_vSudCharValueField[index_]);
		for (int nloop=1;nloop<iNum_;++nloop)
		{
			sRtn.append(",");
			sRtn.append(_vSudCharValueField[nloop+index_]);
		}
		return sRtn;
	}
}


string CPlantInfoOffLine::GetSudChannSpecTypeField(int index_,int iType_)
{
	string sChannNo=_vSudChann[index_]->GetChannInfo()._cChannNo;
	return GetSudChannSpecTypeField(sChannNo,iType_);
}

string CPlantInfoOffLine::GetSudChannSpecTypeField(const string &sChannNo_,int iType_)
{
	string sType;
	switch(iType_)
	{
	case GE_ALLCHAR:
		sType="ALL_";
		break;
	case GE_ONECHAR:
		sType="ONE_";
		break;
	case GE_TWOCHAR:
		sType="TWO_";
		break;
	case GE_THREECHAR:
		sType="THREE_";
		break;
	case GE_HALFCHAR:
		sType="HALF_";
		break;
	case GE_RESCHAR:
		sType="RES_";
		break;
	case GE_PHASECHARHALF:
		sType="HALFPHASE_";
		break;
	case GE_PHASECHARONE:
		sType="ONEPHASE_";
		break;
	case GE_PHASECHARTWO:
		sType="TWOPHASE_";
		break;
	case GE_PHASECHARTHREE:
		sType="THREEPHASE_";
		break;
	case GE_GAP:
		sType="GAP_";
		break;
	case GE_DCCHAR:
		sType="DC_";
		break;
	case -1:
		sType="WAVE_";
		break;
	case -2:
		sType="TYPE_";
		break;
	case -3:
		sType="SAMPLE_FREQ_";
		break;
	case -4:
		sType="SAMPLE_NUM_";
		break;
	case -5:
		sType="REV_";
		break;
	case -6:
		sType="TEMPLETID_";
		break;
	case -7:
		sType="CUSTOM_PARAM_";
		break;
	default:
		sType="ALL_";
	}
	sType.append(sChannNo_);
	return sType;
}

string CPlantInfoOffLine::GetDynChannSpecTypeField(int index_,int iType_)
{
	string sChannNo=_vDynChann[index_]->GetChannInfo()._cChannNo;
	return GetDynChannSpecTypeField(sChannNo,iType_);
}

string CPlantInfoOffLine::GetDynChannSpecTypeField(const string &sChannNo_,int iType_)
{
	string sType;
	switch(iType_)
	{
	case 0:
		sType="ZERO_";
		break;
	case 1:
		sType="ONE_";
		break;
	case 2:
		sType="TWO_";
		break;
	case 3:
		sType="THREE_";
		break;
	case 4:
		sType="FOUR_";
		break;
	case 5:
		sType="FIVE_";
		break;
	case 6:
		sType="SIX_";
		break;
	case 7:
		sType="SEVEN_";
		break;
	case 8:
		sType="EIGHT_";
		break;
	case 9:
		sType="NINE_";
		break;
	case 10:
		sType="PERIOD_START_INDEX_";
		break;
	case 11:
		sType="PERIOD_END_INDEX_";
		break;
	case -1:
		sType="WAVE_";
		break;
	case -2:
		sType="TYPE_";
		break;
	case -3:
		sType="SAMPLE_FREQ_";
		break;
	case -4:
		sType="SAMPLE_NUM_";
		break;
	case -5:
		sType="REV_";
		break;
	case -6:
		sType="TEMPLETID_";
		break;
	case -7:
		sType="CUSTOM_PARAM_";
		break;
	default:
		sType="ZERO_";
	}
	sType.append(sChannNo_);
	return sType;
}

int CPlantInfoOffLine::SaveSudData(int iStartIndex_,int iNum_,S_VibCharValue *apVibCharValue,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_,IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=0;
	if ((iStartIndex_<0)||((iStartIndex_+iNum_)>_iSudChannNum)||(iNum_<1))
		return -1;
	
	int nloop=0;
	int iNums=iNum_+iStartIndex_;
	//得到采样频率，采样点数和转速
	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	int   pnRevFlag[5]={0,0,0,0,0};
	iSmpFreq=apVibCharValue[nCount]._iSmpFreq;
	iSmpNum=apVibCharValue[nCount]._iSmpNum;
	CString sTemp;sTemp.Format("iSmpFreq:%d,iSmpNum:%d",iSmpFreq,iSmpNum);
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=(_vSudChann[nloop])->GetChannInfo()._iRevType;

		switch(iRevType)
		{
		case 1:
			pfRev[0] = apVibCharValue[nCount]._iRev;
			pnRevFlag[0] = 1;
			break;
		case 2:
			pfRev[1] = apVibCharValue[nCount]._iRev;
			pnRevFlag[1] = 1;
			break;
		case 3:
			pfRev[2] = apVibCharValue[nCount]._iRev;
			pnRevFlag[2] = 1;
			break;
		case 4:
			pfRev[3] = apVibCharValue[nCount]._iRev;
			pnRevFlag[3] = 1;
			break;
		case 5:
			pfRev[4] = apVibCharValue[nCount]._iRev;
			pnRevFlag[4] = 1;
			break;
		default:
			pfRev[0] = apVibCharValue[nCount]._iRev;
			pnRevFlag[0] = 1;
		}
		++nCount;
	}

	if(g_bIsDiskFull && iStartIndex_ + iNum_ ==  _vSudChann.size())
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = iStartIndex_; n < iNum_; n++)
			{
				lnTotalWavePtNum += apVibCharValue[n-iStartIndex_]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < iStartIndex_; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vSudChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sSudDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vSudChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = iStartIndex_; n < iNum_; n++)
			{
				lnTotalWavePtNum += apVibCharValue[n-iStartIndex_]._iSmpNum;
			}
		}
		

		vector<CString> lvChannNo;

		for(int n = 0; n < _vSudChann.size(); n++)
		{
			lvChannNo.push_back(_vSudChann[n]->GetChannInfo()._cChannNo);
		}

		DeleteSudVibRecord(pDB_,lEventID_,lvChannNo,lnTotalWavePtNum);

	}


	S_VibCharValue vibCharValue;
	pDB_->BeginTran();
	//插入记录

	pDB_->Cmd("SELECT 1");
	pDB_->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());
	pDB_->Cmd(" WHERE  [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, lEventID_,
		gc_cMicroSecond,iMicroSecond_,
		gc_cTime,TimeConvertString(dtDataTime_));
	if (!pDB_->More())
	{

		pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sSudDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{
			pDB_->Cmd(",%s",_vSudCharValueField[nloop].c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vSudChann[nloop]->GetChannInfo()._cChannNo,-3).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vSudChann[nloop]->GetChannInfo()._cChannNo,-4).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vSudChann[nloop]->GetChannInfo()._cChannNo,-5).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vSudChann[nloop]->GetChannInfo()._cChannNo,-6).c_str());
		}
		pDB_->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",lEventID_,TimeConvertString(dtDataTime_),iMicroSecond_,pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4],iSmpFreq,iSmpNum);
		for (nloop=0;nloop<iNum_;++nloop)
		{
			vibCharValue=apVibCharValue[nloop];
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
			pDB_->Cmd(",'%s'",sTempletID_.c_str());
		}
		pDB_->Cmd(")");
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVESUD;
		}
	}
	else
	{
		string sChann;
		int index=0;
		pDB_->Cmd("UPDATE [%s] SET ",_sSudDataTblName.c_str());
		for (nloop=0;nloop<iNum_;++nloop)
		{
			vibCharValue=apVibCharValue[nloop];
			sChann=_vSudChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_ALLCHAR).c_str(),vibCharValue._fCharAll);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_ONECHAR).c_str(),vibCharValue._fCharOne);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_TWOCHAR).c_str(),vibCharValue._fCharTwo);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_THREECHAR).c_str(),vibCharValue._fCharThree);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_HALFCHAR).c_str(),vibCharValue._fCharHalf);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_RESCHAR).c_str(),vibCharValue._fCharRes);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_DCCHAR).c_str(),vibCharValue._fDC);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_GAP).c_str(),vibCharValue._fGap);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARONE).c_str(),vibCharValue._fPhaseOne);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARTWO).c_str(),vibCharValue._fPhaseTwo);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARTHREE).c_str(),vibCharValue._fPhaseThree);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARHALF).c_str(),vibCharValue._fPhaseHalf);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-3).c_str(),vibCharValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-4).c_str(),vibCharValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-5).c_str(),vibCharValue._iRev);
			pDB_->Cmd(" [%s]='%s',",GetSudChannSpecTypeField(sChann,-6).c_str(),sTempletID_.c_str());
		}
		pDB_->Cmd("[%s]=%d ",gc_cMicroSecond,iMicroSecond_);
		if(pnRevFlag[0] == 1)
		{
			pDB_->Cmd(",[%s] = %.2f",gc_cRev,pfRev[0]);
		}
		if(pnRevFlag[1] == 1)
		{
			pDB_->Cmd(",[%s] = %.2f",gc_cRev2,pfRev[1]);
		}
		if(pnRevFlag[2] == 1)
		{
			pDB_->Cmd(",[%s] = %.2f",gc_cRev3,pfRev[2]);
		}
		if(pnRevFlag[3] == 1)
		{
			pDB_->Cmd(",[%s] = %.2f",gc_cRev4,pfRev[3]);
		}
		if(pnRevFlag[4] == 1)
		{
			pDB_->Cmd(",[%s] = %.2f",gc_cRev5,pfRev[4]);
		}
		pDB_->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, lEventID_,
			gc_cMicroSecond,iMicroSecond_,
			gc_cTime,TimeConvertString(dtDataTime_));
		if (!(pDB_->Exec()))
		{
			iRtn= -3;
			goto QUIT_SAVESUD;
		}
	}
	pDB_->Cmd("SELECT 1");
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		pDB_->Cmd(",%s",_vSudWaveField[nloop].c_str());
	}
	pDB_->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, lEventID_,
		gc_cMicroSecond,iMicroSecond_,
		gc_cTime,TimeConvertString(dtDataTime_));
	if (pDB_->More(TRUE))
	{
		string sChann;
		int iRealNum = 0;
		for (nloop=0;nloop<iNum_;++nloop)
		{
			sChann=_vSudChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			
			if (iRealNum < iRealAllNum_)
			{
				CWaveHead lsHead = _vSudChann[nloop+iStartIndex_]->WaveHead();
				vector<BYTE> lvWave;
				this->WaveAddHead(&pWave_[iRealNum],apVibCharValue[nloop]._iSmpNum,lsHead,lvWave);

				//if(!(pDB_->SetBuffer(GetSudChannSpecTypeField(sChann,-1).c_str(),(BYTE *)&(pWave_[iRealNum]),(apVibCharValue[nloop]._iSmpNum)*sizeof(DATATYPE_WAVE))))
				if((pDB_->SetBuffer(GetSudChannSpecTypeField(sChann,-1).c_str(),&lvWave.front(),(int)lvWave.size())))
				{
					iRtn=-4;
					goto QUIT_SAVESUD;
				}
			}
			pDB_->SetBuffer(GetSudChannSpecTypeField(sChann,-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
			iRealNum+=apVibCharValue[nloop]._iSmpNum;
			iRtn++;
		}
	}
QUIT_SAVESUD:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	
	return iRtn;
}


int CPlantInfoOffLine::SaveVibSudData(IDBInterFace *apDB, long aEventID, CBHDateTime aDatetime, int aMicroSecond,
		int aStartIndex,int aChannNum, S_VibCharValue *apCharValue, DATATYPE_WAVE *apWave, int aWaveNum, S_CustomParamData *apParam, string aTemplate,
		int aSmpNum, int aSmpFreq, float apRev[5], int apHead[])
{
	SwitchDbNameEx(apDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	std::string lsChannNo;

	int lnVibChannNum = _vVibChann.size();

	int iRet = 0;

	if ((aStartIndex<0) || (aChannNum<1) || ((aStartIndex+aChannNum) > lnVibChannNum))
		return -1;


	

	int lnWaveOffset = 0;

	for (int i=0;i<aChannNum;++i)
	{
		if (lnWaveOffset + apCharValue[i]._iSmpNum > aWaveNum)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	int lnChannIndex = 0;
	int lnEndIndex   = aStartIndex + aChannNum;
	
	if(g_bIsDiskFull && aStartIndex + aChannNum ==  _vVibChann.size())
	{
		int lnTotalWavePtNum = 0;
		if(aStartIndex == 0)
		{
			for(int n = aStartIndex; n < aChannNum; n++)
			{
				lnTotalWavePtNum += apCharValue[n-aStartIndex]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < aStartIndex; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			apDB->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sSudDataTblName.c_str(),gc_cTime,TimeConvertString(aDatetime),gc_cEventID,aEventID);

			if(apDB->More())
			{
				for(int n = 0; n < aStartIndex; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += apDB->GetLong(lstrColumn);
				}
			}

			for(int n = aStartIndex; n < aChannNum; n++)
			{
				lnTotalWavePtNum += apCharValue[n-aStartIndex]._iSmpNum;
			}
		}
	
		vector<CString> lvChannNo;
		for(int n = 0; n < _vVibChann.size(); n++)
		{
			lvChannNo.push_back(_vVibChann[n]->GetChannInfo()._cChannNo);
		}

		DeleteSudVibRecord(apDB,aEventID,lvChannNo,lnTotalWavePtNum);

	}

	apDB->BeginTran();

	apDB->Cmd("SELECT 1");
	apDB->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());
	apDB->Cmd(" WHERE  [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, aEventID,
		gc_cMicroSecond,aMicroSecond,
		gc_cTime,TimeConvertString(aDatetime));
	if (!apDB->More())
	{

		apDB->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sSudDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);

		for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
		{
			lsChannNo = _vVibChann[lnChannIndex]->GetChannInfo()._cChannNo;

			apDB->Cmd(",%s",_vVibCharValueField[lnChannIndex].c_str());
			apDB->Cmd(",%s",GetSudChannSpecTypeField(lsChannNo,-3).c_str());
			apDB->Cmd(",%s",GetSudChannSpecTypeField(lsChannNo,-4).c_str());
			apDB->Cmd(",%s",GetSudChannSpecTypeField(lsChannNo,-5).c_str());
			apDB->Cmd(",%s",GetSudChannSpecTypeField(lsChannNo,-6).c_str());
		}

		apDB->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",aEventID,TimeConvertString(aDatetime),aMicroSecond,apRev[0],apRev[1],apRev[2],apRev[3],apRev[4],aSmpFreq,aSmpNum);
		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			const S_VibCharValue& vibCharValue = apCharValue[lnChannIndex];

			apDB->Cmd(",%.2f",vibCharValue._fCharAll);
			apDB->Cmd(",%.2f",vibCharValue._fCharOne);
			apDB->Cmd(",%.2f",vibCharValue._fCharTwo);
			apDB->Cmd(",%.2f",vibCharValue._fCharThree);
			apDB->Cmd(",%.2f",vibCharValue._fCharHalf);
			apDB->Cmd(",%.2f",vibCharValue._fCharRes);
			apDB->Cmd(",%.2f",vibCharValue._fDC);
			apDB->Cmd(",%.2f",vibCharValue._fGap);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseOne);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseTwo);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseThree);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseHalf);
			apDB->Cmd(",%d",vibCharValue._iSmpFreq);
			apDB->Cmd(",%d",vibCharValue._iSmpNum);
			apDB->Cmd(",%d",vibCharValue._iRev);
			apDB->Cmd(",'%s'",aTemplate.c_str());
		}

		apDB->Cmd(")");
		if (!(apDB->Exec()))
		{
			iRet= -2;
			goto QUIT_SAVE_SUD_VIB_DATA;
		}
	}
	else
	{
		apDB->Cmd("UPDATE [%s] SET ",_sSudDataTblName.c_str());

		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			const S_VibCharValue& vibCharValue = apCharValue[lnChannIndex];

			lsChannNo = _vVibChann[lnChannIndex+aStartIndex]->GetChannInfo()._cChannNo;

			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_ALLCHAR).c_str(),vibCharValue._fCharAll);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_ONECHAR).c_str(),vibCharValue._fCharOne);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_TWOCHAR).c_str(),vibCharValue._fCharTwo);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_THREECHAR).c_str(),vibCharValue._fCharThree);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_HALFCHAR).c_str(),vibCharValue._fCharHalf);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_RESCHAR).c_str(),vibCharValue._fCharRes);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_DCCHAR).c_str(),vibCharValue._fDC);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_GAP).c_str(),vibCharValue._fGap);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_PHASECHARONE).c_str(),vibCharValue._fPhaseOne);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_PHASECHARTWO).c_str(),vibCharValue._fPhaseTwo);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_PHASECHARTHREE).c_str(),vibCharValue._fPhaseThree);
			apDB->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(lsChannNo,GE_PHASECHARHALF).c_str(),vibCharValue._fPhaseHalf);
			apDB->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(lsChannNo,-3).c_str(),vibCharValue._iSmpFreq);
			apDB->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(lsChannNo,-4).c_str(),vibCharValue._iSmpNum);
			apDB->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(lsChannNo,-5).c_str(),vibCharValue._iRev);
			apDB->Cmd(" [%s]='%s',",GetSudChannSpecTypeField(lsChannNo,-6).c_str(),aTemplate.c_str());
		}
		apDB->Cmd("[%s]=%d ",gc_cMicroSecond,aMicroSecond);
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond, aMicroSecond,
			gc_cTime, TimeConvertString(aDatetime));
		if (!(apDB->Exec()))
		{
			iRet= -3;
			goto QUIT_SAVE_SUD_VIB_DATA;
		}
	}

	apDB->Cmd("SELECT 1");
	for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
	{
		apDB->Cmd(",%s",_vVibWaveField[lnChannIndex].c_str());
	}
	apDB->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());
	apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, aEventID,
		gc_cMicroSecond,aMicroSecond,
		gc_cTime,TimeConvertString(aDatetime));
	if (apDB->More(TRUE))
	{
		lnWaveOffset = 0;

		for (int i=0;i<aChannNum;++i)
		{
			S_ChannInfo loChannInfo = _vVibChann[aStartIndex + i]->GetChannInfo();

			lsChannNo = loChannInfo._cChannNo;

			int lnSmpNum = apCharValue[i]._iSmpNum;

			if (lnSmpNum > 0)
			{
				int lnLength = lnSmpNum;
				if(apHead)
				{
					lnLength += apHead[i]/sizeof(DATATYPE_WAVE);
				}

				if (apWave)
				{
					if(!(apDB->SetBuffer(GetSudChannSpecTypeField(lsChannNo,-1).c_str(),(BYTE*)&apWave[lnWaveOffset],lnLength*sizeof(DATATYPE_WAVE))))
					{
						iRet=-4;
						goto QUIT_SAVE_SUD_VIB_DATA;
					}
				}
				else
				{
					ASSERT(FALSE);
				}

				lnWaveOffset += lnLength;
			}

			apDB->SetBuffer(GetSudChannSpecTypeField(lsChannNo,-7).c_str(),(BYTE *)(apParam[i].pfCustomParom),sizeof(S_CustomParamData));


			iRet++;
		}
	}

QUIT_SAVE_SUD_VIB_DATA:

	if (iRet>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	
	return iRet;
}

int CPlantInfoOffLine::SaveDynSudData(IDBInterFace *apDB, long aEventID, CBHDateTime aDatetime, int aMicroSecond,
									  int aStartIndex,int aChannNum, S_DynCharValue *apCharValue, DATATYPE_WAVE *apWave, int aWaveNum, S_CustomParamData *apParam, string aTemplate,
									  int aSmpNum, int aSmpFreq, float apRev[5], int apHead[])
{
	SwitchDbNameEx(apDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	std::string lsChannNo;

	int lnDynChannNum = _vDynChann.size();

	int iRet = 0;

	if ((aStartIndex<0) || (aChannNum<1) || ((aStartIndex+aChannNum) > lnDynChannNum))
		return -1;


	int lnWaveOffset = 0;

	for (int i=0;i<aChannNum;++i)
	{
		if (lnWaveOffset + apCharValue[i]._iSmpNum > aWaveNum)
		{
			ASSERT(FALSE);
			return -1;
		}
	}


	int lnChannIndex = 0;
	int lnEndIndex   = aStartIndex + aChannNum;

	apDB->BeginTran();

	apDB->Cmd("SELECT 1");
	apDB->Cmd(" FROM [%s] ",_sSudDynDataTblName.c_str());
	apDB->Cmd(" WHERE  [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, aEventID,
		gc_cMicroSecond,aMicroSecond,
		gc_cTime,TimeConvertString(aDatetime));
	if (!apDB->More())
	{
		if(g_bIsDiskFull && aStartIndex == 0)
		{
			this->DeleteSudDynRecord(apDB,aEventID);
		}

		apDB->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sSudDynDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);

		for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
		{
			lsChannNo = _vDynChann[lnChannIndex]->GetChannInfo()._cChannNo;

			apDB->Cmd(",%s",_vDynCharValueField[lnChannIndex].c_str());
			apDB->Cmd(",%s",GetDynChannSpecTypeField(lsChannNo,-3).c_str());
			apDB->Cmd(",%s",GetDynChannSpecTypeField(lsChannNo,-4).c_str());
			apDB->Cmd(",%s",GetDynChannSpecTypeField(lsChannNo,-5).c_str());
			apDB->Cmd(",%s",GetDynChannSpecTypeField(lsChannNo,-6).c_str());
		}

		apDB->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",aEventID,TimeConvertString(aDatetime),aMicroSecond,apRev[0],apRev[1],apRev[2],apRev[3],apRev[4],aSmpFreq,aSmpNum);
		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			S_DynCharValue& dynCharValue = apCharValue[lnChannIndex];

			for (int i = 0; i < 12; i++)
				apDB->Cmd(",%.2f",dynCharValue.GetSpecTypeData(i));

			apDB->Cmd(",%d",dynCharValue._iSmpFreq);
			apDB->Cmd(",%d",dynCharValue._iSmpNum);
			apDB->Cmd(",%d",dynCharValue._iRev);
			apDB->Cmd(",'%s'",aTemplate.c_str());
		}

		apDB->Cmd(")");
		if (!(apDB->Exec()))
		{
			iRet= -2;
			goto QUIT_SAVE_DYN_SUD_DATA;
		}
	}
	else
	{
		apDB->Cmd("UPDATE [%s] SET ",_sSudDynDataTblName.c_str());

		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			S_DynCharValue& dynCharValue = apCharValue[lnChannIndex];

			lsChannNo = _vDynChann[lnChannIndex+aStartIndex]->GetChannInfo()._cChannNo;

			for (int i = 0; i < 12; i++)
				apDB->Cmd(" [%s]=%.2f,",GetDynChannSpecTypeField(lsChannNo,i).c_str(),dynCharValue.GetSpecTypeData(i));
				
			apDB->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(lsChannNo,-3).c_str(),dynCharValue._iSmpFreq);
			apDB->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(lsChannNo,-4).c_str(),dynCharValue._iSmpNum);
			apDB->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(lsChannNo,-5).c_str(),dynCharValue._iRev);
			apDB->Cmd(" [%s]='%s',",GetDynChannSpecTypeField(lsChannNo,-6).c_str(),aTemplate.c_str());
		}
		apDB->Cmd("[%s]=%d ",gc_cMicroSecond,aMicroSecond);
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond, aMicroSecond,
			gc_cTime, TimeConvertString(aDatetime));
		if (!(apDB->Exec()))
		{
			iRet= -3;
			goto QUIT_SAVE_DYN_SUD_DATA;
		}
	}

	apDB->Cmd("SELECT 1");
	for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
	{
		apDB->Cmd(",%s",_vDynWaveField[lnChannIndex].c_str());
	}
	apDB->Cmd(" FROM [%s] ",_sSudDynDataTblName.c_str());
	apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, aEventID,
		gc_cMicroSecond,aMicroSecond,
		gc_cTime,TimeConvertString(aDatetime));
	if (apDB->More(TRUE))
	{
		lnWaveOffset = 0;

		for (int i=0;i<aChannNum;++i)
		{
			lsChannNo = _vDynChann[aStartIndex + i]->GetChannInfo()._cChannNo;

			int lnSmpNum = apCharValue[i]._iSmpNum;

			if (lnSmpNum > 0)
			{
				int lnLength = lnSmpNum;
				if(apHead)
				{
					lnLength += apHead[i]/sizeof(DATATYPE_WAVE);
				}

				if(apWave)
				{
					if(!(apDB->SetBuffer(GetDynChannSpecTypeField(lsChannNo,-1).c_str(),(BYTE *)&apWave[lnWaveOffset],lnLength*sizeof(DATATYPE_WAVE))))
					{
						iRet=-4;
						goto QUIT_SAVE_DYN_SUD_DATA;
					}

					lnWaveOffset += lnLength;
				}
				
			}

			apDB->SetBuffer(GetDynChannSpecTypeField(lsChannNo,-7).c_str(),(BYTE *)(apParam[i].pfCustomParom),sizeof(S_CustomParamData));	

			iRet++;
		}
	}

QUIT_SAVE_DYN_SUD_DATA:

	if (iRet>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	return iRet;
}


int CPlantInfoOffLine::SaveProcSudData(
	IDBInterFace *  apDB,
	int             aEventID,
	CBHDateTime     aDateTime,
	int             aMicrosecond,
	float*          aCharVal,
	int             aStartIndex,
	int             aChannNum,
	float apRev[5])
{
	ASSERT(apDB);
	ASSERT(!_sCompanyAlias.empty());


	int lnProcChannNum = _vProcChann.size();

	ASSERT(aStartIndex >= 0 && aChannNum > 0 && (aStartIndex + aChannNum) <= lnProcChannNum);

	if(aStartIndex < 0 || aChannNum < 1)
	{
		BHLOG_ERROR(STR("StartIndex或ChannNum不正确: (%d,%d)", aStartIndex, aChannNum));
		return -3;
	}

	if((aStartIndex + aChannNum) > lnProcChannNum)
	{
		BHLOG_ERROR(STR("StartIndex或ChannNum不正确: (%d,%d)", aStartIndex, aChannNum));
		return -4;
	}


	CString lsDataDbName;
	lsDataDbName.Format("%s%s", _sCompanyAlias.c_str(), _sPlantInfo._cPlantNo);

	SwitchDbNameEx(apDB, lsDataDbName);

	int iRtn = 1;

	CString lsCondition;
	lsCondition.Format("[%s]=%d AND [%s]='%s' AND [%s]=%d ",
		gc_cEventID, aEventID,
		gc_cTime, TimeConvertString(aDateTime),
		gc_cMicroSecond, aMicrosecond);

	apDB->BeginTran();

	do {
		
		apDB->Cmd("select 1 from [%s] where %s", _sSudProcDataTblName.c_str(), lsCondition);
		if (!apDB->More())
		{
			CString lsColNames;
			CString lsColVals;


			if(g_bIsDiskFull)
			{
				this->DeleteSudProcRecord(apDB,aEventID);
			}

			for (int iChann = 0; iChann < aChannNum; iChann++)
			{
				S_ChannInfo channInfo = this->_vProcChann[aStartIndex + iChann]->GetChannInfo();

				if (lsColNames.IsEmpty())
				{
					lsColNames.Format("[%s]", channInfo._cChannNo);
				}
				else
				{
					lsColNames.AppendFormat(",[%s]", channInfo._cChannNo);
				}

				if (lsColVals.IsEmpty())
				{
					lsColVals.Format("%.2f", aCharVal[iChann]);
				}
				else
				{
					lsColVals.AppendFormat(",%.2f", aCharVal[iChann]);
				}
			}

			apDB->Cmd("insert into [%s]", _sSudProcDataTblName.c_str());
			apDB->Cmd("(%s,%s,%s,%s, %s, %s,%s,%s,%s,%s)",
				gc_cEventID, gc_cDateTime, gc_cMicroSecond, gc_cSynFlag, 
				lsColNames, 
				gc_cRev, gc_cRev2, gc_cRev3, gc_cRev4, gc_cRev5); 
			apDB->Cmd("values(%d,'%s',%d,%d, %s, %d,%d,%d,%d,%d)",
				aEventID,TimeConvertString(aDateTime), aMicrosecond, 0, 
				lsColVals,
				apRev[0], apRev[1],apRev[2],apRev[3],apRev[4]);
			if (!(apDB->Exec()))
			{
				iRtn= -2;
				break;
			}
		}
		else
		{
			apDB->Cmd("UPDATE [%s] SET ", _sSudProcDataTblName.c_str());

			for (int iChann = 0; iChann < aChannNum; iChann++)
			{
				S_ChannInfo channInfo = this->_vProcChann[aStartIndex + iChann]->GetChannInfo();

				apDB->Cmd(" [%s]=%.2f,",channInfo._cChannNo, aCharVal[iChann]);
			}

			apDB->Cmd(" [%s]=%.2f, ",gc_cRev,apRev[0]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev2,apRev[1]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev3,apRev[2]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev4,apRev[3]);
			apDB->Cmd(" [%s]=%.2f ",gc_cRev5,apRev[4]);
			apDB->Cmd(" WHERE %s", lsCondition);
			if (!(apDB->Exec()))
			{
				iRtn = -2;
				break;
			}
		}
	} while(0);


	if (iRtn>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	return iRtn;
}


int CPlantInfoOffLine::SaveVibFAlarmData(
	IDBInterFace *pDB_,
	int iStartIndex_,int iNum_,S_VibCharValue *pVibCharValue_,unsigned int *piType_,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_, int apHead[])
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	if ((iStartIndex_<0)||((iStartIndex_+iNum_)> _sPlantInfo._iVibChannNum)||(iNum_<1))
		return iRtn;
	
	int nloop=0;
	int iNums=iNum_+iStartIndex_;

	//得到采样频率，采样点数和转速
    int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	int   pnRevUpdate[5]={0,0,0,0,0};

	iSmpFreq=pVibCharValue_[nCount]._iSmpFreq;
	iSmpNum=pVibCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=_vVibChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[0] = 1;
			break;
		case 2:
			pfRev[1] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[1] = 1;
			break;
		case 3:
			pfRev[2] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[2] = 1;
			break;
		case 4:
			pfRev[3] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[3] = 1;
			break;
		case 5:
			pfRev[4] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[4] = 1;
			break;
		default:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
			pnRevUpdate[0] = 1;
		}
		++nCount;
	}

	if(g_bIsDiskFull && iStartIndex_ + iNum_ == _sPlantInfo._iVibChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iNum_; n++)
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

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sAlarmVibDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pVibCharValue_[n]._iSmpNum;
			}
		}

		if(!DeleteVibTrendRecord(pDB_,lnTotalWavePtNum))
		{
			DeleteVibAlarmRecord(pDB_,lEventID_,lnTotalWavePtNum);
		}
	}


	S_VibCharValue vibCharValue;
	CString strSql;//构造SQL语句。
	//构造查询这条报警数据记录是否已经存在
	strSql.Format("select 1 from [%s] where [%s]='%s' AND [%s]=%d AND [%s]=%d ",_sAlarmVibDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
	pDB_->BeginTran();
	pDB_->Cmd(strSql);
	//插入记录
	if (!pDB_->More())
	{

		pDB_->ClearSql();
		pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sAlarmVibDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{
			pDB_->Cmd(",%s",_vVibCharValueField[nloop].c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vVibChann[nloop]->GetChannInfo()._cChannNo,-2).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vVibChann[nloop]->GetChannInfo()._cChannNo,-3).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vVibChann[nloop]->GetChannInfo()._cChannNo,-4).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vVibChann[nloop]->GetChannInfo()._cChannNo,-5).c_str());
			pDB_->Cmd(",%s",GetSudChannSpecTypeField(_vVibChann[nloop]->GetChannInfo()._cChannNo,-6).c_str());
		}
		pDB_->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",lEventID_,TimeConvertString(dtDataTime_),iMicroSecond_,pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4],iSmpFreq,iSmpNum);
		for (nloop=0;nloop<iNum_;++nloop)
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
			pDB_->Cmd(",%d",piType_[nloop]);
			pDB_->Cmd(",%d",vibCharValue._iSmpFreq);
			pDB_->Cmd(",%d",vibCharValue._iSmpNum);
			pDB_->Cmd(",%d",vibCharValue._iRev);
			pDB_->Cmd(",'%s'",sTempletID_.c_str());
		}
		pDB_->Cmd(")");
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	else
	{
		string sChann;
		int index=0;
		pDB_->Cmd("UPDATE [%s] SET ",_sAlarmVibDataTblName.c_str());
		for (nloop=0;nloop<iNum_;++nloop)
		{
			vibCharValue=pVibCharValue_[nloop];
			sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_ALLCHAR).c_str(),vibCharValue._fCharAll);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_ONECHAR).c_str(),vibCharValue._fCharOne);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_TWOCHAR).c_str(),vibCharValue._fCharTwo);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_THREECHAR).c_str(),vibCharValue._fCharThree);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_HALFCHAR).c_str(),vibCharValue._fCharHalf);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_RESCHAR).c_str(),vibCharValue._fCharRes);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_DCCHAR).c_str(),vibCharValue._fDC);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_GAP).c_str(),vibCharValue._fGap);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARONE).c_str(),vibCharValue._fPhaseOne);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARTWO).c_str(),vibCharValue._fPhaseTwo);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARTHREE).c_str(),vibCharValue._fPhaseThree);
			pDB_->Cmd(" [%s]=%.2f,",GetSudChannSpecTypeField(sChann,GE_PHASECHARHALF).c_str(),vibCharValue._fPhaseHalf);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-2).c_str(),piType_[nloop]);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-3).c_str(),vibCharValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-4).c_str(),vibCharValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",GetSudChannSpecTypeField(sChann,-5).c_str(),vibCharValue._iRev);
			pDB_->Cmd(" [%s]='%s',",GetSudChannSpecTypeField(sChann,-6).c_str(),sTempletID_.c_str());
		}
		if(pnRevUpdate[0] == 1)
		{
			pDB_->Cmd("[%s]=%.2f, ",gc_cRev,pfRev[0]);
		}
		if(pnRevUpdate[1] == 1)
		{
			pDB_->Cmd("[%s]=%.2f,  ",gc_cRev2,pfRev[1]);
		}
		if(pnRevUpdate[2] == 1)
		{
			pDB_->Cmd("[%s]=%.2f, ",gc_cRev3,pfRev[2]);
		}
		if(pnRevUpdate[3] ==1)
		{
			pDB_->Cmd("[%s]=%.2f, ",gc_cRev4,pfRev[3]);
		}
		if(pnRevUpdate[4] == 1)
		{
			pDB_->Cmd("[%s]=%.2f, ",gc_cRev5,pfRev[4]);
		}
		pDB_->Cmd("[%s]=%d ",gc_cMicroSecond,iMicroSecond_);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]=%d AND [%s]=%d ",gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	pDB_->Cmd("SELECT 1");
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
		pDB_->Cmd(",%s",_vVibWaveField[nloop].c_str());
	pDB_->Cmd(" FROM [%s] ",_sAlarmVibDataTblName.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);
	if (pDB_->More(TRUE))
	{
		string sChann;
		int iRealNum=0;
		for (nloop=0;nloop<iNum_;++nloop)
		{
			S_ChannInfo loChannInfo = _vVibChann[nloop+iStartIndex_]->GetChannInfo();
			sChann = loChannInfo._cChannNo;

			S_VibCharValue * lpCharVal = pVibCharValue_+nloop;

			if (!lpCharVal)
			{
				ASSERT(FALSE);
				continue;
			}

			if (pWave_ && lpCharVal->_iSmpNum > 0)
			{
				int lnLength = lpCharVal->_iSmpNum;
				if(apHead)
				{
					lnLength += apHead[nloop]/sizeof(DATATYPE_WAVE);
				}

				if(!(pDB_->SetBuffer(GetSudChannSpecTypeField(sChann,-1).c_str(),(BYTE *)&pWave_[iRealNum],lnLength*sizeof(DATATYPE_WAVE))))
				{
					BHLOG_ERROR(STR("时间:%s同步振动报警数据，保存%s波形失败",dtDataTime_.Format(),GetSudChannSpecTypeField(sChann,-1).c_str()));
				}

				iRealNum+=lnLength;
			}

			pDB_->SetBuffer(GetSudChannSpecTypeField(sChann,-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));


			iRtn=nloop+1;
		}
	}
QUIT_SAVEFALARM:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	
	return iRtn;
}

int CPlantInfoOffLine::SaveDynAlarmData(IDBInterFace *pDB_,int iStartIndex_,int iNum_,S_DynCharValue *pDynCharValue_,unsigned int *piType_,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_,int apHead[])
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn = -1;
	if ((iStartIndex_<0)||((iStartIndex_+iNum_)> _sPlantInfo._iDynChannNum)||(iNum_<1))
		return iRtn;

	int nloop=0,nloop1;
	int iNums=iNum_+iStartIndex_;
	//得到采样频率，采样点数和转速

	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	iSmpFreq=pDynCharValue_[nCount]._iSmpFreq;
	iSmpNum=pDynCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=(_vDynChann[nloop])->GetChannInfo()._iRevType;
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
		++nCount;
	}

	if(g_bIsDiskFull && iStartIndex_ + iNum_ == _sPlantInfo._iDynChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iNum_; n++)
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

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sAlarmDynDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vDynChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pDynCharValue_[n]._iSmpNum;
			}
		}

		if(!DeleteDynTrendRecord(pDB_,lnTotalWavePtNum))
		{
			DeleteDynAlarmRecord(pDB_,lEventID_,lnTotalWavePtNum);
		}
	}


	S_DynCharValue dynCharValue;
	CString strSql;//构造SQL语句。
	//构造查询这条报警数据记录是否已经存在
	strSql.Format("select 1 from [%s] where [%s]='%s' AND [%s]=%d AND [%s]=%d ",_sAlarmDynDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
	pDB_->BeginTran();
	pDB_->Cmd(strSql);
	//插入记录
	if (!pDB_->More())
	{

		pDB_->ClearSql();
		pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sAlarmDynDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{
			pDB_->Cmd(",%s",_vDynCharValueField[nloop].c_str());
			pDB_->Cmd(",%s",GetDynChannSpecTypeField(_vDynChann[nloop]->GetChannInfo()._cChannNo,-2).c_str());
			pDB_->Cmd(",%s",GetDynChannSpecTypeField(_vDynChann[nloop]->GetChannInfo()._cChannNo,-3).c_str());
			pDB_->Cmd(",%s",GetDynChannSpecTypeField(_vDynChann[nloop]->GetChannInfo()._cChannNo,-4).c_str());
			pDB_->Cmd(",%s",GetDynChannSpecTypeField(_vDynChann[nloop]->GetChannInfo()._cChannNo,-5).c_str());
			pDB_->Cmd(",%s",GetDynChannSpecTypeField(_vDynChann[nloop]->GetChannInfo()._cChannNo,-6).c_str());

		}
		pDB_->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",lEventID_,TimeConvertString(dtDataTime_),iMicroSecond_,pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4],iSmpFreq,iSmpNum);

		for (nloop=0;nloop<iNum_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			for (nloop1 = 0; nloop1 < 12; nloop1++)
				pDB_->Cmd(",%.2f",dynCharValue.GetSpecTypeData(nloop1));
			pDB_->Cmd(",%d",piType_[nloop]);
			pDB_->Cmd(",%d",dynCharValue._iSmpFreq);
			pDB_->Cmd(",%d",dynCharValue._iSmpNum);
			pDB_->Cmd(",%d",dynCharValue._iRev);
			pDB_->Cmd(",'%s'",sTempletID_.c_str());
		}
		pDB_->Cmd(")");
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	else
	{
		string sChann;
		int index=0;
		pDB_->Cmd("UPDATE [%s] SET ",_sAlarmDynDataTblName.c_str());
		for (nloop=0;nloop<iNum_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			for (nloop1 = 0; nloop1 < 12; nloop1++)
				pDB_->Cmd(" [%s]=%.2f,",GetDynChannSpecTypeField(sChann,nloop1).c_str(),dynCharValue.GetSpecTypeData(nloop1));
			pDB_->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(sChann,-2).c_str(),piType_[nloop]);
			pDB_->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(sChann,-3).c_str(),dynCharValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(sChann,-4).c_str(),dynCharValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",GetDynChannSpecTypeField(sChann,-5).c_str(),dynCharValue._iRev);
			pDB_->Cmd(" [%s]='%s',",GetDynChannSpecTypeField(sChann,-6).c_str(),sTempletID_.c_str());
		}
		pDB_->Cmd("[%s]=%d ",gc_cMicroSecond,iMicroSecond_);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]=%d AND [%s]=%d ",gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	pDB_->Cmd("SELECT 1");
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
		pDB_->Cmd(",%s",_vDynWaveField[nloop].c_str());
	pDB_->Cmd(" FROM [%s] ",_sAlarmDynDataTblName.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);
	if (pDB_->More(TRUE))
	{
		string sChann;
		int iRealNum =0;
		for (nloop=0;nloop<iNum_;++nloop)
		{
			sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			
			int lnSmpNum = pDynCharValue_[nloop]._iSmpNum;

			if (lnSmpNum > 0)
			{
				int lnLength = lnSmpNum;
				if(apHead)
				{
					lnLength += apHead[nloop]/sizeof(DATATYPE_WAVE);
				}

				if(pWave_)
				{
					if(!(pDB_->SetBuffer(GetDynChannSpecTypeField(sChann,-1).c_str(),(BYTE *)&pWave_[iRealNum],lnLength*sizeof(DATATYPE_WAVE))))
					{
						BHLOG_ERROR(STR("同步动态报警数据，保存波形失败"));
					}

					iRealNum += lnLength;
				}

			}

			if(!pDB_->SetBuffer(GetDynChannSpecTypeField(sChann,-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom), sizeof(S_CustomParamData)))
			{
				BHLOG_ERROR(STR("同步动态报警数据，保存自定义buff数据失败"));
			}
			iRtn=nloop+1;
		}
	}
QUIT_SAVEFALARM:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	
	return iRtn;
}



int CPlantInfoOffLine::SaveProcAlarmData(
	IDBInterFace *  pDB_,
	int             AlarmType_,
	int             EventID_,
	CBHDateTime    DateTime_,
	int            iMicroSecond_,
	CString         ChannNo_,
	int             ChannType_,
	float           ChannValue_,
	int             Rev_
	)
{
	ASSERT(pDB_);
	ASSERT(!_sCompanyAlias.empty());
	
	CString lsDataDbName;
	lsDataDbName.Format("%s%s", _sCompanyAlias.c_str(), _sPlantInfo._cPlantNo);

	SwitchDbNameEx(pDB_, lsDataDbName);


	int iRtn = 1;

	// 查询这条报警数据记录是否已经存在: 不存在时Insert, 存在时Update

	CString lsCondition;
	lsCondition.Format("[%s]=%d AND [%s]='%s' AND [%s]='%s' and [%s] = %d ",
		gc_cEventID, EventID_,
		gc_cChannNo, ChannNo_,
		gc_cTime, TimeConvertString(DateTime_),
		gc_cMicroSecond,iMicroSecond_);

	pDB_->BeginTran();

	do {
		pDB_->Cmd("select 1 from [%s] where %s", _sAlarmProcDataTblName.c_str(), lsCondition);
		if (!pDB_->More())
		{
			if(g_bIsDiskFull)
			{
				if(!this->DeleteProcTrendRecord(pDB_))
				{
					this->DeleteProcAlarmRecord(pDB_,EventID_);
				}
			}

			pDB_->ClearSql();

			pDB_->Cmd("insert into [%s]", _sAlarmProcDataTblName.c_str());
			pDB_->Cmd("(%s,%s,%s,%s,%s,[%s],[%s],%s,[%s])",
				gc_cEventID, gc_cChannNo, gc_cChannType, gc_cProcValue, gc_cRev, gc_cType, gc_cDateTime, gc_cSynFlag,gc_cMicroSecond); 
			pDB_->Cmd("values(%d,'%s',%d,%.2f,%d,%d,'%s',%d,%d)",
				EventID_, ChannNo_, ChannType_, ChannValue_, Rev_, AlarmType_, TimeConvertString(DateTime_), 0,iMicroSecond_);
			if (!(pDB_->Exec()))
			{
				iRtn= -2;
				break;
			}
		}
		else
		{
			pDB_->Cmd("UPDATE [%s] SET", _sAlarmProcDataTblName.c_str());
			pDB_->Cmd("%s=%d", gc_cChannType, ChannType_);
			pDB_->Cmd(",%s=%.2f", gc_cProcValue, ChannValue_);
			pDB_->Cmd(",%s=%d", gc_cRev, Rev_);
			pDB_->Cmd(",[%s]=%d", gc_cType, AlarmType_);
			pDB_->Cmd(",%s=%d", gc_cSynFlag, 0);
			pDB_->Cmd(" WHERE %s", lsCondition);
			if (!(pDB_->Exec()))
			{
				iRtn = -2;
				break;
			}
		}
	} while(0);


	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	return iRtn;
}


int CPlantInfoOffLine::UpdateVibAlarmStatus(int iChannIndex_,int iEventID_,CBHDateTime dtEnd_,int iNewThID_,IDBInterFace * pDB_)
{

	/*SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	string sChannID=GetSpecVibChannInfo(iChannIndex_)._cChannID;
	pDB_->Cmd("update [%s] set ",_sAlarmLogTblName.c_str());
	pDB_->Cmd(" [%s]='%s',[%s]=%d ",gc_cEnd_T,TimeConvertString(dtEnd_),gc_cThreshold_ID_New,iNewThID_);
	pDB_->Cmd(" where [%s]=%d and [%s]='%s'",gc_cEventID,iEventID_,gc_cName,sChannID.c_str());
	if (pDB_->Exec())
	  return 1;
	else
	  return -2;*/

	//开启了报警判断，不处理报警日志, 将报警同步标志复位，以便重新同步
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("update [%s] set ",_sAlarmLogTblName.c_str());
	pDB_->Cmd(" [%s]=%d",gc_cSynFlag,0);
	pDB_->Cmd(" where [%s]=%d and [%s] = %d",
		gc_cEventID,iEventID_,gc_cStop_Flag,1);
	if(!pDB_->Exec())
	{
		return -2;
	}

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();
	if (loTimeSpan.GetTotalHours() > GetAlarmOverridTime())
	{
		pDB_->Cmd("update [%s] set ",_sAlarmLogTblName.c_str());
		pDB_->Cmd(" [%s]='%s',[%s]=%d,[%s]=%d ",gc_cEnd_T,TimeConvertString(dtEnd_),\
			gc_cThreshold_ID_New,iNewThID_,gc_cStop_Flag,1);
		pDB_->Cmd(" where [%s]=%d and [%s]=%d",gc_cEventID,iEventID_,gc_cStop_Flag,0);
		return (pDB_->Exec()?1:-2);
	}

	return 1;
	
	
}
int CPlantInfoOffLine::AddAlarmLog(string sChann_,int iEventID_,int iLogID_,CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,unsigned int uiAlarmType_,int iThID_,int iThIDNew_,IDBInterFace *pDB_,int iAlarmLevel,int iStopFlag)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	//先查询是否存在,如果存在就更新，不存在就插入
    pDB_->Cmd(" select * from [%s] ",_sAlarmLogTblName.c_str());
	pDB_->Cmd(" where [%s]=%d and [%s]='%s' and [%s]='%s'",
		gc_cEventID,iEventID_,
		gc_cName,sChann_.c_str(),
		gc_cStart_T, TimeConvertString(dtStart_));
	if(pDB_->More(TRUE))
	{
		iRtn = pDB_->GetLong(gc_cEventID);

		try
		{
			pDB_->Cmd("update [%s] set [%s] = %d,[%s] = %d,[%s] = '%s',[%s] = %d",_sAlarmLogTblName.c_str(),gc_cType,uiAlarmType_,gc_cChannType,iChannType_,gc_cEnd_T,dtEnd_.Format(),gc_cStop_Flag,iStopFlag);
			pDB_->Cmd(" where [%s]=%d and [%s]='%s' and [%s]='%s'",
				gc_cEventID,iEventID_,
				gc_cName,sChann_.c_str(),
				gc_cStart_T, TimeConvertString(dtStart_));
			pDB_->Exec();
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
		bool lbInsertAlarmLevel = false;

		pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmLogTblName.c_str(),gc_cAlarmLevel);
		if(!pDB_->More())
		{
			pDB_->Cmd("ALTER TABLE %s ADD %s INT NULL ",_sAlarmLogTblName.c_str(),gc_cAlarmLevel);
			if(pDB_->Exec())
			{
				lbInsertAlarmLevel = true;
			}
		}
		else
		{
			lbInsertAlarmLevel = true;
		}

		if(lbInsertAlarmLevel)
		{
			pDB_->Cmd(" insert into [%s](",_sAlarmLogTblName.c_str());
			pDB_->Cmd("[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) ",gc_cID,gc_cEventID,gc_cName,gc_cChannType,gc_cType,gc_cStart_T,gc_cEnd_T,gc_cThreshold_ID,gc_cThreshold_ID_New,gc_cSynFlag,gc_cStop_Flag,gc_cAlarmLevel);
			pDB_->Cmd(" values(%d,%d,'%s',%d,%d,'%s','%s',%d,%d,0,%d,%d) ",iLogID_,iEventID_,sChann_.c_str(),iChannType_,uiAlarmType_,TimeConvertString(dtStart_),TimeConvertString(dtEnd_),iThID_,iThIDNew_,iStopFlag,iAlarmLevel);

		}
		else
		{
			pDB_->Cmd(" insert into [%s](",_sAlarmLogTblName.c_str());
			pDB_->Cmd("[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) ",gc_cID,gc_cEventID,gc_cName,gc_cChannType,gc_cType,gc_cStart_T,gc_cEnd_T,gc_cThreshold_ID,gc_cThreshold_ID_New,gc_cSynFlag,gc_cStop_Flag);
			pDB_->Cmd(" values(%d,%d,'%s',%d,%d,'%s','%s',%d,%d,0,%d) ",iLogID_,iEventID_,sChann_.c_str(),iChannType_,uiAlarmType_,TimeConvertString(dtStart_),TimeConvertString(dtEnd_),iThID_,iThIDNew_,iStopFlag);
		}
		if (pDB_->Exec())
			iRtn=iEventID_;
	}
	return iRtn;
}

int CPlantInfoOffLine::CalcSpecLocationValue(string sLocation_,float fGapVer_,float fDCVer_,float fGapHor_,float fDCHor_,float &fX_,float &fY_)
{
	int index=GetSpecAxisLocationIndex(sLocation_);
	if (index<0)
		return -1;
	CAxisChannInfo axis;
	_vAxisChann[index]->GetChannInfo(axis);

	int lnHorChannIndex = GetSpecVibChannIndexNo(axis._cHorChann);

	if (lnHorChannIndex<0)
	{
		return -1;
	}
	int lnVerChannIndex = GetSpecVibChannIndexNo(axis._cVerChann);


	if (lnVerChannIndex<0)
	{
		return -1;
	}

	float fSensitivityHor=_vVibChann[lnHorChannIndex]->GetChannInfo()._fSensitivity;
	float fSensitivityVer=_vVibChann[lnVerChannIndex]->GetChannInfo()._fSensitivity;
    S_AxisData axisData;
	axisData._fHorDC=fDCHor_/10.0;
	axisData._fHorGap=fGapHor_;
	axisData._fVerDC=fDCVer_/10.0;
	axisData._fVerGap=fGapVer_;
	if(axisData.CalcAxisLocation(axis._iLocationType,axis._iRotateDirection,axis._fAxisDia,fSensitivityVer,fSensitivityHor))
	{
		fX_=axisData._fCoorX;
		fY_=axisData._fCoorY;
		return 1;
	}
	else
		return -1;
}

int CPlantInfoOffLine::CalcSpecLocationValue(string sChannHor_,string sChannVer_,float fGapVer_,float fDCVer_,float fGapHor_,float fDCHor_,float &fX_,float &fY_)
{
	int index=GetSpecAxisLocationIndex(sChannHor_,sChannVer_);
	if (index<0)
		return -1;
	CAxisChannInfo axis;
	_vAxisChann[index]->GetChannInfo(axis);

	int lnHorChannIndex = GetSpecVibChannIndexNo(axis._cHorChann);

	if (lnHorChannIndex<0)
	{
		return -1;
	}
	int lnVerChannIndex = GetSpecVibChannIndexNo(axis._cVerChann);


	if (lnVerChannIndex<0)
	{
		return -1;
	}




	float fSensitivityHor=_vVibChann[lnHorChannIndex]->GetChannInfo()._fSensitivity;
	float fSensitivityVer=_vVibChann[lnVerChannIndex]->GetChannInfo()._fSensitivity;
	S_AxisData axisData;
	axisData._fHorDC=fDCHor_/10.0;
	axisData._fHorGap=fGapHor_;
	axisData._fVerDC=fDCVer_/10.0;
	axisData._fVerGap=fGapVer_;
	if(axisData.CalcAxisLocation(axis._iLocationType,axis._iRotateDirection,axis._fAxisDia,fSensitivityVer,fSensitivityHor))
	{
		fX_=axisData._fCoorX;
		fY_=axisData._fCoorY;
		return 1;
	}
	else
		return -1;
}

bool CPlantInfoOffLine::CalcSpecDynChannLocationValue(string sLocation_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_)
{
   	int index=GetSpecAxisLocationIndex(sLocation_);
	if (index<0)
		return -1;
	CAxisChannInfo axis;
	_vAxisChann[index]->GetChannInfo(axis);
	int iChannIndex=GetSpecDynChannIndexNo(axis._cHorChann);
	if(iChannIndex<0)
	{
		return false;
	}
	float fHorGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	float fSensitivityHor=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	iChannIndex=GetSpecDynChannIndexNo(axis._cVerChann);
	if(iChannIndex<0)
	{
		return false;
	}
	float fVerGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	float fSensitivityVer=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	S_AxisData axisData;
	vector<S_AxisData> vAxisChannValue;
	vAxisChannValue.push_back(axisData);
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	if (iWaveNum_<=0)
	{
		return 0;
	}
	pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	fSensitivityHor==0?8:fSensitivityHor;
	fSensitivityVer==0?8:fSensitivityVer;
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		pVerWave[iRtn]=pVerWave_[iRtn]+(fVerGapV/fSensitivityHor)*1000*10.0;
		pHorWave[iRtn]=pHorWave_[iRtn]+(fHorGapV/fSensitivityHor)*1000*10.0;
	}
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		vAxisChannValue[index]._fHorDC=pHorWave[iRtn]*fSensitivityHor/(1000*10.0);
		vAxisChannValue[index]._fHorGap=fHorGapV;
		vAxisChannValue[index]._fVerDC=pVerWave[iRtn]*fSensitivityVer/(1000*10.0);
		vAxisChannValue[index]._fVerGap=fVerGapV;
		vAxisChannValue[index].CalcAxisLocation(axis._iLocationType,axis._iRotateDirection,axis._fAxisDia,fSensitivityVer,fSensitivityHor);
		pHorAxesWave_[iRtn]=vAxisChannValue[index]._fCoorX;
		pVerAxesWave_[iRtn]=vAxisChannValue[index]._fCoorY;
	}
	vAxisChannValue.clear();
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	return true;
}

bool CPlantInfoOffLine::CalcSpecDynChannLocationValue(string sChannHor_,string sChannVer_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_)
{
	int index=GetSpecDynAxisLocationIndex(sChannHor_,sChannVer_);
	if (index<0)
		return -1;
	CAxisChannInfo axis;
	_vAxisChann[index]->GetChannInfo(axis);

	int iChannIndex=GetSpecDynChannIndexNo(axis._cHorChann);
	if (iChannIndex <0)
	{
		return false;
	}
	//CString strInfo;strInfo.Format("%d,%s,%d,%d",_vAxisChann.size(),axis._cHorChann,index,iChannIndex);
	//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","CalcSpecDynChannLocationValue");
	float fHorGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	float fSensitivityHor=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	iChannIndex=GetSpecDynChannIndexNo(axis._cVerChann);
	if (iChannIndex <0)
	{
		return false;
	}
	float fVerGapV =_vDynChann[iChannIndex]->GetChannInfo()._fGapVoltage;
	float fSensitivityVer=_vDynChann[iChannIndex]->GetChannInfo()._fSensitivity;
	S_AxisData axisData;
    axisData._bCalc = false;
	vector<S_AxisData> vAxisChannValue;
	for (int i=0;i<_vAxisChann.size();i++)
	{
		vAxisChannValue.push_back(axisData);
	}
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	if (iWaveNum_<=0)
	{
		return false;
	}
	pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	fSensitivityHor==0?8:fSensitivityHor;
	fSensitivityVer==0?8:fSensitivityVer;
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		pVerWave[iRtn]=pVerWave_[iRtn]+(fVerGapV/fSensitivityHor)*1000*10.0;
		pHorWave[iRtn]=pHorWave_[iRtn]+(fHorGapV/fSensitivityHor)*1000*10.0;
	}
	for (int iRtn=0;iRtn<iWaveNum_;iRtn++)
	{
		vAxisChannValue[index]._fHorDC=pHorWave[iRtn]*fSensitivityHor/(1000*10.0);
		vAxisChannValue[index]._fHorGap=fHorGapV;
		vAxisChannValue[index]._fVerDC=pVerWave[iRtn]*fSensitivityVer/(1000*10.0);
		vAxisChannValue[index]._fVerGap=fVerGapV;
		//CString strInfo;strInfo.Format("水平原始 %d,%d,%d,%f,%f",iWaveNum_,pHorWave_[iRtn],pVerWave_[iRtn],vAxisChannValue[index]._fHorDC,vAxisChannValue[index]._fVerDC);
		//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","GetSpecDynIndexAxisLocationData");
		vAxisChannValue[index].CalcAxisLocation(axis._iLocationType,axis._iRotateDirection,axis._fAxisDia,fSensitivityVer,fSensitivityHor);
		//strInfo.Format("计算后的波形值 %d,%d",(int)vAxisChannValue[index]._fCoorX,(int)vAxisChannValue[index]._fCoorY);
		//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","GetSpecDynIndexAxisLocationData");
		pHorAxesWave_[iRtn] = vAxisChannValue[index]._fCoorX;
		pVerAxesWave_[iRtn] = vAxisChannValue[index]._fCoorY;
		//strInfo.Format("计算后输出的波形值 %d,%d",(int)pHorAxesWave_[iRtn],(int)pVerAxesWave_[iRtn]);
		//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","GetSpecDynIndexAxisLocationData");
	}
	vAxisChannValue.clear();
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	return true;
}

string CPlantInfoOffLine::GetSpecChannAxisChann(string sChann_)
{
	string sCorrChann="";
	V_PAXISCHANNINFO::iterator it=_vAxisChann.begin();
	CAxisChannInfo axisChannInfo;
	for (;it!=_vAxisChann.end();++it)
	{
        (*it)->GetChannInfo(axisChannInfo);
       if ((axisChannInfo._cVerChann)==sChann_)
	   {
		   sCorrChann=axisChannInfo._cHorChann;
		   break;
	   }
	   if ((axisChannInfo._cHorChann)==sChann_)
	   {
		   sCorrChann=axisChannInfo._cVerChann;
		   break;
	   }
	}
	return sCorrChann;
}

list<string> CPlantInfoOffLine::GetSpecChannSameRevChanns(string sChann_,int &iRevType_)
{
	list<string> lChann;
	//查找这个测点的转速类型
	V_PCHANNINFO::iterator it;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		if (((*it)->GetChannInfo()._cChannID)==sChann_)
		{
			iRevType_=(*it)->GetChannInfo()._iRevType;
			break;
		}
	}
	if (it==_vVibChann.end())
	{//没有这个测点
		iRevType_=-1;

	}
	else
	{
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
          if (iRevType_==((*it)->GetChannInfo()._iRevType))
		  {
			  if (((*it)->GetChannInfo()._cChannID)!=sChann_)
			  	  lChann.push_back((*it)->GetChannInfo()._cChannID);
          }
		}
	}
	return lChann;
}

int CPlantInfoOffLine::AddWholeSudLog(IDBInterFace *pDB_,E_SUD_TYPE sudType_,CBHDateTime dtStart_,CBHDateTime dtEnd_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iEventID;
	_pLockSudData->BeginWrite();
	//得到事件ID
	pDB_->Cmd("SELECT max([%s]) FROM [%s] ",gc_cEventID,_sSudLogTblName.c_str());
	if (pDB_->More())
		iEventID=pDB_->GetLong(gc_cEventID)+1;
	else
		iEventID=1;
	//添加日志
	pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s) ",_sSudLogTblName.c_str(),gc_cID,gc_cEventID,gc_cStart_T,gc_cEnd_T,gc_cType,gc_cStop_Flag,gc_cSynFlag);
	pDB_->Cmd(" VALUES(%d,%d,'%s','%s',%d,1,0)",iEventID,iEventID,TimeConvertString(dtStart_),TimeConvertString(dtEnd_),sudType_);
	if (!(pDB_->Exec()))
	{
		//如果添加出错，察看是否存在这条记录
		pDB_->Cmd(" select [%s] from [%s] ",gc_cEventID,_sSudLogTblName.c_str());
		pDB_->Cmd(" where [%s]='%s' ",gc_cStart_T,TimeConvertString(dtStart_));
		if (pDB_->More())
			iEventID=pDB_->GetLong(gc_cEventID);
		else
			iEventID=-1;
	}
	_pLockSudData->EndWrite();
	return iEventID;
}

void CPlantInfoOffLine::GetSpecVibChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_/* =false */)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		this->GetICESpecVibChannStartEndIndex(pDB_,sChann_,timeSpec_,iStartIndex_,iEndIndex_,bAlarmData_);
		return;
	}
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	int iSmpNum = 0,iSmpFreq = 0;
	int  iRev = 1;
	CString strShowField,strTmp;;
	strShowField.Format("[THREEPHASE_%s],[HALFPHASE_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
		sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
	if (bAlarmData_==true)
	{
		CString strTable=_sAlarmVibDataTblName.c_str();
		pDB_->Cmd("select %s from [%s]",strShowField,strTable);
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if (!pDB_->More())
		{
			CString strSqlWhere;
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			//BHTRACE_DEBUG(strSqlWhere);
			//获取报警数据
			pDB_->Cmd("select %s from [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			if(!(pDB_->More()))
				return;
		}
		strTmp.Format("THREEPHASE_%s",sChann_.c_str());
		iStartIndex_ = pDB_->GetDouble(strTmp);
		strTmp.Format("HALFPHASE_%s",sChann_.c_str());
		iEndIndex_ = pDB_->GetDouble(strTmp);
		iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
		iRev     =  SimADO_GetRevEx(pDB_, sChann_);
	}
	else
	{
		vector<string> sTable;
		vector<string>::iterator it;
		sTable.push_back(_sVibTrendSecTblName);
		sTable.push_back(_sVibTrendHourTblName);
		sTable.push_back(_sVibTrendDayTblName);
		sTable.push_back(_sVibTrendMonthTblName);
		sTable.push_back( _sVibTrendFastTblName);

		for(it = sTable.begin();it!=sTable.end();it++)
		{
			pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
			if(!pDB_->More())
			{
				continue;
			}
			else
			{
				strTmp.Format("THREEPHASE_%s",sChann_.c_str());
				iStartIndex_ = pDB_->GetDouble(strTmp);
				strTmp.Format("HALFPHASE_%s",sChann_.c_str());
				iEndIndex_ = pDB_->GetDouble(strTmp);
				iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
				iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
				iRev     =  SimADO_GetRevEx(pDB_, sChann_);
				break;
			}
		}
	}
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

void CPlantInfoOffLine::GetICESpecVibChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	int iSmpNum = 0,iSmpFreq = 0;
	int  iRev = 1;
	CString strShowField,strTmp;;
	CString lstrChann;
	BOOL lbSpecData = FALSE;
	strShowField.Format("[ALL_%s],[ONE_%s],[TWOPHASE_%s],[THREE_%s],[HALF_%s],[THREEPHASE_%s],[HALFPHASE_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
		sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
	if (bAlarmData_==true)
	{
		CString strTable=_sAlarmVibDataTblName.c_str();
		pDB_->Cmd("select %s from [%s]",strShowField,strTable);
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if (!pDB_->More())
		{
			CString strSqlWhere;
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			//BHTRACE_DEBUG(strSqlWhere);
			//获取报警数据
			pDB_->Cmd("select %s from [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			if(!(pDB_->More()))
				return;
		}
		lstrChann = sChann_.c_str();
		if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
		{
			strTmp.Format("ALL_%s",sChann_.c_str());
			lbSpecData =  TRUE;

		}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
		{
			strTmp.Format("TWOPHASE_%s",sChann_.c_str());
			lbSpecData =  TRUE;

		}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
		{
			strTmp.Format("HALF_%s",sChann_.c_str());
			lbSpecData =  TRUE;
		}else
		{
			strTmp.Format("THREE_%s",sChann_.c_str());
			lbSpecData =  TRUE;
		}

		iStartIndex_ = pDB_->GetDouble(strTmp);

		if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
		{
			strTmp.Format("ONE_%s",sChann_.c_str());
		}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
		{
			strTmp.Format("THREEPHASE_%s",sChann_.c_str());
		}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
		{
			strTmp.Format("RES_%s",sChann_.c_str());
		}else
		{
			strTmp.Format("HALF_%s",sChann_.c_str());
		}


		iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
		iRev     =  SimADO_GetRevEx(pDB_, sChann_);

		iEndIndex_ = pDB_->GetDouble(strTmp);
		iEndIndex_ = iEndIndex_-1;
		iStartIndex_ = iStartIndex_ - 1;

		
		if ((iEndIndex_-iStartIndex_)<= 256)
		{
			iStartIndex_ = 0;
			iEndIndex_ = iSmpNum;
		}



	}
	else
	{
		vector<string> sTable;
		vector<string>::iterator it;
		sTable.push_back(_sVibTrendSecTblName);
		sTable.push_back(_sVibTrendHourTblName);
		sTable.push_back(_sVibTrendDayTblName);
		sTable.push_back(_sVibTrendMonthTblName);
		sTable.push_back( _sVibTrendFastTblName);

		for(it = sTable.begin();it!=sTable.end();it++)
		{
			pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
			if(!pDB_->More())
			{
				continue;
			}
			else
			{
				lstrChann = sChann_.c_str();
				if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
				{
					strTmp.Format("ALL_%s",sChann_.c_str());
					lbSpecData =  TRUE;

				}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
				{
					strTmp.Format("TWOPHASE_%s",sChann_.c_str());
					lbSpecData =  TRUE;

				}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
				{
					strTmp.Format("HALF_%s",sChann_.c_str());
					lbSpecData =  TRUE;
				}else
				{
					strTmp.Format("THREE_%s",sChann_.c_str());
					lbSpecData =  TRUE;
				}

				iStartIndex_ = pDB_->GetDouble(strTmp);

				if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
				{
					strTmp.Format("ONE_%s",sChann_.c_str());
				}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
				{
					strTmp.Format("THREEPHASE_%s",sChann_.c_str());
				}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
				{
					strTmp.Format("RES_%s",sChann_.c_str());
				}else
				{
					strTmp.Format("HALF_%s",sChann_.c_str());
				}


				iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
				iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
				iRev     =  SimADO_GetRevEx(pDB_, sChann_);

				iEndIndex_ = pDB_->GetDouble(strTmp);
				iEndIndex_ = iEndIndex_-1;
				iStartIndex_ = iStartIndex_ - 1;


				if ((iEndIndex_-iStartIndex_)<= 256)
				{
					iStartIndex_ = 0;
					iEndIndex_ = iSmpNum;
				}



				break;
			}
		}
	}
	//当设备停机转速为-1时，按额定转速计算起止索引
	iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;
	if (lbSpecData)
	{
		return;
	}
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

void CPlantInfoOffLine::GetSpecDynChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_/* =false */)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		this->GetICESpecDynChannStartEndIndex(pDB_,sChann_,timeSpec_,iStartIndex_,iEndIndex_,bAlarmData_);
		return;
	}
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	int iSmpNum = 0,iSmpFreq = 0;
	int  iRev = 1;
	CString strShowField,strTmp;;
	strShowField.Format("[PERIOD_START_INDEX_%s],[PERIOD_END_INDEX_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
		sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
	if (bAlarmData_==true)
	{
		CString strTable=_sAlarmDynDataTblName.c_str();
		pDB_->Cmd("select %s from [%s]",strShowField,strTable);
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if (!pDB_->More())
		{
			CString strSqlWhere;
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			//BHTRACE_DEBUG(strSqlWhere);
			//获取报警数据
			pDB_->Cmd("select %s from [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			if(!(pDB_->More()))
				return;
		}
		strTmp.Format("PERIOD_START_INDEX_%s",sChann_.c_str());
		iStartIndex_ = pDB_->GetDouble(strTmp);
		strTmp.Format("PERIOD_END_INDEX_%s",sChann_.c_str());
		iEndIndex_ = pDB_->GetDouble(strTmp);
		iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
		iRev     =  SimADO_GetRevEx(pDB_, sChann_);
	}
	else
	{
		vector<string> sTable;
		vector<string>::iterator it;
		sTable.push_back(_sDynTrendSecTblName);
		sTable.push_back(_sDynTrendHourTblName);
		sTable.push_back(_sDynTrendDayTblName);
		sTable.push_back(_sDynTrendMonthTblName);
		sTable.push_back(_sDynTrendFastTblName);
		for(it = sTable.begin();it!=sTable.end();it++)
		{
			pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
			if(!pDB_->More())
			{
				continue;
			}
			else
			{
				strTmp.Format("PERIOD_START_INDEX_%s",sChann_.c_str());
				iStartIndex_ = pDB_->GetDouble(strTmp);
				strTmp.Format("PERIOD_END_INDEX_%s",sChann_.c_str());
				iEndIndex_ = pDB_->GetDouble(strTmp);
				iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
				iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
				iRev     =  SimADO_GetRevEx(pDB_, sChann_);
				break;
			}
		}
	}
	//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%d",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,iRev);
	//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","GetSpecDynChannStartEndIndex");
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

void CPlantInfoOffLine::GetICESpecDynChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	int iSmpNum = 0,iSmpFreq = 0;
	int  iRev = 1;
	CString strShowField,strTmp;;
	strShowField.Format("[PERIOD_START_INDEX_%s],[PERIOD_END_INDEX_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
		sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
	if (bAlarmData_==true)
	{
		CString strTable=_sAlarmDynDataTblName.c_str();
		pDB_->Cmd("select %s from [%s]",strShowField,strTable);
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if (!pDB_->More())
		{
			CString strSqlWhere;
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			//BHTRACE_DEBUG(strSqlWhere);
			//获取报警数据
			pDB_->Cmd("select %s from [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			if(!(pDB_->More()))
				return;
		}
		strTmp.Format("PERIOD_START_INDEX_%s",sChann_.c_str());
		iStartIndex_ = pDB_->GetDouble(strTmp);
		strTmp.Format("PERIOD_END_INDEX_%s",sChann_.c_str());
		iEndIndex_ = pDB_->GetDouble(strTmp);
		iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
		iRev     =  SimADO_GetRevEx(pDB_, sChann_);
	}
	else
	{
		vector<string> sTable;
		vector<string>::iterator it;
		sTable.push_back(_sDynTrendSecTblName);
		sTable.push_back(_sDynTrendHourTblName);
		sTable.push_back(_sDynTrendDayTblName);
		sTable.push_back(_sDynTrendMonthTblName);
		sTable.push_back(_sDynTrendFastTblName);
		for(it = sTable.begin();it!=sTable.end();it++)
		{
			pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
			if(!pDB_->More())
			{
				continue;
			}
			else
			{
				strTmp.Format("PERIOD_START_INDEX_%s",sChann_.c_str());
				iStartIndex_ = pDB_->GetDouble(strTmp);
				strTmp.Format("PERIOD_END_INDEX_%s",sChann_.c_str());
				iEndIndex_ = pDB_->GetDouble(strTmp);
				iSmpNum  = SimADO_GetSampleNumEx(pDB_, sChann_);
				iSmpFreq = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
				iRev     =  SimADO_GetRevEx(pDB_, sChann_);
				break;
			}
		}
	}
	//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%d",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,iRev);
	//CHZLogManage::Debug( strInfo, "CPlantInfoOffLine","GetSpecDynChannStartEndIndex");
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

void CPlantInfoOffLine::GetSpecVibChannFreqNumRev(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iSmpFreq_,int &iSmpNum_,int &iRev_,bool bExactTime_/* =true */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	vector<string> sTable;
	vector<string>::iterator it;
	sTable.push_back(_sVibTrendSecTblName);
	sTable.push_back(_sVibTrendHourTblName);
	sTable.push_back(_sVibTrendDayTblName);
	sTable.push_back(_sVibTrendMonthTblName);
	sTable.push_back(_sVibTrendFastTblName);
	for(it = sTable.begin();it!=sTable.end();it++)
	{
		CString strShowField;
		strShowField.Format("[THREEPHASE_%s],[HALFPHASE_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
			sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
		pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
		if (bExactTime_ ==true)
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		else
		{
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, TRENDRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			pDB_->Cmd("where [%s] > '%s' and [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd));
		}
		if(!pDB_->More())
		{
			continue;
		}
		else
		{
			CString strTmp;
			iSmpNum_  = SimADO_GetSampleNumEx(pDB_, sChann_);
			iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
			iRev_     =  SimADO_GetRevEx(pDB_, sChann_);
			break;
		}
	}
}

void CPlantInfoOffLine::GetSpecDynChannFreqNumRev(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iSmpFreq_,int &iSmpNum_,int &iRev_,bool bExactTime_/* =true */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	vector<string> sTable;
	vector<string>::iterator it;
	sTable.push_back(_sDynTrendSecTblName);
	sTable.push_back(_sDynTrendHourTblName);
	sTable.push_back(_sDynTrendDayTblName);
	sTable.push_back(_sDynTrendMonthTblName);
	sTable.push_back(_sDynTrendFastTblName);
	for(it = sTable.begin();it!=sTable.end();it++)
	{
		CString strShowField;
		strShowField.Format("[PERIOD_START_INDEX_%s],[PERIOD_END_INDEX_%s],[SAMPLE_NUM_%s],[SAMPLE_FREQ_%s],[REV_%s],%s",
			sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(), ConsCommonField());
		pDB_->Cmd("select %s from [%s]",strShowField,(*it).c_str());
		if (bExactTime_ ==true)
			pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		else
		{
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, TRENDRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			pDB_->Cmd("where [%s] > '%s' and [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd));
		}
		if(!pDB_->More())
		{
			continue;
		}
		else
		{
			iSmpNum_  = SimADO_GetSampleNumEx(pDB_, sChann_);
			iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, sChann_);
			iRev_     =  SimADO_GetRevEx(pDB_, sChann_);
			break;
		}
	}
}

void CPlantInfoOffLine::GetSpecVibChannCustomParam(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,float *fCustomParom_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	vector<string> sTable;
	vector<string>::iterator it;
	sTable.push_back(_sVibTrendSecTblName);
	sTable.push_back(_sVibTrendHourTblName);
	sTable.push_back(_sVibTrendDayTblName);
	sTable.push_back(_sVibTrendMonthTblName);
	sTable.push_back(_sVibTrendFastTblName);
	for(it = sTable.begin();it!=sTable.end();it++)
	{
		pDB_->Cmd("select [CUSTOM_PARAM_%s] from [%s]",sChann_.c_str(),(*it).c_str());
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if(!pDB_->More())
		{
			continue;
		}
		else
		{
			CString strTmp;
			strTmp.Format("CUSTOM_PARAM_%s",sChann_.c_str());
			long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
			//得到自定义参数信息
			CWaveHead lsWaveHead;
			SimADO_SafeGetBufferEx(pDB_, strTmp, (BYTE*)fCustomParom_, iAllBytes,lsWaveHead);
			break;
		}
	}
}

void CPlantInfoOffLine::GetSpecDynChannCustomParam(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,float *fCustomParom_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	vector<string> sTable;
	vector<string>::iterator it;
	sTable.push_back(_sDynTrendSecTblName);
	sTable.push_back(_sDynTrendHourTblName);
	sTable.push_back(_sDynTrendDayTblName);
	sTable.push_back(_sDynTrendMonthTblName);
	sTable.push_back(_sDynTrendFastTblName);
	for(it = sTable.begin();it!=sTable.end();it++)
	{
		pDB_->Cmd("select [CUSTOM_PARAM_%s] from [%s]",sChann_.c_str(),(*it).c_str());
		pDB_->Cmd(" where [%s]='%s'",gc_cTime,strTime);
		if(!pDB_->More())
		{
			continue;
		}
		else
		{
			CString strTmp;
			strTmp.Format("CUSTOM_PARAM_%s",sChann_.c_str());
			long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
			//得到自定义参数信息
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB_, strTmp, (BYTE*)fCustomParom_, iAllBytes, sWaveHead);
			break;
		}
	}
}

int CPlantInfoOffLine::GetSpecChannCylinderNo(IDBInterFace *pDB_,string sChann_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int iRtn = -1,iCylinderNo =0;
	//得到事件ID
	pDB_->Cmd("select [%s] from [%s]",gc_cCylinderNo,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cChannID,sChann_.c_str());
	if (pDB_->More())
	{
		iCylinderNo = pDB_->GetLong(gc_cCylinderNo);
	}
	iRtn = iCylinderNo;
	return iRtn;
}

int CPlantInfoOffLine::ChangeDynCharValueToNew(S_DynCharValue *pDynCharValue_,S_OldDynCharValue *pOldDynCharValue_,float *pfRev_,const int &iSmpNum_, const int &iSmpFreq_, const int iRealNum_,const int &iLen_,const int &iStartIndex_)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//转化特征值数据
		try
		{
			for(int loop=iStartIndex_; loop< (iStartIndex_ + iLen_); ++loop)
			{
				float fRev=.0;
				int iRevType=(_vDynChann[loop])->GetChannInfo()._iRevType;
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

	}
	return nCount;
}

int CPlantInfoOffLine::ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,S_OldVibCharValue *pOldVibCharValue_,float *pfRev_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//转化特征值数据
		for(int loop=iStartIndex_;loop<(iStartIndex_ + iLen_);++loop)
		{
			float fRev=.0;
			int iRevType=(_vVibChann[loop])->GetChannInfo()._iRevType;
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
			pVibCharValue_[nCount]._iRev=(int)fRev;
			//CString sTemp;sTemp.Format("Vib:_fCharAll,%f,_fCharOne:%f,_fCharTwo:%f",pVibCharValue_[nCount]._fCharAll,pVibCharValue_[nCount]._fCharOne,pVibCharValue_[nCount]._fCharTwo);BHTRACE_DEBUG(sTemp);
			nCount++;
		}
	}
	return nCount;
}

bool CPlantInfoOffLine::GetSpecTimeChannAndTypeAlarmState(IDBInterFace *pDB_,CBHDateTime timeSpec_,string sSuffix_,int index_,string &sChann_,string &sChannAlias_,string &sUnit_,int &iList_,unsigned int &iState_,int &iChannDetailType_,int iType_)
{
	iState_ = 0;
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	if (index_<0)
		return false;
	S_ChannInfo channInfo;
	string sTableName;
	CString strSqlWhere,strShowField;
	COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
	dtStart = timeSpec_ - timeSpan;
	dtEnd   = timeSpec_ + timeSpan;
	strSqlWhere.Format(" [%s] >= '%s' AND [%s] < '%s' order by %s ", gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd),gc_cTime );

	int iCharType = 0;




	//BHTRACE_DEBUG(strSqlWhere);
	switch (iType_)
	{
	case GE_VIBCHANN:
		{
			if (index_> _sPlantInfo._iVibChannNum)
				return false;
			channInfo=_vVibChann[index_]->GetChannInfo();
			sUnit_ = CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType);
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			//查询报警状态
			sTableName="Alarm_VIB_"+sSuffix_;
			int iEventID = -1;
			strShowField.Format("TYPE_%s",channInfo._cChannNo);
			pDB_->Cmd("SELECT top 1 [%s], [%s] FROM [%s] ",gc_cEventID,strShowField,sTableName.c_str());
			pDB_->Cmd(" WHERE %s ", strSqlWhere );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(strShowField);
				iEventID = pDB_->GetLong(gc_cEventID);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}

			sTableName="Alarm_LOG_"+sSuffix_;
			pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cType,sTableName.c_str());
			pDB_->Cmd(" WHERE %s = %d and %s = '%s'", gc_cEventID,iEventID,gc_cName,channInfo._cChannNo );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(gc_cType);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}
		}
		break;
	case GE_AXIALCHANN:
		{
			if (index_>_iAxialVibChannNum)
				return false;
			index_=index_+_iRadialVibChannNum;
			channInfo=_vVibChann[index_]->GetChannInfo();
			sUnit_ = CharInfoHelper::GetCharUnit(channInfo._iInfoType, GE_AXIALDISPLACE);
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			int iEventID = -1;
			//查询报警状态
			sTableName="Alarm_VIB_"+sSuffix_;
			strShowField.Format("TYPE_%s",channInfo._cChannNo);
			pDB_->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ",gc_cEventID,strShowField,sTableName.c_str());
			pDB_->Cmd(" WHERE %s ", strSqlWhere );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(strShowField);
				iEventID = pDB_->GetLong(gc_cEventID);
			}

			sTableName="Alarm_LOG_"+sSuffix_;
			pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cType,sTableName.c_str());
			pDB_->Cmd(" WHERE %s = %d and %s = '%s'", gc_cEventID,iEventID,gc_cName,channInfo._cChannNo );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(gc_cType);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}
		}
		break;
	case GE_AXISLOCATIONCHANN:
		{
			if (index_>_iAxesPairNum)
				return false;
			CString sAxisLocation;sAxisLocation.Format("轴心位置%d",index_+1);
			sChann_=sAxisLocation.GetString();
			sChannAlias_=sChann_;
			int iEventID = -1;
			//查询报警状态
			sTableName="Alarm_AXIS_"+sSuffix_;
			strShowField="TYPE";
			pDB_->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ",gc_cEventID,strShowField,sTableName.c_str());
			pDB_->Cmd(" WHERE %s ", strSqlWhere );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(strShowField);
				iEventID = pDB_->GetLong(gc_cEventID);
			}
			sTableName="Alarm_LOG_"+sSuffix_;
			pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cType,sTableName.c_str());
			pDB_->Cmd(" WHERE %s = %d and %s = '%s'", gc_cEventID,iEventID,gc_cName,channInfo._cChannNo );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(gc_cType);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}
		}
		break;
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		{
			if (index_> _sPlantInfo._iProcChannNum)
				return false;
			channInfo=_vProcChann[index_]->GetChannInfo();
			sUnit_=channInfo._cUnit;
			sChann_=channInfo._cChannID;
			CString lstrChannNo = channInfo._cChannNo;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			int iEventID = -1;
			//查询报警状态
			sTableName="Alarm_PROC_"+sSuffix_;
			strShowField="TYPE";
			pDB_->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ",gc_cEventID,strShowField,sTableName.c_str());
			pDB_->Cmd(" WHERE [%s]='%s' AND %s ",gc_cChannNo,lstrChannNo, strSqlWhere );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(strShowField);
				iEventID = pDB_->GetLong(gc_cEventID);
			}
			sTableName="Alarm_LOG_"+sSuffix_;
			pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cType,sTableName.c_str());
			pDB_->Cmd(" WHERE %s = %d and %s = '%s'", gc_cEventID,iEventID,gc_cName,channInfo._cChannNo );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(gc_cType);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}
		}
		break;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
	default:
		{
			if (index_> _sPlantInfo._iDynChannNum)
				return false;
			channInfo=_vDynChann[index_]->GetChannInfo();
			sUnit_ = CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType);
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			int iEventID = -1;
			//查询报警状态
			sTableName="Alarm_DYN_"+sSuffix_;
			strShowField.Format("TYPE_%s",channInfo._cChannNo);
			pDB_->Cmd("SELECT  top 1 [%s],[%s] FROM [%s] ",gc_cEventID,strShowField,sTableName.c_str());
			pDB_->Cmd(" WHERE %s ", strSqlWhere );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(strShowField);
				iEventID = pDB_->GetLong(gc_cEventID);

			}
			sTableName="Alarm_LOG_"+sSuffix_;
			pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cType,sTableName.c_str());
			pDB_->Cmd(" WHERE %s = %d and %s = '%s'", gc_cEventID,iEventID,gc_cName,channInfo._cChannNo );
			if (pDB_->More())
			{
				iState_= pDB_->GetLong(gc_cType);
				//CString sTemp;sTemp.Format("iState_:%d",iState_);BHTRACE_DEBUG(sTemp);
			}
		}
	}
	return true;
}


int CPlantInfoOffLine::GetDynPressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_)
{
	int index=-1;
	vector<string> vChann;
	SwitchDbNameEx(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNPRESSCHANN);
	while (pDB_->More())
	{
		vChann.push_back(pDB_->Get(gc_cChannID));
	}
	if (vChann.size()>1)
	{
		sDynChann1_=vChann[0];
		sDynChann2_=vChann[1];
		index=1;
	}
	vChann.clear();
	return index;
}

int CPlantInfoOffLine::GetDynStressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_)
{
	int index=-1;
	SwitchDbNameEx(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNSTRESSCHANN);
	if (pDB_->More())
	{
		sDynStressChann_ =pDB_->Get(gc_cChannID);
		if (!sDynStressChann_.empty())
			index=1;
	}
	else
		index=-1;
	return index;
}

BOOL CPlantInfoOffLine::IsSample() const
{
	return m_bIsSample;
}

void CPlantInfoOffLine::IsSample( BOOL val )
{
	this->SetIsSampleTime(CBHDateTime::GetCurrentTime());
	m_bIsSample = val;
}

CBHDateTime CPlantInfoOffLine::SetIsSampleTime() const
{
	return m_oSetIsSampleTime;
}

void CPlantInfoOffLine::SetIsSampleTime( CBHDateTime val )
{
	m_oSetIsSampleTime = val;
}

int CPlantInfoOffLine::GetSpecTypeChannInfo(S_ChannInfo &sChannInfo_,int iIndex_/* =0 */,int iType_/* =GE_ALLPROC */)
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


int CPlantInfoOffLine::SetOffVibAxesLocationParam( int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_ )
{
	if (anHorChannIndex<0 || anVerChannIndex < 0 || anAxisChannIndex < 0)
	{
		return -1;
	}

	this->_pLockSudData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anHorChannIndex < this->_vVibChann.size())
		{
			this->_vVibChann[anHorChannIndex]->_channInfo._fSensitivity = aHorChannInfo._fSensitivity;
			this->_vVibChann[anHorChannIndex]->_channInfo._fGapVoltage = aHorChannInfo._fGapVoltage;
		}
		if(anVerChannIndex < this->_vVibChann.size())
		{
			this->_vVibChann[anVerChannIndex]->_channInfo._fSensitivity = aVerChannInfo._fSensitivity;
			this->_vVibChann[anVerChannIndex]->_channInfo._fGapVoltage = aVerChannInfo._fGapVoltage;
		}

		if(anAxisChannIndex < this->_vAxisChann.size())
		{
			CAxisChannInfo lAxisChannInfo = this->_vAxisChann[anAxisChannIndex]->GetChannInfo();
			lAxisChannInfo._fAxisDia = aAxisChanInfo_._fAxisDia;
			lAxisChannInfo._iLocationType = aAxisChanInfo_._iLocationType;
			lAxisChannInfo._iRotateDirection = aAxisChanInfo_._iRotateDirection;

			this->_vAxisChann[anAxisChannIndex]->SetChannInfo(lAxisChannInfo);

		}

		END_ERROR_HANDLE

			this->_pLockSudData->EndWrite();

		return TRUE;
}

int CPlantInfoOffLine::SetOffDynAxesLocationParam( int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_ )
{
	if (anHorChannIndex<0 || anVerChannIndex < 0 || anAxisChannIndex < 0)
	{
		return -1;
	}

	this->_pLockSudData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anHorChannIndex < this->_vDynChann.size())
		{
			this->_vDynChann[anHorChannIndex]->_channInfo._fSensitivity = aHorChannInfo._fSensitivity;
			this->_vDynChann[anHorChannIndex]->_channInfo._fGapVoltage = aHorChannInfo._fGapVoltage;
		}
		if(anVerChannIndex < this->_vDynChann.size())
		{
			this->_vDynChann[anVerChannIndex]->_channInfo._fSensitivity = aVerChannInfo._fSensitivity;
			this->_vDynChann[anVerChannIndex]->_channInfo._fGapVoltage = aVerChannInfo._fGapVoltage;
		}

		if(anAxisChannIndex < this->_vAxisChann.size())
		{
			CAxisChannInfo lAxisChannInfo = this->_vAxisChann[anAxisChannIndex]->GetChannInfo();
			lAxisChannInfo._fAxisDia = aAxisChanInfo_._fAxisDia;
			lAxisChannInfo._iLocationType = aAxisChanInfo_._iLocationType;
			lAxisChannInfo._iRotateDirection = aAxisChanInfo_._iRotateDirection;

			this->_vAxisChann[anAxisChannIndex]->SetChannInfo(lAxisChannInfo);
		}

		END_ERROR_HANDLE

			this->_pLockSudData->EndWrite();

		return TRUE;
}