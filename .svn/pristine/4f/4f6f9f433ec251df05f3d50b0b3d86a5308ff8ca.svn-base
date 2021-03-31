#include "stdafx.h"
#include <algorithm>
#include "implement.h"
#include "SqlBuilderForExpert.h"
#include <fstream>
#include <MiddleWare.h>
#include <additions/DebugHelper.h>
#include "BHChannValueJudgeValidHelper.h"
#include "BusinessObject.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/BHCommWrapper.h>


typedef struct _SqlParaStruct
{
	CString			m_strName;
	CBHDateTime	m_oDateTimeStart;
	CBHDateTime	m_oDateTimeEnd;
	CHANNTYPE		m_ChannType;

} SqlParaStruct, *p_SqlParaStruct;

CImplement::CImplement(void)
{
}

CImplement::~CImplement(void)
{
}

/**得到指定报警ID时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             报警ID     FIELD_SERVICE_ALARMID
*             测点ID     FIELD_SERVICE_CHANN1
*             时间       FIELD_SERVICE_OFF_TIME
*返回的字段码：
*             报警通道   FIELD_SERVICE_OFF_CHANN1
*             工作转速   FIELD_SERVICE_OFF_REV
*             事实ID     FIELD_SERVICE_EX_RULEID           //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE          //对应为int形
#define    SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT      30001
*/
int CImplement::ExpertSysGetRegAlarmFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *apDB)
{
	int nCount=0;
	string sCompany,sFactory,sPlant,sAlias,sTable,sChannName,sChann="";
	string sAxisLoction,sDirection,sChannHor,sChannVer;
	int iAlarmID,iLeadFreq=-1,iAccomFreq=-1,iOrbitShape=-1,iPrecession=-1;
	float fRev = .0;
	S_ChannInfo channInfo;
	CPlantInfoExpertSys *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;

	S_PlantInfo loInfo = pPlantInfo->GetPlantInfo();

	if(loInfo._iMachineType == GE_MACHINETYPE_RC || loInfo._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		loInfo._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || loInfo._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR ||
		loInfo._iMachineType == GE_MACHINETYPE_ICE || loInfo._iMachineType == GE_MACHINETYPE_DISEL )
	{
		CBHCommWrapper loCommWrapper;
		loCommWrapper.SetFixComm(pFixComm->GetFixComm());

		COleSqlServerDB loDbWrapper;
		loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

		CBODiagnosisRC loBoRc;

		return loBoRc.Do(loCommWrapper,loDbWrapper);

	}else if (loInfo._iMachineType == GE_MACHINETYPE_KEYPUMP
			|| loInfo._iMachineType == GE_MACHINETYPE_WINDPEQ)
	{

		CBHCommWrapper loCommWrapper;
		loCommWrapper.SetFixComm(pFixComm->GetFixComm());

		COleSqlServerDB loDbWrapper;
		loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

		CBODiagnosisW loBoW;

		return loBoW.Do(loCommWrapper,loDbWrapper);
	}
	
	COleSqlServerDB loDbWrapper;

	loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

	IDBInterFace *pDB = &loDbWrapper ;
	
	SwitchDbNameEx(pDB,sAlias.c_str());

	iAlarmID = pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	sTable = pPlantInfo->GetAlarmLogTblName();
	CString sTmp;sTmp.Format("%s,%s,%s,%s,%d",sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),sAlias.c_str(),iAlarmID);
	//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetRegAlarmFeatureExtract");
	if (sChann=="")
	{
		//如果得到通道名为空，则根据查询通频最大值的对应特征值和波形进行诊断
		pDB->Cmd(" select top 1 [%s],[%s] from [%s] ",gc_cName,gc_cStart_T,sTable.c_str());
		pDB->Cmd(" where [%s]=%d and [%s]=%d ",gc_cEventID,iAlarmID,gc_cChannType,GE_VIBCHANN);
		pDB->Cmd(" order by [%s],[%s] desc ",gc_cEventID,gc_cName);
		if (pDB->More())
		{
			sChannName = pDB->Get(gc_cName);
		}
		channInfo = pPlantInfo->GetSpecVibChannNoInfo(sChannName);
	}
	else
	{
		//如果得到了通道名，则根据查询该通道对应时间点的特征值和波形进行诊断
		channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
		sChannName=channInfo._cChannNo;
	}
	if (!(sChannName.empty()))
	{//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' OR [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),gc_cHorChann,sChannName.c_str(),gc_cVerChann,sChannName.c_str());
		if (pDB->More())
		{
			sChannHor=pDB->Get(gc_cHorChann);
			sChannVer=pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}

	sTable = "ALARM_VIB_";
	sTable.append(sAlias.c_str());
	SwitchDbNameEx(pDB,sAlias.c_str());
	CString strShowField;
	DATATYPE_WAVE * pDtWaveHor=NULL;
	DATATYPE_WAVE * pDtWaveVer=NULL;
	double *pfWaveHor=NULL;
	double *pfWaveVer=NULL;
	strShowField.Format("%s,[ALL_%s],[WAVE_%s],[WAVE_%s],%s",
		ConsVibChannField(sChannName.c_str(),false),
		sChannName.c_str(),
		sChannHor.c_str(),
		sChannVer.c_str(),
		ConsCommonField());
	pDB->Cmd(" select %s from [%s] ",strShowField,sTable.c_str());
	if (sChann=="")
	{
		pDB->Cmd(" where [ALL_%s] = (select isnull(max([ALL_%s]),0) from %s where [%s] = %d) and [%s] = %d",
			sChannName.c_str(),sChannName.c_str(),sTable.c_str(),gc_cEventID,iAlarmID,gc_cEventID,iAlarmID);
	}
	else
	{
		pDB->Cmd("where [%s]='%s'",
			gc_cTime,CPlantInfoBase::TimeConvertString(timeOff));
	}

	//CHZLogManage::Debug(pDB->GetSql(),"ExpertSys","ExpertSysGetRegAlarmFeatureExtract");
	if (pDB->More())
	{
		S_VibCharValue vibCharValue;
		int inum     = SimADO_GetSampleNumEx(pDB, sChannHor);
		int iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannHor);
		switch(channInfo._iRevType)
		{
		case 1:
			fRev = pDB->GetDouble( gc_cRev);
			break;
		case 2:
			fRev = pDB->GetDouble( gc_cRev2 );
			break;
		case 3:
			fRev = pDB->GetDouble( gc_cRev3);
			break;
		case 4:
			fRev = pDB->GetDouble( gc_cRev4);
			break;
		case 5:
			fRev = pDB->GetDouble( gc_cRev5);
			break;
		default:
			fRev = pDB->GetDouble( gc_cRev);
		}
		pDtWaveHor=new DATATYPE_WAVE[inum];
		memset(pDtWaveHor,0,inum*sizeof(DATATYPE_WAVE));
		pDtWaveVer=new DATATYPE_WAVE[inum];
		memset(pDtWaveVer,0,inum*sizeof(DATATYPE_WAVE));
		if (inum>1)
		{//为了防止采样点数为0的情况下出现异常。
			vibCharValue=ReadVibCharValue(sChannName,pDB);
			long isize=inum * sizeof(DATATYPE_WAVE);
			CString strWaveHor,strWaveVer;
			strWaveHor.Format("WAVE_%s",sChannHor.c_str());
			strWaveVer.Format("WAVE_%s",sChannVer.c_str());
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, strWaveHor,  (BYTE *)pDtWaveHor,  isize, sWaveHead);
			SimADO_SafeGetBufferEx(pDB, strWaveVer,  (BYTE *)pDtWaveVer,  isize, sWaveHead);
			inum=isize/sizeof(DATATYPE_WAVE);
			if (inum>GD_MIDDLEWARE_WAVE_POINT)
				inum=GD_MIDDLEWARE_WAVE_POINT;
			//writefile(pDtWaveHor,inum,1);
			//writefile(pDtWaveVer,inum,2);
			pfWaveHor=new double[inum];
			memset(pfWaveHor,0,inum*sizeof(double));
			pfWaveVer=new double[inum];
			memset(pfWaveVer,0,inum*sizeof(double));
			pPlantInfo->ConvertDTWaveToDouble(pfWaveHor,pDtWaveHor,inum,sWaveHead);
			pPlantInfo->ConvertDTWaveToDouble(pfWaveVer,pDtWaveVer,inum,sWaveHead);
			//writefile(pfWaveHor,inum,11);
			//writefile(pfWaveVer,inum,22);
			double dWorkFreq=(double)fRev/60;
			CSynopsis2KB calcOrbitAndFreq;
			//计算轴心轨迹形状和进动方向
			int icalcOrbit=-1;
			/*
			writefile(pfWaveVer,inum,1111);
			writefile(pfWaveHor,inum,2222);*/

			if (dWorkFreq>.0)
			{
				try
				{
					icalcOrbit = calcOrbitAndFreq.GetOrbitShape(pfWaveVer,pfWaveHor,inum,dWorkFreq,iSmpFreq,iPrecession,iOrbitShape);
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
			if (icalcOrbit < 0)
				CHZLogManage::Error("计算轴心轨迹形状出错","ExpertSys","ExpertSysGetRegAlarmFeatureExtract");

			//sTmp.Format("报警诊断:sChannName:%s,iOrbitShape:%d,iPrecession:%d,icalcOrbit:%d",sChannName.c_str(),iOrbitShape,iPrecession,icalcOrbit);BHTRACE_DEBUG(sTmp);
			//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetRegAlarmFeatureExtract");

			int icalcFreq = -1;
			//计算主导频率和常伴频率
			/*			writefile(pfWaveVer,inum,1111);*/
			if (dWorkFreq>.0)
			{
				try
				{
					if (strcmp(sChannName.c_str(),sChannHor.c_str())==0)
					{
						icalcFreq = calcOrbitAndFreq.GetLeadFreqAndAccomFreq(pfWaveHor,inum,iSmpFreq,dWorkFreq,iLeadFreq,iAccomFreq,25,3);
					}
					else
					{
						icalcFreq = calcOrbitAndFreq.GetLeadFreqAndAccomFreq(pfWaveVer,inum,iSmpFreq,dWorkFreq,iLeadFreq,iAccomFreq,25,3);
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

			if (icalcFreq < 0)
				CHZLogManage::Error("计算主导和常伴频率出错","ExpertSys","ExpertSysGetRegAlarmFeatureExtract");
			//sTmp.Format("报警诊断:icalcFreq:%d,iLeadFreq:%d,iAccomFreq:%d",icalcFreq,iLeadFreq,iAccomFreq);BHTRACE_DEBUG(sTmp);
			//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetRegAlarmFeatureExtract");
		}
	}
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//报警测点
	if (channInfo._cChannID!=NULL)
	{
		int iWorkRev=(int)fRev;
		pFixComm->SetItem(FIELD_SERVICE_OFF_CHANN1,channInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,iWorkRev);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	//主导频率
	if (iLeadFreq>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-ActLeadF",iLeadFreq,pFixComm);
	}
	//其他频率
	if (iAccomFreq>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-ElseFreq",iAccomFreq,pFixComm);
	}
	//轴心轨迹形状
	if (iOrbitShape>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-AxisType",iOrbitShape,pFixComm);
	}
	//进动方向
	/*
	if (iPrecession>-1)
	{
	nCount++;
	WriteFertureInfo("S-RM-Precessi",iPrecession,pFixComm);
	}
	*/
	//机组信息
	WriteMachineFertureInfo(pPlantInfo,pFixComm);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pDtWaveHor);
	KD_DeletePoint(pDtWaveVer);
	KD_DeletePoint(pfWaveHor);
	KD_DeletePoint(pfWaveVer);
	return nCount;
}

/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             测点ID     FIELD_SERVICE_CHANN1
*             起始时间   FIELD_SERVICE_OFF_TIMESTART
*             终止时间   FIELD_SERVICE_OFF_TIMEEND
*返回的字段码：
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT      30002
*/
int CImplement::ExpertSysGetSpecChannFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *apDB)
{
	int nCount=0;
	string sCompany,sFactory,sPlant,sAlias,sTable,sChannName;
	string sAxisLoction,sDirection,sChannHor,sChannVer;
	int iLeadFreq=-1,iAccomFreq=-1,iOrbitShape=-1,iPrecession=-1;
	CPlantInfoExpertSys *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
	{	
		return -1;
	}
	S_PlantInfo loInfo = pPlantInfo->GetPlantInfo();

	if(loInfo._iMachineType == GE_MACHINETYPE_RC || loInfo._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		loInfo._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || loInfo._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR || 
		loInfo._iMachineType == GE_MACHINETYPE_ICE || loInfo._iMachineType == GE_MACHINETYPE_DISEL )
	{
		CBHCommWrapper loCommWrapper;
		loCommWrapper.SetFixComm(pFixComm->GetFixComm());

		COleSqlServerDB loDbWrapper;
		loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

		CBODiagnosisRC loBoRc;

		return loBoRc.Do(loCommWrapper,loDbWrapper);

		
	}else if (loInfo._iMachineType == GE_MACHINETYPE_KEYPUMP
		|| loInfo._iMachineType == GE_MACHINETYPE_WINDPEQ)
	{

		CBHCommWrapper loCommWrapper;
		loCommWrapper.SetFixComm(pFixComm->GetFixComm());

		COleSqlServerDB loDbWrapper;
		loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

		CBODiagnosisW loBoW;

		return loBoW.Do(loCommWrapper,loDbWrapper);
	}


	COleSqlServerDB loDbWrapper;

	loDbWrapper.setPDB((CCustomDB *)apDB->getPDB());

	IDBInterFace *pDB = &loDbWrapper ;

	sChannName=pFixComm->Get(FIELD_SERVICE_CHANN1);

	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChannName);
	string sChann=channInfo._cChannNo;
	CString sTmp;sTmp.Format("%s,%s,%s,%s,%s,%s",sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),sAlias.c_str(),sChann.c_str(),sChannName.c_str());
	//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetSpecChannFeatureExtract");
	if (!(sChann.empty()))
	{//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' OR [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),gc_cHorChann,sChann.c_str(),gc_cVerChann,sChann.c_str());
		TRACE("获取测点%s %s@%d\n", pDB->GetSql(), __FUNCTION__, __LINE__);
		if (pDB->More())
		{
			sChannHor=pDB->Get(gc_cHorChann);
			sChannVer=pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}

	SwitchDbNameEx(pDB,sAlias.c_str());
	
	sTable = "TREND_VIBSECOND_";
	sTable.append(sAlias.c_str());
	CString strShowField;
	DATATYPE_WAVE * pDtWaveHor=NULL;
	DATATYPE_WAVE * pDtWaveVer=NULL;
	double *pfWaveHor=NULL;
	double *pfWaveVer=NULL;
	strShowField.Format("%s,[ALL_%s],[WAVE_%s],[WAVE_%s],%s",
						ConsVibChannField(sChann.c_str(),false),
						sChann.c_str(),sChannHor.c_str(),sChannVer.c_str(),
						ConsCommonField());
	pDB->Cmd(" select %s from [%s] ",strShowField,sTable.c_str());
	if (timeStart==timeEnd)
	{
		//如果得到的起止时间相等，则根据查询对应时间点的特征值和波形进行诊断
		pDB->Cmd("where [%s]='%s'",gc_cTime,CPlantInfoBase::TimeConvertString(timeStart));
	}
	else
	{
		//如果得到的起止时间不等，则根据查询通频最大值的对应特征值和波形进行诊断
		//pDB->Cmd(" where [ALL_%s] = (select isnull(max([ALL_%s]),0) from %s where [%s] between '%s' and '%s') and [%s] between '%s' and '%s' ",sChann.c_str(),sChann.c_str(),sTable.c_str(),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));

		CString lstrSqlLast = pDB->GetSql();

		pDB->ClearSql();

		pDB->Cmd("select isnull(max([ALL_%s]),0) as maxdata from %s where [%s] between '%s' and '%s'",
				sChann.c_str(),
				sTable.c_str(),
				gc_cTime,
				CPlantInfoBase::TimeConvertString(timeStart), 
				CPlantInfoBase::TimeConvertString(timeEnd));

		double ldbData = 0;

		if (pDB->More())
		{
			ldbData = pDB->GetDouble("maxdata");

		}

		pDB->Cmd(lstrSqlLast);

		pDB->Cmd(" where [ALL_%s] = (%f) and [%s] between '%s' and '%s' ",sChann.c_str(),ldbData,gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));

		//如果得到的起止时间不等，则根据查询通频最大值的对应特征值和波形进行诊断
		//pDB->Cmd(" where [ALL_%s] = (select isnull(max([ALL_%s]),0) from %s where [%s] between '%s' and '%s') and [%s] between '%s' and '%s' ",sChann.c_str(),sChann.c_str(),sTable.c_str(),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
	}
	//CHZLogManage::Debug(pDB->GetSql(),"ExpertSys","ExpertSysGetSpecChannFeatureExtract");
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_MONTH;
	TRACE("获取数据%s %s@%d\n", pDB->GetSql(), __FUNCTION__, __LINE__);
	BOOL lbDataGet = TRUE;
	while(!(pDB->More()))
	{
		if (pDB->GetLastError() < 0)
		{	
			lbDataGet = FALSE;
			break;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
		{	
			lbDataGet = FALSE;
			break;
		}
		CString strTable;
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sAlias.c_str());
		pDB->Cmd(" select %s from [%s] ",strShowField, strTable);
		if (timeStart==timeEnd)
		{
			//如果得到的起止时间相等，则根据查询对应时间点的特征值和波形进行诊断
			pDB->Cmd("where [%s]='%s'",gc_cTime,CPlantInfoBase::TimeConvertString(timeStart));
		}
		else
		{
			//如果得到的起止时间不等，则根据查询通频最大值的对应特征值和波形进行诊断
			//pDB->Cmd(" where [ALL_%s] = (select isnull(max([ALL_%s]),0) from %s where [%s] between '%s' and '%s') and [%s] between '%s' and '%s' ",sChann.c_str(),sChann.c_str(),sTable.c_str(),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd),gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
			CString lstrSqlLast = pDB->GetSql();

			pDB->ClearSql();

			pDB->Cmd("select isnull(max([ALL_%s]),0) as maxdata from %s where [%s] between '%s' and '%s'",
					 sChann.c_str(),
					 sTable.c_str(),
					 gc_cTime,
					 CPlantInfoBase::TimeConvertString(timeStart), 
					 CPlantInfoBase::TimeConvertString(timeEnd));

			double ldbData = 0;

			if (pDB->More())
			{
				ldbData = pDB->GetDouble("maxdata");

			}

			pDB->Cmd(lstrSqlLast);

			pDB->Cmd(" where [ALL_%s] = (%f) and [%s] between '%s' and '%s' ",sChann.c_str(),ldbData,gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));

		}
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);

		//主导频率
		if (iLeadFreq>-1)
		{
			nCount++;
			WriteFertureInfo("S-RM-ActLeadF",iLeadFreq,pFixComm);
		}
		//其他频率
		if (iAccomFreq>-1)
		{
			nCount++;
			WriteFertureInfo("S-RM-ElseFreq",iAccomFreq,pFixComm);
		}
		//轴心轨迹形状
		if (iOrbitShape>-1)
		{
			nCount++;
			WriteFertureInfo("S-RM-AxisType",iOrbitShape,pFixComm);
		}
	}

	//常规数据表中查不到，则查询报警表
	if (!lbDataGet)
	{
		sTable = "ALARM_VIB_";
		sTable.append(sAlias.c_str());

		strShowField.Format("%s,[ALL_%s],[WAVE_%s],[WAVE_%s],%s",
			ConsVibChannField(sChann.c_str(),false),
			sChann.c_str(),sChannHor.c_str(),sChannVer.c_str(),
			ConsCommonField());
		pDB->Cmd(" select %s from [%s] ",strShowField,sTable.c_str());
		if (timeStart==timeEnd)
		{
			//如果得到的起止时间相等，则根据查询对应时间点的特征值和波形进行诊断
			pDB->Cmd("where [%s]='%s'",gc_cTime,CPlantInfoBase::TimeConvertString(timeStart));
		}	
		else
		{
			CString lstrSqlLast = pDB->GetSql();

			pDB->ClearSql();

			pDB->Cmd("select isnull(max([ALL_%s]),0) as maxdata from %s where [%s] between '%s' and '%s'",
				sChann.c_str(),
				sTable.c_str(),
				gc_cTime,
				CPlantInfoBase::TimeConvertString(timeStart), 
				CPlantInfoBase::TimeConvertString(timeEnd));

			double ldbData = 0;

			if (pDB->More())
			{
				ldbData = pDB->GetDouble("maxdata");

			}

			pDB->Cmd(lstrSqlLast);

			pDB->Cmd(" where [ALL_%s] = (%f) and [%s] between '%s' and '%s' ",sChann.c_str(),ldbData,gc_cTime,CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
		}

		if(pDB->More())
		{
			lbDataGet = TRUE;
		}
	}

	if (lbDataGet)
	{
		S_VibCharValue vibCharValue;
		int inum     = SimADO_GetSampleNumEx(pDB, sChannHor);
		int iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannHor);
		//CString sTmp;sTmp.Format("inum:%d,iSmpFreq:%d",inum,iSmpFreq);BHTRACE_DEBUG(sTmp);
		float fRev = .0;
		switch(channInfo._iRevType)
		{
		case 1:
			fRev = pDB->GetDouble( gc_cRev);
			break;
		case 2:
			fRev = pDB->GetDouble( gc_cRev2 );
			break;
		case 3:
			fRev = pDB->GetDouble( gc_cRev3);
			break;
		case 4:
			fRev = pDB->GetDouble( gc_cRev4);
			break;
		case 5:
			fRev = pDB->GetDouble( gc_cRev5);
			break;
		default:
			fRev = pDB->GetDouble( gc_cRev);
		}
		pDtWaveHor=new DATATYPE_WAVE[inum];
		memset(pDtWaveHor,0,inum*sizeof(DATATYPE_WAVE));
		pDtWaveVer=new DATATYPE_WAVE[inum];
		memset(pDtWaveVer,0,inum*sizeof(DATATYPE_WAVE));
		if (inum>1)
		{//为了防止采样点数为0的情况下出现异常。
			vibCharValue=ReadVibCharValue(sChann,pDB);
			long isize=inum * sizeof(DATATYPE_WAVE);
			CString strWaveHor,strWaveVer;
			strWaveHor.Format("WAVE_%s",sChannHor.c_str());
			strWaveVer.Format("WAVE_%s",sChannVer.c_str());
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, strWaveHor,  (BYTE *)pDtWaveHor,  isize, sWaveHead);
			SimADO_SafeGetBufferEx(pDB, strWaveVer,  (BYTE *)pDtWaveVer,  isize, sWaveHead);
			inum=isize/sizeof(DATATYPE_WAVE);

			if (inum%2 != 0)
			{
				inum -= 1;
			}

			if (inum>GD_MIDDLEWARE_WAVE_POINT)
				inum=GD_MIDDLEWARE_WAVE_POINT;
			//writefile(pDtWaveHor,inum,1);
			//writefile(pDtWaveVer,inum,2);
			pfWaveHor=new double[inum];
			memset(pfWaveHor,0,inum*sizeof(double));
			pfWaveVer=new double[inum];
			memset(pfWaveVer,0,inum*sizeof(double));
			pPlantInfo->ConvertDTWaveToDouble(pfWaveHor,pDtWaveHor,inum,sWaveHead);
			pPlantInfo->ConvertDTWaveToDouble(pfWaveVer,pDtWaveVer,inum,sWaveHead);
			//writefile(pfWaveHor,inum,33);
			//writefile(pfWaveVer,inum,44);
			double dWorkFreq=(double)fRev/60;
			CSynopsis2KB calcOrbitAndFreq;
			//计算轴心轨迹形状和进动方向
			int icalcOrbit=-1;
			if (dWorkFreq>.0)
			{
				try
				{
					icalcOrbit = calcOrbitAndFreq.GetOrbitShape(pfWaveVer,pfWaveHor,inum,dWorkFreq,iSmpFreq,iPrecession,iOrbitShape);
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

			if (icalcOrbit < 0)
				CHZLogManage::Error("计算轴心轨迹形状出错","ExpertSys","ExpertSysGetSpecChannFeatureExtract");
			int icalcFreq = -1;
			CString sTmp;sTmp.Format("自动诊断:sChann:%s,iOrbitShape:%d,iPrecession:%d,icalcOrbit:%d",sChann.c_str(),iOrbitShape,iPrecession,icalcOrbit);
			//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetSpecChannFeatureExtract");
			//计算主导频率和常伴频率
			if (dWorkFreq>.0)
			{
				try
				{
					if (strcmp(sChann.c_str(),sChannHor.c_str())==0)
						icalcFreq = calcOrbitAndFreq.GetLeadFreqAndAccomFreq(pfWaveHor,inum,iSmpFreq,dWorkFreq,iLeadFreq,iAccomFreq,25,3);
					else
						icalcFreq = calcOrbitAndFreq.GetLeadFreqAndAccomFreq(pfWaveVer,inum,iSmpFreq,dWorkFreq,iLeadFreq,iAccomFreq,25,3);
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
			if (icalcFreq < 0)
				CHZLogManage::Error("计算主导和常伴频率出错","ExpertSys","ExpertSysGetSpecChannFeatureExtract");

			sTmp.Format("自动诊断:icalcFreq:%d,iLeadFreq:%d,iAccomFreq:%d",icalcFreq,iLeadFreq,iAccomFreq);
			//CHZLogManage::Debug( sTmp, "ExpertSys:CImplement","ExpertSysGetSpecChannFeatureExtract");
		}
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	if (iLeadFreq>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-ActLeadF",iLeadFreq,pFixComm);
	}
	//其他频率
	if (iAccomFreq>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-ElseFreq",iAccomFreq,pFixComm);
	}
	//轴心轨迹形状
	if (iOrbitShape>-1)
	{
		nCount++;
		WriteFertureInfo("S-RM-AxisType",iOrbitShape,pFixComm);
	}

	//进动方向
	/*
	if (iPrecession>-1)
	{
	nCount++;
	WriteFertureInfo("S-RM-Precessi",iPrecession,pFixComm);
	}
	*/
	//机组信息
	WriteMachineFertureInfo(pPlantInfo,pFixComm);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pDtWaveHor);
	KD_DeletePoint(pDtWaveVer);
	KD_DeletePoint(pfWaveHor);
	KD_DeletePoint(pfWaveVer);
	return nCount;
}

/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*返回的字段码：
*             公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             事实名     FIELD_SERVICE_REMARK     //对应为字符串
*#define    SERVICE_EXPERTSYS_GET_PLANTFACTNAME      30004
*/
int CImplement::ExpertSysGetPlantFactName(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	int lnCount = 0;

	CString sCompany_ =pFixComm->Get(FIELD_SYSTEM_COMPANY);
	CString sFactory_ =pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CString sPlant_   =pFixComm->Get(FIELD_SERVICE_PLANTID);

	//如果输入设备名为空，则返回所有设备的事实名称
	bool lbGetAll = false;
	if (sPlant_.IsEmpty())
	{
		lbGetAll = true;
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	map<string,CPlantInfoExpertSys *>::iterator lpIt;

	for (lpIt = g_mapPlantInfo.begin(); lpIt != g_mapPlantInfo.end(); ++lpIt)
	{
		CPlantInfoExpertSys * lpPlantInfo = lpIt->second; 

		if (NULL == lpPlantInfo)
		{
			continue;
		}

		if (!lbGetAll && (sCompany_ != lpPlantInfo->GetCompanyName().c_str() || 
			              sFactory_ != lpPlantInfo->GetFactoryName().c_str() ||
						  sPlant_   != lpPlantInfo->GetPlantID().c_str()))
		{
			continue;
		}

		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,lpPlantInfo->GetCompanyName().c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,lpPlantInfo->GetFactoryName().c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANTID,lpPlantInfo->GetPlantID().c_str());
		pFixComm->SetItem(FIELD_SERVICE_REMARK,lpPlantInfo->GetPlantFactName());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();

		lnCount++;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return lnCount;
}

/*   事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形  */
void CImplement::WriteFertureInfo(string sFactID_,int iValue_,ICommInterFace *pFixComm_)
{
	if (iValue_>-1)
	{
		pFixComm_->SetItem(FIELD_SERVICE_EX_RULEID,sFactID_.c_str());
		pFixComm_->SetItem(FIELD_SERVICE_EX_IDVALUE,iValue_);
		pFixComm_->Write(pFixComm_->GetAci());
		pFixComm_->CreateNextAnswer();
	}
}

void CImplement::WriteMachineFertureInfo(CPlantInfoExpertSys *pPlantInfo_ ,ICommInterFace *pFixComm_)
{
	int iMachType=-1,iConnType=-1,iRotateType=-1,iWorkRev=-1,iDriverType=-1,iBearType=-1,iSealType=-1;
	//机器类型
	iMachType   = pPlantInfo_->GetPlantInfo()._iMachineType;
	if (iMachType>-1)
	{
		WriteFertureInfo("S-RM-MachType",iMachType,pFixComm_);
	}
	//连接形式
	iConnType   = pPlantInfo_->GetPlantInfo()._iConnType;
	if (iConnType>-1)
	{
		iConnType = ConvertConnType(iConnType);
		WriteFertureInfo("S-RM-ConnMode",iConnType,pFixComm_);
	}
	//转子形式
	iRotateType = pPlantInfo_->GetPlantInfo()._iRotorType;
	if (iRotateType>-1)
	{
		iRotateType = ConvertRotateType(iRotateType);
		WriteFertureInfo("S-RM-RotoMode",iRotateType,pFixComm_);
	}
	//工作转速
	iWorkRev    = pPlantInfo_->GetPlantInfo()._iWorkRevType;
	if (iWorkRev>-1)
	{
		iWorkRev = ConvertWorkRev(iWorkRev);
		WriteFertureInfo("S-RM-WorkRev",iWorkRev,pFixComm_);
	}
	//驱动类型
	iDriverType = pPlantInfo_->GetPlantInfo()._iDriverType;
	if (iDriverType>-1)
	{
		iDriverType = ConvertDriverType(iDriverType);
		WriteFertureInfo("S-RM-DrivType",iDriverType,pFixComm_);
	}
	//轴承类型
	iBearType = pPlantInfo_->GetPlantInfo()._iBearingType;
	if (iBearType>-1)
	{
		iBearType = ConvertBearType(iBearType);
		WriteFertureInfo("S-RM-BearType",iBearType,pFixComm_);
	}
	//密封形式
	iSealType = pPlantInfo_->GetPlantInfo()._iSealType;
	if (iSealType>-1)
	{
		iSealType = ConvertSealType(iSealType);
		WriteFertureInfo("S-RM-SealType",iSealType,pFixComm_);
	}
}

CPlantInfoExpertSys * CImplement::ReadCheckCompanyInfo(string & sCompany_,string & sFactory_,string & sPlant_,string & sSuffix_,ICommInterFace *pFixComm_,bool bRtn_)
{
	sCompany_=pFixComm_->Get(FIELD_SYSTEM_COMPANY);
	sFactory_=pFixComm_->Get(FIELD_SYSTEM_FACTORY);
	sPlant_=pFixComm_->Get(FIELD_SERVICE_PLANTID);
	return CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix_,pFixComm_,bRtn_);
}

CPlantInfoExpertSys * CImplement::CheckCompanyInfo(const string &sCompany_, const string &sFactory_, const string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_)
{
	if ((sPlant_.empty())||(sCompany_.empty())||(sFactory_.empty()))
	{
		if (bRtn_)
		{
			pFixComm_->CreateAnswer(pFixComm_->GetFunc());
			pFixComm_->Write(pFixComm_->GetAci());
		}
		return NULL;
	}
	string sConn;
	sConn=sCompany_;
	sConn.append(sPlant_);
	MAP_PLANTINFO::iterator it;
	it=g_mapPlantInfo.find(sConn);
	if (it==g_mapPlantInfo.end())
	{
		if (bRtn_)
		{
			pFixComm_->CreateAnswer(pFixComm_->GetFunc());
			pFixComm_->Write(pFixComm_->GetAci());
		}
		return NULL;
	}
	sSuffix_=it->second->GetCompanyAlias();
	sSuffix_.append(it->second->GetPlantNo());
	return it->second;
}

CString CImplement::ConsCommonField()
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

CString CImplement::ConsVibChannField(string sChann_,bool bWave_/* =false */)
{
	CString sFields="";
	if (bWave_)
		sFields.Format("[ALL_%s],[ONE_%s],[TWO_%s],[THREE_%s],[HALF_%s],[RES_%s],[ONEPHASE_%s],[TWOPHASE_%s],[THREEPHASE_%s],[HALFPHASE_%s],[DC_%s],[GAP_%s],[WAVE_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s], 0) AS [SAMPLE_NUM_%s], ISNULL([REV_%s], 0) AS [REV_%s]",
		sChann_.c_str(),sChann_.c_str(), sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	else
		sFields.Format("[ALL_%s],[ONE_%s],[TWO_%s],[THREE_%s],[HALF_%s],[RES_%s],[ONEPHASE_%s],[TWOPHASE_%s],[THREEPHASE_%s],[HALFPHASE_%s],[DC_%s],[GAP_%s], ISNULL([SAMPLE_FREQ_%s], 0) AS [SAMPLE_FREQ_%s],ISNULL([SAMPLE_NUM_%s],0) AS [SAMPLE_NUM_%s],ISNULL([REV_%s], 0) AS [REV_%s]",
		sChann_.c_str(),sChann_.c_str(), sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str(),sChann_.c_str());
	return sFields;
}


S_VibCharValue CImplement::ReadVibCharValue(string sChann_,IDBInterFace *pDB_)
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

//从指定数据连接中读取指定测点所有类型特征值
S_VibCharValue CImplement::ReadVibCharValue(string sChann_,CCustomDB *pDB_)
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
	vibValue.SetSpecTypeData(SimADO_GetSampleFrequency(pDB_, sChann_),GE_SAMPLEFREQ);
	vibValue.SetSpecTypeData(SimADO_GetSampleNum(pDB_, sChann_),GE_SAMPLENUM);
	vibValue.SetSpecTypeData(SimADO_GetRev(pDB_, sChann_),GE_REV);
	return vibValue;
}

CString CImplement::GetTblNamePrefix(const CHANNTYPE &iChannType_, E_TREND_TBL_TYPE &iTblType_/* =GE_TREND_TBLTYPE_SECOND */)
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

E_TBL_CHANNTYPE CImplement::GetChannOneType(const CHANNTYPE &iChannType_)
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


/**根据设备信息,测点信息，得到测点所在缸的机组一级转速，如果是温度测点，就取同缸的振动和动态测点的转速
*如果是振动/动态测点，直接返回测点的转速
*@param apPlantInfo   设备信息指针
*@param arefChanninfo 给定测点的引用
*@param apCustomDB	  数据库指针
*@return >=0 转速值，-1数据错误
*/
int CImplement::GetSpecSectionRev(
								  const CPlantInfoExpertSys * apPlantInfo,
								  const S_ChannInfo & arefChanninfo,
								  IDBInterFace & arefDB,
								  CBHDateTime					&	aoletimeStart,
								  CBHDateTime					&	aoletimeEnd)
{
	if (NULL == apPlantInfo)
	{
		return -1;
	}
	string lstrChannName;
	S_ChannInfo loChannInfo;

	switch(arefChanninfo._iChannType)
	{
	case GE_VIBCHANN:               /*径向振动测点.*/
	case GE_AXIALCHANN:             /*轴向振动测点.axial displacement*/
	case GE_PRESSCHANN:				/*压力测点.*/
	case GE_IMPACTCHANN:			/*撞击通道*/
	case GE_DYNPRESSCHANN:		    /*撞击通道*/			    
	case GE_RODSINKCHANN:			/*活塞杆下沉量信号*/
	case GE_DYNSTRESSCHANN:         /*动态应力测点*/				
		lstrChannName = arefChanninfo._cChannNo;
		loChannInfo = arefChanninfo;
		break;	
	default:
		if (apPlantInfo->GetVibChannNum()>0)
		{
			loChannInfo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSpecVibChannInfo(0);
			lstrChannName = loChannInfo._cChannNo;

		}else if (apPlantInfo->GetDynChannNum()>0)
		{
			loChannInfo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSpecDynChannInfo(0);
			lstrChannName = loChannInfo._cChannNo;

		}else
		{
			//没有振动测点、没有动态测点
			ASSERT(FALSE);
			return -1;
		}
	}

	//to be fixed，需要修改
	CString lstrField =SqlHelperForExpert::ConsChannField(lstrChannName,(CHANNTYPE)loChannInfo._iChannType) +" , " + SqlHelperForExpert::ConsCommonField();

	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);

	CString lstrTableSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;

	bool lbUseSubtbl = false;

	bool lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
													lstrTableSuffix.GetBuffer(0),
													lstrPlantDBName.GetBuffer(0),
													lstrField.GetBuffer(0),
													(CHANNTYPE)loChannInfo._iChannType,
													iTblType,
													aoletimeStart,
													aoletimeEnd,
													lbUseSubtbl
													);
	if (lbRet<=0)
	{
		return -1;
	}
	int lnRev = 0;

	switch(SqlHelperForExpert::GetCharCat((CHANNTYPE)(loChannInfo._iChannType)))
	{
	case SqlHelperForExpert::CHAN_VIB:
		
		{
			S_VibCharValue loCharValue = SqlHelperForExpert::ReadVibCharValue(lstrChannName,&arefDB);
			lnRev = loCharValue._iRev;
		}

		break;
	case  SqlHelperForExpert::CHAN_DYN:

		{
			S_DynCharValue loCharValue = SqlHelperForExpert::ReadDynCharValue(lstrChannName,&arefDB);
			lnRev = loCharValue._iRev;
		}

		break;

	default:

		ASSERT(FALSE);

		lnRev = 0;	

	}

	return lnRev;


}

int CImplement::GetRevOfAbnormalData( const CPlantInfoExpertSys		*	apPlantInfo,
									  const S_ChannInfo				&	arefChanninfo,
									  IDBInterFace					&	arefDB,
									  COleDateTime					&	aoletimeStart,
									  COleDateTime					&	aoletimeEnd)
{
	if (NULL == apPlantInfo)
	{
		return -1;
	}
	string lstrChannName;
	S_ChannInfo loChannInfo;

	switch(arefChanninfo._iChannType)
	{
	case GE_VIBCHANN:               /*径向振动测点.*/
	case GE_AXIALCHANN:             /*轴向振动测点.axial displacement*/
	case GE_PRESSCHANN:				/*压力测点.*/
	case GE_IMPACTCHANN:			/*撞击通道*/
	case GE_DYNPRESSCHANN:		    /*撞击通道*/			    
	case GE_RODSINKCHANN:			/*活塞杆下沉量信号*/
	case GE_DYNSTRESSCHANN:         /*动态应力测点*/				
		lstrChannName = arefChanninfo._cChannNo;
		loChannInfo = arefChanninfo;
		break;	
	default:
		if (apPlantInfo->GetVibChannNum()>0)
		{
			loChannInfo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSpecVibChannInfo(0);
			lstrChannName = loChannInfo._cChannNo;

		}else if (apPlantInfo->GetDynChannNum()>0)
		{
			loChannInfo = ((CPlantInfoExpertSys *)apPlantInfo)->GetSpecDynChannInfo(0);
			lstrChannName = loChannInfo._cChannNo;

		}else
		{
			//没有振动测点、没有动态测点
			ASSERT(FALSE);
			return -1;
		}
	}

	CString lstrField =SqlHelperForExpert::ConsChannField(lstrChannName,(CHANNTYPE)loChannInfo._iChannType) +" , " + SqlHelperForExpert::ConsCommonField();

	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);

	CString lstrTableSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;

	bool lbUseSubtbl = false;

	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd;
	SwitchDbNameEx(&arefDB,lstrPlantDBName);

	strTbl.Format("%s%s",SqlHelperForExpert::GetTblNamePrefixForTrend2( (CHANNTYPE)loChannInfo._iChannType, aoletimeStart,aoletimeEnd,iTblType,lbUseSubtbl,bUseSupTbl),lstrTableSuffix);
	strTimeStart= CPlantInfoBase::TimeConvertString(aoletimeStart);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(aoletimeEnd);
	strWhere.Format("  [%s] BETWEEN '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

	int iNums=0;

	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	int iRealTblType=SqlHelperForExpert::GetTrendTblType(aoletimeStart);

	bool bRead=false;//是否可以读取记录
	//判断记录数目是否满足要求的趋势记录数目。如果不满足，就判断是否使用的趋势表类型不是起始时间的趋势表类型而是它的上一级。
	{
REALTBLETYPE_READRECORDS:
		if (!(iTblType<iRealTblType))
		{
			strTbl.Format("%s%s",GetTblNamePrefix((CHANNTYPE)loChannInfo._iChannType,iTblType),lstrTableSuffix);
			arefDB.Cmd("SELECT %s FROM [%s]",lstrField,strTbl);
			arefDB.Cmd(" WHERE %s ",strWhere);


			if (arefDB.More())
			{
				iNums=arefDB.GetCount();
			}
			else {
				iNums=0;

				// 防止超时或其它异常，
				if(arefDB.GetLastError() < 0){
					bUseSupTbl = false;
				}
			}

			bUseSupTbl = false;
		}
	}

	//遍历所有数据点，获取异常数据时间点的转速，如果停车期间数据超过异常门限，则认为是传感器异常
	int lnRev = 0;

	bool lbUnNormal = false;

	while(arefDB.More())
	{
		int lnNormalData = GE_DATA_VALUE_NO_ERROR;

		switch(SqlHelperForExpert::GetCharCat((CHANNTYPE)(loChannInfo._iChannType)))
		{
		case SqlHelperForExpert::CHAN_VIB:

			{
				S_VibCharValue loCharValue = SqlHelperForExpert::ReadVibCharValue(lstrChannName,&arefDB);

				lnNormalData = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loCharValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);

				lnRev = loCharValue._iRev;
				//采集到的数据异常并且当前转速为停车，则必然是传感器故障
				if (lnNormalData < 0 && loCharValue._iRev < 0)
				{
					lbUnNormal = true;   

					break;
				}
			}

			break;
		case  SqlHelperForExpert::CHAN_DYN:

			{
				S_DynCharValue loCharValue = SqlHelperForExpert::ReadDynCharValue(lstrChannName,&arefDB);

				lnNormalData = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loCharValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);

				//采集到的数据异常并且当前转速为0，则必然是传感器故障
				lnRev = loCharValue._iRev;

				if (lnNormalData < 0)
				{
					lbUnNormal = true;
					
					break;
				}
			}

			break;

		case  SqlHelperForExpert::CHAN_PROC:

			{
				double ldblProc = arefDB.GetDouble(lstrChannName.c_str());

				lnNormalData = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(ldblProc,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);
			}

			break;
		default:

			ASSERT(FALSE);

			lnRev = 0;	

		}

		if (lnNormalData)
		{
			break;
		}
	}

	return lnRev;
}

/**根据设备信息，测点信息，起止时间，得到测点所在缸，给定通道类型的测点的报警状态
*@param arefDB		  [in]	   数据库接口
*@param arefFixComm	  [in]	   通信接口
*@param apPlantInfo   [in]     设备信息指针
*@param arefChanninfo [in]     给定测点的引用
*@param aoletimeStart [in]     判断报警开始时间
*@param aoletimeEnd	  [in]     判断报警结束时间
*@param  aenumType    [in]     测点类型
*@return 0-正常，1-异常，2-不确定， -1数据错误
*/
E_RC_AlarmStatus CImplement::GetSpecChannTypeAlarmStatus(
	IDBInterFace						& arefDB,
	ICommInterFace						& arefFixComm,
	const CPlantInfoExpertSys			* apPlantInfo,
	const S_ChannInfo					& arefChanninfo,
	CBHDateTime						& aoletimeStart,
	CBHDateTime						& aoletimeEnd,
	ALARM_CHANN_TYPE					aenumType /*= EXPERT_CHANN_TYPE_TEMP*/
	)
{
	if (!apPlantInfo)
	{
		return E_RC_AlarmStatus_Error;
	}

	CHANNTYPE anChannType;

	anChannType = this->TranlateChannType(aenumType);

	if (anChannType == GE_OTHERCHANN)
	{
		return E_RC_AlarmStatus_Error;
	}

	CString lstrSql;

	BOOL lbRet =  FALSE;

	if (anChannType == GE_RODSINKCHANN)
	{
		if (aenumType == EXPERT_CHANN_TYPE_SINK)
		{
			lbRet = SqlHelperForExpert::GetSqlForGetSpecChannTypeAlarmStatus(
																			apPlantInfo,
																			arefChanninfo,
																			anChannType,
																			aoletimeStart,
																			aoletimeEnd,
																			lstrSql,
																			aenumType,
																			MEASURE_DIRECTION_VERTICAL);
		}else
		{
			lbRet = SqlHelperForExpert::GetSqlForGetSpecChannTypeAlarmStatus(
																			apPlantInfo,
																			arefChanninfo,
																			anChannType,
																			aoletimeStart,
																			aoletimeEnd,
																			lstrSql,
																			aenumType,
																			MEASURE_DIRECTION_HORIZONTAL);
		}

		
	}else
	{
		lbRet = SqlHelperForExpert::GetSqlForGetSpecChannTypeAlarmStatus(
																			apPlantInfo,
																			arefChanninfo,
																			anChannType,
																			aoletimeStart,
																			aoletimeEnd,
																			lstrSql,
																			aenumType);

	}	

	if (!lbRet)
	{
		ASSERT(FALSE);
		return E_RC_AlarmStatus_Error;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	if (arefDB.More())
	{		
		lnAlarmFound = 1;
	}

	if (lnAlarmFound>0)
	{
		return E_RC_AlarmStatus_AbNormal;
	}
	else
	{
		return E_RC_AlarmStatus_Normal;
	}

}


CString GetChannStr(int anType,CString & lstrChannNameTemp,CImplement * apThis)
{
	CString lstrFiledTemp ;
	BOOL lbVib = FALSE;
	switch(anType)
	{
	case  GE_VIBCHANN:
	case  GE_AXIALCHANN:
		  lstrFiledTemp.Format(_T("[ALL_%s]"),lstrChannNameTemp);
		  lbVib = TRUE;
	case  GE_DYNPRESSCHANN:
	case  GE_RODSINKCHANN:
	case  GE_DYNSTRESSCHANN:
	case  GE_AXISLOCATIONCHANN:
		if (!lbVib)
		{
		  lstrFiledTemp.Format(_T("[ZERO_%s]"),lstrChannNameTemp);
		}
		lstrChannNameTemp =  SqlHelperForExpert::ConsChannField(lstrChannNameTemp.GetBuffer(0),(CHANNTYPE)anType);
		lstrChannNameTemp = lstrChannNameTemp +"," + apThis->ConsCommonField();
		break;
	default:
		lstrFiledTemp = gc_cProcValue;
		lstrChannNameTemp = '['+lstrChannNameTemp+']';
	}
	return lstrFiledTemp;
}


/**根据设备信息，测点信息，起止时间，得到测点所在缸，各个监测点的传感器故障状态
*@param arefDB		  [in]	   数据库接口
*@param arefFixComm	  [in]	   通信接口
*@param apPlantInfo   [in]     设备信息指针
*@param arefChanninfo [in]     给定测点的引用
*@param aoletimeStart [in]     判断故障开始时间
*@param aoletimeEnd	  [in]     判断故障结束时间
*@param  aenumType    [in]     测点类型
*@return  0-否（传感器正常），1-是（传感器故障），2-不确定，-1数据错误
*/
E_BoolExType CImplement::GetSpecChannTypeSensorStatus(
	IDBInterFace						& arefDB,
	ICommInterFace						& arefFixComm,
	const CPlantInfoExpertSys			* apPlantInfo,
	const S_ChannInfo					& arefChanninfo,
	CBHDateTime						& aoletimeStart,
	CBHDateTime						& aoletimeEnd,
	ALARM_CHANN_TYPE aenumType /*= EXPERT_CHANN_TYPE_TEMP*/
	)
{
	if (!apPlantInfo)
	{
		return E_BoolExType_Error;
	}

	CHANNTYPE anChannType;

	anChannType = this->TranlateChannType(aenumType);

	//CString lstrSql;

	////得到给定分类的报警测点ID,测点No
	//BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannTypeSensorStatus(
	//																	apPlantInfo,
	//																	arefChanninfo,
	//																	anChannType,
	//																	aoletimeStart,
	//																	aoletimeEnd,
	//																	lstrSql,
	//																	aenumType);

	//arefDB.Cmd(lstrSql);

	vector<S_ChannInfo> loAllSpecTypeChanns;

	int lnChannCount = this->GetSpecTypeChanns(apPlantInfo,aenumType,loAllSpecTypeChanns);

	std::map<CString,SqlParaStruct> loMapDate;

	S_ChannInfo loChannInfo;

	for(int lnChannIndex = 0; lnChannIndex < lnChannCount; lnChannIndex++)
	{
		loChannInfo = loAllSpecTypeChanns.at(lnChannIndex);

		CString lstrChann = loChannInfo._cChannNo;

		SqlParaStruct loStruct;
		loStruct.m_strName			= lstrChann;
		loStruct.m_oDateTimeStart	= aoletimeStart;
		loStruct.m_oDateTimeEnd		= aoletimeEnd;
		loStruct.m_ChannType		= (CHANNTYPE)loChannInfo._iChannType;

		loMapDate[lstrChann] = loStruct;
	}

	if (loMapDate.size()<=0)
	{
		return E_BoolExType_NO;
	}

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool lbNeedSubTbl = false;

	int lnAbnormalCount = 0;

	for(std::map<CString,SqlParaStruct>::iterator loAlarmChann = loMapDate.begin();
		loAlarmChann != loMapDate.end();
		loAlarmChann++)
	{
		//得到指定时间点和测点NO的趋势
		CString lstrChannNameTemp = loAlarmChann->first;
		CString lstrChannName = lstrChannNameTemp;
		CString lstrFiledTemp;
		SqlParaStruct * lpParam = &loAlarmChann->second;

		lstrFiledTemp =GetChannStr(lpParam->m_ChannType,lstrChannNameTemp,this);

		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		std::string lstrSubDb =  SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		int lbNormal =GE_DATA_VALUE_NO_ERROR;

		S_VibCharValue  loVibValue;

		S_DynCharValue  loDynValue;

		bool lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
			lstrSuffix,
			lstrSubDb,
			lstrChannNameTemp.GetBuffer(0),
			loAlarmChann->second.m_ChannType,
			iTblType,
			loAlarmChann->second.m_oDateTimeStart,
			loAlarmChann->second.m_oDateTimeEnd,
			lbNeedSubTbl
			);

		if (lbRet)
		{
			while(arefDB.More())
			{
				switch(loAlarmChann->second.m_ChannType)
				{
				case  GE_VIBCHANN:
				case  GE_AXIALCHANN:
				case  GE_AXISLOCATIONCHANN:				
					loVibValue= this->ReadVibCharValue(lstrChannName.GetBuffer(0),&arefDB);
					lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loVibValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);
					break;

				case  GE_DYNPRESSCHANN:
				case  GE_RODSINKCHANN:
				case  GE_DYNSTRESSCHANN:
					loDynValue = SqlHelperForExpert::ReadDynCharValue(lstrChannName.GetBuffer(0),&arefDB);
					lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loDynValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);
					break;

				default:
					double ldblProc = arefDB.GetDouble(lstrChannName);
					lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(ldblProc,(S_ChannInfo *)&loChannInfo,(CPlantInfoBase *)apPlantInfo);
					break;
				}

				if (lbNormal != GE_DATA_VALUE_NO_ERROR)
				{
					lnAbnormalCount++;
				}
			}
		}
	}


	if (lnAbnormalCount>0)
	{
		return E_BoolExType_YES;
	}
	else
	{
		return E_BoolExType_NO;
	}
	return E_BoolExType_Error;
}

int CImplement::GetTempValueTrend(IDBInterFace						& arefDB,
								  ICommInterFace					& arefFixComm,
								  std::vector<double>			    & aorefTrendData,
								  const CPlantInfoExpertSys			* apPlantInfo,
								  const S_ChannInfo					& arefChanninfo,
								  COleDateTime						& aoletimeStart,
								  COleDateTime						& aoletimeEnd,
								  BOOL						        & abIsValveGasIn)
{
	if (!apPlantInfo)
	{
		return -1;
	}

	std::map<CString,SqlParaStruct> loMapDate;

	CString lstrChann = arefChanninfo._cChannNo;

	SqlParaStruct loStruct;
	loStruct.m_strName			= lstrChann;
	loStruct.m_oDateTimeStart	= aoletimeStart;
	loStruct.m_oDateTimeEnd		= aoletimeEnd;
	loStruct.m_ChannType		= (CHANNTYPE)arefChanninfo._iChannType;

	loMapDate[lstrChann] = loStruct;
	//得到指定时间点和测点NO的趋势
	std::map<CString,SqlParaStruct>::iterator loAlarmChann = loMapDate.begin();

	CString lstrChannNameTemp = loAlarmChann->first;
	CString lstrChannName = lstrChannNameTemp;
	CString lstrFiledTemp;
	SqlParaStruct * lpParam = &loAlarmChann->second;

	lstrFiledTemp =GetChannStr(lpParam->m_ChannType,lstrChannNameTemp,this);

	std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

	std::string lstrSubDb =  SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

	int lbNormal =GE_DATA_VALUE_NO_ERROR;

	S_VibCharValue  loVibValue;

	S_DynCharValue  loDynValue;

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool lbNeedSubTbl = false;

	bool lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
		lstrSuffix,
		lstrSubDb,
		lstrChannNameTemp.GetBuffer(0),
		loAlarmChann->second.m_ChannType,
		iTblType,
		loAlarmChann->second.m_oDateTimeStart,
		loAlarmChann->second.m_oDateTimeEnd,
		lbNeedSubTbl
		);

	if (lbRet)
	{
		while(arefDB.More())
		{
			double ldblProc = arefDB.GetDouble(lstrChannName);

			aorefTrendData.push_back(ldblProc);
		}
	}

	return aorefTrendData.size();
}

/**根据设备信息，测点信息，起止时间，得到测点所在缸发生报警的温度通道的前后10分钟的数据，只取第一个发现的温度测点的报警数据
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param aorefTrendData [in/out] 趋势数据,点数需要不会过100个
*@param adblThreshold  [in/out] 报警数据的阈值
*@param apPlantInfo    [in]     设备信息指针
*@param arefChanninfo  [in]     给定测点的引用
*@param aoletimeStart  [in]     判断报警开始时间
*@param aoletimeEnd	   [in]     判断报警结束时间
*@param  aenumType     [in]     测点类型
*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
*/
int CImplement::GetSpecChannAlarmValueTempTrend(
	IDBInterFace				& arefDB,
	ICommInterFace				& arefFixComm,
	std::vector<double>			& aorefTrendData,
	double						& adblThreshold,
	const CPlantInfoExpertSys	* apPlantInfo,
	const S_ChannInfo			& arefChanninfo,
	CBHDateTime				& aoletimeStart,
	CBHDateTime				& aoletimeEnd,
	BOOL						&  abIsValveGasIn
	)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到测点所在缸发生报警的温度通道，只取第一个发现的温度测点
	CString lstrSql;
	
	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_TEMPCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;

	if (arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.GetTime(gc_cStart_T).Format();

		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);

		//loOleStartTime = loVtime;
		loOleStartTime = arefDB.GetTime(gc_cStart_T);

		lstrChannNo = arefDB.Get(gc_cChannNo).c_str();

	}else
	{
		return 0;
	}

	//2.根据测点号，开始时间得到趋势数据的SQL
	CString lstrSql2;
	COleDateTimeSpan loTimeSpan( 0, 0, TIME_SPAN_ALARM_VALUE_TEMP_TREND_AFTER, 0);
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	loOleEndTime   = loOleStartTime + loTimeSpan;
	COleDateTimeSpan loTimeSpan2( 0, 0, TIME_SPAN_ALARM_VALUE_TEMP_TREND_BEFORE, 0);
	loOleStartTime = loOleStartTime - loTimeSpan2;

	CString lstrFiledTemp;
	lstrFiledTemp.Format(_T("[%s]"),lstrChannNo);

	bool lbUseSubTable = FALSE;
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);
	CString lstrPlantTableSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);
	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
													    lstrPlantTableSuffix.GetBuffer(0),
														lstrPlantDBName.GetBuffer(0),
														lstrFiledTemp.GetBuffer(0),
														GE_TEMPCHANN,
														iTblType,
														loOleStartTime,
														loOleEndTime,
														lbUseSubTable);

	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
													lstrPlantTableSuffix.GetBuffer(0),
													lstrPlantDBName.GetBuffer(0),
													lstrFiledTemp.GetBuffer(0),
													GE_TEMPCHANN,
													iTblType,
													loOleStartTime,
													loOleEndTime,
													lbUseSubTable);
	}

	if (!lbRet)
	{
		return 0;
	}

	int lnTrendCount = 0;

	aorefTrendData.clear();

	while (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrChannNo);

		aorefTrendData.push_back(ldblData);

		lnTrendCount++;
		if (lnTrendCount>=TIME_SPAN_ALARM_VALUE_TEMP_TREND_DATA_LIMIT)
		{
			break;
		}

		lbRet = arefDB.More();
	}

	S_ChannInfo loInfo = ((CPlantInfoExpertSys	*) apPlantInfo)->GetSpecProcChannNoInfo(lstrChannNo.GetBuffer(0));

	switch(loInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		adblThreshold = loInfo._chAlarmParam._fAlarmHL;
		break;
	case E_ALARMCHECK_HIGHPASS:
		adblThreshold = loInfo._chAlarmParam._fAlarmLH;
		break;
	case E_ALARMCHECK_BANDPASS:
		adblThreshold = loInfo._chAlarmParam._fAlarmLH;
		break;
	case E_ALARMCHECK_BANDSTOP:
		adblThreshold = loInfo._chAlarmParam._fAlarmLL;
		break;
	default:
		ASSERT(FALSE);
		adblThreshold = loInfo._chAlarmParam._fAlarmHL;
		break;
	}

	abIsValveGasIn = ((CPlantInfoExpertSys	*) apPlantInfo)->IsValveGasIn(loInfo);

	return lnTrendCount;

}


/**根据设备信息,测点信息，得到测点所在缸的撞击测点的撞击次数
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param apPlantInfo    [in]		设备信息指针
*@param arefChanninfo  [in]		给定测点的引用
*@return >=0 撞击次数值，-1数据错误
*/
int  CImplement::GetSpecImpactTimes( 
									IDBInterFace						& arefDB,
									ICommInterFace						& arefFixComm,
									CPlantInfoExpertSys					* apPlantInfo,
									const S_ChannInfo					& arefChanninfo,
									unsigned int						& airefTimes_, 
									unsigned int						& airefTh_,
									CBHDateTime						& aoletimeStart,
									CBHDateTime						& aoletimeEnd
									)
{
	if (!apPlantInfo)
	{
		ASSERT(FALSE);

		return -1;
	}


	//1.设置诊断测点所在缸撞击测点
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSetImpactTimes(
															arefDB,
															apPlantInfo,
															arefChanninfo,
															GE_TEMPCHANN,
															aoletimeStart,
															aoletimeEnd,
															lstrSql);

	if (!lbRet)
	{
		return 0;
	}


	arefDB.Cmd(lstrSql);

	CString lstrChannId;
	CString lstrChannNo;

	if (arefDB.More())
	{
		lstrChannId = arefDB.Get(gc_cChannID).c_str();
		lstrChannNo = arefDB.Get(gc_cChannNo).c_str();

	}else
	{
		return 0;
	}

	//2.得到诊断测点所在缸的报警时间
	lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_TEMPCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_ALL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}
	
	arefDB.Cmd(lstrSql);

	CBHDateTime loOleEndTime;
	CBHDateTime loOleStartTime;

	if (arefDB.More())
	{
		loOleStartTime = arefDB.GetTime(gc_cStart_T);
	}

	COleDateTimeSpan loTimeSpan( 0, 0, 0, TIME_SPAN_ALARM_VALUE_DEFLECT_TREND);
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	loOleEndTime   = loOleStartTime + loTimeSpan;

	CString lstrFiledTemp;
	lstrFiledTemp.Format(_T("[%s]"),lstrChannNo);

	bool lbUseSubTable = FALSE;

	SwitchDbNameEx(&arefDB,SqlHelperForExpert::GetSpecPlantDBName(*apPlantInfo));
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);
	CString lstrTableSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);


	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
														lstrTableSuffix.GetBuffer(0),
														lstrPlantDBName.GetBuffer(0),
														lstrFiledTemp.GetBuffer(0),
														GE_TEMPCHANN,
														iTblType,
														loOleStartTime,
														loOleEndTime,
														lbUseSubTable);


	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
													lstrTableSuffix.GetBuffer(0),
													lstrPlantDBName.GetBuffer(0),
													lstrFiledTemp.GetBuffer(0),
													GE_TEMPCHANN,
													iTblType,
													loOleStartTime,
													loOleEndTime,
													lbUseSubTable);

	}

	if (!lbRet)
	{
		return 0;
	}

	int lnImpactCount = 0;

	if (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrFiledTemp);

		lnImpactCount += ldblData;

	}

	return lnImpactCount;
};

/**根据设备信息,测点信息，得到测点所在缸的报警前后各5s的沉降量趋势。 点数不需要超过50个。
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param aorefTrendData [in/out] 趋势数据,点数需要不会过50个
*@param adblThreshold  [in/out] 沉降量的阈值，暂时不处理
*@param apPlantInfo    [in]     设备信息指针
*@param arefChanninfo  [in]     给定测点的引用
*@param aoletimeStart  [in]     判断报警开始时间
*@param aoletimeEnd	   [in]     判断报警结束时间
*@param  aenumType     [in]     测点类型
*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
*/
int CImplement::GetSpecSinkTrend(
					 IDBInterFace					& arefDB,
					 ICommInterFace					& arefFixComm,
					 std::vector<double>			& aorefTrendData,
					 double							& adblThreshold,
					 CPlantInfoExpertSys			* apPlantInfo,
					 const S_ChannInfo				& arefChanninfo,
					 CBHDateTime					& aoletimeStart,
					 CBHDateTime					& aoletimeEnd,
					 double							& aRefFLoAlarmValue_, 
					 double							& aRefFHiAlarmValue_, 
					 double							& aRefFMaxRange_, 
					 bool							& aRefBHiPass_
					 )
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻沉降量通道的测点名
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_RODSINKCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_VERTICAL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;
	BOOL lbChannFound = FALSE;

	while(arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
		//loOleStartTime = loVtime;

		loOleStartTime = arefDB.GetTime(gc_cStart_T);

		int lnLoopCount = apPlantInfo->GetDynChannNum();
		lstrChannNo = arefDB.Get(gc_cChannNo).c_str();


		S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

		if (GE_RODSINKCHANN == loInfo._iChannType)
		{
			lstrChannNo = loInfo._cChannNo;
			lbChannFound = TRUE;
			break;
		}


	}

	if (!lbChannFound)
	{
		return 0;
	}	

	//2.根据测点名，得到趋势数据

	COleDateTimeSpan loTimeSpan( 0, 0, 0, TIME_SPAN_ALARM_VALUE_SINK_TREND_AFTER);
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	loOleEndTime   = loOleStartTime + loTimeSpan;
	COleDateTimeSpan loTimeSpan2( 0, 0, 0, TIME_SPAN_ALARM_VALUE_SINK_TREND_BEFORE);
	loOleStartTime = loOleStartTime - loTimeSpan2;
	bool lbUseSubTable = FALSE;
	
	CString lstrFiledTemp;
	lstrFiledTemp.Format(_T("ZERO_%s"),lstrChannNo);
	CString lstrShowFields;
	lstrShowFields = SqlHelperForExpert::ConsChannField(lstrChannNo.GetBuffer(0),
													(CHANNTYPE)(arefChanninfo._iChannType));

	
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);

	CString lstrPlantSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);


	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
														lstrPlantSuffix.GetBuffer(0),
														lstrPlantDBName.GetBuffer(0),
														lstrShowFields.GetBuffer(0),
														GE_RODSINKCHANN,
														iTblType,
														loOleStartTime,
														loOleEndTime,
														lbUseSubTable);


	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
													lstrPlantSuffix.GetBuffer(0),
													lstrPlantDBName.GetBuffer(0),
													lstrShowFields.GetBuffer(0),
													GE_RODSINKCHANN,
													iTblType,
													loOleStartTime,
													loOleEndTime,
													lbUseSubTable);
	}

	if (!lbRet)
	{
		return 0;
	}

	int lnTrendCount = 0;

	aorefTrendData.clear();

	int lnDataLimit = ALARM_VALUE_SINK_TREND_DATA_LIMIT;

	while (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrFiledTemp);

		aorefTrendData.push_back(ldblData);

		lnTrendCount++;

		if (lnTrendCount>=lnDataLimit)
		{
			break;
		}

		lbRet = arefDB.More();
	}

	S_ChannInfo loInfo = ((CPlantInfoExpertSys	*) apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

	
	aRefBHiPass_ = ParseTrendAlarmHighLowValue(aorefTrendData,
												loInfo,
												aRefFHiAlarmValue_,
												aRefFLoAlarmValue_);

	CString lstrUnit = loInfo._cUnit;
   
	if (lstrUnit.Trim().CompareNoCase("um")==0)
	{
		aRefFMaxRange_ =1500;

	}else if (lstrUnit.Trim().CompareNoCase("mm")==0)
	{
		aRefFMaxRange_ =1.5;
	}else
	{
		aRefFMaxRange_ =1500;
		ASSERT(FALSE);
	}

	

	return lnTrendCount;

}


/**根据设备信息,测点信息，得到测点所在缸的沉降报警时刻的振动波形。。
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param aorefTrendData [in/out] 波形数据
*@param anFs		   [in/out]	采样频率
*@param apPlantInfo    [in]     设备信息指针
*@param arefChanninfo  [in]     给定测点的引用
*@param aoletimeStart  [in]     判断报警开始时间
*@param aoletimeEnd	   [in]     判断报警结束时间
*@param  aenumType     [in]     测点类型
*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
*/
int CImplement::GetSpecSinkWave(
					IDBInterFace					& arefDB,
					ICommInterFace					& arefFixComm,
					std::vector<double>				& aorefWaveData,
					int								& anFs,
					 CPlantInfoExpertSys			* apPlantInfo,
					const S_ChannInfo				& arefChanninfo,
					CBHDateTime					& aoletimeStart,
					CBHDateTime					& aoletimeEnd,
					double							& ldbThrethold
					)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻沉降量通道的测点名
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_RODSINKCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_VERTICAL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;
	CBHDateTime loReqTime;


	if (arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
		//loOleStartTime = loVtime;
		loOleStartTime = arefDB.GetTime(gc_cStart_T);
		loReqTime = loOleStartTime;

		COleDateTimeSpan loSpan = COleDateTimeSpan(0,0,5,0);
		loOleStartTime = loOleStartTime - loSpan;
		loOleEndTime   = loReqTime + loSpan;

		int lnLoopCount = apPlantInfo->GetDynChannNum();

		for (int i =0;i<lnLoopCount;i++)
		{
			S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecDynChannInfo(i);

			if ( loInfo._iCylinderNo == arefChanninfo._iCylinderNo 
				&& loInfo._iMeassureDirection  == MEASURE_DIRECTION_VERTICAL)
			{
				lstrChannNo = loInfo._cChannNo;

				break;
			}
		}

	}else
	{
		return 0;
	}

	BOOL lbGetFromAlarm = TRUE;
	
	//2.取报警日志点最近的波形点
	{
		bool lbUseSubTable = FALSE;
		string lstrDatetime = gc_cDateTime;
		lstrDatetime = "["+lstrDatetime;
		lstrDatetime = lstrDatetime+"]";
		E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix
			(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);
		std::string lstrDBName= SqlHelperForExpert::GetSpecPlantDBName
			(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);
		bool lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(
			&arefDB,
			lstrSuffix,
			lstrDBName,
			lstrDatetime,
			GE_VIBCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable
			);

		if (!lbRet)
		{
			lbGetFromAlarm = FALSE;

			lbRet = SqlHelperForExpert::GetTrendRecords(
				&arefDB,
				lstrSuffix,
				lstrDBName,
				lstrDatetime,
				GE_VIBCHANN,
				iTblType,
				loOleStartTime,
				loOleEndTime,
				lbUseSubTable
				);
		}
		//没有读到趋势数据
		if (!lbRet)
		{
			ASSERT(FALSE);

		}else
		{
			CBHDateTime loFindTimeStart;
			CBHDateTime loFindTimeEnd;

			CBHDateTime loReqStart = loOleStartTime;
			CBHDateTime loReqEnd   = loOleEndTime;

			BOOL lbDateFound = FALSE;
			CString lstrReqStart = aoletimeStart.Format("%H:%M:%S");

			while (lbRet)
			{

				CBHDateTime loDatetime = arefDB.GetTime(gc_cDateTime);
				CString lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
				CString lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

				if (!lbDateFound)
				{
					loFindTimeStart = loDatetime;
					loFindTimeEnd = loDatetime;
					lbDateFound = TRUE;
				}

				if (loDatetime < aoletimeStart)
				{
					loFindTimeStart = loDatetime;
				}

				if (loDatetime > aoletimeEnd)
				{
					loFindTimeEnd = loDatetime;

					lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
					lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

					break;
				}

				lbRet = arefDB.More();			

			}


			ASSERT(lbDateFound);

			if (lbDateFound)
			{
				loOleStartTime = loFindTimeStart;
				loOleEndTime = loFindTimeEnd;
			}
		}

	}
	//3.读取波形
	aorefWaveData.clear();

	if (lbGetFromAlarm)
	{
		SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefWaveData,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			anFs
			);
	}else
	{
		SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefWaveData,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			anFs
			);
	}


	S_ChannInfo loInfo = ((CPlantInfoExpertSys	*) apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

	switch(loInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		ldbThrethold = loInfo._chAlarmParam._fAlarmHH;
		break;
	case E_ALARMCHECK_HIGHPASS:
		ldbThrethold =  loInfo._chAlarmParam._fAlarmLL;
		break;
	case E_ALARMCHECK_BANDPASS:
		ldbThrethold =  loInfo._chAlarmParam._fAlarmHL;
		break;
	case E_ALARMCHECK_BANDSTOP:
		ldbThrethold =  loInfo._chAlarmParam._fAlarmHH;
		break;
	default:
		ASSERT(FALSE);
		ldbThrethold = loInfo._chAlarmParam._fAlarmHH;
		break;
	}

	return aorefWaveData.size();

}

/**根据设备信息,测点信息，得到测点所在缸的报警前后各5s内的偏摆量趋势。 点数不需要超过50个。
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param aorefTrendData [in/out] 趋势数据,点数需要不会过50个
*@param adblThreshold  [in/out] 偏摆量的阈值，暂时不处理
*@param apPlantInfo    [in]     设备信息指针
*@param arefChanninfo  [in]     给定测点的引用
*@param aoletimeStart  [in]     判断报警开始时间
*@param aoletimeEnd	   [in]     判断报警结束时间
*@param  aenumType     [in]     测点类型
*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
*/
int CImplement::GetSpecDeflectTrend(
						IDBInterFace					& arefDB,
						ICommInterFace					& arefFixComm,
						std::vector<double>				& aorefTrendData,
						double							& adblThreshold,
						CPlantInfoExpertSys		* apPlantInfo,
						const S_ChannInfo				& arefChanninfo,
						CBHDateTime					& aoletimeStart,
						CBHDateTime					& aoletimeEnd,
						double							& aRefFLoAlarmValue_, 
						double							& aRefFHiAlarmValue_, 
						double							& aRefFMaxRange_, 
						bool							& aRefBHiPass_
						)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻的偏摆通道的测点名
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_RODSINKCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_HORIZONTAL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;

	BOOL lbChannFound = FALSE;

	while (arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
		//loOleStartTime = loVtime;
		
		loOleStartTime = arefDB.GetTime(gc_cStart_T);

		int lnLoopCount = apPlantInfo->GetDynChannNum();

		lstrChannNo = arefDB.Get(gc_cChannNo).c_str();

		S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

		if (GE_RODSINKCHANN == loInfo._iChannType)
		{
			lstrChannNo = loInfo._cChannNo;

			lbChannFound = TRUE;

			break;
		}
	}

	if (!lbChannFound)
	{
		return 0;
	}	

	//2.根据测点名，得到趋势数据

	COleDateTimeSpan loTimeSpan( 0, 0, 0, TIME_SPAN_ALARM_VALUE_DEFLECT_TREND_AFTER);
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	loOleEndTime   = loOleStartTime + loTimeSpan;
	COleDateTimeSpan loTimeSpan2( 0, 0, 0, TIME_SPAN_ALARM_VALUE_DEFLECT_TREND_BEFORE);
	loOleStartTime = loOleStartTime - loTimeSpan2;
	CString lstrFiledTemp;
	lstrFiledTemp.Format(_T("ZERO_%s"),lstrChannNo);
	//lstrChannNo = '['+lstrChannNo+']';
	bool lbUseSubTable = FALSE;
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);
	CString lstrPlantSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);
	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
		lstrPlantSuffix.GetBuffer(0),
		lstrPlantDBName.GetBuffer(0),
		lstrFiledTemp.GetBuffer(0),
		GE_RODSINKCHANN,
		iTblType,
		loOleStartTime,
		loOleEndTime,
		lbUseSubTable);

	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
			lstrPlantSuffix.GetBuffer(0),
			lstrPlantDBName.GetBuffer(0),
			lstrFiledTemp.GetBuffer(0),
			GE_RODSINKCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable);
	}

	if (!lbRet)
	{
		return 0;
	}

	int lnTrendCount = 0;

	aorefTrendData.clear();

	int lnDataLimit = ALARM_VALUE_DEFLECT_TREND_DATA_LIMIT;

	while (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrFiledTemp);

		aorefTrendData.push_back(ldblData);

		lnTrendCount++;

		if (lnTrendCount>=lnDataLimit)
		{
			break;
		}

		lbRet = arefDB.More();
	}


	S_ChannInfo loInfo = ((CPlantInfoExpertSys	*) apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

	aRefBHiPass_ = ParseTrendAlarmHighLowValue(aorefTrendData,
		                                       loInfo,
											   aRefFHiAlarmValue_,
											   aRefFLoAlarmValue_);

	CString lstrUnit = loInfo._cUnit;

	if (lstrUnit.Trim().CompareNoCase("um")==0)
	{
		aRefFMaxRange_ =3000;

	}else if (lstrUnit.Trim().CompareNoCase("mm")==0)
	{
		aRefFMaxRange_ =3;
	}else
	{
		aRefFMaxRange_ =3000;
		ASSERT(FALSE);
	}

	return lnTrendCount;
}


int CImplement::GetSpecRodCharTrend(
						IDBInterFace					& arefDB,
						ICommInterFace					& arefFixComm,
						std::vector<double>				& aorefTrendData,
						CPlantInfoExpertSys				* apPlantInfo,
						const S_ChannInfo				& arefChanninfo,
						CBHDateTime					& aoletimeStart,
						CBHDateTime					& aoletimeEnd,
						E_ALARM_CHARTYPE                 aoCharType,
						GE_MEASURE_DIRECTION             aoMeasureDirection)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	CString lstrChannNo = arefChanninfo._cChannNo;
	//2.根据测点名，得到趋势数据

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	CString lstrFiledTemp;
	if (aoCharType == E_TH_ONECHAR)
	{
		lstrFiledTemp.Format(_T("ONE_%s"),lstrChannNo);
	}
	else if(aoCharType == E_TH_ONEPHASE)
	{
		lstrFiledTemp.Format(_T("SIX_%s"),lstrChannNo);
	}
	else if (aoCharType == E_TH_THREEPHASE)
	{
		lstrFiledTemp.Format(_T("EIGHT_%s"),lstrChannNo);
	}
	
	//lstrChannNo = '['+lstrChannNo+']';
	bool lbUseSubTable = FALSE;
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);
	CString lstrPlantSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);
	BOOL lbRet = FALSE;
	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
		lstrPlantSuffix.GetBuffer(0),
		lstrPlantDBName.GetBuffer(0),
		lstrFiledTemp.GetBuffer(0),
		GE_RODSINKCHANN,
		iTblType,
		aoletimeStart,
		aoletimeEnd,
		lbUseSubTable);

	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
			lstrPlantSuffix.GetBuffer(0),
			lstrPlantDBName.GetBuffer(0),
			lstrFiledTemp.GetBuffer(0),
			GE_RODSINKCHANN,
			iTblType,
			aoletimeStart,
			aoletimeEnd,
			lbUseSubTable);
	}

	if (!lbRet)
	{
		return 0;
	}

	int lnTrendCount = 0;

	aorefTrendData.clear();

	while (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrFiledTemp);

		aorefTrendData.push_back(ldblData);

		lnTrendCount++;

		lbRet = arefDB.More();
	}

	return lnTrendCount;
}

int CImplement::GetCrankTrendData(
								  IDBInterFace					& arefDB,
								  ICommInterFace					& arefFixComm,
								  std::vector<double>				& aorefTrendData,
								  CPlantInfoExpertSys				* apPlantInfo,
								  const S_ChannInfo				& arefChanninfo,
								  CBHDateTime					& aoletimeStart,
								  CBHDateTime					& aoletimeEnd,
								  E_ALARM_CHARTYPE                aoCharType)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻的偏摆通道的测点名
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_VIBCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_ALL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;

	BOOL lbChannFound = FALSE;

	while (arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
		//loOleStartTime = loVtime;

		loOleStartTime = arefDB.GetTime(gc_cStart_T);

		int lnLoopCount = apPlantInfo->GetDynChannNum();

		lstrChannNo = arefDB.Get(gc_cChannNo).c_str();

		S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

		if (GE_VIBCHANN == loInfo._iChannType && loInfo._iCylinderNo == 0)
		{
			lstrChannNo = loInfo._cChannNo;

			lbChannFound = TRUE;

			break;
		}
	}

	if (!lbChannFound)
	{
		return 0;
	}	

	//2.根据测点名，得到趋势数据

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	CString lstrFiledTemp;
	if (aoCharType == E_TH_ALLCHAR)
	{
		lstrFiledTemp.Format(_T("ALL_%s"),lstrChannNo);
	}
	else if(aoCharType == E_TH_ONECHAR)
	{
		lstrFiledTemp.Format(_T("ONE_%s"),lstrChannNo);
	}
	else if (aoCharType == E_TH_TWOCHAR)
	{
		lstrFiledTemp.Format(_T("TWO_%s"),lstrChannNo);
	}

	//lstrChannNo = '['+lstrChannNo+']';
	bool lbUseSubTable = FALSE;
	CString lstrPlantDBName = SqlHelperForExpert::GetSpecPlantDBName(*(CPlantInfoBase *)apPlantInfo);
	CString lstrPlantSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*(CPlantInfoBase *)apPlantInfo);
	lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(&arefDB,
		lstrPlantSuffix.GetBuffer(0),
		lstrPlantDBName.GetBuffer(0),
		lstrFiledTemp.GetBuffer(0),
		GE_VIBCHANN,
		iTblType,
		loOleStartTime,
		loOleEndTime,
		lbUseSubTable);

	if (!lbRet)
	{
		lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
			lstrPlantSuffix.GetBuffer(0),
			lstrPlantDBName.GetBuffer(0),
			lstrFiledTemp.GetBuffer(0),
			GE_VIBCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable);
	}

	if (!lbRet)
	{
		return 0;
	}

	int lnTrendCount = 0;

	aorefTrendData.clear();

	while (lbRet)
	{
		double ldblData = arefDB.GetDouble(lstrFiledTemp);

		aorefTrendData.push_back(ldblData);

		lnTrendCount++;

		lbRet = arefDB.More();
	}

	return lnTrendCount;
}

/**根据设备信息,测点信息，得到测点所在缸的偏摆报警时刻的振动波形。。
*@param arefDB		   [in]		数据库接口
*@param arefFixComm	   [in]		通信接口
*@param aorefTrendData [in/out] 波形数据,
*@param anFs		   [in/out]	采样频率
*@param apPlantInfo    [in]     设备信息指针
*@param arefChanninfo  [in]     给定测点的引用
*@param aoletimeStart  [in]     判断报警开始时间
*@param aoletimeEnd	   [in]     判断报警结束时间
*@param  aenumType     [in]     测点类型
*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
*/
int CImplement::GetSpecDeflectWave(
					   IDBInterFace						& arefDB,
					   ICommInterFace					& arefFixComm,
					   std::vector<double>				& aorefWaveData,
					   int								& anFs,
					   CPlantInfoExpertSys				* apPlantInfo,
					   const S_ChannInfo				& arefChanninfo,
					   CBHDateTime						& aoletimeStart,
					   CBHDateTime						& aoletimeEnd,
					   double							& aRefDbThrethold
					   )
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻的偏摆通道的测点名
	CString lstrSql;

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		arefChanninfo,
		GE_RODSINKCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_HORIZONTAL,
		TRUE);

	if (!lbRet)
	{
		return 0;
	}

	arefDB.Cmd(lstrSql);

	int lnAlarmFound = 0;

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;
	CBHDateTime loReqTime;

	if (arefDB.More())
	{		
		lnEventId = arefDB.GetLong(gc_cEventID);

		//CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
		//COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
		//loOleStartTime = loVtime;
		loOleStartTime = arefDB.GetTime(gc_cStart_T);
		loReqTime = loOleStartTime;

		COleDateTimeSpan loSpan = COleDateTimeSpan(0,0,0,20);
		loOleStartTime = loOleStartTime - loSpan;
		loOleEndTime   = loReqTime + loSpan;

		int lnLoopCount = apPlantInfo->GetDynChannNum();

		for (int i =0;i<lnLoopCount;i++)
		{
			S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecDynChannInfo(i);

			if (
				loInfo._iCylinderNo == arefChanninfo._iCylinderNo 
				&& loInfo._iMeassureDirection  == MEASURE_DIRECTION_HORIZONTAL
				)
			{
				lstrChannNo = loInfo._cChannNo;
				break;
			}
		}

	}else
	{
		return 0;
	}

	BOOL lbGetFromAlarm = TRUE;

	//2.取报警日志点最近的波形点
	{
		bool lbUseSubTable = FALSE;
		string lstrDatetime = gc_cDateTime;
		lstrDatetime = "["+lstrDatetime;
		lstrDatetime = lstrDatetime+"]";
		E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix
									(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);
		std::string lstrPlantDb = SqlHelperForExpert::GetSpecPlantDBName
			(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);
		bool lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(
			&arefDB,
			lstrSuffix,
			lstrPlantDb,
			lstrDatetime,
			GE_VIBCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable
			);

		if (!lbRet)
		{
			lbGetFromAlarm = FALSE;
			lbRet = SqlHelperForExpert::GetTrendRecords(
				&arefDB,
				lstrSuffix,
				lstrPlantDb,
				lstrDatetime,
				GE_VIBCHANN,
				iTblType,
				loOleStartTime,
				loOleEndTime,
				lbUseSubTable
				);
		}
		//没有读到趋势数据
		if (!lbRet)
		{
			ASSERT(FALSE);

		}else
		{
			CBHDateTime loFindTimeStart;
			CBHDateTime loFindTimeEnd;

			CBHDateTime loReqStart = loOleStartTime;
			CBHDateTime loReqEnd   = loOleEndTime;

			BOOL lbDateFound = FALSE;
			CString lstrReqStart = aoletimeStart.Format("%H:%M:%S");

			while (lbRet)
			{

				CBHDateTime loDatetime = arefDB.GetTime(gc_cDateTime);
				CString lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
				CString lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

				if (!lbDateFound)
				{
					loFindTimeStart = loDatetime;
					loFindTimeEnd = loDatetime;
					lbDateFound = TRUE;
				}

				if (loDatetime < aoletimeStart)
				{
					loFindTimeStart = loDatetime;
				}

				if (loDatetime > aoletimeEnd)
				{
					loFindTimeEnd = loDatetime;

					lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
					lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

					break;
				}

				lbRet = arefDB.More();			

			}


			ASSERT(lbDateFound);

			if (lbDateFound)
			{
				loOleStartTime = loFindTimeStart;
				loOleEndTime = loFindTimeEnd;
			}
		}

	}




	//3.读取波形
	aorefWaveData.clear();

	if (lbGetFromAlarm)
	{
		SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefWaveData,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			anFs
			);
	}else
	{
		SqlHelperForExpert::ReadDynChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefWaveData,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			anFs
			);
	}


	S_ChannInfo loInfo = ((CPlantInfoExpertSys	*) apPlantInfo)->GetSpecDynChannNoInfo(lstrChannNo.GetBuffer(0));

	switch(loInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		aRefDbThrethold = loInfo._chAlarmParam._fAlarmHH;
		break;
	case E_ALARMCHECK_HIGHPASS:
		aRefDbThrethold =  loInfo._chAlarmParam._fAlarmLL;
		break;
	case E_ALARMCHECK_BANDPASS:
		aRefDbThrethold =  loInfo._chAlarmParam._fAlarmHL;
		break;
	case E_ALARMCHECK_BANDSTOP:
		aRefDbThrethold =  loInfo._chAlarmParam._fAlarmHH;
		break;
	default:
		ASSERT(FALSE);
		aRefDbThrethold = loInfo._chAlarmParam._fAlarmHH;
		break;
	}
	return aorefWaveData.size();
}


/**得到诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形
*报警前1小时的波形
*@param arefDB					[in]		数据库接口
*@param arefFixComm				[in]		通信接口
*@param apPlantInfo				[in]		设备信息指针
*@param arefChanninfo			[in]		给定测点的引用
*@param aorefTrendDataBefore	[in/out]	报警前的波形数据
*@param aiStartIndBeforeAlarm_	[in/out]	报警前波形的一个周期的起始索引
*@param aiEndIndBeforeAlarm_	[in/out]	报警前波形的一个周期的结束索引
*@param aorefTrendDataAfter		[in/out]	报警后的波形数据
*@param aiStartIndAfterAlarm_	[in/out]	报警后波形的一个周期的起始索引
*@param aiEndIndAfterAlarm_		[in/out]	报警后波形一个周期的结束索引
*@param aiFsAfterAlarm_			[in/out]	报警后波形的采样频率
*@return 小于0表示获取失败
*/
int CImplement::GetCrossHeadWave(
					 IDBInterFace						&	arefDB,
					 ICommInterFace						&	arefFixComm,
					 CPlantInfoExpertSys				*	apPlantInfo,
					 const S_ChannInfo					&	arefChanninfo,
					 std::vector<double>				&	aorefTrendDataBefore,
					 int								&	aiStartIndBeforeAlarm_, 
					 int								&	aiEndIndBeforeAlarm_, 
					 std::vector<double>				&	aorefTrendDataAfter,
					 int								&	aiStartIndAfterAlarm_, 
					 int								&	aiEndIndAfterAlarm_, 
					 int								&   aiFsAfterAlarm_,
					 CBHDateTime						&	aoletimeStart,
					 CBHDateTime						&	aoletimeEnd,
					 double								&	aRefFLoAlarmValue_, 
					 double								&	aRefFHiAlarmValue_)

{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个缸体十字头振动的测点名
	CString lstrSql;

	int lnSectionNo = arefChanninfo._iCylinderNo;

	std::vector<S_ChannInfo> loCrossHeadInfos;

	BOOL lbRet = apPlantInfo->GetCrossHeadChannInfo(loCrossHeadInfos,lnSectionNo);

	if (loCrossHeadInfos.size()<=0)
	{
		return 0;
	}

	S_ChannInfo loCrossHeadOneInfo = loCrossHeadInfos.at(0);

	 lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		loCrossHeadOneInfo,
		GE_VIBCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_ALL,
		TRUE);

	CString lstrChannNo;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;
	BOOL lbAlarmGet = FALSE;
	
	//有报警状态的
	if (lbRet)
	{
		SwitchDbNameEx(&arefDB,SqlHelperForExpert::GetMainDBName());
			
		arefDB.Cmd(lstrSql);

		int lnAlarmFound = 0;


		if (arefDB.More())
		{		
			lnEventId = arefDB.GetLong(gc_cEventID);
			CString lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
			CBHDateTime loDateTimeTemp;
			loDateTimeTemp.ParseDateTime(lstrDateTime);
			aoletimeStart = loDateTimeTemp;
			lstrDateTime = loDateTimeTemp.Format("%H:%M:%S");
			COleDateTimeSpan loTimeSpan( 0, 0, TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE_DATA, 0);
			loOleStartTime = loDateTimeTemp - loTimeSpan;
			loOleEndTime = loDateTimeTemp  + loTimeSpan;
			lbAlarmGet = TRUE;
		}
	}
	//没有报警状态取前后各3天的
	if (!lbAlarmGet)
	{
		COleDateTimeSpan loTimeSpan( TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE_NO_ALARM, 0, 0, 0);
		loOleStartTime = aoletimeStart - loTimeSpan;
		loOleEndTime = aoletimeEnd + loTimeSpan;
	}

	int lnLoopCount = apPlantInfo->GetVibChannNum();

	BOOL lbChannGet = FALSE;
	
	//取振动测点名
	for (int i =0;i<lnLoopCount;i++)
	{
		S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecVibChannInfo(i);

		if (GE_VIBCHANN == loInfo._iChannType && loInfo._iCylinderNo == loCrossHeadOneInfo._iCylinderNo)
		{
			lstrChannNo = loInfo._cChannNo;

			switch(loInfo._chAlarmParam._iRegJudgeType)
			{
			case E_ALARMCHECK_LOWPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			case E_ALARMCHECK_HIGHPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmLL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmLH;

				break;
			case E_ALARMCHECK_BANDPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			case E_ALARMCHECK_BANDSTOP:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			default:
				ASSERT(FALSE);
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			}

			lbChannGet = TRUE;

			break;
		}
		
	}

	if (!lbChannGet)
	{
		//没有取到所在缸的振动通道
		ASSERT(FALSE);
		return -1;
	}
	
	BOOL lbFromAlarm = TRUE;
	
	//2.得到趋势数据，得到离报警时间点最近的一个点的时间
	if (lbAlarmGet)
	{
		bool lbUseSubTable = FALSE;
		string lstrDatetime = gc_cDateTime;
		lstrDatetime = "["+lstrDatetime;
		lstrDatetime = lstrDatetime+"]";
		E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);
		std::string lstrDbName= SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		bool lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(
			&arefDB,
			lstrSuffix,
			lstrDbName,
			lstrDatetime,
			GE_VIBCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable
			);
		//没有读到趋势数据
		if (!lbRet)
		{
			lbRet = SqlHelperForExpert::GetTrendRecords(
				&arefDB,
				lstrSuffix,
				lstrDbName,
				lstrDatetime,
				GE_VIBCHANN,
				iTblType,
				loOleStartTime,
				loOleEndTime,
				lbUseSubTable
				);

			lbFromAlarm = FALSE;

			if (!lbRet)
			{
				ASSERT(FALSE);

			}

		}else
		{
			CBHDateTime loFindTimeStart;
			CBHDateTime loFindTimeEnd;

			CBHDateTime loReqStart = loOleStartTime;
			CBHDateTime loReqEnd   = loOleEndTime;
			
			BOOL lbDateFound = FALSE;
			CString lstrReqStart = aoletimeStart.Format("%H:%M:%S");

			while (lbRet)  
			{

				CBHDateTime loDatetime = arefDB.GetTime(gc_cDateTime);
				CString lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
				CString lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

				if (!lbDateFound)
				{
					loFindTimeStart = loDatetime;
					loFindTimeEnd = loDatetime;
					lbDateFound = TRUE;
				}
				
				if (loDatetime < aoletimeStart)
				{
					loFindTimeStart = loDatetime;
				}

				if (loDatetime > aoletimeEnd)
				{
					loFindTimeEnd = loDatetime;

					lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
					lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");
					
					break;
				}

				lbRet = arefDB.More();			
				
			}


			ASSERT(lbDateFound);

			if (lbDateFound)
			{
				loOleStartTime = loFindTimeStart;
				loOleEndTime = loFindTimeEnd;
			}

		}

	}


	//3.读取波形
	aorefTrendDataBefore.clear();

	if (lbFromAlarm)
	{
		    SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefTrendDataBefore,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			aiFsAfterAlarm_
			);

	}else
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefTrendDataBefore,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			aiFsAfterAlarm_
			);
	}


	aiStartIndBeforeAlarm_ = 0;
	aiEndIndBeforeAlarm_ = aorefTrendDataBefore.size()/2;




	aorefTrendDataAfter.clear();

	if (lbFromAlarm)
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefTrendDataAfter,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleEndTime,
			aiFsAfterAlarm_
			);
	}else
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefTrendDataAfter,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannNo.GetBuffer(0),
			GD_400LINES,
			0,
			loOleEndTime,
			aiFsAfterAlarm_
			);
	}

	aiStartIndAfterAlarm_ = 0;
	aiEndIndAfterAlarm_ = aorefTrendDataAfter.size()/2;

	return aorefTrendDataAfter.size();
}


	/**得到诊断测点所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形
	  *报警前1小时的波形
	  *@param arefDB				[in]		数据库接口
	  *@param arefFixComm			[in]		通信接口
	  *@param apPlantInfo			[in]		设备信息指针
	  *@param arefChanninfo			[in]		给定测点的引用
	  *@param aorefTrendDataBefore	[in/out]	报警前的波形数据
	  *@param aiStartIndBeforeAlarm_[in/out]	报警前波形的一个周期的起始索引
	  *@param aiEndIndBeforeAlarm_  [in/out]	报警前波形的一个周期的结束索引
	  *@param aorefTrendDataAfter	[in/out]	报警后的波形数据
	  *@param aiStartIndAfterAlarm_ [in/out]	报警后波形的一个周期的起始索引
	  *@param aiEndIndAfterAlarm_	[in/out]	报警后波形一个周期的结束索引
	  *@param aiFsAfterAlarm_		[in/out]	报警后波形的采样频率
	  *@return 小于0表示获取失败
	  */
int CImplement::GetCrankWave(
									 IDBInterFace					&	arefDB,
									 ICommInterFace					&	arefFixComm,
									 CPlantInfoExpertSys			*	apPlantInfo,
									 const S_ChannInfo				&	arefChanninfo,
									 std::vector<double>			&	aorefTrendDataBefore,
									 int							&	aiStartIndBeforeAlarm_, 
									 int							&	aiEndIndBeforeAlarm_, 
									 std::vector<double>			&	aorefTrendDataAfter,
									 int							&	aiStartIndAfterAlarm_, 
									 int							&	aiEndIndAfterAlarm_, 
									 int							&   aiFsAfterAlarm_,
									 CBHDateTime					&	aoletimeStart,
									 CBHDateTime					&	aoletimeEnd,
									 double							&	aRefFLoAlarmValue_, 
									 double							&	aRefFHiAlarmValue_)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	//1.根据设备信息，测点信息，起止时间，得到给定时间段内的第一个报警时刻的通道的测点名
	CString lstrSql;

	//得到曲轴箱测点
	std::vector<S_ChannInfo> loCrankChanns;
	apPlantInfo->GetCrankChanninfo(loCrankChanns);
	if (loCrankChanns.size()<=0)
	{
		return 0;
	}

	S_ChannInfo loChanInfo = loCrankChanns.at(0);

	BOOL lbRet = SqlHelperForExpert::GetSqlForSpecChannAlarmChannByType(
		arefDB,
		apPlantInfo,
		loChanInfo,
		GE_VIBCHANN,
		aoletimeStart,
		aoletimeEnd,
		lstrSql,
		MEASURE_DIRECTION_ALL,
		TRUE);

	CString lstrChannName;
	long	lnEventId;
	CBHDateTime loOleStartTime;
	CBHDateTime loOleEndTime;
	CBHDateTime loDateTimeTempAlarm;
	BOOL lbAlarmGet = FALSE;
	CString lstrChannNo;
	CString lstrDateTime;
	//有报警状态的
	if (lbRet)
	{
		arefDB.Cmd(lstrSql);

		int lnAlarmFound = 0;

		if (arefDB.More())
		{		
			lnEventId = arefDB.GetLong(gc_cEventID);
			lstrChannNo = arefDB.Get(gc_cChannNo).c_str();
			/*lstrDateTime = arefDB.Get(gc_cStart_T).c_str();
			COleVariant loVtime(lstrDateTime);loVtime.ChangeType(VT_DATE);
			loDateTimeTempAlarm = loVtime;*/
			loDateTimeTempAlarm = arefDB.GetTime(gc_cStart_T);
			COleDateTimeSpan loTimeSpan( 0, 12, TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE, 0);
			loOleStartTime = loDateTimeTempAlarm - loTimeSpan;
			loOleEndTime = loDateTimeTempAlarm  + loTimeSpan;
			lbAlarmGet = TRUE;
		}
	}
	//没有报警状态取前后各3天的
	if (!lbAlarmGet)
	{
		COleDateTimeSpan loTimeSpan( TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE_NO_ALARM, 0, 0, 0);
		loOleStartTime = aoletimeStart - loTimeSpan;
		loOleEndTime = aoletimeEnd + loTimeSpan;
	}

	int lnLoopCount = apPlantInfo->GetVibChannNum();

	lbAlarmGet = FALSE;

	//取振动测点名
	//for (int i =0;i<lnLoopCount;i++)
	{
		S_ChannInfo loInfo = ((CPlantInfoBase *)apPlantInfo)->GetSpecVibChannNoInfo(lstrChannNo.GetBuffer(0));

		if (GE_VIBCHANN == loInfo._iChannType && loInfo._iCylinderNo == loChanInfo._iCylinderNo)
		{
			lstrChannName = loInfo._cChannNo;

			lbAlarmGet = TRUE;

			switch(loInfo._chAlarmParam._iRegJudgeType)
			{
			case E_ALARMCHECK_LOWPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			case E_ALARMCHECK_HIGHPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmLL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmLH;

				break;
			case E_ALARMCHECK_BANDPASS:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			case E_ALARMCHECK_BANDSTOP:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			default:
				ASSERT(FALSE);
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;

				break;
			}

			//break;
		}

	}

	if (!lbAlarmGet)
	{
		//没有取到振动通道
		if(lbRet)
		{
			ASSERT(FALSE);
		}

		return -1;
	}
	
	BOOL lbGetFromAlarm = TRUE;
	
	//2.得到距离报警时刻最近的时间点
	{
		bool lbUseSubTable = FALSE;
		string lstrDatetime = gc_cDateTime;
		lstrDatetime = "["+lstrDatetime;
		lstrDatetime = lstrDatetime+"]";
		E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		std::string lstrDbName = SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		bool lbRet = SqlHelperForExpert::GetTrendRecordsForAlarm(
			&arefDB,
			lstrSuffix,
			lstrDbName,
			lstrDatetime,
			GE_VIBCHANN,
			iTblType,
			loOleStartTime,
			loOleEndTime,
			lbUseSubTable
			);


		if (!lbRet)
		{
			lbRet = SqlHelperForExpert::GetTrendRecords(
				&arefDB,
				lstrSuffix,
				lstrDbName,
				lstrDatetime,
				GE_VIBCHANN,
				iTblType,
				loOleStartTime,
				loOleEndTime,
				lbUseSubTable
				);

			lbGetFromAlarm = FALSE;
		}
		//没有读到趋势数据
		if (!lbRet)
		{
			ASSERT(FALSE);

		}else
		{
			CBHDateTime loFindTimeStart;
			CBHDateTime loFindTimeEnd;

			CBHDateTime loReqStart = loOleStartTime;
			CBHDateTime loReqEnd   = loOleEndTime;

			BOOL lbDateFound = FALSE;
			CString lstrReqStart = aoletimeStart.Format("%H:%M:%S");

			while (lbRet)
			{

				CBHDateTime loDatetime = arefDB.GetTime(gc_cDateTime);
				CString lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
				CString lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

				if (!lbDateFound)
				{
					loFindTimeStart = loDatetime;
					loFindTimeEnd = loDatetime;
					lbDateFound = TRUE;
				}

				if (loDatetime < aoletimeStart)
				{
					loFindTimeStart = loDatetime;
				}

				if (loDatetime > aoletimeEnd)
				{
					loFindTimeEnd = loDatetime;

					lstrDatetimeStart	= loFindTimeStart.Format("%H:%M:%S");
					lstrDatetimeEnd		= loFindTimeEnd.Format("%H:%M:%S");

					break;
				}

				lbRet = arefDB.More();			

			}


			ASSERT(lbDateFound);

			if (lbDateFound)
			{
				loOleStartTime = loFindTimeStart;
				loOleEndTime = loFindTimeEnd;
			}
		}

	}



	
	//3.读取波形
	aorefTrendDataBefore.clear();

	if (lbGetFromAlarm)
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefTrendDataBefore,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannName.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			aiFsAfterAlarm_
			);
	}else
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefTrendDataBefore,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannName.GetBuffer(0),
			GD_400LINES,
			0,
			loOleStartTime,
			aiFsAfterAlarm_
			);
	}


	aiStartIndBeforeAlarm_ = 0;
	aiEndIndBeforeAlarm_ = aorefTrendDataBefore.size()/2;
	

	aorefTrendDataAfter.clear();

	if (lbGetFromAlarm)
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrumForAlarm(
			&arefDB,
			apPlantInfo,
			aorefTrendDataAfter,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannName.GetBuffer(0),
			GD_400LINES,
			0,
			loOleEndTime,
			aiFsAfterAlarm_
			);

	}else
	{
		SqlHelperForExpert::ReadVibChannSpecTimeWaveAndSpectrum(
			&arefDB,
			apPlantInfo,
			aorefTrendDataAfter,
			apPlantInfo->GetCompanyName(),
			apPlantInfo->GetFactoryName(),
			apPlantInfo->GetPlantID(),
			lstrChannName.GetBuffer(0),
			GD_400LINES,
			0,
			loOleEndTime,
			aiFsAfterAlarm_
			);
	}

	aiStartIndAfterAlarm_ = 0;
	aiEndIndAfterAlarm_ = aorefTrendDataAfter.size()/2;

	return aorefTrendDataAfter.size();
}
/**得到诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数
*@param aiSensorFaults_  [in/out] 气阀传感器有故障的个数
*@param aiAlarmChannels_ [in/out] 没有传感器故障的气阀报警通道数
*@param aiChannels_      [in/out] 所在缸的监测气阀温度的通道数
*/


void CImplement::GetCylinderValueAlarmsAndSensorFaults(
	IDBInterFace					& arefDB,
	ICommInterFace					& arefFixComm,
	const CPlantInfoExpertSys		* apPlantInfo,
	const S_ChannInfo				& arefChanninfo,
	CBHDateTime					& aoletimeStart,
	CBHDateTime					& aoletimeEnd,
	unsigned int					& aiSensorFaults_, 		
	unsigned int					& aiAlarmChannels_, 	
	int								& aiChannels_)
{
	if (!apPlantInfo)
	{
		return ;
	}
	
	//得到发生报警的全部通道的名称和时间的sql
	CString lstrSql;

	if (SqlHelperForExpert::GetSqlForSpecChannTypeSensorStatus(apPlantInfo,
															   arefChanninfo,
															   GE_OTHERCHANN,
															   aoletimeStart,
															   aoletimeEnd,
															   lstrSql,
															   EXPERT_CHANN_TYPE_ALL)<=0)
	{
		ASSERT(FALSE);

		return ;
	}

	arefDB.Cmd(lstrSql);

	std::map<CString,SqlParaStruct> loMapDate;

	while(arefDB.More())
	{
		CString lstrChann = arefDB.Get(gc_cChannNo).c_str();
		
		SqlParaStruct loStruct;
		loStruct.m_strName			= lstrChann;
		loStruct.m_oDateTimeStart	= arefDB.GetTime(gc_cStart_T);
		loStruct.m_oDateTimeEnd		= loStruct.m_oDateTimeStart + COleDateTimeSpan(0,0,0,5);
		loStruct.m_ChannType		= (CHANNTYPE)arefDB.GetLong(gc_cChannType);

		loMapDate[lstrChann] = loStruct;
	}

	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool lbNeedSubTbl = false;

	int lnAbnormalCount = 0;

	for(std::map<CString,SqlParaStruct>::iterator loAlarmChann = loMapDate.begin();
		loAlarmChann != loMapDate.end();
		loAlarmChann++)
	{
		//得到指定时间点和测点NO的趋势
		CString lstrChannNameTemp = loAlarmChann->first;
		CString lstrChannName = lstrChannNameTemp;
		CString lstrFiledTemp;
		CHANNTYPE lnType =loAlarmChann->second.m_ChannType;
	
		switch(lnType)
		{
		case  GE_VIBCHANN:
		case  GE_AXIALCHANN:
		case  GE_DYNPRESSCHANN:
		case  GE_RODSINKCHANN:
		case  GE_DYNSTRESSCHANN:
		case  GE_AXISLOCATIONCHANN:
			lstrFiledTemp.Format(_T(" [ALL_%s] "),lstrChannNameTemp);
			lstrChannNameTemp =  SqlHelperForExpert::ConsChannField(lstrChannNameTemp.GetBuffer(0),loAlarmChann->second.m_ChannType,false) +"  , " + this->ConsCommonField();
			break;
		default:
			lstrFiledTemp = gc_cProcValue;
			lstrChannNameTemp = " ["+lstrChannNameTemp+"] ";

		}
		
		std::string lstrDB = SqlHelperForExpert::GetSpecPlantDBName(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);

		std::string lstrSuffix = SqlHelperForExpert::GetSpecPlantTableSuffix(*((CPlantInfoBase *)apPlantInfo)).GetBuffer(0);


		BOOL lbNormal =GE_DATA_VALUE_NO_ERROR;

		S_VibCharValue  loVibValue;

		S_DynCharValue  loDynValue;

		bool lbRet = SqlHelperForExpert::GetTrendRecords(&arefDB,
											lstrSuffix,
											lstrDB,
											lstrChannNameTemp.GetBuffer(0),
											loAlarmChann->second.m_ChannType,
											iTblType,
											loAlarmChann->second.m_oDateTimeStart,
											loAlarmChann->second.m_oDateTimeEnd,
											lbNeedSubTbl
											);

		if (lbRet)
		{
			switch(loAlarmChann->second.m_ChannType)
			{
			case  GE_VIBCHANN:
			case  GE_AXIALCHANN:
			case  GE_AXISLOCATIONCHANN:				
				loVibValue= this->ReadVibCharValue(lstrChannName.GetBuffer(0),arefDB.getPDB());
				lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loVibValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);
				break;

			case  GE_DYNPRESSCHANN:
			case  GE_RODSINKCHANN:
			case  GE_DYNSTRESSCHANN:
				loDynValue = SqlHelperForExpert::ReadDynCharValue(lstrChannName.GetBuffer(0),&arefDB);
				lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(&loDynValue,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);
				break;
			
			default:
				double ldblProc = arefDB.GetDouble(lstrChannName.GetBuffer(0));
				lbNormal = CBHChannValueJudgeValidHelper::AbnormalData_IsCharValid(ldblProc,(S_ChannInfo *)&arefChanninfo,(CPlantInfoBase *)apPlantInfo);

			}

		}

		if (lbNormal != GE_DATA_VALUE_NO_ERROR)
		{
			lnAbnormalCount++;
		}
	}

	int lnProcNum = apPlantInfo->GetProcChannNum();

	for (int i =0;i<lnProcNum;i++)
	{
		S_ChannInfo  loChannInfo =((CPlantInfoExpertSys *)apPlantInfo)->GetSpecProcChannInfo(i);
		
		if (loChannInfo._iCylinderNo == arefChanninfo._iCylinderNo)
		{
			aiChannels_++;
		}
	}

	aiSensorFaults_ = lnAbnormalCount;

	aiAlarmChannels_ = loMapDate.size() - aiSensorFaults_;

	return;	
}

BOOL CImplement::ParseTrendAlarmHighLowValue(
												std::vector<double>			    &	aorefTrendData,
												const S_ChannInfo				&	anRefChannInfo,
												double					        &	aRefFHiAlarmValue_,
												double				            &	aRefFLoAlarmValue_)
{
	int i =0;
	double lbMax = 0;
	double lbMin = 0;
	int lnDetectAlarmType = 0;
	const S_ChannInfo & loInfo = anRefChannInfo;
	BOOL aBHiPass_ = FALSE;

	switch(loInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
		aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
		aBHiPass_ = FALSE;
		break;
	case E_ALARMCHECK_HIGHPASS:
		aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmLL;
		aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmLH;
		aBHiPass_ = TRUE;
		break;
	case E_ALARMCHECK_BANDPASS:

		if (aorefTrendData.size()>0)
		{
			lbMax = aorefTrendData[0];
			lbMin = aorefTrendData[0];

			for (i =0;i<aorefTrendData.size();i++)
			{
				if (lbMax <= aorefTrendData[i])
				{
					lbMax = aorefTrendData[i];
				}
				if (lbMin >=  aorefTrendData[i])
				{
					lbMin = aorefTrendData[i];
				}
			}
			if (lbMax>=  loInfo._chAlarmParam._fAlarmHL )
			{
				lnDetectAlarmType = lnDetectAlarmType | 0x01;
			}

			if (lbMin <=  loInfo._chAlarmParam._fAlarmLH)
			{
				lnDetectAlarmType = lnDetectAlarmType | 0x10;
			}

			switch(lnDetectAlarmType)
			{
			case 0x0:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
				aBHiPass_ = FALSE;
				break;

			case 0x01:
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
				aBHiPass_ = FALSE;

			case 0x10:

				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmLH;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmLL;
				aBHiPass_ = TRUE;
				break;	

			case 0x11:
				if (abs(lbMax-loInfo._chAlarmParam._fAlarmHL)>abs(lbMin - loInfo._chAlarmParam._fAlarmLH))
				{
					aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
					aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
					aBHiPass_ = FALSE;
				}else
				{
					aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmLH;
					aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmLL;
					aBHiPass_ = TRUE;
				}

				break;	

			default:
				ASSERT(FALSE);
				aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
				aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
				aBHiPass_ = FALSE;
			}
		}else
		{
			aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
			aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
			aBHiPass_ = FALSE;
		}

		break;
	case E_ALARMCHECK_BANDSTOP:

		aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
		aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
		aBHiPass_ = FALSE;
		ASSERT(FALSE);

	default:
		ASSERT(FALSE);
		aRefFLoAlarmValue_ =  loInfo._chAlarmParam._fAlarmHL;
		aRefFHiAlarmValue_ =  loInfo._chAlarmParam._fAlarmHH;
		aBHiPass_ = FALSE;
		break;
	}

	return aBHiPass_;
}

CHANNTYPE CImplement::TranlateChannType(ALARM_CHANN_TYPE aenumType)
{
	CHANNTYPE anChannType = GE_OTHERCHANN;

	switch(aenumType)
	{
		//需要确定曲轴箱测点，十字头测点，以及偏摆量测点的对应的通道类型 to be fixed!!!
	case EXPERT_CHANN_TYPE_CRANK:
		anChannType = GE_VIBCHANN;
		break;
	case EXPERT_CHANN_TYPE_CROSSHEAD:
		anChannType = GE_VIBCHANN;
		break;
	case EXPERT_CHANN_TYPE_DEFLECT:
		anChannType = GE_RODSINKCHANN;
		break;
	case EXPERT_CHANN_TYPE_IMPACTS:
		anChannType = GE_IMPACTCHANN;
		break;
	case EXPERT_CHANN_TYPE_PV:
		anChannType = GE_DYNPRESSCHANN;
		break;
	case EXPERT_CHANN_TYPE_SINK:
		anChannType = GE_RODSINKCHANN;
		break;
	case EXPERT_CHANN_TYPE_TEMP_INVAL:
	case EXPERT_CHANN_TYPE_TEMP:
		anChannType = GE_TEMPCHANN;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return anChannType;
}

int CImplement::GetSpecTypeChanns(const CPlantInfoExpertSys	* apPlantInfo,ALARM_CHANN_TYPE arChannType, vector<S_ChannInfo> &arChannInfo)
{
	if (!apPlantInfo)
	{
		return 0;
	}

	switch(arChannType)
	{
	case  EXPERT_CHANN_TYPE_TEMP_INVAL:
		for (int i = 0; i < apPlantInfo->_vProcChann.size(); i++)
		{
			S_ChannInfo loChannInfo = apPlantInfo->_vProcChann[i]->GetChannInfo();

			if (loChannInfo._iChannType == GE_TEMPCHANN && loChannInfo._iLocationNo == TEMP_INVAL_LOCATION_TYPE)
			{
				arChannInfo.push_back(loChannInfo);
			}
		}
		break;
	case  EXPERT_CHANN_TYPE_TEMP:
		{
			for (int i = 0; i < apPlantInfo->_vProcChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vProcChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_TEMPCHANN && loChannInfo._iLocationNo == TEMP_OUTVAL_LOCATION_TYPE)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}

		break;
	case  EXPERT_CHANN_TYPE_SINK:
		{
			for (int i = 0; i < apPlantInfo->_vDynChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vDynChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_RODSINKCHANN && loChannInfo._iMeassureDirection == MEASURE_DIRECTION_VERTICAL)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}
		break;
	case  EXPERT_CHANN_TYPE_CROSSHEAD:	
		{
			for (int i = 0; i < apPlantInfo->_vVibChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vVibChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_VIBCHANN && loChannInfo._iLocationNo == CROSSS_HEAD_LOCATION_TYPE)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}
		break;
	case  EXPERT_CHANN_TYPE_IMPACTS:
		{
			for (int i = 0; i < apPlantInfo->_vProcChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vProcChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_IMPACTCHANN)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}

		break;
	case  EXPERT_CHANN_TYPE_CRANK:
		{
			for (int i = 0; i < apPlantInfo->_vVibChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vVibChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_VIBCHANN && loChannInfo._iCylinderNo == 0)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}

		break;
	case  EXPERT_CHANN_TYPE_DEFLECT:
		{
			for (int i = 0; i < apPlantInfo->_vDynChann.size(); i++)
			{
				S_ChannInfo loChannInfo = apPlantInfo->_vDynChann[i]->GetChannInfo();

				if (loChannInfo._iChannType == GE_RODSINKCHANN && loChannInfo._iMeassureDirection == MEASURE_DIRECTION_HORIZONTAL)
				{
					arChannInfo.push_back(loChannInfo);
				}
			}
		}
		break;
	case  EXPERT_CHANN_TYPE_PV:

		break;
	default:

		break;
	}

	return arChannInfo.size();
}

int CImplement::readfile(double * ag_fwave,int len_,CString strFile_)
{
	ifstream fileWave;

	fileWave.open(strFile_,ios::in);
	for(int i=0;i<len_;i++)
		fileWave >> ag_fwave[i];
	fileWave.close();
	return len_;
}