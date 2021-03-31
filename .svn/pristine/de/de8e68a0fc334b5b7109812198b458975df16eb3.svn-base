#include "StdAfx.h"
#include "Implement.h"
#include "SqlBuilderForExpert.h"
#include "ExpertSysGlobal.h"
#include <additions/DebugHelper.h>
#include "MiddleWare.h"

BOOL SqlHelperForExpert::GetSqlForGetSpecChannTypeAlarmStatus(	const CPlantInfoExpertSys		* apPlantInfo,	
															    const S_ChannInfo				& arefChanninfo,
															    CHANNTYPE						  anChannType,
															    CBHDateTime					& arefTimeStart,
															    CBHDateTime					& arefTimeEnd,															  
															    CString							& acstrSql,
															    CImplement::ALARM_CHANN_TYPE	aenumType,
																GE_MEASURE_DIRECTION			aenumDirection /*=MEASURE_DIRECTION_ALL*/)
{
	if (!apPlantInfo)
	{
		return FALSE;
	}

	CString lcstrTable;
	CString lstrPlantNo = apPlantInfo->GetPlantNo().c_str();

	lcstrTable.Format(_T(" [%s]..[%s] t2,[%s]..[%s] t1 "),
						SqlHelperForExpert::GetMainDBName(),
						gc_cChannTable,
						SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)),
						apPlantInfo->GetAlarmLogTblName().c_str());

	int lnSectionNo = arefChanninfo._iCylinderNo;
	if (aenumType == CImplement::EXPERT_CHANN_TYPE_CRANK)
	{
		lnSectionNo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSectionCrankNo();
	}
	//沉降类测点
	if (GE_RODSINKCHANN == anChannType )
	{
		acstrSql.Format("   SELECT top 1 *  FROM %s WHERE	(t1.[%s] = t2.[%s]) AND (t1.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = %d) AND (t2.[%s] = %d) AND (t2.[%s] = %d) AND (t2.[%s] = '%s') ",	
			lcstrTable,
			gc_cName,
			gc_cChannNo,
			gc_cStart_T,
			CPlantInfoBase::TimeConvertString(arefTimeStart),
			CPlantInfoBase::TimeConvertString(arefTimeEnd),
			gc_cChannType,
			anChannType,
			gc_cCylinderNo,
			lnSectionNo,
			gc_cMesurement_Direction,
			aenumDirection,
			gc_cPlantNo,
			lstrPlantNo.GetBuffer(0)
			);	
	}else
	{
		if (aenumType == CImplement::EXPERT_CHANN_TYPE_CROSSHEAD)
		{
			int lnLocationIdx = -3;

			lnLocationIdx = ((CPlantInfoExpertSys *)apPlantInfo)->GetCrossHeadLocationIndex(lnSectionNo);			
			
			acstrSql.Format("   SELECT top 1 *  FROM %s WHERE	(t1.[%s] = t2.[%s]) AND (t1.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = %d) AND (t2.[%s] = %d) AND (t2.[%s] = %d) AND (t2.[%s] = '%s') ",	
				lcstrTable,
				gc_cName,
				gc_cChannNo,
				gc_cStart_T,
				CPlantInfoBase::TimeConvertString(arefTimeStart),
				CPlantInfoBase::TimeConvertString(arefTimeEnd),
				gc_cChannType,
				anChannType,
				gc_cCylinderNo,
				lnSectionNo,
				gc_cLocation_IDX,
				lnLocationIdx,
				gc_cPlantNo,
				lstrPlantNo.GetBuffer(0)
				);
		}else
		{
			acstrSql.Format("   SELECT top 1 *  FROM %s WHERE	(t1.[%s] = t2.[%s]) AND (t1.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = %d) AND (t2.[%s] = %d)  AND (t2.[%s] = '%s') ",	
				lcstrTable,
				gc_cName,
				gc_cChannNo,
				gc_cStart_T,
				CPlantInfoBase::TimeConvertString(arefTimeStart),
				CPlantInfoBase::TimeConvertString(arefTimeEnd),
				gc_cChannType,
				anChannType,
				gc_cCylinderNo,
				lnSectionNo,
				gc_cPlantNo,
				lstrPlantNo.GetBuffer(0)
				);
		}


	}


	return TRUE;
}


BOOL SqlHelperForExpert::GetSqlForSpecChannTypeSensorStatus(	
	const CPlantInfoExpertSys		* apPlantInfo,	
	const S_ChannInfo				& arefChanninfo,
	CHANNTYPE						  anChannType,
	CBHDateTime					& arefTimeStart,
	CBHDateTime					& arefTimeEnd,	
	CString							& acstrSql,
	CImplement::ALARM_CHANN_TYPE	  aenumType)
{
	if (!apPlantInfo)
	{
		return FALSE;
	}

	CString lstPlantNO = apPlantInfo->GetPlantNo().c_str();
	CString lcstrTable;

	lcstrTable.Format(_T(" [%s]..[%s] t1,[%s]..[%s] t2 "),
						SqlHelperForExpert::GetMainDBName(),
						gc_cChannTable,
						SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)),
						apPlantInfo->GetAlarmLogTblName().c_str());

	int lnSectionNo = arefChanninfo._iCylinderNo;
	if (aenumType == CImplement::EXPERT_CHANN_TYPE_CRANK)
	{
		lnSectionNo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSectionCrankNo();
	}

	if (GE_OTHERCHANN == anChannType)
	{
		acstrSql.Format("   SELECT t1.[%s],t1.[%s], t2.[%s]  FROM %s WHERE (t2.[%s] = t1.[%s]) AND (t2.[%s] BETWEEN '%s' AND '%s')  AND (t1.[%s] = %d) AND t1.[%s] = '%s' ",	
						gc_cChannNo,
						gc_cChannType,
						gc_cStart_T,
						lcstrTable,
						gc_cName,
						gc_cChannNo,
						gc_cStart_T,
						CPlantInfoBase::TimeConvertString(arefTimeStart),
						CPlantInfoBase::TimeConvertString(arefTimeEnd),
						gc_cCylinderNo,
						lnSectionNo,
						gc_cPlantNo,
						lstPlantNO.GetBuffer(0)
						);	
	}else if (CImplement::EXPERT_CHANN_TYPE_SINK == aenumType)
	{
		acstrSql.Format("   SELECT t1.[%s],t1.[%s], t2.[%s]  FROM %s WHERE (t2.[%s] = t1.[%s]) AND (t2.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = %d) AND (t1.[%s] = %d) AND (t1.[%s]= %d) AND t1.[%s] = '%s' ",	
						gc_cChannNo,
						gc_cChannType,
						gc_cStart_T,
						lcstrTable,
						gc_cName,
						gc_cChannNo,
						gc_cStart_T,
						CPlantInfoBase::TimeConvertString(arefTimeStart),
						CPlantInfoBase::TimeConvertString(arefTimeEnd),
						gc_cChannType,
						anChannType,
						gc_cCylinderNo,
						lnSectionNo,
						gc_cMesurement_Direction,
						MEASURE_DIRECTION_VERTICAL,
						gc_cPlantNo,
						lstPlantNO.GetBuffer(0)
						);	

	}else if (CImplement::EXPERT_CHANN_TYPE_DEFLECT == aenumType)
	{
		acstrSql.Format("   SELECT t1.[%s],t1.[%s], t2.[%s]  FROM %s WHERE (t2.[%s] = t1.[%s]) AND (t2.[%s] BETWEEN '%s' AND '%s')  AND (t1.[%s] = %d) AND (t1.[%s] = %d) AND (t1.[%s]= %d) AND t1.[%s] = '%s' ",	
						gc_cChannNo,
						gc_cChannType,
						gc_cStart_T,
						lcstrTable,
						gc_cName,
						gc_cChannNo,
						gc_cStart_T,
						CPlantInfoBase::TimeConvertString(arefTimeStart),
						CPlantInfoBase::TimeConvertString(arefTimeEnd),
						gc_cChannType,
						anChannType,
						gc_cCylinderNo,
						lnSectionNo,
						gc_cMesurement_Direction,
						MEASURE_DIRECTION_HORIZONTAL,
						gc_cPlantNo,
						lstPlantNO.GetBuffer(0)
						);	
	}
	else{
		acstrSql.Format("   SELECT t1.[%s],t1.[%s], t2.[%s]  FROM %s WHERE (t2.[%s] = t1.[%s]) AND (t2.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = %d) AND (t1.[%s] = %d) AND t1.[%s] = '%s' ",	
						gc_cChannNo,
						gc_cChannType,
						gc_cStart_T,
						lcstrTable,
						gc_cName,
						gc_cChannNo,
						gc_cStart_T,
						CPlantInfoBase::TimeConvertString(arefTimeStart),
						CPlantInfoBase::TimeConvertString(arefTimeEnd),
						gc_cChannType,
						anChannType,
						gc_cCylinderNo,
						lnSectionNo,
						gc_cPlantNo,
						lstPlantNO.GetBuffer(0)
						);	
	}



	return TRUE;
}

BOOL SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(    IDBInterFace & arefDB,
																const CPlantInfoExpertSys * apPlantInfo, 
																const S_ChannInfo & arefChanninfo, 
																CHANNTYPE anChannType, 
																CBHDateTime & arefTimeStart, 
																CBHDateTime & arefTimeEnd, 
																CString & acstrSql,
																GE_MEASURE_DIRECTION aiDirection /*= MEASURE_DIRECTION_ALL*/,
																BOOL abAllChann /*= FALSE*/ )
{

	if (!apPlantInfo)
	{
		return FALSE;
	}

	CString lcstrTable;
	CString lstrPlantNo = apPlantInfo->GetPlantNo().c_str();

	lcstrTable.Format(_T(" [%s]..[%s] t2,[%s]..[%s] t1 "),
		SqlHelperForExpert::GetMainDBName(),
		gc_cChannTable,
		SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)),
		apPlantInfo->GetAlarmLogTblName().c_str());


	if (abAllChann)
	{
		SwitchDbNameEx(&arefDB,SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo));

		acstrSql.Format(" SELECT t1.[%s], t1.[%s],t2.[%s] FROM %s WHERE (t2.[%s] = %d) AND (t1.[%s] BETWEEN '%s' AND '%s') AND (t1.[%s] = t2.[%s]) AND (t2.[%s] = '%s') ORDER BY t1.[%s]",
			gc_cEventID,
			gc_cStart_T,
			gc_cChannNo,
			lcstrTable,
			gc_cCylinderNo,
			arefChanninfo._iCylinderNo,
			gc_cStart_T,
			CPlantInfoBase::TimeConvertString(arefTimeStart),
			CPlantInfoBase::TimeConvertString(arefTimeEnd),
			gc_cName,
			gc_cChannNo,
			gc_cPlantNo,
			lstrPlantNo.GetBuffer(0),
			gc_cStart_T
			);

		arefDB.Cmd(acstrSql);

		if (arefDB.More())
		{
			CBHDateTime loTimeStart = arefDB.GetTime(gc_cStart_T);
			CString lstrChannNo = arefDB.Get(gc_cChannNo).c_str();

			SwitchDbNameEx(&arefDB,SqlHelperForExpert::GetMainDBName());
			
			if (aiDirection == MEASURE_DIRECTION_ALL)
			{
				acstrSql.Format(" SELECT [%s],[%s], '%s' as [%s] FROM %s WHERE ([%s] = %d) AND ([%s] = %d) AND ([%s] = '%s') ",
					gc_cChannID,
					gc_cChannNo,
					CPlantInfoBase::TimeConvertString(loTimeStart),
					gc_cStart_T,
					gc_cChannTable,
					gc_cCylinderNo,
					arefChanninfo._iCylinderNo,
					gc_cChannType,
					anChannType,
					gc_cChannNo,
					lstrChannNo
					);
			}else
			{

				acstrSql.Format(" SELECT [%s],[%s], '%s' as '%s' FROM %s WHERE ([%s] = %d) AND ([%s] = %d) AND ([%s] = %d)",
					gc_cChannID,
					gc_cChannNo,
					CPlantInfoBase::TimeConvertString(loTimeStart),
					gc_cStart_T,
					gc_cChannTable,
					gc_cCylinderNo,
					arefChanninfo._iCylinderNo,
					gc_cChannType,
					anChannType,
					gc_cMesurement_Direction,
					aiDirection
					);
			}

		}else
		{
			return FALSE;
		}

	}else
	{
		acstrSql.Format("   SELECT [%s], [%s], [%s], [%s],t2.[%s]  FROM %s WHERE (t1.[%s] = t2.[%s])  AND (t1.[%s] BETWEEN '%s' AND '%s')  AND (t1.[%s] = %d) AND (t2.[%s] = %d)  ORDER BY t1.[%s]",	
			gc_cEventID,
			gc_cName,
			gc_cStart_T,
			gc_cEnd_T,
			gc_cChannNo,
			lcstrTable,
			gc_cName,
			gc_cChannNo,
			gc_cStart_T,
			CPlantInfoBase::TimeConvertString(arefTimeStart),
			CPlantInfoBase::TimeConvertString(arefTimeEnd),
			gc_cChannType,
			anChannType,
			gc_cCylinderNo,
			arefChanninfo._iCylinderNo,
			gc_cStart_T
			);	
	}


	return TRUE;
}



/*  根据时间获取表类型
*	根据当前时间与要求时间两者之间的间隔
*	@param[in] time_
*  return 表类型 月，日，时，秒
*/
E_TREND_TBL_TYPE SqlHelperForExpert::GetTrendTblType(CBHDateTime  time_)
{
	COleDateTimeSpan span=CBHDateTime::GetCurrentTime()-time_;
	int iDays=span.GetTotalDays();
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;

	if (iDays>D_SECONDTREND_DAYS)
	{
		if (iDays>D_HOURTREND_DAYS)
		{
			if (iDays>D_DAYTREND_DAYS)
			{	
				iTblType=GE_TREND_TBLTYPE_MONTH;//月表范围
			}
			else
			{	
				iTblType=GE_TREND_TBLTYPE_DAY;//天表保存范围
			}
		}
		else
		{	
			iTblType=GE_TREND_TBLTYPE_HOUR;//小时表保存范围
		}
	}
	else
	{	
		iTblType=GE_TREND_TBLTYPE_SECOND;//秒表保存范围
	}

	return iTblType;
}

//得到给定测点所在缸撞击次数测点的sql
 BOOL SqlHelperForExpert::GetSqlForSetImpactTimes(
									IDBInterFace					& arefDB,
									CPlantInfoExpertSys				* apPlantInfo,	
									const S_ChannInfo				& arefChanninfo,
									CHANNTYPE						  anChannType,
									CBHDateTime					& arefTimeStart,
									CBHDateTime					& arefTimeEnd,	
									CString							& acstrSql)
{
	
	//1.得到测点所在缸的撞击测点的测点号
	if (!apPlantInfo)
	{
		return FALSE;
	}

	CString lstrDBName = SqlHelperForExpert::GetSpecPlantDBName(*apPlantInfo);
	SwitchDbNameEx(&arefDB,SqlHelperForExpert::GetMainDBName());
	
	CString lstrSql_Format = "SELECT [%s],[%s] from [%s] WHERE [%s] = %d";
	lstrSql_Format.Format(lstrSql_Format,
						  gc_cChannID,
						  gc_cChannNo,
						  gc_cChannTable,
						  gc_cCylinderNo,
						  arefChanninfo._iCylinderNo);	

	return FALSE;
}

CString SqlHelperForExpert::GetTblNamePrefixForTrend2(const CHANNTYPE &iChannType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_,bool & bSubTbl_,bool &bSupTbl_)
{
	/*	1、通过开始时间判断可以在那个表中取数据；
	2、判断是否可以跨表。
	如果可以跨表，则取的记录不够的可以跨表取。
	级别从高到低:月-天-小时-秒
	*/
	//判断可以从那级表中获得数据
	iTblType_=GetTrendTblType(timeStart_);//趋势表范围

	//判断是否可以在其下一级表中取数据
	if (GetTrendTblType(timeEnd_)==iTblType_)
		bSubTbl_=false;
	else
		bSubTbl_=true;

	//判断是否需要在其上级表取数据
	COleDateTimeSpan span=timeEnd_-timeStart_;
	int iHours=span.GetTotalHours();
	int iMinutes=span.GetTotalMinutes();

	bool bSupTblTmp=false;
GetTblNamePrefixForTrend2_JUDGE_TBLTYPE:
	bSupTblTmp=false;
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_SECOND://理论上1秒一条记录
		//前一个60表示1分钟有60条记录，后面这个60是通过秒趋势表和小时趋势表两者保存记录间隔差异得到
		//if ((iMinutes*60)>(60*D_TRENDRECORDS))
		//这里为了提高计算效率，把两个60都省略
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF))
			bSupTblTmp=true;
		break;
	case GE_TREND_TBLTYPE_HOUR:
		//20是通过小趋势表和天趋势表两者保存记录间隔差异得到
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF*20))
			bSupTblTmp=true;
		break;
	case GE_TREND_TBLTYPE_DAY:
		//系数3是因为天表中一小时保存3条记录；9是通过天趋势表（20分钟）和月趋势表（3小时）两者保存记录间隔差异得到
		if ((3*iHours)>(D_TRENDRECORDS*TRENDRECORDS_COEF*9))
			bSupTblTmp=true;
		break;
	default:
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF))
			bSupTblTmp=true;
	}
	//	CString strTmp;strTmp.Format("%d,%d,%d,%d",bSupTbl_,iTblType_,iMinutes,iHours);AfxMessageBox(strTmp);
	if (bSupTblTmp)
	{
		bSupTbl_=true;
		iTblType_=(E_TREND_TBL_TYPE)(iTblType_+1);
		if (iTblType_<GE_TREND_TBLTYPE_MONTH)
			goto GetTblNamePrefixForTrend2_JUDGE_TBLTYPE;
	}
	//	strTmp.Format("%d,%d",bSupTbl_,iTblType_);AfxMessageBox(strTmp);
	return GetTblNamePrefix(iChannType_,iTblType_);
}



CString SqlHelperForExpert::GetTblNamePrefixForTrend2Alarm(const CHANNTYPE &iChannType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_,bool & bSubTbl_,bool &bSupTbl_)
{
	/*	1、通过开始时间判断可以在那个表中取数据；
	2、判断是否可以跨表。
	如果可以跨表，则取的记录不够的可以跨表取。
	级别从高到低:月-天-小时-秒
	*/
	//判断可以从那级表中获得数据
	iTblType_=GetTrendTblType(timeStart_);//趋势表范围

	//判断是否可以在其下一级表中取数据
	if (GetTrendTblType(timeEnd_)==iTblType_)
		bSubTbl_=false;
	else
		bSubTbl_=true;

	//判断是否需要在其上级表取数据
	COleDateTimeSpan span=timeEnd_-timeStart_;
	int iHours=span.GetTotalHours();
	int iMinutes=span.GetTotalMinutes();

	bool bSupTblTmp=false;
GetTblNamePrefixForTrend2_JUDGE_TBLTYPE:
	bSupTblTmp=false;
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_SECOND://理论上1秒一条记录
		//前一个60表示1分钟有60条记录，后面这个60是通过秒趋势表和小时趋势表两者保存记录间隔差异得到
		//if ((iMinutes*60)>(60*D_TRENDRECORDS))
		//这里为了提高计算效率，把两个60都省略
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF))
			bSupTblTmp=true;
		break;
	case GE_TREND_TBLTYPE_HOUR:
		//20是通过小趋势表和天趋势表两者保存记录间隔差异得到
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF*20))
			bSupTblTmp=true;
		break;
	case GE_TREND_TBLTYPE_DAY:
		//系数3是因为天表中一小时保存3条记录；9是通过天趋势表（20分钟）和月趋势表（3小时）两者保存记录间隔差异得到
		if ((3*iHours)>(D_TRENDRECORDS*TRENDRECORDS_COEF*9))
			bSupTblTmp=true;
		break;
	default:
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF))
			bSupTblTmp=true;
	}
	//	CString strTmp;strTmp.Format("%d,%d,%d,%d",bSupTbl_,iTblType_,iMinutes,iHours);AfxMessageBox(strTmp);
	if (bSupTblTmp)
	{
		bSupTbl_=true;
		iTblType_=(E_TREND_TBL_TYPE)(iTblType_+1);
		if (iTblType_<GE_TREND_TBLTYPE_MONTH)
			goto GetTblNamePrefixForTrend2_JUDGE_TBLTYPE;
	}
	//	strTmp.Format("%d,%d",bSupTbl_,iTblType_);AfxMessageBox(strTmp);
	return GetTblNamePrefixForAlarm(iChannType_,iTblType_);
}

/*! 通过测点类型和表类型确定表前缀
\param[in]  iChannType_: 振动 动态 过程量
\param[in]  iTblType_:   月，日，时，秒
\return 表前缀
*/
CString SqlHelperForExpert::GetTblNamePrefix(const CHANNTYPE &iChannType_, E_TREND_TBL_TYPE &iTblType_/* =GE_TREND_TBLTYPE_SECOND */)
{
	CString strSuffix="";

	switch(GetChannOneType( iChannType_))
	{
	case E_TBL_CHANNTYPE_VIB:
		strSuffix="TREND_VIB";
		break;
	case E_TBL_CHANNTYPE_DYN:
		strSuffix="TREND_DYN";
		break;
	case E_TBL_CHANNTYPE_PROC:
		strSuffix="TREND_PROC";
		break;
	default:
		strSuffix="TREND_VIB";
	}
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_SECOND:
		strSuffix+="SECOND_";
		break;
	case GE_TREND_TBLTYPE_HOUR:
		strSuffix+="HOUR_";
		break;
	case GE_TREND_TBLTYPE_DAY:
		strSuffix+="DAY_";
		break;
	case GE_TREND_TBLTYPE_MONTH:
		strSuffix+="MONTH_";
		break;
	case GE_TREND_TBLTYPE_UNKNOWN:
	default:
		strSuffix+="SECOND_";
		iTblType_=GE_TREND_TBLTYPE_SECOND;
	}
	return strSuffix;
}


CString SqlHelperForExpert::GetTblNamePrefixForAlarm(const CHANNTYPE &iChannType_, E_TREND_TBL_TYPE &iTblType_/* =GE_TREND_TBLTYPE_SECOND */)
{
	CString strSuffix="";

	switch(GetChannOneType( iChannType_))
	{
	case E_TBL_CHANNTYPE_VIB:
		strSuffix="ALARM_VIB";
		break;
	case E_TBL_CHANNTYPE_DYN:
		strSuffix="ALARM_DYN";
		break;
	case E_TBL_CHANNTYPE_PROC:
		strSuffix="ALARM_PROC";
		break;
	default:
		strSuffix="ALARM_VIB";
	}
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_SECOND:
		strSuffix+="_";
		break;
	case GE_TREND_TBLTYPE_HOUR:
		strSuffix+="_";
		break;
	case GE_TREND_TBLTYPE_DAY:
		strSuffix+="_";
		break;
	case GE_TREND_TBLTYPE_MONTH:
		strSuffix+="_";
		break;
	case GE_TREND_TBLTYPE_UNKNOWN:
	default:
		strSuffix+="_";
		iTblType_=GE_TREND_TBLTYPE_SECOND;
	}

	return strSuffix;
}


E_TBL_CHANNTYPE SqlHelperForExpert::GetChannOneType(const CHANNTYPE &iChannType_)
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

bool SqlHelperForExpert::GetTrendRecords(IDBInterFace *pDB,
										 string sTblNameSuffix_,
										 string sPlantDbName,
										 string sShowField_,
										 const CHANNTYPE &iChannType_,
										 E_TREND_TBL_TYPE &iTblType_,
										 CBHDateTime& timeStart_,
										 CBHDateTime timeEnd_,
										 bool & bNeedSubTbl_)
{
	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd;
	SwitchDbNameEx(pDB,sPlantDbName.c_str());

	strTbl.Format("%s%s",GetTblNamePrefixForTrend2( iChannType_, timeStart_,timeEnd_,iTblType_,bNeedSubTbl_,bUseSupTbl),sTblNameSuffix_.c_str());
	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] BETWEEN '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

	int iNums=0;

	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	int iRealTblType=GetTrendTblType(timeStart_);

	bool bRead=false;//是否可以读取记录
	//判断记录数目是否满足要求的趋势记录数目。如果不满足，就判断是否使用的趋势表类型不是起始时间的趋势表类型而是它的上一级。
	{
REALTBLETYPE_READRECORDS:
		if (!(iTblType_<iRealTblType))
		{
			strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());
			pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
			pDB->Cmd(" WHERE %s ",strWhere);


			if (pDB->More()){
				iNums=pDB->GetCount();
			}
			else {
				iNums=0;

				// 防止超时或其它异常，
				if(pDB->GetLastError() < 0){

					bNeedSubTbl_ = false;
					return false;
				}
			}

			bNeedSubTbl_ = false;
			return (iNums > 0);

		}
	}

	return bRead;
}


bool SqlHelperForExpert::GetTrendRecordsForAlarm(IDBInterFace *pDB,
												string sTblNameSuffix_,
												string sPlantDbName,
												string sShowField_,
												const CHANNTYPE &iChannType_,
												E_TREND_TBL_TYPE &iTblType_,
												CBHDateTime& timeStart_,
												CBHDateTime timeEnd_,
												bool & bNeedSubTbl_)
{
	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd;
	SwitchDbNameEx(pDB,sPlantDbName.c_str());

	strTbl.Format("%s%s",GetTblNamePrefixForTrend2Alarm( iChannType_, timeStart_,timeEnd_,iTblType_,bNeedSubTbl_,bUseSupTbl),sTblNameSuffix_.c_str());
	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] BETWEEN '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

	int iNums=0;

	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	int iRealTblType=GetTrendTblType(timeStart_);

	bool bRead=false;//是否可以读取记录
	//判断记录数目是否满足要求的趋势记录数目。如果不满足，就判断是否使用的趋势表类型不是起始时间的趋势表类型而是它的上一级。
	{
REALTBLETYPE_READRECORDS:
		if (!(iTblType_<iRealTblType))
		{
			strTbl.Format("%s%s",GetTblNamePrefixForAlarm(iChannType_,iTblType_),sTblNameSuffix_.c_str());
			pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
			pDB->Cmd(" WHERE %s ",strWhere);


			if (pDB->More()){
				iNums=pDB->GetCount();
			}
			else {
				iNums=0;

				// 防止超时或其它异常，
				if(pDB->GetLastError() < 0){

					bNeedSubTbl_ = false;
					return false;
				}
			}

			bNeedSubTbl_ = false;
			return (iNums > 0);

		}
	}

	return bRead;
}

CString SqlHelperForExpert::GetMainDBName()
{
	return g_strMainDbName;
}

 CString SqlHelperForExpert::GetSpecPlantTableSuffix(CPlantInfoBase & arefPlantInfo)
{
	CString lstrDbName;

	lstrDbName.Format(_T("%s%s"),arefPlantInfo.GetCompanyAlias().c_str(),arefPlantInfo.GetPlantNo().c_str()); 

	return lstrDbName;
}

CString SqlHelperForExpert::GetSpecPlantDBName(CPlantInfoBase & arefPlantInfo)
{
	if (g_iDbType==1)
	{
		CString lstrDbName;

		lstrDbName.Format(_T("%s%s"),arefPlantInfo.GetCompanyAlias().c_str(),arefPlantInfo.GetPlantNo().c_str()); 

		return lstrDbName;

	}else
	{
		return g_strMainDbName;
	}
}


/*! 根据时间确定表类型，再根据测点类型和表类型获取表前缀
\param[in]  iChannType_
\param[in]  timeEnd_
\param[out] iTblType_
*/
CString SqlHelperForExpert::GetTblNamePrefixForWave2(const CHANNTYPE &iChannType_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_)
{
	iTblType_=GetTrendTblType(timeEnd_);
	return GetTblNamePrefix( iChannType_,iTblType_);
}

CString SqlHelperForExpert::GetTblNamePrefixForWave2ForAlarm(const CHANNTYPE &iChannType_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_)
{
	iTblType_=GetTrendTblType(timeEnd_);
	return GetTblNamePrefixForAlarm( iChannType_,iTblType_);
}

/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*/
int SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrumForAlarm(
																	IDBInterFace *pDB_, 
																	CPlantInfoExpertSys * apPlantInfo, 
																	std::vector<double> & aorefTrendData, 
																	string sCompany_, 
																	string sFactory_, 
																	string sPlant_, 
																	string sChann_, 
																	int iLines_, 
																	int iWaveNum_, 
																	CBHDateTime timeSpec_, 
																	int & aifreq,
																	BOOL abShouldReadSpectrum /* = FALSE */)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;


	CPlantInfoExpertSys *pPlantInfo = (CPlantInfoExpertSys*)apPlantInfo; /*CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_)*/

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);

	string sSuffix;//后缀
	sSuffix = GetSpecPlantDBName(*pPlantInfo);

	if (pPlantInfo == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SwitchDbNameEx(pDB_,sSuffix.c_str());

	if (iWaveNum_<100)
	{
/*		ASSERT(FALSE);*/
		iWaveNum_=513;
	}

	if((iLines_>6400)||(iLines_<400))
	{
		iLines_=400;
	}

	iRealWaveNum=iLines_*2.56;

	sSuffix = GetSpecPlantTableSuffix(*pPlantInfo);

	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannNoInfo( sChann_ );
	sPrefix=GetTblNamePrefixForWave2ForAlarm( GE_VIBCHANN,timeSpec_,iTableType);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsChannField(channInfo._cChannNo,(CHANNTYPE)channInfo._iChannType,true),
		ConsCommonField());

	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
		{
			ASSERT(FALSE);
			return -1;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			return -1;
		strTable.Format("%s%s",GetTblNamePrefixForAlarm( GE_VIBCHANN,iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	}

	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	
	aifreq = ifreq;
	//得到波形数据
	long isize;
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		pDtWave=new DATATYPE_WAVE[lnWavePtMax];//按照设备组态值开辟内存
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;

		strWave.Format("WAVE_%s",channInfo._cChannNo);

		//在数据库组态的设备采样点数和数据库中实际保存的采样点数中会取一个较小的值，拷贝到pDtWave中，并将isize设置为较小的值
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize,sWaveHead);

		inum=isize/sizeof(DATATYPE_WAVE);//

		for (int lnBufferIndex = 0;lnBufferIndex< inum;lnBufferIndex++)
		{
			aorefTrendData.push_back((double)(pDtWave[lnBufferIndex])/10);
		}
		if(inum<iRealWaveNum)
		{
			ASSERT(FALSE);
			iRealWaveNum=inum;
		}


		if (abShouldReadSpectrum)
		{
			ASSERT(FALSE);
			/*
			pDtfreq=new DATATYPE_WAVE[1+(iLines_*1.28)];

			iWaveNum_=iWaveNum_>inum?inum:iWaveNum_;

			iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,inum,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df);*/

		}


	}
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );


	return inum;
}



/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*/
int SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrum(
	IDBInterFace *pDB_, 
	CPlantInfoExpertSys * apPlantInfo, 
	std::vector<double> & aorefTrendData, 
	string sCompany_, 
	string sFactory_, 
	string sPlant_, 
	string sChann_, 
	int iLines_, 
	int iWaveNum_, 
	CBHDateTime timeSpec_, 
	int & aifreq,
	BOOL abShouldReadSpectrum /* = FALSE */)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;


	CPlantInfoExpertSys *pPlantInfo = (CPlantInfoExpertSys*)apPlantInfo; /*CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_)*/

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);

	string sSuffix;//后缀
	sSuffix = GetSpecPlantDBName(*pPlantInfo);

	if (pPlantInfo == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SwitchDbNameEx(pDB_,sSuffix.c_str());

	if (iWaveNum_<100)
	{
/*		ASSERT(FALSE);*/
		iWaveNum_=513;
	}

	if((iLines_>6400)||(iLines_<400))
	{
		iLines_=400;
	}

	iRealWaveNum=iLines_*2.56;

	sSuffix = GetSpecPlantTableSuffix(*pPlantInfo);

	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannNoInfo( sChann_ );
	sPrefix=GetTblNamePrefixForWave2( GE_VIBCHANN,timeSpec_,iTableType);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsChannField(channInfo._cChannNo,(CHANNTYPE)channInfo._iChannType,true),
		ConsCommonField());

	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
		{
			ASSERT(FALSE);
			return -1;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			return -1;
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	}

	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	
	aifreq = ifreq;
	//得到波形数据
	long isize;
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		pDtWave=new DATATYPE_WAVE[lnWavePtMax];//按照设备组态值开辟内存
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;

		strWave.Format("WAVE_%s",channInfo._cChannNo);

		//在数据库组态的设备采样点数和数据库中实际保存的采样点数中会取一个较小的值，拷贝到pDtWave中，并将isize设置为较小的值
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize, sWaveHead);

		inum=isize/sizeof(DATATYPE_WAVE);//

		for (int lnBufferIndex = 0;lnBufferIndex< inum;lnBufferIndex++)
		{
			aorefTrendData.push_back((double)(pDtWave[lnBufferIndex])/10);
		}
		if(inum<iRealWaveNum)
		{
			ASSERT(FALSE);
			iRealWaveNum=inum;
		}


		if (abShouldReadSpectrum)
		{
			ASSERT(FALSE);
			/*
			pDtfreq=new DATATYPE_WAVE[1+(iLines_*1.28)];

			iWaveNum_=iWaveNum_>inum?inum:iWaveNum_;

			iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,inum,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df);*/

		}


	}
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );


	return inum;
}


/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*/
int SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrum(
	IDBInterFace *pDB_, 
	CPlantInfoExpertSys * apPlantInfo, 
	std::vector<double> & aorefTrendData, 
	string sCompany_, 
	string sFactory_, 
	string sPlant_, 
	string sChann_, 
	int iLines_, 
	int iWaveNum_, 
	CBHDateTime timeSpec_, 
	int & aifreq,
	BOOL abShouldReadSpectrum /* = FALSE */)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;


	CPlantInfoExpertSys *pPlantInfo = (CPlantInfoExpertSys*)apPlantInfo; /*CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_)*/

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);

	string sSuffix;//后缀
	sSuffix = GetSpecPlantDBName(*pPlantInfo);

	if (pPlantInfo == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SwitchDbNameEx(pDB_,sSuffix.c_str());

	if (iWaveNum_<100)
	{
/*
		ASSERT(FALSE);
*/
		iWaveNum_=513;
	}

	if((iLines_>6400)||(iLines_<400))
	{
		iLines_=400;
	}

	iRealWaveNum=iLines_*2.56;

	sSuffix = GetSpecPlantTableSuffix(*pPlantInfo);

	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecDynChannNoInfo( sChann_ );
	sPrefix=GetTblNamePrefixForWave2((CHANNTYPE)channInfo._iChannType,timeSpec_,iTableType);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsChannField(channInfo._cChannNo,(CHANNTYPE)channInfo._iChannType,true),
		ConsCommonField());

	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
		{
			ASSERT(FALSE);
			return -1;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			return -1;
		strTable.Format("%s%s",GetTblNamePrefix((CHANNTYPE) channInfo._iChannType,iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	}

	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	
	aifreq = ifreq;
	//得到波形数据
	long isize;
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		pDtWave=new DATATYPE_WAVE[lnWavePtMax];//按照设备组态值开辟内存
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;

		strWave.Format("WAVE_%s",channInfo._cChannNo);

		//在数据库组态的设备采样点数和数据库中实际保存的采样点数中会取一个较小的值，拷贝到pDtWave中，并将isize设置为较小的值
		CWaveHead sWaveHead;

		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize, sWaveHead);

		inum=isize/sizeof(DATATYPE_WAVE);//

		for (int lnBufferIndex = 0;lnBufferIndex< inum;lnBufferIndex++)
		{
			aorefTrendData.push_back((double)(pDtWave[lnBufferIndex])/10);
		}
		if(inum<iRealWaveNum)
		{
			ASSERT(FALSE);
			iRealWaveNum=inum;
		}


		if (abShouldReadSpectrum)
		{
			ASSERT(FALSE);
			/*
			pDtfreq=new DATATYPE_WAVE[1+(iLines_*1.28)];

			iWaveNum_=iWaveNum_>inum?inum:iWaveNum_;

			iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,inum,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df);*/

		}


	}
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );
	return inum;
}



/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*/
int SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrumForAlarm(
	IDBInterFace *pDB_, 
	CPlantInfoExpertSys * apPlantInfo, 
	std::vector<double> & aorefTrendData, 
	string sCompany_, 
	string sFactory_, 
	string sPlant_, 
	string sChann_, 
	int iLines_, 
	int iWaveNum_, 
	CBHDateTime timeSpec_, 
	int & aifreq,
	BOOL abShouldReadSpectrum /* = FALSE */)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;


	CPlantInfoExpertSys *pPlantInfo = (CPlantInfoExpertSys*)apPlantInfo; /*CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_)*/

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);

	string sSuffix;//后缀
	sSuffix = GetSpecPlantDBName(*pPlantInfo);

	if (pPlantInfo == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SwitchDbNameEx(pDB_,sSuffix.c_str());

	if (iWaveNum_<100)
	{
/*
		ASSERT(FALSE);
*/
		iWaveNum_=513;
	}

	if((iLines_>6400)||(iLines_<400))
	{
		iLines_=400;
	}

	iRealWaveNum=iLines_*2.56;

	sSuffix = GetSpecPlantTableSuffix(*pPlantInfo);

	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecDynChannNoInfo( sChann_ );
	BOOL lbSub = FALSE;
	sPrefix=GetTblNamePrefixForWave2ForAlarm((CHANNTYPE)channInfo._iChannType,timeSpec_,iTableType);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsChannField(channInfo._cChannNo,(CHANNTYPE)channInfo._iChannType,true),
		ConsCommonField());

	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
		{
			ASSERT(FALSE);
			return -1;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			return -1;
		strTable.Format("%s%s",GetTblNamePrefixForAlarm((CHANNTYPE) channInfo._iChannType,iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	}

	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	
	aifreq = ifreq;
	//得到波形数据
	long isize;
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		pDtWave=new DATATYPE_WAVE[lnWavePtMax];//按照设备组态值开辟内存
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;

		strWave.Format("WAVE_%s",channInfo._cChannNo);

		//在数据库组态的设备采样点数和数据库中实际保存的采样点数中会取一个较小的值，拷贝到pDtWave中，并将isize设置为较小的值
		
		CWaveHead sWaveHead;

		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize,sWaveHead);

		inum=isize/sizeof(DATATYPE_WAVE);//

		for (int lnBufferIndex = 0;lnBufferIndex< inum;lnBufferIndex++)
		{
			aorefTrendData.push_back((double)(pDtWave[lnBufferIndex])/10);
		}
		if(inum<iRealWaveNum)
		{
			ASSERT(FALSE);
			iRealWaveNum=inum;
		}


		if (abShouldReadSpectrum)
		{
			ASSERT(FALSE);
			/*
			pDtfreq=new DATATYPE_WAVE[1+(iLines_*1.28)];

			iWaveNum_=iWaveNum_>inum?inum:iWaveNum_;

			iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,inum,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df);*/

		}


	}
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );
	return inum;
}

BOOL SqlHelperForExpert::GetCylinderValueAlarmsAndSensorFaults(
	IDBInterFace					& arefDB,
	ICommInterFace					& arefFixComm,
	const CPlantInfoExpertSys		* apPlantInfo,
	const S_ChannInfo				& arefChanninfo,
	CBHDateTime					& aoletimeStart,
	CBHDateTime					& aoletimeEnd,
	unsigned int					& aiSensorFaults_, 		
	unsigned int					& aiAlarmChannels_, 	
	int								& aiChannels_,
	CString							& astrSql)
{
	return FALSE;
}



CString SqlHelperForExpert::ConsCommonField()
{
	CString sFields;
	sFields.Format("%s,%s,%s,%s,%s,%s,%s",
		gc_cSampleNum,
		gc_cSampleFreq,
		gc_cRev,
		gc_cRev2,
		gc_cRev3,
		gc_cRev4,
		gc_cRev5);
	return sFields;
}

CString SqlHelperForExpert::ConsChannField( string sChann_,CHANNTYPE anType,bool bWave_/*=false*/ )
{
	switch(anType)
	{
	case	GE_VIBCHANN:
	case    GE_AXIALCHANN:
		return SqlHelperForExpert::ConsVibChannField(sChann_,bWave_);
		break;
	case    GE_PRESSCHANN :
	case	GE_DYNPRESSCHANN:   
	case	GE_RODSINKCHANN:    
	case	GE_DYNSTRESSCHANN:
		return SqlHelperForExpert::ConsDynChannField(sChann_,bWave_);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return _T("");
}

SqlHelperForExpert::CHAN_CAT SqlHelperForExpert::GetCharCat(CHANNTYPE anChannType)
{
	switch(anChannType)
	{
	case	GE_VIBCHANN:
	case    GE_AXIALCHANN:
		return SqlHelperForExpert::CHAN_VIB;
		break;
	case    GE_PRESSCHANN :
	case	GE_DYNPRESSCHANN:   
	case	GE_RODSINKCHANN:    
	case	GE_DYNSTRESSCHANN:
		return SqlHelperForExpert::CHAN_DYN;
		break;
	default:
		ASSERT(FALSE);
		return SqlHelperForExpert::CHAN_PROC;
		break;
	}
}

CString SqlHelperForExpert::ConsVibChannField(string sChann_,bool bWave_)
{
	CString strRtn="";
	if (bWave_)
		strRtn.Format("[ALL_%s],[ONE_%s],[TWO_%s],[THREE_%s],[HALF_%s],[RES_%s],[ONEPHASE_%s],[TWOPHASE_%s],[THREEPHASE_%s],[HALFPHASE_%s],[DC_%s],[GAP_%s],[WAVE_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s], 0) AS [SAMPLE_NUM_%s], ISNULL([REV_%s], 0) AS [REV_%s]",
		sChann_.c_str(),sChann_.c_str(), sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	else
		strRtn.Format("[ALL_%s],[ONE_%s],[TWO_%s],[THREE_%s],[HALF_%s],[RES_%s],[ONEPHASE_%s],[TWOPHASE_%s],[THREEPHASE_%s],[HALFPHASE_%s],[DC_%s],[GAP_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s],0) AS [SAMPLE_NUM_%s],ISNULL([REV_%s], 0) AS [REV_%s]",
		sChann_.c_str(),sChann_.c_str(), sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	return strRtn;
}

CString SqlHelperForExpert::ConsDynChannField(string sChann_,bool bWave_/* =false */)
{
	CString strRtn="";
	if (bWave_)
		strRtn.Format("[ZERO_%s],[ONE_%s],[TWO_%s],[THREE_%s],[FOUR_%s],[FIVE_%s],[SIX_%s],[SEVEN_%s],[EIGHT_%s],[NINE_%s],[WAVE_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s], 0) AS [SAMPLE_NUM_%s], ISNULL([REV_%s], 0) AS [REV_%s],ISNULL([PERIOD_START_INDEX_%s],0) as [PERIOD_START_INDEX_%s],ISNULL([PERIOD_END_INDEX_%s],0) as [PERIOD_END_INDEX_%s]",
		sChann_.c_str(),sChann_.c_str(), sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	else
		strRtn.Format("[ZERO_%s],[ONE_%s],[TWO_%s],[THREE_%s],[FOUR_%s],[FIVE_%s],[SIX_%s],[SEVEN_%s],[EIGHT_%s],[NINE_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s], 0) AS [SAMPLE_NUM_%s], ISNULL([REV_%s], 0) AS [REV_%s],ISNULL([PERIOD_START_INDEX_%s],0) as [PERIOD_START_INDEX_%s],ISNULL([PERIOD_END_INDEX_%s],0) as [PERIOD_END_INDEX_%s]",
		sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	return strRtn;
}

//从指定数据连接中读取指定动态测点所有类型特征值
S_DynCharValue SqlHelperForExpert::ReadDynCharValue(string sChann_,IDBInterFace *pDB_)
{
	CString strTmp;
	S_DynCharValue vibValue;
	strTmp.Format("ZERO_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),0);
	strTmp.Format("ONE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),1);
	strTmp.Format("TWO_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),2);
	strTmp.Format("THREE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),3);
	strTmp.Format("FOUR_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),4);
	strTmp.Format("FIVE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),5);
	strTmp.Format("SIX_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),6);
	strTmp.Format("SEVEN_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),7);
	strTmp.Format("EIGHT_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),8);
	strTmp.Format("NINE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),9);
	strTmp.Format("PERIOD_START_INDEX_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),10);
	strTmp.Format("PERIOD_END_INDEX_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),11);
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_PHASECHARHALF);
	vibValue.SetSpecTypeData(SimADO_GetSampleFrequencyEx(pDB_, sChann_), GE_SAMPLEFREQ);
	vibValue.SetSpecTypeData(SimADO_GetSampleNumEx(pDB_, sChann_), GE_SAMPLENUM);
	vibValue.SetSpecTypeData(SimADO_GetRevEx(pDB_, sChann_), GE_REV);
	return vibValue;
}


S_VibCharValue SqlHelperForExpert::ReadVibCharValue(string sChann_,IDBInterFace *pDB_)
{
	CString strTmp;
	S_VibCharValue vibValue;
	strTmp.Format("ALL_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_ALLCHAR);
	strTmp.Format("ONE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_ONECHAR);
	strTmp.Format("TWO_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_TWOCHAR);
	strTmp.Format("THREE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_THREECHAR);
	strTmp.Format("HALF_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_HALFCHAR);
	strTmp.Format("DC_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_DCCHAR);
	strTmp.Format("GAP_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_GAP);
	strTmp.Format("RES_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_RESCHAR);
	strTmp.Format("ONEPHASE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_PHASECHARONE);
	strTmp.Format("TWOPHASE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_PHASECHARTWO);
	strTmp.Format("THREEPHASE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_PHASECHARTHREE);
	strTmp.Format("HALFPHASE_%s",sChann_.c_str());
	vibValue.SetSpecTypeData(pDB_->GetDouble(strTmp),GE_PHASECHARHALF);
	vibValue.SetSpecTypeData(SimADO_GetSampleFrequencyEx(pDB_, sChann_),GE_SAMPLEFREQ);
	vibValue.SetSpecTypeData(SimADO_GetSampleNumEx(pDB_, sChann_),GE_SAMPLENUM);
	vibValue.SetSpecTypeData(SimADO_GetRevEx(pDB_, sChann_),GE_REV);
	return vibValue;
}
