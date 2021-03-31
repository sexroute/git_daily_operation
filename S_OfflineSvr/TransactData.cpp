#include "stdafx.h"
#include ".\transactdata.h"
#include "off_globe.h"
#include "ZoomAnalysis.h"
#include "zipimpl.h"
#include "MiddleWare.h"
#include "additions/DebugHelper.h"
#include "additions/BHOleDbWrapper.h"
#include <math.h>
#include <fstream>

CTransactData::CTransactData(void)
{
}

CTransactData::~CTransactData(void)
{
}



/**得到当前趋势，当前的趋势都是72条记录
* @param：  输入参数字段码
*          公司ID       FIELD_SYSTEM_COMPANY
*		   分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          趋势类型     FIELD_SERVICE_OFF_TRENDTYPE
*          特征值类型   FIELD_SERVICE_TREND_CHARTYPE
*          测点类型：   FIELD_SERVICE_OFF_CHANNTYPE
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
* @return  返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*/
#define     SERVICE_SERVICE_OFF_GETCURTREND           21001

int CTransactData::GetCurTrend(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	int iTrendType=pFixComm->GetLong(FIELD_SERVICE_OFF_TRENDTYPE);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_OFF_CHANNTYPE);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	int  index=0;
	CBHDateTime timestart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeend=CBHDateTime::GetCurrentTime();
	//通过趋势类型得到当前趋势的起始时间
	switch(iTrendType)
	{
		//    case GE_ONEMINUTETREND:    timestart=timeend-COleDateTimeSpan(0,0,1,10);	break;
	case GE_FIVEMINUTETREND:
		timestart=timeend-COleDateTimeSpan(0,0,5,10);
		break;
		//	case GE_TENMINUTETREND:	    timestart=timeend-COleDateTimeSpan(0,0,10,10);		break;
	case GE_HALFHOURTREND:
		timestart=timeend-COleDateTimeSpan(0,0,31,10);
		break;
	case GE_HOURTREND:
		timestart=timeend-COleDateTimeSpan(0,1,10,0);
		break;
	case GE_DAYTREND:
		timestart=timeend-COleDateTimeSpan(1,1,0,0);
		break;
		//	case GE_WEEKTREND:		timestart=timeend-COleDateTimeSpan(7,2,0,0);		break;
	case GE_MONTHTREND:
		timestart=timeend-COleDateTimeSpan(30,2,0,0);
		break;
	case GE_QUARTERTREND:
		timestart=timeend-COleDateTimeSpan(90,3,0,0);
		break;
	case GE_YEARTREND:
		timestart=timeend-COleDateTimeSpan(365,10,0,0);
		break;
	default:
		timestart=timeend-COleDateTimeSpan(0,1,10,0);
		break;
	}
	int iType=GetChannOneType((CHANNTYPE)iChannType);
	if (iType == E_TBL_CHANNTYPE_VIB)
		index=ReadVibTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timestart,timeend,true);
	else if(iType == E_TBL_CHANNTYPE_PROC)
		index=ReadProcTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timestart,timeend,true);
	else
		index = ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timestart,timeend,true);
	delete [] pChannBuff;
	return index;
}

/**得到指定公司、分厂、机组、时间段、任意多个振动测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          转速Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETVIBCHANNSTREND         21017
*/
int CTransactData::GetVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//CString strTmp;strTmp.Format("iChannType:%d",iChannType);BHTRACE_DEBUG(strTmp);
	if (iChannType<=0)
	{//如果通道类型是0，则说明是过程量测点，该接口只处理振动测点和动态测点，则加1后为振动测点
		iChannType = GE_VIBCHANN;
	}
	//CString strTmp;strTmp.Format("%d,%d,%s,%d",iChannType,iBytes,pChannBuff,GetChannOneType((CHANNTYPE)iChannType));AfxMessageBox(strTmp);
	int nCount=0;
	if (GetChannOneType((CHANNTYPE)iChannType)==E_TBL_CHANNTYPE_VIB)
	{	nCount=ReadVibTrendAllCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timeStart,timeEnd);
	}
	else
	{
		nCount=ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,-5,timeStart,timeEnd);
	}
	delete [] pChannBuff;
	return nCount;
}

/**得到指定公司、分厂、机组、时间段、任意多个振动测点测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *输入参数字段码：
  *          公司ID        FIELD_SYSTEM_COMPANY
  *			 分厂名        FIELD_SYSTEM_FACTORY
  *          机组ID        FIELD_SERVICE_PLANTID
  *          测点类型：    FIELD_SERVICE_CHANNTYPE
  *          测点buff      FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数      FIELD_SERVICE_CHANN_NUM
  *          起始时间      FIELD_SERVICE_OFF_TIMESTART
  *          终止时间      FIELD_SERVICE_OFF_TIMEEND
  *          查询的数据类型 FIELD_SYSTEM_TYPE（0:只有常规数据，1:包含报警数据 2:包含启停车数据）
  *返回值的字段码：
  *          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
  *          测点个数             FIELD_SERVICE_CHANN_NUM
  *          时间值               FIELD_SERVICE_OFF_TIME
  *          转速Buff             FIELD_SERVICE_REV_BUFF
  * #define     SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX         21081
*/
int CTransactData::GetVibChannsTrendEx(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int lnDataType = pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//CString strTmp;strTmp.Format("iChannType:%d",iChannType);BHTRACE_DEBUG(strTmp);
	if (iChannType<=0)
	{//如果通道类型是0，则说明是过程量测点，该接口只处理振动测点和动态测点，则加1后为振动测点
		iChannType = GE_VIBCHANN;
	}
	//CString strTmp;strTmp.Format("%d,%d,%s,%d",iChannType,iBytes,pChannBuff,GetChannOneType((CHANNTYPE)iChannType));AfxMessageBox(strTmp);

	int nCount=ReadVibTrendAllCharValueEx(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timeStart,timeEnd,iChannType);

	delete [] pChannBuff;
	return nCount;
}

/**得到当前时间向后推一个趋势类型的趋势数据。指定公司、分厂、机组、任意多个振动测点的所有特征值趋势数据，
*通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          趋势类型      FIELD_SERVICE_OFF_TRENDTYPE
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          转速Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND      21018
*/
int CTransactData::GetCurrVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iTrendType = pFixComm->GetLong(FIELD_SERVICE_OFF_TRENDTYPE);
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;

	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//	CString strTmp;strTmp.Format("%d,%s",iBytes,pChannBuff);AfxMessageBox(strTmp);
	CBHDateTime timestart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeend=CBHDateTime::GetCurrentTime();
	//通过趋势类型得到当前趋势的起始时间
	switch(iTrendType)
	{
	case GE_FIVEMINUTETREND:
		timestart=timeend-COleDateTimeSpan(0,0,5,10);
		break;
	case GE_HALFHOURTREND:
		timestart=timeend-COleDateTimeSpan(0,0,31,10);
		break;
	case GE_HOURTREND:
		timestart=timeend-COleDateTimeSpan(0,1,10,0);
		break;
	case GE_DAYTREND:
		timestart=timeend-COleDateTimeSpan(1,1,0,0);
		break;
	case GE_MONTHTREND:
		timestart=timeend-COleDateTimeSpan(30,2,0,0);
		break;
	case GE_QUARTERTREND:
		timestart=timeend-COleDateTimeSpan(90,3,0,0);
		break;
	case GE_YEARTREND:
		timestart=timeend-COleDateTimeSpan(365,10,0,0);
		break;
	case GE_WEEKTREND:
		timestart=timeend-COleDateTimeSpan(7,1,0,0);
		break;
	default:
		timestart=timeend-COleDateTimeSpan(0,1,10,0);
	}
	int nCount=0;
	if (iChannType <= 0)
		iChannType = GE_VIBCHANN;
	//	CString strInfo;	strInfo.Format("通道类型：%d，表类型：%d", iChannType, GetChannOneType((CHANNTYPE)iChannType) );
	//	CHZLogManage::Info( strInfo, "Offline", "CTransactData::GetCurrVibChannsTrend");
	if (GetChannOneType((CHANNTYPE)iChannType) == E_TBL_CHANNTYPE_VIB)
		nCount=ReadVibTrendAllCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timestart,timeend,true);
	else
		nCount=ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,-5,timestart,timeend,true);
	KD_DeletePoint(pChannBuff);
	return nCount;
}

/**得到指定两个振动测点的历史波形（循环两次得到两个测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*			 分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一          FIELD_SERVICE_OFF_CHANN1
*          测点一时间值          FIELD_SERVICE_OFF_TIME
*          公司ID2         FIELD_SYSTEM_COMPANY2
*			 分厂名2         FIELD_SYSTEM_FACTORY2
*          机组ID2         FIELD_SERVICE_PLANTID2
*          测点二          FIELD_SERVICE_OFF_CHANN2
*          测点二时间值          FIELD_SERVICE_OFF_TIME2
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数        FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*返回值的字段码：
*          公司ID          FIELD_SYSTEM_COMPANY
*			 分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点ID          FIELD_SERVICE_CHANN1
*          波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDATA           21019
*/
int CTransactData::GetTwoChannData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount=-1;
	string sCompany1=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory1=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant1=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sChann1=pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	string sCompany2=pFixComm->Get(FIELD_SYSTEM_COMPANY2);
	string sFactory2=pFixComm->Get(FIELD_SYSTEM_FACTORY2);
	string sPlant2=pFixComm->Get(FIELD_SERVICE_PLANTID2);
	string sChann2=pFixComm->Get(FIELD_SERVICE_OFF_CHANN2);
	int   iLines=pFixComm->GetLong(FIELD_SERVICE_OFF_FREQANALYSIS_LINES);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff1=CBHDateTime::GetCurrentTime();
	CBHDateTime timeOff2=CBHDateTime::GetCurrentTime();
	timeOff1=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	timeOff2=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME2);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if(!(ReadVibChannSpecTimeWaveAndSpectrum(pFixComm,pDB,sCompany1,sFactory1,sPlant1,sChann1,iLines,iWaveNum,timeOff1)<0))
	{
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann1.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany1.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory1.c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant1.c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	nCount=ReadVibChannSpecTimeWaveAndSpectrum(pFixComm,pDB,sCompany2,sFactory2,sPlant2,sChann2,iLines,iWaveNum,timeOff2);
	if (nCount>0)
	{
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann2.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany2.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory2.c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant2.c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

// 得到表名.小时趋势表中两条记录的间隙为一分钟，天趋势表中两条记录间隙为20分钟，月趋势表中两条记录的间隔为720分钟（12小时）
CString CTransactData::GetTblNamePrefixForTrend(int ag_iChannType,CBHDateTime ag_timeStart,CBHDateTime ag_timeEnd)
{
	CString strTable,strType;
	COleDateTimeSpan span,spanCurr;

	switch(ag_iChannType)
	{
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
		strTable="TREND_VIB";
		break;
	default:
		strTable="TREND_PROC";
	}
	//秒趋势（当前趋势）表单中，3秒钟保存一次，12小时的记录
	//小时趋势表单中,1分钟保存一次，360天的记录
	//天趋势表单中，20分钟保存一次，3*360天的记录
	//保存月趋势的间隔，3小时一次,一直保存,不删除
	/*判断从起始时间和时间间隔来判断从那个表中取数据
	规则：
	1、如果起始时间和当前时间相差在12小时之内:
	起始时间和结束时间间隔在6分钟之内，在秒趋势表单中取数据;
	起始时间和结束时间间隔超过6分钟,在小时表中取数据；
	2、如果起始时间和当前时间相差在360天之内:
	起始时间和结束时间时间间隔在2小时之内，则在小时趋势表单中取数据
	起始时间和结束时间时间间隔在2小时和720小时之间,否则在天趋势表单中取数据；
	起始时间和结束时间时间间隔超过720小时,就在月趋势表单中取数据。
	3、如果起始时间和当前时间相差在1080天之内：
	起始时间和结束时间时间间隔在2天之内，则在天趋势表单中取数据；
	起始时间和结束时间时间间隔如果超过2天，月趋势表单中取数据
	4、如果起始时间和当前时间相差超过1080天，则在月趋势表单中取数据。
	*/
	span=ag_timeEnd-ag_timeStart;
	spanCurr=CBHDateTime::GetCurrentTime()-ag_timeStart;
	int iDays=spanCurr.GetTotalDays();
	int iHours=spanCurr.GetTotalHours();
	if (iHours<12)//秒表保存范围
	{
		if((span.GetTotalMinutes())<6)
			strType="SECOND_";
		else
			strType="HOUR_";
	}
	else if (iDays<360)//小时表保存范围
	{
		iHours=span.GetTotalHours();
		if (iHours<2)
			strType="HOUR_";
		else if (iHours<720)
			strType="DAY_";
		else
			strType="MONTH_";
	}
	else if (iDays<1080)//天表保存范围
	{
		if ((span.GetTotalDays())<2)
			strType="DAY_";
		else
			strType="MONTH_";
	}
	else
		strType="MONTH_";

	strTable=strTable+strType;
	return strTable;
}

// 读取振动趋势
int CTransactData::ReadVibTrendSpecType(ICommInterFace * pFixComm, 
										IDBInterFace *pDB,
										string sCompany_,
										string sFactory_,
										string sPlant_,
										char *pcChannBuf_,
										int iChannNum_,
										int iCharType_,
										CBHDateTime timeStart_,
										CBHDateTime timeEnd_,
										bool bCurr_)
{
	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendSpecType");
	list<string>::iterator itChann;
	list<string>::iterator itChannRevField;
	list<string> lChannRevField;
	list<string> lChannField;
	//假如为true，说明所代表测点为空值
	int index=0;
	lCHANNID_NO lChannPair=ParseVibChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	lCHANNID_NO::iterator itPair;
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		if (itPair->second.c_str() =="")
			continue;
		lChannField.push_back(ConsVibChannSpecTypeField(itPair->second,(CHARTYPE)iCharType_));
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecVibChannInfo(itPair->first)._iRevType));
	}

	iChannNum_=lChannField.size();
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum_];
	int           *piRev  = new int[iChannNum_];
	//需要获得的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append("[");
		sShowField.append(*itChann);
		sShowField.append("],");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	int nloop=0;
	int iInterval=0;
	int iNums=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool bRead=false;
	bool bNeedSubTbl=false;//是否需要从下一级表中读取数据
	int iTmp=0;
	//得到趋势记录
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
	if(iNums > 0)
	{
		
		iInterval=iNums/D_TRENDRECORDS;
		
		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);
		if(bRead)
		{
			index = 0;
			do
			{
				index++;
				nloop=0;
				itChannRevField = lChannRevField.begin();
				for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
				{
					pfValue[nloop]=pDB->GetDouble(itChann->c_str());
					piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
					++itChannRevField;
					++nloop;
				}
				//	CString strTmp;strTmp.Format("ddd %.2f,%.2f",pfValue[0],pfValue[1]);AfxMessageBox(strTmp);
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
				
			}while(pDB->More());
		}
	}
	else
	{		
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);

		//开始取数据
		if (bRead)
		{//如果需要从下一级表中读取数据，不需要间隔判断，否需要间隔取数
			if (bNeedSubTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if (iInterval>1)
					iInterval=iInterval-1;
				else
					iInterval=0;
			}

			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pfValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						++itChannRevField;
						++nloop;
					}
					//	CString strTmp;strTmp.Format("ddd %.2f,%.2f",pfValue[0],pfValue[1]);AfxMessageBox(strTmp);
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());

		}
	}
	
	if (bNeedSubTbl)
	{
RECALL_READDATA:
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN ,iTblType),sSuffix.c_str());
		pDB->Cmd("SELECT %s from [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ORDER BY [%s]",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_),gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if (iInterval>1)
				iInterval=iInterval-1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pfValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			itChannRevField = lChannRevField.begin();
			for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
			{
				pfValue[nloop]=pDB->GetDouble(itChann->c_str());
				//				CString strTmp;strTmp.Format("dd %s",itChann->c_str());AfxMessageBox(strTmp);
				piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
				++itChannRevField;
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	if(NULL!=pfValue) delete [] pfValue;
	KD_DeletePoint(piRev);
	lChannField.clear();
	lChannPair.clear();
	lChannRevField.clear();
	return index;
}

// 读取动态测点趋势
int CTransactData::ReadDynTrendSpecType(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
										int iCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynTrendSpecType");
	list<string>::iterator itChann;
	list<string>::iterator itChannRevField;
	list<string> lChannRevField;
	list<string> lChannField;
	//假如为true，说明所代表测点为空值
	int index=0;
	lCHANNID_NO lChannPair=ParseDynChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	lCHANNID_NO::iterator itPair;

	if (-5 == iCharType_ )
	{///得到所有特征值
		string sTmp;
		for ( itPair=lChannPair.begin(); itPair!=lChannPair.end(); ++itPair)
		{
			//CString sTemp;sTemp.Format("测点名:%s",(itMap->second).c_str());
			//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynTrendSpecType");
			if (CString(itPair->second.c_str())=="")
				continue;
			sTmp=ConsDynChannField( itPair->second );
			lChannField.push_back(sTmp);
			lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecDynChannInfo(itPair->first)._iRevType));

		}
	}
	else
	{
		for ( itPair=lChannPair.begin(); itPair != lChannPair.end(); ++itPair)
		{
			if (CString(itPair->second.c_str())=="")
				continue;
			lChannField.push_back( ConsDynChannSpecTypeField(itPair->second,(CHARTYPE)iCharType_));
			lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecDynChannInfo(itPair->first)._iRevType));

		}
	}

	iChannNum_=lChannField.size();
	S_DynCharValue *pValueS= NULL;
	DATATYPE_CHAR  *pValue = NULL;
	int           *piRev  = new int[iChannNum_];
	if (-5 == iCharType_)
		pValueS= new S_DynCharValue[iChannNum_];
	else
		pValue= new DATATYPE_CHAR[iChannNum_];
	//需要获得的字段
	string sShowField="";
	BOOL lbReadFalse = TRUE;
	for ( itChann=lChannField.begin(); itChann!=lChannField.end(); ++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");

		lbReadFalse = FALSE;
	}

	if (lbReadFalse )
	{
		KD_DeletePoint(piRev);
		KD_DeletePoint(pValueS);
		KD_DeletePoint(pValue);
		return -1;
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	int nloop=0;
	int iInterval=0;
	int iNums=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool bRead=false;
	bool bNeedSubTbl=false;//是否需要从下一级表中读取数据
	int iTmp=0;
	//得到趋势记录
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
	
	//开始取数据
	if (iNums > 0)
	{//如果需要从下一级表中读取数据，不需要间隔判断，否需要间隔取数
		
		iInterval=iNums/D_TRENDRECORDS;

		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);

		if(bRead)
		{
			index = 0;
			do
			{
				index++;
				nloop=0;
				if (-5 == iCharType_)
				{
					itChannRevField = lChannRevField.begin();
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						pValueS[nloop]=ReadDynCharValue(itPair->second,pDB);
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pValueS,sizeof(S_DynCharValue)*iChannNum_);
				}
				else
				{
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				}
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
			
			}while(pDB->More());
		}


	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
		if(bRead)
		{
			if (bNeedSubTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if (iInterval>1)
					iInterval=iInterval-1;
				else
					iInterval=0;
			}
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					if (-5 == iCharType_)
					{
						itChannRevField = lChannRevField.begin();
						for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
						{
							//CString sTemp;sTemp.Format("%s",(itMap->second).c_str());
							//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynTrendSpecType");
							pValueS[nloop]=ReadDynCharValue(itPair->second,pDB);
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pValueS,sizeof(S_DynCharValue)*iChannNum_);
					}
					else
					{
						itChannRevField = lChannRevField.begin();
						for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
						{
							pValue[nloop]=pDB->GetDouble(itChann->c_str());
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					}
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		
	}
	if (bNeedSubTbl)
	{
RECALL_READDATA:
		strTable.Format("%s%s", GetTblNamePrefix( GE_DYNPRESSCHANN, iTblType), sSuffix.c_str());
		pDB->Cmd("SELECT %s from [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if (iInterval>1)
				iInterval=iInterval-1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					if (-5 == iCharType_)
					{
						itChannRevField = lChannRevField.begin();
						for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
						{
							pValueS[nloop]=ReadDynCharValue(itPair->second,pDB);
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pValueS,sizeof(S_DynCharValue)*iChannNum_);
					}
					else
					{
						itChannRevField = lChannRevField.begin();
						for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
						{
							pValue[nloop]=pDB->GetDouble(itChann->c_str());
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					}
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_DYNSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			if (-5 == iCharType_)
			{
				itChannRevField = lChannRevField.begin();
				for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
				{
					pValueS[nloop]=ReadDynCharValue(itPair->second,pDB);
					piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pValueS,sizeof(S_DynCharValue)*iChannNum_);
			}
			else
			{
				itChannRevField = lChannRevField.begin();
				for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
				{
					pValue[nloop]=pDB->GetDouble(itChann->c_str());
					piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			}
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	KD_DeletePoint(pValue);
	KD_DeletePoint(pValueS);
    KD_DeletePoint(piRev);
	lChannField.clear();
	lChannPair.clear();
	lChannRevField.clear();
	return index;
}

int CTransactData::ReadVibTrendAllCharValue(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	list<string> lChannField;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	list<string>::iterator itChann;
	lCHANNID_NO lChannPair;
	lCHANNID_NO::iterator itPair;
	int index=0; 
	int nloop=0;
	string sTmp;
	lChannPair=ParseVibChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(itMapChann->second).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendAllCharValue");
		if (itPair->second.c_str()=="")
		   continue;
		sTmp=ConsVibChannField(itPair->second);
		lChannField.push_back(sTmp);
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecVibChannInfo(itPair->first)._iRevType));
	}
	iChannNum_=lChannPair.size();

	S_VibCharValue *pfVibValue=new S_VibCharValue[iChannNum_];
	int            *piRev     =new int[iChannNum_];
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(ConsCommonField());
	sShowField.append(",");
	sShowField.append(gc_cTime);
	bool bRead=false;
	bool bNeedSudTbl=false;
	int iNums,iInterval,iTmp;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();
	iNums = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
	if(iNums > 0)
	{
		
		iInterval=iNums/D_TRENDRECORDS;
	
		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl,iInterval);
		if(bRead)
		{
			index=0;
			do
			{
				index++;
				nloop=0;
				itChannRevField = lChannRevField.begin();
				for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
				{
					pfVibValue[nloop]=ReadVibCharValue(itPair->second,pDB);
					piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
					//当获取到的转速小于0时,则显示转速为0
					if (piRev[nloop]<0)
						piRev[nloop] = 0;
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pfVibValue,sizeof(S_VibCharValue)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
				
			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
		if (bRead)
		{
			if (bNeedSudTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if(iInterval>1)
					iInterval-=1;
				else
					iInterval=0;
			}
			//CString sTmp;sTmp.Format("iNums:%d,iInterval:%d",iNums,iInterval);BHTRACE_DEBUG(sTmp);

		 
			iTmp=0;
			index=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						pfVibValue[nloop]=ReadVibCharValue(itPair->second,pDB);
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						//当获取到的转速小于0时,则显示转速为0
						if (piRev[nloop]<0)
							piRev[nloop] = 0;
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pfVibValue,sizeof(S_VibCharValue)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());

		}
	}
	
	if (bNeedSudTbl)
	{ //跨表读取
RECALL_READDATA:
		strTable.Format("%s%s", GetTblNamePrefix( GE_VIBCHANN, iTblType), sSuffix.c_str());
		pDB->Cmd("SELECT %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if(iInterval>1)
				iInterval-=1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						pfVibValue[nloop]=ReadVibCharValue(itPair->second,pDB);
						piRev[nloop] = pDB->GetLong(itChannRevField->c_str());
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pfVibValue,sizeof(S_VibCharValue)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}

	if (bCurr_)
	{
		strTable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			itChannRevField = lChannRevField.begin();
			for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
			{
				pfVibValue[nloop]=ReadVibCharValue(itPair->second,pDB);
				piRev[nloop] = pDB->GetLong(itChannRevField->c_str());
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pfVibValue,sizeof(S_VibCharValue)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	if(NULL!=pfVibValue) delete [] pfVibValue;
	KD_DeletePoint(piRev);
	lChannField.clear();
	lChannPair.clear();
	return index;
}

int CTransactData::ReadVibTrendAllCharValueEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,CBHDateTime timeStart_,CBHDateTime timeEnd_, int iChannType)
{
	CString strTable;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
	{
		return -1;
	}
	SwitchDbNameEx(pDB,sSuffix.c_str());
	list<string> lChannField;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	list<string>::iterator itChann;
	lCHANNID_NO lChannPair;
	lCHANNID_NO::iterator itPair;
	int index=0; 
	int nloop=0;
	string sTmp;
	lChannPair=ParseVibChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		if (itPair->second.c_str()=="")
			continue;
		sTmp=ConsVibChannField(itPair->second);
		lChannField.push_back(sTmp);
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecVibChannInfo(itPair->first)._iRevType));
	}
	iChannNum_=lChannPair.size();

	S_VibCharValue *pfVibValue=new S_VibCharValue[iChannNum_];
	int            *piRev     =new int[iChannNum_];
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(ConsCommonField());
	sShowField.append(",");
	sShowField.append(gc_cTime);

	//将联合查询结果返回
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	//首先查报警表
	bool bNeedSudTbl=false;
	CString strTblAlarm,strTblRegular,strWhere,strTimeStart,strTimeEnd,strTblFast;
	int lnRealNums = 0;

	E_TREND_TBL_TYPE lenTblTypeFast = GE_TREND_TBLTYPE_FAST;

	strTblAlarm.Format("%s%s",GetTblNamePrefixAlarmSud((CHANNTYPE)iChannType,SEARCH_ALARM),sSuffix.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format(" WHERE [%s] BETWEEN '%s' AND '%s' ",gc_cTime,strTimeStart,strTimeEnd);

	strTblFast.Format("%s%s",GetTblNamePrefix(GE_VIBCHANN,lenTblTypeFast),sSuffix.c_str());

	CString strTimeArrayRegular,strTimeArrayAlarm,strTimeArrayFast;

	vector<CBHDateTime> lvTimeRegularTmp,lvTimeAlarmTmp,lvTimeFastTmp;

	vector<CBHDateTime> lvTimeRegular,lvTimeAlarm,lvTimeFast;

	set<CBHDateTime> lsTimeTmp;

	lvTimeRegularTmp = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblRegular,E_TBL_CHANNTYPE_VIB,false);

	lvTimeAlarmTmp   = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblAlarm,E_TBL_CHANNTYPE_VIB,true);

	lvTimeFastTmp    = this->GetTrendFastRecordTimeArray(pDB,sSuffix,GE_VIBCHANN,lenTblTypeFast,timeStart_,timeEnd_);


	for(size_t nloop = 0; nloop < lvTimeAlarmTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeAlarmTmp[nloop]).second)
		{
			lvTimeAlarm.push_back(lvTimeAlarmTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeRegularTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeRegularTmp[nloop]).second)
		{
			lvTimeRegular.push_back(lvTimeRegularTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeFastTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeFastTmp[nloop]).second)
		{
			lvTimeFast.push_back(lvTimeFastTmp[nloop]);
		}
	}

	strTimeArrayAlarm = GetTimeStringByTimeArray(lvTimeAlarm);

	strTimeArrayRegular = GetTimeStringByTimeArray(lvTimeRegular);

	strTimeArrayFast = GetTimeStringByTimeArray(lvTimeFast);
	
	
	if(strTimeArrayRegular.Trim().GetLength() > 0 || strTimeArrayAlarm.Trim().GetLength() > 0 || strTimeArrayFast.Trim().GetLength())
	{
		if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s) UNION(",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);

		}
		else if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() == 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblAlarm,strWhere);

		}
		else if(strTimeArrayRegular.Trim().GetLength() == 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblRegular,strWhere);
		}
		else
		{
			pDB->Cmd("SELECT %s FROM [%s] %s",
				sShowField.c_str(),strTblRegular,strWhere);
		}

		if(strTimeArrayFast.Trim().GetLength() > 0)
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblFast,gc_cTime,strTimeArrayFast);
		}
		else
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblFast,strWhere);
		}

		pDB->Cmd(" ORDER BY %s",gc_cTime);
	}
	else
	{
		pDB->Cmd("SELECT %s FROM [%s] %s UNION SELECT %s FROM [%s] %s ORDER BY [%s]",sShowField.c_str(),strTblAlarm,strWhere,sShowField.c_str(),strTblRegular,strWhere,gc_cTime);
	}

	//strTblRegular.Format("%s%s", GetTblNamePrefixForTrend2(GE_VIBCHANN, timeStart_,timeEnd_,lenTblType,bNeedSubTbl,bUseSupTbl), sSuffix.c_str());


	//pDB->Cmd("SELECT %s FROM [%s] %s UNION SELECT %s FROM [%s] %s ORDER BY [%s]",sShowField.c_str(),strTblAlarm,strWhere,sShowField.c_str(),strTblRegular,strWhere,gc_cTime);

	if (pDB->More())
	{
		lnRealNums=pDB->GetCount();

		if (lnRealNums < 1)
		{
			pFixComm->CreateEndMak();
			pFixComm->Write(pFixComm->GetAci());
			KD_DeletePoint(pfVibValue);
			KD_DeletePoint(piRev);
			return lnRealNums;
		}
	}
	else 
	{
		// 防止超时或其它异常，
		if(pDB->GetLastError() < 0)
		{
			lnRealNums=0;
		}

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
		KD_DeletePoint(pfVibValue);
		KD_DeletePoint(piRev);
		return 0;
	}

	////最多返回2000条报警记录
	/*int lnTotalNum = 2000;
	int lnIntervel = 0;
	if(lnRealNums > lnTotalNum)
	{
		lnIntervel = (int)lnRealNums/lnTotalNum;
	}*/

	index=0;
	int lnTemp = 0;
	vector<CBHDateTime> lvSendTime;
	do
	{
		CBHDateTime ltTime = pDB->GetTime(gc_cTime);
		if(std::find(lvSendTime.begin(),lvSendTime.end(),ltTime) != lvSendTime.end())
		{
			continue;
		}

		index++;
		nloop = 0;
		itChannRevField = lChannRevField.begin();
		for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
		{
			pfVibValue[nloop]=ReadVibCharValue(itPair->second,pDB);
			piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
			//当获取到的转速小于0时,则显示转速为0
			if (piRev[nloop]<0)
				piRev[nloop] = 0;
			++itChannRevField;
			++nloop;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pfVibValue,sizeof(S_VibCharValue)*iChannNum_);
		pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);

		lvSendTime.push_back(ltTime);
		pFixComm->Write( pFixComm->GetAci() );
		pFixComm->CreateNextAnswer();
		
		
	}while(pDB->More());

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	if(NULL!=pfVibValue) delete [] pfVibValue;
	KD_DeletePoint(piRev);
	lChannField.clear();
	lChannPair.clear();
	return index;
}


// 读过程量趋势
int CTransactData::ReadProcTrendSpecType(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
										 CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string sSuffix,sPrefix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//假如为true，说明所代表测点为空值
	int index=0;
	int nloop=0;
	lCHANNID_NO lChannPair;
	lCHANNID_NO::iterator itPair;
	DATATYPE_CHAR *pfValue=NULL;
	pfValue=new DATATYPE_CHAR[iChannNum_];
	memset(pfValue,0,iChannNum_*sizeof(DATATYPE_CHAR));
	string sShowField="";
	lChannPair=ParseProcChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(itMapChann->second).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadProcTrendSpecType");
		sShowField.append("[");
		sShowField.append(itPair->second);
		sShowField.append("],");
	}
	iChannNum_=lChannPair.size();

	sShowField.append(gc_cTime);
	bool bRead=false;
	bool bNeedSudTbl=false;
	int iNums=0,iInterval=0,iTmp=0;
	S_ChannInfo channInfo;
	index=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
	if(iNums > 0)
	{
		iInterval=iNums/D_TRENDRECORDS;
		
		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl,iInterval);

		if(bRead)
		{
			
			index = 0;
			do
			{
				index++;
				nloop=0;
				for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
				{
					pfValue[nloop]=pDB->GetDouble(itPair->second.c_str());
					channInfo = pPlantInfo->GetSpecProcChannInfo(itPair->first);
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
				pFixComm->Write  ( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
				
			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl);

		if(bRead)
		{
			if (bNeedSudTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if(iInterval>1)
					iInterval-=1;
				else
					iInterval=0;
			}
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						pfValue[nloop]=pDB->GetDouble(itPair->second.c_str());
						channInfo = pPlantInfo->GetSpecProcChannInfo(itPair->first);
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
					pFixComm->Write  ( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
	}
	
	if(bNeedSudTbl)
	{
RECALL_READ_RECORD_PROC1:
		strTable.Format("%s%s", GetTblNamePrefix( GE_ALLPROC, iTblType), sSuffix.c_str());
		pDB->Cmd("SELECT %s from [%s] ",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READ_RECORD_PROC1;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if(iInterval>1)
				iInterval-=1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						pfValue[nloop]=pDB->GetDouble(itPair->second.c_str());
						channInfo = pPlantInfo->GetSpecProcChannInfo(itPair->first);
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
					pFixComm->Write  ( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READ_RECORD_PROC1;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_PROCSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s  FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
			{
				pfValue[nloop]=pDB->GetDouble(itPair->second.c_str());
				channInfo = pPlantInfo->GetSpecProcChannInfo(itPair->first);
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChannPair.clear();
	if (NULL!=pfValue)  delete [] pfValue;
	return index;
}

// 读取振动测点自定义参数趋势
int CTransactData::ReadVibCustomParamTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	list<string> lChannField;
	list<string>::iterator itChann;
	lCHANNID_NO lChannPair;
	lCHANNID_NO::iterator itPair;
	int index=0;
	int nloop=0;
	CString strTmp;
	lChannPair=ParseVibChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(itMapChann->second).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendAllCharValue");
		strTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
		string sTmp=strTmp.GetString();
		lChannField.push_back(sTmp);
	}
	iChannNum_=lChannPair.size();

	S_CustomParamData  *pfCustomParom =new S_CustomParamData[iChannNum_];
	memset(pfCustomParom,0,iChannNum_*sizeof(S_CustomParamData));
	//构建要查询的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(gc_cTime);
	bool bRead=false;
	bool bNeedSudTbl=false;
	int iNums,iInterval,iTmp;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
	if(iNums > 0)
	{
		
		iInterval=iNums/D_TRENDRECORDS;
		
		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl,iInterval);
		if(bRead)
		{
			index=0;
			do
			{
				index++;
				nloop=0;
				for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
				{
					long iAllBytes=sizeof(S_CustomParamData);
					CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
					//得到自定义参数信息
					CWaveHead sWaveHead;
					SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes,sWaveHead);
					++nloop;
				}
				int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
				pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
				
			}while(pDB->More());
		}

	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSudTbl);

		if (bRead)
		{
			if (bNeedSudTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if(iInterval>1)
					iInterval-=1;
				else
					iInterval=0;
			}
			//CString sTmp;sTmp.Format("iNums:%d,iInterval:%d",iNums,iInterval);BHTRACE_DEBUG(sTmp);

		
			iTmp=0;
			index=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						long iAllBytes=sizeof(S_CustomParamData);
						CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
						//得到自定义参数信息
						CWaveHead sWaveHead;
						SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes,sWaveHead);
						++nloop;
					}
					int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());

		}
	}
	
	if (bNeedSudTbl)
	{ //跨表读取
RECALL_READDATA:
		strTable.Format("%s%s", GetTblNamePrefix( GE_VIBCHANN, iTblType), sSuffix.c_str());
		pDB->Cmd("SELECT %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if(iInterval>1)
				iInterval-=1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						long iAllBytes=sizeof(S_CustomParamData);
						CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
						//得到自定义参数信息
						CWaveHead sWaveHead;
						SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes,sWaveHead);
						++nloop;
					}
					int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
			{
				long iAllBytes=sizeof(S_CustomParamData);
				CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
				//得到自定义参数信息
				CWaveHead sWaveHead;
				SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes, sWaveHead);
				++nloop;
			}
			int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
			pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
    KD_DeletePoint(pfCustomParom);
	lChannField.clear();
	lChannPair.clear();
	return index;
}

// 读取动态测点自定义参数趋势
int CTransactData::ReadDynCustomParamTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
										CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynTrendSpecType");
	list<string>::iterator itChann;
	list<string> lChannField;
	//假如为true，说明所代表测点为空值
	int index=0;
	lCHANNID_NO lChannPair=ParseDynChannList(pPlantInfo,pcChannBuf_,iChannNum_);
	lCHANNID_NO::iterator itPair;
	S_CustomParamData  *pfCustomParom  =new S_CustomParamData[iChannNum_];
	memset(pfCustomParom,0,iChannNum_*sizeof(S_CustomParamData));
    CString strTmp;
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(itPair->second).c_str());BHTRACE_DEBUG(sTemp);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendAllCharValue");
		strTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
		string sTmp=strTmp.GetString();
		lChannField.push_back(sTmp);
	}
	iChannNum_=lChannField.size();
	//构建要查询的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(gc_cTime);
	int nloop=0;
	int iInterval=0;
	int iNums=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool bRead=false;
	bool bNeedSubTbl=false;//是否需要从下一级表中读取数据
	int iTmp=0;
	//得到趋势记录
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);

	if(iNums > 0)
	{
		iInterval=iNums/D_TRENDRECORDS;
		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);

		//开始取数据
		if (bRead)
		{
			
			index = 0;
			do
			{
				index++;
				nloop=0;
				for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
				{
					long iAllBytes=sizeof(S_CustomParamData);
					CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
					//得到自定义参数信息
					CWaveHead sWaveHead;
					SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes, sWaveHead);
					++nloop;
				}
				int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
				pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
				pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
			
			}while(pDB->More());
		}
			
	}

	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);

		//开始取数据
		if (bRead)
		{//如果需要从下一级表中读取数据，不需要间隔判断，否需要间隔取数
			if (bNeedSubTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if (iInterval>1)
					iInterval=iInterval-1;
				else
					iInterval=0;
			}
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						long iAllBytes=sizeof(S_CustomParamData);
						CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
						//得到自定义参数信息
						CWaveHead sWaveHead;
						SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes,sWaveHead);
						++nloop;
					}
					int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
	}
	
	if (bNeedSubTbl)
	{
RECALL_READDATA:
		strTable.Format("%s%s", GetTblNamePrefix( GE_DYNPRESSCHANN, iTblType), sSuffix.c_str());
		pDB->Cmd("SELECT %s from [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if (iInterval>1)
				iInterval=iInterval-1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
					{
						long iAllBytes=sizeof(S_CustomParamData);
						CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
						//得到自定义参数信息
						CWaveHead sWaveHead;
						SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes, sWaveHead);
						++nloop;
					}
					int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
					pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_DYNSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
			{
				long iAllBytes=sizeof(S_CustomParamData);
				CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",(itPair->second).c_str());
				//得到自定义参数信息
				CWaveHead sWaveHead;
				SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)&pfCustomParom[nloop], iAllBytes, sWaveHead);
				++nloop;
			}
			int iAllBuffSize=sizeof(S_CustomParamData)*iChannNum_;
			pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBuffSize);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pfCustomParom);
	lChannField.clear();
	lChannPair.clear();
	return index;
}

/**得到指定公司、分厂机组和测点指定时刻的频谱数据
*输入参数：
*          公司ID       FIELD_SYSTEM_COMPANY
*	    	 分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          测点ID       FIELD_SERVICE_OFF_CHANN1
*          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数：   FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*          时间         FIELD_SERVICE_OFF_TIME
*          起始频率     FIELD_SERVICE_ZOOM_STARTFREQ
*          截止频率     FIELD_SERVICE_ZOOM_STOPFREQ
*返回值的字段码：
*          波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          转速            FIELD_SERVICE_OFF_REV
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
*          测点起始索引                          FIELD_SERVICE_STARTINDEX
*          测点结束索引                          FIELD_SERVICE_ENDINDEX
*          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
#define     SERVICE_SERVICE_OFF_GETSPECFREQWAVE       21002
*/
int CTransactData::GetSpecFreqWave(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sChann1=pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	int   ilines=pFixComm->GetLong(FIELD_SERVICE_OFF_FREQANALYSIS_LINES);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	float fStartFreq = pFixComm->GetDouble(FIELD_SERVICE_ZOOM_STARTFREQ);
	float fStopFreq  = pFixComm->GetDouble(FIELD_SERVICE_ZOOM_STOPFREQ);
	if (fStartFreq < 0) fStartFreq = 0;
	if (!(fStopFreq > 0)) fStopFreq = -1;//如果截止频率为-1，则表示默认
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if (ilines % 2 != 0)
	{
		ilines -= 1;
	}

	iWaveNum=ReadVibChannSpecTimeWaveAndZoomSpectrum(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1,ilines,iWaveNum,timeOff,fStartFreq, fStopFreq);
	pFixComm->Write(pFixComm->GetAci());
	return iWaveNum;
}

/**得到指定时间段、特征值类型、任意多个测点的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          特征值类型  FIELD_SERVICE_TREND_CHARTYPE
*          测点类型：  FIELD_SERVICE_OFF_CHANNTYPE
*          公司ID      FIELD_SYSTEM_COMPANY
*		   分厂名      FIELD_SYSTEM_FACTORY
*          机组ID      FIELD_SERVICE_PLANTID
*          测点buff    FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数    FIELD_SERVICE_CHANN_NUM
*          起始时间    FIELD_SERVICE_OFF_TIMESTART
*          终止时间    FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          测点单位             FIELD_SERVICE_CHANN1_UNIT
SERVICE_SERVICE_OFF_GETTREND                       21003
*/
int CTransactData::GetTrend(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sTmp;
	//如果设备ID和测点1ID有一个为空就不能读历史趋势
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sTmp,pFixComm)==NULL)
		return -1;
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_OFF_CHANNTYPE);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int  index=0;
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	E_TBL_CHANNTYPE iType=GetChannOneType((CHANNTYPE)iChannType);
	if (iType==E_TBL_CHANNTYPE_VIB)
		index = ReadVibTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timeStart,timeEnd);
	else if (iType==E_TBL_CHANNTYPE_PROC)
		index = ReadProcTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timeStart,timeEnd);
	else
		index = ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timeStart,timeEnd);
	delete [] pChannBuff;
	return index;
}
/*!得到指定测点、时间的基本报警信息
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点名称：      FIELD_SERVICE_OFF_CHANN1
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*返回值的字段码
*          特征值：      FIELD_SERVICE_CHARVALUE_S
*          报警门限Buff  FIELD_SERVICE_THRESHOLD_BUFF
*          门限的个数    FIELD_SERVICE_CHANN_NUM
*          报警类型      FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO   21010
*/
int CTransactData::GetSpecVibChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChann=pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	CThreshold *pTh=NULL;
	int ilen=0;
	string sTable="THRESHOLD_";
	sTable.append(sAlias);
	//得到指定事件测点的各个特征值门限
	pDB->Cmd("SELECT %s,%s FROM [%s] ",gc_cThreshold,gc_cThreshold_Len,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cName,sChann.c_str());
	if (pDB->More())
	{
		ilen=pDB->GetLong(gc_cThreshold_Len);
		pTh=new CThreshold[ilen];
		long iBytes=ilen*sizeof(CThreshold);
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cThreshold, (BYTE*)pTh, iBytes, sWaveHead);
	}
	else
		ilen=0;
	sTable="ALARM_VIB_";
	sTable.append(sAlias);
	S_VibCharValue vibValue;
	pDB->Cmd("SELECT %s,%s,%s FROM [%s] ",
		ConsVibChannField(channInfo._cChannNo),
		ConsVibChannSpecTypeField(channInfo._cChannNo,-2).c_str(),
		ConsCommonField(),
		sTable.c_str());
	pDB->Cmd("WHERE [%s]=%d AND %s='%s' AND %s=%d",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(dt),gc_cMicroSecond,iMicroSecond);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(pDB->More())
	{
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(ConsVibChannSpecTypeField(channInfo._cChannNo,-2).c_str()));
		pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,ilen);
		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF,(char *)pTh,ilen*sizeof(CThreshold));
	}
	else
	{
		pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,-1);
	}
	pFixComm->Write(pFixComm->GetAci());
	if(pTh!=NULL) delete [] pTh;
	return ilen;
}

/*!得到指定起止时间的指定机组和指定振动测点类型的报警记录
*输入参数字段码
*          公司ID   FIELD_SYSTEM_COMPANY
*    	   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID： FIELD_SERVICE_PLANTID
*          测点名： FIELD_SERVICE_CHANN1
*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          报警开始时间:   FIELD_SERVICE_OFF_TIMESTART
*          报警结束时间:   FIELD_SERVICE_OFF_TIMEEND
*          报警类型：      FIELD_SYSTEM_TYPE
*          事件ID          FIELD_SYSTEM_EVENT_ID
*          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
#define     SERVICE_OFF_GETVIBCHANNALARMLOG      21011
*/
int CTransactData::GetVibChannAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm)==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	int index=0,iAlarmType;
	CString strTable;
	CString strAlarmTypeDisc="";//报警类型描述

	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	//得到快变和缓变报警日志
	strTable.Format("ALARM_LOG_%s",sAlias.c_str());
	pDB->Cmd("SELECT * FROM [%s] ",strTable);
	pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
	if (sChann!="")
		pDB->Cmd(" AND [%s]='%s'",gc_cName,sChann.c_str());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		iAlarmType=pDB->GetLong(gc_cType);
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,pDB->GetLong(gc_cEventID));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iAlarmType);
		strAlarmTypeDisc=GetAlarmTypeDiscribe(iAlarmType);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,pDB->GetTime(gc_cStart_T));
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cEnd_T));
		pFixComm->SetItem(FIELD_SYSTEM_RESERVE_STRING,strAlarmTypeDisc);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++index;
	}
	CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();
	//得到趋势报警日志
	strTable.Format("ALARM_TREND_%s",sAlias.c_str());
	pDB->Cmd("SELECT %s,%s,%s FROM [%s] ",gc_cType,gc_cTime,gc_cEventID,strTable);
	pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
	if (sChann!="")
		pDB->Cmd(" AND [%s]='%s'",gc_cChannNo,sChann.c_str());
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,pDB->GetLong(gc_cEventID));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));
		dtTmp=pDB->GetTime(gc_cTime);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,dtTmp);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,dtTmp);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++index;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          各个测点特征值Buff  FIELD_SERVICE_CHARVALUE_SBUFF
*          对应报警类型Buff    FIELD_SERVICE_TYPE_BUFF
*          测点个数            FIELD_SERVICE_CHANN_NUM
*          时间值              FIELD_SERVICE_OFF_TIME
*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND
*/
int CTransactData::GetVibChannAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sTmp;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sTmp,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	//如果设备ID和测点1ID有一个为空就不能读历史趋势
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	lCHANNID_NO lChannPair=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	iChannNum=ReadVibChannSpecEventIDAlarmData(pFixComm,pDB,sCompany,sFactory,sPlant,lChannPair,iEventID);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChannPair.clear();
	delete [] pChannBuff;
	return iChannNum;
}

/**得到指定时间的指定测点的报警记录的波形和各种特征值，对于多个测点就循环得到
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数：      FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点ID：        FIELD_SERVICE_OFF_CHANN1
*          报警类型：      FIELD_SYSTEM_TYPE
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          波形：          FIELD_SERVICE_OFF_WAVE
*          频谱：          FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：      FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：      FIELD_SERVICE_OFF_WAVE_NUM
*          频谱数据点数    FIELD_SERVICE_ZOOM_LINES
*          转速：          FIELD_SERVICE_OFF_REV
*          自定义参数数据BUFF：   FIELD_SERVICE_LEARNPARAM_BUFF
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD     21012
*/
int CTransactData::GetSpecAlarmTimeRecord(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int   ilines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	//如果设备ID和测点1ID有一个为空就不能读历史趋势
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);

	lCHANNID_NO lChann=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	lCHANNID_NO::iterator itChann;

	list<S_ChannInfo> lChannInfo;
	list<S_ChannInfo>::iterator itInfo;
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		lChannInfo.push_back(pPlantInfo->GetSpecVibChannInfo(itChann->first));
	string sShowField = ConsCommonField();
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if (itChann->second.c_str()=="")
			continue;
		sShowField.append(",");
		sShowField.append(ConsVibChannField(itChann->second,true));
		//sShowField.append(",");
		//sShowField.append(ConsVibChannSpecTypeField(itChann->second,-2));
		sShowField.append(",");
		CString strTmp;
		strTmp.Format("CUSTOM_PARAM_%s",(itChann->second).c_str());
		string sTmp=strTmp.GetString();
		sShowField.append(sTmp);
	}
	iChannNum=lChann.size();
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iRealNums=0,iSmpFreq=1,iSmpNums=1,iRev=0;
	float df=0,fRev=.0;
	S_VibCharValue vibValue;
	//获取数据
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	
	CString strTable;
	E_TREND_TBL_TYPE iTableType = GE_TREND_TBLTYPE_SECOND;
	
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN, pDB,timeOff, iTableType, sSuffix),sSuffix.c_str());

	pDB->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",sShowField.c_str(),sSuffix.c_str());
	if(iEventID > 0)
	{
		pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(timeOff));

	}
	else
	{
		pDB->Cmd(" WHERE [%s]='%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeOff));
	}

	if(!pDB->More())
	{
		pDB->Cmd("SELECT %s FROM %s ",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] = '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeOff));

		if(!(pDB->More()))
		{
			//当按照精确时间查询失败时，则按照相邻的一段时间查询数据
			CString strSqlWhere;

			BOOL lbDataFound = FALSE;

			for (int i =0 ;i< 50;i++)
			{
				int lnTimeSpan = ALARMRECORDS_QUERYTIMESPAN *pow(i+1,2);

				COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, lnTimeSpan );
				CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
				CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
				dtStart = timeOff - timeSpan;
				dtEnd   = timeOff + timeSpan;
				strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
				//获取报警数据
				pDB->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",sShowField.c_str(),sSuffix.c_str());
				pDB->Cmd(" WHERE %s AND [%s]=%d ", strSqlWhere,gc_cEventID,iEventID);

				if (lnTimeSpan > ALARMRECORDS_QUERYTIMESPAN_MAX)
				{
					break;
				}

				if(!(pDB->More()))
				{
					continue;
				}else
				{
					lbDataFound = TRUE;

					break;
				}

			}

			if (!lbDataFound)
			{
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				KD_DeletePoint(pChannBuff);
				return -2;
			}
		}
	}
	
	
	if (iWaveNum<100)
		iWaveNum=513;

	DATATYPE_WAVE *pWave=NULL;
	DATATYPE_WAVE *pSpectrum=NULL;
	float *pfWave = NULL;
	float *pfSpectrum = NULL;

	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfSpectrum(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &lvfWave.front();
	pfSpectrum = &lvfSpectrum.front();

	itInfo=lChannInfo.begin();
	for (itChann=lChann.begin();(itChann!=lChann.end())&&(itInfo!=lChannInfo.end());++itChann)
	{
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, itChann->second);
		iSmpNums = SimADO_GetSampleNumEx(pDB, itChann->second);
		iRev     = SimADO_GetRevEx(pDB, itChann->second);
		//CString sTemp;sTemp.Format("iSmpFreq:%d,iSmpNums:%d,iRev:%d",iSmpFreq,iSmpNums,iRev);BHTRACE_DEBUG(sTemp);
		long iBytes=iSmpNums*sizeof(DATATYPE_WAVE);
		if (iSmpNums<=10)
		{
			KD_DeletePoint(pChannBuff);
			return -1;
		}

		if (ilines == 0)
		{
			ilines=iSmpNums/2.56;
		}
		else 
		{
			if((ilines>6400)||(ilines<400))
				ilines=400;
		}
		pWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pSpectrum=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		vibValue=ReadVibCharValue(itChann->second,pDB);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(ConsVibChannSpecTypeField(itChann->second,-2).c_str()));
		pFixComm->SetItem(FIELD_SERVICE_OFF_CHANN1,itChann->first.c_str());
		S_ChannInfo channInfo=pPlantInfo->GetSpecVibChannInfo(itChann->first.c_str());
		fRev=(float)iRev;
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev);
		//	  CString strTmp;strTmp.Format("%f,%f",fRev1,fRev2);AfxMessageBox(strTmp);
		CWaveHead sWaveHead;

		SimADO_SafeGetBufferEx(pDB, ConsVibChannSpecTypeField(itChann->second, -1).c_str(), (BYTE*)pWave,iBytes,sWaveHead);
		iRealNums=iBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealNums?iRealNums:iWaveNum;
		//writefile(pWave,iSmpNums,1);
		iRealNums=SpectrumAnalysis(pPlantInfo,pWave,iRealNums,pSpectrum,ilines,itInfo->_iSensorType,iSmpFreq,df,pfWave,pfSpectrum,sWaveHead);
		//CString strTmp;strTmp.Format("iRealNums:%d,ilines:%d,iSensorType:%d,iSmpFreq:%d,df:%f",iRealNums,ilines,itInfo->_iSensorType,iSmpFreq,df);
		//BHTRACE_DEBUG(strTmp);
		//writefile(pSpectrum,ilines,2);
		//  if (iRealNums!=0) df=(float)(iSmpFreq/(float)iRealNums);因为细化分析注释掉
		pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pWave,iWaveNum*sizeof(DATATYPE_WAVE));
		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char*)pSpectrum,ilines*sizeof(DATATYPE_WAVE));
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,ilines);
		pFixComm->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		//得到自定义参数信息
		float  *pfCustomParom =new float[GD_CUSTOMPARAM_BUFFSIZE];
		memset(pfCustomParom,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
		CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)pfCustomParom, iAllBytes,sWaveHead);
        /* CString sTemp;
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			sTemp.Format("Vib:pfCustomParom[%d]:%f",i,pfCustomParom[i]);
			BHTRACE_DEBUG(sTemp);
		}*/
		pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBytes);

		int lnPrecisionEnabled = sWaveHead.m_pParam[1];
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iWaveNum*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)pfSpectrum,ilines*sizeof(float));
		}


		++itInfo;
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		KD_DeletePoint(pWave);
		KD_DeletePoint(pSpectrum);
		KD_DeletePoint(pfCustomParom);
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	lChannInfo.clear();
	lChann.clear();
	delete [] pChannBuff;
	return iSmpNums;
}



/**得到指定时间的指定测点的报警记录的各种特征值，对于多个测点就循环得到
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*          测点ID：        FIELD_SERVICE_OFF_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          报警类型：      FIELD_SYSTEM_TYPE
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速：          FIELD_SERVICE_OFF_REV
*          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE    21034
*/
int CTransactData::GetSpecAlarmRecordCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	//如果设备ID和测点1ID有一个为空就不能读历史趋势
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	lCHANNID_NO lChann;
	lCHANNID_NO::iterator itChann;
	lChann=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	list<S_ChannInfo> lChannInfo;
	list<S_ChannInfo>::iterator itInfo;
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		lChannInfo.push_back(pPlantInfo->GetSpecVibChannInfo(itChann->first));
	string sShowField = ConsCommonField().GetString();
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if (itChann->second.c_str()=="")
			continue;
		sShowField.append(",");
		sShowField.append(ConsVibChannField(itChann->second,false));
		sShowField.append(",");
		sShowField.append(ConsVibChannSpecTypeField(itChann->second,-2));
		sShowField.append(",");
		sShowField.append(ConsVibChannSpecTypeField(itChann->second,-5));
	}
	iChannNum=lChann.size();
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	float fRev=.0;
    int iRev=0;
	//获取数据
	pDB->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",sShowField.c_str(),sSuffix.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(timeOff),gc_cMicroSecond,iMicroSecond);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (pDB->More())
	{
		S_VibCharValue vibValue;
		itInfo=lChannInfo.begin();
		unsigned int iType=0;
		for (itChann=lChann.begin();(itChann!=lChann.end())&&(itInfo!=lChannInfo.end());++itChann)
		{
			iRev=pDB->GetLong(ConsVibChannSpecTypeField(itChann->second,-5).c_str());
			vibValue=ReadVibCharValue(itChann->second,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
			iType=pDB->GetLong(ConsVibChannSpecTypeField(itChann->second,-2).c_str());
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,(int)iType);
			pFixComm->SetItem(FIELD_SYSTEM_RESERVE_STRING,GetAlarmTypeDiscribe(iType));
			pFixComm->SetItem(FIELD_SERVICE_OFF_CHANN1,itChann->first.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,pPlantInfo->GetSpecVibChannInfo(itChann->first)._cChannAlias);
		    fRev=(float)iRev;
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,itInfo->_chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,itInfo->_chAlarmParam._fAlarmHH);
			++itInfo;
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	lChannInfo.clear();
	lChann.clear();
	delete [] pChannBuff;
	return 0;
}

// 通过测点类型、机组号时间间隔得到从那个数据库表单中获得数据。
CString CTransactData::GetTblNamePrefixForWave(int ag_iChannType,CBHDateTime timeEnd_,int& iTableType_)
{
	CString strtable;
	COleDateTimeSpan span1;
	long iday=0,ihour=0;
	switch(ag_iChannType)
	{
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
		strtable="TREND_VIB";
		break;
	default:
		strtable="TREND_PROC";
	}

	/*判断从起始时间和时间间隔来判断从那个表中取数据
	规则：1、如果起始时间和当前时间相差在30天之内，并且起始时间和结束时间间隔超过在24个小时之内，则在小时趋势表单中取数据；
	2、如果起始时间和当前时间相差在30天之内，并且起始时间和结束时间时间间隔在24小时和720小时之间，则在天趋势表单中取数据；
	3、如果起始时间和当前时间相差在200天之内，则在天趋势表单中取数据；
	4、如果起始时间和当前时间相差超过200天，则在月趋势表单中取数据；
	*/
	//秒趋势（当前趋势）表单中，保存记录的最大条数。3秒钟保存一次，所以秒趋势表单中总共保存12小时的记录
	//小时趋势表单中，保存记录的最大条数。一分钟保存一次，一天保存1440条，所以小时趋势表单中总共保存360天的记录
	//天趋势表单中，保存记录的最大条数。记录为一天为72条记录（即20分钟一个点），总共保存78840条记录。即3*360天的记录
	CBHDateTime timetmp=CBHDateTime::GetCurrentTime();
	span1=timetmp-timeEnd_;
	iday=span1.GetTotalDays();
	ihour=span1.GetTotalHours();
	if(iday<360)
	{
		if (ihour<12)
		{
			strtable=strtable+"SECOND_";
			iTableType_=0;
		}
		else
		{
			strtable=strtable+"HOUR_";
			iTableType_=1;
		}
	}
	else if(iday<1080)//3*360
	{
		strtable=strtable+"DAY_";
		iTableType_=2;
	}
	else
	{
		strtable=strtable+"MONTH_";
		iTableType_=3;
	}
	return strtable;
}

/**得到指定公司、分厂、机组号、振动测点号、起始时间段的所有特征值趋势数据
*输入参数字段码：
*          公司ID      FIELD_SYSTEM_COMPANY
*	         分厂名      FIELD_SYSTEM_FACTORY
*          机组ID      FIELD_SERVICE_PLANTID
*          测点类型：  FIELD_SERVICE_CHANNTYPE
*          测点ID      FIELD_SERVICE_OFF_CHANN1
*          起始时间    FIELD_SERVICE_OFF_TIMESTART
*          终止时间    FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          时间             FIELD_SERVICE_OFF_TIME
#define    SERVICE_SERVICE_OFF_GETAVIBCHANNTREND      21004
*/
int CTransactData::GetAVibChannTrend(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	char *pChann=new char[sChann.size()+1];
	strncpy(pChann,sChann.c_str(),sChann.size());
	int nCount;
	if (GetChannOneType((CHANNTYPE)iChannType)==E_TBL_CHANNTYPE_VIB)
		nCount=ReadVibTrendAllCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,pChann,1,timeStart,timeEnd);
	else
		nCount=ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChann,1,-5,timeStart,timeEnd);
	delete [] pChann;
	return nCount;
}

// 得到轴心位置的历史趋势
/**得到指定公司、分厂名、机组ID、时间段、水平测点ID和垂直测点ID的轴心位置历史数据，通过循环依次得到所有数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID   FIELD_SYSTEM_COMPANY
*   	   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          起始时间    FIELD_SERVICE_OFF_TIMESTART
*          终止时间    FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          水平方向振动测点数据  FIELD_SERVICE_AXESLOCATION_HOR
*          垂直方向振动测点数据  FIELD_SERVICE_AXESLOCATION_VER
*          时间值                FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND  21005
*/
int CTransactData::GetAxisLocationTrend(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix,sPrefix;CString sTmp;
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	//如果设备ID和测点1ID有一个为空就不能读轴心位置历史趋势
	if (pPlant==NULL)
		return -1;

	string sChannIDHor = pFixComm->Get(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->Get(FIELD_SERVICE_CHANNID_VER);

	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int nCount=-1;

	S_ChannInfo channInfoHor;
	S_ChannInfo channInfoVer;

	string sChannNoHor;
	string sChannNoVer;

	if ((sChannIDHor.empty())||(sChannIDVer.empty()))
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,pPlant->GetPlantNo().c_str(),gc_cLocation,sLocation.c_str());
		if (pDB->More())
		{
			sChannNoHor = pDB->Get(gc_cHorChann);
			sChannNoVer = pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		channInfoVer = pPlant->GetSpecVibChannNoInfo( sChannNoVer );
		channInfoHor = pPlant->GetSpecVibChannNoInfo( sChannNoHor );

		sChannIDHor  = channInfoHor._cChannID;
		sChannIDVer  = channInfoVer._cChannID;
	}
	else
	{
		channInfoVer = pPlant->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlant->GetSpecVibChannInfo( sChannIDHor );

		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;
	}

	//CString sTmp;sTmp.Format("%s,%s",sChannIDHor.c_str(),sChannIDVer.c_str());BHTRACE_DEBUG(sTmp);
	CString strTable;
	string  sShowField,sChannVerDC,sChannHorDC,sChannVerGap,sChannHorGap;
	sShowField.append(gc_cTime);
	sShowField.append(gc_cDot);

	sChannVerDC="DC_";
	sChannVerDC.append(sChannNoVer);
	sChannVerGap="GAP_";
	sChannVerGap.append(sChannNoVer);
	sChannHorDC  = "DC_"  + sChannNoHor;
	sChannHorGap = "GAP_" + sChannNoHor;

	sShowField.append(sChannHorDC);
	sShowField.append(gc_cDot);
	sShowField.append(sChannVerDC);
	sShowField.append(gc_cDot);
	sShowField.append(sChannHorGap);
	sShowField.append(gc_cDot);
	sShowField.append(sChannVerGap);
	bool bRead=false;
	bool bNeedSudTbl=false;
	int iInterval=0,iNums=0,iTmp=0;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart,timeEnd,bNeedSudTbl);

	if(iNums > 0)
	{
		
		iInterval=iNums/D_TRENDRECORDS;

		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart,timeEnd,bNeedSudTbl,iInterval);
		if(bRead)
		{
			DATATYPE_CHAR fVerDC,fVerGap,fHorDC,fHorGap,fX=0,fY=0;
			do
			{
				++nCount;
				fVerDC=pDB->GetDouble(sChannVerDC.c_str());
				fVerGap=pDB->GetDouble(sChannVerGap.c_str());
				fHorDC=pDB->GetDouble(sChannHorDC.c_str());
				fHorGap=pDB->GetDouble(sChannHorGap.c_str());
				//sTmp.Format("1处:fHorDC:%f,fVerDC:%f,fHorGap:%f,fVerGap:%f",fHorDC,fVerDC,fHorGap,fVerGap);BHTRACE_DEBUG(sTmp);
				if((pPlant->CalcSpecLocationValue(sChannIDHor,sChannIDVer,fVerGap,fVerDC,fHorGap,fHorDC,fX,fY))<0)
					continue;
				//sTmp.Format("1处:fX:%f,fY:%f",fX,fY);BHTRACE_DEBUG(sTmp);
				pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,fX);
				pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,fY);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart,timeEnd,bNeedSudTbl);

		if (bRead)
		{	//得到每次移动的间隔,因为每次返回的记录条数在50和100之间
			if (bNeedSudTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if(iInterval>1)
					iInterval-=1;
				else
					iInterval=0;
			}
			//sTmp.Format("1处:iNums:%d,iInterval:%d",iNums,iInterval);BHTRACE_DEBUG(sTmp);
			iTmp=0;
			DATATYPE_CHAR fVerDC,fVerGap,fHorDC,fHorGap,fX=0,fY=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					++nCount;
					fVerDC=pDB->GetDouble(sChannVerDC.c_str());
					fVerGap=pDB->GetDouble(sChannVerGap.c_str());
					fHorDC=pDB->GetDouble(sChannHorDC.c_str());
					fHorGap=pDB->GetDouble(sChannHorGap.c_str());
					//sTmp.Format("1处:fHorDC:%f,fVerDC:%f,fHorGap:%f,fVerGap:%f",fHorDC,fVerDC,fHorGap,fVerGap);BHTRACE_DEBUG(sTmp);
					if((pPlant->CalcSpecLocationValue(sChannIDHor,sChannIDVer,fVerGap,fVerDC,fHorGap,fHorDC,fX,fY))<0)
						continue;
					//sTmp.Format("1处:fX:%f,fY:%f",fX,fY);BHTRACE_DEBUG(sTmp);
					pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,fX);
					pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,fY);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
	}
	
	if (bNeedSudTbl)
	{
RECALL_READRECORDS_AXISDISPLACE:
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN, iTblType),sSuffix.c_str());
		pDB->Cmd("SELECT %s FROM [%s]" ,sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
		pDB->Cmd(" ORDER BY [%s]",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+nCount)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READRECORDS_AXISDISPLACE;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-nCount);
			if(iInterval>1)
				iInterval-=1;
			else
				iInterval=0;
			//sTmp.Format("2处:iNums:%d,iInterval:%d",iNums,iInterval);BHTRACE_DEBUG(sTmp);
			iTmp=0;
			DATATYPE_CHAR fVerDC,fVerGap,fHorDC,fHorGap,fX=0,fY=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					++nCount;
					fVerDC=pDB->GetDouble(sChannVerDC.c_str());
					fVerGap=pDB->GetDouble(sChannVerGap.c_str());
					fHorDC=pDB->GetDouble(sChannHorDC.c_str());
					fHorGap=pDB->GetDouble(sChannHorGap.c_str());
					//sTmp.Format("2处:fHorDC:%f,fVerDC:%f,fHorGap:%f,fVerGap:%f",fHorDC,fVerDC,fHorGap,fVerGap);BHTRACE_DEBUG(sTmp);
					if((pPlant->CalcSpecLocationValue(sChannIDHor,sChannIDVer,fVerGap,fVerDC,fHorGap,fHorDC,fX,fY))<0)
						continue;
                   // sTmp.Format("2处:fX:%f,fY:%f",fX,fY);BHTRACE_DEBUG(sTmp);
					pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,fX);
					pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,fY);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READRECORDS_AXISDISPLACE;
			}
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}



/*!得到指定时间范围的指定机组的启停车记录
*输入参数字段码
*          公司ID      FIELD_SYSTEM_COMPANY
*			 分厂名      FIELD_SYSTEM_FACTORY
*          机组ID：    FIELD_SERVICE_PLANTID
*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          启停车类型：  FIELD_SYSTEM_TYPE
*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
*define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD   21013
*/
int CTransactData::GetSpecTimeRangeSud(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	int nCount=-1;
	string sAlias;
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm)==NULL)
	{
		return -1;
	}
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sTimeStart,sTimeEnd,sTable="SUD_LOG_";
	sTable.append(sAlias);

	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	sTimeStart = CPlantInfoBase::TimeConvertString(timeStart);
	sTimeEnd   = CPlantInfoBase::TimeConvertString(timeEnd);
	//从启停车日志表中得到启停车记录
	pDB->Cmd("select * from [%s] ",sTable.c_str());
	pDB->Cmd(" WHERE [%s]<'%s' AND [%s]>'%s' ORDER BY [%s] ",gc_cStart_T,sTimeEnd.c_str(),gc_cStart_T,sTimeStart.c_str(),gc_cStart_T);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,pDB->GetLong(gc_cEventID));
		timeStart=pDB->GetTime(gc_cStart_T);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,timeStart);
		if (0==pDB->GetLong(gc_cStop_Flag))
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,timeStart);
		}
		else
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cEnd_T));
		}
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/*!得到指定启停车开始时刻和指定机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分别通过转速或保存这组数据的时刻、机组号和测点号
*输入参数字段码
*          公司ID:       FIELD_SYSTEM_COMPANY
*			 分厂名:       FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:        FIELD_SERVICE_OFF_REV
*          时间:        FIELD_SERVICE_OFF_TIME
*          对应时间的微秒         FIELD_SERVICE_MICROSECOND
*          相位：       FIELD_SERVICE_OFF_PHASE
*          幅值：       FIELD_SERVICE_OFF_AMP
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD   21014
*/
int CTransactData::GetSpecTimeChannSud(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;//公司别名
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime timeSud=CBHDateTime::GetCurrentTime();
	timeSud=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	string sAmp,sPhase,sRev,sTable="SUD_";
	sTable.append(sAlias);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	string sSpecChannAllField = ConsVibChannField( channInfo._cChannNo, false);
	sAmp=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_ONECHAR);
	sPhase=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_PHASECHARONE);
	sRev=ConsVibChannSpecTypeField(channInfo._cChannNo,-5);
	int nCount=-1;
	pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
		sSpecChannAllField.c_str(),
		sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s] IS NOT NULL ORDER BY [%s],[%s] ",gc_cEventID,iEventID,sRev.c_str(),gc_cTime,gc_cMicroSecond);
	S_VibCharValue vibCharValue;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();
	_DECLARE_PERF_MEASURE_TIME();
	_BEGIN_PERF_MEASURE_TIME();
	while(pDB->More())
	{
		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		dtTmp=pDB->GetTime(gc_cTime);
		int lnMicroSecond = pDB->GetLong(gc_cMicroSecond);
		dtTmp.SetMillisecond(lnMicroSecond);
		if (dtTmp.GetYear()<2007)//为了防止年份错误引起一些的BUG
		{
			continue;
		}
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtTmp);
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE,pDB->GetDouble(sPhase.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,pDB->GetDouble(sAmp.c_str()));
		//pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetLong(sRev.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,vibCharValue._iRev);
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
		//CString strTemp;
		//strTemp.Format("振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
		//CHZLogManage::Debug(strTemp,"CTransactData","GetSpecTimeChannSud");
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}
	_END_PERF_MEASURE_TIME("test");
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/*!得到指定启停车事件ID、指定公司、分厂、机组、指定测点和指定转速的启停车的详细数据。
*输入参数字段码
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          转速:         FIELD_SERVICE_OFF_REV
*          时间点：      FIELD_SERVICE_OFF_TIME
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数      FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          是否有高精度波形           FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO
*		   高精度波形			FIELD_SERVICE_SEND_FLOAT_WAVE1
*		   高精度频谱  FIELD_SERVICE_SEND_FLOAT_FREQ1
*define    SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD    21015
*/
int CTransactData::GetSpecChannRevSud(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if ( pPlant == NULL )
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	float fRev=pFixComm->GetLong(FIELD_SERVICE_OFF_REV);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime loDateTime = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);

	if (0==iWaveNum)
	{
		return -1;
	}
	S_ChannInfo channInfo = pPlant->GetSpecVibChannInfo(sChann);

	string sSmpFreq,sSmpNum,sRev,sTable,sWave="WAVE_",sChannRev = "REV_";
	sWave.append(channInfo._cChannNo);
	sChannRev.append(channInfo._cChannNo);
	//	sAmp=ConsVibChannSpecTypeField(sChann,GE_ONECHAR);
	string sSpecChannAllField = ConsVibChannField( channInfo._cChannNo, true);
	int iSensorType = channInfo._iSensorType;//=GetSpecChannSensorType(sCompany,sPlant,sChann,iRevType,pDB);
	
	if(iLines>1600)
	{	
		iLines=1600;
	}

	if (iLines<200)
	{
		iLines = 200;
	}
	
	int nCount=-1;
	sTable="SUD_";
	sTable.append(sAlias);

	pDB->Cmd("select top 1 [%s] from [%s] ",
		gc_cEventID,
		sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d and [%s] = '%s' and %s = %f",
		gc_cEventID,iEventID,gc_cTime,loDateTime.Format(),sChannRev.c_str(),fRev);
	if(pDB->More())
	{
		pDB->Cmd("select top 1 %s,%s from [%s] ",
			sSpecChannAllField.c_str(),
			ConsCommonField(),
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and [%s] = '%s' and %s = %f",
			gc_cEventID,iEventID,gc_cTime,loDateTime.Format(),sChannRev.c_str(),fRev);
	}
	else
	{
		sRev = pPlant->GetRevField(channInfo._iRevType);
		pDB->Cmd("select top 1 %s,%s from [%s] ",
			sSpecChannAllField.c_str(),
			ConsCommonField(),
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and %s > %f and %s < %f",gc_cEventID,iEventID,sRev.c_str(),fRev-1, sRev.c_str(), fRev+1);
	}
	
	
		

	//pDB->Cmd(" WHERE [%s]=%d and %s > %f and %s < %f",gc_cEventID,iEventID,sRev.c_str(),fRev-1, gc_cRev, fRev+1);
	

	S_VibCharValue vibCharValue;
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(pDB->More())
	{
		int inum,iquest_wavenum,ifreq,irev,istep=0,istep2=0;

		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		inum  = SimADO_GetSampleNumEx(pDB, channInfo);
		ifreq = SimADO_GetSampleFrequencyEx(pDB, channInfo);
		//CString sTmp;sTmp.Format("%d,%d",inum,ifreq);
		//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecChannRevSud");
		irev = SimADO_GetRevEx(pDB, channInfo);
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE, vibCharValue._fPhaseOne);
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,  vibCharValue._fCharOne);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		DATATYPE_WAVE *pDtWave=NULL,*pDtFreq=NULL;
		float *pfWave=NULL,*pfSpectrum=NULL;
		float df;
		iquest_wavenum=iLines*2.56;
		long isize=0;
		if (inum>0)
		{
			pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			//pfSpectrum=new float[1+inum/2];
			pfSpectrum=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtFreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			isize=inum*sizeof(DATATYPE_WAVE);
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, sWave.c_str(), (BYTE *)pDtWave, isize,sWaveHead);

			inum=isize/sizeof(DATATYPE_WAVE);

			

			iWaveNum = iWaveNum > inum ? inum:iWaveNum;

			pPlant->ConvertDTWaveToFloat(pfWave,pDtWave,iWaveNum,sWaveHead);

			pPlant->ConvertFloatToDTWave(pDtWave,pfWave,iWaveNum,sWaveHead);
			//        writefile(pfwave,inum);

			{//细化谱和正常谱分析 2007.4.7添加
				float fStartFreq=0;
				//int iRtn=2;
				int iRtn=pPlant->ZoomSpectrumAnalysis(pfWave,iWaveNum,ifreq,pfSpectrum,iLines,df,fStartFreq);
				//CString sTmp;sTmp.Format("%d,%d,%d,%d",inum,ifreq,iLines,iRtn);
				//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecChannRevSud");
				if (iRtn<0)
				{//错误
					iLines=0;
				}
				else
				{
					if (iRtn==2)
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
						{
							pPlant->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2);
							for(int nloop = 0; nloop < iLines; nloop++)
							{
								pfSpectrum[nloop] = pfSpectrum[nloop]*2.0;
							}
						}
						else
							pPlant->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1);
					}
					else
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
						{
							pPlant->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2.818);
							for(int nloop = 0; nloop < iLines; nloop++)
							{
								pfSpectrum[nloop] = pfSpectrum[nloop]*2.818;
							}
						}
						else
						{
							pPlant->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1.414);
							for(int nloop = 0; nloop < iLines; nloop++)
							{
								pfSpectrum[nloop] = pfSpectrum[nloop]*1.414;
							}
						}
					}
				}
			}

			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtFreq,iLines*sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
			pFixComm->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iLines);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);

			int lnPrecisionEnabled = sWaveHead.m_pParam[1];
			if(lnPrecisionEnabled == 1)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));

				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)pfSpectrum,iLines*sizeof(float));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
			}
		}
		else
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
		if(pDtFreq!=NULL) delete [] pDtFreq ;
		if(pfWave!=NULL) delete [] pfWave;
		if(pDtWave!=NULL) delete [] pDtWave;
		if(pfSpectrum!=NULL) delete [] pfSpectrum;
		nCount++;
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}
/*!得到指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名       FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车数据保存时间:   FIELD_SERVICE_OFF_TIME
*          对应时间的微秒         FIELD_SERVICE_MICROSECOND
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数      FIELD_SERVICE_ZOOM_LINES
*          调用接口状态：FIELD_SYSTEM_STATUS
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD    21016
*/
int CTransactData::GetSpecChannSaveTimeSud(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	if(pFixComm->GetLong(FIELD_SYSTEM_STATUS) == 1)
	{
		return this->GetSpecChannSaveTimeSudByTimeRange(pFixComm,pDB);
	}
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime timeSud=CBHDateTime::GetCurrentTime();
	timeSud=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	if((iLines>1600)||(iLines<200))
		iLines=401;
	string sTimeSud,sAllField,sTable,sSmpFreq,sSmpNum,sRev,sWave="WAVE_";
	sWave.append(channInfo._cChannNo);
	sTimeSud = CPlantInfoBase::TimeConvertString(timeSud);
	//	sAmp     = ConsVibChannSpecTypeField(sChann,GE_ONECHAR);
	//	sPhase   = ConsVibChannSpecTypeField(sChann,GE_PHASECHARONE);
	sAllField = ConsVibChannField( channInfo._cChannNo, true);
	int iSensorType = channInfo._iSensorType;//GetSpecChannSensorType(sCompany,sPlant,sChann,iRevType,pDB);

	int nCount=-1;
	sTable="SUD_";
	sTable.append(sAlias);
	
	pDB->Cmd("select %s,%s from [%s] ",
		sAllField.c_str(),
		ConsCommonField(),
		sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d and [%s]='%s' AND [%s]=%d",
		gc_cEventID,iEventID,gc_cTime,sTimeSud.c_str(),gc_cMicroSecond,iMicroSecond);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(pDB->More())
	{
		int ifreq,inum,irev,iquest_wavenum,istep=0,istep2=0;
		S_VibCharValue vibCharValue;
		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		inum  = SimADO_GetSampleNumEx(pDB, channInfo);
		ifreq = SimADO_GetSampleFrequencyEx(pDB, channInfo);
		irev  = SimADO_GetRevEx(pDB, channInfo);
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE, vibCharValue._fPhaseOne);
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,  vibCharValue._fCharOne);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,irev);
		DATATYPE_WAVE *pDtWave=NULL,*pDtFreq=NULL;
		float *pfWave=NULL,*pfSpectrum=NULL;
		float df;
		iquest_wavenum=iLines*2.56;
		long isize=0;
		//	irev=pDB->Ge(gc_cRev);
		if (inum>0)
		{
			pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pfSpectrum=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtFreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			isize=inum*sizeof(DATATYPE_WAVE);
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, sWave.c_str(), (BYTE *)pDtWave, isize,sWaveHead);
			inum=isize/sizeof(DATATYPE_WAVE);
			pPlantInfo->ConvertDTWaveToFloat(pfWave,pDtWave,inum,sWaveHead);
			//        writefile(pfwave,inum);

			/*		if(inum<iquest_wavenum)
			iquest_wavenum=inum;
			iLines=iquest_wavenum/2.56;
			if (iWaveNum>inum)
			iWaveNum=inum;

			df=(float)ifreq/iquest_wavenum;
			SpectralMeasureMent(pfWave,iquest_wavenum,pfSpectrum);
			iLines=iLines+1;
			if (iSensorType==GE_SENSORTYPE_VOTEX)
			{
			for (int nloop=0;nloop<iLines;++nloop)
			pfSpectrum[nloop]=pfSpectrum[nloop]*2.0;//江老师要求添加的系数
			}
			else
			{
			for (int nloop=0;nloop<iLines;++nloop)
			pfSpectrum[nloop]=pfSpectrum[nloop]*1.414;//江老师要求添加的系数
			}
			ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines);
			*/
			{//细化谱和正常谱分析 2007.4.7添加
				float fStartFreq=0;
				int iRtn=pPlantInfo->ZoomSpectrumAnalysis(pfWave,inum,ifreq,pfSpectrum,iLines,df,fStartFreq);
				if (iRtn<0)
				{//错误
					iLines=0;
				}
				else
				{
					if (iRtn==2)
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1);
					}
					else
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2.818);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1.414);
					}
				}
			}

			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtFreq,iLines*sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
			pFixComm->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iLines);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		}
		else
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,inum);
		if(pDtFreq!=NULL) delete [] pDtFreq ;
		if(pfWave!=NULL) delete [] pfWave;
		if(pDtWave!=NULL) delete [] pDtWave;
		if(pfSpectrum!=NULL) delete [] pfSpectrum;
		nCount++;
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/**得到指定历史时刻、机组振动测点的细化频谱分析和波形。
*输入参数：机组ID           FIELD_SERVICE_PLANTID
*          测点             FIELD_SERVICE_OFF_CHANN1
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
*          时间             FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
*          通频特征值：     FIELD_SERVICE_TREND_OVERALLCHAR
*          状态             FIELD_SYSTEM_STATUS
*          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_OFF_GETZOOMSPEC             21007
*/
int CTransactData::GetZoomSpec(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo == NULL )
		return -100;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	DATATYPE_WAVE *pfwave=NULL,*pfZoomSpm=NULL;
	float *pfZoom1=NULL;
	float *pfwave1=NULL;
	float fDf=.0;
	int iSmpNum=1,iSmpFreq=1,ihours,iErrorCode=0;
	int iPtNum=513;
	int iTableType=0;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CString strChann=sChann.c_str();
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	float fStartFreq=pFixComm->GetDouble(FIELD_SERVICE_ZOOM_STARTFREQ);
	int iStopFreq=pFixComm->GetLong(FIELD_SERVICE_ZOOM_STOPFREQ);
	CBHDateTime tTime=CBHDateTime::GetCurrentTime();
	tTime=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);

	COleDateTimeSpan timespan=CBHDateTime::GetCurrentTime()-tTime;
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	//要提取波形数据的表单名字
	CString strtable,sTmp;
	CString strtime = CPlantInfoBase::TimeConvertString(tTime);
	S_VibCharValue vibCharValue;
	CWaveHead sWaveHead;

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(!((fStartFreq>0)&&(iStopFreq>fStartFreq)&&(iLines>0))){//起始频率必须小于终止频率，采样频率的一半不能大于终止频率
		iErrorCode=-3;
		goto CLEANGETZOOMSPEC;
	}
	//如果在两个小时之类就在小时趋势表中读取波形数据。因为如果web端显示的是当前趋势，则最新的一个点是从小时趋势中提取的，而且只要近期的点只要其它趋势中有的点，小时趋势中都有
	ihours=timespan.GetTotalHours();
	if (ihours<1){
		iTableType=0;
		strtable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
	}
	else if(ihours<720){//30*24
		iTableType=1;
		strtable.Format("TREND_VIBHOUR_%s",sSuffix.c_str());
	}
	else if(ihours<4800){//200*24
		iTableType=2;
		strtable.Format("TREND_VIBDAY_%s",sSuffix.c_str());
	}
	else{
		iTableType=3;
		strtable.Format("TREND_VIBMONTH_%s",sSuffix.c_str());
	}
	//构造查询Sql语句
	pDB->Cmd("SELECT WAVE_%s,ALL_%s,SAMPLE_FREQ_%s,SAMPLE_NUM_%s,%s FROM [%s]",
		channInfo._cChannNo,channInfo._cChannNo,channInfo._cChannNo,channInfo._cChannNo,ConsCommonField(),
		strtable);
	pDB->Cmd(" WHERE %s='%s'",gc_cTime,strtime);
	while(!(pDB->More()))
	{  //取数据，直到从表中取出为止
		if(iTableType==1){
			iTableType=0;
			strtable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
			vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		}
		else if(iTableType==2){
			iTableType=1;
			strtable.Format("TREND_VIBHOUR_%s",sSuffix.c_str());
			vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		}
		else if(iTableType==3){
			iTableType=2;
			strtable.Format("TREND_VIBDAY_%s",sSuffix.c_str());
			vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		}
		else{
			iErrorCode=-101;
			goto CLEANGETZOOMSPEC;
		}
		pDB->Cmd("SELECT [WAVE_%s],[ALL_%s],[SAMPLE_FREQ_%s],[SAMPLE_NUM_%s],%s FROM [%s]",
			channInfo._cChannNo,channInfo._cChannNo,channInfo._cChannNo,channInfo._cChannNo,ConsCommonField(),
			strtable);
		pDB->Cmd(" WHERE %s='%s'",gc_cTime,strtime);

	}

	iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChann);
	iSmpNum  = SimADO_GetSampleNumEx(pDB, sChann);

	float fRealDf=(float)iSmpFreq/iSmpNum;
	fDf=(iStopFreq-fStartFreq)/iLines;

	if(iStopFreq>(iSmpFreq/2)){
		iErrorCode=-6;
		goto CLEANGETZOOMSPEC;
	}
	//要求最小要细化2倍
	if(!((2*fDf)<fRealDf)){
		iErrorCode=-1;
		goto CLEANGETZOOMSPEC;
	}
	//细化分析不能超过20倍
	if(!((20*fDf)>fRealDf)){
		iErrorCode=-2;
		goto CLEANGETZOOMSPEC;
	}

	long iByteSize;
	sTmp="ALL_"+strChann;
	pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,pDB->GetDouble(sTmp));
	//得到波形数据
	if (iLines<=0 || iSmpNum<=0)
	{
		return -1;
	}
	pfwave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pfZoomSpm=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pfZoom1=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pfwave1=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
	iByteSize=iSmpNum*sizeof(DATATYPE_WAVE);
	sTmp="WAVE_"+strChann;
	SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE *)pfwave, iByteSize,sWaveHead);
	int iRealWaveNum=iByteSize/sizeof(DATATYPE_WAVE);
	pPlantInfo->ConvertDTWaveToFloat(pfwave1,pfwave,iRealWaveNum,sWaveHead);
	long iRealLines=iLines;
	float iRealStartFreq=0;
	iPtNum=iPtNum>iRealWaveNum?iRealWaveNum:iPtNum;
	//ZoomFFT(float data[], long data_len, short startFreq, short stopFreq, short nLines, short SamplingRate, float zoomSpectrum[], long *len, float *f0, float *df);
	try
	{
#ifndef __NO_ZOOM_FFT

		if (access("c:\\ee.txt", 0) == 0)
		{
			if (!DeleteFile("c:\\ee.txt"))
			{
				ASSERT(FALSE);
			}
		}

		iErrorCode=ZoomFFT(pfwave1,iRealWaveNum,fStartFreq,iStopFreq,iLines,iSmpFreq,pfZoom1,&iRealLines,&iRealStartFreq,&fDf);
		
		if (iRealLines >3)
		{
			pfZoom1[iRealLines-1] = 0;
			pfZoom1[iRealLines-2] = 0;
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
		iErrorCode = -1;
	}

	pPlantInfo->ConvertFloatToDTWave(pfZoomSpm,pfZoom1,iRealLines,sWaveHead);
	if(iErrorCode==0){
		pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char *)pfwave,iPtNum*sizeof(DATATYPE_WAVE));
		pFixComm->SetItemBuf(FIELD_SERVICE_ZOOMSPECTRUM,(char*)pfZoomSpm,iRealLines*sizeof(DATATYPE_WAVE));
		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iPtNum);
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_STARTFREQ,iRealStartFreq);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,fDf);
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iRealLines);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
	}
	if(pfwave!=NULL) delete [] pfwave;
	if(pfZoomSpm!=NULL) delete [] pfZoomSpm;
	if(pfwave1!=NULL) delete [] pfwave1;
	if(pfZoom1!=NULL) delete [] pfZoom1;
CLEANGETZOOMSPEC:
	switch(iErrorCode){
	case 0:
		sTmp="";
		break;
	case -1:
		sTmp="细化分辨率大于未细化的分辨率";
		break;
	case -2:
		sTmp="细化倍数超过范围（即超过20倍）";
		break;
	case -3:
		sTmp="起始频率大于终止频率或者设置参数有误";
		break;
	case -4:
	case -5:
		sTmp="分析出错";
		break;
	case -6:
		sTmp="终止频率超出范围";
		break;
	case -100:
		sTmp="设备或测点信息有误";
		break;
	case -101:
		sTmp="数据库中没有记录";
		break;
	default:
		sTmp="未知错误";
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,sTmp);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iErrorCode);
	pFixComm->Write(pFixComm->GetAci());
	return iErrorCode;
}


/**得到指定公司、分厂、机组、测点和时间的一组轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          时间                 FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          垂直方向波形数据                 FIELD_SERVICE_AXESTRACK_WAVE_VER
*          垂直方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          垂直方向特征值结构体             FIELD_SERVICE_CHARVALUE_S2
*          垂直方向测点报警值               FIELD_SYSTEM_ALARM_LOW2
*          垂直方向测点危险值               FIELD_SYSTEM_ALARM_HIGH2
*          垂直方向测点单位                 FIELD_SERVICE_CHANN2_UNIT
*          垂直方向测点别名                 FIELD_SYSTEM_CHANN2_ALIAS
*          水平方向波形数据                 FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          水平方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          水平方向特征值结构体             FIELD_SERVICE_CHARVALUE_S
*          水平方向测点报警值               FIELD_SYSTEM_ALARM_LOW
*          水平方向测点危险值               FIELD_SYSTEM_ALARM_HIGH
*          水平方向测点单位                 FIELD_SERVICE_CHANN1_UNIT
*          水平方向测点别名                 FIELD_SYSTEM_CHANN_ALIAS
*          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
*          指定机组测点的转速               FIELD_SERVICE_REV
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                       FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
#define     SERVICE_SERVICE_OFF_GETAXESTRACK           21008
*/
int GetNearestPoint(int anPoint)
{
	int lnPoint = GD_MIDDLEWARE_WAVE_POINT;
	if (anPoint<GD_MIDDLEWARE_WAVE_POINT)
	{
		BOOL lbFound = false;
		for (int i=14;i>0;i--)
		{
			int lnData = (int)pow(2,i);
			if(anPoint>=lnData)
			{
				lnPoint = lnData;
				lbFound = true;
				break;
			} 
		}
		if (!lbFound)
		{
			lnPoint = 0;
		}
	}
	return lnPoint;
}
int CTransactData::GetAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	CString sTemp;
	int iErrorCode=-1;
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo ==NULL)
		return -1;

	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);

	string sLocation=pFixComm->GetItem(FIELD_SYSTEM_AXISLOCATION);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	CString strTime =  CPlantInfoBase::TimeConvertString(timeOff);
	CString strRevType="";
	int ihours=0;
	int iSmpFreq=1,iSmpNum=1,iRev=0;
	BYTE *pZippedBytes=NULL;
	BYTE *pUnZippedBytes=NULL;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	double *pWaveVerData=NULL;
	double *pWaveHorData=NULL;
	float fRev=0;
	long iZipBytes=-1,iAllBytes=0;
	iWaveNum=iWaveNum<100?513:iWaveNum;

	S_ChannInfo channInfoVer;
	S_ChannInfo channInfoHor;

	string sChannNoVer;
	string sChannNoHor;

	if ((sChannIDHor.empty())||(sChannIDVer.empty()))
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),gc_cLocation,sLocation.c_str());
		if (pDB->More())
		{
			sChannNoHor = pDB->Get(gc_cHorChann);
			sChannNoVer = pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		channInfoHor = pPlantInfo->GetSpecVibChannNoInfo(sChannNoHor);
		channInfoVer = pPlantInfo->GetSpecVibChannNoInfo(sChannNoVer);

		sChannIDHor  = channInfoHor._cChannID;
		sChannIDVer  = channInfoVer._cChannID;
	}
	else
	{
		channInfoHor = pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
		channInfoVer = pPlantInfo->GetSpecVibChannInfo(sChannIDVer);

		sChannNoHor  = channInfoHor._cChannNo;
		sChannNoVer  = channInfoVer._cChannNo;
	}

	CZipImpl zipImpl;
	//要提取波形数据的表单名字
	CString strTable;

	CString strShowField;
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	//sTemp.Format("sChannNoVer:%s,sChannNoHor:%s",channInfoVer._cChannNo,channInfoHor._cChannNo);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"Off:CTransactData","GetAxesTrack");
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	S_VibCharValue vibCharValue;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN, pDB,timeOff, iTableType, sSuffix),sSuffix.c_str());
	strShowField.Format("%s,%s,%s",
		ConsVibChannField(sChannNoVer,true),
		ConsVibChannField(sChannNoHor,true),
		ConsCommonField());
	//pDB->ClearSql();
	//构造查询Sql语句
	
	pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
	pDB->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;

	BOOL lbGetDataSucc = TRUE;

	while(!(pDB->More()))
	{
		if (pDB->GetLastError() < 0)
		{
			lbGetDataSucc = FALSE;
			goto CLEAN_GETAXESTRACT;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
		{
			lbGetDataSucc = FALSE;
			break;
		}
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN, iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
		pDB->Cmd(" WHERE %s='%s'",gc_cTime,strTime);
	}
	
	if (!lbGetDataSucc)
	{
		strTable.Format("%s%s",GetTblNamePrefixAlarmSud(GE_VIBCHANN,SEARCH_ALARM),sSuffix.c_str());

		pDB->Cmd("SELECT %s FROM [%s]",strShowField,strTable);    
		pDB->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);

		if (!pDB->More())
		{
			if (pDB->GetLastError() < 0)
			{		
				return -1;
			}

			//当按照精确时间查询失败时，则按照相邻的一段时间查询数据
			CString strSqlWhere;

			BOOL lbDataFound = FALSE;

			for (int i =0 ;i< 50;i++)
			{
				int lnTimeSpan = ALARMRECORDS_QUERYTIMESPAN *pow(i+1,2);

				COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, lnTimeSpan );
				CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
				CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
				dtStart = timeOff - timeSpan;
				dtEnd   = timeOff + timeSpan;
				strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
				//获取报警数据
				pDB->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
				pDB->Cmd(" WHERE %s ", strSqlWhere);

				if (lnTimeSpan > ALARMRECORDS_QUERYTIMESPAN_MAX)
				{
					break;
				}

				if(!(pDB->More()))
				{
					continue;
				}else
				{
					lbDataFound = TRUE;
					break;
				}

			}

			if (!lbDataFound)
			{
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				return -2;
			}
		}
	}

	//得到波形数据
	GetSpecWaveByTimeRange( pDB,
							strTable,
							iTableType,
							timeOff,
							strShowField,
							SimADO_GetSampleNumEx(pDB, sChannNoHor),
							sChannNoHor.c_str(),
							GE_DATA_SOURCE_ONLINE);


	iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannNoHor);
	iSmpNum  = SimADO_GetSampleNumEx(pDB, sChannNoHor);
	iRev     = SimADO_GetRevEx(pDB, sChannNoHor);
	fRev=(float)iRev;
	//sTemp.Format("_iSmpFreq:%d,_iSmpFreq:%d",iSmpFreq,iSmpNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"Off:CTransactData","GetAxesTrack");
	//fRev=(float)iRev;
	if (iSmpNum>1)
	{
		std::vector<BYTE> loUnZippedBytes,loZippedBytes;
		std::vector<DATATYPE_WAVE> loHorWave,loVerWave;
		std::vector<double>loWaveHorData,loWaveVerData;
		std::vector<float> lofWaveHorData,lofWaveVerData;

		iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
		loUnZippedBytes.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		loZippedBytes.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pUnZippedBytes = &loUnZippedBytes.front();
		pZippedBytes   = &loZippedBytes.front();
		//得到水平方向的波形数据
		strRevType.Format("WAVE_%s", sChannNoHor.c_str());
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, strRevType, pUnZippedBytes, iAllBytes,sWaveHead);
		int iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		//sTemp.Format("iSmpNum:%d,iRealWaveNum:%d",iSmpNum,iRealWaveNum);BHTRACE_DEBUG(sTemp);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		//iWaveNum = iWaveNum-iWaveNum%2;
		iWaveNum = GetNearestPoint(iWaveNum);
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
		//转化水平方向波形数据为double形数据
		//sTemp.Format("iWaveNum:%d",iWaveNum);BHTRACE_DEBUG(sTemp);
		loHorWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pHorWave = &loHorWave.front();
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
		memcpy( pHorWave,pUnZippedBytes,iAllBytes);
		loWaveHorData.resize(iWaveNum);
		pWaveHorData = &loWaveHorData.front();
		pPlantInfo->ConvertDTWaveToDouble(pWaveHorData,pHorWave,iWaveNum,sWaveHead);


		/*
		if (zipImpl.Zip(pUnZippedBytes,iAllBytes,pZippedBytes,iAllBytes,iZipBytes))
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char *)pZippedBytes,iZipBytes);
		else
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pUnZippedBytes,iAllBytes);
		*/

		// 压缩可能崩溃
		int lnPrecisionEnable = 0;
		if(sWaveHead.GetLength() > 1)
		{
			lnPrecisionEnable = sWaveHead.m_pParam[1];
			
		}

		lofWaveHorData.resize(iWaveNum);
		pPlantInfo->ConvertDTWaveToFloat(&lofWaveHorData.front(),pHorWave,iWaveNum,sWaveHead);

		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnable);
		if(lnPrecisionEnable > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR_FLOAT,(char*)&lofWaveHorData.front(),(int)iWaveNum*sizeof(float));
		}
		pPlantInfo->ConvertFloatToDTWave(pHorWave,&lofWaveHorData.front(),iWaveNum,sWaveHead);

		pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorWave,iAllBytes);
		pFixComm->SetItem(FIELD_SERVICE_ZIPWAVE_BYTESNUM1,iZipBytes);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);

		iZipBytes=-1;
		//得到垂直方向的波形数据
		strRevType.Format("WAVE_%s", sChannNoVer.c_str());
		iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
		SimADO_SafeGetBufferEx(pDB, strRevType, pUnZippedBytes, iAllBytes,sWaveHead);
		iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		//iWaveNum = iWaveNum-iWaveNum%2;
		iWaveNum = GetNearestPoint(iWaveNum);
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
        //转化垂直方向波形数据为double形数据
		loVerWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pVerWave = &loVerWave.front();
		//pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
		memcpy( pVerWave,pUnZippedBytes,iAllBytes);
		loWaveVerData.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pWaveVerData = &loWaveVerData.front();
		//pWaveVerData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pPlantInfo->ConvertDTWaveToDouble(pWaveVerData,pVerWave,iWaveNum,sWaveHead);

		/*
		//	writefile((float*)pUnZippedBytes,iAllBytes/4);
		if (zipImpl.Zip(pUnZippedBytes,iAllBytes,pZippedBytes,iAllBytes,iZipBytes))
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char *)pZippedBytes,iZipBytes);
		else
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pUnZippedBytes,iAllBytes);
		*/

		// 压缩可能崩溃
		

		lofWaveVerData.resize(iWaveNum);
		pPlantInfo->ConvertDTWaveToFloat(&lofWaveVerData.front(),pVerWave,iWaveNum,sWaveHead);

		if(lnPrecisionEnable > 0)
		{
			
			
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER_FLOAT,(char*)&lofWaveVerData.front(),(int)iWaveNum*sizeof(float));
		}

		pPlantInfo->ConvertFloatToDTWave(pVerWave,&lofWaveVerData.front(),iWaveNum,sWaveHead);

		pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerWave,iAllBytes);
		pFixComm->SetItem(FIELD_SERVICE_ZIPWAVE_BYTESNUM,iZipBytes);

		pFixComm->SetItem(FIELD_SERVICE_AXESTRACK_WAVENUM,iWaveNum);
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
		pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
		//转子旋向
		int iRotation =-1;
		iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);
		//进动方向
		if(iWaveNum>1)
		{
			int iPression = 0;
			int iOrbitShape=0;
			loWaveHorData.clear();
			loWaveVerData.clear();
			loWaveHorData.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
			loWaveVerData.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
			pWaveHorData = &loWaveHorData.front();
			pWaveVerData = &loWaveVerData.front();
			pPlantInfo->ConvertDTWaveToDouble(pWaveVerData,pVerWave,iWaveNum,sWaveHead);
			pPlantInfo->ConvertDTWaveToDouble(pWaveHorData,pHorWave,iWaveNum,sWaveHead);
			double dWorkFreq=(double)fRev/60;
			CSynopsis2KB calcOrbit;
			int icalcOrbit=-1;
			if (dWorkFreq>.0)
			{
				try
				{
					icalcOrbit = calcOrbit.GetOrbitShape(pWaveVerData,pWaveHorData,iWaveNum,dWorkFreq,iSmpFreq,iPression,iOrbitShape);
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

				//-----计算全频谱 begin-------
				CSigMath calcFullSpecInfo;
				double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
				int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM,iFullSpecInfoSize=0;
				int index = 0;
				vector<double> vFullSpecX;
				vector<double> vFullSpecY;
				vFullSpecX.resize(iWaveNum);
				vFullSpecY.resize(iWaveNum);
				BEGIN_ERROR_HANDLE;					
				index = calcFullSpecInfo.FullSpectrumII(pWaveVerData,pWaveHorData,iWaveNum,dWorkFreq,iSmpFreq,vFullSpecX,vFullSpecY);
				END_ERROR_HANDLE;

				typedef pair<float,float> FreqAmp;
				vector<FreqAmp> vFullInfo;
				iFullSpecInfoSize = vFullSpecX.size();

				if (index > -1)
				{
					index = 0;
					for (int iNum =0;iNum<iFullSpecInfoSize;iNum++)
					{
						index++;

						vFullInfo.push_back(make_pair(vFullSpecX[iNum],vFullSpecY[iNum]));
					}

					int iFullInfoSize = vFullInfo.size();

					float lfLeftData = -1;
					float lfRightData = -1;
					int lbFound = 0;
					double ldblDf = iSmpFreq*1.0/iWaveNum;
					double ldblLeftSearchFreqBegin = -(dWorkFreq+2*ldblDf);
					double ldblLeftSearchFreqEnd   = -(dWorkFreq-2*ldblDf);

					double ldblRightSearchFreqBegin = (dWorkFreq-2*ldblDf);
					double ldblRightSearchFreqEnd   = (dWorkFreq+2*ldblDf);

					for (int i=0;i<iFullInfoSize;i++)
					{
						FreqAmp loParam = vFullInfo[i];
						float lfDiff = abs(abs(loParam.first)-dWorkFreq);
						float lfCurrentFreq = loParam.first;
						if (lfCurrentFreq<ldblLeftSearchFreqBegin)
						{
							continue;
						}
						if (lfCurrentFreq > ldblRightSearchFreqEnd)
						{
							break;
						}
						if ((lfCurrentFreq<=ldblLeftSearchFreqEnd) 
							&& (lfCurrentFreq>= ldblLeftSearchFreqBegin))
						{
							float lfCurrent = loParam.second;

							if (lfCurrent> lfLeftData)
							{
								lfLeftData = lfCurrent;
							}

						}

						if ((lfCurrentFreq<=ldblRightSearchFreqEnd) 
							&& (lfCurrentFreq>= ldblRightSearchFreqBegin))
						{
							float lfCurrent = loParam.second;

							if (lfCurrent> lfRightData)
							{
								lfRightData = lfCurrent;
							}

						}

					}

					if ((lfLeftData>0)&&(lfRightData>0))
					{
						//反进动
						if ((lfLeftData*1.05)>lfRightData)
						{
							iPression = 1;
						}else
						{
							iPression = 2;
						}
					}
				} //if (index > -1)

				//-----计算全频谱 begin-------
			}


			if (icalcOrbit < 0)
			{	/*CHZLogManage::Error("计算轴心进动方向出错","S_OfflineSev","CTransactData::GetAxesTrack");*/}
			if (iPression==0)//如果计算为不确定则为正进动
				iPression=2;
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,iPression);
			//CString sTemp;sTemp.Format("iRotation:%d,iPression:%d,iOrbitShape:%d",iRotation,iPression,iOrbitShape);BHTRACE_DEBUG(sTemp);
		}
		//得到水平特征值
		vibCharValue=ReadVibCharValue(sChannNoHor,pDB);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfoHor._chAlarmParam._fAlarmHL);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfoHor._chAlarmParam._fAlarmHH);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfoHor._cUnit);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfoHor._cChannAlias);
		//得到垂直特征值
		vibCharValue=ReadVibCharValue(sChannNoVer,pDB);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibCharValue,sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfoVer._chAlarmParam._fAlarmHL);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfoVer._chAlarmParam._fAlarmHH);
		pFixComm->SetItem(FIELD_SERVICE_CHANN2_UNIT,channInfoVer._cUnit);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN2_ALIAS,channInfoVer._cChannAlias);
	}
	else
		pFixComm->SetItem(FIELD_SERVICE_AXESTRACK_WAVENUM,iSmpNum);
	iErrorCode=iSmpNum;
CLEAN_GETAXESTRACT:
	pFixComm->Write(pFixComm->GetAci());
	return iErrorCode;
}

CString CTransactData::ConsCommonField()
{
	CString sFields;
	sFields.Format("%s,%s,%s,%s,%s,%s,%s",
		gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	return sFields;
}

CString CTransactData::ConsVibChannField(string sChann_,bool bWave_/* =false */)
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

string CTransactData::ConsVibChannSpecTypeField(string sChann_,int iType_/* =GE_ALLCHAR */)
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
	default:
		sType="ALL_";
	}
	sType=sType+sChann_;
	return sType;
}


CString CTransactData::ConsDynChannField(string sChann_,bool bWave_/* =false */)
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

string CTransactData::ConsDynChannSpecTypeField(string sChann_,int iType_/* =GE_ALLCHAR */)
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
	default:
		sType="ZERO_";
	}
	sType=sType+sChann_;
	return sType;
}

//从指定数据连接中读取指定测点所有类型特征值
S_VibCharValue CTransactData::ReadVibCharValue(string sChann_,IDBInterFace *pDB_)
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

//从指定数据连接中读取指定动态测点所有类型特征值
S_DynCharValue CTransactData::ReadDynCharValue(string sChann_,IDBInterFace *pDB_)
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

list<string> CTransactData::ParseChannList(char *pChannBuf_,int iNum_)
{
	list<string> lChann;
	lChann.clear();
	char *pTmp=NULL;
	const char pcSep[]={","};
	string sChann;
	if (iNum_<1)
		return lChann;
	else if (iNum_<2)
		lChann.push_back(pChannBuf_);
	else
	{
		pTmp=strtok(pChannBuf_,pcSep);
		while (NULL!=pTmp)
		{
			sChann=pTmp;
			lChann.push_back(sChann);
			pTmp=strtok(NULL,pcSep);
		}
	}
	return lChann;
}

lCHANNID_NO CTransactData::ParseVibChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,int iNum_)
{
	lCHANNID_NO lChannPair;
	list<string> lChann=ParseChannList(pChannBuf_,iNum_);
	list<string>::iterator it;
	for (it=lChann.begin();it!=lChann.end();++it)
		lChannPair.push_back(make_pair(*it,pPlantInfo_->GetSpecVibChannInfo(*it)._cChannNo));
	lChann.clear();
	return lChannPair;
}

lCHANNID_NO CTransactData::ParseDynChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,const int &iNum_)
{
	lCHANNID_NO lChannPair;
	list<string> lChann=ParseChannList(pChannBuf_,iNum_);
	list<string>::iterator it;
	for (it=lChann.begin();it!=lChann.end();++it)
	{
		lChannPair.push_back(make_pair(*it,pPlantInfo_->GetSpecDynChannInfo(*it)._cChannNo));
		//	CString strInfo;strInfo.Format("%s,%s", it->c_str(), pPlantInfo_->GetSpecDynChannInfo(*it)._cChannNo);
		//	CHZLogManage::Info(strInfo, "Offline", "CTransactData::ParseDynChannList");
	}
	lChann.clear();
	return lChannPair;
}

lCHANNID_NO CTransactData::ParseProcChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,int iNum_)
{
	lCHANNID_NO lChannPair;
	list<string> lChann=ParseChannList(pChannBuf_,iNum_);
	list<string>::iterator it;
	for (it=lChann.begin();it!=lChann.end();++it)
		lChannPair.push_back(make_pair(*it,pPlantInfo_->GetSpecProcChannInfo(*it)._cChannNo));
	lChann.clear();
	return lChannPair;
}

// get specification channel sensor type
int CTransactData::GetSpecChannSensorType(string sCompany_,string ag_strPlant, string ag_strChann, int &iRevType_,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int  iSensorType=0;
	pDB->Cmd("SELECT %s,%s FROM [%s] ",gc_cSensorType,gc_cRevType,gc_cChannTable);
	pDB->Cmd(" WHERE %s='%s' AND  %s='%s' AND %s='%s'",gc_cCompany,sCompany_.c_str(),gc_cPlantID,ag_strPlant.c_str(),gc_cChannID,ag_strChann.c_str());
	if (pDB->More())
	{
		iSensorType=pDB->GetLong(gc_cSensorType);
		iRevType_=pDB->GetLong(gc_cRevType);
	}
	else
	{
		iSensorType=GE_SENSORTYPE_OTHER;
		iRevType_=1;
	}
	return iSensorType;
}


/*          波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
*/
int CTransactData::ReadVibChannSpecTimeWaveAndSpectrum(ICommInterFace *pFixComm_,
													   IDBInterFace *pDB_,
													   string sCompany_,
													   string sFactory_,
													   string sPlant_,
													   string sChann_,
													   int iLines_,
													   int iWaveNum_,
													   CBHDateTime timeSpec_)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlantInfo = CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_);
	
	if (pPlantInfo == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SwitchDbNameEx(pDB_,sSuffix.c_str());
	
	if (iWaveNum_<100)
	{
		ASSERT(FALSE);
		iWaveNum_=513;
	}
	
	if((iLines_>6400)||(iLines_<400))
	{
		iLines_=400;
	}
	
	iRealWaveNum=iLines_*2.56;

	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo( sChann_ );
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	sPrefix=GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_,timeSpec_,iTableType,sSuffix);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
						ConsVibChannField(channInfo._cChannNo,true),
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

	GetSpecWaveByTimeRange(pDB_,strTable,iTableType,timeSpec_,strShowField,SimADO_GetSampleNumEx(pDB_, channInfo),channInfo._cChannNo,channInfo._iDataSource);
	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);

	//得到波形数据
	long isize;
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];//按照设备组态值开辟内存

		vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		vector<float> lvfSpectrum(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

		float *pfWave = &lvfWave.front();
		float *pfSpectrum = &lvfSpectrum.front();

		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;

		strWave.Format("WAVE_%s",channInfo._cChannNo);
	
		//在数据库组态的设备采样点数和数据库中实际保存的采样点数中会取一个较小的值，拷贝到pDtWave中，并将isize设置为较小的值
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize,sWaveHead);
	
		inum=isize/sizeof(DATATYPE_WAVE);//

	
		if(inum<iRealWaveNum)
		{
			ASSERT(FALSE);
			iRealWaveNum=inum;
		}

		pDtfreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		iWaveNum_=iWaveNum_>inum?inum:iWaveNum_;

		iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,inum,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df,pfWave,pfSpectrum,sWaveHead);
		//if (iRealWaveNum>0) 	   df=(float)(ifreq)/(float)iRealWaveNum;因为细化分析注释掉
	
		S_VibCharValue vibValue;
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);
		pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtfreq,(iLines_)*sizeof(DATATYPE_WAVE));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_NUM,iLines_);
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum_*sizeof(DATATYPE_WAVE));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum_);
		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,iRev);
		pFixComm_->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);

		int lnPrecisionEnabled = sWaveHead.m_pParam[1];

		pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
		if(lnPrecisionEnabled > 0)
		{
			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iWaveNum_*sizeof(float));
			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)pfSpectrum,iLines_*sizeof(float));
		}
	}
	else
		pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,0);
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );
	return inum;
}

/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          单位            FIELD_SERVICE_CHANN1_UNIT
*          转速            FIELD_SERVICE_OFF_REV
*/
int CTransactData::ReadVibChannSpecTimeWaveAndZoomSpectrum(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,  int iLines_,int iWaveNum_, CBHDateTime timeSpec_, float fStartFreq_, float fStopFreq_ /* = -1 */)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL,*pDtfreq=NULL;
	float *pfWave = NULL, *pfFreq = NULL;
	float df=0.0,fRev=.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	int iRealWaveNum=0,istep2=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	fStopFreq_ = -1;

	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlantInfo = CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());

	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo( sChann_ );
	WriteChannInfoToFixComm(pFixComm_,channInfo);

	sPrefix=GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_,timeSpec_,iTableType,sSuffix);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());

	CString strShowField;
	strShowField.Format("%s, %s, CUSTOM_PARAM_%s",
		ConsCommonField(),
		ConsVibChannField(channInfo._cChannNo,true),
		channInfo._cChannNo);
	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	//BHTRACE_DEBUG(pDB_->GetSql());
	iTableType=GE_TREND_TBLTYPE_MONTH;
	
	bool lbFindData = true;

	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
		{		
			return -1;
		}

		//常规表中没有查到波形则从报警表中查询
		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
		{
			lbFindData = false;
			break;
		}
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		//BHTRACE_DEBUG(pDB_->GetSql());
	}

	if (!lbFindData)
	{
		strTable.Format("%s%s",GetTblNamePrefixAlarmSud(GE_VIBCHANN,SEARCH_ALARM),sSuffix.c_str());
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);

		if (pDB_->More())
		{
			if (pDB_->GetLastError() < 0)
			{		
				return -1;
			}
		}
	}

	GetSpecWaveByTimeRange(pDB_,strTable,iTableType,timeSpec_,strShowField,SimADO_GetSampleNumEx(pDB_, channInfo),channInfo._cChannNo,channInfo._iDataSource);
	
	// 得到波形数据
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();

	// 波形字节数
	long isize = 0;
	iWaveNum_ = GD_MIDDLEWARE_WAVE_POINT_MAX;
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。2006.5.10 by marble
		//得到自定义参数信息

		float  *pfCustomParom =new float[GD_CUSTOMPARAM_BUFFSIZE];
		memset(pfCustomParom,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
		CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",channInfo._cChannNo);
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB_, sTmp, (BYTE*)pfCustomParom, iAllBytes,sWaveHead);
		/*CString sTemp;
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			sTemp.Format("Vib:pfCustomParom[%d]:%f",i,pfCustomParom[i]);
			BHTRACE_DEBUG(sTemp);
		}*/
		pFixComm_->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBytes);
        //得到波形数据
		pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];	
		pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];	
		pfFreq = new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize ,sWaveHead);
		inum=isize/sizeof(DATATYPE_WAVE);
		//        writefile(pfwave,inum);

		if (iWaveNum_<100)
		{
			iWaveNum_=513;
		}

		if (iLines_ == 0)
		{
			iLines_ = (int)inum/2.56;
		}
		else if((iLines_>6400)||(iLines_<400))
		{
			iLines_=400;
		}

		iRealWaveNum=iLines_*2.56;

		if(inum<iRealWaveNum)
		{
			iRealWaveNum=inum;
		}
		
		//int iFreqNum = 10+(iLines_*1.28);
		//iFreqNum = iFreqNum > inum ? iFreqNum:inum;
		pDtfreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		iWaveNum_ = iWaveNum_ > inum ? inum:iWaveNum_;

		//CString strTmp;strTmp.Format("%d,%d,%d",inum,iLines_,ifreq);AfxMessageBox(strTmp);
		//iRealWaveNum=SpectrumAnalysis(pPlantInfo,pDtWave,iWaveNum_,pDtfreq,iLines_,channInfo._iSensorType,ifreq,df, fStartFreq_, fStopFreq_);
		//if (iRealWaveNum>0) 	   df=(float)(ifreq)/(float)iRealWaveNum;因为细化分析注释掉
		// CString strTmp;strTmp.Format("%.2f",df);AfxMessageBox(strTmp);
		S_VibCharValue vibValue;
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);
		int iStartIndex,iEndIndex;
		pPlantInfo->GetSpecVibChannStartEndIndex(pDB_,channInfo._cChannNo,timeSpec_,iStartIndex,iEndIndex);
		iRealWaveNum=SpectrumAnalysis(pPlantInfo,
			pDtWave,
			iWaveNum_,
			pDtfreq,
			iLines_,
			channInfo._iSensorType,
			ifreq,
			df, 
			fStartFreq_, 
			fStopFreq_,
			pfWave,
			pfFreq,
			sWaveHead);

		
		if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
		{
			if (iWaveNum_>iEndIndex*2)
			{
				pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iEndIndex*2);
			}else
			{
				pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum_);
			}
		}
		else
		{
			pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum_);
		}


		pFixComm_->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtfreq,(iLines_)*sizeof(DATATYPE_WAVE));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_NUM,iLines_);
		pFixComm_->SetItem(FIELD_SERVICE_ZOOM_STARTFREQ, fStartFreq_);
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum_*sizeof(DATATYPE_WAVE));

		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
		fRev=(float)iRev;
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
		//CString sTemp;sTemp.Format("ALARM_LOW:%f, ALARM_HIGH:%f",channInfo._chAlarmParam._pThLearnParamS[9]._fParam2,channInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibChannSpecTimeWaveAndZoomSpectrum");
		pFixComm_->SetItem(FIELD_SERVICE_ALARM_LOW,channInfo._chAlarmParam._pThLearnParamS[9]._fParam2);
		pFixComm_->SetItem(FIELD_SERVICE_ALARM_HIGH,channInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
		pPlantInfo->GetSpecVibChannStartEndIndex(pDB_,channInfo._cChannNo,timeSpec_,iStartIndex,iEndIndex);
		pFixComm_->SetItem(FIELD_SERVICE_STARTINDEX,iStartIndex);
		pFixComm_->SetItem(FIELD_SERVICE_ENDINDEX,iEndIndex);
		pFixComm_->SetItem(FIELD_SERVICE_VERFREQ,ifreq);

		int lnPrecisionEnabled = sWaveHead.m_pParam[1];

		pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
		if(lnPrecisionEnabled > 0)
		{
			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iWaveNum_*sizeof(float));
			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)pfFreq,iLines_*sizeof(float));
		}
		KD_DeletePoint(pfCustomParom);
	}
	else
	{
		S_VibCharValue vibValue;
		if (lbFindData)
		{
			vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);
			pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));	
			pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,0);
		}
	}
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );
	KD_DeletePoint( pfWave );
	KD_DeletePoint( pfFreq );
	return inum;
}

/*         波形数据        FIELD_SERVICE_OFF_WAVE
*          倒谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          倒谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          倒谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          单位            FIELD_SERVICE_CHANN1_UNIT
*          转速            FIELD_SERVICE_OFF_REV
*/
int CTransactData::ReadVibChannSpecTimeWaveAndCepstrum(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,int iWaveNum_, CBHDateTime timeSpec_)
{
	COleDateTimeSpan timeSpan;
	DATATYPE_WAVE *pDtWave=NULL;
	double *pDtfreq=NULL;
	float df=0.0;
	int inum=0,ifreq=0,iRev=0,ihours=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	//要提取波形数据的表单名字
	CString strTable;

	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlantInfo = CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if (iWaveNum_<100)
		iWaveNum_=513;
	string sPrefix;

	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo( sChann_ );
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	sPrefix=GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_,timeSpec_,iTableType,sSuffix);
	strTable.Format("%s%s",sPrefix.c_str(),sSuffix.c_str());
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsVibChannField(channInfo._cChannNo,true),
		ConsCommonField());
	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
			return -1;

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			return -1;
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	}
	//得到波形数据
	long isize;

	GetSpecWaveByTimeRange(pDB_,strTable,iTableType,timeSpec_,strShowField,SimADO_GetSampleNumEx(pDB_, channInfo),channInfo._cChannNo,channInfo._iDataSource);


	// 得到采样点数和采样频率
	ifreq = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum  = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev  = SimADO_GetRevEx(pDB_, channInfo);
	int lnWavePtMax = pPlantInfo->GetMiddleWareWavePointNumMax();

	float fRev=(float)iRev;
	if (inum>1)
	{
		CWaveHead lsWaveHead;
		vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		isize=inum*sizeof(DATATYPE_WAVE);
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave, (BYTE *)pDtWave, isize,lsWaveHead);
		inum=isize/sizeof(DATATYPE_WAVE);
		pDtfreq=new double[lnWavePtMax/2];
		iWaveNum_ = iWaveNum_ > inum ? inum:iWaveNum_;
		float df  = (float)1.0/ifreq;
		df=1000.0*df;//转化为倒谱的横坐标ms
		int iCepsNums=0;
		pPlantInfo->CepstrumAnalysis(pDtWave, inum, pDtfreq,iCepsNums,lsWaveHead);
		S_VibCharValue vibValue;
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);
		pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm_->SetItem(FIELD_SERVICE_WAVE_COEF,lsWaveHead.m_pParam[0]);
		std::vector<float> loBufferWriteBack;
		loBufferWriteBack.resize(iCepsNums);
		for (int iBuffer= 0; iBuffer < loBufferWriteBack.size();iBuffer++)
		{
			loBufferWriteBack[iBuffer]= pDtfreq[iBuffer];
		}
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)(&loBufferWriteBack.front()),iCepsNums*sizeof(float));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_NUM,iCepsNums);
	    //writefile(pDtfreq,iCepsNums,102);
		pFixComm_->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum_*sizeof(DATATYPE_WAVE));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum_);

		if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
		{
			pPlantInfo->ConvertDTWaveToFloat(&lvfWave.front(),pDtWave,iWaveNum_,lsWaveHead);
			pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
			vector<float> lvFreqWave(loBufferWriteBack.size());

			for (int iBuffer =0;iBuffer<loBufferWriteBack.size();iBuffer++)
			{
				lvFreqWave[iBuffer] = loBufferWriteBack[iBuffer]/10.0;
			}

			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&lvfWave.front(),iWaveNum_*sizeof(float));
			pFixComm_->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)(&lvFreqWave.front()),iCepsNums*sizeof(float));

		}
		else
		{
			pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
		}

	    //writefile(pDtWave,iWaveNum_,103);
		pFixComm_->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);

	}
	else
		pFixComm_->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,0);
	KD_DeletePoint( pDtWave );
	KD_DeletePoint( pDtfreq );
	return inum;
}

/**得到指定两个振动测点的特征值信息、报警限等信息（循环两次得到两个测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一          FIELD_SERVICE_OFF_CHANN1
*          测点一时间值    FIELD_SERVICE_OFF_TIME
*          公司ID2         FIELD_SYSTEM_COMPANY2
*		   分厂名2         FIELD_SYSTEM_FACTORY2
*          机组ID2         FIELD_SERVICE_PLANTID2
*          测点二          FIELD_SERVICE_OFF_CHANN2
*          测点二时间值    FIELD_SERVICE_OFF_TIME2
*返回值的字段码：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO    21006
*/
int CTransactData::GetTwoChannDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount=-1;
	string sCompany1=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory1=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant1=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sChann1=pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	string sCompany2=pFixComm->Get(FIELD_SYSTEM_COMPANY2);
	string sFactory2=pFixComm->Get(FIELD_SYSTEM_FACTORY2);
	string sPlant2=pFixComm->Get(FIELD_SERVICE_PLANTID2);
	string sChann2=pFixComm->Get(FIELD_SERVICE_OFF_CHANN2);
	CBHDateTime timeOff1=CBHDateTime::GetCurrentTime();
	CBHDateTime timeOff2=CBHDateTime::GetCurrentTime();
	timeOff1=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	timeOff2=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME2);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if(!(ReadVibChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany1,sFactory1,sPlant1,sChann1,timeOff1)==NULL))
	{
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany1.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory1.c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant1.c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	nCount=ReadVibChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany2,sFactory2,sPlant2,sChann2,timeOff2);
	if (nCount>0)
	{
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany2.c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory2.c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant2.c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定公司、分厂、设备的任意多个振动测点的特征值信息、报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一时间值    FIELD_SERVICE_OFF_TIME
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO   21009
*/
int CTransactData::GetSpecPlantChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	lCHANNID_NO lChann=ParseVibChannList(pPlant,pChannBuff,iChannNum);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	iChannNum=ReadVibChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,sPlant,lChann,timeOff);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	delete [] pChannBuff;
	return iChannNum;
}

/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*	        分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO       21023
*/
int CTransactData::GetSpecPlantChannInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany;
	string sFactory;
	string sPlant;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL) return -1;

	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=0;
	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		channInfo = pPlantInfo ->GetSpecVibChannInfo( *itChann);
		if( channInfo._cChannID == NULL)
			continue;
		//	channInfo=ReadChannInfo(pDB);
		WriteChannInfoToFixComm(pFixComm,channInfo);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		iChannNum++;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	delete [] pChannBuff;
	return iChannNum;
}

/**        测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
*/
int CTransactData::ReadVibChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_/* =0 */,bool bExactTime_/* =true */,float *pfRev_/* =NULL */)
{
	int ihours=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	//要提取波形数据的表单名字
	CString strTable;
	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_, timeSpec_, iTableType, sSuffix),sSuffix.c_str());

	//得到这个测点基本信息
	S_ChannInfo channInfo=pPlantInfo->GetSpecVibChannInfo(sChann_);

	WriteChannInfoToFixComm(pFixComm_,channInfo);
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsVibChannField(channInfo._cChannNo,false),
		ConsCommonField());
	if (bExactTime_==true)
	{
		//构造查询Sql语句
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;
			strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
			//	AfxMessageBox(pDB_->More());
		}
	}
	else
	{
		//按照相邻的一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, 60 );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;
			strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		}
	}
	S_VibCharValue vibValue;
	vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);
	
	pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));

	/* 参照20014接口, 轴位移测点时直接返回轴位移特征值，其它返回请求的特征值 */
	float fValue = 0;
	std::string sUnit;
	
	if (GE_AXIALCHANN == channInfo._iChannType){
		fValue = vibValue.GetSpecTypeData(GE_AXIALDISPLACE);
	}
	else {
        fValue = vibValue.GetSpecTypeData(iCharType_);
	}
	
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE, fValue);
	
	float fRev=.0;
	switch(channInfo._iRevType)
	{
	case 1:
		fRev = pDB_->GetDouble( gc_cRev);
		pFixComm_->SetItem(FIELD_SERVICE_REV,vibValue._iRev);
		break;
	case 2:
		fRev = pDB_->GetDouble( gc_cRev2 );
		pFixComm_->SetItem(FIELD_SERVICE_REV2,vibValue._iRev);
		break;
	case 3:
		fRev = pDB_->GetDouble( gc_cRev3);
		pFixComm_->SetItem(FIELD_SERVICE_REV3,vibValue._iRev);
		break;
	case 4:
		fRev = pDB_->GetDouble( gc_cRev4);
		pFixComm_->SetItem(FIELD_SERVICE_REV4,vibValue._iRev);
		break;
	case 5:
		fRev = pDB_->GetDouble( gc_cRev5);
		pFixComm_->SetItem(FIELD_SERVICE_REV5,vibValue._iRev);
		break;
	default:
		fRev = pDB_->GetDouble( gc_cRev);
	}
	pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
	//得到5个转速
	float *pfRev=new float[5];
	pfRev[0] = pDB_->GetDouble( gc_cRev);
	pfRev[1] = pDB_->GetDouble( gc_cRev2);
	pfRev[2] = pDB_->GetDouble( gc_cRev3);
	pfRev[3] = pDB_->GetDouble( gc_cRev4);
	pfRev[4] = pDB_->GetDouble( gc_cRev5);
	//CString sTemp;sTemp.Format("%f,%f,%f,%f,%f",pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4]);BHTRACE_DEBUG(sTemp);
	if (pfRev_!=NULL)
		memcpy(pfRev_,pfRev,5*sizeof(float));
	KD_DeletePoint(pfRev);
	return 1;
}


int CTransactData::ReadVibChannSpecTimeCharValueInfoEx(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_/* =0 */,bool bExactTime_/* =true */,float *pfRev_/* =NULL */)
{
	int ihours=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	//要提取波形数据的表单名字
	CString strTable;
	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());

	//得到这个测点基本信息
	S_ChannInfo channInfo=pPlantInfo->GetSpecVibChannInfo(sChann_);

	WriteChannInfoToFixComm(pFixComm_,channInfo);
	CString strShowField;
	strShowField.Format("%s,%s",
		ConsVibChannField(channInfo._cChannNo,false),
		ConsCommonField());
	if (bExactTime_==true)
	{
		//构造查询Sql语句
		strTable.Format("ALARM_VIB_%s",sSuffix.c_str());
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		if(!(pDB_->More()))
		{
			strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_, timeSpec_, iTableType, sSuffix),sSuffix.c_str());

			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
			iTableType=GE_TREND_TBLTYPE_MONTH;
			while(!(pDB_->More()))
			{
				if (pDB_->GetLastError() < 0)
					return -1;

				if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
					return -1;
				strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
				pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
				pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
				iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
				//	AfxMessageBox(pDB_->More());
			}
		}


	}
	else
	{
		//按照相邻的一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 50, 0 );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;
			strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		}
	}
	S_VibCharValue vibValue;
	vibValue=ReadVibCharValue(channInfo._cChannNo,pDB_);

	pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));

	/* 参照20014接口, 轴位移测点时直接返回轴位移特征值，其它返回请求的特征值 */
	float fValue = 0;
	std::string sUnit;

	if (GE_AXIALCHANN == channInfo._iChannType){
		fValue = vibValue.GetSpecTypeData(GE_AXIALDISPLACE);
	}
	else {
		fValue = vibValue.GetSpecTypeData(iCharType_);
	}

	pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE, fValue);

	float fRev=.0;
	switch(channInfo._iRevType)
	{
	case 1:
		fRev = pDB_->GetDouble( gc_cRev);
		pFixComm_->SetItem(FIELD_SERVICE_REV,vibValue._iRev);
		break;
	case 2:
		fRev = pDB_->GetDouble( gc_cRev2 );
		pFixComm_->SetItem(FIELD_SERVICE_REV2,vibValue._iRev);
		break;
	case 3:
		fRev = pDB_->GetDouble( gc_cRev3);
		pFixComm_->SetItem(FIELD_SERVICE_REV3,vibValue._iRev);
		break;
	case 4:
		fRev = pDB_->GetDouble( gc_cRev4);
		pFixComm_->SetItem(FIELD_SERVICE_REV4,vibValue._iRev);
		break;
	case 5:
		fRev = pDB_->GetDouble( gc_cRev5);
		pFixComm_->SetItem(FIELD_SERVICE_REV5,vibValue._iRev);
		break;
	default:
		fRev = pDB_->GetDouble( gc_cRev);
	}
	pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
	//得到5个转速
	float *pfRev=new float[5];
	pfRev[0] = pDB_->GetDouble( gc_cRev);
	pfRev[1] = pDB_->GetDouble( gc_cRev2);
	pfRev[2] = pDB_->GetDouble( gc_cRev3);
	pfRev[3] = pDB_->GetDouble( gc_cRev4);
	pfRev[4] = pDB_->GetDouble( gc_cRev5);
	//CString sTemp;sTemp.Format("%f,%f,%f,%f,%f",pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4]);BHTRACE_DEBUG(sTemp);
	if (pfRev_!=NULL)
		memcpy(pfRev_,pfRev,5*sizeof(float));
	KD_DeletePoint(pfRev);
	return 1;
}


/**        测点ID          FIELD_SERVICE_CHANN1
*        测点别名        FIELD_SYSTEM_CHANN_ALIAS
*        特征值结构体    FIELD_SERVICE_CHARVALUE_S
*        转速            FIELD_SERVICE_OFF_REV
*        报警值          FIELD_SYSTEM_ALARM_LOW
*        危险值          FIELD_SYSTEM_ALARM_HIGH
*        单位            FIELD_SERVICE_CHANN1_UNIT
*/
int CTransactData::ReadVibChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,lCHANNID_NO lChann_,CBHDateTime timeSpec_)
{
	int nCount;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	list<S_ChannInfo> lChannInfo;
	lCHANNID_NO::iterator itChann;
	list<S_ChannInfo>::iterator itInfo;

	for (itChann=lChann_.begin();itChann!=lChann_.end();++itChann)
		lChannInfo.push_back(pPlantInfo->GetSpecVibChannInfo(itChann->first));
	nCount=lChannInfo.size();
	CString strShow,strRequire,strTmp;
	strShow = ConsCommonField();
	//构造显示字符串
	for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
	{
		if (itInfo->_cChannNo =="")
			continue;
		strTmp.Format(",%s",ConsVibChannField(itInfo->_cChannNo,false));
		strShow+=strTmp;
	}
	strRequire.Format(" [%s]='%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	//要提取波形数据的表单名字
	CString strTable;

	//如果在两个小时之类就在小时趋势表中读取波形数据。因为如果web端显示的是当前趋势，则最新的一个点是从小时趋势中提取的，而且只要近期的点只要其它趋势中有的点，小时趋势中都有
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN, pDB_,timeSpec_, iTableType, sSuffix),sSuffix.c_str());
	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShow,strTable);
	pDB_->Cmd(" WHERE %s",strRequire);
	//	AfxMessageBox(pDB_->GetSql());
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
			return -1;


		if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
		{
			lChannInfo.clear();
			return -1;
		}
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN,iTableType),sSuffix.c_str());
		pDB_->Cmd("SELECT %s FROM [%s]",strShow,strTable);
		pDB_->Cmd(" WHERE %s",strRequire);
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		//	AfxMessageBox(pDB_->GetSql());
	}
	S_VibCharValue vibValue;
	for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
	{
		vibValue=ReadVibCharValue(itInfo->_cChannNo,pDB_);
		pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		WriteChannInfoToFixComm(pFixComm_,(*itInfo));
		float fRev=.0;
		switch(itInfo->_iRevType)
		{
		case 1:
			fRev = pDB_->GetDouble( gc_cRev);
			pFixComm_->SetItem(FIELD_SERVICE_REV,vibValue._iRev);
			break;
		case 2:
			fRev = pDB_->GetDouble( gc_cRev2 );
			pFixComm_->SetItem(FIELD_SERVICE_REV2,vibValue._iRev);

			break;
		case 3:
			fRev = pDB_->GetDouble( gc_cRev3);
			pFixComm_->SetItem(FIELD_SERVICE_REV3,vibValue._iRev);
			break;
		case 4:
			fRev = pDB_->GetDouble( gc_cRev4);
			pFixComm_->SetItem(FIELD_SERVICE_REV4,vibValue._iRev);
            break;
		case 5:
			fRev = pDB_->GetDouble( gc_cRev5);
			pFixComm_->SetItem(FIELD_SERVICE_REV5,vibValue._iRev);
            break;
		default:
			fRev = pDB_->GetDouble( gc_cRev);
		}
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
		pFixComm_->Write(pFixComm_->GetAci());
		pFixComm_->CreateNextAnswer();
	}
	lChannInfo.clear();
	return nCount;
}

int CTransactData::ReadDynChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_/* =0 */,bool bExactTime_/* =true */)
{
	int ihours=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	CString strTable;
	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_DYNPRESSCHANN,pDB_, timeSpec_, iTableType, sSuffix),sSuffix.c_str());

	//得到这个测点基本信息
	S_ChannInfo channInfo=pPlantInfo->GetSpecDynChannInfo(sChann_);
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	CString strShowField;
	strShowField.Format("%s,%s,%s,%s,%s,%s",ConsDynChannField(channInfo._cChannNo,false),gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	if (bExactTime_==true)
	{
		//构造查询Sql语句
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
				return -1;
			strTable.Format("%s%s",GetTblNamePrefix( GE_DYNPRESSCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		}
	}
	else
	{
		//按照一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, 60 );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;
			//获取报警数据
			strTable.Format("%s%s",GetTblNamePrefix( GE_DYNPRESSCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		}
	}
	S_DynCharValue dynValue;
	dynValue=ReadDynCharValue(channInfo._cChannNo,pDB_);
	pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&dynValue,sizeof(S_DynCharValue));
	float fValue=(float)dynValue.GetSpecTypeData(iCharType_);
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,fValue);
	float fRev=.0;
	switch(channInfo._iRevType)
	{
	case 1:
		fRev = pDB_->GetDouble( gc_cRev);
		pFixComm_->SetItem(FIELD_SERVICE_REV,dynValue._iRev);
		break;
	case 2:
		fRev = pDB_->GetDouble( gc_cRev2 );
		pFixComm_->SetItem(FIELD_SERVICE_REV2,dynValue._iRev);
		break;
	case 3:
		fRev = pDB_->GetDouble( gc_cRev3);
		pFixComm_->SetItem(FIELD_SERVICE_REV3,dynValue._iRev);
		break;
	case 4:
		fRev = pDB_->GetDouble( gc_cRev4);
		pFixComm_->SetItem(FIELD_SERVICE_REV4,dynValue._iRev);
		break;
	case 5:
		fRev = pDB_->GetDouble( gc_cRev5);
		pFixComm_->SetItem(FIELD_SERVICE_REV5,dynValue._iRev);
		break;
	default:
		fRev = pDB_->GetDouble( gc_cRev);
	}
	pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
	return 1;
}

int CTransactData::ReadDynChannSpecTimeCharValueInfoEx(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_/* =0 */,bool bExactTime_/* =true */)
{
	int ihours=0;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;

	CString strTable;
	CString strTime= CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());

	//得到这个测点基本信息
	S_ChannInfo channInfo=pPlantInfo->GetSpecDynChannInfo(sChann_);
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	CString strShowField;
	strShowField.Format("%s,%s,%s,%s,%s,%s",ConsDynChannField(channInfo._cChannNo,false),gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	if (bExactTime_==true)
	{
		//构造查询Sql语句
		strTable.Format("ALARM_DYN_%s",sSuffix.c_str());
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		if(!pDB_->More())
		{
			strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_DYNPRESSCHANN,pDB_, timeSpec_, iTableType, sSuffix),sSuffix.c_str());

			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
			iTableType=GE_TREND_TBLTYPE_MONTH;
			while(!(pDB_->More()))
			{
				if (pDB_->GetLastError() < 0)
					return -1;

				if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
					return -1;
				strTable.Format("%s%s",GetTblNamePrefix( GE_DYNPRESSCHANN,iTableType),sSuffix.c_str());
				pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
				pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
				iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
			}
		}

	}
	else
	{
		//按照一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 50, 0 );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);
		iTableType=GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;
			//获取报警数据
			strTable.Format("%s%s",GetTblNamePrefix( GE_DYNPRESSCHANN,iTableType),sSuffix.c_str());
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere);
			iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		}
	}
	S_DynCharValue dynValue;
	dynValue=ReadDynCharValue(channInfo._cChannNo,pDB_);
	pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&dynValue,sizeof(S_DynCharValue));
	float fValue=(float)dynValue.GetSpecTypeData(iCharType_);
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,fValue);
	float fRev=.0;
	switch(channInfo._iRevType)
	{
	case 1:
		fRev = pDB_->GetDouble( gc_cRev);
		pFixComm_->SetItem(FIELD_SERVICE_REV,dynValue._iRev);
		break;
	case 2:
		fRev = pDB_->GetDouble( gc_cRev2 );
		pFixComm_->SetItem(FIELD_SERVICE_REV2,dynValue._iRev);
		break;
	case 3:
		fRev = pDB_->GetDouble( gc_cRev3);
		pFixComm_->SetItem(FIELD_SERVICE_REV3,dynValue._iRev);
		break;
	case 4:
		fRev = pDB_->GetDouble( gc_cRev4);
		pFixComm_->SetItem(FIELD_SERVICE_REV4,dynValue._iRev);
		break;
	case 5:
		fRev = pDB_->GetDouble( gc_cRev5);
		pFixComm_->SetItem(FIELD_SERVICE_REV5,dynValue._iRev);
		break;
	default:
		fRev = pDB_->GetDouble( gc_cRev);
	}
	pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
	return 1;
}


S_ChannInfo CTransactData::ReadChannInfo(IDBInterFace *pDB_)
{
	S_ChannInfo channInfo;
	
	CWaveHead sWaveHead;

	long lAlarmParamSize=sizeof(CChannAlarmParam);
	channInfo._bKeyIF=pDB_->GetLong(gc_cKeyIF)>0?true:false;
	strncpy(channInfo._cChannID,pDB_->Get(gc_cChannID).c_str(),D_BHKD_CHANNID_LEN-1);
	strncpy(channInfo._cChannAlias,pDB_->Get(gc_cChannIDA).c_str(),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(channInfo._cUnit,pDB_->Get(gc_cUnit).c_str(),sizeof(channInfo._cUnit)-1);
	SimADO_SafeGetBufferEx(pDB_, gc_cAlarmParam, (BYTE *)&(channInfo._chAlarmParam), lAlarmParamSize, sWaveHead);
	channInfo._iRevType=pDB_->GetLong(gc_cRevType);
	channInfo._iSensorType=pDB_->GetLong(gc_cSensorType);
	channInfo._iChannType=pDB_->GetLong(gc_cChannType);
	return channInfo;
}

/**得到指定公司、分厂、设备的任意多个过程测点的值信息、报警限等信息（依次循环测点的数据）
*输入参数：
*        公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*        机组ID          FIELD_SERVICE_PLANTID
*          测点时间值      FIELD_SERVICE_OFF_TIME
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值          FIELD_SERVICE_TREND_PROCESS
*          转速            FIELD_SERVICE_OFF_REV
*          报警限          FIELD_SYSTEM_ALARM_LOW
*          危险限          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO   21020
*/
int CTransactData::GetSpecPlantProcChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	lCHANNID_NO lChann=ParseProcChannList(pPlantInfo,pChannBuff,iChannNum);
	lCHANNID_NO::iterator itChann;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	iChannNum=ReadProcChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,sPlant,lChann,timeOff);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	lChann.clear();
	delete [] pChannBuff;
	return iChannNum;
}

/**
*           测点ID          FIELD_SERVICE_CHANN1
*           测点别名        FIELD_SYSTEM_CHANN_ALIAS
*           特征值          FIELD_SERVICE_TREND_PROCESS
*           转速            FIELD_SERVICE_OFF_REV
*           报警限          FIELD_SYSTEM_ALARM_LOW
*           危险限          FIELD_SYSTEM_ALARM_HIGH
*           单位            FIELD_SERVICE_CHANN1_UNIT
*/
int CTransactData::ReadProcChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,lCHANNID_NO lChann_,CBHDateTime timeSpec_)
{
	int nCount=0;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	list<S_ChannInfo> lChannInfo;
	lCHANNID_NO::iterator itChann;
	list<S_ChannInfo>::iterator itInfo;
	for (itChann=lChann_.begin();itChann!=lChann_.end();++itChann)
		lChannInfo.push_back(pPlantInfo->GetSpecProcChannInfo(itChann->first));
	nCount=lChannInfo.size();
	CString strShow,strRequire,strTmp;
	strShow.Format(" [%s],[%s] ",gc_cRev,gc_cRev2);
	//构造显示字符串
	for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
	{
		strTmp.Format(",[%s]",itInfo->_cChannID);
		strShow+=strTmp;
	}
	strRequire.Format(" [%s]='%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));

	//要提取波形数据的表单名字
	CString strTable;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	//如果在两个小时之类就在小时趋势表中读取波形数据。因为如果web端显示的是当前趋势，则最新的一个点是从小时趋势中提取的，而且只要近期的点只要其它趋势中有的点，小时趋势中都有
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_ALLPROC,pDB_, timeSpec_, iTableType, sSuffix),sSuffix.c_str());
	iTableType=GE_TREND_TBLTYPE_MONTH;
	//构造查询Sql语句
	pDB_->Cmd("SELECT %s FROM [%s]",strShow,strTable);
	pDB_->Cmd(" WHERE %s",strRequire);
	while(!(pDB_->More()))
	{
		if (pDB_->GetLastError() < 0)
			return -1;

		if(iTableType==GE_TREND_TBLTYPE_UNKNOWN)
		{
			lChannInfo.clear();
			return -1;
		}
		strTable.Format("%s%s",GetTblNamePrefix( GE_ALLPROC, iTableType), sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB_->Cmd("SELECT %s FROM [%s]",strShow,strTable);
		pDB_->Cmd(" WHERE %s",strRequire);
	}
	for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
	{
		WriteChannInfoToFixComm(pFixComm_,(*itInfo));
		pFixComm_->SetItem(FIELD_SERVICE_TREND_PROCESS,pDB_->GetDouble(itInfo->_cChannID));
		if (itInfo->_iRevType==2)
			pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,pDB_->GetDouble(gc_cRev2));
		else
			pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,pDB_->GetDouble(gc_cRev));
		pFixComm_->Write(pFixComm_->GetAci());
		pFixComm_->CreateNextAnswer();
	}
	lChannInfo.clear();
	return nCount;
}

/**
*           测点ID          FIELD_SERVICE_CHANN1
*           测点别名        FIELD_SYSTEM_CHANN_ALIAS
*           特征值          FIELD_SERVICE_CHANN1VALUE
*           转速            FIELD_SERVICE_REV
*           报警限          FIELD_SYSTEM_ALARM_LOW
*           危险限          FIELD_SYSTEM_ALARM_HIGH
*           单位            FIELD_SERVICE_CHANN1_UNIT
*/
int CTransactData::ReadProcChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,bool bExactTime_/* =true */)
{
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false);
	if (pPlantInfo==NULL)
		return -1;

	/* 根据测点类型分别取数*/
	S_ChannInfo channInfo=pPlantInfo->GetSpecProcChannInfo(sChann_);

	WriteChannInfoToFixComm(pFixComm_,channInfo);

	int iRet;
	if(GE_DATA_SOURCE_OFFLINE == channInfo._iDataSource)
		iRet = GetProcChannCharValUsingOfflineDataSourcByTime(pFixComm_, pDB_, channInfo, timeSpec_, sSuffix, bExactTime_);
	else
		iRet = GetProcChannCharValUsingOnlineDataSourcByTime(pFixComm_, pDB_, channInfo, timeSpec_, sSuffix, bExactTime_);

	return iRet;
}

int CTransactData::GetProcChannCharValUsingOnlineDataSourcByTime(
	ICommInterFace *pFixComm_,IDBInterFace *pDB_,
	const S_ChannInfo & channInfo,
	const CBHDateTime & timeSpec_,
	const string &DBName,
	bool bSearchByExactTime_)
{
	SwitchDbNameEx(pDB_, DBName.c_str());

	CString DBDataTblName;
	E_TREND_TBL_TYPE DBDataTblType = GE_TREND_TBLTYPE_SECOND;

	DBDataTblName.Format("%s%s",GetTblNamePrefix(GE_ALLPROC, DBDataTblType), DBName.c_str());

	//构造显示字符串
	CString retrieveFields;
	retrieveFields.Format("[%s],[%s],[%s], [%s],[%s],[%s]",
		channInfo._cChannNo,gc_cRev,gc_cRev2,
		gc_cRev3,gc_cRev4,gc_cRev5);

	bool lbGetValueFromAlarmValue = false;

	CString searchTime = CPlantInfoBase::TimeConvertString(timeSpec_);
	if (bSearchByExactTime_) {
		/* 按照确定时间段查询*/

		string lstrAlarmTableName = "ALARM_PROC_" + DBName;

		pDB_->Cmd("SELECT [%s],[%s] FROM [%s]",gc_cProcValue,gc_cRev, lstrAlarmTableName.c_str());
		pDB_->Cmd(" WHERE [%s]='%s' AND %s = '%s'",gc_cTime, searchTime,gc_cChannNo,channInfo._cChannNo);

		if(!pDB_->More())
		{
			pDB_->Cmd("SELECT %s FROM [%s]",retrieveFields, DBDataTblName);
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime, searchTime);

			/* 从月表查到秒表 */
			DBDataTblType = GE_TREND_TBLTYPE_MONTH;

			while(!(pDB_->More()))
			{
				if (pDB_->GetLastError() < 0)
					return -1;

				if (GE_TREND_TBLTYPE_UNKNOWN == DBDataTblType)
					return -1;

				DBDataTblName.Format("%s%s", GetTblNamePrefix(GE_ALLPROC, DBDataTblType), DBName.c_str());

				pDB_->Cmd("SELECT %s FROM [%s]",retrieveFields,DBDataTblName);
				pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,searchTime);

				DBDataTblType = (E_TREND_TBL_TYPE)(DBDataTblType-1);
			}
		}
		else
		{
			lbGetValueFromAlarmValue = true;
		}
		
	}
	else {
		/* 按照一段时间查询数, 前后一分钟*/
		COleDateTimeSpan timeSpan = COleDateTimeSpan( 0, 0, 0, 60 );
		CBHDateTime dtStart = timeSpec_ - timeSpan;
		CBHDateTime dtEnd   = timeSpec_ + timeSpan;

		CString strSqlWhere;
		strSqlWhere.Format("[%s] > '%s' AND [%s] < '%s' ",
			gc_cTime, CPlantInfoBase::TimeConvertString(dtStart),
			gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );


		pDB_->Cmd("SELECT %s FROM [%s]", retrieveFields, DBDataTblName);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);

		DBDataTblType = GE_TREND_TBLTYPE_SECOND;

		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if(DBDataTblType==GE_TREND_TBLTYPE_UNKNOWN)
				return -1;

			DBDataTblName.Format("%s%s",GetTblNamePrefix(GE_ALLPROC,DBDataTblType),DBName.c_str());

			pDB_->Cmd("SELECT TOP 1 %s FROM [%s]",retrieveFields, DBDataTblName);
			pDB_->Cmd(" WHERE %s <= '%s' ORDER BY %s DESC", gc_cTime,timeSpec_.Format(),gc_cTime);

			if(!pDB_->More())
			{
				pDB_->Cmd("SELECT TOP 1 %s FROM [%s]",retrieveFields, DBDataTblName);
				pDB_->Cmd(" WHERE %s >= '%s' ORDER BY %s ASC", gc_cTime,timeSpec_.Format(),gc_cTime);
			}
			else
			{
				break;
			}

			DBDataTblType=(E_TREND_TBL_TYPE)(DBDataTblType+1);

			if(DBDataTblType > GE_TREND_TBLTYPE_FAST)
			{
				return -1;
			}
		}
	}


	/* 设置特征值
	*/
	if(lbGetValueFromAlarmValue)
	{
		pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,pDB_->GetDouble(gc_cProcValue));
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,(float)pDB_->GetDouble(gc_cRev));
	}
	else
	{
		pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,pDB_->GetDouble(channInfo._cChannNo));

		// 根据转速类型获取转速
		SetRevByRevType(pFixComm_, pDB_, channInfo._iRevType);
	}
	

	



	return 1;
}


int CTransactData::GetProcChannCharValUsingOfflineDataSourcByTime(
	ICommInterFace *pFixComm_,IDBInterFace *pDB_,
	const S_ChannInfo & channInfo,
	const CBHDateTime & timeSpec_,
	const string &DBName,
	bool bSearchByExactTime_)
{

	SwitchDbNameEx(pDB_, DBName.c_str());

	CString DBDataTblName;
	DBDataTblName.Format("%s_%s", gc_cOfflineTable_Prefix, DBName.c_str());

	CString retrieveFields;
	retrieveFields.Format("[%s],[%s],[%s], [%s],[%s],[%s]",
		gc_cAll,
		gc_cRev,gc_cRev2, gc_cRev3,gc_cRev4,gc_cRev5);

	CString searchTime = CPlantInfoBase::TimeConvertString(timeSpec_);
	if (bSearchByExactTime_) {
		/* 按照确定时间段查询*/
		pDB_->Cmd("SELECT %s FROM [%s]",retrieveFields, DBDataTblName);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",
			gc_cChannNo, channInfo._cChannNo,
			gc_cTime,    searchTime);
		if(!pDB_->More())
		{
			return -1;
		}
	}
	else {
		/* 按照一段时间查询数, 前后一分钟*/
		COleDateTimeSpan timeSpan = COleDateTimeSpan( 0, 0, 0, 60 );
		CBHDateTime dtStart = timeSpec_ - timeSpan;
		CBHDateTime dtEnd   = timeSpec_ + timeSpan;

		CString strSqlWhere;
		strSqlWhere.Format("[%s]='%s' AND [%s] > '%s' AND [%s] < '%s' ",
			gc_cChannNo, channInfo._cChannNo,
			gc_cTime,    CPlantInfoBase::TimeConvertString(dtStart),
			gc_cTime,    CPlantInfoBase::TimeConvertString(dtEnd) );


		pDB_->Cmd("SELECT TOP 1 %s FROM [%s]", retrieveFields, DBDataTblName);
		pDB_->Cmd(" WHERE %s ", strSqlWhere);


		if(!pDB_->More())
		{
			return -1;
		}
	}


	/* 设置特征值
	*/
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE, pDB_->GetDouble(gc_cAll));

	// 根据转速类型获取转速
	SetRevByRevType(pFixComm_, pDB_, channInfo._iRevType);

	return 1;
}


/* 根据转速类型获取转速*/
void CTransactData::SetRevByRevType(ICommInterFace *pFixComm_,IDBInterFace *pDB_, int iRevType_)
{
	map<int, char *>   mapRevTypeDBField;

	mapRevTypeDBField[1] = (char *)gc_cRev;
	mapRevTypeDBField[2] = (char *)gc_cRev2;
	mapRevTypeDBField[3] = (char *)gc_cRev3;
	mapRevTypeDBField[4] = (char *)gc_cRev4;
	mapRevTypeDBField[5] = (char *)gc_cRev5;

	float fRev;
	map<int, char *>::iterator it = mapRevTypeDBField.find(iRevType_);

	if (it == mapRevTypeDBField.end())
		fRev = pDB_->GetDouble(gc_cRev);
	else
		fRev = pDB_->GetDouble(it->second);

	pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev);
}


CPlantInfoOffLine * CTransactData::ReadCheckCompanyInfo(string & sCompany_,string & sFactory_,string & sPlant_,string & sSuffix_,ICommInterFace *pFixComm_,bool bRtn_)
{
	sCompany_=pFixComm_->Get(FIELD_SYSTEM_COMPANY);
	sFactory_=pFixComm_->Get(FIELD_SYSTEM_FACTORY);
	sPlant_=pFixComm_->Get(FIELD_SERVICE_PLANTID);
	return CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix_,pFixComm_,bRtn_);
}

CPlantInfoOffLine * CTransactData::CheckCompanyInfo(const string &sCompany_, const string &sFactory_, const string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_)
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
	//	AfxMessageBox(sConn.c_str());
	CPlantInfoOffLine * lpRet = NULL;
	BOOL lbLockGot = FALSE;
	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{
			MAP_PLANTINFO::iterator it;
			it=g_mapPlantInfo.find(sConn);
			if (it==g_mapPlantInfo.end())
			{
				if (bRtn_)
				{
					pFixComm_->CreateAnswer(pFixComm_->GetFunc());
					pFixComm_->Write(pFixComm_->GetAci());
				}
				g_MapLock.ReleaseReaderLock();
				return NULL;
			}
			sSuffix_=it->second->GetCompanyAlias();
			sSuffix_.append(it->second->GetPlantNo());

			lpRet = it->second;
		}
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
		try
		{
				g_MapLock.ReleaseReaderLock();
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

	}


	return lpRet;
}

/*!得到指定公司、分厂、机组、任意多测点的指定启停车事件的趋势数据。
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          特征值类型      FIELD_SERVICE_TREND_CHARTYPE
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          启停车事件ID    FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*           对应时间的微秒         FIELD_SERVICE_MICROSECOND
*          转速:                FIELD_SERVICE_OFF_REV
*          转速2：              FIELD_SERVICE_OFF_REV2
#define    SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND       21021
*/
int CTransactData::GetSpecEventChannsSudTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount=-1;
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);

	lCHANNID_NO lChann=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	lCHANNID_NO::iterator itChann;
	list<string>::iterator it;
	list<string> lChannField;
	//假如为true，说明所代表测点为空值
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if (itChann->second.c_str() =="")
			continue;
		lChannField.push_back(ConsVibChannSpecTypeField(itChann->second,(CHARTYPE)iCharType));
    }
	CString strShowField,strTmp;
	strShowField.Format("[%s],[%s],[%s],[%s]",gc_cRev,gc_cRev2,gc_cTime,gc_cMicroSecond);
	for (it=lChannField.begin();it!=lChannField.end();++it)
	{
		strTmp.Format(",[%s]",it->c_str());
		strShowField+=strTmp;
	}
	int index=0;
	DATATYPE_CHAR *pfData=NULL;
	iChannNum=lChannField.size();
	pfData=new DATATYPE_CHAR[iChannNum];
	strTmp.Format("SUD_%s",sAlias.c_str());
	pDB->Cmd("SELECT %s from [%s] ",strShowField,strTmp);
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();
	while(pDB->More())
	{
		index=0;
		for (it=lChannField.begin();it!=lChannField.end();++it)
		{
			pfData[index]=pDB->GetDouble(it->c_str());
			++index;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfData,iChannNum*sizeof(DATATYPE_CHAR));
		pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum);
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,pDB->GetDouble(gc_cRev2));

		dtTmp=pDB->GetTime(gc_cTime);
		if (dtTmp.GetYear()<2007)//为了防止年份错误引起一些的BUG
			continue;
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtTmp);
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++nCount;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	if (pfData!=NULL) delete [] pfData;
	delete [] pChannBuff;
	lChann.clear();
	lChannField.clear();
	return nCount;
}

/*!得到指定公司、分厂、机组、事件、任意多测点、保存时间的详细数据
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          启停车事件ID    FIELD_SYSTEM_EVENT_ID
*    启停车数据保存时间    FIELD_SERVICE_OFF_TIME
*   对应时间的微秒         FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*          测点ID               FIELD_SERVICE_CHANN1
*          测点别名             FIELD_SYSTEM_CHANN_ALIAS
*          测点特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          转速:                FIELD_SERVICE_OFF_REV
*          额定转速：           FIELD_SYSTEM_PLANT_REV
*          第一临界转速         FIELD_SERVICE_REV
*          第二临界转速         FIELD_SERVICE_REV2
*          报警限               FIELD_SYSTEM_ALARM_LOW
*          危险限               FIELD_SYSTEM_ALARM_HIGH
*          单位                 FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO    21022
*/
int CTransactData::GetSpecEventTimeChannsSudDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	CBHDateTime timeSud=CBHDateTime::GetCurrentTime();
	timeSud=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	lCHANNID_NO lChann=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	lCHANNID_NO::iterator itChann;
	string sShowField="";
	//假如为true，说明所代表测点为空值
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if (itChann->second.c_str() =="")
			continue;
		sShowField.append(ConsVibChannField(itChann->second,false));
		sShowField.append(",");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(ConsCommonField().GetString());
	list<S_ChannInfo> lChannInfo;
	list<S_ChannInfo>::iterator itInfo;
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		lChannInfo.push_back(pPlantInfo->GetSpecVibChannInfo(itChann->first));
	//得到额定转速
	int iRatingRev1=0,iRatingRev2=0,iCritSpeed1=0,iCritSpeed2=0;
	iRatingRev1=pPlantInfo->GetPlantInfo()._piRev[0];
	iRatingRev2=pPlantInfo->GetPlantInfo()._piRev[1];
	iCritSpeed1=pPlantInfo->GetPlantInfo()._iCritSpeed1st;
	iCritSpeed2=pPlantInfo->GetPlantInfo()._iCritSpeed2nd;

	string	sTimeSud= CPlantInfoBase::TimeConvertString(timeSud);

	int nCount=-1;
	string	sTable="SUD_";
	sTable.append(sAlias);
	pDB->Cmd("select %s from [%s] ",sShowField.c_str(),sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d and [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID,gc_cTime,sTimeSud.c_str(),gc_cMicroSecond,iMicroSecond);
	//	AfxMessageBox(pDB->GetSql());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	if(pDB->More())
	{
		float fRev1=.0,fRev2=.0;
		S_VibCharValue vibValue;
		fRev1=pDB->GetDouble(gc_cRev);
		fRev2=pDB->GetDouble(gc_cRev2);
		for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
		{
			WriteChannInfoToFixComm(pFixComm,(*itInfo));
			pFixComm->SetItem(FIELD_SERVICE_REV,iCritSpeed1);
			pFixComm->SetItem(FIELD_SERVICE_REV2,iCritSpeed2);
			if (itInfo->_iRevType==2)
			{
				pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev2);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,iRatingRev2);
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev1);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,iRatingRev1);
			}
			vibValue=ReadVibCharValue(itInfo->_cChannNo,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibValue,sizeof(S_VibCharValue));
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			nCount++;
		}
	}
	else//中心级同步后时间带毫秒
	{
		timeSud.SetMillisecond(iMicroSecond);
		sTimeSud= CPlantInfoBase::TimeConvertString(timeSud);
		pDB->Cmd("select %s from [%s] ",sShowField.c_str(),sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID,gc_cTime,sTimeSud.c_str(),gc_cMicroSecond,iMicroSecond);

		if(pDB->More())
		{
			float fRev1=.0,fRev2=.0;
			S_VibCharValue vibValue;
			fRev1=pDB->GetDouble(gc_cRev);
			fRev2=pDB->GetDouble(gc_cRev2);
			for (itInfo=lChannInfo.begin();itInfo!=lChannInfo.end();++itInfo)
			{
				WriteChannInfoToFixComm(pFixComm,(*itInfo));
				pFixComm->SetItem(FIELD_SERVICE_REV,iCritSpeed1);
				pFixComm->SetItem(FIELD_SERVICE_REV2,iCritSpeed2);
				if (itInfo->_iRevType==2)
				{
					pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev2);
					pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,iRatingRev2);
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev1);
					pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,iRatingRev1);
				}
				vibValue=ReadVibCharValue(itInfo->_cChannNo,pDB);
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibValue,sizeof(S_VibCharValue));
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				nCount++;
			}
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pChannBuff);
	return nCount;
}

/*          测点ID               FIELD_SERVICE_CHANN1
*          测点别名             FIELD_SYSTEM_CHANN_ALIAS
*          报警限               FIELD_SYSTEM_ALARM_LOW
*          危险限               FIELD_SYSTEM_ALARM_HIGH
*          单位                 FIELD_SERVICE_CHANN1_UNIT
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
*/
void CTransactData::WriteChannInfoToFixComm(ICommInterFace *pFixComm_,S_ChannInfo channInfo_)
{
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1,channInfo_._cChannID);
	pFixComm_->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo_._cChannAlias);
	pFixComm_->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo_._chAlarmParam._fAlarmHL);
	pFixComm_->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo_._chAlarmParam._fAlarmHH);
	pFixComm_->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo_._cUnit);
	pFixComm_->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo_._chAlarmParam._pThLearnParamF[8]._fParam1);
	pFixComm_->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo_._chAlarmParam._pThLearnParamF[8]._fParam2);
	pFixComm_->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo_._chAlarmParam._pThLearnParamF[8]._fParam3);
	pFixComm_->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo_._chAlarmParam._pThLearnParamF[8]._fParam4);
}

CString CTransactData::GetTblNamePrefixForTrend2(const CHANNTYPE &iChannType_,
												 CBHDateTime timeStart_,
												 CBHDateTime timeEnd_,
												 E_TREND_TBL_TYPE& iTblType_,
												 bool & bSubTbl_,
												 bool &bSupTbl_)
{
	if (G_TEMP_ONLINE)
	{
		return GetTblNamePrefixForTrendNotOverWrite(iChannType_,
													timeStart_,
													timeEnd_,
													iTblType_,
													bSubTbl_,
													bSupTbl_);
	}
	/*	1、通过开始时间判断可以在那个表中取数据；
		2、判断是否可以跨表。
	如果可以跨表，则取的记录不够的可以跨表取。
	级别从高到低:月-天-小时-秒
	*/
	//判断可以从那级表中获得数据
	iTblType_=GetTrendTblType(timeStart_);//趋势表范围

	//判断是否可以在其下一级表中取数据
	if (GetTrendTblType(timeEnd_)==iTblType_)
	{
		bSubTbl_=false;
	}
	else
	{
		bSubTbl_=true;
	}

	//判断是否需要在其上级表取数据
	COleDateTimeSpan span=timeEnd_-timeStart_;
	int iHours=span.GetTotalHours();
	int iMinutes=span.GetTotalMinutes();
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

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
		{
			bSupTblTmp=true;
		}
		break;
	case GE_TREND_TBLTYPE_HOUR:
		//20是通过小趋势表和天趋势表两者保存记录间隔差异得到
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF*20))
		{
			bSupTblTmp=true;
		}
		break;
	case GE_TREND_TBLTYPE_DAY:
		//系数3是因为天表中一小时保存3条记录；9是通过天趋势表（20分钟）和月趋势表（3小时）两者保存记录间隔差异得到
		if ((3*iHours)>(D_TRENDRECORDS*TRENDRECORDS_COEF*9))
		{
			bSupTblTmp=true;
		}
		break;
	default:
		break;
	}
	//	CString strTmp;strTmp.Format("%d,%d,%d,%d",bSupTbl_,iTblType_,iMinutes,iHours);AfxMessageBox(strTmp);
	if (bSupTblTmp)
	{
		bSupTbl_=true;
		iTblType_=(E_TREND_TBL_TYPE)(iTblType_+1);
		if (iTblType_<GE_TREND_TBLTYPE_MONTH)
		{
			goto GetTblNamePrefixForTrend2_JUDGE_TBLTYPE;
		}
	}
	//	strTmp.Format("%d,%d",bSupTbl_,iTblType_);AfxMessageBox(strTmp);
	return GetTblNamePrefix(iChannType_,iTblType_);
}

/*! 根据时间确定表类型，再根据测点类型和表类型获取表前缀
 \param[in]  iChannType_
 \param[in]  timeEnd_
 \param[out] iTblType_
 */
CString CTransactData::GetTblNamePrefixForWave2(const CHANNTYPE &iChannType_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_)
{
	iTblType_=GetTrendTblType(timeEnd_);
	return GetTblNamePrefix( iChannType_,iTblType_);
}

/*! 根据时间确定表类型，再根据测点类型和表类型获取表前缀
 \param[in]  iChannType_
 \param[in]  timeEnd_
 \param[out] iTblType_
 */
CString CTransactData::GetTblNamePrefixForWave2Ex(const CHANNTYPE &iChannType_,IDBInterFace *pDB_,CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_,const string& sSuffix_)
{
	iTblType_=GetTrendTblTypeEx(iChannType_,pDB_,timeEnd_,sSuffix_);
	return GetTblNamePrefix( iChannType_,iTblType_);
}

/*! 通过测点类型和表类型确定表前缀
 \param[in]  iChannType_: 振动 动态 过程量
 \param[in]  iTblType_:   月，日，时，秒
 \return 表前缀
 */
CString CTransactData::GetTblNamePrefix(const CHANNTYPE &iChannType_, E_TREND_TBL_TYPE &iTblType_/* =GE_TREND_TBLTYPE_SECOND */)
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
	case GE_TREND_TBLTYPE_FAST:
		strSuffix+="FAST_";
		break;
	case GE_TREND_TBLTYPE_UNKNOWN:
	default:
		strSuffix+="SECOND_";
		iTblType_=GE_TREND_TBLTYPE_SECOND;
	}
	return strSuffix;
}

/*! 根据时间获取表类型
 根据当前时间与要求时间两者之间的间隔
 \param[in] time_
 \return 表类型 月，日，时，秒
 */
E_TREND_TBL_TYPE CTransactData::GetTrendTblType(CBHDateTime time_)
{
	COleDateTimeSpan span=CBHDateTime::GetCurrentTime()-time_;
	
	int iDays=span.GetTotalDays();
	
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;

	if (G_TEMP_ONLINE)
	{
		return iTblType;
	}

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

/*! 根据时间获取表类型
 根据当前时间与要求时间两者之间的间隔
 \param[in] time_
 \return 表类型 月，日，时，秒
 */
E_TREND_TBL_TYPE CTransactData::GetTrendTblTypeEx(const CHANNTYPE &iChannType_,IDBInterFace *pDB_,CBHDateTime time_,const string& sSuffix_)
{
	E_TREND_TBL_TYPE iTblType = GE_TREND_TBLTYPE_MONTH;

	int i = GE_TREND_TBLTYPE_FAST;
	iTblType = (E_TREND_TBL_TYPE)i;
	pDB_->Cmd("select top 1 %s from %s%s where %s = '%s'",
		gc_cTime,GetTblNamePrefix(iChannType_,iTblType),sSuffix_.c_str(),
		gc_cTime,CPlantInfoBase::TimeConvertString(time_));
	if(pDB_->More())
	{
		iTblType = (E_TREND_TBL_TYPE)i;
		return iTblType;
	}

	for(i = GE_TREND_TBLTYPE_SECOND; i <= GE_TREND_TBLTYPE_MONTH; i++)
	{
		iTblType = (E_TREND_TBL_TYPE)i;
		pDB_->Cmd("select top 1 %s from %s%s where %s = '%s'",
			gc_cTime,GetTblNamePrefix(iChannType_,iTblType),sSuffix_.c_str(),
			gc_cTime,CPlantInfoBase::TimeConvertString(time_));
		if(pDB_->More())
		{
			iTblType = (E_TREND_TBL_TYPE)i;
			break;
		}
	}

	return iTblType;
}

CString CTransactData::GetTblNamePrefixForTrendNotOverWrite( const CHANNTYPE &iChannType_, 
															CBHDateTime timeStart_, 
															CBHDateTime timeEnd_, 
															E_TREND_TBL_TYPE& iTblType_, 
															bool & bSubTbl_, 
															bool &bSupTbl_ )
{
	/*	1、通过(结束时间-开始时间)判断可以在那个表中取数据；
	*/
	//判断可以从那级表中获得数据
	iTblType_=GetTrendTblTypeNotOverwrite(timeStart_,timeEnd_);//趋势表范围

	bSubTbl_ = false;

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
/*
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF))
		{
			bSupTblTmp=true;
		}*/

		break;
	case GE_TREND_TBLTYPE_HOUR:
		//20是通过小趋势表和天趋势表两者保存记录间隔差异得到
/*
		if (iMinutes>(D_TRENDRECORDS*TRENDRECORDS_COEF*20))
		{
			bSupTblTmp=true;
		}*/

		break;
	case GE_TREND_TBLTYPE_DAY:
		//系数3是因为天表中一小时保存3条记录；9是通过天趋势表（20分钟）和月趋势表（3小时）两者保存记录间隔差异得到
/*
		if ((3*iHours)>(D_TRENDRECORDS*TRENDRECORDS_COEF*9))
		{
			bSupTblTmp=true;
		}*/

		break;
	default:
		break;
	}
	//	CString strTmp;strTmp.Format("%d,%d,%d,%d",bSupTbl_,iTblType_,iMinutes,iHours);AfxMessageBox(strTmp);
	if (bSupTblTmp)
	{
		bSupTbl_=true;
		iTblType_=(E_TREND_TBL_TYPE)(iTblType_+1);
		if (iTblType_<GE_TREND_TBLTYPE_MONTH)
		{
			goto GetTblNamePrefixForTrend2_JUDGE_TBLTYPE;
		}
	}
	//	strTmp.Format("%d,%d",bSupTbl_,iTblType_);AfxMessageBox(strTmp);
	return GetTblNamePrefix(iChannType_,iTblType_);
}

E_TREND_TBL_TYPE CTransactData::GetTrendTblTypeNotOverwrite(	CBHDateTime & arefTimeStart,
																CBHDateTime & arefTimeEnd )
{
	COleDateTimeSpan span = arefTimeEnd-arefTimeStart;

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


CString CTransactData::GetTblNamePrefixForWaveNotOverWrite( const CHANNTYPE &iChannType_, 
															CBHDateTime timeStart_, 
															CBHDateTime timeEnd_, 
															E_TREND_TBL_TYPE& iTblType_ )
{
	iTblType_=GetTrendTblTypeNotOverwrite(timeStart_,timeEnd_);
	return GetTblNamePrefix( iChannType_,iTblType_);

}

bool CTransactData::GetTrendRecordsSearch( IDBInterFace *pDB,
									 string sTblNameSuffix_,
									 string sShowField_,
									 const CHANNTYPE &iChannType_,
									 E_TREND_TBL_TYPE &iTblType_,
									 CBHDateTime &timeStart_,
									 CBHDateTime timeEnd_,
									 bool & bNeedSubTbl_,
									 int anSearchMode)
{
	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd;

	strTbl.Format("%s%s",
						GetTblNamePrefixForTrend2(iChannType_, 
						timeStart_,
						timeEnd_,
						iTblType_,
						bNeedSubTbl_,
						bUseSupTbl),
						sTblNameSuffix_.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] between '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

	int iNums=0;

	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	int iRealTblType=GetTrendTblType(timeStart_);

	bool bRead=false;//是否可以读取记录
	//判断记录数目是否满足要求的趋势记录数目。如果不满足，就判断是否使用的趋势表类型不是起始时间的趋势表类型而是它的上一级。
	{
		int lnRet = (SEARCH_TREND&anSearchMode);

		if (lnRet)
		{
			E_TREND_TBL_TYPE lnE_TREND_TBL_TYPE = (E_TREND_TBL_TYPE)iTblType_;

			if(GE_TREND_TBLTYPE_UNKNOWN >=((E_TREND_TBL_TYPE)iTblType_)) 
			{
				iTblType_ = GE_TREND_TBLTYPE_SECOND;

			}else if(((E_TREND_TBL_TYPE)iTblType_)>GE_TREND_TBLTYPE_MONTH)
			{
				iTblType_ = GE_TREND_TBLTYPE_MONTH;
			}

			while((lnE_TREND_TBL_TYPE<=GE_TREND_TBLTYPE_MONTH))
			{
				strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,lnE_TREND_TBL_TYPE),sTblNameSuffix_.c_str());
				pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
				pDB->Cmd(" WHERE %s ",strWhere);

				if (pDB->More())
				{
					iNums=pDB->GetCount();
					bNeedSubTbl_ = false;
					return (iNums > 0);
				}
				else 
				{
					iNums=0;
					// 防止超时或其它异常，
					if(pDB->GetLastError() < 0)
					{

						bNeedSubTbl_ = false;
						return false;
					}
				}

				lnE_TREND_TBL_TYPE = (E_TREND_TBL_TYPE)(lnE_TREND_TBL_TYPE+1);
			}

			while((lnE_TREND_TBL_TYPE >=GE_TREND_TBLTYPE_SECOND))
			{
				strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,lnE_TREND_TBL_TYPE),sTblNameSuffix_.c_str());
				pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
				pDB->Cmd(" WHERE %s ",strWhere);

				if (pDB->More())
				{
					iNums=pDB->GetCount();
					bNeedSubTbl_ = false;
					return (iNums > 0);
				}
				else 
				{
					iNums=0;
					// 防止超时或其它异常，
					if(pDB->GetLastError() < 0)
					{

						bNeedSubTbl_ = false;
						return false;
					}
				}

				lnE_TREND_TBL_TYPE = (E_TREND_TBL_TYPE)(lnE_TREND_TBL_TYPE-1);
			}
			
		}

		lnRet = (SEARCH_ALARM&anSearchMode);
		
		if (lnRet)
		{
			strTbl.Format("%s%s",GetTblNamePrefixAlarmSud(iChannType_,SEARCH_ALARM),sTblNameSuffix_.c_str());

			pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
			pDB->Cmd(" WHERE %s ",strWhere);

			if (pDB->More())
			{
				iNums=pDB->GetCount();
				bNeedSubTbl_ = false;
				return (iNums > 0);
			}
			else 
			{
				iNums=0;
				// 防止超时或其它异常，
				if(pDB->GetLastError() < 0)
				{
					bNeedSubTbl_ = false;
				}
			}
		}

		lnRet = (SEARCH_SUD&anSearchMode);
		
		if(lnRet)
		{
			CString lstrTable = GetTblNamePrefixAlarmSud(iChannType_,SEARCH_SUD);

			if (lstrTable.GetLength()>0)
			{
				strTbl.Format("%s%s",GetTblNamePrefixAlarmSud(iChannType_,SEARCH_SUD),sTblNameSuffix_.c_str());

				pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
				pDB->Cmd(" WHERE %s ",strWhere);

				if (pDB->More())
				{
					iNums=pDB->GetCount();
					bNeedSubTbl_ = false;	
					return (iNums > 0);
				}
				else 
				{
					iNums=0;
					// 防止超时或其它异常，
					if(pDB->GetLastError() < 0)
					{
						bNeedSubTbl_ = false;
					}
				}
			}else
			{
				bNeedSubTbl_ = false;
			}

		}

		return false;
	}

	return bRead;
}

CString CTransactData::GetTblNamePrefixAlarmSud( const CHANNTYPE &iChannType_,GE_DATA_SEARCH_MODE  iTableMode )
{
	CString strSuffix="";

	switch(iTableMode)
	{
	case SEARCH_SUD:
		if (GetChannOneType( iChannType_)== E_TBL_CHANNTYPE_VIB)
		{
			strSuffix="SUD_";
			return strSuffix;
		}else
		{
			strSuffix = _T("");
			return strSuffix;
		}
		break;
	default:
		strSuffix="";		
	}

	switch(GetChannOneType( iChannType_))
	{
	case E_TBL_CHANNTYPE_VIB:
		strSuffix="ALARM_VIB_";
		break;
	case E_TBL_CHANNTYPE_DYN:
		strSuffix="ALARM_DYN_";
		break;
	case E_TBL_CHANNTYPE_PROC:
		strSuffix="ALARM_PROC_";
		break;
	default:
		strSuffix="ALARM_VIB_";
	}

	return strSuffix;
}

CString CTransactData::GetTblNamePrefixSUD( const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_ )
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
		switch(GetChannOneType( iChannType_))
		{
		case E_TBL_CHANNTYPE_VIB:
			strSuffix="SUD_";
			break;
		case E_TBL_CHANNTYPE_DYN:
			strSuffix="SUD_DYN";
			break;
		case E_TBL_CHANNTYPE_PROC:
			strSuffix="SUD_PROC";
			break;
		default:
			strSuffix="SUD_";
		}
		return strSuffix;
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

BOOL CTransactData::GetSpecWaveByTimeRange( IDBInterFace *pDB_,
										   const CString& sTableName_,
										   E_TREND_TBL_TYPE iTblType_,
										   CBHDateTime& timeSpec_,
										   const CString& sShowField_, 
										   int iSampNum_,
										   const CString& sChannNo_,
										   int iDataSource_)
{
	if(NULL == pDB_  || iDataSource_ == GE_DATA_SOURCE_WIRELESS || iSampNum_ == 0)
	{
		return TRUE;
	}

	COleDateTimeSpan timeSpan;

	string sSampleNumField = "SAMPLE_NUM_" + sChannNo_;

	CString lstrTblName = sTableName_;

	if(lstrTblName.MakeUpper().Find(_T("SECOND"))>=0)
	{
		iTblType_ = GE_TREND_TBLTYPE_SECOND;
	}
	else if(lstrTblName.MakeUpper().Find(_T("HOUR"))>=0)
	{
		iTblType_ = GE_TREND_TBLTYPE_HOUR;
	}
	else if(lstrTblName.MakeUpper().Find(_T("DAY"))>=0)
	{
		iTblType_ = GE_TREND_TBLTYPE_DAY;
	}
	else if (lstrTblName.MakeUpper().Find(_T("MONTH"))>=0)
	{
		iTblType_ = GE_TREND_TBLTYPE_MONTH;
	}
	else if(lstrTblName.MakeUpper().Find(_T("FAST"))>=0)
	{
		iTblType_ = GE_TREND_TBLTYPE_FAST;
	}
	else
	{
		return TRUE;
	}

	CString strWave;

	strWave.Format("WAVE_%s",sChannNo_);

	pDB_->Cmd("SELECT %s,ISNULL(DATALENGTH(%s),0) as WavePoint FROM %s WHERE %s = '%s'",\
		sShowField_,strWave,sTableName_,gc_cTime,timeSpec_.Format());
	if(pDB_->More())
	{
		if(pDB_->GetLong("WavePoint") > 1)
		{
			return TRUE;
		}
	}

	CBHDateTime dtStart = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime dtEnd = CBHDateTime::GetCurrentTimeEx();
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_MONTH:
		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 10 %s from %s where %s <= '%s' order by %s desc) order by %s",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtStart = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 10 %s from %s where %s >= '%s' order by %s asc) order by %s desc",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtEnd = pDB_->GetTime(gc_cTime);
		}
		break;
	case GE_TREND_TBLTYPE_DAY:
		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 20 %s from %s where %s <= '%s' order by %s desc) order by %s",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtStart = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 20 %s from %s where %s >= '%s' order by %s asc) order by %s desc",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtEnd = pDB_->GetTime(gc_cTime);
		}
		break;
	case GE_TREND_TBLTYPE_HOUR:
		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 50 %s from %s where %s <= '%s' order by %s desc) order by %s",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtStart = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 50 %s from %s where %s >= '%s' order by %s asc) order by %s desc",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtEnd = pDB_->GetTime(gc_cTime);
		}
		break;
	case GE_TREND_TBLTYPE_SECOND:
		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 150 %s from %s where %s <= '%s' order by %s desc) order by %s",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtStart = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("select top 1 %s from %s where %s in\
				  (select top 150 %s from %s where %s >= '%s' order by %s asc) order by %s desc",\
				  gc_cTime,sTableName_,gc_cTime,gc_cTime,sTableName_,gc_cTime,timeSpec_.Format(),gc_cTime,gc_cTime);
		if(pDB_->More())
		{
			dtEnd = pDB_->GetTime(gc_cTime);
		}
		break;
	case GE_TREND_TBLTYPE_FAST:
		timeSpan = COleDateTimeSpan(0,0,10,0);
		dtStart = timeSpec_ - timeSpan;
		dtEnd = timeSpec_ + timeSpan;
		break;
	default:
		return TRUE;

	}
	
	pDB_->Cmd("SELECT TOP 1 %s,%s FROM %s WHERE %s BETWEEN '%s' and '%s' and DATALENGTH(%s) > 0 ORDER BY %s DESC",\
		gc_cTime,sShowField_,sTableName_,gc_cTime,dtStart.Format(gc_cTimeFormat),\
		timeSpec_.Format(gc_cTimeFormat),strWave,gc_cTime);
	if(pDB_->More())
	{
		timeSpec_ = pDB_->GetTime(gc_cTime);
		return TRUE;
	}
	else
	{
		pDB_->Cmd("SELECT TOP 1 %s,%s FROM %s WHERE %s BETWEEN '%s' and '%s' and DATALENGTH(%s) > 0 ORDER BY %s",\
			gc_cTime,sShowField_,sTableName_,gc_cTime,timeSpec_.Format(gc_cTimeFormat),\
			dtEnd.Format(gc_cTimeFormat),strWave,gc_cTime);
		if(pDB_->More())
		{
			timeSpec_ = pDB_->GetTime(gc_cTime);
			return TRUE;
		}
	}
	return FALSE;
}

vector<CBHDateTime> CTransactData::GetTrendFastRecordTimeArray( IDBInterFace *pDB,
												   string sTblNameSuffix_,
												   const CHANNTYPE &iChannType_,
												   E_TREND_TBL_TYPE& iTblType_,
												   CBHDateTime &timeStart_,
												   CBHDateTime & timeEnd_ )
{
	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd,strTimeArray;

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);

	vector<CBHDateTime> lvDateTime;

	bool bGetDataBySpecTime = false;

	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());

	int lnRecordCount = 0;

	pDB->Cmd("select count(%s) as RecordCount from %s where %s between '%s' and '%s'",\
		gc_cID,strTbl,gc_cTime,strTimeStart,strTimeEnd);
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}

	if(lnRecordCount > 0)
	{
		int lnInterval = lnRecordCount/D_TRENDRECORDS;

		if(lnInterval > 0)
		{
			int lnID = 0;

			pDB->Cmd("select top 1 %s from %s where %s between '%s' and '%s' order by %s",\
				gc_cID,strTbl,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
			if(pDB->More())
			{
				lnID = pDB->GetLong(gc_cID);

			}
			pDB->Cmd("select [%s] from %s where (%s between '%s' and '%s') and (%s-(%d))%%%d = 0",\
				gc_cTime,strTbl,gc_cTime,strTimeStart,strTimeEnd,gc_cID,lnID,lnInterval,gc_cTime);
			while(pDB->More())
			{
				lvDateTime.push_back(pDB->GetTime(gc_cTime));
			}
		}
		else
		{
			pDB->Cmd("select [%s] from %s where %s between '%s' and '%s'",\
				gc_cTime,strTbl,gc_cTime,strTimeStart,strTimeEnd);
			while(pDB->More())
			{
				lvDateTime.push_back(pDB->GetTime(gc_cTime));
			}
		}
	}

	return lvDateTime;

	
}

CString CTransactData::GetTimeStringByTimeArray( const vector<CBHDateTime>& vDateTime_ )
{
	CString lstrTimeArray = "";

	if(vDateTime_.size() > 0)
	{
		lstrTimeArray = "'" + vDateTime_[0].Format(gc_cTimeFormat) + "'";
		for(unsigned int i = 1; i < vDateTime_.size(); i++)
		{
			lstrTimeArray = lstrTimeArray + ",'" + vDateTime_[i].Format(gc_cTimeFormat)+"'";
		}
	}

	return lstrTimeArray;
}

bool CTransactData::GetTrendRecords(IDBInterFace *pDB,
									string sTblNameSuffix_,
									string sShowField_,
									const CHANNTYPE &iChannType_,
									E_TREND_TBL_TYPE &iTblType_,
									CBHDateTime &timeStart_,
									CBHDateTime timeEnd_,
									bool & bNeedSubTbl_)
{
	int lnSearchMode = IsEnableSearchAllTable();
	if (lnSearchMode)
	{
		int lnRet =this->GetTrendRecordsSearch(pDB,
											sTblNameSuffix_,
											sShowField_,
											iChannType_,
											iTblType_,
											timeStart_,
											timeEnd_,
											bNeedSubTbl_,
											lnSearchMode);
		return lnRet;

	}

	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd;

	strTbl.Format("%s%s",
				  GetTblNamePrefixForTrend2(iChannType_, 
											timeStart_,
											timeEnd_,
											iTblType_,
											bNeedSubTbl_,
											bUseSupTbl),
				  sTblNameSuffix_.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] between '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

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


			if (pDB->More())
			{
				iNums=pDB->GetCount();
			}
			else 
			{
				iNums=0;
				// 防止超时或其它异常，
				if(pDB->GetLastError() < 0)
				{

					bNeedSubTbl_ = false;
					return false;
				}
			}

			// 为了提高效率，暂时取消跨表查询
			bNeedSubTbl_ = false;
			return (iNums > 0);
			
#if 1 /* 由于取消跨表查询，以下代码暂时禁用 */
		}
	}
#else /* 由于取消跨表查询，以下代码暂时禁用 */		

			
/*
#ifdef _DEBUG
		//	CString strTmp;strTmp.Format("GetTrendRecords1 %d,%d,%d",iNums,iTblType_,iRealTblType);AfxMessageBox(strTmp);
#endif
			if (iNums<(TRENDRECORDS_COEF*D_TRENDRECORDS))
			{
				iTblType_=(E_TREND_TBL_TYPE)(iTblType_-1);//得到当前起始时间应该的趋势表类型
				goto REALTBLETYPE_READRECORDS;
			}
			else
			{
				bNeedSubTbl_=false;
				return true;
			}
		}
		if (iNums>0)
			bRead=true;
		else
			return false;
	}

	E_TREND_TBL_TYPE iTblTypeEnd;
	iTblType_=GetTrendTblType(timeStart_);
	iTblTypeEnd=GetTrendTblType(timeEnd_);
	bNeedSubTbl_= iTblType_>iTblTypeEnd? true:false;
	//如果当前数据连接有记录数目，并且有下一级趋势表，则就判断是否需要从当前趋势表类型中读取数据，并重新设置起始时间
	if (bNeedSubTbl_&&bRead)
	{//如果记录数目不满足，就不从下一级表中取数据了；否则判断做如下判断
		bool bFlag=false;//是否整个时间段取数据
		//下一级表数据的截止时间到查询数据要求的起始时间，在当前趋势类型的表中是否存在数据，如果不存在，就把直接从这个表中读取数据
		CBHDateTime timeTmp=CBHDateTime::GetCurrentTime();
MUTITABLE_QUERY:
		if (iTblType_>iTblTypeEnd)
		{
			if (bFlag)
				timeTmp=timeEnd_;
			else
				timeTmp=GetSpecTrendTimeOff((E_TREND_TBL_TYPE)(iTblType_-1));
			strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());
			pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
			pDB->Cmd(" WHERE [%s] between '%s' and '%s' ORDER BY [%s] ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeTmp),gc_cTime);
#ifdef _DEBUG
		//	AfxMessageBox(pDB->GetSql());
#endif
			if(pDB->More())
				iNums=pDB->GetCount();
			else
				iNums=0;
			if(!bFlag)
				timeStart_=timeTmp;
#ifdef _DEBUG
		//	CString strTmp;strTmp.Format("hh %d,%d",iNums,iTblType_);AfxMessageBox(strTmp);
#endif
			if (iNums==0)
			{
				iTblType_=(E_TREND_TBL_TYPE)(iTblType_-1);
				if (iTblType_>iTblTypeEnd)
				{
					bFlag=true;
					goto MUTITABLE_QUERY;
				}
				else
					bRead=false;
			}
			else
			{
				bRead=true;
				if (bFlag)
				{
					if (iNums>(TRENDRECORDS_COEF*D_TRENDRECORDS))
						bNeedSubTbl_=false;
					else
					{
						if (iTblType_>iTblTypeEnd)
						{
							bFlag=false;
							goto MUTITABLE_QUERY;
						}
						else
							bNeedSubTbl_=false;
					}
				}
			}
		}
	}
	else
		bNeedSubTbl_=false;
	return bRead;
*/
#endif /* 由于取消跨表查询，以下代码暂时禁用 */
	return bRead;
}

bool CTransactData::GetTrendRecordsEx(IDBInterFace *pDB,
									string sTblNameSuffix_,
									string sShowField_,
									const CHANNTYPE &iChannType_,
									E_TREND_TBL_TYPE &iTblType_,
									CBHDateTime &timeStart_,
									CBHDateTime timeEnd_,
									bool & bNeedSubTbl_,
									int iInterval_)
{
	bool bUseSupTbl=false;//是否使用了上一级表单
	CString strTbl,strWhere,strTimeStart,strTimeEnd,strFastTbl;

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);

	vector<CBHDateTime> lvDatetimeRegularTmp,lvDatetimeFastTmp;

	vector<CBHDateTime> lvDatetimeRegular,lvDatetimeFast;

	set<CBHDateTime> lsDataTime;
	
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	E_TREND_TBL_TYPE lnTbleType = GE_TREND_TBLTYPE_FAST;

	strFastTbl.Format("%s%s",GetTblNamePrefix(iChannType_,lnTbleType),sTblNameSuffix_.c_str());

	lvDatetimeFastTmp = GetTrendFastRecordTimeArray(pDB,sTblNameSuffix_,iChannType_,lnTbleType,timeStart_,timeEnd_);

	if(iInterval_ > 0)
	{
		//将所有时间段内数据减去第一条记录ID，保证能取到第一条记录
		int nID = 0;
		strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());
		pDB->Cmd("select top 1 [%s] from %s where %s between '%s' and '%s' order by [%s]",\
			gc_cID,strTbl,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
		if(pDB->More())
		{
			nID = pDB->GetLong(gc_cID);
		}

		pDB->Cmd("select [%s] from %s where (%s between '%s' and '%s') and (%s-(%d))%%%d = 0  ORDER BY [%s]",\
			gc_cTime,strTbl,gc_cTime,strTimeStart,strTimeEnd,gc_cID,nID,iInterval_,gc_cTime);
		while(pDB->More())
		{
			lvDatetimeRegularTmp.push_back(pDB->GetTime(gc_cTime));
		}
	}
	else
	{
		strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());
		pDB->Cmd("select [%s] from %s where (%s between '%s' and '%s') ORDER BY [%s]",\
			gc_cTime,strTbl,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
		while(pDB->More())
		{
			lvDatetimeRegularTmp.push_back(pDB->GetTime(gc_cTime));
		}
	}
	
	for(size_t nloop = 0; nloop < lvDatetimeRegularTmp.size(); nloop++)
	{
		if(lsDataTime.insert(lvDatetimeRegularTmp[nloop]).second)
		{
			lvDatetimeRegular.push_back(lvDatetimeRegularTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvDatetimeFastTmp.size(); nloop++)
	{	
		if(lsDataTime.insert(lvDatetimeFastTmp[nloop]).second)
		{
			lvDatetimeFast.push_back(lvDatetimeFastTmp[nloop]);
		}
	}

	CString lstrRegularTimeArray = GetTimeStringByTimeArray(lvDatetimeRegular);

	CString lstrFastTimeArray = GetTimeStringByTimeArray(lvDatetimeFast);

	int iNums=0;
	
	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	int iRealTblType=GE_TREND_TBLTYPE_UNKNOWN;

	bool bRead=false;//是否可以读取记录
	//判断记录数目是否满足要求的趋势记录数目。如果不满足，就判断是否使用的趋势表类型不是起始时间的趋势表类型而是它的上一级。
	{
REALTBLETYPE_READRECORDS:
		if (!(iTblType_<iRealTblType))
		{
			strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTblType_),sTblNameSuffix_.c_str());

			
			if(lstrRegularTimeArray.Trim().GetLength()>0)
			{
				pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
				pDB->Cmd(" WHERE %s IN(%s)",gc_cTime,lstrRegularTimeArray);

			}
			else
			{
				pDB->Cmd("SELECT %s FROM [%s]",sShowField_.c_str(),strTbl);
				pDB->Cmd(" WHERE %s between '%s' and '%s'",gc_cTime,strTimeStart,strTimeEnd);
			}
			
			if(lstrFastTimeArray.Trim().GetLength()>0)
			{
				CString lstrShowField = sShowField_.c_str();
				if(lstrShowField.MakeUpper().Find("CUSTOM_PARAM")>=0)
				{
					pDB->Cmd(" UNION ALL(SELECT %s FROM %s WHERE %s IN(%s))",sShowField_.c_str(),strFastTbl,gc_cTime,lstrFastTimeArray);
				}
				else
				{
					pDB->Cmd(" UNION(SELECT %s FROM %s WHERE %s IN(%s))",sShowField_.c_str(),strFastTbl,gc_cTime,lstrFastTimeArray);
				}
				
			}
			else
			{
				CString lstrShowField = sShowField_.c_str();
				if(lstrShowField.MakeUpper().Find("CUSTOM_PARAM")>=0)
				{
					pDB->Cmd(" UNION ALL(SELECT %s FROM %s WHERE %s between '%s' and '%s')",sShowField_.c_str(),strFastTbl,gc_cTime,strTimeStart,strTimeEnd);
				}
				else
				{
					pDB->Cmd(" UNION(SELECT %s FROM %s WHERE %s between '%s' and '%s')",sShowField_.c_str(),strFastTbl,gc_cTime,strTimeStart,strTimeEnd);
				}
			}

			pDB->Cmd(" ORDER BY %s",gc_cTime);

			if (pDB->More())
			{
				iNums=pDB->GetCount();
			}
			else 
			{
				iNums=0;
				// 防止超时或其它异常，
				if(pDB->GetLastError() < 0)
				{

					bNeedSubTbl_ = false;
					return false;
				}
			}

			// 为了提高效率，暂时取消跨表查询
			bNeedSubTbl_ = false;
			return (iNums > 0);
			
		}
	}
	return bRead;
}

int CTransactData::GetTrendRecordsNum(IDBInterFace *pDB,
									  string sTblNameSuffix_,
									  string sShowField_,
									  const CHANNTYPE &iChannType_,
									  E_TREND_TBL_TYPE &iTblType_,
									  CBHDateTime &timeStart_,
									  CBHDateTime timeEnd_,
									  bool & bNeedSubTbl_)
{
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	int lnSearchMode = IsEnableSearchAllTable();

	if (lnSearchMode)
	{
		return 0;
	}

	// 为了提高效率，暂时取消跨表查询
	bNeedSubTbl_ = false;

	int nMaxCount = 0;

	CString strTbl,strWhere,strTimeStart,strTimeEnd;

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] between '%s' AND '%s'",gc_cTime,strTimeStart,strTimeEnd);

	for(int i = GE_TREND_TBLTYPE_MONTH; i >= GE_TREND_TBLTYPE_SECOND ; i--)
	{
		E_TREND_TBL_TYPE iTbType = (E_TREND_TBL_TYPE)i;

		strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTbType),sTblNameSuffix_.c_str());

		pDB->Cmd("SELECT count(%s) as RecordCount FROM [%s]",gc_cID,strTbl);
		pDB->Cmd(" WHERE %s ",strWhere);

		int count = 0;

		if(pDB->More())
		{
			count = pDB->GetLong("RecordCount");
		}

		if (count > D_TRENDRECORDS)
		{
			iTblType_ = (E_TREND_TBL_TYPE)i;

			return count;
		}

		if(nMaxCount < count)
		{
			nMaxCount = count;

			iTblType_ = (E_TREND_TBL_TYPE)i;
		}
	}

    if(nMaxCount <= 0)
	{
		E_TREND_TBL_TYPE iTbType = GE_TREND_TBLTYPE_FAST;

		strTbl.Format("%s%s",GetTblNamePrefix(iChannType_,iTbType),sTblNameSuffix_.c_str());

		pDB->Cmd("SELECT top 1 %s FROM [%s]",gc_cID,strTbl);
		pDB->Cmd(" WHERE %s ORDER BY %s",strWhere,gc_cTime);
		if(pDB->More())
		{
			nMaxCount = 1;
		}
	}
	return nMaxCount;
}

CBHDateTime CTransactData::GetSpecTrendTimeOff(E_TREND_TBL_TYPE iTblType_)
{
	CBHDateTime timeTmp=CBHDateTime::GetCurrentTime();
	COleDateTimeSpan span;
	switch(iTblType_)
	{
	case GE_TREND_TBLTYPE_DAY:
		span.SetDateTimeSpan(1080,0,0,0);
		break;
	case GE_TREND_TBLTYPE_HOUR:
		span.SetDateTimeSpan(90,0,0,0);
		break;
	case GE_TREND_TBLTYPE_SECOND:
	default:
		span.SetDateTimeSpan(7,0,0,0);
	}
	timeTmp=CBHDateTime::GetCurrentTime()-span;
	return timeTmp;
}

int CTransactData::ReadVibChannSpecEventIDAlarmData(ICommInterFace *pFixComm_,
													IDBInterFace *pDB_,
													string sCompany_,
													string sFactory_,
													string sPlant_,
													lCHANNID_NO lChann_,
													int iEventID_)
{
	int nCount=-1;
	string sSuffix;
	if(CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm_,false)==NULL)
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	list<string> lChannField;
	list<string>::iterator it;
	lCHANNID_NO::iterator itChann;
	string sTmp;
	for (itChann=lChann_.begin();itChann!=lChann_.end();++itChann)
	{
		if (itChann->second.c_str()=="")
			continue;
		sTmp=ConsVibChannField(itChann->second);
		sTmp.append(",TYPE_");
		sTmp.append(itChann->second);
		lChannField.push_back(sTmp);
	}
	sTmp.append(",");
	sTmp.append(ConsCommonField().GetString());
	int iChannNum=lChann_.size();
	S_VibCharValue *pfVibValue=new S_VibCharValue[iChannNum];
	int *piType=new int[iChannNum];
	string sShowField="";
	for (it=lChannField.begin();it!=lChannField.end();++it)//构建要查询的字段
	{
		sShowField.append(*it);
		sShowField.append(",");
	}
	sShowField.append(gc_cTime);
	sShowField.append(",");
	sShowField.append(gc_cMicroSecond);
	int nloop=0;
	string sTable="ALARM_VIB_";
	sTable.append(sSuffix);
	pDB_->Cmd("SELECT %s FROM [%s] ",sShowField.c_str(),sTable.c_str());
	pDB_->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID_);
	while (pDB_->More())
	{
		pFixComm_->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum);
		pFixComm_->SetItem(FIELD_SERVICE_OFF_TIME,pDB_->GetTime(gc_cTime));
		pFixComm_->SetItem(FIELD_SERVICE_MICROSECOND,pDB_->GetLong(gc_cMicroSecond));
		nloop=0;
		for (itChann=lChann_.begin();itChann!=lChann_.end();++itChann)
		{
			if (itChann->second.c_str() =="")
				continue;
			sTmp=ConsVibChannSpecTypeField(itChann->second,-2);//报警类型字段
			piType[nloop]=pDB_->GetLong(sTmp.c_str());
			pfVibValue[nloop]=ReadVibCharValue(itChann->second,pDB_);
			nloop++;
		}
		pFixComm_->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char *)pfVibValue,sizeof(S_VibCharValue)*iChannNum);
		pFixComm_->SetItemBuf(FIELD_SERVICE_TYPE_BUFF,(char *)piType,sizeof(int)*iChannNum);
		pFixComm_->Write(pFixComm_->GetAci());
		pFixComm_->CreateNextAnswer();
		++nCount;
	}
	lChannField.clear();
	delete [] pfVibValue;
	delete [] piType;
	return nCount;
}

int CTransactData::SpectrumAnalysis(CPlantInfoOffLine * pPlant_,DATATYPE_WAVE *pWave_,int iLen_,DATATYPE_WAVE *pSpectrum_,int &iLines_,int iSensorType_,int iSmpFreq_,float &fDf_,float* pfWave_,float * pfSpectrum_,CWaveHead& sWaveHead_)
{
	int iNums = iLines_*1.28 +10;
	iNums = iNums > iLen_? iNums : iLen_;
	//float *pfSpectrum=new float[(iLines_*1.28)+10];
	/*2007.4.11 因为要修改为细化分析，所以注释掉
	int iPtNums=iLines_*2.56;
	if (iPtNums>iLen_)
	iPtNums=iLen_;
	iLines_=iPtNums/2.56+1;
	ConvertDTWaveToFloat(pfWave,pWave_,iPtNums);
	SpectralMeasureMent(pfWave,iPtNums,pfSpectrum);
	//       writefile(pffreq,iquest_wavenum/2);
	if (iSensorType_==GE_SENSORTYPE_VOTEX)
	{
	for (int nloop=0;nloop<iLines_;++nloop)
	pfSpectrum[nloop]=pfSpectrum[nloop]*2.0;//江老师要求添加的系数
	}
	else
	{
	for (int nloop=0;nloop<iLines_;++nloop)
	pfSpectrum[nloop]=pfSpectrum[nloop]*1.414;//江老师要求添加的系数
	}
	ConvertFloatToDTWave(pSpectrum_,pfSpectrum,iLines_);
	*/

	{//细化谱和正常谱分析 2007.4.7添加
		pPlant_->ConvertDTWaveToFloat(pfWave_,pWave_,iLen_,sWaveHead_);

		pPlant_->ConvertFloatToDTWave(pWave_,pfWave_,iLen_,sWaveHead_,1);

		float fStartFreq=0;
		int iRtn=pPlant_->ZoomSpectrumAnalysis(pfWave_,iLen_,iSmpFreq_,pfSpectrum_,iLines_,fDf_,fStartFreq);
		if (iRtn<0)
		{//错误
			iLines_=0;
		}
		else
		{
			if (iRtn==2)
			{
				if (GE_SENSORTYPE_VOTEX==iSensorType_)
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,2);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*2.0;
					}
				}
				else
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,1);
			}
			else
			{
				if (GE_SENSORTYPE_VOTEX==iSensorType_)
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,2.818);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*2.818;
					}
				}
				else
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,1.414);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*1.414;
					}
				}
			}
		}
	}

	return iLen_;
}

int CTransactData::SpectrumAnalysis(CPlantInfoOffLine * pPlant_,DATATYPE_WAVE *pWave_,int iLen_,DATATYPE_WAVE *pSpectrum_,int &iLines_,int iSensorType_,int iSmpFreq_,float &fDf_, float &fStartFreq_, float fStopFreq_,float* pfWave_,float * pfSpectrum_,CWaveHead& sWaveHead_)
{
	int iNums = iLines_*1.28 +10;
	iNums = iNums > iLen_? iNums : iLen_;

	{//细化谱和正常谱分析 2007.4.7添加
		pPlant_->ConvertDTWaveToFloat(pfWave_,pWave_,iLen_,sWaveHead_);

		pPlant_->ConvertFloatToDTWave(pWave_,pfWave_,iLen_,sWaveHead_,1);
		int iRtn=pPlant_->ZoomSpectrumAnalysis(pfWave_,iLen_,iSmpFreq_,pfSpectrum_,iLines_,fDf_,fStartFreq_, fStopFreq_);
		if (iRtn<0)
		{//错误
			iLines_=0;
		}
		else
		{
			if (iRtn==2)
			{
				if (GE_SENSORTYPE_VOTEX==iSensorType_)
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,2);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*2.0;
					}
				}
				else
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,1);
				}
				
			}
			else
			{
				if (GE_SENSORTYPE_VOTEX==iSensorType_)
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,2.818);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*2.818;
					}
				}
				else
				{
					pPlant_->ConvertFloatToDTWave(pSpectrum_,pfSpectrum_,iLines_,sWaveHead_,1.414);
					for(int i = 0; i < iLines_; i++)
					{
						pfSpectrum_[i] = pfSpectrum_[i]*1.414;
					}
				}
			}
		}
	}
	return iLen_;
}

/**得到指定公司、分厂、机组、测点和报警事件ID的轴心位置趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:           FIELD_SERVICE_OFF_REV
*          水平方向值      FIELD_SERVICE_AXESLOCATION_HOR
*          水平方向直流量  FIELD_SERVICE_TREND_DC
*          水平方向间隙电压 FIELD_SERVICE_TREND_GAP
*          垂直方向值      FIELD_SERVICE_AXESLOCATION_VER
*          垂直方向直流量  FIELD_SERVICE_TREND_DC2
*          垂直方向间隙电压 FIELD_SERVICE_TREND_GAP2
*          报警类型         FIELD_SYSTEM_TYPE
*          时间         FIELD_SERVICE_OFF_TIME
*          报警标示     FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND       21025
*/
int CTransactData::GetAxesLocationAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	//如果设备ID和测点1ID有一个为空就不能读轴心位置历史趋势
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	string sChannIDHor = pFixComm->Get(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->Get(FIELD_SERVICE_CHANNID_VER);
	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	int nCount=-1;
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	//得到报警触发的时间
	string sTable;
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int  iType=0;//报警点标示
	bool bHavedMark=false;

	if (sLocation.empty())
	{
		if ((sChannIDHor.empty())||(sChannIDVer.empty()))
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		S_ChannInfo channInfoHor;
		S_ChannInfo channInfoVer;
		string sChannNoVer;
		string sChannNoHor;

		channInfoVer = pPlant->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlant->GetSpecVibChannInfo( sChannIDHor );
		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cLocation,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",
			gc_cCompany,sCompany.c_str(),
			gc_cFactory_Name,sFactory.c_str(),
			gc_cPlantNo,pPlant->GetPlantNo().c_str(),
			gc_cVerChann,sChannNoVer.c_str());
		if (pDB->More())
			sLocation=pDB->Get(gc_cLocation);
		else
		{
			nCount=-2;
			goto QUIT_GETAXESLOCATIONTREND;
		}
	}
	//得到报警触发的时间
	sTable="ALARM_LOG_";
	sTable.append(sSuffix);
	SwitchDbNameEx(pDB,sSuffix.c_str());
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cStart_T,sTable.c_str());
	pDB->Cmd("WHERE [%s]=%d",gc_cEventID,iEventID);
	if (pDB->More())
		dtStart=pDB->GetTime(gc_cStart_T);
	else
	{
		nCount=-3;
		goto QUIT_GETAXESLOCATIONTREND;
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	sTable="ALARM_AXIS_";
	sTable.append(sSuffix);
	pDB->Cmd("SELECT * FROM [%s] ",sTable.c_str());
	pDB->Cmd("WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cName,sLocation.c_str());
	while (pDB->More())
	{
		dtData=pDB->GetTime(gc_cTime);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtData);
		iType=pDB->GetLong(gc_cType);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iType);
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,pDB->GetDouble(gc_cCoor_X));
		pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,pDB->GetDouble(gc_cCoor_Y));
		pFixComm->SetItem(FIELD_SERVICE_TREND_DC,pDB->GetDouble(gc_cDC_X));
		pFixComm->SetItem(FIELD_SERVICE_TREND_DC2,pDB->GetDouble(gc_cDC_Y));
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,pDB->GetDouble(gc_cGap_X));
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP2,pDB->GetDouble(gc_cGap_Y));
		if (bHavedMark)
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		else
		{
			if ((dtData==dtStart)||(iType&D_BHKD_AT_LOCATION))
			{
				bHavedMark=true;
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
			}
			else
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		}
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}
QUIT_GETAXESLOCATIONTREND:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定报警事件的报警数据保存的开始截止时间
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
#define SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE             21026
*/
int CTransactData::GetAlarmDataTimeRange(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	//如果设备ID和测点1ID有一个为空就不能读轴心位置历史趋势
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm)==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	string sTable="ALARM_LOG_";
	sTable.append(sSuffix);
	CHANNTYPE iChannType= GE_ALLPROC;
	int iRtn=-1;
	//得到报警事件的通道类型
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cChannType,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		iChannType = (CHANNTYPE)(pDB->GetLong(gc_cChannType));
		E_TBL_CHANNTYPE channTblType = GetChannOneType( iChannType );
		//if ((iChannType==GE_VIBCHANN)||(iChannType==GE_AXIALCHANN))
		if ( channTblType == E_TBL_CHANNTYPE_VIB)
		{
			if ( iChannType == GE_AXISLOCATIONCHANN)
				sTable = "ALARM_AXIS_";
			else
				sTable = "ALARM_VIB_";
		}
		else if( channTblType == E_TBL_CHANNTYPE_PROC)
			sTable = "ALARM_PROC_";
		else
			sTable = "ALARM_DYN_";
		sTable.append(sSuffix);
		pDB->Cmd("SELECT TOP 1 [%s] FROM [%s] ",gc_cTime,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d",gc_cEventID,iEventID);
		pDB->Cmd(" ORDER BY [%s] ASC",gc_cTime);
		if (pDB->More())
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,pDB->GetTime(gc_cTime));
			pDB->Cmd("SELECT TOP 1 [%s] FROM [%s] ",gc_cTime,sTable.c_str());
			pDB->Cmd(" WHERE [%s]=%d",gc_cEventID,iEventID);
			pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
			if (pDB->More())
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cTime));
			else
				iRtn=-5;
		}
		else
			iRtn=-4;
	}
	else
		iRtn=-3;
	pFixComm->Write(pFixComm->GetAci());
	return iRtn;
}

/**得到指定轴心位置报警事件、时间的报警数据
*          公司ID   FIELD_SYSTEM_COMPANY
*		    分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          时间                 FIELD_SERVICE_OFF_TIME
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:         FIELD_SERVICE_OFF_REV
*          水平方向值      FIELD_SERVICE_AXESLOCATION_HOR
*          水平方向直流量  FIELD_SERVICE_TREND_DC
*          水平方向间隙电压 FIELD_SERVICE_TREND_GAP
*          垂直方向值      FIELD_SERVICE_AXESLOCATION_VER
*          垂直方向直流量  FIELD_SERVICE_TREND_DC2
*          垂直方向间隙电压 FIELD_SERVICE_TREND_GAP2
*          报警点类型      FIELD_SYSTEM_TYPE
#define SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA            21027
*/
int CTransactData::GetAxesLocationAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	//如果设备ID和测点1ID有一个为空就不能读轴心位置历史趋势
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	string sChannIDHor = pFixComm->Get(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->Get(FIELD_SERVICE_CHANNID_VER);
	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	int nCount=-1;

	if ((sLocation.empty()))
	{
		if ((sChannIDHor.empty())||(sChannIDVer.empty()))
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		S_ChannInfo channInfoHor;
		S_ChannInfo channInfoVer;
		string sChannNoVer;
		string sChannNoHor;

		channInfoVer = pPlant->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlant->GetSpecVibChannInfo( sChannIDHor );
		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cLocation,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",
			gc_cCompany,sCompany.c_str(),
			gc_cFactory_Name,sFactory.c_str(),
			gc_cPlantNo,pPlant->GetPlantNo().c_str(),
			gc_cVerChann,sChannNoVer.c_str());
		if (pDB->More())
			sLocation=pDB->Get(gc_cLocation);
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}

	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	string sTable="ALARM_AXIS_";
	sTable.append(sSuffix);
	SwitchDbNameEx(pDB,sSuffix.c_str());
	pDB->Cmd("SELECT * FROM [%s] ",sTable.c_str());
	pDB->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]='%s'",gc_cEventID,iEventID,gc_cName,sLocation.c_str(),gc_cTime, CPlantInfoBase::TimeConvertString(dt));
	pDB->Cmd(" ORDER BY [%s]",gc_cTime);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,pDB->GetDouble(gc_cCoor_X));
		pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,pDB->GetDouble(gc_cCoor_Y));
		pFixComm->SetItem(FIELD_SERVICE_TREND_DC,pDB->GetDouble(gc_cDC_X));
		pFixComm->SetItem(FIELD_SERVICE_TREND_DC2,pDB->GetDouble(gc_cDC_Y));
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,pDB->GetDouble(gc_cGap_X));
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP2,pDB->GetDouble(gc_cGap_Y));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));
		nCount=1;
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}
/**得到指定公司、分厂、机组、振动测点、报警事件ID的振动详细数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_CHARVALUE_S
*          测点门限Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          门限个数            FIELD_SERVICE_LEN
*          时间值              FIELD_SERVICE_OFF_TIME
*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
*          事件类型            FIELD_SYSTEM_EVENT_TYPE
*          报警类型            FIELD_SYSTEM_TYPE
*          报警点标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND          21028
*/
int CTransactData::GetVibAlarmInfoTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	S_ChannInfo channInfo;
	S_VibCharValue vibValue;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEvent=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int nCount=-1,index=0,index1=0;
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	int iType=0,iAlarmType=0,ilen,iRecords;
	bool bHaveFinish=false;
	string sTable,sShowField,sRev,sType;
	typedef pair<CBHDateTime,int> DTTYPE;
	vector<DTTYPE> vDtType;
	vector<int> vThID;
	vector<int>::iterator itI;
	//阈值信息
	VP_TH vpTh;
	channInfo=pPlant->GetSpecVibChannInfo(sChann);
	if (channInfo._iRevType==2)
		sRev=gc_cRev2;
	else
		sRev=gc_cRev;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//得到报警事件详细
	sTable="AlARM_LOG_";
	sTable.append(sSuffix);
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cType,gc_cStart_T,gc_cThreshold_ID,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s' ",gc_cEventID,iEvent,gc_cName,sChann.c_str());
	pDB->Cmd(" ORDER BY [%s]  ",gc_cStart_T);
	while(pDB->More())
	{
		dtStart=pDB->GetTime(gc_cStart_T);
		iType=pDB->GetLong(gc_cType);
		vDtType.push_back(make_pair(dtStart,iType));
		vThID.push_back(pDB->GetLong(gc_cThreshold_ID));
	}
	iRecords=vDtType.size();
	if (iRecords<1)
	{
		nCount=-2;
		goto QUIT_GETVIBALARMINFOTREND;
	}
	vpTh=GetSpecIDThreshold(vThID,pDB,sSuffix);
	//得到报警数据
	sType=ConsVibChannSpecTypeField(channInfo._cChannNo,-2);
	sShowField=ConsVibChannField(channInfo._cChannNo);
	sShowField.append(",");
	sShowField.append(sType);
	sShowField.append(",");
	sShowField.append(sRev);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	sShowField.append(",");
	sShowField.append(gc_cMicroSecond);
	sShowField.append(",");
	sShowField.append(ConsCommonField().GetString());
	sTable="AlARM_VIB_";
	sTable.append(sSuffix);
	pDB->Cmd("Select %s from [%s] ",sShowField.c_str(),sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEvent);
	pDB->Cmd(" ORDER BY [%s]",gc_cTime);
	index=0;
	index1=0;
	bHaveFinish=false;
	while (pDB->More())
	{
		dtData=pDB->GetTime(gc_cTime);
		iType=pDB->GetLong(sType.c_str());
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtData);
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(sRev.c_str()));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iType);
		if (bHaveFinish)
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		else
		{
			if ((dtData==(vDtType[index].first))&&(iType==(vDtType[index].second)))
			{
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
				index++;
				index1=index-1;
				if (!(index<iRecords))
				{
					index--;
					bHaveFinish=true;
				}
			}
			else
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		}
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,vDtType[index1].second);
		ilen=vpTh[index1].second;
		pFixComm->SetItem(FIELD_SERVICE_LEN,ilen);
		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF,(char*)(vpTh[index1].first),ilen*sizeof(CThreshold));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}
QUIT_GETVIBALARMINFOTREND:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	VP_TH::iterator itvpTh;
	for (itvpTh=vpTh.begin();itvpTh!=vpTh.end();++itvpTh)
	{
		if ((itvpTh->first)!=NULL)
			delete [] itvpTh->first;
	}
	vpTh.clear();
	vThID.clear();
	vDtType.clear();
	return nCount;
}

VP_TH CTransactData::GetSpecIDThreshold(vector<int> vID,IDBInterFace *pDB_,string sSuffix_)
{
	SwitchDbNameEx(pDB_,sSuffix_.c_str());
	VP_TH vpTh;
	CThreshold *pTh=NULL;
	vector<int>::iterator it;
	long ilen;
	CWaveHead sWaveHead;
	for (it=vID.begin();it!=vID.end();++it)
	{
		pDB_->Cmd("Select [%s],[%s] from [THRESHOLD_%s] ",gc_cThreshold,gc_cThreshold_Len,sSuffix_.c_str());
		pDB_->Cmd("WHERE [%s]=%d",gc_cThreshold_ID,(*it));
		if (pDB_->More())
		{
			ilen=pDB_->GetLong(gc_cThreshold_Len);
			pTh=new CThreshold[ilen];
			SimADO_SafeGetBufferEx(pDB_, gc_cThreshold, (BYTE*)pTh, ilen, sWaveHead);
			KD_DeletePoint(pTh);
		}
		else
		{
			ilen=0;
			pTh=NULL;
		}
		vpTh.push_back(make_pair(pTh,ilen));
	}
	return vpTh;
}

CThreshold CTransactData::GetSpecIDThreshold(int iID_,IDBInterFace *pDB_,string sSuffix_)
{
	SwitchDbNameEx(pDB_,sSuffix_.c_str());
	CThreshold th;
	long iSize=0;
	CWaveHead sWaveHead;
	pDB_->Cmd("Select [%s] from [THRESHOLD_%s] ",gc_cThreshold,sSuffix_.c_str());
	pDB_->Cmd("WHERE [%s]=%d",gc_cThreshold_ID,iID_);
	if (pDB_->More())
	{
		SimADO_SafeGetBufferEx(pDB_, gc_cThreshold, (BYTE*)&th, iSize,sWaveHead);
	}
	return th;
}

/**得到指定公司、分厂、机组、轴位移测点、报警事件ID的数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*	       分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_TREND_AXISDISPLACE
*          时间值              FIELD_SERVICE_OFF_TIME
*          测点门限            FIELD_SERVICE_THRESHOLD
*          报警点标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND      21029
*/
int CTransactData::GetAxisDisplaceAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int nCount=-2;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	//得到报警事件信息
	string sTable="ALARM_LOG_",sRev,sField;
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	int iThID;
	bool bHaveMark=false;
	int iType,iEventType;
	CThreshold th;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//得到通道信息
	S_ChannInfo channInfo= pPlant->GetSpecVibChannInfo( sChann );
	if (strcmp(channInfo._cChannID, "") == 0)
		goto QUIT_GETAXISDISPLACEALARMTREND;
	sTable="ALARM_LOG_";
	sTable.append(sSuffix);
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cStart_T,gc_cType,gc_cThreshold_ID,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	if(pDB->More())
	{
		iThID=pDB->GetLong(gc_cThreshold_ID);
		iEventType=pDB->GetLong(gc_cType);
		dtStart=pDB->GetTime(gc_cStart_T);
	}
	else
	{
		nCount=-1;
		goto QUIT_GETAXISDISPLACEALARMTREND;
	}
	if (channInfo._iRevType==2)
		sRev=gc_cRev2;
	else
		sRev=gc_cRev;
	th=GetSpecIDThreshold(iThID,pDB,sSuffix);
	sTable="ALARM_VIB_";
	sTable.append(sSuffix);
	sField=ConsVibChannSpecTypeField(sChann,GE_AXIALDISPLACE);
	//使用三倍频相位表示轴位移值
	pDB->Cmd("Select %s,%s,%s,%s from [%s] ",gc_cTime,sField.c_str(),sRev.c_str(),gc_cType,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(sRev.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_TREND_AXISDISPLACE,pDB->GetDouble(sField.c_str()));
		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD,(char *)&th,sizeof(CThreshold));
		dtData=pDB->GetTime(gc_cTime);
		iType=pDB->GetLong(gc_cType);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtData);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iType);
		if (bHaveMark)
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		else
		{
			if ((dtData==dtStart)&&(iType==iEventType))
			{
				bHaveMark=true;
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
			}
			else
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		}
		nCount++;
	}
QUIT_GETAXISDISPLACEALARMTREND:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/**得到指定公司、分厂、机组、过程量测点、报警事件ID的数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点值              FIELD_SERVICE_TREND_PROCESS
*          时间值              FIELD_SERVICE_OFF_TIME
*          测点门限            FIELD_SERVICE_THRESHOLD
*          报警类型            FIELD_SYSTEM_TYPE
*          报警点标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETPROCALARMTREND        21030
*/
int CTransactData::GetProcAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm)==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int nCount=-2;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	//得到报警事件信息
	string sTable="ALARM_LOG_";
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	int iThID;
	bool bHaveMark=false;
	int iType,iEventType;
	CThreshold th;
	sTable.append(sSuffix);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cStart_T,gc_cType,gc_cThreshold_ID,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	if(pDB->More())
	{
		iThID=pDB->GetLong(gc_cThreshold_ID);
		iEventType=pDB->GetLong(gc_cType);
		dtStart=pDB->GetTime(gc_cStart_T);
	}
	else
	{
		nCount=-1;
		goto QUIT_GETPROCALARMTREND;
	}
	th=GetSpecIDThreshold(iThID,pDB,sSuffix);
	sTable="ALARM_PROC_";
	sTable.append(sSuffix);
	pDB->Cmd("Select %s,%s,%s,%s from [%s] ",gc_cTime,gc_cProcValue,gc_cRev,gc_cType,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SERVICE_TREND_PROCESS,pDB->GetDouble(gc_cProcValue));
		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD,(char *)&th,sizeof(CThreshold));
		dtData=pDB->GetTime(gc_cTime);
		iType=pDB->GetLong(gc_cType);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtData);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,iType);
		if (bHaveMark)
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		else
		{
			if ((dtData==dtStart)&&(iType==iEventType))
			{
				bHaveMark=true;
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
			}
			else
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		}
		nCount++;
	}
QUIT_GETPROCALARMTREND:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定公司、分厂、机组、过程量测点、报警事件ID、时间的数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*          时间值              FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点值              FIELD_SERVICE_TREND_PROCESS
#define     SERVICE_SERVICE_OFF_GETPROCALARMDATA        21031
*/
int CTransactData::GetProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm)==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int nCount=-2;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	dtData=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pDB->Cmd("SELECT [%s],[%s] from [ALARM_PROC_%s] ",gc_cRev,gc_cProcValue,sSuffix.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]='%s'",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(dtData),gc_cChannID,sChann.c_str());
	if (pDB->More())
	{
		nCount=1;
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SERVICE_TREND_PROCESS,pDB->GetDouble(gc_cProcValue));
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定公司、分厂、机组、测点、报警事件ID的趋势报警数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          特征值Buff          FIELD_SERVICE_CHARVALUE_SBUFF
*          特征Buff长度        FIELD_SERVICE_TIMEWAVE_NUM
*          测点门限Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          门限个数           FIELD_SERVICE_LEN
*          报警类型            FIELD_SYSTEM_TYPE
*          转速:               FIELD_SERVICE_OFF_REV
#define     SERVICE_SERVICE_OFF_GETTRENDALARMDATA        21032
*/
int CTransactData::GetTrendAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	if (ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm)==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int nCount=-2;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	pDB->Cmd("SELECT * from [ALARM_TREND_%s] ",sSuffix.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		CTrendCheckParam *pCheck=NULL;
		S_VibCharValue *pVibValue=NULL;
		int iLen=pDB->GetLong(gc_cThreshold_Len);
		long iSize=0;
		pCheck=new CTrendCheckParam[iLen];
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cThreshold, (BYTE*)pCheck, iSize, sWaveHead);
		pFixComm->SetItem(FIELD_SERVICE_LEN,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF,(char *)pCheck,iSize);
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(gc_cRev));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));
		iLen=pDB->GetLong(gc_cData_Len);
		pVibValue=new S_VibCharValue[iLen];
		SimADO_SafeGetBufferEx(pDB, gc_cData, (BYTE*)pVibValue, iSize, sWaveHead);
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iLen);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)pVibValue,iSize);
		delete [] pVibValue;
		delete [] pCheck;
		nCount=iLen;
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/**得到指定公司、分厂、机组、轴位移测点、报警事件ID、时间的详细数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*          时间值              FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_TREND_AXISDISPLACE
*          直流量              FIELD_SERVICE_TREND_DC
*          间隙电压            FIELD_SERVICE_TREND_GAP
*          报警类型            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA    21033
*/
int CTransactData::GetAxisDisplaceAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sSuffix="";
	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlant==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int nCount=-2;
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	CBHDateTime dtData=CBHDateTime::GetCurrentTime();
	dtData=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	string sTable,sRev,sField;
	//得到通道信息
	S_ChannInfo channInfo= pPlant->GetSpecVibChannInfo( sChann );
	if (channInfo._iRevType==2)
		sRev=gc_cRev2;
	else
		sRev=gc_cRev;
	sTable="ALARM_VIB_";
	sTable.append(sSuffix);
	sField=ConsVibChannSpecTypeField(sChann,GE_AXIALDISPLACE);
	//使用三倍频相位表示轴位移值
	pDB->Cmd("Select %s,%s,%s,%s,%s from [%s] ",ConsVibChannSpecTypeField(sChann,GE_DCCHAR).c_str(),ConsVibChannSpecTypeField(sChann,GE_GAP).c_str(),sField.c_str(),sRev.c_str(),gc_cType,sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(dtData));
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetDouble(sRev.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_TREND_AXISDISPLACE,pDB->GetDouble(sField.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_TREND_DC,pDB->GetDouble(ConsVibChannSpecTypeField(sChann,GE_DCCHAR).c_str()));
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,pDB->GetDouble(ConsVibChannSpecTypeField(sChann,GE_GAP).c_str()));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));
		nCount=1;
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定历史时刻、机组示功图测点的波形。
*输入参数值：公司ID             FIELD_SYSTEM_COMPANY
*		     分厂名            FIELD_SYSTEM_FACTORY
*            机组ID             FIELD_SERVICE_PLANTID
*            测点buff           FIELD_SERVICE_CHANN_BUFF
*            测点buff长度       FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数           FIELD_SERVICE_CHANN_NUM
*            示功图类型         FIELD_SYSTEM_TYPE       P-V, P-a, P-t
*            时间               FIELD_SERVICE_OFF_TIME
*            波形数据个数      FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*            测点的波形数据   FIELD_SERVICE_OFF_WAVE
*            波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*            测点的X轴数据    FIELD_SERVICE_ZOOMSPECTRUM
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            报警值           FIELD_SYSTEM_ALARM_LOW
*            危险值           FIELD_SYSTEM_ALARM_HIGH
*            单位             FIELD_SERVICE_CHANN1_UNIT
*            测点别名         FIELD_SYSTEM_CHANN_ALIAS
*            转速             FIELD_SERVICE_REV
*            采样频率        FIELD_SERVICE_SAMPLE_FREQ
*            吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*            吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*            排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*            排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_OFF_GETPVDATA                   21035
*/
int CTransactData::GetPVData( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	int iRtn = -1;
	string sCompany,sFactory,sPlant,sChann,sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant, sSuffix, pFixComm, true);
	if (pPlantInfo ==NULL)
		return -1;
	int iPvType=pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	//CString strInfo;strInfo.Format("off测点数目%d",iChannNum);
	//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	int   iWaveNum = pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	list<string> lChann;
	list<string>::iterator it;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	iWaveNum = iWaveNum <100 ? 513: iWaveNum;
	DATATYPE_WAVE *pWave=NULL;
	DATATYPE_WAVE *pXCoor=NULL;
	float         *pfWave = NULL;

	pWave = new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &lvfWave.front();

	S_DynCharValue dynCharValue;
	float fRev =.1;
	int iSmpFreq =1;
	int iStartIndex = 0,iEndIndex = 0;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//循环测点通道得到波形数据及其相关数据
	for (it=lChann.begin();it!=lChann.end();++it)
	{
		sChann = *(it);
		S_ChannInfo channInfo = pPlantInfo->GetSpecDynChannInfo(sChann);
		CWaveHead sWaveHead;
		iRtn=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann, pWave, iWaveNum, dynCharValue, iSmpFreq, fRev, timeOff, pfWave,sWaveHead);
		if (iRtn>0)
		{
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
			//CString strInfo;strInfo.Format("off得到的周期起始和结束索引%d,%d,波形点数%d",iStartIndex, iEndIndex,iWaveNum);BHTRACE_DEBUG(strInfo);
			//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
			//判断整周期点数是否大于波形点数，避免内存泄露
			iStartIndex=0;
			pXCoor = new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			int iCylinderNo = pPlantInfo->GetSpecChannCylinderNo(pDB,sChann);
			pFixComm->SetItemBuf( FIELD_SERVICE_CHARVALUE_S,(char *)&dynCharValue, sizeof(S_DynCharValue));
			pFixComm->SetItem( FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
			if (iPvType == GE_PVTYPE_Pt)
			{///假如画p-t图，则不按周期点数画，而按照全部实际的波形点数画
				pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWave, iWaveNum * sizeof(DATATYPE_WAVE));
				for (int nloop=0; nloop<iWaveNum;nloop++)
					pXCoor[nloop] = (float)nloop;
				if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iWaveNum))
				{
					pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWave, iWaveNum * sizeof(DATATYPE_WAVE));
					iWaveNum = pPlantInfo->GetPVXCoorData( iSmpFreq, iWaveNum, iPvType, fRev,iCylinderNo,pXCoor);
					if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);

						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));

					}
					else
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
					}
				}
				else
				{
					pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)&(pWave[iStartIndex]), (iEndIndex-iStartIndex) * sizeof(DATATYPE_WAVE));
					iWaveNum = pPlantInfo->GetPVXCoorData(iSmpFreq,iEndIndex-iStartIndex,iPvType,iCylinderNo,pXCoor);
					if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);

						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&pfWave[iStartIndex],(iEndIndex-iStartIndex)*sizeof(float));

					}
					else
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
					}
				}
			}
			//writefile(pXCoor,iWaveNum,222);
			//strInfo.Format("iWaveNum:%d",iWaveNum);BHTRACE_DEBUG(strInfo);
			//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
			pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iWaveNum);
			pFixComm->SetItemBuf( FIELD_SERVICE_ZOOMSPECTRUM, (char *)pXCoor ,iWaveNum * sizeof(DATATYPE_WAVE) );
			pFixComm->SetItem( FIELD_SERVICE_REV, fRev);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			KD_DeletePoint( pXCoor);
		}
		else
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM, 0);
	}
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pWave);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iRtn;
}

/**得到指定历史时刻、机组活塞杆沉降量测点的波形和频谱。
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		    分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点             FIELD_SERVICE_OFF_CHANN1
*          时间             FIELD_SERVICE_OFF_TIME
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          报警值           FIELD_SYSTEM_ALARM_LOW
*          危险值           FIELD_SYSTEM_ALARM_HIGH
*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
*          单位             FIELD_SERVICE_CHANN1_UNIT
*          转速             FIELD_SERVICE_REV
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
*          测点起始索引                          FIELD_SERVICE_STARTINDEX
*          测点结束索引                          FIELD_SERVICE_ENDINDEX
*          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036
*/
int CTransactData::GetRodSinkData( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	string sCompany;
	string sFactory;
	string sPlant,sSuffix;
	CPlantInfoOffLine *pPlant = ReadCheckCompanyInfo( sCompany, sFactory, sPlant, sSuffix, pFixComm, true);
	if (pPlant ==NULL)
		return -1;
	string sChann1 = pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	int   iWaveNum = pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	//CString sTemp;sTemp.Format("%d",iWaveNum);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	iWaveNum = iWaveNum <100 ? 513: iWaveNum;
	//CString sTemp;sTemp.Format("iSmpNums:%d",iSmpNums);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
	DATATYPE_WAVE *pWave = NULL;
	DATATYPE_WAVE *pWaveNotDC = NULL;
	float *pfWave = NULL;
	float *pfWaveNotDC = NULL;

	S_DynCharValue dynCharValue;
	S_ChannInfo channInfo;
	float fRev =.1;
	int iSmpFreq =1,iSmpNums=1,iRev=0;
	channInfo=pPlant->GetSpecDynChannInfo(sChann1);
	iSmpNums=pPlant->GetPlantInfo()._iSmpNum;
	if (iSmpNums<4)
	{
		return -1;
			 
	}
	CWaveHead sWaveHead;
	pWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	pWaveNotDC = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];

	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfWaveNotDC(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &lvfWave.front();
	pfWaveNotDC = &lvfWaveNotDC.front();

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	int iRtn=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1, pWave, iSmpNums, dynCharValue, iSmpFreq, fRev, timeOff, pfWave, sWaveHead);
	if (iRtn>0)
	{
		iWaveNum = iWaveNum >iSmpNums ? iSmpNums: iWaveNum;
		int iStartIndex,iEndIndex;

		pPlant->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);

		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
		
		DATATYPE_WAVE *pSpectrum = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX];

		float  *pfSpectrum;
		vector<float> lvfSpectrum(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		pfSpectrum = &lvfSpectrum.front();

		
		
		DATATYPE_WAVE *pWaveTemp = pWave;


		//CString sTemp;sTemp.Format("iWaveNum:%d,iSmpNums:%d",iWaveNum,iSmpNums);
		//CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
		pPlant->SubtractWaveDC(pWaveTemp,pWaveNotDC,iWaveNum);

		pPlant->SubtractWaveDC(pfWave,pfWaveNotDC,iWaveNum);
		
		
		pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWaveNotDC, iWaveNum * sizeof(DATATYPE_WAVE));
		//pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iWaveNum);
		pFixComm->SetItemBuf( FIELD_SERVICE_CHARVALUE_S, (char *)&dynCharValue, sizeof(S_DynCharValue));
		
		int lnWaveNum = iWaveNum;

		int lnDataSize = pPlant->SpectrumAnalysis(pfWaveNotDC, lnWaveNum, pfSpectrum, channInfo._iSensorType);
		int            iLines    = lnWaveNum/2.56;
		int            lnSampleFreq = dynCharValue._iSmpFreq;
		float          fDf       = (float)lnSampleFreq/(float)iWaveNum;

		pPlant->ConvertFloatToDT(pSpectrum,pfSpectrum,iLines);

		pFixComm->SetItemBuf( FIELD_SERVICE_OFF_FREQWAVE, (char *)pSpectrum ,iLines * sizeof(DATATYPE_WAVE) );
		pFixComm->SetItem( FIELD_SERVICE_OFF_FREQWAVE_DF, fDf);
		pFixComm->SetItem( FIELD_SERVICE_OFF_FREQWAVE_NUM, iLines);
		pFixComm->SetItem( FIELD_SERVICE_CHANN1_UNIT, channInfo._cUnit);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo._chAlarmParam._pThLearnParamF[8]._fParam4);
		//sTemp.Format("ALARM_LOW:%f, ALARM_HIGH:%f",pPlant->GetSpecDynChannInfo(sChann1)._chAlarmParam._pThLearnParamS[9]._fParam2,pPlant->GetSpecDynChannInfo(sChann1)._chAlarmParam._pThLearnParamS[9]._fParam1);
		//CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
		pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,channInfo._chAlarmParam._pThLearnParamS[9]._fParam2);
		pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,channInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
		pFixComm->SetItem( FIELD_SERVICE_REV, fRev);

		if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);
			
			pFixComm->SetItemBuf( FIELD_SERVICE_SEND_FLOAT_WAVE1, (char *)pfWaveNotDC, iWaveNum * sizeof(float));

			pFixComm->SetItemBuf( FIELD_SERVICE_SEND_FLOAT_FREQ1, (char *)pfSpectrum, iLines * sizeof(float));
			
		}
		else
		{
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
		}
	
		//int iStartIndex,iEndIndex;
		
		//pPlant->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		//iEndIndex = iWaveNum;
/*		iEndIndex = 128;*/
		pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,iStartIndex);
		pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,iEndIndex);
		KD_DeletePoint( pSpectrum);
	}
	else
		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM, 0);
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pWave);
	KD_DeletePoint(pWaveNotDC);
	return iRtn;
}

/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其指定时间波形和特征参数
*如果测点为空就不会返回对应的数据
*输入参数值：
*            公司ID                     FIELD_SYSTEM_COMPANY
*			 分厂名                     FIELD_SYSTEM_FACTORY
*            机组ID                     FIELD_SERVICE_PLANTID
*            振动测点buff               FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数               FIELD_SERVICE_CHANN_NUM
*            动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数               FIELD_SERVICE_CHANN_NUM2
*            活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
*            时间                       FIELD_SERVICE_OFF_TIME
*            波形数据个数               FIELD_SERVICE_OFF_WAVE_NUM
*返回字段：
*            波形数据的点数               FIELD_SERVICE_TIMEWAVE_NUM
*            采样频率                     FIELD_SERVICE_SAMPLE_FREQ
*            测点名称                     FIELD_SERVICE_CHANN1
*            测点波形                     FIELD_SERVICE_TIMEWAVE1
*            测点特征值                   FIELD_SERVICE_CHARVALUE_S
*            横坐标                       FIELD_SERVICE_FREQWAVE2
*            转速                         FIELD_SERVICE_OFF_REV
*            吸气阀开启相位角             FIELD_SERVICE_ANGLEPARAM1
*            吸气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM2
*            排气阀开启相位角             FIELD_SERVICE_ANGLEPARAM3
*            排气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA           21037
*/
int CTransactData::GetRcMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	int nCount=0;
	string sCompany;
	string sFactory;
	string sPlant ,sSuffix,sVibChann,sDynChann,sSinkChann;
	CPlantInfoOffLine *pPlant = ReadCheckCompanyInfo( sCompany, sFactory, sPlant, sSuffix, pFixComm, true);
	if (pPlant ==NULL)
		return -1;
	//得到振动测点
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//得到动态测点
	int iChannNum2   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM2);
	int iBytes2      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE2)+1;
	char *pChannBuff2= new char[iBytes2] ;
	memset(pChannBuff2,0,iBytes2);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF2,pChannBuff2,iBytes2);
	//得到活塞杆沉降量测点
	int iChannNum3   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM3);
	int iBytes3      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE3)+1;
	char *pChannBuff3= new char[iBytes3];
	memset(pChannBuff3,0,iBytes3);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF3,pChannBuff3,iBytes3);

	list<string> lVibChann;
	list<string>::iterator itVib;
	list<string> lDynChann;
	list<string>::iterator itDyn;
	list<string> lSinkChann;
	list<string>::iterator itSink;
	//解析振动测点
	lVibChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lVibChann.size();
	//解析动态测点
	lDynChann=ParseChannList(pChannBuff2,iChannNum2);
	iChannNum2=lDynChann.size();
	//解析活塞杆沉降量测点
	lSinkChann=ParseChannList(pChannBuff3,iChannNum3);
	iChannNum3=lSinkChann.size();

	int   iWaveNum        = pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff  = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	iWaveNum              = iWaveNum <100 ? 513: iWaveNum;
	int iSmpNumsDyn       = pPlant->GetPlantInfo()._iSmpNum;

	int iSmpNumsVib       = iSmpNumsDyn;
	int iSmpNumsRodSink   = iSmpNumsDyn;
	DATATYPE_WAVE *pWaveVib     = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pWaveDyn     = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pWaveRodSink = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pXCoor       = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pWaveNotDC   = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];

	float *pfWave = NULL; 
	float *pfWaveNotDC = NULL;

	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfWaveNotDC(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &lvfWave.front();
	pfWaveNotDC = &lvfWaveNotDC.front();

	CWaveHead lsWaveHead;

	int iSmpFreqDyn,iSmpFreqVib,iStartIndex=0,iEndIndex=0,iCylinderNo;
	S_DynCharValue dynCharValue,dynCharValueSink;
	S_VibCharValue vibCharValue;
	S_ChannInfo channInfo;
	float    fRev=.1;
	int iRtnDyn = 0, iRtnRodSink = 0, iRtnVib = 0;
	int iRealSmpNums = 0; //一个周期的采样点数

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//振动测点
	for (itVib=lVibChann.begin();itVib!=lVibChann.end();++itVib)
	{
		sVibChann = *itVib;
		iRtnVib     = ReadVibChannSpecTimeWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sVibChann, pWaveVib, iSmpNumsVib,vibCharValue, iSmpFreqVib, fRev, timeOff, pfWave, lsWaveHead);
		channInfo = pPlant->GetSpecVibChannInfo(sVibChann);
		pPlant->GetSpecVibChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		iEndIndex = iRtnVib;
		iCylinderNo = pPlant->GetSpecChannCylinderNo(pDB,sVibChann);
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if(pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE  || pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL )
		{
			iPeriodSmpNums = iEndIndex;
			iStartIndex = 0;
		}
		if ((pPlant->IsNotDynPeriodIndex(iStartIndex,iRtnVib))|| (iPeriodSmpNums>iSmpNumsVib))
			iRealSmpNums = pPlant->GetPVXCoorData( iSmpFreqVib, iSmpNumsVib, GE_PVTYPE_Pa, fRev,iCylinderNo, pXCoor);
		else
		{
			if(pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
			{
				iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqVib,iEndIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
			}
			else
			{
				iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqVib,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
			}
			
		}
		
		if (fRev<=0)
		{
			try
			{
				//ZeroMemory(pXCoor,sizeof(DATATYPE_WAVE )*GD_MIDDLEWARE_WAVE_POINT_MAX);
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
		if ( iRtnVib > 0 )
		{// 说明有振动信号
			++nCount;

			if (pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iSmpNumsVib))
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)pWaveVib, iRealSmpNums* sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pXCoor, iRealSmpNums * sizeof(DATATYPE_WAVE));

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iRealSmpNums*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}

			}
			else
			{
				if(pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlant->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)&(pWaveVib[0]), iRealSmpNums*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[0]), iRealSmpNums*sizeof(DATATYPE_WAVE));

					if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iRealSmpNums*sizeof(float));

					}
					else
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
					}
				}
				else
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)&(pWaveVib[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));

					if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&pfWave[iStartIndex],(iEndIndex-iStartIndex)*sizeof(float));

					}
					else
					{
						pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
					}
				}
				
			}
			
			
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealSmpNums );
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreqVib);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	//动态压力
	for (itDyn=lDynChann.begin();itDyn!=lDynChann.end();++itDyn)
	{
		sDynChann=*itDyn;
		iRtnDyn     = ReadDynChannSpecTimeWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sDynChann, pWaveDyn, iSmpNumsDyn, dynCharValue,iSmpFreqDyn,fRev, timeOff,pfWave,lsWaveHead,false);
		channInfo = pPlant->GetSpecDynChannInfo(sDynChann);
		if (iEndIndex<1 && iStartIndex<1)//防止周期点数不一致
		    pPlant->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		iCylinderNo = pPlant->GetSpecChannCylinderNo(pDB,sDynChann);
		iStartIndex=0;
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex))|| (iPeriodSmpNums>iSmpNumsDyn))
			iRealSmpNums = pPlant->GetPVXCoorData( iSmpFreqDyn, iSmpNumsDyn, GE_PVTYPE_Pa, fRev,iCylinderNo, pXCoor);
		else
			iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqDyn,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);

		if (fRev<=0)
		{
			try
			{
				//ZeroMemory(pXCoor,sizeof(DATATYPE_WAVE )*GD_MIDDLEWARE_WAVE_POINT_MAX);
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
		if ( iRtnDyn >0 )
		{//说明有动态压力信号
			++nCount;
			if (pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex))
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pWaveDyn, iRealSmpNums * sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pXCoor, iRealSmpNums * sizeof(DATATYPE_WAVE));
				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iRealSmpNums*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pWaveDyn[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&pfWave[iStartIndex],(iEndIndex-iStartIndex)*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char*)&dynCharValue, sizeof(S_DynCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealSmpNums );
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreqDyn);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	//活塞杆沉降量
	for (itSink=lSinkChann.begin();itSink!=lSinkChann.end();++itSink)
	{
		sSinkChann=*itSink;
		iRtnRodSink = ReadDynChannSpecTimeWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sSinkChann, pWaveRodSink, iSmpNumsRodSink, dynCharValueSink, iSmpFreqDyn, fRev, timeOff, pfWave,lsWaveHead, false);
		int i =0;
		for ( i=0;i<iRtnRodSink-1;i++)
		{
			if (IsFloatZero(pWaveRodSink[i])&&IsFloatZero(pWaveRodSink[i+1]))
			{
				break;
			}
		}
		iEndIndex = i;
		channInfo = pPlant->GetSpecDynChannInfo(sSinkChann);
		if (iEndIndex<1 && iStartIndex<1)//防止周期点数不一致
		{	
			pPlant->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		}

		iCylinderNo = pPlant->GetSpecChannCylinderNo(pDB,sSinkChann);
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iSmpNumsDyn))
			iRealSmpNums = pPlant->GetPVXCoorData( iSmpFreqDyn, iSmpNumsDyn, GE_PVTYPE_Pa, fRev,iCylinderNo, pXCoor);
		else
			iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqDyn,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
	
// 		for ( i=0;i<iRealSmpNums-1;i++)
// 		{
// 			if (IsFloatZero(pWaveRodSink[i])&&IsFloatZero(pWaveRodSink[i+1]))
// 			{
// 				break;
// 			}
// 		}
/*		iRealSmpNums = i;*/
		pPlant->SubtractWaveDC(pWaveRodSink,pWaveNotDC,iRealSmpNums);
		pPlant->SubtractWaveDC(pfWave,pfWaveNotDC,iRealSmpNums);

		if (fRev<=0)
		{
			try
			{
				ZeroMemory(pXCoor,sizeof(DATATYPE_WAVE )*GD_MIDDLEWARE_WAVE_POINT_MAX);
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
		if ( iRtnRodSink > 0 )
		{// 说明有沉降量信号
			++nCount;
		
			if (pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iSmpNumsDyn))
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)pWaveRodSink, iRealSmpNums * sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pXCoor, iRealSmpNums * sizeof(DATATYPE_WAVE));

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iRealSmpNums*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)&(pWaveRodSink[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&pfWave[iStartIndex],(iEndIndex-iStartIndex)*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&dynCharValueSink,sizeof(S_DynCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealSmpNums );
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreqDyn);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lVibChann.clear();
	lDynChann.clear();
	lSinkChann.clear();
	KD_DeletePoint(pWaveVib);
    KD_DeletePoint(pWaveDyn);
    KD_DeletePoint(pWaveRodSink);
	KD_DeletePoint(pWaveNotDC);
	KD_DeletePoint(pXCoor);
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pChannBuff2);
	KD_DeletePoint(pChannBuff3);
	return nCount;
}

/**历史概貌图，得到指定时间、指定机组所有测点数据
*输入参数值：
*            公司ID         FIELD_SYSTEM_COMPANY
*	         分厂名         FIELD_SYSTEM_FACTORY
*            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
*            设备号buff     FIELD_SERVICE_CHANN_BUFF
*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            设备号个数     FIELD_SERVICE_CHANN_NUM
*            时间值         FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            设备ID         FIELD_SERVICE_PLANTID
*            测点ID         FIELD_SERVICE_CHANN1
*            测点值         FIELD_SERVICE_CHANN1VALUE
*            测点单位       FIELD_SERVICE_CHANN1_UNIT
*            测点所属列表   FIELD_SERVICE_CHANN1_LIST
*            测点的状态     FIELD_SERVICE_CHANN1_STATUS
*            测点特征值     FIELD_SERVICE_CHARVALUE_S
*            测点类型       FIELD_SERVICE_CHANNTYPE
*            转速1          FIELD_SERVICE_REV
*            转速2          FIELD_SERVICE_REV2
*            转速3          FIELD_SERVICE_REV3
*            转速4          FIELD_SERVICE_REV4
*            转速5          FIELD_SERVICE_REV5
#define     SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE    21038
*/
int CTransactData::GetSpecTimeAllChannValue( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	string sPlant;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int iPlantNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pPlantBuff=new char[iBytes];
	ZeroMemory(pPlantBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pPlantBuff,iBytes);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	list<string> lPlant;
	list<string>::iterator itPlant;
	lPlant=ParseChannList(pPlantBuff,iPlantNum);
	iPlantNum=lPlant.size();
	CPlantInfoOffLine *pPlant=NULL;
	string sChann="";
	string sSuffix;
	string sChannAlias;
	unsigned int iState=0;
	string sUnit="";
	int iList=0;
	int iNum=0,nCount=-1;
	int iloop=0;
	int iChannNum=0;
	int iChannType=0;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itPlant=lPlant.begin();itPlant!=lPlant.end();++itPlant)
	{
		//CString sTemp;sTemp.Format("%s",(*itPlant).c_str());BHTRACE_DEBUG(sTemp);
		pPlant = CheckCompanyInfo( sCompany, sFactory, (*itPlant), sSuffix, pFixComm, true);
		if (pPlant!=NULL)
		{
			int loop1=0;
			iChannNum=pPlant->GetRadialVibChannNum();
			S_VibCharValue vibCharValue;
			string sUnit;
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlant->GetSpecTimeChannAndTypeAlarmState(  pDB,
																timeOff,
																sSuffix,
																loop1,
																sChann,
																sChannAlias,
																sUnit,
																iList,
																iState,
																iChannType,
																GE_VIBCHANN))
				{//转速在读取数据的函数体里面设置
					float pfRev[5]={.0,.0,.0,.0,.0};

					BOOL lbDataGet = FALSE;

					lbDataGet = ReadVibChannSpecTimeCharValueInfoEx(
																	pFixComm,
																	pDB,
																	sCompany,
																	sFactory,
																	(*itPlant),
																	sChann,
																	timeOff,
																	iCharType,
																	true,
																	pfRev);
					
					if (lbDataGet <0)
					{
						lbDataGet = ReadVibChannSpecTimeCharValueInfoEx(
							pFixComm,
							pDB,
							sCompany,
							sFactory,
							(*itPlant),
							sChann,
							timeOff,
							iCharType,
							false,
							pfRev);
					}
					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					//CString sTemp;sTemp.Format("iState:%d,ParseAlarmStatus:%d",iState,ParseAlarmStatus(iState));BHTRACE_DEBUG(sTemp);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					//CString sTemp;sTemp.Format("%f,%f,%f,%f,%f",pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4]);BHTRACE_DEBUG(sTemp);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}
			iChannNum=pPlant->GetAxialVibChannNum();
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlant->GetSpecTimeChannAndTypeAlarmState(pDB,timeOff,sSuffix,loop1,sChann,sChannAlias,sUnit,iList,iState,iChannType,GE_AXIALCHANN))
				{
					BOOL lbDataGet = FALSE;

					lbDataGet = ReadVibChannSpecTimeCharValueInfoEx(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,iCharType,true);

					if (lbDataGet <0)
					{
						lbDataGet = ReadVibChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,iCharType,false);
					}

					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}

			iChannNum=pPlant->GetDynChannNum();
			S_DynCharValue dynCharValue;
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlant->GetSpecTimeChannAndTypeAlarmState(pDB,
															timeOff,
															sSuffix,
															loop1,
															sChann,
															sChannAlias,
															sUnit,
															iList,
															iState,
															iChannType,
															GE_DYNPRESSCHANN))
				{
					BOOL lbDataGet = FALSE;

					lbDataGet = ReadDynChannSpecTimeCharValueInfoEx(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,iCharType,true);

					if (lbDataGet <0)
					{
						lbDataGet = ReadDynChannSpecTimeCharValueInfoEx(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,iCharType,false);					
					}

					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}

			iChannNum=pPlant->GetProcChannNum();
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlant->GetSpecTimeChannAndTypeAlarmState(pDB,timeOff,sSuffix,loop1,sChann,sChannAlias,sUnit,iList,iState,iChannType,GE_ALLPROC))
				{

					BOOL lbDataGet = FALSE;

					lbDataGet = ReadProcChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,true);

					if (lbDataGet < 0)
					{
						lbDataGet = ReadProcChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,(*itPlant),sChann,timeOff,false);
					}

					if (lbDataGet!=0)
					{
						pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
						pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
						pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
						pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
						pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
						pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
						pFixComm->Write(pFixComm->GetAci());
						pFixComm->CreateNextAnswer();
						++nCount;
					}
				}
			}
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	KD_DeletePoint(pPlantBuff);
	return nCount;
}


/**得到指定公司、分厂、机组、测点,指定启停车事件和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*          分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          启停车事件ID         FIELD_SYSTEM_EVENT_ID
*          转速                 FIELD_SERVICE_OFF_REV
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          转子旋向：                     FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*          转速                           FIELD_SERVICE_OFF_REV
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK           21039
*/
int CTransactData::GetSudPurifyAxesTrack( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	int nCount=-1;
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	string sLocation=pFixComm->GetItem(FIELD_SYSTEM_AXISLOCATION);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iRev=pFixComm->GetLong(FIELD_SERVICE_OFF_REV);
	int lnRevType = pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	//CString sTemp;sTemp.Format("%s,%s,%d,%d,%d",sChannIDHor,sChannIDVer,iWaveNum,iEventID,iRev);BHTRACE_DEBUG(sTemp);
	CString strRevType="";
	float fRev=.0;
	fRev=(float)iRev;
	int iSmpFreq=0,iSmpNum=0,iErrorCode=-1;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	BYTE *pVerBytes=NULL;
	BYTE *pHorBytes=NULL;
	double *pVerWaveData=NULL;
	double *pHorWaveData=NULL;
	vector<SSigParam> vSigParamV,vSigParamH;
	long iAllBytes=0;
	iWaveNum=iWaveNum<100?513:iWaveNum;

	S_ChannInfo channInfoHor;
	S_ChannInfo channInfoVer;
	string sChannNoVer;
	string sChannNoHor;

	if ((sChannIDHor.empty())||(sChannIDVer.empty()))
	{
		//查询得到轴心位置的对应的垂直测点和水平测点
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",
			gc_cCompany,sCompany.c_str(),
			gc_cFactory_Name,sFactory.c_str(),
			gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),
			gc_cLocation,sLocation.c_str());
		if (pDB->More())
		{
			sChannNoHor = pDB->Get(gc_cHorChann);
			sChannNoVer = pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		channInfoHor = pPlantInfo->GetSpecVibChannNoInfo(sChannNoHor);
		channInfoVer = pPlantInfo->GetSpecVibChannNoInfo(sChannNoVer);
		sChannIDHor  = channInfoHor._cChannID;
		sChannIDVer  = channInfoVer._cChannID;
	}
	else
	{
		channInfoVer = pPlantInfo->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlantInfo->GetSpecVibChannInfo( sChannIDHor );
		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;
	}


	string sSpecChannAllFieldVer = ConsVibChannField( sChannNoVer, true);
	string sSpecChannAllFieldHor = ConsVibChannField( sChannNoHor, true);

	SwitchDbNameEx(pDB,sAlias.c_str());

	string lsRev = pPlantInfo->GetRevField(lnRevType);

	string sTable="SUD_";
	sTable.append(sAlias);

	if (fRev>-1)
	{
		pDB->Cmd("select %s,%s,%s,%s from [%s] ",
			sSpecChannAllFieldVer.c_str(),
			sSpecChannAllFieldHor.c_str(),
			ConsCommonField(),
			gc_cTime,
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and %s > %f and %s < %f",
			gc_cEventID,iEventID,
			lsRev.c_str(),fRev-1,
			lsRev.c_str(), fRev+1);
	}
	else
	{
		pDB->Cmd("select %s,%s,%s,%s from [%s] ",
			sSpecChannAllFieldVer.c_str(),
			sSpecChannAllFieldHor.c_str(),
			ConsCommonField(),
			gc_cTime,
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d order by [%s]",gc_cEventID,iEventID,gc_cTime);
	}

	//BHTRACE_DEBUG(pDB->GetSql());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if(pDB->More())
	{
		iSmpNum  = SimADO_GetSampleNumEx(pDB, sChannNoHor);
		iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannNoHor);
		iRev     = SimADO_GetRevEx(pDB, sChannNoHor);
		//CString sTmp;sTmp.Format("%d,%d,%d",iSmpNum,iSmpFreq,iRev);BHTRACE_DEBUG(sTmp);
		//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecChannRevSud");
		double f0 = (double)iRev/60;
		double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
		int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM;
		vSigParamV.resize(MLcNum+MHcNum);
		vSigParamH.resize(MLcNum+MHcNum);
		if (iSmpNum>1)
		{
			iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
			pHorBytes=new BYTE[iAllBytes];
			//得到水平方向的波形数据
			strRevType.Format("WAVE_%s", sChannNoHor.c_str());
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, strRevType, pHorBytes, iAllBytes,sWaveHead);
			int iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
			iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
			pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
			memcpy( pHorWave,pHorBytes,iAllBytes);
			pHorWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pPlantInfo->ConvertDTWaveToDouble(pHorWaveData,pHorWave,iWaveNum,sWaveHead);
			//writefile(pHorWaveData,iWaveNum,3);
			//得到垂直方向的波形数据
			iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
			pVerBytes=new BYTE[iAllBytes];
			strRevType.Format("WAVE_%s", sChannNoVer.c_str());
			SimADO_SafeGetBufferEx(pDB, strRevType, pVerBytes, iAllBytes,sWaveHead);
			iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
			iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
			pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
			memcpy( pVerWave,pVerBytes,iAllBytes);
			pVerWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pPlantInfo->ConvertDTWaveToDouble(pVerWaveData,pVerWave,iWaveNum,sWaveHead);
			//writefile(pVerWaveData,iWaveNum,4);
			//获得提纯轴心轨迹的各个谐波数据
			CSigMath calcAxesData;
			try
			{
				iErrorCode = calcAxesData.SCorbitpurifie_Rp(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth,vSigParamV,vSigParamH);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			//CString sTemp;sTemp.Format("iErrorCode:%d,iWaveNum:%d,f0:%f,iSmpFreq:%d,MHcNum:%d,AHcth:%f,MLcNum:%d,ALcth:%f",iErrorCode,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth);BHTRACE_DEBUG(sTemp);
			//CHZLogManage::Debug(sTemp,"CTransactData","GetPurifyAxesTrack");

			if (iErrorCode>-1)
			{
				int iSize=0;
				iSize=vSigParamH.size();
				for (int iNum =0;iNum<iSize;iNum++)
				{
					if (vSigParamH[iNum]._fFreq!=0 && vSigParamH[iNum]._fAmp!=0)
					{
						iErrorCode++;
						pFixComm->SetItem(FIELD_SERVICE_VERFREQ,vSigParamV[iNum]._fFreq);
						pFixComm->SetItem(FIELD_SERVICE_VERAMP,vSigParamV[iNum]._fAmp);
						pFixComm->SetItem(FIELD_SERVICE_VERPHASE,vSigParamV[iNum]._fPhase);
						pFixComm->SetItem(FIELD_SERVICE_HORFREQ,vSigParamH[iNum]._fFreq);
						pFixComm->SetItem(FIELD_SERVICE_HORAMP,vSigParamH[iNum]._fAmp);
						pFixComm->SetItem(FIELD_SERVICE_HORPHASE,vSigParamH[iNum]._fPhase);
						pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
						//CString sTemp;sTemp.Format("提纯轨迹:iNum:%d,Fv:%f,Av:%f,Fh:%f,Ah:%f",iNum,vSigParamV[iNum]._fFreq/f0,vSigParamV[iNum]._fAmp,vSigParamH[iNum]._fFreq/f0,vSigParamH[iNum]._fAmp);BHTRACE_DEBUG(sTemp);
						//CHZLogManage::Debug(sTemp,"Off:CTransactData","GetPurifyAxesTrack");
						//CString sTemp;sTemp.Format("iNum:%d,Fv:%f,Av:%f,Pv:%f,Fh:%f,Ah:%f,Ph:%f",iNum,vSigParamV[iNum]._fFreq,vSigParamV[iNum]._fAmp,vSigParamV[iNum]._fPhase,vSigParamH[iNum]._fFreq,vSigParamH[iNum]._fAmp,vSigParamH[iNum]._fPhase);BHTRACE_DEBUG(sTemp);

						//转速
						pFixComm->SetItem(FIELD_SERVICE_OFF_REV,iRev);

						//转子旋向
						int iRotation =-1;
						iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);

						//进动方向
						int iPression = 0;
						int iOrbitShape=0;
						CSynopsis2KB calcOrbit;
						int icalcOrbit=-1;
						if (f0>.0)
						{
							try
							{
								icalcOrbit = calcOrbit.GetOrbitShape(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,iPression,iOrbitShape);
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
							CHZLogManage::Error("计算轴心进动方向出错","S_OnlineSev","CTransactData::OnGetPurifyAxesTrack");
						if (iPression==0)//如果计算为不确定则为正进动
							iPression=2;
						pFixComm->SetItem(FIELD_SYSTEM_STATUS,iPression);
						//CString sTemp;sTemp.Format("iRotation:%d,iPression:%d,iOrbitShape:%d",iRotation,iPression,iOrbitShape);BHTRACE_DEBUG(sTemp);


						S_VibCharValue vibValue;
						/* 得到水平特征值 */
						vibValue=ReadVibCharValue(sChannNoHor,pDB);
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
						{
						#if 0
							CString sLog;
							sLog.Format("Hor VibCharValue(ac=%f,1c=%f,2c=%f,3c=%f,hc=%f,dc=%f,gap=%f,rc=%f,1p=%f,2p=%f,3p=%f,hp=%f,rev=%d,SmpFreq=%d,SmpNum=%d)",
							vibValue.GetSpecTypeData(GE_ALLCHAR),
							vibValue.GetSpecTypeData(GE_ONECHAR),
							vibValue.GetSpecTypeData(GE_TWOCHAR),
							vibValue.GetSpecTypeData(GE_THREECHAR),
							vibValue.GetSpecTypeData(GE_HALFCHAR),
							vibValue.GetSpecTypeData(GE_DCCHAR),
							vibValue.GetSpecTypeData(GE_GAP),
							vibValue.GetSpecTypeData(GE_RESCHAR),
							vibValue.GetSpecTypeData(GE_PHASECHARONE),
							vibValue.GetSpecTypeData(GE_PHASECHARTWO),
							vibValue.GetSpecTypeData(GE_PHASECHARTHREE),
							vibValue.GetSpecTypeData(GE_PHASECHARHALF),
							vibValue._iRev,
							vibValue._iSmpFreq,
							vibValue._iSmpNum
							);
							BHTRACE_DEBUG(sLog);
						#endif
						}
						/* 得到垂直特征值 */
						vibValue=ReadVibCharValue(sChannNoVer,pDB);
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibValue,sizeof(S_VibCharValue));
						{
						#if 0
							CString sLog;
							sLog.Format("Ver VibCharValue(ac=%f,1c=%f,2c=%f,3c=%f,hc=%f,dc=%f,gap=%f,rc=%f,1p=%f,2p=%f,3p=%f,hp=%f,rev=%d,SmpFreq=%d,SmpNum=%d)",
							vibValue.GetSpecTypeData(GE_ALLCHAR),
							vibValue.GetSpecTypeData(GE_ONECHAR),
							vibValue.GetSpecTypeData(GE_TWOCHAR),
							vibValue.GetSpecTypeData(GE_THREECHAR),
							vibValue.GetSpecTypeData(GE_HALFCHAR),
							vibValue.GetSpecTypeData(GE_DCCHAR),
							vibValue.GetSpecTypeData(GE_GAP),
							vibValue.GetSpecTypeData(GE_RESCHAR),
							vibValue.GetSpecTypeData(GE_PHASECHARONE),
							vibValue.GetSpecTypeData(GE_PHASECHARTWO),
							vibValue.GetSpecTypeData(GE_PHASECHARTHREE),
							vibValue.GetSpecTypeData(GE_PHASECHARHALF),
							vibValue._iRev,
							vibValue._iSmpFreq,
							vibValue._iSmpNum
							);
							BHTRACE_DEBUG(sLog);
						#endif
						}

						pFixComm->Write(pFixComm->GetAci());
						pFixComm->CreateNextAnswer();
					}
				}
			}

		}
		else
		{
			pFixComm->SetItem(FIELD_CODE,-1);
			pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
			iErrorCode= -1;
		}
	}

CLEAN_GETAXESTRACT:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWaveData);
	KD_DeletePoint(pHorWaveData);
	KD_DeletePoint(pVerBytes);
	KD_DeletePoint(pHorBytes);
	return iErrorCode;
}


/**往复式系统报警多参数分析功能，读取其指定报警时间的波形和特征参数，读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。如果测点为空就不会返回对应的数据。
*输入参数值：
*            公司ID                     FIELD_SYSTEM_COMPANY
*			 分厂名                     FIELD_SYSTEM_FACTORY
*            机组ID                     FIELD_SERVICE_PLANTID
*            振动测点buff               FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数               FIELD_SERVICE_CHANN_NUM
*            动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数               FIELD_SERVICE_CHANN_NUM2
*            事件ID                     FIELD_SYSTEM_EVENT_ID
*            报警时间值                 FIELD_SERVICE_OFF_TIME
*返回字段：
*            波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
*            采样频率                   FIELD_SERVICE_SAMPLE_FREQ
*            测点名称                   FIELD_SERVICE_CHANN1
*            测点波形                   FIELD_SERVICE_TIMEWAVE1
*            测点特征值                 FIELD_SERVICE_CHARVALUE_S
*            横坐标                     FIELD_SERVICE_FREQWAVE2
*            是否有高精度波形           FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO
*            高精度波形				    FIELD_SERVICE_SEND_FLOAT_WAVE1
#define   SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA       21041
*/
int CTransactData::GetSpecTimeAlarmMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	int nCount=0;
	string sCompany;
	string sFactory;
	string sPlant ,sSuffix,sVibChann,sDynChann,sSinkChann;
	CPlantInfoOffLine *pPlant = ReadCheckCompanyInfo( sCompany, sFactory, sPlant, sSuffix, pFixComm, true);
	if (pPlant ==NULL)
		return -1;
	//得到振动测点
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//得到动态测点
	int iChannNum2   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM2);
	int iBytes2      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE2)+1;
	char *pChannBuff2= new char[iBytes2] ;
	memset(pChannBuff2,0,iBytes2);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF2,pChannBuff2,iBytes2);

	list<string> lVibChann;
	list<string>::iterator itVib;
	list<string> lDynChann;
	list<string>::iterator itDyn;
	//解析振动测点
	lVibChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lVibChann.size();
	//解析动态测点
	lDynChann=ParseChannList(pChannBuff2,iChannNum2);
	iChannNum2=lDynChann.size();
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff  = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iSmpNumsDyn       = pPlant->GetPlantInfo()._iSmpNum;
	int iSmpNumsVib       = iSmpNumsDyn;
	DATATYPE_WAVE *pWaveVib     = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pWaveDyn     = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_WAVE *pXCoor       = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_CHAR *pfWaveVib     = new DATATYPE_CHAR[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	DATATYPE_CHAR *pfWaveDyn     = new DATATYPE_CHAR[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	int iSmpFreqDyn,iSmpFreqVib,iStartIndex=0,iEndIndex=0,iCylinderNo;
	S_DynCharValue dynCharValue;
	S_VibCharValue vibCharValue;
	S_ChannInfo channInfo;
	float    fRev=.1;
	int iRtnDyn = 0, iRtnVib = 0;
	int iRealSmpNums = 0; //一个周期的采样点数

	CWaveHead lsWaveHead;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//振动测点
	for (itVib=lVibChann.begin();itVib!=lVibChann.end();++itVib)
	{
		sVibChann = *itVib;
		iRtnVib     = ReadVibChannSpecAlarmWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sVibChann, pWaveVib,pfWaveVib, iSmpNumsVib,vibCharValue, iSmpFreqVib, fRev, timeOff,iEventID,lsWaveHead);
		channInfo = pPlant->GetSpecVibChannInfo(sVibChann);
		pPlant->GetSpecVibChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		iCylinderNo = pPlant->GetSpecChannCylinderNo(pDB,sVibChann);
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex))|| (iPeriodSmpNums>iSmpNumsVib))
			iRealSmpNums = pPlant->GetPVXCoorData( iSmpFreqVib, iSmpNumsVib, GE_PVTYPE_Pa, fRev,iCylinderNo, pXCoor);
		else
			iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqVib,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
		if ( iRtnVib > 0 )
		{// 说明有振动信号
			++nCount;

			
			if (pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iSmpNumsVib))
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)pWaveVib, iRealSmpNums* sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pXCoor, iRealSmpNums * sizeof(DATATYPE_WAVE));

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1, (char *)pfWaveVib, iRealSmpNums* sizeof(DATATYPE_CHAR));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1, (char *)&(pWaveVib[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1, (char *)&(pfWaveVib[iStartIndex]), (iEndIndex-iStartIndex)* sizeof(DATATYPE_CHAR));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lsWaveHead.m_pParam[0]);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1, channInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealSmpNums );
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreqVib);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	//动态测点
	for (itDyn=lDynChann.begin();itDyn!=lDynChann.end();++itDyn)
	{
		sDynChann=*itDyn;
		iRtnDyn     = ReadDynChannSpecAlarmWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sDynChann, pWaveDyn,pfWaveDyn, iSmpNumsDyn, dynCharValue,iSmpFreqDyn,fRev, timeOff,iEventID,lsWaveHead);
		channInfo = pPlant->GetSpecDynChannInfo(sDynChann);
		if (iEndIndex<1 && iStartIndex<1)//防止周期点数不一致
			pPlant->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex);
		iCylinderNo = pPlant->GetSpecChannCylinderNo(pDB,sDynChann);
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex))|| (iPeriodSmpNums>iSmpNumsDyn))
			iRealSmpNums = pPlant->GetPVXCoorData( iSmpFreqDyn, iSmpNumsDyn, GE_PVTYPE_Pa, fRev,iCylinderNo, pXCoor);
		else
			iRealSmpNums = pPlant->GetPVXCoorData(iSmpFreqDyn,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);

		if ( iRtnDyn >0 )
		{//说明有动态测点信号
			++nCount;
			if (pPlant->IsNotDynPeriodIndex(iStartIndex,iEndIndex))
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pWaveDyn, iRealSmpNums * sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)pXCoor, iRealSmpNums * sizeof(DATATYPE_WAVE));

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1, (char *)pfWaveDyn, iRealSmpNums* sizeof(DATATYPE_CHAR));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pWaveDyn[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE2, (char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1, (char*)&(pfWaveDyn[iStartIndex]), (iEndIndex-iStartIndex)* sizeof(DATATYPE_CHAR));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char*)&dynCharValue, sizeof(S_DynCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_CHANN1, channInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealSmpNums );
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreqDyn);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF, lsWaveHead.m_pParam[0]);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lVibChann.clear();
	lDynChann.clear();
	KD_DeletePoint(pWaveVib);
	KD_DeletePoint(pWaveDyn);
	KD_DeletePoint(pfWaveVib);
	KD_DeletePoint(pfWaveDyn);
	KD_DeletePoint(pXCoor);
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pChannBuff2);
	return nCount;
}

/**往复式系统报警应力监测分析功能，得到指定机组指定报警时间、指定缸号的应力波形和振动测点波形
*输入参数：
*            公司ID        　　　    FIELD_SYSTEM_COMPANY
*		     分厂名          　　　  FIELD_SYSTEM_FACTORY
*            机组ID          　　　  FIELD_SERVICE_PLANTID
*            汽缸号                  FIELD_SERVICE_CYLINDERNO
*            事件ID                  FIELD_SYSTEM_EVENT_ID
*            报警时间                FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*            惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*            活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*            波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
*		     测点名				   FIELD_SYSTEM_CHANN
*		     测点名2			   FIELD_SYSTEM_CHANN2
*		     测点1角度参数1		   FIELD_SERVICE_ANGLEPARAM1
*		     测点1角度参数2		   FIELD_SERVICE_ANGLEPARAM2
*		     测点1角度参数3		   FIELD_SERVICE_ANGLEPARAM3
*		     测点1角度参数4		   FIELD_SERVICE_ANGLEPARAM4
*		     测点2角度参数1		   FIELD_SERVICE_ANGLEPARAM5
*		     测点2角度参数2		   FIELD_SERVICE_ANGLEPARAM6
*		     测点2角度参数3		   FIELD_SERVICE_ANGLEPARAM7
*		     测点2角度参数4		   FIELD_SERVICE_ANGLEPARAM8
*			 时间				   FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE       21040
*/
int CTransactData::GetSpecTimeAlarmDynStressWave( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	int index = -1;
	string sCompany,sFactory,sPlant,sSuffix,sChann1,sChann2,sStressChann;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	int iCylinderNo=pFixComm->GetLong(FIELD_SERVICE_CYLINDERNO);
	string sChannVib=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	//CString strInfo;strInfo.Format("%s,%d",sChannVib.c_str(),iCylinderNo);
	//CHZLogManage::Debug( strInfo, "CTransactData","GetSpecTimeDynStressWave");
	DATATYPE_WAVE *pWave1=NULL;
	DATATYPE_WAVE *pWave2=NULL;
	DATATYPE_WAVE *pRodLoadWave=NULL;
	float *pfStressWave=NULL;
	float *pfInertiaWave=NULL;
	float *pfRodLoadWave=NULL;
	DATATYPE_WAVE *pVibWave=NULL;
	CWaveHead lsWaveHeadStress,lsWaveHeadInert,lsWaveHeadRod;
	if (pPlantInfo->GetDynStressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sStressChann)>0)
	{
		//如果有动态应力测点、则直接从数采获得活塞杆应力波形数据
		//BHTRACE_DEBUG("直接从数采获得活塞杆应力波形数据");
		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0)
			return -2;
		//strInfo.Format("%s,%s",sChann1.c_str(),sChann2.c_str());
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;

		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;
		//strInfo.Format("%f,%f",fAngle1,fAngle2);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1,iRev=0;
		int iStartIndex = 0, iEndIndex = 0;
		S_DynCharValue dynCharValue;
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;
		float fRev =.1;
		iSmpNum=pPlantInfo->GetPlantInfo()._iSmpNum;
		//strInfo.Format("%d,%d",iSmpNum,iSmpFreq);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");
		DATATYPE_WAVE *pWave1=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		DATATYPE_WAVE *pWave2= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		DATATYPE_WAVE *pRodLoadWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfStressWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfInertiaWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfRodLoadWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		DATATYPE_WAVE *pVibWave= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		int iRtn1=ReadDynChannSpecAlarmWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1, pWave1, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,iEventID,lsWaveHeadStress);
		loDynCharValueWriteBack1 = dynCharValue;
		int iRtn2=ReadDynChannSpecAlarmWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann2, pWave2, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,iEventID,lsWaveHeadInert);
		loDynCharValueWriteBack2 = dynCharValue;
		int iRtn3=ReadDynChannSpecAlarmWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sStressChann, pRodLoadWave, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,iEventID,lsWaveHeadRod);
		if (iRtn1>0 && iRtn2>0 && iRtn3)
		{
			//得到活塞杆负荷波形
			pPlantInfo->ConvertDTWaveToFloat(pfRodLoadWave,pRodLoadWave,iSmpNum,lsWaveHeadRod);
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo1._cChannNo,timeOff,iStartIndex,iEndIndex,true);
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
			//计算获得惯性力波形
			if (fAngle1-fAngle2>0)
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			else
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			//strInfo.Format("%d,%d",iPeriodSmpNums,iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			//计算获得的惯性力波形数据由N转化为KN
			for (int nloop=0;nloop<iSmpNum;nloop++)
			{	
				pfInertiaWave[nloop]=pfInertiaWave[nloop]/1000.0;
			}
			
			//计算获得气体压力波形
			for (int nloop=0;nloop<iSmpNum;nloop++)
			{	
				pfStressWave[nloop]=pfRodLoadWave[nloop]-pfInertiaWave[nloop];
			}
			
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
			pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);


			pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());


			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&loDynCharValueWriteBack1,sizeof(S_DynCharValue));
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&loDynCharValueWriteBack2,sizeof(S_DynCharValue));

			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam4);


			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM5,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM6,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM7,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM8,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam4);

			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lsWaveHeadRod.m_pParam[0]);

			pFixComm->Write(pFixComm->GetAci());
			index=1;
		}
	}
	else
	{
		//如果没有动态应力测点、则根据两个动态压力测点计算获得活塞杆应力波形数据
		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0)
			return -2;
		//strInfo.Format("%s,%s",sChann1.c_str(),sChann2.c_str());
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;

		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;
		//strInfo.Format("%f,%f",fAngle1,fAngle2);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1,iRev=0;
		int iStartIndex = 0, iEndIndex = 0;

		S_DynCharValue dynCharValue;
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;

		float fRev =.1;
		iSmpNum=pPlantInfo->GetPlantInfo()._iSmpNum;
		//strInfo.Format("%d,%d",iSmpNum,iSmpFreq);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");
		DATATYPE_WAVE *pWave1=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		DATATYPE_WAVE *pWave2= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfStressWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfInertiaWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		float *pfRodLoadWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		DATATYPE_WAVE *pVibWave= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		int iRtn1=ReadDynChannSpecAlarmWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1, pWave1, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,iEventID,lsWaveHeadStress);
		loDynCharValueWriteBack1 = dynCharValue;
	
		int iRtn2=ReadDynChannSpecAlarmWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann2, pWave2, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,iEventID,lsWaveHeadInert);
		loDynCharValueWriteBack2 = dynCharValue;

		if (iRtn1>0 && iRtn2>0)
		{
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo1._cChannNo,timeOff,iStartIndex,iEndIndex,true);
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
			CString strInfo;strInfo.Format("iStartIndex:%d,%d,%d,%d",iStartIndex,iEndIndex,iPeriodSmpNums,iSmpNum);
			//计算气体压力和惯性力波形
			if (fAngle1-fAngle2>0)
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			else
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			//strInfo.Format("%d,%d",iPeriodSmpNums,iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			//计算获得的气体压力和惯性力波形数据由N转化为KN
			for (int nloop=0;nloop<iSmpNum;nloop++)
			{
				pfInertiaWave[nloop]=pfInertiaWave[nloop]/1000.0;
				pfStressWave[nloop]=pfStressWave[nloop]/1000.0;
			}
			//计算活塞杆负荷波形
			for (int nloop=0;nloop<iSmpNum;nloop++)
				pfRodLoadWave[nloop]=pfStressWave[nloop]+pfInertiaWave[nloop];
			//strInfo.Format("iSmpNum:%d",iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
			pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);

			pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());


			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&loDynCharValueWriteBack1,sizeof(S_DynCharValue));
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&loDynCharValueWriteBack2,sizeof(S_DynCharValue));

			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam4);


			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM5,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM6,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM7,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM8,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam4);

			CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();				
			timeCurr=timeOff;
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lsWaveHeadInert.m_pParam[0]);

			pFixComm->Write(pFixComm->GetAci());
			index=1;
		}
	}
	KD_DeletePoint(pWave1);
	KD_DeletePoint(pWave2);
	KD_DeletePoint(pRodLoadWave);
	KD_DeletePoint(pfStressWave);
	KD_DeletePoint(pfInertiaWave);
	KD_DeletePoint(pfRodLoadWave);
	return index;
}

/*!得到指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名       FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数      FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX    21042
*/
int CTransactData::GetSpecChannSaveTimeSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());

	int lnRecordNum = this->GetSudJilianWaveCount();

	if(lnRecordNum > 400)
	{
		lnRecordNum = 400;
	}
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);

	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	if((iLines>1600)||(iLines<200))
		iLines=401;

	string sTimeSud,sAllField,sTable,sSmpFreq,sSmpNum,sRev,sWave="WAVE_";
	sWave.append(channInfo._cChannNo);
	sAllField = ConsVibChannField( channInfo._cChannNo, true);
	int iSensorType = channInfo._iSensorType;//GetSpecChannSensorType(sCompany,sPlant,sChann,iRevType,pDB);

	int nCount=-1;
	sTable="SUD_";
	sTable.append(sAlias);

	float lnRev = 1;

	int lnRecordCount = 0;

	pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d",
		gc_cEventID,sTable.c_str(),gc_cEventID,iEventID);
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}
	
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
     
	if(lnRecordCount <= lnRecordNum)
	{
		pDB->Cmd("select %s,%s from [%s] ",
			sAllField.c_str(),
			ConsCommonField(),
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d order by [%s] asc",
			gc_cEventID,iEventID,gc_cTime);
	}
	else
	{
		pDB->Cmd("select top 1 %s from %s ",
			gc_cTime,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and [%s] in (%f,%f,%f,%f) order by [%s] asc",
			gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime);
		if(pDB->More())
		{
			pDB->Cmd("select top %d %s,%s from [%s] ",
				lnRecordNum,
				sAllField.c_str(),
				ConsCommonField(),
				sTable.c_str());
			pDB->Cmd(" WHERE [%s]=%d and [%s] in (%f,%f,%f,%f) order by [%s] asc",
				gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime);
		}
		else
		{
			pDB->Cmd("select top %d %s,%s from [%s] ",
				lnRecordNum,
				sAllField.c_str(),
				ConsCommonField(),
				sTable.c_str());
			pDB->Cmd(" WHERE [%s]=%d order by [%s] asc",
				gc_cEventID,iEventID,gc_cTime);
		}
		
	}
	
	while(pDB->More())
	{
		int ifreq,inum,irev,iquest_wavenum,istep=0,istep2=0;
		S_VibCharValue vibCharValue;
		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		inum  = SimADO_GetSampleNumEx(pDB, channInfo);
		ifreq = SimADO_GetSampleFrequencyEx(pDB, channInfo);
		irev  = SimADO_GetRevEx(pDB, channInfo);
		if(irev< 0)
		{
			continue;
		}
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE, vibCharValue._fPhaseOne);
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,  vibCharValue._fCharOne);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,irev);
		DATATYPE_WAVE *pDtWave=NULL,*pDtFreq=NULL;
		float *pfWave=NULL,*pfSpectrum=NULL;
		float df;
		iquest_wavenum=iLines*2.56;
		long isize=0;
		if (inum>0)
		{
			pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pfSpectrum=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtFreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			isize=inum*sizeof(DATATYPE_WAVE);
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, sWave.c_str(), (BYTE *)pDtWave, isize, sWaveHead);
			inum=isize/sizeof(DATATYPE_WAVE);
			pPlantInfo->ConvertDTWaveToFloat(pfWave,pDtWave,inum,sWaveHead);
			
			{//细化谱和正常谱分析 2007.4.7添加
				float fStartFreq=0;
				int iRtn=pPlantInfo->ZoomSpectrumAnalysis(pfWave,inum,ifreq,pfSpectrum,iLines,df,fStartFreq);
				if (iRtn<0)
				{//错误
					iLines=0;
				}
				else
				{
					if (iRtn==2)
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1);
					}
					else
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2.818);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1.414);
					}
				}
			}

			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtFreq,iLines*sizeof(DATATYPE_WAVE));
			//pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
			pFixComm->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iLines);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		}
		else
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,inum);
		}
			
		if(pDtFreq!=NULL) delete [] pDtFreq ;
		if(pfWave!=NULL) delete [] pfWave;
		if(pDtWave!=NULL) delete [] pDtWave;
		if(pfSpectrum!=NULL) delete [] pfSpectrum;
		nCount++;
	
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/*!得到临时在线指定启停车开始时刻和指定机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分别通过转速或保存这组数据的时刻、机组号和测点号
*输入参数字段码
*          公司ID:       FIELD_SYSTEM_COMPANY
*			 分厂名:       FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:        FIELD_SERVICE_OFF_REV
*          时间:        FIELD_SERVICE_OFF_TIME
*          对应时间的微秒         FIELD_SERVICE_MICROSECOND
*          相位：       FIELD_SERVICE_OFF_PHASE
*          幅值：       FIELD_SERVICE_OFF_AMP
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX   21043
*/
int CTransactData::GetSpecTimeChannSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;//公司别名
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime timeSud=CBHDateTime::GetCurrentTime();
	timeSud=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	string sAmp,sPhase,sRev,sTable="SUD_";
	sTable.append(sAlias);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	string sSpecChannAllField = ConsVibChannField( channInfo._cChannNo, false);
	sAmp=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_ONECHAR);
	sPhase=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_PHASECHARONE);
	sRev=ConsVibChannSpecTypeField(channInfo._cChannNo,-5);
	int nCount=-1;

	int lnSudTrendCount = this->GetSudTrendCount();

	int lnRecordType = 1;
	int lnRecordCount = 0;


	
	pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d",
		gc_cEventID,sTable.c_str(),gc_cEventID,iEventID);
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}

	if(lnRecordCount <= lnSudTrendCount)
	{
		lnRecordType = 0;
	}
	else
	{
		pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d and [%s] in(%f,%f,%f,%f)",
			gc_cEventID,sTable.c_str(),gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0);
		if(pDB->More())
		{
			int lnRecordCount = pDB->GetLong("RecordCount");
			if(lnRecordCount > lnSudTrendCount)
			{
				pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d and [%s] in(%f,%f,%f,%f)",
					gc_cEventID,sTable.c_str(),gc_cEventID,iEventID,gc_cRev5,2.0,3.0,6.0,7.0);
				if(pDB->More())
				{
					lnRecordCount = pDB->GetLong("RecordCount");
				}
				if(lnRecordCount > lnSudTrendCount)
				{
					lnRecordType = 4;
				}
				else
				{
					lnRecordType = 2;
				}
			}
			else
			{
				lnRecordType = 1;
			}
		}
	}
	

	switch(lnRecordType)
	{
	case 1:
		pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
			sSpecChannAllField.c_str(),
			sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) ORDER BY [%s],[%s] ",
			gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime,gc_cMicroSecond);
		break;
	case 2:
		pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
			sSpecChannAllField.c_str(),
			sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) ORDER BY [%s],[%s]",
			gc_cEventID,iEventID,gc_cRev5,2.0,3.0,6.0,7.0,gc_cTime,gc_cMicroSecond);
		break;
	case 4:
		pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
			sSpecChannAllField.c_str(),
			sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) ORDER BY [%s],[%s] ",
			gc_cEventID,iEventID,gc_cRev5,4.0,5.0,6.0,7.0,gc_cTime,gc_cMicroSecond);
		break;
	default:
		pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
			sSpecChannAllField.c_str(),
			sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d ORDER BY [%s],[%s] ",gc_cEventID,iEventID,gc_cTime,gc_cMicroSecond);
		break;
	}
	

	S_VibCharValue vibCharValue;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();

	while(pDB->More())
	{
		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		dtTmp=pDB->GetTime(gc_cTime);
		int lnMicroSecond = pDB->GetLong(gc_cMicroSecond);
		dtTmp.SetMillisecond(lnMicroSecond);
		if (dtTmp.GetYear()<2007)//为了防止年份错误引起一些的BUG
		{
			continue;
		}
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtTmp);
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE,pDB->GetDouble(sPhase.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,pDB->GetDouble(sAmp.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetLong(sRev.c_str()));
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
		//CString strTemp;
		//strTemp.Format("振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
		//CHZLogManage::Debug(strTemp,"CTransactData","GetSpecTimeChannSud");
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/*!得到指定启停车事件ID和指定公司、分厂机组、指定测点指定时间段的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分   *别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *			 分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          开始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          结束时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
  *对应时间的微秒        FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  
	#define    SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME          21044
*/

int CTransactData::GetSpecChannSudByTime(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;
	string sAlias;//公司别名
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());

	typedef pair<CBHDateTime,CBHDateTime> pTimePair;
	vector<pTimePair> vTimePair;
	
	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);

	int lnTimePairNum = pFixComm->GetLong(FIELD_SERVICE_OFF_TIMEEND);

	vTimePair.resize(lnTimePairNum);

	pFixComm->GetItemBuf(FIELD_SERVICE_OFF_TIMESTART,(char*)&vTimePair.front(),sizeof(pTimePair)*lnTimePairNum);

	CBHDateTime timeSudStart = CBHDateTime::GetCurrentTime();
	CBHDateTime  timeSudEnd = CBHDateTime::GetCurrentTime();
	//timeSudStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	//timeSudEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	string sAmp,sPhase,sRev,sTable="SUD_";
	sTable.append(sAlias);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	string sSpecChannAllField = ConsVibChannField( channInfo._cChannNo, false);
	sAmp=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_ONECHAR);
	sPhase=ConsVibChannSpecTypeField(channInfo._cChannNo,GE_PHASECHARONE);
	sRev=ConsVibChannSpecTypeField(channInfo._cChannNo,-5);
	int nCount=-1;

	int lnRecordCount = 0;


	int iRecordNum  = this->GetSudTrendCount();

	if(vTimePair.size() > 1)
	{
		iRecordNum = iRecordNum/2;
	}
	
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	for(unsigned int i = 0; i < vTimePair.size(); i++)
	{
		timeSudStart = vTimePair[i].first;
		timeSudEnd = vTimePair[i].second;

		pDB->Cmd("select count([%s]) as RecordCount from [%s] ",gc_cEventID,sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and [%s] between '%s' and '%s'",
			gc_cEventID,iEventID,gc_cTime,timeSudStart.Format(),timeSudEnd.Format());
		if(pDB->More())
		{
			lnRecordCount = pDB->GetLong("RecordCount");
		}

		if(lnRecordCount <= iRecordNum)
		{
			pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
				sSpecChannAllField.c_str(),
				sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
			pDB->Cmd(" WHERE [%s]=%d and [%s] between '%s' and '%s' ORDER BY [%s],[%s] ",
				gc_cEventID,iEventID,gc_cTime,timeSudStart.Format(),timeSudEnd.Format(),gc_cTime,gc_cMicroSecond);
		}
		else
		{
			int lnRecordType = 1;

			pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d \
					 and [%s] in(%f,%f,%f,%f) and [%s] between '%s' and '%s'",
					 gc_cEventID,sTable.c_str(),gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,\
					 gc_cTime,timeSudStart.Format(),timeSudEnd.Format());
			if(pDB->More())
			{
				lnRecordCount = pDB->GetLong("RecordCount");
				if(lnRecordCount > iRecordNum)
				{
					pDB->Cmd("select count([%s]) as RecordCount from [%s] where [%s] = %d \
							 and [%s] in(%f,%f,%f,%f) and [%s] between '%s' and '%s'",
							 gc_cEventID,sTable.c_str(),gc_cEventID,iEventID,gc_cRev5,2.0,3.0,6.0,7.0,\
							 gc_cTime,timeSudStart.Format(),timeSudEnd.Format());
					if(pDB->More())
					{
						lnRecordCount = pDB->GetLong("RecordCount");
					}
					if(lnRecordCount > iRecordNum)
					{
						lnRecordType = 4;
					}
					else
					{
						lnRecordType = 2;
					}
				}
				else
				{
					lnRecordType = 1;
				}
			}

			switch(lnRecordType)
			{
			case 1:
				pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
					sSpecChannAllField.c_str(),
					sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
				pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) and [%s] between '%s' and '%s' ORDER BY [%s],[%s] ",
					gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime,timeSudStart.Format(),
					timeSudEnd.Format(),gc_cTime,gc_cMicroSecond);
				break;
			case 2:
				pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
					sSpecChannAllField.c_str(),
					sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
				pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) and [%s] between '%s' and '%s' ORDER BY [%s],[%s]",
					gc_cEventID,iEventID,gc_cRev5,2.0,3.0,6.0,7.0,gc_cTime,timeSudStart.Format(),
					timeSudEnd.Format(),gc_cTime,gc_cMicroSecond);
				break;
			case 4:
				pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
					sSpecChannAllField.c_str(),
					sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
				pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) and [%s] between '%s' and '%s' ORDER BY [%s],[%s]",
					gc_cEventID,iEventID,gc_cRev5,4.0,5.0,6.0,7.0,gc_cTime,timeSudStart.Format(),
					timeSudEnd.Format(),gc_cTime,gc_cMicroSecond);
				break;
			default:
				pDB->Cmd("select %s,[%s],[%s],%s,[%s],%s from [%s] ",
					sSpecChannAllField.c_str(),
					sAmp.c_str(),sPhase.c_str(),ConsCommonField(),gc_cTime,gc_cMicroSecond,sTable.c_str());
				pDB->Cmd(" WHERE [%s]=%d  and [%s] in (%f,%f,%f,%f) and [%s] between '%s' and '%s' ORDER BY [%s],[%s]",
					gc_cEventID,iEventID,gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime,timeSudStart.Format(),
					timeSudEnd.Format(),gc_cTime,gc_cMicroSecond);
				break;
			}
		}

		S_VibCharValue vibCharValue;

		CBHDateTime dtTmp=CBHDateTime::GetCurrentTime();
		while(pDB->More())
		{
			vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
			dtTmp=pDB->GetTime(gc_cTime);
			int lnMicroSecond = pDB->GetLong(gc_cMicroSecond);
			dtTmp.SetMillisecond(lnMicroSecond);
			if (dtTmp.GetYear()<2007)//为了防止年份错误引起一些的BUG
				continue;
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtTmp);
			pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE,pDB->GetDouble(sPhase.c_str()));
			pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,pDB->GetDouble(sAmp.c_str()));
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,pDB->GetLong(sRev.c_str()));
			pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
			//CString strTemp;
			//strTemp.Format("振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
			//CHZLogManage::Debug(strTemp,"CTransactData","GetSpecTimeChannSud");
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			nCount++;
		}
	}
    
	
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定时间的月、公司、分厂、机组各个测点的各种特征值最大值最小值平均值，测点特征值buff中依次是最大值、最小值和平均值，特征值个数应该是3
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          时间值               FIELD_SERVICE_OFF_TIME
*返回值的字段：
*          测点特征值Buff      FIELD_SERVICE_CHARVALUE_SBUFF
*          特征值个数          FIELD_SERVICE_CHANN_NUM
*          测点ID              FIELD_SERVICE_CHANN1
#define SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT        21051
*/
int CTransactData::GetSpecPlantMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount=-1;
	string sCompany,sFactory,sPlant,sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if( pPlantInfo == NULL )
		return -1;
	CBHDateTime dtSpec=CBHDateTime::GetCurrentTime();
	dtSpec=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	string sTable="TREND_VIBMONTH_";
	sTable.append(sSuffix);
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
	int iMonth=dtSpec.GetMonth();
	dtStart.SetDateTime(dtSpec.GetYear(),iMonth,1,0,0,0);
	if (iMonth==12)
		dtEnd.SetDateTime(dtSpec.GetYear()+1,1,1,0,0,0);
	else
		dtEnd.SetDateTime(dtSpec.GetYear(),dtSpec.GetMonth()+1,1,0,0,0);
	//得到所有测点
	list<string> lChann;
	list<string>::iterator itChann;
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT %s FROM [%s] ",gc_cChannNo,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo, pPlantInfo->GetPlantNo().c_str(),gc_cChannType,GE_VIBCHANN);
	while (pDB->More())
		lChann.push_back(pDB->Get(gc_cChannNo));
	if (lChann.size()==0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}
	//得到数据
	string sShowField="";
	itChann=lChann.begin();
	sShowField=ConsVibChannField(*itChann);
	++itChann;
	for (;itChann!=lChann.end();++itChann)
	{
		if ((*itChann).c_str()=="")
			continue;
		sShowField.append(",");
		sShowField.append(ConsVibChannField(*itChann));
	}
	sShowField.append(",");
	sShowField.append(ConsCommonField().GetString());
	int iChannNum=lChann.size();
	int nloop=0;
	S_VibCharValue **ppVibCharValue=new S_VibCharValue*[iChannNum];
	for (nloop=0;nloop<iChannNum;++nloop)
		ppVibCharValue[nloop]=new S_VibCharValue[3];
	S_VibCharValue vibValue;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	pDB->Cmd("Select %s from [%s] ",sShowField.c_str(),sTable.c_str());
	pDB->Cmd(" WHERE [%s] between '%s' AND '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), CPlantInfoBase::TimeConvertString(dtEnd));
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	nCount=0;
	if(pDB->More())
	{
		nCount++;
		nloop=0;
		for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		{
			vibValue=ReadVibCharValue((*itChann),pDB);
			ppVibCharValue[nloop][0]=vibValue;//最大值
			ppVibCharValue[nloop][1]=vibValue;//最小值
			ppVibCharValue[nloop][2]=vibValue;//平均值
			nloop++;
		}
		while (pDB->More())
		{
			nloop=0;
			for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
			{
				vibValue=ReadVibCharValue((*itChann),pDB);
				ppVibCharValue[nloop][0]=MaxVibCharValue(ppVibCharValue[nloop][0],vibValue);//最大值
				ppVibCharValue[nloop][1]=MinVibCharValue(ppVibCharValue[nloop][1],vibValue);//最小值
				ppVibCharValue[nloop][2]=SumVibCharValue(ppVibCharValue[nloop][2], vibValue);//平均值
				nloop++;
			}
			nCount++;
		}
		nloop=0;
		for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		{
			ppVibCharValue[nloop][2]._fCharAll=(ppVibCharValue[nloop][2]._fCharAll)/nCount;
			ppVibCharValue[nloop][2]._fCharOne=(ppVibCharValue[nloop][2]._fCharOne)/nCount;
			ppVibCharValue[nloop][2]._fCharTwo=(ppVibCharValue[nloop][2]._fCharTwo)/nCount;
			ppVibCharValue[nloop][2]._fCharThree=(ppVibCharValue[nloop][2]._fCharThree)/nCount;
			ppVibCharValue[nloop][2]._fCharHalf=(ppVibCharValue[nloop][2]._fCharHalf)/nCount;
			ppVibCharValue[nloop][2]._fCharRes=(ppVibCharValue[nloop][2]._fCharRes)/nCount;
			ppVibCharValue[nloop][2]._fGap=(ppVibCharValue[nloop][2]._fGap)/nCount;
			ppVibCharValue[nloop][2]._fDC=(ppVibCharValue[nloop][2]._fDC)/nCount;
			ppVibCharValue[nloop][2]._fPhaseOne=(ppVibCharValue[nloop][2]._fPhaseOne)/nCount;
			ppVibCharValue[nloop][2]._fPhaseTwo=(ppVibCharValue[nloop][2]._fPhaseTwo)/nCount;
			ppVibCharValue[nloop][2]._fPhaseThree=(ppVibCharValue[nloop][2]._fPhaseThree)/nCount;
			ppVibCharValue[nloop][2]._fPhaseHalf=(ppVibCharValue[nloop][2]._fPhaseHalf)/nCount;
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)ppVibCharValue[nloop],3*sizeof(S_VibCharValue));
			pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,3);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			nloop++;
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	for (nloop=0;nloop<iChannNum;++nloop)
		delete [] ppVibCharValue[nloop];
	delete [] ppVibCharValue;
	lChann.clear();
	return nCount;
}

/**得到指定公司、分厂、指定时间范围的报警信息、启停车信息。
*振动通道报警次数、过程量通道报警、累计停车次数、累计停机时间、累计运行时间，时间单位为小时
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
*返回的字段：
*          机组ID               FIELD_SERVICE_PLANTID
*          所有测点报警次数     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*          累计停车次数         FIELD_SERVICE_LEN
*          累计停机时间(小时)   FIELD_SERVICE_SAMPLE_FREQ
*          累计运行时间(小时)   FIELD_SERVICE_SAMPLE_NUM
#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052
*/
int CTransactData::GetSpecTimeRangeFactoryRunInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);

	CBHDateTime dtStart = CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd   = CBHDateTime::GetCurrentTime();
	dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	dtEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	list<string> lPlant;
	list<string>::iterator itPlant;
	if(dtEnd>(CBHDateTime::GetCurrentTime()))
		dtEnd=CBHDateTime::GetCurrentTime();

	CBHDateTime loTempdtStart = dtStart - COleDateTimeSpan(0,3,0,0);
	CBHDateTime loTempdtEnd = dtEnd + COleDateTimeSpan(0,3,0,0);
	//累计停车次数
	int nStopCount = 0;

	//得到指定公司分厂下所有设备
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT %s FROM [%s] ",gc_cPlantID,gc_cMachineTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());
	while(pDB->More())
		lPlant.push_back(pDB->Get(gc_cPlantID));

	string sSuffix,sTable;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itPlant=lPlant.begin();itPlant!=lPlant.end();++itPlant)
	{
		if (CheckCompanyInfo(sCompany,sFactory,(*itPlant),sSuffix,pFixComm,false)!=NULL)
		{
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());

			/* 切换至报警据表所在数据库 */
			sTable="ALARM_LOG_";
			sTable.append(sSuffix);
			SwitchDbNameEx(pDB,sSuffix.c_str());

			/* 得到所有测点报警次数
			 * - 根据开始时间查*/
			pDB->Cmd("SELECT count(distinct(%s)) from [%s] ",gc_cEventID,sTable.c_str());
			pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",
					gc_cStart_T, CPlantInfoBase::TimeConvertString(dtStart), CPlantInfoBase::TimeConvertString(dtEnd));
			if (pDB->More())
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,pDB->GetLong(1));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,-1);
			}


			//得到累计停车次数
			sTable="TREND_VIBMONTH_";
			sTable.append(sSuffix);
			list<pair<CBHDateTime,int> >time_rev_table;
			list <pair<CBHDateTime,int> > ::iterator it;

			int lnDataCount = 0;

			const int DATA_COUNT_MONTH = 240;

			pDB->Cmd(" SELECT count(*) AS DataNum from [%s] ",sTable.c_str());

			pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(loTempdtStart), CPlantInfoBase::TimeConvertString(loTempdtEnd));

			if (pDB->More())
			{
				lnDataCount = pDB->GetLong("DataNum");
			}

			if (lnDataCount > DATA_COUNT_MONTH * 2)//如果超过月表理论数据量，则发生了快变
			{
				pDB->Cmd(" SELECT %s from [%s]",gc_cTime,sTable.c_str());
				pDB->Cmd(" WHERE ([%s] between '%s' and '%s') ",gc_cTime, CPlantInfoBase::TimeConvertString(loTempdtStart), CPlantInfoBase::TimeConvertString(loTempdtEnd));
				pDB->Cmd(" and %s < 10 ", gc_cRev);
				pDB->Cmd(" order by [%s] ", gc_cTime);
				
				vector<CBHDateTime> time_all_stop_data;
				while(pDB->More())
				{
					time_all_stop_data.push_back(pDB->GetTime(gc_cTime));
				}

				CBHDateTime ldtDataStartTime,ldtDataEndTime;
				//查询当月数据开始时间
				pDB->Cmd(" SELECT top 1 %s from [%s]",gc_cTime,sTable.c_str());
				pDB->Cmd(" WHERE ([%s] between '%s' and '%s') ",gc_cTime, CPlantInfoBase::TimeConvertString(loTempdtStart), CPlantInfoBase::TimeConvertString(loTempdtEnd));
				pDB->Cmd(" order by [%s] asc", gc_cTime);

				if (pDB->More())
				{
					ldtDataStartTime = pDB->GetTime(gc_cTime);
				}

				if (ldtDataStartTime < dtStart)
				{
					ldtDataStartTime = dtStart;
				}

				pDB->Cmd(" SELECT top 1 %s from [%s]",gc_cTime,sTable.c_str());
				pDB->Cmd(" WHERE ([%s] between '%s' and '%s') ",gc_cTime, CPlantInfoBase::TimeConvertString(loTempdtStart), CPlantInfoBase::TimeConvertString(loTempdtEnd));
				pDB->Cmd(" order by [%s] desc", gc_cTime);

				if (pDB->More())
				{
					ldtDataEndTime = pDB->GetTime(gc_cTime);
				}

				if (ldtDataEndTime > dtEnd)
				{
					ldtDataEndTime = dtEnd;
				}
				
				CString lstrStart = ldtDataStartTime.Format();
				CString lstrEnd = ldtDataEndTime.Format();

				COleDateTimeSpan ldtSpan = ldtDataEndTime-ldtDataStartTime;

				int lnStopInterval = 4 * 60;// 月表数据间隔是3小时，超过4个小时则认为是一次停车

				int lnTotalHours = ldtSpan.GetTotalHours();

				int lnTotalMinites = ldtSpan.GetTotalMinutes();

				float lfTotalHour = (float)lnTotalMinites/60.0;

				if (lfTotalHour - (float)lnTotalHours  > 0.5)
				{
					lnTotalHours += 1;
				}

				int lnStopCount = 0;
				int lnStopHours = 0;
				int lnRunHours = 0;
				int lnStopMinites = 0;

				if (time_all_stop_data.size() == 0)
				{
					pFixComm->SetItem(FIELD_SERVICE_LEN, lnStopCount);//累计停车次数
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, lnStopHours); //累计停车时间
					lnRunHours = lnTotalHours;
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM,  lnRunHours);
				}
				else
				{
					COleDateTimeSpan dtStopSpan = COleDateTimeSpan(0);

					CBHDateTime ldtLast,ldtCurrent,ldtNext;
					
					vector<CBHDateTime>::iterator lpIter = time_all_stop_data.begin();

					ldtLast = *lpIter;
					
					++lpIter;
					
					while(lpIter != time_all_stop_data.end())
					{
						ldtCurrent = *lpIter;

						CString lstrLast = ldtLast.Format();
						CString lstrCur = ldtCurrent.Format();
						
						dtStopSpan = ldtCurrent - ldtLast;

						int lnStopTimeDec = dtStopSpan.GetTotalMinutes();
						
						if (lnStopTimeDec > lnStopInterval)
						{
							++lnStopCount;
						}
						else
						{
							lnStopMinites+=lnStopTimeDec;
						}

						ldtLast = ldtCurrent;

						++lpIter;
					}


					float lfStopHour = (float)lnStopMinites/60.0;

					lnStopHours = (int)lfStopHour;

					float lfTempStop = lfStopHour - (float)lnStopHours;

					if (lfStopHour - (float)lnStopHours  > 0.5)
					{
						lnStopHours += 1;
					}

					pFixComm->SetItem(FIELD_SERVICE_LEN, lnStopCount);//累计停车次数
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, lnStopHours); //累计停车时间
					lnRunHours = lnTotalHours - lnStopHours;
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM,  lnRunHours);
				}

			}
			else
			{
				pDB->Cmd(" SELECT %s,%s from [%s] ",gc_cTime,gc_cRev,sTable.c_str());
				pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(loTempdtStart), CPlantInfoBase::TimeConvertString(loTempdtEnd));
				pDB->Cmd(" order by [%s] ", gc_cTime);
				while (pDB->More()) {
					time_rev_table.push_back(make_pair(pDB->GetTime(gc_cTime), pDB->GetLong(gc_cRev)));
				}

				int iLastRev = -1;

				CBHDateTime dtLastStop      = CBHDateTime(1999,1,1,1,1,1);
				CBHDateTime dtNextStart     = CBHDateTime(1999,1,1,1,1,1);

				CBHDateTime dtLastDate     = CBHDateTime();
				CBHDateTime loDtInitValue = CBHDateTime();

				COleDateTimeSpan dtStopSpan = COleDateTimeSpan(0);

				//起止点前后各多取三个小时，判断之前的数据转速是否小于10，以此来确定时间间隔
				list <pair<CBHDateTime,int> > ::iterator lpItPre = time_rev_table.begin();

				CBHDateTime lotemp;

				COleDateTimeSpan lodtSpanPre = COleDateTimeSpan(0);


				/* 得到累计运行时间和停机时间*/
				CBHDateTime lodtStart = CBHDateTime::GetCurrentTime();

				CBHDateTime lodtEnd   = CBHDateTime::GetCurrentTime();

				bool lbDataLastMonth = false;

				while(lpItPre != time_rev_table.end())
				{
					lotemp = lpItPre->first;

					if (lotemp < dtStart)
					{
						lodtStart = dtStart;

						lbDataLastMonth = true;
					}
					else
					{
						if (!lbDataLastMonth)
						{
							lodtStart = lpItPre->first;
						}

						break;
					}

					++lpItPre;
				}

				list <pair<CBHDateTime,int> > ::reverse_iterator lpItTail = time_rev_table.rbegin();

				bool lbDataNextMonth = false;

				while(lpItTail != time_rev_table.rend())
				{
					lotemp = lpItTail->first;

					if (lotemp > dtEnd)
					{
						lodtEnd = dtEnd;

						lbDataNextMonth = true;
					}
					else
					{
						if (!lbDataNextMonth)
						{
							lodtEnd = lpItTail->first;
						}

						break;
					}

					++lpItTail;
				}


				nStopCount=0;

				bool lbFindAll = false;
				if(time_rev_table.size() > 0) {
					it = time_rev_table.begin();
					dtLastDate = it->first;
					iLastRev   = it->second;
					++it;

					for (; it != time_rev_table.end();++it) 
					{
						if (lbFindAll)
						{
							break;
						}

						if (dtLastDate < dtStart)
						{
							dtLastDate = dtStart;
							iLastRev   = it->second;
							continue;
						}

						if (iLastRev > 10 && it->second < 10) 
						{
							++nStopCount;                           // 累计停车次数
						}

						if (iLastRev < 10 && it->second < 10) 
						{  
							if (dtLastDate>loDtInitValue)
							{
								if ( it->first > dtEnd)
								{
									it->first = dtEnd;
									lbFindAll =true;
								}
								dtStopSpan  += (it->first - dtLastDate);  // 累计停车时间
							}					
						}

						dtLastDate = it->first;
						iLastRev   = it->second;
					}

					pFixComm->SetItem(FIELD_SERVICE_LEN, nStopCount);

					//累计总时间
					COleDateTimeSpan dtTotalSpan = (lodtEnd - lodtStart);

					int iTotalHour = dtTotalSpan.GetTotalHours();

					int iTotalMinites = dtTotalSpan.GetTotalMinutes();

					float lfTotalHour = (float)iTotalMinites/60.0;

					if (lfTotalHour - (float)iTotalHour  > 0.5)
					{
						iTotalHour += 1;
					}

					//累计停机时间
					int lnStopMinites = dtStopSpan.GetTotalMinutes();
					
					float lfStopHour = (float)lnStopMinites/60.0;

					int iStopHours = lfStopHour;

					float lfTempStop = lfStopHour - (float)iStopHours;

					if (lfStopHour - (float)iStopHours  > 0.5)
					{
						iStopHours += 1;
					}

					int iRunHours  = iTotalHour - iStopHours; //累计开车时间

					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iStopHours);
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM,  iRunHours);
				}
				else {

					/* 无数据时运行时间，运行次数均显示0 */
					pFixComm->SetItem(FIELD_SERVICE_LEN, 0);
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, 0);
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM,  0);
				}
			}

			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();

			time_rev_table.clear();
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lPlant.clear();

	return nStopCount;
}

/**关联诊断。这个报警ID的开始截止时间范围内，返回的记录只有一条，0表示没有，1表示有。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*  	       分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点ID   FIELD_SERVICE_CHANN1
*          事件ID   FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          同转速测点是否报警               FIELD_SERVICE_OFF_REV
*          同转速测点是否存在               FIELD_SERVICE_OFF_REV2
*          同转速测点是否存在残振或通频报警 FIELD_SERVICE_TREND_RESCHAR
*          对应测点是否报警          FIELD_SERVICE_REV
*          对应测点是否存在          FIELD_SERVICE_REV2
*          对应测点是否存在残振或通频报警   FIELD_SERVICE_TREND_DC
#define SERVICE_SERVICE_OFF_CORRDIGN                21053
*/
int CTransactData::CorrDign(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sSuffix;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	string sChann=pFixComm->Get(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	list<string> lChann;
	string sCorrChann;
	string sAlarmLogTblName="ALARM_LOG_";
	sAlarmLogTblName.append(sSuffix);
	int iRevType=-1;
	int iType=0;
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	sCorrChann=pPlantInfo->GetSpecChannAxisChann(sChann);
	if (sCorrChann.empty())
		pFixComm->SetItem(FIELD_SERVICE_REV2,0);
	else
	{
		pFixComm->SetItem(FIELD_SERVICE_REV2,1);
		//进行相关查询
		pDB->Cmd("Select [%s] from [%s]",gc_cType,sAlarmLogTblName.c_str());
		pDB->Cmd(" Where [%s]=%d and [%s]='%s'",gc_cEventID,iEventID,gc_cName,sCorrChann.c_str());
		if (pDB->More())
		{
			pFixComm->SetItem(FIELD_SERVICE_REV,1);
			iType=pDB->GetLong(gc_cType);
			if ((iType&D_BHKD_AT_CHARALL)||((iType&D_BHKD_AT_CHARRES)))
				pFixComm->SetItem(FIELD_SERVICE_TREND_DC,1);
			else
				pFixComm->SetItem(FIELD_SERVICE_TREND_DC,0);
		}
		else
			pFixComm->SetItem(FIELD_SERVICE_REV,0);
	}
	lChann=pPlantInfo->GetSpecChannSameRevChanns(sChann,iRevType);
	list<string>::iterator it;
	if (lChann.empty())
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,0);
	else
	{
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,1);
		//进行同转速测点的相关查询
		string sChannConn;
		it=lChann.begin();
		sChannConn=*it;
		++it;
		for (;it!=lChann.end();++it)
		{
			sChannConn.append(",");
			sChannConn.append(*it);
		}
		pDB->Cmd("Select [%s] FROM [%s] ",gc_cType,sAlarmLogTblName.c_str());
		pDB->Cmd(" where [%s]=%d AND [%s] in (%s)",gc_cEventID,iEventID,gc_cName,sChannConn.c_str());
		if (pDB->More())
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,1);
			iRevType=0;
			do
			{
				iType=pDB->GetLong(gc_cType);
				if ((iType&D_BHKD_AT_CHARALL)||((iType&D_BHKD_AT_CHARRES)))
				{
					iRevType=1;
					break;
				}

			} while (pDB->More());
			pFixComm->SetItem(FIELD_SERVICE_TREND_RESCHAR,iRevType);
		}
		else
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,0);
	}
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	return 1;
}

/**集团级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     设备数           FIELD_SERVICE_PLANT_NUM
*		     报警设备数       FIELD_SERVICE_ALARM_PLANTNUM
#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT             21054
*/
int CTransactData::GetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0,iPlantNum,iAlarmPlantNum;
	CString strTable;
	string sCompany,sFactory,sPlant,sAlias,sTable,sSuffix;
	sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	CPlantInfoOffLine *pPlant = NULL;
	map<string,string> mPlant;
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cPlantID, gc_cFactory_Name,gc_cMachineTable);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (sFactory.empty())
		pDB->Cmd(" WHERE [%s]='%s' ",gc_cCompany,sCompany.c_str());
	else
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());

	//得到分厂和设备列表
	iPlantNum = 0;
	while (pDB->More())
	{
		sFactory = pDB->Get(gc_cFactory_Name);
		mPlant.insert(make_pair(pDB->Get(gc_cPlantID), sFactory));
		++index;
	}
	iPlantNum = index;
	iAlarmPlantNum = 0;
	map<string,string>::iterator it;
	for ( it =mPlant.begin(); it != mPlant.end(); ++it)
	{
		pPlant = CheckCompanyInfo(sCompany,it->second,it->first,sSuffix,NULL,false);
		if (pPlant != NULL)
		{

			sTable = pPlant->GetAlarmLogTblName();
			SwitchDbNameEx(pDB,sSuffix.c_str());
			pDB->Cmd("SELECT distinct %s FROM [%s]",gc_cEventID,sTable.c_str());
			pDB->Cmd(" WHERE %s between '%s' AND '%s' ",gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
			if(pDB->More())
				++iAlarmPlantNum;
		}
	}
	//CString sTemp;sTemp.Format("分厂为空：设备数:%d,报警设备数:%d",iPlantNum,iAlarmPlantNum);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetGroupAlarmStat");
	pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,iPlantNum);
	pFixComm->SetItem(FIELD_SERVICE_ALARM_PLANTNUM,iAlarmPlantNum);
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**公司级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            分厂名           FIELD_SYSTEM_FACTORY
*		     设备名           FIELD_SERVICE_PLANTID
*		     报警次数         FIELD_SERVICE_ALARM_PLANTNUM
*		     起停车次数       FIELD_SERVICE_STOP_PLANTNUM

#define    SERVICE_SERVICE_OFF_COMPANYALARMSTAT           21055
*/
int CTransactData::GetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0,iPlantNum,iAlarmNum,iStopNum;
	string sCompany,sFactory,sTable,sSuffix;
	sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	CPlantInfoOffLine *pPlant = NULL;
	map<string,string> mPlant;
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cPlantID, gc_cFactory_Name,gc_cMachineTable);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (sFactory.empty())
		pDB->Cmd(" WHERE [%s]='%s' ",gc_cCompany,sCompany.c_str());
	else
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());

	//得到分厂和设备列表
	iPlantNum = 0;
	while (pDB->More())
	{
		sFactory = pDB->Get(gc_cFactory_Name);
		mPlant.insert(make_pair(pDB->Get(gc_cPlantID), sFactory));
		++index;
	}
	iPlantNum = index;
	map<string,string>::iterator it;
	for ( it =mPlant.begin(); it != mPlant.end(); ++it)
	{
		pPlant = CheckCompanyInfo(sCompany,it->second,it->first,sSuffix,NULL,false);
		if (pPlant != NULL)
		{
			iAlarmNum = 0;
			iStopNum  = 0;
			SwitchDbNameEx(pDB,sSuffix.c_str());
			sTable = pPlant->GetAlarmLogTblName();
			pDB->Cmd("SELECT count(distinct(%s)) from [%s] ",gc_cEventID,sTable.c_str());
			pDB->Cmd(" WHERE %s between '%s' AND '%s' ",gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
			if (pDB->More())
				iAlarmNum = pDB->GetLong(1);

			sTable = pPlant->GetSudLogTblName();
			pDB->Cmd("SELECT count(distinct(%s)) from [%s] ",gc_cEventID,sTable.c_str());
			pDB->Cmd(" WHERE %s between '%s' AND '%s' ",gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
			if (pDB->More())
				iStopNum = pDB->GetLong(1);

			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,it->second.c_str());
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,it->first.c_str());
			pFixComm->SetItem(FIELD_SERVICE_ALARM_PLANTNUM,iAlarmNum);
			pFixComm->SetItem(FIELD_SERVICE_STOP_PLANTNUM,iStopNum);
			//CString sTemp;sTemp.Format("表名:%s,设备名:%s,报警次数:%d,停车次数:%d",sTable.c_str(),it->first.c_str(),iAlarmNum,iStopNum);
			//CHZLogManage::Debug(sTemp,"CTransactData","GetCompanyAlarmStat");
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**监测站级历史报警ID列表
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
			 

*返回值的字段码：
			报警ID						FIELD_SERVICE_ALARMID
			报警ID个数					FIELD_SERVICE_ALARMID_NUM
			报警ID对应的报警通道数		FIELD_SERVICE_ALARMID_CHANN_NUM
			接口数据上限				FIELD_SERVICE_DATALIMIT

#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21076
*/
int CTransactData::GetAlarmIDS(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sChann,sAlias,sCharValueField;
	CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_ChannInfo channInfo;
	S_AlarmlogInfo AlarmlogInfo;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	list<S_AlarmlogInfo> EventID;
	list<S_AlarmlogInfo>::iterator it;
	sTable = pPlantInfo->GetAlarmLogTblName();

	CString lstrEventIDArray;

	int lnRecordCount = 0,lnInterval = 0;

	int D_TRENDRECORDS = 300;

	pDB->Cmd("select count(distinct %s) as RecordCount from %s where %s between '%s' and '%s'",
		gc_cEventID,sTable.c_str(),gc_cStart_T,
		CPlantInfoBase::TimeConvertString(timeStart),
		CPlantInfoBase::TimeConvertString(timeEnd));
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}

	lnInterval = lnRecordCount/D_TRENDRECORDS;

	int lnEventID = 0;

	vector<int> lvEventID;

	if(lnInterval > 0)
	{
		pDB->Cmd("select top 1 %s from %s where %s between '%s' and '%s' order by %s",
			gc_cEventID,sTable.c_str(),gc_cStart_T,
			CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd),gc_cStart_T);
		if(pDB->More())
		{
			lnEventID = pDB->GetLong(gc_cEventID);
		}
		pDB->Cmd("select distinct %s from %s where (%s-(%d))%%%d = 0 and (%s between '%s' and '%s')",\
			gc_cEventID,sTable.c_str(),gc_cEventID,lnEventID,lnInterval,
			gc_cStart_T,CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd));
		while(pDB->More())
		{
			lvEventID.push_back(pDB->GetLong(gc_cEventID));
		}

		if(lvEventID.size() > 0)
		{
			CString lstrTmp;
			lstrTmp.Format("%d",lvEventID[0]);
			lstrEventIDArray += lstrTmp;
			for(unsigned int i = 0; i < lvEventID.size(); i++)
			{
				lstrTmp.Format(",%d",lvEventID[i]);
				lstrEventIDArray += lstrTmp;
			}
		}

		pDB->Cmd("SELECT COUNT([%s]) AS [%s], [%s] FROM [%s] ",gc_cName,gc_cChannNo, gc_cEventID ,sTable.c_str());
		pDB->Cmd(" WHERE %s in(%s) GROUP BY [%s]",gc_cEventID, 
			lstrEventIDArray,
			gc_cEventID);

	}
	else
	{
		pDB->Cmd("SELECT COUNT([%s]) AS [%s], [%s] FROM [%s] ",gc_cName,gc_cChannNo, gc_cEventID ,sTable.c_str());
		pDB->Cmd(" WHERE %s BETWEEN '%s' AND '%s' GROUP BY [%s]",gc_cStart_T, 
			CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd),
			gc_cEventID);
	}

	std::vector<int> loIds;
	std::vector<int> loChannCounts;

	int lnDataLimit = (FIELD_SYSTEM_MAX_TRANSFER_DATA_LIMIT_IN_BYTE);

	while (pDB->More())
	{
		AlarmlogInfo._iAlarmID = pDB->GetLong(gc_cEventID);
		EventID.push_back(AlarmlogInfo);
		loIds.push_back(AlarmlogInfo._iAlarmID);
		loChannCounts.push_back(pDB->GetLong(gc_cChannNo));
		if (loIds.size()*sizeof(int)>=lnDataLimit)
		{
			break;
		}
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	int lnDataSize = loIds.size();
	pFixComm->SetItemBuf(FIELD_SERVICE_ALARMID,(char *)&loIds.front(),lnDataSize*sizeof(int));	
	pFixComm->SetItemBuf(FIELD_SERVICE_ALARMID_CHANN_NUM,(char *)&loChannCounts.front(),lnDataSize*sizeof(int));	
	pFixComm->SetItem(FIELD_SERVICE_DATALIMIT,FIELD_SYSTEM_MAX_TRANSFER_DATA_LIMIT_IN_NUM);
	pFixComm->SetItem(FIELD_SERVICE_ALARMID_NUM,lnDataSize);
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return EventID.size();
}

/**监测站级历史报警ID列表
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND


*返回值的字段码：
			报警ID		              FIELD_SERVICE_ALARMID
			报警ID对应的测点名        FIELD_SERVICE_CHANN1
			报警ID对应的时间          FIELD_SERVICE_OFF_TIMESTART
			报警类型                  FIELD_SERVICE_ALARM_TYPE


#define     SERVICE_SERVICE_OFF_GETALARMIDSEX    21087
*/
int CTransactData::GetAlarmIDSEx(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sChann,sAlias,sCharValueField,sAllCharValueField;
	CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_AlarmlogInfo AlarmlogInfo;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	sTable = pPlantInfo->GetAlarmLogTblName();

	CString lstrEventIDArray;

	int lnRecordCount = 0,lnInterval = 0;

	int D_TRENDRECORDS = 300;

	pDB->Cmd("select count(distinct %s) as RecordCount from %s where %s between '%s' and '%s'",
		gc_cEventID,sTable.c_str(),gc_cStart_T,
		CPlantInfoBase::TimeConvertString(timeStart),
		CPlantInfoBase::TimeConvertString(timeEnd));
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}

	lnInterval = lnRecordCount/D_TRENDRECORDS;

	int lnEventID = 0;

	vector<int> lvEventID;

	if(lnInterval > 0)
	{
		pDB->Cmd("select top 1 %s from %s where %s between '%s' and '%s' order by %s",
			gc_cEventID,sTable.c_str(),gc_cStart_T,
			CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd),gc_cStart_T);
		if(pDB->More())
		{
			lnEventID = pDB->GetLong(gc_cEventID);
		}
		pDB->Cmd("select distinct %s from %s where (%s-(%d))%%%d = 0 and (%s between '%s' and '%s')",\
			gc_cEventID,sTable.c_str(),gc_cEventID,lnEventID,lnInterval,
			gc_cStart_T,CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd));
		while(pDB->More())
		{
			lvEventID.push_back(pDB->GetLong(gc_cEventID));
		}

		if(lvEventID.size() > 0)
		{
			CString lstrTmp;
			lstrTmp.Format("%d",lvEventID[0]);
			lstrEventIDArray += lstrTmp;
			for(unsigned int i = 0; i < lvEventID.size(); i++)
			{
				lstrTmp.Format(",%d",lvEventID[i]);
				lstrEventIDArray += lstrTmp;
			}
		}

		pDB->Cmd("SELECT distinct([%s]), [%s], [%s],[%s], [%s],[%s] FROM [%s] ",gc_cName,gc_cEventID,gc_cStart_T,gc_cEnd_T,gc_cType,gc_cChannType, sTable.c_str());
		pDB->Cmd(" WHERE %s in(%s) ORDER BY [%s]",gc_cEventID, 
			lstrEventIDArray,
			gc_cEventID);

	}
	else
	{
		pDB->Cmd("SELECT distinct([%s]), [%s], [%s], [%s], [%s], [%s] FROM [%s] ",gc_cName,gc_cEventID ,gc_cStart_T,gc_cEnd_T,gc_cType,gc_cChannType, sTable.c_str());
		pDB->Cmd(" WHERE %s BETWEEN '%s' AND '%s' ORDER BY [%s]",gc_cStart_T, 
			CPlantInfoBase::TimeConvertString(timeStart),
			CPlantInfoBase::TimeConvertString(timeEnd),
			gc_cEventID);
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	S_ChannInfo loChannInfo;

	CString lstrMonthTable;

	int lnRetInfo = 0;

	vector<S_AlarmlogInfo> lvAllLogInfo;

	S_AlarmlogInfo loLogInfo;

	IDBInterFace * pDBTemp = pDB;

	while (pDBTemp->More())
	{
		loLogInfo._iAlarmID = pDBTemp->GetLong(gc_cEventID);

		loLogInfo._sName = pDBTemp->Get(gc_cName);

		loLogInfo._cChannType = (CHANNTYPE)pDBTemp->GetLong(gc_cChannType);

		loLogInfo._tStart = pDBTemp->GetTime(gc_cStart_T);

		loLogInfo._tEnd = pDBTemp->GetTime(gc_cEnd_T);

		loLogInfo._iAlarmType = pDBTemp->GetLong(gc_cType);

		lvAllLogInfo.push_back(loLogInfo);
	}
	
	for (vector<S_AlarmlogInfo>::iterator lpIter = lvAllLogInfo.begin(); lpIter != lvAllLogInfo.end(); ++lpIter)
	{
		S_AlarmlogInfo loLogInfo = *lpIter;

		int lnAlarmID = loLogInfo._iAlarmID;

		string lstrChannNO = loLogInfo._sName;

		CHANNTYPE lnChannType = (CHANNTYPE)loLogInfo._cChannType;

		E_TBL_CHANNTYPE lenChannTblType = GetChannOneType( lnChannType );
		switch(lenChannTblType)
		{
		case E_TBL_CHANNTYPE_VIB:
			loChannInfo = pPlantInfo->GetSpecVibChannNoInfo(lstrChannNO);
			break;
		case E_TBL_CHANNTYPE_DYN:
			loChannInfo = pPlantInfo->GetSpecDynChannNoInfo(lstrChannNO);
			break;
		case E_TBL_CHANNTYPE_PROC:
			loChannInfo = pPlantInfo->GetSpecProcChannNoInfo(lstrChannNO);
			break;
		default:
			break;
		}

		CBHDateTime loDtCheckStart = loLogInfo._tStart;

		CBHDateTime loDtCheckEnd = loLogInfo._tEnd;

		E_TREND_TBL_TYPE lenTableType = GE_TREND_TBLTYPE_MONTH;
		lstrMonthTable.Format("%s%s",GetTblNamePrefix(lnChannType,lenTableType),sSuffix.c_str());

		if ( lenChannTblType == E_TBL_CHANNTYPE_VIB)
		{
			if ( lnChannType == GE_AXISLOCATIONCHANN)
			{
				sTable = "ALARM_AXIS_";
				sCharValueField = "";
			}
			else
			{
				sAlias = loChannInfo._cChannAlias;
				sChann = loChannInfo._cChannID;
				sTable = "ALARM_VIB_";
				if (loChannInfo._iChannType == GE_AXIALCHANN)
					sCharValueField = "THREEPHASE_";
				else
					sCharValueField = "ALL_";
				sCharValueField.append(loChannInfo._cChannNo);

				sAllCharValueField=ConsVibChannField(lstrChannNO);
			}
		}
		else if( lenChannTblType == E_TBL_CHANNTYPE_PROC)
		{
			sTable = "ALARM_PROC_";
			sAlias = loChannInfo._cChannAlias;
			sChann = loChannInfo._cChannID;
			sCharValueField = gc_cProcValue;
			sAllCharValueField = gc_cProcValue;

		}
		else
		{
			sAlias = loChannInfo._cChannAlias;
			sChann = loChannInfo._cChannID;
			sTable = "ALARM_DYN_";
			sCharValueField = "ZERO_";
			sCharValueField.append(loChannInfo._cChannNo);
			sAllCharValueField=ConsDynChannField(lstrChannNO);
		}

		sTable.append(sSuffix);
		if (sCharValueField.empty())
		{
			if (lnChannType == GE_AXISLOCATIONCHANN)
			{
				pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,sTable.c_str());
			}
			else
			{
				pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",gc_cRev,sTable.c_str());
			}
		}
		else
		{
			pDB->Cmd("SELECT top 1 %s,[%s] FROM [%s] ", sAllCharValueField.c_str(), gc_cRev,sTable.c_str());
		}

		if( lenChannTblType == E_TBL_CHANNTYPE_PROC)
		{
			pDB->Cmd(" WHERE [%s]=%d AND %s = '%s' AND [%s] >= '%s' order by %s",gc_cEventID,lnAlarmID,gc_cChannNo,lstrChannNO.c_str(),gc_cDateTime,loLogInfo._tStart.Format(),gc_cDateTime);
		}
		else
		{
			pDB->Cmd(" WHERE [%s]=%d and [%s] >= '%s' order by %s",gc_cEventID,lnAlarmID,gc_cDateTime,loLogInfo._tStart.Format(),gc_cDateTime);
		}
		

		bool lbGetDataSucc = false;

		if (pDB->More())
		{
			lbGetDataSucc = true;
		}

		if (!lbGetDataSucc)
		{
			if (sCharValueField.empty())
			{
				if (lnChannType == GE_AXISLOCATIONCHANN)
				{
					pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,lstrMonthTable);
				}
				else
				{
					pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",gc_cRev,lstrMonthTable);
				}
			}
			else
			{
				if(lenChannTblType == E_TBL_CHANNTYPE_PROC)
				{
					sCharValueField = loChannInfo._cChannNo;

					pDB->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ",sCharValueField.c_str() ,gc_cRev,lstrMonthTable);
				}
				else
				{
					pDB->Cmd("SELECT top 1 %s,[%s] FROM [%s] ", sAllCharValueField.c_str(),gc_cRev,lstrMonthTable);
				}
			}

			pDB->Cmd(" WHERE [datetime] between '%s' and '%s'",loDtCheckStart.Format(),loDtCheckEnd.Format());

			if (pDB->More())
			{	
				lbGetDataSucc = true;
			}
		}

		if (lbGetDataSucc)
		{
			iRev = pDB->GetLong(gc_cRev);
			if (! sCharValueField.empty())
			{
				fValue = pDB->GetDouble(sCharValueField.c_str());
			}

			if (lnChannType == GE_AXISLOCATIONCHANN)
			{
				float fCooX=.0,fCooY=.0;
				fCooX=pDB->GetDouble(gc_cCoor_X);
				fCooY=pDB->GetDouble(gc_cCoor_Y);
				fValue=sqrt(pow(abs(fCooX),2)+pow(abs(fCooY),2));
			}
		}

		lnRetInfo++;

		CString lstrChannName = loChannInfo._cChannID;

		int lnAlarmType = loLogInfo._iAlarmType;

		pFixComm->SetItem(FIELD_SERVICE_ALARMID,lnAlarmID);	
		pFixComm->SetItem(FIELD_SERVICE_ALARMID_CHANN_NUM,lstrChannName);	
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,loDtCheckStart);
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,ParseAlarmStatus(lnAlarmType));
		pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);

		if(lenChannTblType ==  E_TBL_CHANNTYPE_VIB)
		{	
			S_VibCharValue lsVibCharValue = ReadVibCharValue(lstrChannNO,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsVibCharValue,sizeof(S_VibCharValue));
			
		}
		else if(lenChannTblType ==  E_TBL_CHANNTYPE_DYN)
		{
			S_DynCharValue lsDynCharValue = ReadDynCharValue(lstrChannNO,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsDynCharValue,sizeof(S_DynCharValue));

		}
		
		pFixComm->SetItem( FIELD_SYSTEM_CHAR_TYPE, 1);
			
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return lnRetInfo;
}

/**监测站级历史报警统计
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
			 报警ID列表	      FIELD_SERVICE_ALARMIDS
			 报警ID个数	      FIELD_SERVICE_ALARMID_NUM
			 报警ID缓冲区大小 FIELD_SERVICE_ALARMID_BUFFSIZE
*返回值的字段码：
*		     报警ID           FIELD_SERVICE_ ALARM ID
*		     测点编号         FIELD_SERVICE_CHANN1
*			 测点类型         FIELD_SERVICE_CHANNTYPE
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
*		     报警类型         FIELD_SERVICE_ALARM_TYPE
*            成功标示         FIELD_SYSTEM_STATUS
			 接口数据上限	  FIELD_SERVICE_DATALIMIT
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21075
*/

int CTransactData::GetAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sChann,sAlias,sCharValueField,sAllCharValueField;
	CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_ChannInfo channInfo;
	S_AlarmlogInfo AlarmlogInfo;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
	{	
		return -1;
	}
	SwitchDbNameEx(pDB,sSuffix.c_str());

	int lnAlarmNum = pFixComm->GetLong(FIELD_SERVICE_ALARMID_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_ALARMID_BUFFSIZE)+1;

	if (iBytes<=0)
	{
		ASSERT(FALSE);
		return -1;
	}
	std::vector<CHAR> loCharBuffer;
	loCharBuffer.resize(iBytes);
	CHAR * sAlarmIDS = &loCharBuffer.front();

	pFixComm->GetItemBuf(FIELD_SERVICE_ALARMIDS,sAlarmIDS,iBytes);

	list<S_AlarmlogInfo> EventID;
	list<S_AlarmlogInfo>::iterator it;
	sTable = pPlantInfo->GetAlarmLogTblName();
	try
	{

		pDB->Cmd("SELECT * FROM [%s] ",sTable.c_str());
		pDB->Cmd(" WHERE [%s] in (%s) ",gc_cEventID, sAlarmIDS);
		while (pDB->More())
		{
			AlarmlogInfo._iAlarmID = pDB->GetLong(gc_cEventID);
			AlarmlogInfo._tStart = pDB->GetTime(gc_cStart_T);
			AlarmlogInfo._tEnd = pDB->GetTime(gc_cEnd_T);
			AlarmlogInfo._iAlarmType = pDB->GetLong(gc_cType);
			AlarmlogInfo._sName = pDB->Get(gc_cName);
			AlarmlogInfo._cChannType = pDB->GetLong(gc_cChannType);
			AlarmlogInfo._iAlarmLevel = pDB->GetLong(gc_cAlarmLevel);
			int lnAlarmStatus = ParseAlarmStatus(AlarmlogInfo._iAlarmType);

			if(lnAlarmStatus == 1)
			{
				AlarmlogInfo._iAlarmLevel = ALARM_URGENT;
			}
			else if(lnAlarmStatus == 2)
			{
				AlarmlogInfo._iAlarmLevel = ALARM_DANGEOUS;
			}
			else
			{
				if(AlarmlogInfo._iAlarmLevel <= 0 ||AlarmlogInfo._iAlarmLevel > ALARM_DANGEOUS)
				{
					AlarmlogInfo._iAlarmLevel = ALARM_POTENTIAL;
				}
			}
			EventID.push_back(AlarmlogInfo);
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
	CString lstrMonthTable;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	int lnDataLimit = FIELD_SYSTEM_MAX_TRANSFER_DATA_LIMIT_IN_NUM;
	for(it=EventID.begin();it!=EventID.end(); ++it)
	{
		CHANNTYPE iChannType= GE_ALLPROC;
		iChannType = (CHANNTYPE)(it->_cChannType);
		E_TBL_CHANNTYPE channTblType = GetChannOneType( iChannType );

		E_TREND_TBL_TYPE lenTableType = GE_TREND_TBLTYPE_MONTH;
		lstrMonthTable.Format("%s%s",GetTblNamePrefix(iChannType,lenTableType),sSuffix.c_str());

		if ( channTblType == E_TBL_CHANNTYPE_VIB)
		{
			if ( iChannType == GE_AXISLOCATIONCHANN)
			{
				sTable = "ALARM_AXIS_";
				sCharValueField = "";
				CString sAxisLocation;sAxisLocation.Format("轴心位置%s",it->_sName.c_str());
				sChann=sAxisLocation.GetString();
				sAlias=it->_sName;
			}
			else
			{
				channInfo = pPlantInfo->GetSpecVibChannNoInfo(it->_sName);
				sAlias = channInfo._cChannAlias;
				sChann = channInfo._cChannID;
				sTable = "ALARM_VIB_";
				if (channInfo._iChannType == GE_AXIALCHANN)
					sCharValueField = "THREEPHASE_";
				else
					sCharValueField = "ALL_";
				sCharValueField.append(channInfo._cChannNo);
				sAllCharValueField=ConsVibChannField(it->_sName);
			}
		}
		else if( channTblType == E_TBL_CHANNTYPE_PROC)
		{
			sTable = "ALARM_PROC_";
			channInfo = pPlantInfo->GetSpecProcChannNoInfo(it->_sName);
			sAlias = channInfo._cChannAlias;
			sChann = channInfo._cChannID;
			sCharValueField = gc_cProcValue;
			sAllCharValueField = gc_cProcValue;
			//sCharValueField.append(channInfo._cChannNo);
		}
		else
		{
			channInfo = pPlantInfo->GetSpecDynChannNoInfo(it->_sName);
			sAlias = channInfo._cChannAlias;
			sChann = channInfo._cChannID;
			sTable = "ALARM_DYN_";
			sCharValueField = "ZERO_";
			sCharValueField.append(channInfo._cChannNo);
			sAllCharValueField=ConsDynChannField(it->_sName);
		}
		if (index<lnDataLimit)
		{
			sTable.append(sSuffix);
			if (sCharValueField.empty())
			{
				if (iChannType == GE_AXISLOCATIONCHANN)
				{
					pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,sTable.c_str());
				}
				else
				{
					pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",gc_cRev,sTable.c_str());
				}
			}
			else
			{
				pDB->Cmd("SELECT top 1 %s,[%s] FROM [%s] ", sAllCharValueField.c_str(), gc_cRev,sTable.c_str());
			}

			if(channTblType == E_TBL_CHANNTYPE_PROC)
			{
				pDB->Cmd(" WHERE [%s]=%d and %s = '%s' and %s >= '%s' order by %s",gc_cEventID,it->_iAlarmID,gc_cTime,it->_tStart.Format(),gc_cChannNo,channInfo._cChannNo,gc_cTime);
			}
			else
			{
				pDB->Cmd(" WHERE [%s]=%d and %s >= '%s' order by %s",gc_cEventID,it->_iAlarmID,gc_cTime,it->_tStart.Format(),gc_cTime);
			}
			bool lbGetDataSucc = false;

			if (pDB->More())
			{
				lbGetDataSucc = true;
			}

			if (!lbGetDataSucc)
			{
				if (sCharValueField.empty())
				{
					if (iChannType == GE_AXISLOCATIONCHANN)
					{
						pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,lstrMonthTable);
					}
					else
					{
						pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",gc_cRev,lstrMonthTable);
					}
				}
				else
				{
					if(channTblType == E_TBL_CHANNTYPE_PROC)
					{
						sCharValueField = channInfo._cChannNo;

						pDB->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ", sCharValueField.c_str(),gc_cRev,lstrMonthTable);
					}
					else
					{
						pDB->Cmd("SELECT top 1 %s,[%s] FROM [%s] ", sAllCharValueField.c_str(),gc_cRev,lstrMonthTable);
					}
				}

				pDB->Cmd(" WHERE [datetime] between '%s' and '%s'",it->_tStart.Format(),it->_tEnd.Format());

				if (pDB->More())
				{	
					lbGetDataSucc = true;
				}
			}

			if (lbGetDataSucc)
			{
				iRev = pDB->GetLong(gc_cRev);
				if (! sCharValueField.empty())
				{
					fValue = pDB->GetDouble(sCharValueField.c_str());
				}
				if (iChannType == GE_AXISLOCATIONCHANN)
				{
					float fCooX=.0,fCooY=.0;
					fCooX=pDB->GetDouble(gc_cCoor_X);
					fCooY=pDB->GetDouble(gc_cCoor_Y);
					fValue=sqrt(pow(abs(fCooX),2)+pow(abs(fCooY),2));
				}
				//CString sTemp;sTemp.Format("报警ID:%d,报警类型:%d,测点名:%s",it->_iAlarmID,it->_iAlarmType,it->_sName.c_str());
				//CHZLogManage::Debug(sTemp,"CTransactData","GetMonitorAlarmStat");
				pFixComm->SetItem(FIELD_SERVICE_ALARMID,it->_iAlarmID);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
				pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,it->_tStart);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,it->_tEnd);
				int lnAlarmStatus = ParseAlarmStatus(it->_iAlarmType);
				if(lnAlarmStatus > 2)
				{
					lnAlarmStatus = 1;
					pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,(int)E_ALARM_TYPE_SEFL_LEARNING);
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,(int)E_ALARM_TYPE_THRESHOLD);
				}

				if(lnAlarmStatus == 5)
				{
					lnAlarmStatus = 2;
				}
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,lnAlarmStatus);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmStatus);
				pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sAlias.c_str());
				pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
				pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
				pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,it->_iAlarmLevel);

				CHANNTYPE lnChannType = (CHANNTYPE)it->_cChannType;

				E_TBL_CHANNTYPE lenChannTblType = GetChannOneType( lnChannType );

				if(lenChannTblType ==  E_TBL_CHANNTYPE_VIB)
				{	
					S_VibCharValue lsVibCharValue = ReadVibCharValue(it->_sName,pDB);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsVibCharValue,sizeof(S_VibCharValue));

				}
				else if(lenChannTblType ==  E_TBL_CHANNTYPE_DYN)
				{
					S_DynCharValue lsDynCharValue = ReadDynCharValue(it->_sName,pDB);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsDynCharValue,sizeof(S_DynCharValue));

				}
				pFixComm->SetItem( FIELD_SYSTEM_CHAR_TYPE, 1);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				index++;
			}
		}
		else
		{
			fValue= 0;iRev=0;
			pFixComm->SetItem(FIELD_SERVICE_ALARMID,it->_iAlarmID);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,it->_tStart);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,it->_tEnd);
			int lnAlarmStatus = ParseAlarmStatus(it->_iAlarmType);
			if(lnAlarmStatus > 2)
			{
				lnAlarmStatus = 1;
				pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,(int)E_ALARM_TYPE_SEFL_LEARNING);
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ALARM_STATUS,(int)E_ALARM_TYPE_THRESHOLD);
			}

			if(lnAlarmStatus == 5)
			{
				lnAlarmStatus = 2;
			}
			pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,lnAlarmStatus);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmStatus);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,lnAlarmStatus);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sAlias.c_str());
			pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
			pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,it->_iAlarmLevel);

			CHANNTYPE lnChannType = (CHANNTYPE)it->_cChannType;

			E_TBL_CHANNTYPE lenChannTblType = GetChannOneType( lnChannType );

			if(lenChannTblType ==  E_TBL_CHANNTYPE_VIB)
			{	
				S_VibCharValue lsVibCharValue = ReadVibCharValue(it->_sName,pDB);
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsVibCharValue,sizeof(S_VibCharValue));

			}
			else if(lenChannTblType ==  E_TBL_CHANNTYPE_DYN)
			{
				S_DynCharValue lsDynCharValue = ReadDynCharValue(it->_sName,pDB);
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char*)&lsDynCharValue,sizeof(S_DynCharValue));

			}

			pFixComm->SetItem( FIELD_SYSTEM_CHAR_TYPE, 1);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			index++;

		}
	}
	pFixComm->SetItem(FIELD_SERVICE_DATALIMIT,lnDataLimit);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,index);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	EventID.clear();
	return index;
}


/**监测站级历史报警统计
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID          FIELD_SERVICE_ ALARM ID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
*		     报警类型         FIELD_SERVICE_ALARM_TYPE
*            成功标示         FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21056
*/
int CTransactData::GetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sChann,sAlias,sCharValueField;
    CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_ChannInfo channInfo;
	S_AlarmlogInfo AlarmlogInfo;
    //得到已经显示的报警信息条数，决定是否跳过临界时间点的报警信息
	//这里改成，相同临界时间的报警信息条数，只要有数据就至少有一条临界时间报警条数
	long  ShowedLineNum = pFixComm->GetLong(FIELD_SERVICE_OFF_SHOW_ALARM_INFO_NUM);

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;

    //当连接分库数据库时，切换数据库名
	SwitchDbNameEx(pDB,sSuffix.c_str());

	//查询报警记录开始时间     FIELD_SERVICE_OFF_TIMESTART
	//查询报警记录结束时间     FIELD_SERVICE_OFF_TIMEEND
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

    //存放每一个报警记录中报警信息的list
	list<S_AlarmlogInfo> EventID;
	list<S_AlarmlogInfo>::iterator it;


    //获取报警记录表名字
	sTable = pPlantInfo->GetAlarmLogTblName();

    //拼接sql语句，查询报警记录表"Alarm_LOG_ZLFLFD10"
	pDB->Cmd("SELECT * FROM [%s] ",sTable.c_str());
	pDB->Cmd(" WHERE %s between '%s' AND '%s' order by %s desc " ,gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd),gc_cStart_T);

	while (pDB->More())
	{
	    //获得每一个新报警的报警ID
		AlarmlogInfo._iAlarmID = pDB->GetLong(gc_cEventID);
        
		AlarmlogInfo._tStart = pDB->GetTime(gc_cStart_T);
		AlarmlogInfo._tEnd = pDB->GetTime(gc_cEnd_T);

		AlarmlogInfo._iAlarmType = pDB->GetLong(gc_cType);
		AlarmlogInfo._sName = pDB->Get(gc_cName);
		AlarmlogInfo._cChannType = pDB->GetLong(gc_cChannType);

		AlarmlogInfo._iAlarmLevel = pDB->GetLong(gc_cAlarmLevel);
		int lnAlarmStatus = ParseAlarmStatus(AlarmlogInfo._iAlarmType);

		if(lnAlarmStatus == 1)
		{
			AlarmlogInfo._iAlarmLevel = ALARM_URGENT;
		}
		else if(lnAlarmStatus == 2)
		{
			AlarmlogInfo._iAlarmLevel = ALARM_DANGEOUS;
		}
		else
		{
			if(AlarmlogInfo._iAlarmLevel <= 0 ||AlarmlogInfo._iAlarmLevel > ALARM_DANGEOUS)
			{
				AlarmlogInfo._iAlarmLevel = ALARM_POTENTIAL;
			}
		}

		EventID.push_back(AlarmlogInfo);
	}

	CString lstrMonthTable;
	string lstrFieldRev;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());


    it=EventID.begin();
	//这里是需要的，非默认查询，返回时去掉临界时间那条报警记录
	//相同临界时间，报警信息的条数不确定
	/*if(ShowedLineNum !=0 )
	{
	    it++;
	}*/
    for (int i=0;i<ShowedLineNum;i++)
    {
	    it++;
    }

	for(;it!=EventID.end(); ++it)
	{
	    
		CHANNTYPE iChannType= GE_ALLPROC;
		iChannType = (CHANNTYPE)(it->_cChannType);
		E_TBL_CHANNTYPE channTblType = GetChannOneType( iChannType );
		E_TREND_TBL_TYPE lenTableType = GE_TREND_TBLTYPE_MONTH;
		lstrMonthTable.Format("%s%s",GetTblNamePrefix(iChannType,lenTableType),sSuffix.c_str());
		if ( channTblType == E_TBL_CHANNTYPE_VIB)    //振动通道
		{
			if ( iChannType == GE_AXISLOCATIONCHANN) //轴心位置
			{
				sTable = "ALARM_AXIS_";
				sCharValueField = "";
				CString sAxisLocation;
				sAxisLocation.Format("轴心位置%s",it->_sName.c_str());
				sChann=sAxisLocation.GetString();
				sAlias=it->_sName;
				lstrFieldRev = gc_cRev;
			}
			else
			{    ///得到指定振动测点的详细信息
				channInfo = pPlantInfo->GetSpecVibChannNoInfo(it->_sName);
				sAlias = channInfo._cChannAlias;
				sChann = channInfo._cChannID;
				sTable = "ALARM_VIB_";
				if (channInfo._iChannType == GE_AXIALCHANN) //轴向振动测点
					sCharValueField = "THREEPHASE_";
				else
					sCharValueField = "ALL_";

				sCharValueField.append(channInfo._cChannNo);

				lstrFieldRev = "REV_";
				lstrFieldRev.append(channInfo._cChannNo);
			}
		}
		else if( channTblType == E_TBL_CHANNTYPE_PROC)//过程量通道
		{
			sTable = "ALARM_PROC_";
			channInfo = pPlantInfo->GetSpecProcChannNoInfo(it->_sName);
			sAlias = channInfo._cChannAlias;
			sChann = channInfo._cChannID;
			sCharValueField = gc_cProcValue;
			//sCharValueField.append(channInfo._cChannNo);
			lstrFieldRev = gc_cRev;
		}
		else                                          //动态通道
		{
			channInfo = pPlantInfo->GetSpecDynChannNoInfo(it->_sName);
			sAlias = channInfo._cChannAlias;
			sChann = channInfo._cChannID;
			sTable = "ALARM_DYN_";
			sCharValueField = "ZERO_";
			sCharValueField.append(channInfo._cChannNo);

			lstrFieldRev = "REV_";
			lstrFieldRev.append(channInfo._cChannNo);
		}
		//设置在报警记录范围内需要显示报警信息的条数，这里的条数是有效数据的条数，暂时固定每次显示20条记录
		if (index<(19))
		{
			sTable.append(sSuffix);
			if (sCharValueField.empty())
			{
				if (iChannType == GE_AXISLOCATIONCHANN)   //轴心位置
				{
					pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,sTable.c_str());
				}
				else
					pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",lstrFieldRev.c_str(),sTable.c_str());
			}
			else
			{
				pDB->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ", sCharValueField.c_str(), lstrFieldRev.c_str(),sTable.c_str());
			}
			
			if(channTblType == E_TBL_CHANNTYPE_PROC)
			{
				pDB->Cmd(" WHERE [%s]=%d and %s >= '%s' and %s = '%s' order by %s",gc_cEventID,it->_iAlarmID,gc_cTime,it->_tStart.Format(),gc_cChannNo,channInfo._cChannNo,gc_cTime);
			}
			else
			{
				pDB->Cmd(" WHERE [%s]=%d and %s >= '%s' order by %s",gc_cEventID,it->_iAlarmID,gc_cTime,it->_tStart.Format(),gc_cTime);
			}

			bool lbGetDataSucc = false;
            //判断是否有数据，将取数据和设置数据分开
			if (pDB->More())
			{	
				lbGetDataSucc = true;
			}

			if (!lbGetDataSucc)
			{
				if (sCharValueField.empty())
				{
					if (iChannType == GE_AXISLOCATIONCHANN)
					{
						pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,sTable.c_str());
					}
					else
						pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",lstrFieldRev.c_str(),lstrMonthTable);
				}
				else
				{
					pDB->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ", sCharValueField.c_str(), lstrFieldRev.c_str(),lstrMonthTable);
				}

				pDB->Cmd(" WHERE [datetime] between '%s' and '%s'",it->_tStart.Format(),it->_tEnd.Format());

				if (pDB->More())
				{	
					lbGetDataSucc = true;
				}

			}

			if (lbGetDataSucc)
			{
				iRev = pDB->GetLong(lstrFieldRev.c_str());
				if (! sCharValueField.empty())
					fValue = pDB->GetDouble(sCharValueField.c_str());
				if (iChannType == GE_AXISLOCATIONCHANN)
				{
					float fCooX=.0,fCooY=.0;
					fCooX=pDB->GetDouble(gc_cCoor_X);
					fCooY=pDB->GetDouble(gc_cCoor_Y);
					fValue=sqrt(pow(abs(fCooX),2)+pow(abs(fCooY),2));
				}
				//CString sTemp;sTemp.Format("报警ID:%d,报警类型:%d,测点名:%s",it->_iAlarmID,it->_iAlarmType,it->_sName.c_str());
				//CHZLogManage::Debug(sTemp,"CTransactData","GetMonitorAlarmStat");
				pFixComm->SetItem(FIELD_SERVICE_ALARMID,it->_iAlarmID);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
				pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,it->_tStart);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,it->_tEnd);

				int lnAlarmStatus = ParseAlarmStatus(it->_iAlarmType);
				if(lnAlarmStatus > 2)
				{
					lnAlarmStatus = 1;
				}
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,lnAlarmStatus);
				pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,it->_iAlarmLevel);
				pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sAlias.c_str());
				pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
				pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				index++;
			}
		}
		else//这里是超过指定的显示报警信息个数情况下，显示全部其他每一条报警记录对应的top1报警信息
		{
			//fValue= 0;iRev=0;
			//pFixComm->SetItem(FIELD_SERVICE_ALARMID,it->_iAlarmID);//报警ID
			//pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());//测点名称
			//pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
			//pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,it->_tStart);
			//pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,it->_tEnd);
			//pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,ParseAlarmStatus(it->_iAlarmType));
			//pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sAlias.c_str());//测点别名
			//pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
			//pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
			//pFixComm->Write(pFixComm->GetAci());
			//pFixComm->CreateNextAnswer();
			//index++;
		}
	}
	if (index == -1)
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,index);
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	EventID.clear();
	return index;
}

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名          FIELD_SYSTEM_FACTORY
*	         设备名          FIELD_SERVICE_PLANTID
*            起始时间：      FIELD_SERVICE_OFF_TIMESTART
*            终止时间：      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID          FIELD_SERVICE_ALARMID
*		     测点编号        FIELD_SERVICE_CHANN1
*		     测点位号        FIELD_SYSTEM_CHANN_ALIAS
*		     报警开始时间    FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间    FIELD_SERVICE_OFF_TIMEEND
*		     报警类型        FIELD_SERVICE_ALARM_TYPE
             测点类型        FIELD_SERVICE_CHANNTYPE
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATEX         21088
*/
int CTransactData::GetMonitorAlarmStatEx(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sChann,sAlias,sCharValueField;
    CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_ChannInfo channInfo;
	S_AlarmlogInfo AlarmlogInfo;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;

    //当连接分库数据库时，切换数据库名
	SwitchDbNameEx(pDB,sSuffix.c_str());

	//查询报警记录开始时间     FIELD_SERVICE_OFF_TIMESTART
	//查询报警记录结束时间     FIELD_SERVICE_OFF_TIMEEND
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

    //获取报警记录表名字
	sTable = pPlantInfo->GetAlarmLogTblName();

    //拼接sql语句，查询报警记录表"Alarm_LOG_ZLFLFD10"
	pDB->Cmd("SELECT [%s],[%s],[%s],[%s],[%s],[%s] FROM [%s] ",gc_cEventID,gc_cName,gc_cStart_T,gc_cEnd_T,gc_cType,gc_cChannType,sTable.c_str());
	pDB->Cmd(" WHERE %s between '%s' AND '%s' order by %s desc " ,gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd),gc_cStart_T);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	
	int lnCount = 0;

	while (pDB->More())
	{
		string lstrChann = pDB->Get(gc_cName);

		int lnChannType = pDB->GetLong(gc_cChannType);

		switch(lnChannType)
		{
		case GE_VIBCHANN:
			 channInfo = pPlantInfo->GetSpecVibChannNoInfo(lstrChann);
			 break;
		case GE_DYNPRESSCHANN:
		case GE_RODSINKCHANN:
		case GE_DYNSTRESSCHANN:
			 channInfo = pPlantInfo->GetSpecDynChannNoInfo(lstrChann);
			 break;
		case GE_PRESSCHANN:
		case GE_TEMPCHANN:
		case GE_FLUXCHANN:
		case GE_OTHERCHANN:
		case GE_IMPACTCHANN:
		case GE_CURRENTCHANN:
			channInfo = pPlantInfo->GetSpecProcChannNoInfo(lstrChann);
			break;
		default:
			 channInfo = pPlantInfo->GetSpecVibChannNoInfo(lstrChann);
		}

		
		
		pFixComm->SetItem(FIELD_SERVICE_ALARMID,pDB->GetLong(gc_cEventID));
		
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
		
		pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,lnChannType);
		
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,pDB->GetTime(gc_cStart_T));
		
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cEnd_T));
		
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,ParseAlarmStatus(pDB->GetLong(gc_cType)));
		
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);

		int lnAlarmLevel = pDB->GetLong(gc_cAlarmLevel);
		int lnAlarmType = pDB->GetLong(gc_cType);

		int lnAlarmStatus = ParseAlarmStatus(lnAlarmType);

		if(lnAlarmStatus == 1)
		{
			lnAlarmLevel = ALARM_URGENT;
		}
		else if(lnAlarmStatus == 2)
		{
			lnAlarmLevel = ALARM_DANGEOUS;
		}
		else
		{
			if(lnAlarmLevel <= 0 ||lnAlarmLevel > ALARM_DANGEOUS)
			{
				lnAlarmLevel = ALARM_POTENTIAL;
			}
		}

		pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,lnAlarmLevel);
		
		pFixComm->Write(pFixComm->GetAci());
		
		pFixComm->CreateNextAnswer();

		lnCount++;
	}

	pFixComm->CreateEndMak();
	
	pFixComm->Write(pFixComm->GetAci());

	return lnCount;
}


/**单通道多特征值报警查询，得到指定报警测点的各种类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            时间             FIELD_SERVICE_TIME
*            对应时间的微秒   FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES   21057
*/
int CTransactData::GetSpecChannAlarmMultCharValues(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount =-1;
	string sSuffix,sChann;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iEventID=pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	//得到振动报警特征值数据
	CString strTable;
	S_VibCharValue vibValue;
	CString lstrChar = ConsVibChannField(channInfo._cChannNo);
	strTable.Format("ALARM_VIB_%s",sSuffix.c_str());
	pDB->Cmd("SELECT %s,%s,%s FROM [%s] ",gc_cTime,gc_cMicroSecond,lstrChar,strTable);
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iEventID);
	pDB->Cmd(" ORDER BY [%s] DESC ",gc_cTime);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		vibValue=ReadVibCharValue(channInfo._cChannNo,pDB);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++nCount;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警测点buff     FIELD_SERVICE_CHANN_BUFF
*            报警测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            报警测点个数     FIELD_SERVICE_CHANN_NUM
*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*            时间                 FIELD_SERVICE_TIME
*            对应时间的微秒       FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE   21058
*/
int CTransactData::GetSpecChannsAlarmSingleCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iRtn =-1;
	string sSuffix,sChann,sAlias,sCharValueField;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iEventID=pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	//如果设备ID和测点1ID有一个为空就不能读历史趋势
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	lCHANNID_NO lChannPair=ParseVibChannList(pPlantInfo,pChannBuff,iChannNum);
	lCHANNID_NO::iterator itPair;

	list<string>::iterator itChann;
	list<string> lChannField;
	//从报警日志表中查询得到各个测点的报警开始时间和结束时间
	for (itPair=lChannPair.begin();itPair!=lChannPair.end();++itPair)
	{
		if (itPair->second.c_str() =="")
			continue;
		lChannField.push_back(ConsVibChannSpecTypeField(itPair->second,(CHARTYPE)iCharType));
	}

	//需要获得的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append("[");
		sShowField.append(*itChann);
		sShowField.append("],");
	}
	sShowField.append(gc_cTime);
	sShowField.append(",");
	sShowField.append(gc_cMicroSecond);

	//循环获取各个测点的特征值数据
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum];
	string sTable="ALARM_VIB_";
	sTable.append(sSuffix);
	pDB->Cmd("SELECT %s FROM [%s] ",sShowField.c_str(),sTable.c_str());
	pDB->Cmd(" WHERE [%s] = %d ",gc_cEventID,iEventID);
	pDB->Cmd(" ORDER BY [%s]  ",gc_cTime);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		int nloop = 0;
		for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
		{
			pfValue[nloop]=pDB->GetDouble(itChann->c_str());
			++nloop;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum);
		pFixComm->SetItem(FIELD_SERVICE_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++iRtn;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	if(NULL!=pfValue) delete [] pfValue;
	lChannField.clear();
	lChannPair.clear();
	delete [] pChannBuff;
	return iRtn;
}

/**得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹
*输入参数：
*          公司ID            FIELD_SYSTEM_COMPANY
*		   分厂名            FIELD_SYSTEM_FACTORY
*          机组ID            FIELD_SERVICE_PLANTID
*          轴承位置描述      FIELD_SYSTEM_AXISLOCATION
*          水平方向测点ID    FIELD_SERVICE_CHANNID_HOR
*          垂直方向测点ID    FIELD_SERVICE_CHANNID_VER
*          波形数据个数      FIELD_SERVICE_OFF_WAVE_NUM
*          时间值          　FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*         X方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
*         Y方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
*         实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
*         X方向测点特征值     FIELD_SERVICE_CHARVALUE_S
*         Y方向测点特征值     FIELD_SERVICE_CHARVALUE_S2
*         转速                FIELD_SERVICE_OFF_REV
*		  轴心轨迹圆点X坐标   FIELD_SERVICE_AXIS_COORX
*	      轴心轨迹圆点Y坐标   FIELD_SERVICE_AXIS_COORY
*		  报警椭圆水平轴半径  FIELD_SERVICE_HOR_RADIUS
*		  报警椭圆垂直轴半径  FIELD_SERVICE_VER_RADIUS
#define     SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION            21059
*/
int CTransactData::GetSpecChannRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1;
	DATATYPE_WAVE *pHorWave=NULL;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pPeriodHorWave=NULL;
	DATATYPE_WAVE *pPeriodVerWave=NULL;
	DATATYPE_WAVE *pVerAxesWave=NULL;
	DATATYPE_WAVE *pHorAxesWave=NULL;

	float *pfHorWave=NULL;
	float *pfVerWave=NULL;
	float *pfPeriodHorWave=NULL;
	float *pfPeriodVerWave=NULL;
	float *pfVerAxesWave=NULL;
	float *pfHorAxesWave=NULL;

	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo ==NULL)
		return -1;

	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);

	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	CAxisChannInfo axisChannInfo;

	if (sChannIDHor.empty()||sChannIDVer.empty())
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		//查询得到活塞杆轴心位置的对应的垂直测点和水平测点
		axisChannInfo=pPlantInfo->GetSpecLocationAxisChannInfo(sLocation);
		S_ChannInfo sHorChannInfo = pPlantInfo->GetSpecDynChannInfo(axisChannInfo._cHorChann);
		S_ChannInfo sVerChannInfo = pPlantInfo->GetSpecDynChannInfo(axisChannInfo._cVerChann);
		sChannIDHor = sHorChannInfo._cChannID;
		sChannIDVer = sVerChannInfo._cChannID;
	}
	else
	{
		axisChannInfo=pPlantInfo->GetSpecDynChannAxisChannInfo(sChannIDHor,sChannIDVer);
	}

	iWaveNum = iWaveNum <100 ? 513: iWaveNum;
	int iSmpNums = pPlantInfo->GetPlantInfo()._iSmpNum;
	if (iWaveNum<=0 || iSmpNums<=0)
	{
		return -1;
	}
	//CString strTmp;strTmp.Format("strChannHor：%s,strChannVer：%s,iWaveNum:%d,timeOff:%s,iSmpNums:%d",strChannHor,strChannVer,iWaveNum,CPlantInfoBase::TimeConvertString(timeOff),iSmpNums);
	//CHZLogManage::Debug( strTmp, "CTransactData","GetSpecChannRodSinkAxesLocation");
	pHorWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
	pVerWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];

	vector<float> lvfHorWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfVerWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfHorWave = &lvfHorWave.front();
	pfVerWave = &lvfVerWave.front();

	S_DynCharValue dynHorCharValue;
	S_DynCharValue dynVerCharValue;

	CWaveHead sWaveHeadHor;
	CWaveHead sWaveHeadVer;

	float fRev =.1;
	int iSmpFreq =1;
	//获取波形数据;
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	int iRtnHor=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChannIDHor, pHorWave, iSmpNums, dynHorCharValue, iSmpFreq, fRev, timeOff, pfHorWave,sWaveHeadHor);
	int iRtnVer=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChannIDVer, pVerWave, iSmpNums, dynVerCharValue, iSmpFreq, fRev, timeOff, pfVerWave,sWaveHeadVer);
	if (iRtnHor>0 && iRtnVer>0)
	{
		//得到周期起始和结束索引
		int iPeriodStartIndex = 0,iPeriodEndIndex = 0,iRealWaveNum = 0;
		S_ChannInfo channInfoVer = pPlantInfo->GetSpecDynChannInfo(sChannIDVer);
		pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfoVer._cChannNo,timeOff,iPeriodStartIndex,iPeriodEndIndex);
		int iPeriodNum = iPeriodEndIndex - iPeriodStartIndex;
		if (iPeriodNum<0)
			iPeriodNum=1;//避免周期点数为负数时，导致该接口出现异常的问题
		iWaveNum = iWaveNum>iPeriodNum?iPeriodNum:iWaveNum;
		if (iWaveNum<=0)
		{
			delete[] pHorWave;
			delete[] pVerWave;
			return -1;
		}
		pPeriodHorWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
		pPeriodVerWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX ];
		for (int i=0;i<iWaveNum;i++)
		{//截取一个周期的波形数据
			pPeriodHorWave[i] =  pHorWave[i];
			pPeriodVerWave[i] =  pVerWave[i];
		}
		pVerAxesWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pHorAxesWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		if (iWaveNum>0)
		{
			/*pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pPeriodVerWave,iWaveNum * sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pPeriodHorWave,iWaveNum * sizeof(DATATYPE_WAVE));
			CString strTmp;strTmp.Format("strChannHor：%s,strChannVer：%s,iWaveNum:%d",strChannHor,strChannVer,iWaveNum);
			CHZLogManage::Debug( strTmp, "CTransactData","GetSpecChannRodSinkAxesLocation");*/
			//根据水平和垂直的波形数据,计算活塞杆轴心位置
			if(!((sChannIDVer.empty())||(sChannIDHor.empty())))
			{
				if(pPlantInfo->CalcSpecDynChannLocationValue(sChannIDHor,sChannIDVer,iWaveNum,pPeriodHorWave,pPeriodVerWave,pHorAxesWave,pVerAxesWave))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerAxesWave,iWaveNum * sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorAxesWave,iWaveNum * sizeof(DATATYPE_WAVE));
					//CString strInfo;strInfo.Format("输出的波形值 %d,%d,%d,%d",pHorAxesWave[0],pVerAxesWave[0],pHorAxesWave[1],pVerAxesWave[1]);
					//CHZLogManage::Debug( strInfo, "CTransactData","GetSpecChannRodSinkAxesLocation");
				}

			}
			else if(!sLocation.empty())
			{
				if(pPlantInfo->CalcSpecDynChannLocationValue(sLocation,iWaveNum,pPeriodHorWave,pPeriodVerWave,pHorAxesWave,pVerAxesWave))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerAxesWave,iWaveNum * sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorAxesWave,iWaveNum * sizeof(DATATYPE_WAVE));
				}

			}
		}
		pFixComm->SetItem(FIELD_SERVICE_AXESTRACK_WAVENUM,iWaveNum);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&dynHorCharValue,sizeof(S_DynCharValue));
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&dynVerCharValue,sizeof(S_DynCharValue));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,fRev);
		pFixComm->SetItem(FIELD_SERVICE_AXIS_COORX,axisChannInfo._axisAlarmParam._thLearnParamS._fParam3);
		pFixComm->SetItem(FIELD_SERVICE_AXIS_COORY,axisChannInfo._axisAlarmParam._thLearnParamS._fParam4);
		pFixComm->SetItem(FIELD_SERVICE_HOR_RADIUS,axisChannInfo._axisAlarmParam._thLearnParamS._fParam1);
		pFixComm->SetItem(FIELD_SERVICE_VER_RADIUS,axisChannInfo._axisAlarmParam._thLearnParamS._fParam2);
		//CString sTemp;sTemp.Format("fX:%f,fY:%f,fHorAxis:%f,fVerAxis:%f\n",axisChannInfo._axisAlarmParam._thLearnParamS._fParam3,axisChannInfo._axisAlarmParam._thLearnParamS._fParam4,axisChannInfo._axisAlarmParam._thLearnParamS._fParam1,axisChannInfo._axisAlarmParam._thLearnParamS._fParam2);BHTRACE_DEBUG(sTemp);
		index++;
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		index= -1;
	}
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pPeriodHorWave);
	KD_DeletePoint(pPeriodVerWave);
	KD_DeletePoint(pHorAxesWave);
	KD_DeletePoint(pVerAxesWave);
	return index;
}

/**得到往复式机械设备运行状态历史趋势
*读取所有各种测点的特征值，通过循环依次取得各个测点的第一个特征值。
*输入参数值：公司ID               FIELD_SYSTEM_COMPANY
*		     分厂名               FIELD_SYSTEM_FACTORY
*            机组ID               FIELD_SERVICE_PLANTID
*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数         FIELD_SERVICE_CHANN_NUM
*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
*            过程量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            过程量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            过程量测点个数       FIELD_SERVICE_CHANN_NUM3
*            起始时间             FIELD_SERVICE_OFF_TIMESTART
*            终止时间             FIELD_SERVICE_OFF_TIMEEND
*            曲轴箱振动特征值类型 FIELD_SERVICE_TREND_CHARTYPE
*            缸体振动特征值类型   FIELD_SERVICE_CHANNTYPE
*            其它振动特征值类型   FIELD_SERVICE_OTHER_CHARTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060
*/
int CTransactData::GetSpecChannsRunStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0,nCount=0,iCount=0;
	string sCompany,sFactory,sPlant;
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	//得到振动测点
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//得到动态测点
	int iChannNum2   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM2);
	int iBytes2      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE2)+1;
	char *pChannBuff2= new char[iBytes2] ;
	memset(pChannBuff2,0,iBytes2);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF2,pChannBuff2,iBytes2);
	//得到过程量测点
	int iChannNum3   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM3);
	int iBytes3      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE3)+1;
	char *pChannBuff3= new char[iBytes3];
	memset(pChannBuff3,0,iBytes3);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF3,pChannBuff3,iBytes3);

	int iTraVidCharType = pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iCylVidCharType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iOtherCharType  = pFixComm->GetLong(FIELD_SERVICE_OTHER_CHARTYPE);
	int iPreDynCharType = pFixComm->GetLong(FIELD_SYSTEM_EVENT_TYPE);
	int iRodDynCharType = pFixComm->GetLong(FIELD_SERVICE_OFF_TRENDTYPE);

	//CString sTemp;sTemp.Format("%d,%d,%d,%d",iCylVidCharType,iTraVidCharType,iPreDynCharType,iRodDynCharType);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetSpecChannsRunStatus");
	//CString sTemp;sTemp.Format("%d,%d,%d,%d,%d,%d,%s,%s,%s",iChannNum,iChannNum2,iChannNum3,iBytes,iBytes2,iBytes3,pChannBuff,pChannBuff2,pChannBuff3);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetSpecChannsRunStatus");

	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	//CBHDateTime tCurrTime = CBHDateTime::GetCurrentTime();SYSTEMTIME sysTime;GetLocalTime(&sysTime);
	//CString strTemp1;strTemp1.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//读取振动测点特征值
	if (iChannNum>0)
	{
		index = ReadVibSpecCharValueTrend(pFixComm,pDB,
			sCompany,sFactory,sPlant,
			pChannBuff,iChannNum,
			iCylVidCharType, iTraVidCharType, iOtherCharType,
			timeStart,timeEnd);
	}
	//读取动态测点特征值
	if (iChannNum2>0)
	{
		nCount = ReadDynSpecCharValueTrend(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff2,iChannNum2,iPreDynCharType,iRodDynCharType,timeStart,timeEnd);
	}
	//读取过程量测点特征值
	if (iChannNum3>0)
	{
		iCount = ReadProcSpecValueTrend(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff3,iChannNum3,timeStart,timeEnd);
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pChannBuff2);
	KD_DeletePoint(pChannBuff3);
	//tCurrTime = CBHDateTime::GetCurrentTime();GetLocalTime(&sysTime);
	//CString strTemp2;strTemp2.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	//CHZLogManage::Debug("开始读数据时间"+strTemp1+"读数据完成时间"+strTemp2,"CTransactData","GetSpecChannsRunStatus");
	return (index+nCount+iCount);
}


/**得到往复式机械设备运行状态历史趋势
*读取所有各种测点的特征值，通过循环依次取得各个测点的第一个特征值。
*输入参数值：公司ID               FIELD_SYSTEM_COMPANY
*		     分厂名               FIELD_SYSTEM_FACTORY
*            机组ID               FIELD_SERVICE_PLANTID
*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数         FIELD_SERVICE_CHANN_NUM
*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
*            过程量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            过程量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            过程量测点个数       FIELD_SERVICE_CHANN_NUM3
*            起始时间             FIELD_SERVICE_OFF_TIMESTART
*            终止时间             FIELD_SERVICE_OFF_TIMEEND
*            曲轴箱振动特征值类型 FIELD_SERVICE_TREND_CHARTYPE
*            缸体振动特征值类型   FIELD_SERVICE_CHANNTYPE
*            其它振动特征值类型   FIELD_SERVICE_OTHER_CHARTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX                   21082
*/
int CTransactData::GetSpecChannsRunStatusEx(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int lnVibCount=0,lnDynCount=0,lnProcCount=0;
	string sCompany,sFactory,sPlant;
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	//得到振动测点
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	//得到动态测点
	int iChannNum2   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM2);
	int iBytes2      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE2)+1;
	char *pChannBuff2= new char[iBytes2] ;
	memset(pChannBuff2,0,iBytes2);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF2,pChannBuff2,iBytes2);
	//得到过程量测点
	int iChannNum3   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM3);
	int iBytes3      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE3)+1;
	char *pChannBuff3= new char[iBytes3];
	memset(pChannBuff3,0,iBytes3);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF3,pChannBuff3,iBytes3);

	int iTraVidCharType = pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iCylVidCharType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iOtherCharType  = pFixComm->GetLong(FIELD_SERVICE_OTHER_CHARTYPE);
	int iPreDynCharType = pFixComm->GetLong(FIELD_SYSTEM_EVENT_TYPE);
	int iRodDynCharType = pFixComm->GetLong(FIELD_SERVICE_OFF_TRENDTYPE);

	//CString sTemp;sTemp.Format("%d,%d,%d,%d",iCylVidCharType,iTraVidCharType,iPreDynCharType,iRodDynCharType);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetSpecChannsRunStatus");
	//CString sTemp;sTemp.Format("%d,%d,%d,%d,%d,%d,%s,%s,%s",iChannNum,iChannNum2,iChannNum3,iBytes,iBytes2,iBytes3,pChannBuff,pChannBuff2,pChannBuff3);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetSpecChannsRunStatus");

	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	//CBHDateTime tCurrTime = CBHDateTime::GetCurrentTime();SYSTEMTIME sysTime;GetLocalTime(&sysTime);
	//CString strTemp1;strTemp1.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//读取振动测点特征值
	if (iChannNum>0)
	{
		lnVibCount = ReadVibSpecCharValueTrendEx(pFixComm,pDB,
			sCompany,sFactory,sPlant,
			pChannBuff,iChannNum,
			iCylVidCharType, iTraVidCharType, iOtherCharType,
			timeStart,timeEnd);
	}
	//读取动态测点特征值
	if (iChannNum2>0)
	{
		lnDynCount = ReadDynSpecCharValueTrendEx(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff2,iChannNum2,iPreDynCharType,iRodDynCharType,timeStart,timeEnd);
	}

	//读取过程量测点特征值
	if (iChannNum3>0)
	{
		lnProcCount = ReadProcSpecValueTrend(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff3,iChannNum3,timeStart,timeEnd);
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pChannBuff2);
	KD_DeletePoint(pChannBuff3);
	//tCurrTime = CBHDateTime::GetCurrentTime();GetLocalTime(&sysTime);
	//CString strTemp2;strTemp2.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	//CHZLogManage::Debug("开始读数据时间"+strTemp1+"读数据完成时间"+strTemp2,"CTransactData","GetSpecChannsRunStatus");
	return (lnVibCount+lnDynCount+lnProcCount);
}


/**得到往复式机械设备历史特征值和波形数据趋势横、纵向比较；
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID          FIELD_SERVICE_PLANTID
*            测点buff         FIELD_SERVICE_CHANN_BUFF
*            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数         FIELD_SERVICE_CHANN_NUM
*            测点类型         FIELD_SERVICE_OFF_CHANNTYPE
*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*            起始时间         FIELD_SERVICE_OFF_TIMESTART
*            终止时间         FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*            转速             FIELD_SERVICE_REV_BUFF
*            时间值           FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETHISCOMPARE         21061
*/
int CTransactData::GetSpecChannsHisCompare(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0;
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_OFF_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	int iType=GetChannOneType((CHANNTYPE)iChannType);
	if (iType == E_TBL_CHANNTYPE_VIB)
		index=ReadVibTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timeStart,timeEnd);
	else
		index = ReadDynTrendSpecType(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,iCharType,timeStart,timeEnd);
	KD_DeletePoint(pChannBuff);
	return index;
}

/**得到往复式机械设备所有通道指定时间的特征值和波形数据比较
*输入参数：
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            测点buff         FIELD_SERVICE_CHANN_BUFF
*            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            测点类型         FIELD_SERVICE_CHANNTYPE
*            测点个数         FIELD_SERVICE_CHANN_NUM
*            时间             FIELD_SERVICE_OFF_TIME
*            波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*            测点的波形数据   FIELD_SERVICE_OFF_WAVE
*            波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            转速             FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE         21062
*/
int CTransactData::GetSpecChannsCharValueWave(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0;
	string sChann="",sSuffix;
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	int   iWaveNum = pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	list<string> lChann;
	list<string>::iterator it;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	iWaveNum = iWaveNum <100 ? 513: iWaveNum;
	if (iWaveNum<=0)
	{
		KD_DeletePoint(pChannBuff);
		return -1;
	}
	DATATYPE_WAVE *pWave = new DATATYPE_WAVE[ GD_MIDDLEWARE_WAVE_POINT_MAX];
	float         *pfWave;
	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	pfWave = &lvfWave.front();
	CWaveHead lsWaveHead;

	S_DynCharValue dynCharValue;
	S_VibCharValue vibCharValue;
	S_ChannInfo channInfo;
	float fRev =.1;
	int iSmpFreq =1;
	int iType=GetChannOneType((CHANNTYPE)iChannType);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iType == E_TBL_CHANNTYPE_VIB)
		//循环测点通道得到波形数据及其相关数据
		for (it=lChann.begin();it!=lChann.end();++it)
		{
			sChann = *(it);
			//读取振动测点的特征值和波形
			index=ReadVibChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann,pWave,iWaveNum,vibCharValue,iSmpFreq,fRev,timeOff,pfWave,lsWaveHead,true);
			if (index>0)
			{
				int iEndIndex  = iWaveNum;
				if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
				{
					int iStartIndex = 0;
					
					int lnChannIndex = pPlantInfo->GetSpecVibChannIndex(sChann);
					if (lnChannIndex>=0)
					{
						string lstrChannNo = pPlantInfo->GetSpecVibChannInfo(lnChannIndex)._cChannNo;
						pPlantInfo->GetSpecVibChannStartEndIndex(pDB,lstrChannNo,timeOff,iStartIndex,iEndIndex);

					}
					if (iEndIndex> iWaveNum||iEndIndex<=0)
					{	
						iEndIndex = iWaveNum;
					}
				}
				pFixComm->SetItemBuf( FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
				pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWave, iWaveNum * sizeof(DATATYPE_WAVE));
				pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iEndIndex);
				pFixComm->SetItem( FIELD_SERVICE_REV, fRev);

				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
					
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			else
				pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, 0);

		}
	else if (iType==E_TBL_CHANNTYPE_DYN)
	{   //循环测点通道得到波形数据及其相关数据
		for (it=lChann.begin();it!=lChann.end();++it)
		{
			sChann = *(it);
			//读取动态测点的特征值和波形
			index=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann, pWave, iWaveNum, dynCharValue, iSmpFreq, fRev, timeOff,pfWave,lsWaveHead);
			if (index>0)
			{
				pFixComm->SetItemBuf( FIELD_SERVICE_CHARVALUE_S, (char *)&dynCharValue, sizeof(S_DynCharValue));
				pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWave, iWaveNum * sizeof(DATATYPE_WAVE));
				pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iWaveNum);
				pFixComm->SetItem( FIELD_SERVICE_REV, fRev);
				if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);

					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			else
				pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, 0);
		}
	}
	else
		pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, 0);
	lChann.clear();
	KD_DeletePoint(pWave);
	KD_DeletePoint(pChannBuff);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**得到往复式机械设备振动通道和动态通道指定时间的特征值
*输入参数：
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            振动测点buff     FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数     FIELD_SERVICE_CHANN_NUM
*            动态测点buff     FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数     FIELD_SERVICE_CHANN_NUM2
*            振动特征值时间     FIELD_SERVICE_TIME
*            动态特征值时间     FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            转速             FIELD_SERVICE_OFF_REV
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063
*/
int CTransactData::GetSpecChannsCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index=0,nCount=0;
	string sChann="",sSuffix;
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;
	//得到振动测点
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[iBytes];
	memset(pChannBuff,0,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string>  lChannVib=ParseChannList(pChannBuff,iChannNum);
	list<string> ::iterator itVib;
	//得到动态测点
	int iChannNum2   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM2);
	int iBytes2      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE2);
	char *pChannBuff2= new char[iBytes2] ;
	memset(pChannBuff2,0,iBytes2);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF2,pChannBuff2,iBytes2);
	list<string>  lChannDyn=ParseChannList(pChannBuff2,iChannNum2);
	list<string> ::iterator itDyn;

	CBHDateTime timeOffVib=CBHDateTime::GetCurrentTime();
	CBHDateTime timeOffDyn=CBHDateTime::GetCurrentTime();
	timeOffVib = pFixComm->GetTime(FIELD_SERVICE_TIME);
	timeOffDyn = pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);

	S_DynCharValue dynCharValue;
	S_VibCharValue vibCharValue;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iChannNum>0)
	{
		for (itVib=lChannVib.begin();itVib!=lChannVib.end();++itVib)
		{
			sChann =(*itVib);
			index=ReadVibChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,sPlant,sChann,timeOffVib);
			if (index>0)
			{
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++nCount;
			}
		}

	}
	if (iChannNum2>0)
	{
		for (itDyn=lChannDyn.begin();itDyn!=lChannDyn.end();++itDyn)
		{
			sChann =(*itDyn);
			index=ReadDynChannSpecTimeCharValueInfo(pFixComm,pDB,sCompany,sFactory,sPlant,sChann,timeOffDyn);
			if (index>0)
			{
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++nCount;
			}
		}

	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChannVib.clear();
	lChannDyn.clear();
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pChannBuff2);
	return nCount;
}

/**得到往复式机械设备机组月报表；
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            时间值           FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            测点名           FIELD_SERVICE_CHANN1
*            特征值最大值     FIELD_SERVICE_CHANN1VALUE
*            特征值最小值     FIELD_SERVICE_CHANN2VALUE
*            特征值平均值     FIELD_SERVICE_TREND_VALUE
#define     SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT       21064
*/
int CTransactData::GetSpecRecipMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nCount=-1;
	string sCompany,sFactory,sPlant,sSuffix,sTable,sCharValueField ="";
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if( pPlantInfo == NULL )
		return -1;
	CBHDateTime dtSpec=CBHDateTime::GetCurrentTime();
	dtSpec=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
	int iMonth=dtSpec.GetMonth();
	dtStart.SetDateTime(dtSpec.GetYear(),iMonth,1,0,0,0);
	if (iMonth==12)
		dtEnd.SetDateTime(dtSpec.GetYear()+1,1,1,0,0,0);
	else
		dtEnd.SetDateTime(dtSpec.GetYear(),dtSpec.GetMonth()+1,1,0,0,0);
	//得到所有测点及测点类型
	M_CHANNINDEX mapChann;
	M_CHANNINDEX::iterator itMap;
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT %s,%s FROM [%s] ",gc_cChannID,gc_cChannType,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo, pPlantInfo->GetPlantNo().c_str());
	while (pDB->More())
		mapChann.insert(make_pair(pDB->Get(gc_cChannID),pDB->GetLong(gc_cChannType)));
	if (mapChann.size()==0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}
	SwitchDbNameEx(pDB,sSuffix.c_str());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itMap=mapChann.begin();itMap!=mapChann.end();++itMap)
	{
		S_ChannInfo channInfo;
		int iChannType = itMap->second;
		E_TBL_CHANNTYPE channTblType = GetChannOneType( (CHANNTYPE)iChannType );
		if ( channTblType == E_TBL_CHANNTYPE_VIB)
		{
			sTable="TREND_VIBMONTH_";
			channInfo = pPlantInfo->GetSpecVibChannInfo(itMap->first);
			sCharValueField = "ALL_";
			sCharValueField.append(channInfo._cChannNo);
		}
		else if( channTblType == E_TBL_CHANNTYPE_PROC)
		{
			sTable="TREND_PROCMONTH_";
			channInfo = pPlantInfo->GetSpecProcChannInfo(itMap->first);
			sCharValueField = channInfo._cChannNo;
		}
		else
		{
			sTable="TREND_DYNMONTH_";
			channInfo = pPlantInfo->GetSpecDynChannInfo(itMap->first);
			sCharValueField = "ZERO_";
			sCharValueField.append(channInfo._cChannNo);
		}
		sTable.append(sSuffix);
		//CString strInfo;strInfo.Format("%s",sCharValueField.c_str());
		//CHZLogManage::Debug(strInfo, "CTransactData", "GetSpecRecipMonthReport");
		nCount=0;
		pDB->Cmd("Select isnull(max([%s]),0) as max_value,isnull(min([%s]),0) as min_value,isnull(avg([%s]),0) as avg_value from [%s] ",sCharValueField.c_str(),sCharValueField.c_str(),sCharValueField.c_str(),sTable.c_str());
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), CPlantInfoBase::TimeConvertString(dtEnd));
		float fMaxValue =.0,fMinValue =.0,fAvgValue =.0;int nloop =0;
		while (pDB->More())
		{
			if (! sCharValueField.empty())
			{
				fMaxValue = pDB->GetDouble("max_value");
				fMinValue = pDB->GetDouble("min_value");
				fAvgValue = pDB->GetDouble("avg_value");
			}
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itMap->first.c_str());
			pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fMaxValue);
			pFixComm->SetItem(FIELD_SERVICE_CHANN2VALUE,fMinValue);
			pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE,fAvgValue);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++nCount;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	mapChann.clear();
	return nCount;
}

/**监测站级历史报警统计，优化读取数据，读取20条记录以后的特征值和转速
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065
*/
int CTransactData::GetMonitorAlarmStatChar(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1,iRev=0;
	float fValue = -1.0;
	string sSuffix,sTable,sAlias,sCharValueField,lstrFieldRev;
	CBHDateTime tStart=CBHDateTime::GetCurrentTime();
	CBHDateTime tEnd=CBHDateTime::GetCurrentTime();
	S_ChannInfo channInfo;
	int icChannType;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	string sChann=pFixComm->Get(FIELD_SERVICE_CHANN1);
	int iAlarmID=pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cChannType,gc_cChannTable);
	pDB->Cmd(" WHERE [%s] = '%s' ",gc_cChannID, sChann.c_str());
	if (pDB->More())
	{
		icChannType = pDB->GetLong(gc_cChannType);
		index++;
	}
	CHANNTYPE iChannType= GE_ALLPROC;
	iChannType = (CHANNTYPE)(icChannType);
	if (iChannType>100)
		iChannType=GE_AXISLOCATIONCHANN;
	E_TBL_CHANNTYPE channTblType = GetChannOneType( iChannType );
	if ( channTblType == E_TBL_CHANNTYPE_VIB)
	{
		if ( iChannType==GE_AXISLOCATIONCHANN)
		{
			sTable = "ALARM_AXIS_";
			sCharValueField = "";
		}
		else
		{
			channInfo = pPlantInfo->GetSpecVibChannInfo(sChann.c_str());
			sTable = "ALARM_VIB_";
			if (channInfo._iChannType == GE_AXIALCHANN)
				sCharValueField = "THREEPHASE_";
			else
				sCharValueField = "ALL_";
			sCharValueField.append(channInfo._cChannNo);

			lstrFieldRev = "REV_";
			lstrFieldRev.append(channInfo._cChannNo);
		}
	}
	else if( channTblType == E_TBL_CHANNTYPE_PROC)
	{
		sTable = "ALARM_PROC_";
		channInfo = pPlantInfo->GetSpecProcChannInfo(sChann.c_str());
		sCharValueField = gc_cProcValue;
		lstrFieldRev = gc_cRev;
	}
	else
	{
		channInfo = pPlantInfo->GetSpecDynChannInfo(sChann.c_str());
		sTable = "ALARM_DYN_";
		sCharValueField = "ZERO_";
		sCharValueField.append(channInfo._cChannNo);
		lstrFieldRev = "REV_";
		lstrFieldRev.append(channInfo._cChannNo);
	}

	SwitchDbNameEx(pDB,sSuffix.c_str());
	sTable.append(sSuffix);
	if (iChannType ==GE_AXISLOCATIONCHANN)
	{
		pDB->Cmd("SELECT top 1 [%s],[%s],[%s] FROM [%s] ",gc_cCoor_X,gc_cCoor_Y,gc_cRev,sTable.c_str());
	}
	else
	{
		if (sCharValueField.empty())
			pDB->Cmd("SELECT top 1 [%s] FROM [%s] ",gc_cRev,sTable.c_str());
		else
			pDB->Cmd("SELECT top 1 [%s],[%s] FROM [%s] ", sCharValueField.c_str(), lstrFieldRev.c_str(),sTable.c_str());
	}
	pDB->Cmd(" WHERE [%s]=%d order by datetime",gc_cEventID,iAlarmID);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		iRev = pDB->GetLong(lstrFieldRev.c_str());
		if (!sCharValueField.empty())
			fValue = pDB->GetDouble(sCharValueField.c_str());
		if (iChannType ==GE_AXISLOCATIONCHANN)
		{
			float fCooX=.0,fCooY=.0;
			fCooX=pDB->GetDouble(gc_cCoor_X);
			fCooY=pDB->GetDouble(gc_cCoor_Y);
			fValue=sqrt(pow(abs(fCooX),2)+pow(abs(fCooY),2));
		}
		//CString sTemp;sTemp.Format("特征值:%f,报警类型:%d",fValue,iRev);
		//CHZLogManage::Debug(sTemp,"CTransactData","GetMonitorAlarmStatChar");
		pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
		pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
		pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,icChannType);
		index++;
	}
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**得到指定公司、分厂、机组、测点和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID                         FIELD_SYSTEM_COMPANY
*		   分厂名                         FIELD_SYSTEM_FACTORY
*          机组ID                         FIELD_SERVICE_PLANTID
*          水平方向振动测点ID             FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID             FIELD_SERVICE_CHANNID_VER
*          轴承位置描述                   FIELD_SYSTEM_AXISLOCATION
*          波形数据个数                   FIELD_SERVICE_OFF_WAVE_NUM
*          时间                           FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
*          转子旋向：                     FIELD_SYSTEM_TYPE   //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS //1:为反进动,2为正进动
#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK           21066
*/
int CTransactData::GetPurifyAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	int iErrorCode=-1;
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo ==NULL)
		return -1;
	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	string sLocation=pFixComm->GetItem(FIELD_SYSTEM_AXISLOCATION);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	CString strTime =  CPlantInfoBase::TimeConvertString(timeOff);
	CString strRevType="";
	int iSmpFreq=0,iSmpNum=0,iRev=0;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	BYTE *pVerBytes=NULL;
	BYTE *pHorBytes=NULL;
	double *pVerWaveData=NULL;
	double *pHorWaveData=NULL;
	vector<SSigParam> vSigParamV,vSigParamH;
	long iAllBytes=0;
	iWaveNum=iWaveNum<100?513:iWaveNum;
	//CString sTemp;sTemp.Format("sChannIDHor:%s,sChannIDVer:%s,sLocation:%s,strTime:%s,iWaveNum:%d",sChannIDHor,sChannIDVer,sLocation.c_str(),strTime,iWaveNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetPurifyAxesTrack");

	S_ChannInfo channInfoHor;
	S_ChannInfo channInfoVer;
	string sChannNoVer;
	string sChannNoHor;

	//HZLOG_DEBUG(STR("ID_H=%s, ID_V=%s", sChannIDHor.c_str(), sChannIDVer.c_str()));
	if (sChannIDHor.empty()||sChannIDVer.empty())
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),gc_cLocation,sLocation.c_str());
		if (pDB->More())
		{
			sChannNoHor=pDB->Get(gc_cHorChann);
			sChannNoVer=pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		channInfoHor = pPlantInfo->GetSpecVibChannNoInfo(sChannNoHor);
		channInfoVer = pPlantInfo->GetSpecVibChannNoInfo(sChannNoVer);
		sChannIDHor  = channInfoHor._cChannID;
		sChannIDVer  = channInfoVer._cChannID;
	}
	else
	{
		channInfoVer = pPlantInfo->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlantInfo->GetSpecVibChannInfo( sChannIDHor );
		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;

		//HZLOG_DEBUG(STR("NO_H=%s, NO_V=%s", sChannNoHor.c_str(), sChannNoVer.c_str()));
	}

	//要提取波形数据的表单名字
	CString strTable;
	CString strShowField;

	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_VIBCHANN, pDB,timeOff, iTableType, sSuffix),sSuffix.c_str());
	strShowField.Format("%s,%s,%s",
		ConsVibChannField(sChannNoVer,true),
		ConsVibChannField(sChannNoHor,true),
		ConsCommonField());
	//构造查询Sql语句
	
	pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
	pDB->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);

	//HZLOG_DEBUG(STR("SQL=%s", pDB->GetSql()));

	BOOL lbGetDataSucc = TRUE;

	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB->More()))
	{
		if (pDB->GetLastError() < 0)
		{
			lbGetDataSucc = FALSE;
			goto CLEAN_GETAXESTRACT;
		}

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
		{
			lbGetDataSucc = FALSE;

			goto CLEAN_GETAXESTRACT;
		}
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN, iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
		pDB->Cmd(" WHERE %s='%s'",gc_cTime,strTime);
		//HZLOG_DEBUG(STR("SQL=%s", pDB->GetSql()));
	}

	if (!lbGetDataSucc)
	{
		strTable.Format("%s%s",GetTblNamePrefixAlarmSud(GE_VIBCHANN,SEARCH_ALARM),sSuffix.c_str());
		pDB->Cmd("SELECT %s FROM [%s]",strShowField,strTable);    
		pDB->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);

		if (pDB->More())
		{
			if (pDB->GetLastError() < 0)
			{		
				return -1;
			}
		}
	}


	GetSpecWaveByTimeRange(pDB,strTable,iTableType,timeOff,strShowField,SimADO_GetSampleNumEx(pDB, sChannNoHor),sChannNoHor.c_str(),channInfoHor._iDataSource);


	//得到波形数据
	iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannNoHor);
	iSmpNum  = SimADO_GetSampleNumEx(pDB, sChannNoHor);
	iRev     = SimADO_GetRevEx(pDB, sChannNoHor);

	double f0 = (double)iRev/60;
	double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
	int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM;
	vSigParamV.resize(MLcNum+MHcNum);
	vSigParamH.resize(MLcNum+MHcNum);

	{
	#if 0
	 CString sTemp;
	 sTemp.Format("_iSmpFreq:%d,_iSmpFreq:%d",iSmpFreq,iSmpNum);
	 BHTRACE_DEBUG(sTemp);
	#endif
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (iSmpNum>1)
	{
		iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
		pHorBytes=new BYTE[iAllBytes];
		//得到水平方向的波形数据
		strRevType.Format("WAVE_%s", sChannNoHor.c_str());
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, strRevType, pHorBytes, iAllBytes,sWaveHead);
		int iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		if (iWaveNum<=0)
		{
			KD_DeletePoint(pHorBytes);
			return -1;
		}
		pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
        memcpy( pHorWave,pHorBytes,iAllBytes);
		pHorWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
        pPlantInfo->ConvertDTWaveToDouble(pHorWaveData,pHorWave,iWaveNum,sWaveHead);

		 //HZLOG_ARRAY("pHorWaveData", pHorWaveData, iWaveNum);
        //writefile(pHorWaveData,iWaveNum,3);
		//得到垂直方向的波形数据
		pVerBytes=new BYTE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		strRevType.Format("WAVE_%s", sChannNoVer.c_str());
		iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
		SimADO_SafeGetBufferEx(pDB, strRevType, pVerBytes, iAllBytes,sWaveHead);
		iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	    iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
        memcpy( pVerWave,pVerBytes,iAllBytes);
		pVerWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
        pPlantInfo->ConvertDTWaveToDouble(pVerWaveData,pVerWave,iWaveNum,sWaveHead);
		//HZLOG_ARRAY("pVerWaveData", pVerWaveData, iWaveNum);
        //writefile(pVerWaveData,iWaveNum,4);

		/* 进动方向 */
		int iPression = 0;
		int iOrbitShape=0;
		CSynopsis2KB calcOrbit;
		int icalcOrbit = -1;
		double f0 = (double)iRev/60;
		if (f0 >.0)
		{
			try
			{
				icalcOrbit = calcOrbit.GetOrbitShape(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,iPression,iOrbitShape);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			//-----计算全频谱 begin-------
			CSigMath calcFullSpecInfo;
			double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
			int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM,iFullSpecInfoSize=0;
			int index = 0;
			vector<double> vFullSpecX;
			vector<double> vFullSpecY;
			vFullSpecX.resize(iWaveNum);
			vFullSpecY.resize(iWaveNum);
			BEGIN_ERROR_HANDLE;					
			index = calcFullSpecInfo.FullSpectrumII(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,vFullSpecX,vFullSpecY);
			END_ERROR_HANDLE;

			typedef pair<float,float> FreqAmp;
			vector<FreqAmp> vFullInfo;
			iFullSpecInfoSize = vFullSpecX.size();

			if (index > -1)
			{
				index = 0;
				for (int iNum =0;iNum<iFullSpecInfoSize;iNum++)
				{
					index++;

					vFullInfo.push_back(make_pair(vFullSpecX[iNum],vFullSpecY[iNum]));
				}

				int iFullInfoSize = vFullInfo.size();

				float lfLeftData = -1;
				float lfRightData = -1;
				int lbFound = 0;
				double ldblDf = iSmpFreq*1.0/iWaveNum;
				double ldblLeftSearchFreqBegin = -(f0+2*ldblDf);
				double ldblLeftSearchFreqEnd   = -(f0-2*ldblDf);

				double ldblRightSearchFreqBegin = (f0-2*ldblDf);
				double ldblRightSearchFreqEnd   = (f0+2*ldblDf);

				for (int i=0;i<iFullInfoSize;i++)
				{
					FreqAmp loParam = vFullInfo[i];
					float lfDiff = abs(abs(loParam.first)-f0);
					float lfCurrentFreq = loParam.first;
					if (lfCurrentFreq<ldblLeftSearchFreqBegin)
					{
						continue;
					}
					if (lfCurrentFreq > ldblRightSearchFreqEnd)
					{
						break;
					}
					if ((lfCurrentFreq<=ldblLeftSearchFreqEnd) 
						&& (lfCurrentFreq>= ldblLeftSearchFreqBegin))
					{
						float lfCurrent = loParam.second;

						if (lfCurrent> lfLeftData)
						{
							lfLeftData = lfCurrent;
						}

					}

					if ((lfCurrentFreq<=ldblRightSearchFreqEnd) 
						&& (lfCurrentFreq>= ldblRightSearchFreqBegin))
					{
						float lfCurrent = loParam.second;

						if (lfCurrent> lfRightData)
						{  
							lfRightData = lfCurrent;
						}

					}

				}

				if ((lfLeftData>0)&&(lfRightData>0))
				{
					//反进动
					if ((lfLeftData*1.05)>lfRightData)
					{
						iPression = 1;
					}else
					{
						iPression = 2;
					}
				}
			} //if (index > -1)

			//-----计算全频谱 begin-------
		}

		if (icalcOrbit < 0)
		{	
			HZLOG_ERROR(STR("计算轴心进动方向出错"));

		}

		if (iPression == 0) // 如果计算为不确定则为正进动
		{
			iPression = 2;
		}

        //获得提纯轴心轨迹的各个谐波数据

		CSigMath calcAxesData;
		try
		{
			iErrorCode = calcAxesData.SCorbitpurifie_Rp(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth,vSigParamV,vSigParamH);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		//CString sTemp;sTemp.Format("iErrorCode:%d,iWaveNum:%d,f0:%f,iSmpFreq:%d,MHcNum:%d,AHcth:%f,MLcNum:%d,ALcth:%f",iErrorCode,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth);BHTRACE_DEBUG(sTemp);
		//HZLOG_DEBUG(STR(sTemp));
		if (iErrorCode>-1)
		{
			int iSize=0;
			iSize=vSigParamH.size();
			for (int iNum =0;iNum<iSize;iNum++)
			{
				if (vSigParamH[iNum]._fFreq!=0 && vSigParamH[iNum]._fAmp!=0)
				{
					iErrorCode++;
					pFixComm->SetItem(FIELD_SERVICE_VERFREQ,vSigParamV[iNum]._fFreq);
					pFixComm->SetItem(FIELD_SERVICE_VERAMP,vSigParamV[iNum]._fAmp);
					pFixComm->SetItem(FIELD_SERVICE_VERPHASE,vSigParamV[iNum]._fPhase);
					pFixComm->SetItem(FIELD_SERVICE_HORFREQ,vSigParamH[iNum]._fFreq);
					pFixComm->SetItem(FIELD_SERVICE_HORAMP,vSigParamH[iNum]._fAmp);
					pFixComm->SetItem(FIELD_SERVICE_HORPHASE,vSigParamH[iNum]._fPhase);
					pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
					//CString sTemp;sTemp.Format("提纯轨迹:iNum:%d,Fv:%f,Av:%f,Fh:%f,Ah:%f",iNum,vSigParamV[iNum]._fFreq/f0,vSigParamV[iNum]._fAmp,vSigParamH[iNum]._fFreq/f0,vSigParamH[iNum]._fAmp);HZLOG_DEBUG(STR(sTemp));
					//CString sTemp;sTemp.Format("iNum:%d,Fv:%f,Av:%f,Pv:%f,Fh:%f,Ah:%f,Ph:%f",iNum,vSigParamV[iNum]._fFreq,vSigParamV[iNum]._fAmp,vSigParamV[iNum]._fPhase,vSigParamH[iNum]._fFreq,vSigParamH[iNum]._fAmp,vSigParamH[iNum]._fPhase);BHTRACE_DEBUG(sTemp);

					//转速
					pFixComm->SetItem(FIELD_SERVICE_OFF_REV,iRev);

					S_VibCharValue vibValue;
					/* 得到水平特征值 */
					vibValue=ReadVibCharValue(sChannNoHor,pDB);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));

					/* 得到垂直特征值 */
					vibValue=ReadVibCharValue(sChannNoVer,pDB);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibValue,sizeof(S_VibCharValue));

					/* 转子旋向 */
					int iRotation = -1;
					iRotation = pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);


					pFixComm->SetItem(FIELD_SYSTEM_STATUS,iPression);

					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
			}
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		iErrorCode= -1;
	}

	//HZLOG_DEBUG(STR("iErrorCode=%d", iErrorCode));
CLEAN_GETAXESTRACT:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWaveData);
	KD_DeletePoint(pHorWaveData);
	KD_DeletePoint(pVerBytes);
	KD_DeletePoint(pHorBytes);
	return iErrorCode;
}

/**得到指定公司、分厂机组和测点指定时刻的倒谱数据
*输入参数：
*          公司ID       FIELD_SYSTEM_COMPANY
*	       分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          测点ID       FIELD_SERVICE_OFF_CHANN1
*          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
*          时间         FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          波形数据        FIELD_SERVICE_OFF_WAVE
*          倒谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          倒谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          倒谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          转速            FIELD_SERVICE_OFF_REV
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM       21067
*/
int CTransactData::GetSpecChannCepstrum(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	string sChann1=pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	iWaveNum=ReadVibChannSpecTimeWaveAndCepstrum(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1,iWaveNum,timeOff);
	pFixComm->Write(pFixComm->GetAci());
	return iWaveNum;
}

/**往复机械报警查询，得到指定报警ID最先报警的测点名、特征值趋势
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*            分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            报警测点名              FIELD_SERVICE_CHANN1
*            测点类型                FIELD_SERVICE_CHANNTYPE
*            报警测点的特征值结构体  FIELD_SERVICE_CHARVALUE_S
*            时间                    FIELD_SERVICE_TIME
*            对应时间的微秒          FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068
*/
int CTransactData::GetSpecAlarmIDCharValues(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	int nCount =-1,icChannType=-1;
	float fValue=.0;
	string sSuffix,sChann,sChannName,sTable,sCharValueField;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	int iAlarmID = pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	sTable = pPlantInfo->GetAlarmLogTblName();
	//CString sTmp;sTmp.Format("%s,%s,%s,%s,%d",sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),sSuffix.c_str(),iAlarmID);
	//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmIDCharValues");
	pDB->Cmd(" select top 1 [%s],[%s] from [%s] ",gc_cName,gc_cStart_T,sTable.c_str());
	pDB->Cmd(" where [%s]=%d ",gc_cEventID,iAlarmID);
	pDB->Cmd(" order by [%s],[%s] desc ",gc_cEventID,gc_cName);
	if (pDB->More())
	{
		sChannName = pDB->Get(gc_cName);
	}
	else
		return -2;
	SwitchDbNameEx(pDB,g_strMainDbName);
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cChannType,gc_cChannTable);
	pDB->Cmd(" WHERE [%s] = '%s' ",gc_cChannNo, sChannName.c_str());
	if (pDB->More())
	{
		icChannType = pDB->GetLong(gc_cChannType);
		nCount++;
	}
	//sTmp.Format("sChannName:%s,icChannType:%d",sChannName.c_str(),icChannType);BHTRACE_DEBUG(sTmp);
	//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmIDCharValues");
	S_ChannInfo channInfo;
	CHANNTYPE iChannType= GE_ALLPROC;
	iChannType = (CHANNTYPE)(icChannType);
	if (sChannName=="1")
		iChannType=GE_AXISLOCATIONCHANN;
	E_TBL_CHANNTYPE channTblType = GetChannOneType( iChannType );
	if ( channTblType == E_TBL_CHANNTYPE_VIB)
	{
		if ( iChannType == GE_AXISLOCATIONCHANN)
		{
			sTable = "ALARM_AXIS_";
			sCharValueField = "";
		}
		else
		{
			sTable = "ALARM_VIB_";
			channInfo = pPlantInfo->GetSpecVibChannNoInfo(sChannName.c_str());
			if (channInfo._iChannType == GE_AXIALCHANN)
				sCharValueField = "THREEPHASE_";
			sCharValueField.append(channInfo._cChannNo);
		}
	}
	else if( channTblType == E_TBL_CHANNTYPE_PROC)
	{
		sTable = "ALARM_PROC_";
		channInfo = pPlantInfo->GetSpecProcChannNoInfo(sChannName.c_str());
		sCharValueField = gc_cProcValue;
	}
	else
	{
		sTable = "ALARM_DYN_";
		channInfo = pPlantInfo->GetSpecDynChannNoInfo(sChannName.c_str());
	}
	sTable.append(sSuffix);
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//sTmp.Format("sChannName:%s,icChannType:%d,iChannType:%d,sTable:%s,sCharValueField:%s,channInfo:%s",sChannName.c_str(),icChannType,iChannType,sTable.c_str(),sCharValueField.c_str(),channInfo._cChannNo);BHTRACE_DEBUG(sTmp);
	//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmIDCharValues");
	//得到各种测点的报警特征值数据
	pDB->Cmd("SELECT * FROM [%s] ",sTable.c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cEventID,iAlarmID);
	pDB->Cmd(" ORDER BY [%s]  ",gc_cTime);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		if ( channTblType == E_TBL_CHANNTYPE_VIB)
		{
			if ( iChannType == GE_AXISLOCATIONCHANN)
			{
				float fCooX=.0,fCooY=.0;
				fCooX=pDB->GetDouble(gc_cCoor_X);
				fCooY=pDB->GetDouble(gc_cCoor_Y);
				fValue=sqrt(pow(abs(fCooX),2)+pow(abs(fCooY),2));
				pFixComm->SetItem(FIELD_SERVICE_CHARVALUE_S, fValue);
				icChannType=iChannType;
				strncpy(channInfo._cChannID,sChannName.c_str(),D_BHKD_CHANNID_LEN-1);
			}
			if (channInfo._iChannType == GE_AXIALCHANN)
			{
				if (! sCharValueField.empty())
					fValue = pDB->GetDouble(sCharValueField.c_str());
				pFixComm->SetItem(FIELD_SERVICE_CHARVALUE_S, fValue);
				icChannType=icChannType;
			}
			else
			{
				S_VibCharValue vibValue=ReadVibCharValue(channInfo._cChannNo,pDB);
				//CString sTemp;sTemp.Format("channInfo._cChannNo:%s,vibValue:%f,%f,%f",channInfo._cChannNo,vibValue._fCharAll,vibValue._fCharHalf,vibValue._fCharOne);BHTRACE_DEBUG(sTemp);
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
			}
		}
		else if( channTblType == E_TBL_CHANNTYPE_PROC)
		{
			if (! sCharValueField.empty())
				fValue = pDB->GetDouble(sCharValueField.c_str());
			pFixComm->SetItem(FIELD_SERVICE_CHARVALUE_S, fValue);
		}
		else
		{
			S_DynCharValue dynValue=ReadDynCharValue(channInfo._cChannNo,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&dynValue,sizeof(S_DynCharValue));
		}
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,icChannType);
		pFixComm->SetItem(FIELD_SERVICE_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,pDB->GetLong(gc_cMicroSecond));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++nCount;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**往复机械报警查询,得到指定时间的指定活塞杆沉降或动态压力测点的报警记录
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*          分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点            FIELD_SERVICE_OFF_CHANN1
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          测点的X轴数据    FIELD_SERVICE_ZOOMSPECTRUM
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          转速             FIELD_SERVICE_REV
*          吸气阀开启相位角     FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角     FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM4
*          自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD     21069
*/
int CTransactData::GetSpecAlarmTimeDynRecord(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	DATATYPE_WAVE *pWave=NULL;
	DATATYPE_WAVE *pXCoor=NULL;
	DATATYPE_WAVE *pSpectrum=NULL;
	DATATYPE_WAVE *pWaveNotDC=NULL;
	string sCompany,sFactory,sPlant,sSuffix;
	CPlantInfoOffLine *pPlantInfo=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	string sChann = pFixComm->Get(FIELD_SERVICE_OFF_CHANN1);
	int   iWaveNum = pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	iWaveNum = iWaveNum <100 ? 513: iWaveNum;
	int iSmpNums = pPlantInfo->GetPlantInfo()._iSmpNum;
	if (iWaveNum<=0 || iSmpNums<=0)
	{
		return -1;
	}
	pWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pWaveNotDC=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

	float *pfWave = NULL;
	float *pfWaveNotDC = NULL;
	float *pfSpectrum = NULL;

	vector<float> lvfWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfWaveNotDC(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lvfSpectrum(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &lvfWave.front();
	pfWaveNotDC = &lvfWaveNotDC.front();
	pfSpectrum = &lvfSpectrum.front();

	S_DynCharValue dynCharValue;
	S_ChannInfo channInfo;
	int iStartIndex = 0,iEndIndex = 0;
	channInfo = pPlantInfo->GetSpecDynChannInfo(sChann);
	int iChannType = channInfo._iChannType;
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	//CString sTmp;sTmp.Format("%s,%s,%s,%s,%d,%s",sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),sSuffix.c_str(),iEventID,CPlantInfoBase::TimeConvertString(timeOff));
	//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmTimeDynRecord");
	CString strShowField="";
	strShowField.Format(" %s,%s,CUSTOM_PARAM_%s ",
		ConsDynChannField(channInfo._cChannNo,true),
		ConsCommonField(),
		channInfo._cChannNo);

	//获取报警数据
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	CString strTable;
	E_TREND_TBL_TYPE iTableType = GE_TREND_TBLTYPE_SECOND;

	strTable.Format("%s%s",GetTblNamePrefixForWave2Ex( GE_DYNPRESSCHANN, pDB,timeOff, iTableType, sSuffix),sSuffix.c_str());

	pDB->Cmd("SELECT %s FROM %s ",strShowField,strTable);
	pDB->Cmd(" WHERE [%s] = '%s'",gc_cTime, CPlantInfoBase::TimeConvertString(timeOff));

	if(!pDB->More())
	{
		pDB->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
		pDB->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cTime, CPlantInfoBase::TimeConvertString(timeOff));
		if(!(pDB->More()))
		{
			//当按照精确时间查询失败时，则按照相邻的一段时间查询数据
			CString strSqlWhere;
			COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
			CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
			CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
			dtStart = timeOff - timeSpan;
			dtEnd   = timeOff + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ",
				gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			//获取报警数据
			pDB->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
			pDB->Cmd(" WHERE %s AND [%s]=%d", strSqlWhere,gc_cEventID,iEventID);
			if(!(pDB->More()))
			{
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());

				KD_DeletePoint(pWave);
				KD_DeletePoint(pWaveNotDC);
				return -2;
			}
		}
	}
	
	float fRev=.0;
	int iSmpFreq=1,iNums=1,iRev=0;

	iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, channInfo);
	iNums    = SimADO_GetSampleNumEx(pDB, channInfo);
	iRev     = SimADO_GetRevEx(pDB, channInfo);

	fRev=(float)iRev;
	//CString sTmp;sTmp.Format("iSmpFreq:%d,iSmpNums:%d,iNums:%d",iSmpFreq,iSmpNums,iNums);BHTRACE_DEBUG(sTmp);
	dynCharValue=ReadDynCharValue(channInfo._cChannNo,pDB);
	//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmTimeDynRecord");
	if (iNums>1)
	{//为了防止采样点数为0的情况下，问题异常。
		long iBytes=iNums*sizeof(DATATYPE_WAVE);
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, ConsDynChannSpecTypeField(channInfo._cChannNo, -1).c_str(), (BYTE*)pWave,iBytes,sWaveHead);
		int iRealNums=iBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealNums?iRealNums:iWaveNum;
		//CString sTmp;sTmp.Format("iWaveNum:%d,iRealNums:%d",iWaveNum,iRealNums);BHTRACE_DEBUG(sTmp);
		//CHZLogManage::Debug( sTmp, "CTransactData","GetSpecAlarmTimeDynRecord");
		if (iChannType==GE_RODSINKCHANN)
		{
			//活塞杆轴心轨迹波形和频谱
			iSmpNums = iWaveNum >iSmpNums ? iSmpNums: iWaveNum;
			
			pPlantInfo->ConvertDTWaveToFloat(pfWave,pWave,iWaveNum,sWaveHead);
			pPlantInfo->ConvertFloatToDT(pWave,pfWave,iWaveNum);

			pPlantInfo->SubtractWaveDC(pWave,pWaveNotDC,iWaveNum);
			
			pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWaveNotDC, iWaveNum * sizeof(DATATYPE_WAVE));
			pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iWaveNum);
			pSpectrum = new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			int lnWaveNum = iSmpNums;

			pPlantInfo->SpectrumAnalysis(pfWave, lnWaveNum, pfSpectrum, channInfo._iSensorType);
			int            iLines    = lnWaveNum /2.56+1;
			float          fDf       = (float)iSmpFreq/lnWaveNum;

			pPlantInfo->ConvertFloatToDT(pSpectrum,pfSpectrum,iLines);
			
			pFixComm->SetItemBuf( FIELD_SERVICE_OFF_FREQWAVE, (char *)pSpectrum ,iLines * sizeof(DATATYPE_WAVE) );
			//writefile(pSpectrum,iLines,555);
			pFixComm->SetItem( FIELD_SERVICE_OFF_FREQWAVE_DF, fDf);
			pFixComm->SetItem( FIELD_SERVICE_OFF_FREQWAVE_NUM, iLines);

			if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)pfSpectrum,iLines*sizeof(float));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
			}
		}
		else
		{
			//动态压力波形和P-V图
			pPlantInfo->ConvertDTWaveToFloat(pfWave,pWave,iWaveNum,sWaveHead);
			pPlantInfo->ConvertFloatToDT(pWave,pfWave,iWaveNum);
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo._cChannNo,timeOff,iStartIndex,iEndIndex,true);
			pXCoor = new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			int iCylinderNo = pPlantInfo->GetSpecChannCylinderNo(pDB,sChann);
			pFixComm->SetItem( FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
			if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iWaveNum))
			{
				pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)pWave, iWaveNum * sizeof(DATATYPE_WAVE));
				iWaveNum = pPlantInfo->GetPVXCoorData( iSmpFreq, iWaveNum, GE_PVTYPE_PV, fRev,iCylinderNo,pXCoor);
				if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfWave,iWaveNum*sizeof(float));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			else
			{
				pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE, (char *)&(pWave[iStartIndex]), (iEndIndex-iStartIndex) * sizeof(DATATYPE_WAVE));
				iWaveNum = pPlantInfo->GetPVXCoorData(iSmpFreq,iEndIndex-iStartIndex,GE_PVTYPE_PV,iCylinderNo,pXCoor);
				if(sWaveHead.GetLength() > 1 && (int)sWaveHead.m_pParam[1] == 1)
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)sWaveHead.m_pParam[1]);
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)&pfWave[iStartIndex],(iEndIndex-iStartIndex)*sizeof(float));
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
				}
			}
			//writefile(pXCoor,iWaveNum,222);
			pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, iWaveNum);
			pFixComm->SetItemBuf( FIELD_SERVICE_ZOOMSPECTRUM, (char *)pXCoor ,iWaveNum * sizeof(DATATYPE_WAVE) );
		}
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		pFixComm->SetItemBuf( FIELD_SERVICE_CHARVALUE_S,(char *)&dynCharValue, sizeof(S_DynCharValue));
		pFixComm->SetItem( FIELD_SERVICE_REV, fRev);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo._chAlarmParam._pThLearnParamF[8]._fParam4);
		//得到自定义参数信息
		float  *pfCustomParom =new float[GD_CUSTOMPARAM_BUFFSIZE];
		memset(pfCustomParom,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
		CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB, sTmp, (BYTE*)pfCustomParom, iAllBytes,sWaveHead);
		/*CString sTemp;
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			sTemp.Format("Dyn:pfCustomParom[%d]:%f",i,pfCustomParom[i]);
            BHTRACE_DEBUG(sTemp);
		}*/
		pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBytes);
	    KD_DeletePoint(pfCustomParom);
	}
	else
		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM, 0);
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint( pWave);
	KD_DeletePoint(pWaveNotDC);
	KD_DeletePoint( pSpectrum);
	KD_DeletePoint( pXCoor);
	return iWaveNum;
}

/**得到指定公司、分厂机组和一对振动测点指定时刻的全谱数据
*输入参数：
*          公司ID               			FIELD_SYSTEM_COMPANY
*		   分厂名               			FIELD_SYSTEM_FACTORY
*          机组ID               			FIELD_SERVICE_PLANTID
*          水平方向振动测点ID  			    FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   		    FIELD_SERVICE_CHANNID_VER
*          轴承位置描述        			    FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         			FIELD_SERVICE_OFF_WAVE_NUM
*          时间                             FIELD_SERVICE_OFF_TIME
*          是否应用新算法                   FIELD_SERVICE_FUNCTION_TYPE
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
#define     SERVICE_SERVICE_OFF_GETFULLSPECSTRUM          21070
*/
int CTransactData::GetSpecTimeFullSpectrum(ICommInterFace *pFixComm, IDBInterFace *pDB)
{
	int iErrorCode=-1;
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
	if ( pPlantInfo ==NULL)
		return -1;

	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	string sLocation   = pFixComm->GetItem(FIELD_SYSTEM_AXISLOCATION);

	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
	timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	CString strTime =  CPlantInfoBase::TimeConvertString(timeOff);
	CString strRevType="";
	int iSmpFreq=0,iSmpNum=0,iRev=0;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	BYTE *pVerBytes=NULL;
	BYTE *pHorBytes=NULL;
	double *pVerWaveData=NULL;
	double *pHorWaveData=NULL;
	vector<SSigParam> mapFullSpecInfo;
	long iAllBytes=0;
	iWaveNum=iWaveNum<100?513:iWaveNum;

	S_ChannInfo channInfoHor;
	S_ChannInfo channInfoVer;
	string sChannNoVer;
	string sChannNoHor;

	//CString sTemp;sTemp.Format("sChannIDHor:%s,sChannIDVer:%s,sLocation:%s,strTime:%s,iWaveNum:%d",sChannIDHor,sChannIDVer,sLocation.c_str(),strTime,iWaveNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetPurifyAxesTrack");
	if ((sChannIDHor.empty())||(sChannIDVer.empty()))
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		//查询得到轴心位置的对应的垂直测点和水平测点
		SwitchDbNameEx(pDB,g_strMainDbName);
		pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cVerChann,gc_cHorChann,gc_cPlantAxisTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",
			gc_cCompany,sCompany.c_str(),
			gc_cFactory_Name,sFactory.c_str(),
			gc_cPlantNo,pPlantInfo->GetPlantNo().c_str(),
			gc_cLocation,sLocation.c_str());
		if (pDB->More())
		{
			sChannNoHor = pDB->Get(gc_cHorChann);
			sChannNoVer = pDB->Get(gc_cVerChann);
		}
		else
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		channInfoVer = pPlantInfo->GetSpecVibChannNoInfo( sChannNoVer );
		channInfoHor = pPlantInfo->GetSpecVibChannNoInfo( sChannNoHor );
		sChannIDHor  = channInfoHor._cChannID;
		sChannIDVer  = channInfoVer._cChannID;
	}
	else {
		channInfoVer = pPlantInfo->GetSpecVibChannInfo( sChannIDVer );
		channInfoHor = pPlantInfo->GetSpecVibChannInfo( sChannIDHor );
		sChannNoHor  = channInfoHor._cChannNo;
		sChannNoVer  = channInfoVer._cChannNo;
	}


	//要提取波形数据的表单名字
	CString strTable;
	CString strShowField;
	E_TREND_TBL_TYPE iTableType=GE_TREND_TBLTYPE_SECOND;
	strTable.Format("%s%s",GetTblNamePrefixForWave2( GE_VIBCHANN, timeOff, iTableType),sSuffix.c_str());
	strShowField.Format("%s,%s,%s",
		ConsVibChannField(sChannNoVer,true),
		ConsVibChannField(sChannNoHor,true),
		ConsCommonField());
	//构造查询Sql语句
	SwitchDbNameEx(pDB,sSuffix.c_str());
	pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
	pDB->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
	iTableType=GE_TREND_TBLTYPE_MONTH;
	while(!(pDB->More()))
	{
		if (pDB->GetLastError() < 0)
			goto CLEAN_GETAXESTRACT;

		if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			goto CLEAN_GETAXESTRACT;

		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN, iTableType),sSuffix.c_str());
		iTableType=(E_TREND_TBL_TYPE)(iTableType-1);
		pDB->Cmd("SELECT %s FROM [%s] ",strShowField,strTable);
		pDB->Cmd(" WHERE %s='%s'",gc_cTime,strTime);
	}

	//得到波形数据
	iSmpFreq = SimADO_GetSampleFrequencyEx(pDB, sChannNoHor);
	iSmpNum  = SimADO_GetSampleNumEx(pDB, sChannNoHor);
	iRev     = SimADO_GetRevEx(pDB, sChannNoHor);

	double f0 = (double)iRev/60;
	double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
	int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM,iFullSpecInfoSize=0;
	mapFullSpecInfo.resize(2*(MLcNum+MHcNum));

	//是否按照新的算法FullSpectrumII计算，默认按照FullSpectrum计算
	int lnSigMathType = pFixComm->GetLong(FIELD_SERVICE_FUNCTION_TYPE);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (iSmpNum>1)
	{
		iAllBytes=iSmpNum *sizeof(DATATYPE_WAVE);
		pHorBytes=new BYTE[pPlantInfo->GetMiddleWareWavePointNumMax()*sizeof(DATATYPE_WAVE)];
		//得到水平方向的波形数据
		strRevType.Format("WAVE_%s", sChannNoHor.c_str());
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, strRevType, pHorBytes, iAllBytes,sWaveHead);
		int iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		pHorWave=new DATATYPE_WAVE[pPlantInfo->GetMiddleWareWavePointNumMax()];
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
		memcpy( pHorWave,pHorBytes,iAllBytes);
		pHorWaveData=new double[pPlantInfo->GetMiddleWareWavePointNumMax()];
		pPlantInfo->ConvertDTWaveToDouble(pHorWaveData,pHorWave,iWaveNum,sWaveHead);
		//writefile(pHorWaveData,iWaveNum,3);
		//得到垂直方向的波形数据
		pVerBytes=new BYTE[pPlantInfo->GetMiddleWareWavePointNumMax() *sizeof(DATATYPE_WAVE)];
		strRevType.Format("WAVE_%s", sChannNoVer.c_str());
		iAllBytes=iSmpNum*sizeof(DATATYPE_WAVE);
		SimADO_SafeGetBufferEx(pDB, strRevType, pVerBytes, iAllBytes,sWaveHead);
		iRealWaveNum=iAllBytes/sizeof(DATATYPE_WAVE);
		iWaveNum=iWaveNum>iRealWaveNum?iRealWaveNum:iWaveNum;
		pVerWave=new DATATYPE_WAVE[pPlantInfo->GetMiddleWareWavePointNumMax()];
		iAllBytes=sizeof(DATATYPE_WAVE)*iWaveNum;
		memcpy( pVerWave,pVerBytes,iAllBytes);
		pVerWaveData=new double[pPlantInfo->GetMiddleWareWavePointNumMax()];
		pPlantInfo->ConvertDTWaveToDouble(pVerWaveData,pVerWave,iWaveNum,sWaveHead);
		//writefile(pVerWaveData,iWaveNum,4);
		//获得提纯轴心轨迹的各个谐波数据
		typedef pair<float,float> FreqAmp;
		vector<FreqAmp> vFullInfo;
		vector<double> vFullSpecX;
		vector<double> vFullSpecY;
		vFullSpecX.resize(iWaveNum);
		vFullSpecY.resize(iWaveNum);

		CSigMath calcFullSpecInfo;
		if (f0>.0)
		{
			try
			{
				if (lnSigMathType == 1)
				{
					iErrorCode = calcFullSpecInfo.FullSpectrumII(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,vFullSpecX,vFullSpecY);
				}
				else
				{
					iErrorCode = calcFullSpecInfo.FullSpectrum(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth,mapFullSpecInfo);
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

			//CString sTemp;sTemp.Format("iErrorCode:%d,iWaveNum:%d,f0:%f,iSmpFreq:%d,MHcNum:%d,AHcth:%f,MLcNum:%d,ALcth:%f",iErrorCode,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth);BHTRACE_DEBUG(sTemp);
			//CHZLogManage::Debug(sTemp,"CTransactData","GetSpecTimeFullSpectrum");

			if (lnSigMathType == 1)
			{
				iFullSpecInfoSize = vFullSpecX.size();

				if (iErrorCode>-1)
				{
					for (int iNum =0;iNum<iFullSpecInfoSize;iNum++)
					{
						iErrorCode++;

						vFullInfo.push_back(make_pair(vFullSpecX[iNum],vFullSpecY[iNum]));
					}
				}
			}
			else
			{
				iFullSpecInfoSize = mapFullSpecInfo.size();

				if (iErrorCode > -1)
				{
					for (int iNum = 0; iNum < mapFullSpecInfo.size(); iNum++)
					{
						if (mapFullSpecInfo[iNum]._fFreq != 0 && mapFullSpecInfo[iNum]._fAmp != 0)
						{
							iErrorCode++;

							vFullInfo.push_back(make_pair(mapFullSpecInfo[iNum]._fFreq,mapFullSpecInfo[iNum]._fAmp));
						}
					}
				}
			}
		}
		int iFullInfoSize =vFullInfo.size();

		while ((iFullInfoSize*sizeof(FreqAmp)) >= 90*1000)
		{
			vFullInfo.pop_back();
			iFullInfoSize =vFullInfo.size();
		}
		
		if (iFullInfoSize > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)&vFullInfo[0],iFullInfoSize*sizeof(FreqAmp));
			pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iFullInfoSize);
			pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
			pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
			//转子旋向
			int iRotation =-1;
			iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);
			//得到水平特征值
			S_VibCharValue vibCharValue;
			vibCharValue=ReadVibCharValue(sChannNoHor,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));
			//得到垂直特征值
			vibCharValue=ReadVibCharValue(sChannNoVer,pDB);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibCharValue,sizeof(S_VibCharValue));
		}else
		{
			pFixComm->SetItem(FIELD_CODE,-1);
		}

	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		iErrorCode= -1;
	}
CLEAN_GETAXESTRACT:
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWaveData);
	KD_DeletePoint(pHorWaveData);
	KD_DeletePoint(pVerBytes);
	KD_DeletePoint(pHorBytes);
	return iErrorCode;
}

/**报警查询功能，查询报警ID报警类型
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID            FIELD_SERVICE_ALARMID
*		     报警开始时间      FIELD_SERVICE_OFF_TIMESTART
*		     报警类型          FIELD_SERVICE_ALARM_TYPE
*            成功标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETALARMLOGINFO         21071
*/
int CTransactData::GetAlarmLogInfo(ICommInterFace *pFixComm, IDBInterFace *pDB)
{
	int index = -1;
	string sSuffix,sTable,sAlias;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	sTable = pPlantInfo->GetAlarmLogTblName();
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cEventID,gc_cStart_T,gc_cType,sTable.c_str());
	pDB->Cmd(" WHERE %s between '%s' AND '%s' ",gc_cStart_T, CPlantInfoBase::TimeConvertString(timeStart), CPlantInfoBase::TimeConvertString(timeEnd));
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SERVICE_ALARMID,pDB->GetLong(gc_cEventID));
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIMESTART,pDB->GetTime(gc_cStart_T));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,ParseAlarmStatus(pDB->GetLong(gc_cType)));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		index++;
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,index);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**往复式系统应力监测，得到指定机组指定时间、指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		   分厂名          　　　  FIELD_SYSTEM_FACTORY
*          机组ID          　　　  FIELD_SERVICE_PLANTID
*          汽缸号                  FIELD_SERVICE_CYLINDERNO
*          缸体振动测点            FIELD_SERVICE_OFF_CHANN1
*          时间                    FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*          振动波形数据   　　　 　FIELD_SERVICE_FREQWAVE2
*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
*		   测点名				   FIELD_SYSTEM_CHANN
*		   测点名2				   FIELD_SYSTEM_CHANN2
*		   测点1角度参数1		   FIELD_SERVICE_ANGLEPARAM1
*		   测点1角度参数2		   FIELD_SERVICE_ANGLEPARAM2
*		   测点1角度参数3		   FIELD_SERVICE_ANGLEPARAM3
*		   测点1角度参数4		   FIELD_SERVICE_ANGLEPARAM4
*		   测点2角度参数1		   FIELD_SERVICE_ANGLEPARAM5
*		   测点2角度参数2		   FIELD_SERVICE_ANGLEPARAM6
*		   测点2角度参数3		   FIELD_SERVICE_ANGLEPARAM7
*		   测点2角度参数4		   FIELD_SERVICE_ANGLEPARAM8
*		   时间				   	   FIELD_SERVICE_TIME 

#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE              21072
*/
int CTransactData::GetSpecTimeDynStressWave(ICommInterFace *pFixComm, IDBInterFace *pDB)
{
	int index = -1;
	string sCompany,sFactory,sPlant,sSuffix,sChann1,sChann2,sStressChann;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo( sCompany, sFactory, sPlant,sSuffix, pFixComm);
	if (pPlantInfo == NULL){
		::SetNullReturnFixcomm((CFixComm*)pFixComm->GetFixComm());
		return -1;
	}

	int iCylinderNo=pFixComm->GetLong(FIELD_SERVICE_CYLINDERNO);
	string sChannVib=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	CBHDateTime timeOff=CBHDateTime::GetCurrentTime();
    timeOff=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	//CString strInfo;strInfo.Format("%s,%d",sChannVib.c_str(),iCylinderNo);
	//CHZLogManage::Debug( strInfo, "CTransactData","GetSpecTimeDynStressWave");
	DATATYPE_WAVE *pWave1=NULL;
	DATATYPE_WAVE *pWave2=NULL;
	DATATYPE_WAVE *pRodLoadWave=NULL;
	float *pfStressWave=NULL;
	float *pfInertiaWave=NULL;
	float *pfRodLoadWave=NULL;
	DATATYPE_WAVE *pVibWave=NULL;
	INT lnWavePointMax = pPlantInfo->GetMiddleWareWavePointNumMax();

	CWaveHead lsWaveHead1;
	CWaveHead lsWaveHead2;
	CWaveHead lsWaveHead3;

	pFixComm->CreateAnswer(pFixComm->GetFunc());
    if (pPlantInfo->GetDynStressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sStressChann)>0)
    {
		//如果有动态应力测点、则直接从数采获得活塞杆应力波形数据
		//BHTRACE_DEBUG("直接从数采获得活塞杆应力波形数据");
		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0){
			::SetNullReturnFixcomm((CFixComm*)pFixComm->GetFixComm());
			return -2;
		}

		//strInfo.Format("%s,%s",sChann1.c_str(),sChann2.c_str());
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;
		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;
		//strInfo.Format("%f,%f",fAngle1,fAngle2);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1,iRev=0;
		int iStartIndex = 0, iEndIndex = 0;
		S_DynCharValue dynCharValue;
		float fRev =.1;
        iSmpNum=pPlantInfo->GetPlantInfo()._iSmpNum;
		//strInfo.Format("%d,%d",iSmpNum,iSmpFreq);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");
		pWave1=new DATATYPE_WAVE[lnWavePointMax];
		pWave2= new DATATYPE_WAVE[lnWavePointMax];
		pRodLoadWave=new DATATYPE_WAVE[lnWavePointMax];
		pfStressWave=new float[lnWavePointMax];
		pfInertiaWave=new float[lnWavePointMax];
		pfRodLoadWave=new float[lnWavePointMax];
		pVibWave= new DATATYPE_WAVE[lnWavePointMax];
		
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;

		float *pfVibWave = NULL;
		vector<float> lvVibWave(lnWavePointMax,0);
		pfVibWave = &lvVibWave.front();

		int iRtn1=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1, pWave1, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,pfStressWave,lsWaveHead1);
		loDynCharValueWriteBack1 = dynCharValue;
		
		int iRtn2=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann2, pWave2, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff,pfInertiaWave,lsWaveHead2);
		loDynCharValueWriteBack2 = dynCharValue;
		
		int iRtn3=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sStressChann, pRodLoadWave, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff, pfRodLoadWave, lsWaveHead3);
		if (iRtn1>0 && iRtn2>0 && iRtn3)
		{
			//得到活塞杆负荷波形
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo1._cChannNo,timeOff,iStartIndex,iEndIndex);
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
			//计算获得惯性力波形
			if (fAngle1-fAngle2>0)
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			else
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			//strInfo.Format("%d,%d",iPeriodSmpNums,iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			//计算获得的惯性力波形数据由N转化为KN
			for (int nloop=0;nloop<iSmpNum;nloop++)
				pfInertiaWave[nloop]=pfInertiaWave[nloop]/1000.0;
			//计算获得气体压力波形
			for (int nloop=0;nloop<iSmpNum;nloop++)
				pfStressWave[nloop]=pfRodLoadWave[nloop]-pfInertiaWave[nloop];
			////获得振动测点的波形
			S_VibCharValue vibCharValue;
			CWaveHead lsWaveHead;
			int iRtnVib  = ReadVibChannSpecTimeWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sChannVib, pVibWave, iSmpNum,vibCharValue, iSmpFreq, fRev, timeOff, pfVibWave, lsWaveHead);
			if (iRtnVib<0)
			{
				CHZLogManage::Error("没有读到振动波形数据","Online","CTransactData::OnGetDynStressWave");
			}
			//pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pVibWave, iSmpNum * sizeof(DATATYPE_WAVE));

			if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfVibWave,iSmpNum*sizeof(float));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
			}

			pFixComm->SetItemBuf(FIELD_SYSTEM_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));
			

			pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());
			

			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&loDynCharValueWriteBack1,sizeof(S_DynCharValue));
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&loDynCharValueWriteBack2,sizeof(S_DynCharValue));
			
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam4);


			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM5,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM6,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM7,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM8,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam4);

			pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);
			pFixComm->Write(pFixComm->GetAci());
			index=1;
		}
    }
	else
	{
		//如果没有动态应力测点、则根据两个动态压力测点计算获得活塞杆应力波形数据
		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0){
			::SetNullReturnFixcomm((CFixComm*)pFixComm->GetFixComm());
			return -3;
		}

		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;
		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;
		//strInfo.Format("%f,%f",fAngle1,fAngle2);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1,iRev=0;
		int iStartIndex = 0, iEndIndex = 0;
		S_DynCharValue dynCharValue;
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;

		float fRev =.1;
		iSmpNum=pPlantInfo->GetPlantInfo()._iSmpNum;
		//strInfo.Format("%d,%d",iSmpNum,iSmpFreq);
		//CHZLogManage::Debug(strInfo, "CTransactData","GetSpecTimeDynStressWave");
		pWave1=new DATATYPE_WAVE[lnWavePointMax];
		pWave2= new DATATYPE_WAVE[lnWavePointMax];
		pfStressWave=new float[lnWavePointMax];
		pfInertiaWave=new float[lnWavePointMax];
		pfRodLoadWave=new float[lnWavePointMax];
		pVibWave= new DATATYPE_WAVE[lnWavePointMax];

		float * pfVibWave= NULL;
		vector<float> lvVibWave(lnWavePointMax,0);
		pfVibWave = &lvVibWave.front();
		
		int iRtn1=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann1, pWave1, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff, pfStressWave, lsWaveHead1);
		loDynCharValueWriteBack1 = dynCharValue;

		int iRtn2=ReadDynChannSpecTimeWaveAndCharValue(pFixComm,pDB,sCompany,sFactory,sPlant,sChann2, pWave2, iSmpNum, dynCharValue, iSmpFreq, fRev, timeOff, pfInertiaWave,lsWaveHead2);
		loDynCharValueWriteBack2 = dynCharValue;

		if (iRtn1>0 && iRtn2>0)
		{
			pPlantInfo->GetSpecDynChannStartEndIndex(pDB,channInfo1._cChannNo,timeOff,iStartIndex,iEndIndex);
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
			//计算气体压力和惯性力波形
			if (fAngle1-fAngle2>0)
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			else
				iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
			//strInfo.Format("%d,%d",iPeriodSmpNums,iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			//计算获得的气体压力和惯性力波形数据由N转化为KN
			for (int nloop=0;nloop<iSmpNum;nloop++)
			{
				pfInertiaWave[nloop]=pfInertiaWave[nloop]/1000.0;
				pfStressWave[nloop]=pfStressWave[nloop]/1000.0;
			}
			//计算活塞杆负荷波形
			for (int nloop=0;nloop<iSmpNum;nloop++)
				pfRodLoadWave[nloop]=pfStressWave[nloop]+pfInertiaWave[nloop];
			////获得振动测点的波形
			S_VibCharValue vibCharValue;
			CWaveHead lsWaveHead;
			int iRtnVib  = ReadVibChannSpecTimeWaveAndCharValue( pFixComm, pDB, sCompany, sFactory, sPlant, sChannVib, pVibWave, iSmpNum,vibCharValue, iSmpFreq, fRev, timeOff,pfVibWave,lsWaveHead);
			if (iRtnVib<0)
			{
				CHZLogManage::Error("没有读到振动波形数据","Online","CTransactData::OnGetDynStressWave");
			}
			//strInfo.Format("iSmpNum:%d",iSmpNum);
			//CHZLogManage::Debug(strInfo, "CTransactData","OnGetDynStressWave");
			//pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
			pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pVibWave, iSmpNum * sizeof(DATATYPE_WAVE));
			if(lsWaveHead.GetLength() > 1 && (int)lsWaveHead.m_pParam[1] == 1)
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,(int)lsWaveHead.m_pParam[1]);
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfVibWave,iSmpNum*sizeof(float));
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,0);
			}

			pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());


			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&loDynCharValueWriteBack1,sizeof(S_DynCharValue));
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&loDynCharValueWriteBack2,sizeof(S_DynCharValue));

			pFixComm->SetItemBuf(FIELD_SYSTEM_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));

			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam4);


			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM5,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM6,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM7,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM8,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam4);

			CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();				
			timeCurr=timeOff;
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);


			pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);
			pFixComm->Write(pFixComm->GetAci());
			index=1;
		}
	}
	KD_DeletePoint(pWave1);
	KD_DeletePoint(pWave2);
	KD_DeletePoint(pRodLoadWave);
	KD_DeletePoint(pfStressWave);
	KD_DeletePoint(pfInertiaWave);
	KD_DeletePoint(pfRodLoadWave);
	KD_DeletePoint(pVibWave);

	if(index < 0){
		::SetNullReturnFixcomm((CFixComm*)pFixComm->GetFixComm());
		return -4;
	}

	return index;
}

/**得到指定公司、分厂、机组、时间段、任意多个测点的自定义参数，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUFF
*          自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数               FIELD_SERVICE_CHANN_NUM
*          时间值                 FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND         21073
*/
int CTransactData::GetSpecChannsCustomTrend(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany= pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory= pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant  = pFixComm->Get(FIELD_SERVICE_PLANTID);
	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	if (iChannType<=0)
	{//如果通道类型是0，则说明是过程量测点，该接口只处理振动测点和动态测点，则加1后为振动测点
		iChannType = GE_VIBCHANN;
	}
	int nCount=0;
	if (GetChannOneType((CHANNTYPE)iChannType)==E_TBL_CHANNTYPE_VIB)
		nCount=ReadVibCustomParamTrend(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timeStart,timeEnd);
	else
		nCount=ReadDynCustomParamTrend(pFixComm,pDB,sCompany,sFactory,sPlant,pChannBuff,iChannNum,timeStart,timeEnd);
	delete [] pChannBuff;
	return nCount;
}

/** 获取离线巡检数据：
*          公司ID        FIELD_SYSTEM_COMPANY
*          分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          测点ID       FIELD_SERVICE_OFF_CHANN1
*          时间         FIELD_SERVICE_OFF_TIME
*          数据值       FIELD_SERVICE_OFF_DATA
*          转速         FIELD_SERVICE_OFF_REV
#define     SERVICE_SERVICE_OFF_GET_OFFLINE_DATA         21074
*/
int CTransactData::GetOfflineData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->Get(FIELD_SERVICE_PLANTID);

	// 验证设备
	CString strTable;
	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany, sFactory, sPlant, sSuffix, pFixComm);
	if(pPlantInfo==NULL)
		return -1;

	int iChannType = pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannNum  = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);

	CBHDateTime timeStart = CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd   = CBHDateTime::GetCurrentTime();
	timeStart = pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd   = pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	int iChannBuffSize = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char * pChannBuff  = new char[iChannBuffSize];
	ZeroMemory(pChannBuff,iChannBuffSize);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF, pChannBuff, iChannBuffSize);


	S_ChannInfo channInfo;
	map<string, S_ChannInfo> channNameInfoMap;
	map<string, S_ChannInfo> onlineChannNameInfoMap;
	map<string, S_ChannInfo> offlineChannNameInfoMap;

	list<string> lChann = ParseChannList(pChannBuff,iChannNum);
	for (list<string>::iterator it=lChann.begin();it!=lChann.end();++it){

		channInfo = pPlantInfo->GetSpecProcChannInfo(*it);
		channNameInfoMap[*it] = channInfo;

		if(GE_DATA_SOURCE_OFFLINE == channInfo._iDataSource)
			offlineChannNameInfoMap[*it] = channInfo;
		else
			onlineChannNameInfoMap[*it]  = channInfo;
	}


	if(onlineChannNameInfoMap.size() > 0 && offlineChannNameInfoMap.size() > 0){
		CHZLogManage::Info("无法同时获取离线和在线","S_OfflineSev", __FUNCTION__);
	}

	if (offlineChannNameInfoMap.size() <= 0){
		CHZLogManage::Warn("无离线测点","S_OfflineSev", __FUNCTION__);
		KD_DeletePoint(pChannBuff);
		return 0;
	}

	// 切换至数据库
	SwitchDbNameEx(pDB,sSuffix.c_str());


	CString DBDataTblName;
	DBDataTblName.Format("%s_%s", gc_cOfflineTable_Prefix, sSuffix.c_str());

	string sChannNO;
	vector<CBHDateTime> timeArray;
	vector<float>        dataArray;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	CString retrieveField;
	retrieveField.Format("[%s], [%s], [%s], [%s]",
		gc_cChannNo, gc_cDateTime, gc_cAll, gc_cRev);

	for(map<string, S_ChannInfo>::iterator it = offlineChannNameInfoMap.begin();
	    it != offlineChannNameInfoMap.end();
		++it){
		channInfo = it->second;

		pDB->Cmd("SELECT %s FROM %s", retrieveField, DBDataTblName);
		pDB->Cmd(" WHERE [%s]='%s'", gc_cChannNo, channInfo._cChannNo);
		pDB->Cmd(" AND [%s] > '%s' AND [%s] < '%s'",
			gc_cTime, CPlantInfoBase::TimeConvertString(timeStart),
			gc_cTime, CPlantInfoBase::TimeConvertString(timeEnd));
		pDB->Cmd(" ORDER BY [%s]", gc_cTime);

		while(pDB->More()) {
			pFixComm->SetItem(FIELD_SERVICE_OFF_CHANN1, channInfo._cChannID);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME, pDB->GetTime(gc_cDateTime));
			pFixComm->SetItem(FIELD_SERVICE_OFF_DATA, pDB->GetDouble(gc_cAll));
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV, pDB->GetDouble(gc_cRev));

			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}

	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	delete [] pChannBuff;

	return iChannNum;
}





int CTransactData::ReadVibChannSpecTimeWaveAndCharValue(ICommInterFace *pFixComm_,
														IDBInterFace *pDB_,
														const string &sCompany_, 
														const string &sFactory_, 
														const string &sPlant_, 
														const string &sChann_, 
														DATATYPE_WAVE *pWave_, 
														int &iWaveNum_, 
														S_VibCharValue &vibCharValue_, 
														int &iSmpFreq_, 
														float &fRev_, 
														CBHDateTime &timeSpec_,
														float *pfWave_,
														CWaveHead& sWaveHead_,
														bool bExactTime_)
{
	E_TREND_TBL_TYPE iTableType = GE_TREND_TBLTYPE_SECOND;

	//要提取波形数据的表单名字
	CString strTable;
	string  sSuffix; //后缀
	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;

	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecVibChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	//为了可以准确的读取出波形数据，所以读取某个时间段（左右2分钟）就可以，而不是局限某个时间
	CString strSqlWhere;
	CString strShowField;
	if (bExactTime_==true)
	{   //根据精确的时间取特征值和波形
		CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
		sPrefix = GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_, timeSpec_, iTableType, sSuffix);
		strTable.Format( "%s%s", sPrefix.c_str(), sSuffix.c_str() );
		strShowField.Format(" %s,%s",
			ConsVibChannField(channInfo._cChannNo,true),
			ConsCommonField());
		//构造查询Sql语句
		pDB_->Cmd( "SELECT %s FROM [%s]", strShowField, strTable );
		pDB_->Cmd( " WHERE [%s]='%s'", gc_cTime, strTime );
		iTableType = GE_TREND_TBLTYPE_MONTH;
		while(!(pDB_->More()))
		{
			if (pDB_->GetLastError() < 0)
				return -1;

			if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
				return -1;
			strTable.Format("%s%s ", GetTblNamePrefix( GE_VIBCHANN, iTableType), sSuffix.c_str() );
			iTableType = (E_TREND_TBL_TYPE)(iTableType-1);
			pDB_->Cmd( "SELECT %s FROM [%s]", strShowField, strTable );
			pDB_->Cmd( " WHERE [%s]='%s'", gc_cTime, strTime);
		}
	}
	else
	{
		int lnLoopLimitCount = 4;

		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();

		for (int lnLoopCount =0;lnLoopCount<lnLoopLimitCount;lnLoopCount++)
		{
			COleDateTimeSpan timeSpan;

			switch (lnLoopCount)
			{
			case 0:
				timeSpan = COleDateTimeSpan(0,0,0,5);
				break;
			case 1:
				timeSpan = COleDateTimeSpan(0,0,0,10);
				break;
			case 2:
				timeSpan = COleDateTimeSpan(0,0,0,20);
				break;
			case 3:
				timeSpan = COleDateTimeSpan(0,0,2,0);
				break;
			default:
				timeSpan = COleDateTimeSpan(0,0,0,5);
			}

			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			sPrefix = GetTblNamePrefixForWave2Ex( GE_VIBCHANN,pDB_, timeSpec_, iTableType, sSuffix);
			strTable.Format( "%s%s", sPrefix.c_str(), sSuffix.c_str() );
			strShowField.Format("%s,%s",
				ConsVibChannField(channInfo._cChannNo,true),
				ConsCommonField());
			//构造查询Sql语句
			pDB_->Cmd( "SELECT %s FROM [%s]", strShowField, strTable );
			pDB_->Cmd( " WHERE  %s ", strSqlWhere );
			iTableType = GE_TREND_TBLTYPE_MONTH;

			BOOL lbDataFound = TRUE;

			while(!(pDB_->More()))
			{
				lbDataFound = FALSE;

				if (pDB_->GetLastError() < 0)
					return -1;

				if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
				{
					break;
				}					
				strTable.Format("%s%s ", GetTblNamePrefix( GE_VIBCHANN, iTableType), sSuffix.c_str() );
				iTableType = (E_TREND_TBL_TYPE)(iTableType-1);
				pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
				pDB_->Cmd(" WHERE %s ", strSqlWhere );
			}

			if (lbDataFound)
			{
				break;
			}

		}


	}

	GetSpecWaveByTimeRange(pDB_,strTable,iTableType,timeSpec_,strShowField,SimADO_GetSampleNumEx(pDB_, channInfo),channInfo._cChannNo,channInfo._iDataSource);

	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;

	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);
	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();

	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。
		vibCharValue_ = ReadVibCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev_);
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);
		pFixComm_->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead_.m_pParam[0]);
		inum=isize/sizeof(DATATYPE_WAVE);
		if( inum < iWaveNum_)
		{
			iWaveNum_ = inum;
		}

		//memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));
		pPlant->ConvertDTWaveToFloat(pfWave_,pDtWave,iWaveNum_,sWaveHead_);

		pPlant->ConvertFloatToDTWave(pWave_,pfWave_,iWaveNum_,sWaveHead_,1);
		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}


int CTransactData::ReadDynChannSpecTimeWaveAndCharValue(ICommInterFace *pFixComm_, IDBInterFace *pDB_, const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_,  DATATYPE_WAVE *pWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, CBHDateTime &timeSpec_,float *pfWave_,CWaveHead& sWaveHead_,bool IsExactTime_)
{
	E_TREND_TBL_TYPE iTableType = GE_TREND_TBLTYPE_SECOND;

	//要提取波形数据的表单名字
	CString strTable,strShowField;
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀

	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;
	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecDynChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	WriteChannInfoToFixComm(pFixComm_,channInfo);
	strShowField.Format(" %s,CUSTOM_PARAM_%s,%s",
		ConsDynChannField(channInfo._cChannNo,true),
		channInfo._cChannNo,
		ConsCommonField());
	
	if (IsExactTime_ ==true)
	{
		sPrefix = GetTblNamePrefixForWave2Ex( GE_DYNPRESSCHANN,pDB_,timeSpec_,iTableType,sSuffix);
		strTable.Format( "%s%s", sPrefix.c_str(), sSuffix.c_str() );
		//构造查询Sql语句
		pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
		pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		
		CBHDateTime loDateTime(timeSpec_); 
		
		iTableType = GE_TREND_TBLTYPE_MONTH;
		
		BOOL lbDataFound = TRUE;

		int lnLoopLimit = 5;
		
		while(!(pDB_->More()))
		{
			lbDataFound = FALSE;

			for (int lnLoop = 0;lnLoop < lnLoopLimit;lnLoop++)
			{
				COleDateTimeSpan loSpan;

				loSpan = COleDateTimeSpan( 0, 0, 0, TRENDRECORDS_QUERYTIMESPAN*pow(2.0,lnLoop));

				CBHDateTime loDateBegin = loDateTime - loSpan;

				CBHDateTime loDateEnd = loDateTime + loSpan;

				CString lstrTimeStart = CPlantInfoBase::TimeConvertString(loDateBegin);

				CString lstrTimeEnd = CPlantInfoBase::TimeConvertString(loDateEnd);

				pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);

				pDB_->Cmd(" WHERE [%s] between '%s' and '%s'",gc_cTime,lstrTimeStart,lstrTimeEnd);

				if (pDB_->More())
				{
					lbDataFound = TRUE;

					break;
				}
			}

			if (lbDataFound)
			{
				break;
			}

			if (pDB_->GetLastError() < 0)
			{	
				return -1;
			}

			//如果常规数据表中没有查到，则从报警表中获取
			if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
			{	
				lbDataFound = FALSE;

				break;
			}
			
			strTable.Format("%s%s ", GetTblNamePrefix( GE_DYNPRESSCHANN, iTableType), sSuffix.c_str() );
			
			iTableType = (E_TREND_TBL_TYPE)(iTableType-1);
			
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			
			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);
		}

		if (!lbDataFound)
		{
			strTable.Format("%s%s ", GetTblNamePrefixAlarmSud( GE_DYNPRESSCHANN, SEARCH_ALARM), sSuffix.c_str() );

			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);

			pDB_->Cmd(" WHERE [%s]='%s'",gc_cTime,strTime);

			if (pDB_->More())
			{
				if (pDB_->GetLastError() < 0)
				{	
					return -1;
				}
			}
		}
	}
    else
	{
		//为了多参数可以读取出波形数据，所以读取某个时间段（左右2秒）就可以，而不是局限某个时间
		CString strSqlWhere;


		int lnLoopLimitCount = 4;

		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();

		for (int lnLoopCount =0;lnLoopCount<lnLoopLimitCount;lnLoopCount++)
		{
			COleDateTimeSpan timeSpan;

			switch (lnLoopCount)
			{
			case 0:
				timeSpan = COleDateTimeSpan(0,0,0,5);
				break;
			case 1:
				timeSpan = COleDateTimeSpan(0,0,0,10);
				break;
			case 2:
				timeSpan = COleDateTimeSpan(0,0,0,20);
				break;
			case 3:
				timeSpan = COleDateTimeSpan(0,0,2,0);
				break;
			 default:
				timeSpan = COleDateTimeSpan(0,0,0,5);
			}

			dtStart = timeSpec_ - timeSpan;
			dtEnd   = timeSpec_ + timeSpan;
			strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
			sPrefix = GetTblNamePrefixForWave2Ex( GE_DYNPRESSCHANN,pDB_, timeSpec_, iTableType, sSuffix);
			strTable.Format( "%s%s", sPrefix.c_str(), sSuffix.c_str() );
			//构造查询Sql语句
			pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
			pDB_->Cmd(" WHERE %s ", strSqlWhere );
			iTableType = GE_TREND_TBLTYPE_MONTH;

			BOOL lbDataFound = TRUE;
			while(!(pDB_->More()))
			{
				lbDataFound = FALSE;
				if (pDB_->GetLastError() < 0)
					return -1;

				if (GE_TREND_TBLTYPE_UNKNOWN==iTableType)
				{
					break;
				}
					
				strTable.Format("%s%s ", GetTblNamePrefix( GE_DYNPRESSCHANN, iTableType), sSuffix.c_str() );
				iTableType = (E_TREND_TBL_TYPE)(iTableType-1);
				pDB_->Cmd("SELECT %s FROM [%s]",strShowField,strTable);
				pDB_->Cmd(" WHERE %s ", strSqlWhere );
			}

			if (lbDataFound)
			{
				break;
			}

		}
		
	}
	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;
	GetSpecWaveByTimeRange(pDB_,strTable,iTableType,timeSpec_,strShowField,SimADO_GetSampleNumEx(pDB_, channInfo),channInfo._cChannNo,channInfo._iDataSource);
	// 采样点数和采样频率
	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);
	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();

	//CString strInfo;strInfo.Format("iSmpFreq_:%d,inum:%d,iRev:%d",iSmpFreq_,inum,iRev);BHTRACE_DEBUG(strInfo);
	if (inum>1)
	{//为了防止采样点数为0的情况下，问题异常。
		dynCharValue_=ReadDynCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		pFixComm_->SetItem(FIELD_SERVICE_OFF_REV,fRev_);
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);

		pFixComm_->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead_.m_pParam[0]);
		
		inum=isize/sizeof(DATATYPE_WAVE);
		//writefile(pDtWave,inum,201);
		if( inum < iWaveNum_)
			iWaveNum_ = inum;
		//strInfo.Format("inum:%d,iWaveNum_:%d",inum,iWaveNum_);BHTRACE_DEBUG(strInfo);
		//memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));

		pPlant->ConvertDTWaveToFloat(pfWave_,pDtWave,iWaveNum_,sWaveHead_);

		pPlant->ConvertFloatToDTWave(pWave_,pfWave_,iWaveNum_,sWaveHead_,1);
		
		
		//得到自定义参数信息
		float  *pfCustomParom =new float[GD_CUSTOMPARAM_BUFFSIZE];
		memset(pfCustomParom,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		long iAllBytes=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
		CString sTmp;sTmp.Format("CUSTOM_PARAM_%s",channInfo._cChannNo);
		CWaveHead lsWaveHead;
		SimADO_SafeGetBufferEx(pDB_, sTmp, (BYTE*)pfCustomParom, iAllBytes, lsWaveHead);
		/*CString sTemp;
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
		sTemp.Format("Dyn:pfCustomParom[%d]:%f",i,pfCustomParom[i]);
		BHTRACE_DEBUG(sTemp);
		}*/
		pFixComm_->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomParom,iAllBytes);
		KD_DeletePoint(pfCustomParom);
		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}

int CTransactData::ReadVibSpecCharValueTrend(
	ICommInterFace * pFixComm, IDBInterFace *pDB,
	string sCompany_,string sFactory_,string sPlant_,
	char *pcChannBuf_,int iChannNum_,
	int iClyCharType_, int iTraCharType_, int iOtherCharType_,
	CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{

	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendSpecType");
	list<string> lChannName;
	list<string>::iterator itChannName;
	list<string> lChannField;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	S_ChannInfo channInfo;
	//假如为true，说明所代表测点为空值
	int index=0;
	lChannName = ParseChannList(pcChannBuf_,iChannNum_);
	for (itChannName=lChannName.begin();itChannName!=lChannName.end();++itChannName)
	{
		//得到这个测点基本信息
		channInfo=pPlantInfo->GetSpecVibChannInfo(*itChannName);
		if (channInfo._cChannNo =="")
			continue;
		int iCylinderNo = pPlantInfo->GetSpecVibChannInfo(channInfo._cChannNo)._iCylinderNo;
		//CString sTemp;sTemp.Format("测点名:%s,所属汽缸号:%d",(*itChannName).c_str(),iCylinderNo);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendSpecType");
		if (iCylinderNo == 0)
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iTraCharType_));
		else if (iCylinderNo > 0 && iCylinderNo <=100)
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iClyCharType_));
		else
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iOtherCharType_));
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecVibChannInfo(channInfo._cChannNo)._iRevType));
	}
	iChannNum_=lChannField.size();
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum_];
	int           *piRev  = new int[iChannNum_];
	//需要获得的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append("[");
		sShowField.append(*itChann);
		sShowField.append("],");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	int nloop=0;
	int iInterval=0;
	int iNums=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool bRead=false;
	bool bNeedSubTbl=false;//是否需要从下一级表中读取数据
	int iTmp=0;
	//得到趋势记录
	//CBHDateTime tCurrTime = CBHDateTime::GetCurrentTime();SYSTEMTIME sysTime;GetLocalTime(&sysTime);
	//CString strTemp1;strTemp1.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();

	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
	if(iNums > 0)
	{

		iInterval=iNums/D_TRENDRECORDS;

		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);
		if(bRead)
		{
			index = 0;
			do
			{
				index++;
				nloop=0;
				itChannRevField = lChannRevField.begin();
				for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
				{
					pfValue[nloop]=pDB->GetDouble(itChann->c_str());
					piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
				pFixComm->Write( pFixComm->GetAci() );
				pFixComm->CreateNextAnswer();
			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
		//开始取数据
		if (bRead)
		{//如果需要从下一级表中读取数据，不需要间隔判断，否需要间隔取数
			if (bNeedSubTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if (iInterval>1)
					iInterval=iInterval-1;
				else
					iInterval=0;
			}
			if(iInterval > 0)
			{
				bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_VIBCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);

				if(bRead)
				{
					do
					{
						index++;
						nloop=0;
						itChannRevField = lChannRevField.begin();
						for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
						{
							pfValue[nloop]=pDB->GetDouble(itChann->c_str());
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
						pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
						pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
						pFixComm->Write( pFixComm->GetAci() );
						pFixComm->CreateNextAnswer();
					}while(pDB->More());
				}
			}
			else
			{
				iTmp=0;
				do
				{
					if(iTmp==0)
					{
						iTmp=iInterval;
						index++;
						nloop=0;
						itChannRevField = lChannRevField.begin();
						for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
						{
							pfValue[nloop]=pDB->GetDouble(itChann->c_str());
							piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
							++itChannRevField;
							++nloop;
						}
						pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
						pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
						pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
						pFixComm->Write( pFixComm->GetAci() );
						pFixComm->CreateNextAnswer();
					}
					else
						--iTmp;
				}while(pDB->More());
			}

		}
	}
	
	if (bNeedSubTbl)
	{
RECALL_READDATA:
		strTable.Format("%s%s",GetTblNamePrefix( GE_VIBCHANN ,iTblType),sSuffix.c_str());
		//CString sTemp;sTemp.Format("表名:%s",strTable);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendSpecType");
		pDB->Cmd("SELECT %s from [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if (iInterval>1)
				iInterval=iInterval-1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pfValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_VIBSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			itChannRevField = lChannRevField.begin();
			for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
			{
				pfValue[nloop]=pDB->GetDouble(itChann->c_str());
				piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
				++itChannRevField;
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}

	KD_DeletePoint(pfValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChannField.clear();
	lChannName.clear();
	//tCurrTime = CBHDateTime::GetCurrentTime();GetLocalTime(&sysTime);
	//CString strTemp11;strTemp11.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	//CHZLogManage::Debug("开始得振动趋势时间"+strTemp1+"得振动趋势完成时间"+strTemp11,"CTransactData","ReadVibSpecCharValueTrend");
	return index;
}

int CTransactData::ReadVibSpecCharValueTrendEx(
	ICommInterFace * pFixComm, IDBInterFace *pDB,
	string sCompany_,string sFactory_,string sPlant_,
	char *pcChannBuf_,int iChannNum_,
	int iClyCharType_, int iTraCharType_, int iOtherCharType_,
	CBHDateTime timeStart_,CBHDateTime timeEnd_)
{

	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadVibTrendSpecType");
	list<string> lChannName;
	list<string>::iterator itChannName;
	list<string> lChannField;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	S_ChannInfo channInfo;

	lChannName = ParseChannList(pcChannBuf_,iChannNum_);
	for (itChannName=lChannName.begin();itChannName!=lChannName.end();++itChannName)
	{
		//得到这个测点基本信息
		channInfo=pPlantInfo->GetSpecVibChannInfo(*itChannName);
		if (channInfo._cChannNo =="")
			continue;
		int iCylinderNo = pPlantInfo->GetSpecVibChannInfo(channInfo._cChannNo)._iCylinderNo;
		if (iCylinderNo == 0)
		{
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iTraCharType_));
		}
		else if (iCylinderNo > 0 && iCylinderNo <=100)
		{		
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iClyCharType_));
		}
		else
		{
			lChannField.push_back(ConsVibChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iOtherCharType_));
		}
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecVibChannInfo(channInfo._cChannNo)._iRevType));
	}
	iChannNum_=lChannField.size();
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum_];
	int           *piRev  = new int[iChannNum_];
	//需要获得的字段
	string sShowField="";
	for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)//构建要查询的字段
	{
		sShowField.append("[");
		sShowField.append(*itChann);
		sShowField.append("],");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append("[");
	sShowField.append(gc_cTime);
	sShowField.append("]");

	int nloop=0;
	int iNums=0;

	//首先查报警表
	bool bNeedSudTbl=false;
	CString strTblAlarm,strTblRegular,strWhere,strTimeStart,strTimeEnd,strTblFast;

	strTblAlarm.Format("%s%s",GetTblNamePrefixAlarmSud(GE_VIBCHANN,SEARCH_ALARM),sSuffix.c_str());

	//查询常规数据
	//其次查常规数据表
	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	E_TREND_TBL_TYPE lenTblType = GE_TREND_TBLTYPE_SECOND;
	bool bNeedSubTbl = false;
	bool bUseSupTbl = false;
	E_TREND_TBL_TYPE lenTblTypeFast = GE_TREND_TBLTYPE_FAST;

	//strTblRegular.Format("%s%s", GetTblNamePrefixForTrend2(GE_VIBCHANN, timeStart_,timeEnd_,lenTblType,bNeedSubTbl,bUseSupTbl), sSuffix.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);

	strWhere.Format(" WHERE [%s] BETWEEN '%s' AND '%s'",gc_cTime,strTimeStart,strTimeEnd);

	strTblFast.Format("%s%s",GetTblNamePrefix(GE_VIBCHANN,lenTblTypeFast),sSuffix.c_str());


	CString strTimeArrayRegular,strTimeArrayAlarm,strTimeArrayFast;

	vector<CBHDateTime> lvTimeRegularTmp,lvTimeAlarmTmp,lvTimeFastTmp;

	vector<CBHDateTime> lvTimeRegular,lvTimeAlarm,lvTimeFast;

	set<CBHDateTime> lsTimeTmp;

	lvTimeRegularTmp = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblRegular,E_TBL_CHANNTYPE_VIB,false);

	lvTimeAlarmTmp   = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblAlarm,E_TBL_CHANNTYPE_VIB,true);

	lvTimeFast    = this->GetTrendFastRecordTimeArray(pDB,sSuffix,GE_VIBCHANN,lenTblTypeFast,timeStart_,timeEnd_);


	for(size_t nloop = 0; nloop < lvTimeAlarmTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeAlarmTmp[nloop]).second)
		{
			lvTimeAlarm.push_back(lvTimeAlarmTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeRegularTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeRegularTmp[nloop]).second)
		{
			lvTimeRegular.push_back(lvTimeRegularTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeFastTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeFastTmp[nloop]).second)
		{
			lvTimeFast.push_back(lvTimeFastTmp[nloop]);
		}
	}

	strTimeArrayAlarm = GetTimeStringByTimeArray(lvTimeAlarm);

	strTimeArrayRegular = GetTimeStringByTimeArray(lvTimeRegular);

	strTimeArrayFast = GetTimeStringByTimeArray(lvTimeFast);

	if(strTimeArrayRegular.Trim().GetLength() > 0 || strTimeArrayAlarm.Trim().GetLength() > 0 || strTimeArrayFast.Trim().GetLength())
	{
		if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s) UNION(",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);

		}
		else if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() == 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblAlarm,strWhere);

		}
		else if(strTimeArrayRegular.Trim().GetLength() == 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblRegular,strWhere);
		}
		else
		{
			pDB->Cmd("SELECT %s FROM [%s] %s",
				sShowField.c_str(),strTblRegular,strWhere);
		}

		if(strTimeArrayFast.Trim().GetLength() > 0)
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblFast,gc_cTime,strTimeArrayFast);
		}
		else
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblFast,strWhere);
		}

		pDB->Cmd(" ORDER BY %s",gc_cTime);
	}
	else
	{
		pDB->Cmd("SELECT %s FROM [%s] %s UNION SELECT %s FROM [%s] %s ORDER BY [%s] ",
			      sShowField.c_str(),strTblAlarm,strWhere,sShowField.c_str(),strTblRegular,strWhere,gc_cTime);
	}

	int lnRealNums = 0;

	if (pDB->More())
	{
		lnRealNums=pDB->GetCount();

		if (lnRealNums < 1)
		{
			KD_DeletePoint(pfValue);
			KD_DeletePoint(piRev);
			return lnRealNums;
		}
	}
	else 
	{
		// 防止超时或其它异常，
		if(pDB->GetLastError() < 0)
		{
			lnRealNums=0;		
		}
		KD_DeletePoint(pfValue);
		KD_DeletePoint(piRev);
		return 0;
	}

	////最多返回2000条报警记录
	//int lnTotalNum = 2000;
	//int lnIntervel = 0;
	//if(lnRealNums > lnTotalNum)
	//{
	//	lnIntervel = (int)lnRealNums/lnTotalNum;
	//}

	//将联合查询结果返回
	int index = 0;
	int lnTemp = 0;
	do
	{
		index++;
		nloop=0;
		itChannRevField = lChannRevField.begin();
		for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
		{
			
			pfValue[nloop]=pDB->GetDouble(itChann->c_str());
			piRev [nloop] = pDB->GetLong( itChannRevField->c_str() );
			++itChannRevField;
			++nloop;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
		pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_VIBCHANN);
		pFixComm->Write( pFixComm->GetAci() );
		pFixComm->CreateNextAnswer();
	}while(pDB->More());
	
	KD_DeletePoint(pfValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChannField.clear();
	lChannName.clear();

	return index;
}


int CTransactData::ReadDynSpecCharValueTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
											 int iPreCharType_,int iRodCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",sSuffix.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynTrendSpecType");
	list<string> lChannField;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	list<string> lChannName;
	list<string>::iterator itChannName;
	S_ChannInfo channInfo;
	//假如为true，说明所代表测点为空值
	int index=0;
	lChannName = ParseChannList(pcChannBuf_,iChannNum_);
	for (itChannName=lChannName.begin();itChannName!=lChannName.end();++itChannName)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(*itChannName).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynSpecCharValueTrend");
		//得到这个测点基本信息
		channInfo=pPlantInfo->GetSpecDynChannInfo(*itChannName);
		if (channInfo._cChannNo=="")
			continue;
		if (pPlantInfo->GetSpecDynChannInfo(*itChannName)._iChannType == GE_RODSINKCHANN)
			lChannField.push_back(ConsDynChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iRodCharType_));
		else
			lChannField.push_back(ConsDynChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iPreCharType_));
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecDynChannInfo(channInfo._cChannNo)._iRevType));
	}
	iChannNum_=lChannField.size();
	DATATYPE_CHAR  *pValue = NULL;
	pValue= new DATATYPE_CHAR[iChannNum_];
	int *piRev     =new int[iChannNum_];
	//需要获得的字段
	string sShowField="";
	for ( itChann=lChannField.begin(); itChann!=lChannField.end(); ++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	//CString sTemp;sTemp.Format("测点名:%s",sShowField.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynSpecCharValueTrend");
	int nloop=0;
	int iInterval=0;
	int iNums=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	bool bRead=false;
	bool bNeedSubTbl=false;//是否需要从下一级表中读取数据
	int iTmp=0;
	//得到趋势记录
	//CBHDateTime tCurrTime = CBHDateTime::GetCurrentTime();SYSTEMTIME sysTime;GetLocalTime(&sysTime);
	//CString strTemp1;strTemp1.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();
	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
	if(iNums > 0)
	{

		iInterval=iNums/D_TRENDRECORDS;

		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl,iInterval);
		if(bRead)
		{
			index = 0;
			do
			{
				index++;
				nloop=0;
				itChannRevField = lChannRevField.begin();
				for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
				{
					pValue[nloop]=pDB->GetDouble(itChann->c_str());
					piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
					//当获取到的转速小于0时,则显示转速为0
					if (piRev[nloop]<0)
						piRev[nloop] = 0;
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();

			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,iTblType,timeStart_,timeEnd_,bNeedSubTbl);
		//开始取数据
		if (bRead)
		{//如果需要从下一级表中读取数据，不需要间隔判断，否需要间隔取数
			if (bNeedSubTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if (iInterval>1)
					iInterval=iInterval-1;
				else
					iInterval=0;
			}
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
						//当获取到的转速小于0时,则显示转速为0
						if (piRev[nloop]<0)
							piRev[nloop] = 0;
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
	}

	
	if (bNeedSubTbl)
	{
RECALL_READDATA:
		strTable.Format("%s%s", GetTblNamePrefix( GE_DYNPRESSCHANN, iTblType), sSuffix.c_str());
		//CString sTemp;sTemp.Format("表名:%s",strTable);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynSpecCharValueTrend");
		pDB->Cmd("SELECT %s from [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' AND '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		//CHZLogManage::Debug(pDB->GetSql(),"CTransactData","ReadDynSpecCharValueTrend");
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READDATA;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if (iInterval>1)
				iInterval=iInterval-1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
					{
						pValue[nloop]=pDB->GetDouble(itChann->c_str());
						piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
						//当获取到的转速小于0时,则显示转速为0
						if (piRev[nloop]<0)
							piRev[nloop] = 0;
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READDATA;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_DYNSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			itChannRevField = lChannRevField.begin();
			for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
			{
				pValue[nloop]=pDB->GetDouble(itChann->c_str());
				piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
				//当获取到的转速小于0时,则显示转速为0
				if (piRev[nloop]<0)
					piRev[nloop] = 0;
				++itChannRevField;
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	KD_DeletePoint(pValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChannField.clear();
	lChannName.clear();
	//tCurrTime = CBHDateTime::GetCurrentTime();GetLocalTime(&sysTime);
	//CString strTemp11;strTemp11.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	//CHZLogManage::Debug("开始得动态趋势时间"+strTemp1+"得动态趋势完成时间"+strTemp11,"CTransactData","ReadDynSpecCharValueTrend");
	return index;
}

int CTransactData::ReadDynSpecCharValueTrendEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
											 int iPreCharType_,int iRodCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_)
{
	CString strTable;
	string  sType;
	string sChann;
	string sPrefix,sSuffix;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if (pPlantInfo == NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());

	list<string> lChannField;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	list<string> lChannName;
	list<string>::iterator itChannName;
	S_ChannInfo channInfo;

	lChannName = ParseChannList(pcChannBuf_,iChannNum_);
	for (itChannName=lChannName.begin();itChannName!=lChannName.end();++itChannName)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(*itChannName).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadDynSpecCharValueTrend");
		//得到这个测点基本信息
		channInfo=pPlantInfo->GetSpecDynChannInfo(*itChannName);
		if (channInfo._cChannNo=="")
		{
			continue;
		}
		if (pPlantInfo->GetSpecDynChannInfo(*itChannName)._iChannType == GE_RODSINKCHANN)
		{		
			lChannField.push_back(ConsDynChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iRodCharType_));
		}
		else
		{
			lChannField.push_back(ConsDynChannSpecTypeField(channInfo._cChannNo,(CHARTYPE)iPreCharType_));
		}
		lChannRevField.push_back(pPlantInfo->GetRevField(pPlantInfo->GetSpecDynChannInfo(channInfo._cChannNo)._iRevType));
	}
	iChannNum_=lChannField.size();
	DATATYPE_CHAR  *pValue = NULL;
	pValue= new DATATYPE_CHAR[iChannNum_];
	int *piRev     =new int[iChannNum_];
	//需要获得的字段
	string sShowField="";
	for ( itChann=lChannField.begin(); itChann!=lChannField.end(); ++itChann)//构建要查询的字段
	{
		sShowField.append(*itChann);
		sShowField.append(",");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append("[");
	sShowField.append(gc_cTime);
	sShowField.append("]");

	//首先查报警表
	bool bNeedSudTbl=false;
	CString strTblAlarm,strTblRegular,strWhere,strTimeStart,strTimeEnd,strTblFast;
	int lnNums = 0;
	strTblAlarm.Format("%s%s",GetTblNamePrefixAlarmSud(GE_DYNPRESSCHANN,SEARCH_ALARM),sSuffix.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
		
	strWhere.Format(" WHERE [%s] BETWEEN '%s' AND '%s'",gc_cTime,strTimeStart,strTimeEnd);

	//其次查常规数据表
	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	E_TREND_TBL_TYPE lenTblType = GE_TREND_TBLTYPE_SECOND;
	bool bNeedSubTbl = false;
	bool bUseSupTbl = false;
	E_TREND_TBL_TYPE lenFastTblType = GE_TREND_TBLTYPE_FAST;

	strTblFast.Format("%s%s",GetTblNamePrefix(GE_DYNPRESSCHANN,lenFastTblType),sSuffix.c_str());

	CString strTimeArrayRegular,strTimeArrayAlarm,strTimeArrayFast;

	vector<CBHDateTime> lvTimeRegularTmp,lvTimeAlarmTmp,lvTimeFastTmp;

	vector<CBHDateTime> lvTimeRegular,lvTimeAlarm,lvTimeFast;

	set<CBHDateTime> lsTimeTmp;

	lvTimeRegularTmp = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblRegular,E_TBL_CHANNTYPE_DYN,false);

	lvTimeAlarmTmp   = this->GetSpecTimeByTimeRange(pDB,sSuffix,timeStart_,timeEnd_,strTblAlarm,E_TBL_CHANNTYPE_DYN,true);

	lvTimeFastTmp   = this->GetTrendFastRecordTimeArray(pDB,sSuffix,GE_DYNPRESSCHANN,lenFastTblType,timeStart_,timeEnd_);

	for(size_t nloop = 0; nloop < lvTimeAlarmTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeAlarmTmp[nloop]).second)
		{
			lvTimeAlarm.push_back(lvTimeAlarmTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeRegularTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeRegularTmp[nloop]).second)
		{
			lvTimeRegular.push_back(lvTimeRegularTmp[nloop]);
		}
	}

	for(size_t nloop = 0; nloop < lvTimeFastTmp.size(); ++nloop)
	{
		if(lsTimeTmp.insert(lvTimeFastTmp[nloop]).second)
		{
			lvTimeFast.push_back(lvTimeFastTmp[nloop]);
		}
	}

	strTimeArrayAlarm = GetTimeStringByTimeArray(lvTimeAlarm);

	strTimeArrayRegular = GetTimeStringByTimeArray(lvTimeRegular);

	strTimeArrayFast = GetTimeStringByTimeArray(lvTimeFast);

	if(strTimeArrayRegular.Trim().GetLength() > 0 || strTimeArrayAlarm.Trim().GetLength() > 0 || strTimeArrayFast.Trim().GetLength())
	{
		if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s) UNION(",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);

		}
		else if(strTimeArrayRegular.Trim().GetLength() > 0 && strTimeArrayAlarm.Trim().GetLength() == 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblRegular,gc_cTime,strTimeArrayRegular);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblAlarm,strWhere);

		}
		else if(strTimeArrayRegular.Trim().GetLength() == 0 && strTimeArrayAlarm.Trim().GetLength() > 0)
		{
			pDB->Cmd("SELECT %s FROM [%s] WHERE %s IN(%s)",
				sShowField.c_str(),strTblAlarm,gc_cTime,strTimeArrayAlarm);
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblRegular,strWhere);
		}
		else
		{
			pDB->Cmd("SELECT %s FROM [%s] %s",
				sShowField.c_str(),strTblRegular,strWhere);
		}

		if(strTimeArrayFast.Trim().GetLength() > 0)
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] WHERE %s IN(%s))",
				sShowField.c_str(),strTblFast,gc_cTime,strTimeArrayFast);
		}
		else
		{
			pDB->Cmd(" UNION(SELECT %s FROM [%s] %s)",
				sShowField.c_str(),strTblFast,strWhere);
		}

		pDB->Cmd(" ORDER BY %s",gc_cTime);
	}
	else
	{
		pDB->Cmd("SELECT %s FROM [%s] %s UNION SELECT %s FROM [%s] %s ORDER BY [%s]",sShowField.c_str(),strTblAlarm,strWhere,sShowField.c_str(),strTblRegular,strWhere,gc_cTime);
	}

	int lnRealNums=0;

	if (pDB->More())
	{
		lnRealNums=pDB->GetCount();
		bNeedSudTbl = false;

		if (lnRealNums < 1)
		{
			KD_DeletePoint(pValue);
			KD_DeletePoint(piRev);
			return lnRealNums;
		}
	}
	else 
	{
		// 防止超时或其它异常，
		if(pDB->GetLastError() < 0)
		{
			lnRealNums = 0;
		}

		KD_DeletePoint(pValue);
		KD_DeletePoint(piRev);
		return 0;
	}

	////最多返回2000条报警记录
	//int lnTotalNum = 2000;
	//int lnIntervel = 0;
	//if(lnRealNums > lnTotalNum)
	//{
	//	lnIntervel = (int)lnRealNums/lnTotalNum;
	//}

	//将联合查询结果返回
	int index=0;
	int nloop = 0;
	int lnTemp = 0;
	do
	{
		index++;  
		nloop=0;
		itChannRevField = lChannRevField.begin();
		for (itChann=lChannField.begin();itChann!=lChannField.end();++itChann)
		{
			pValue[nloop]=pDB->GetDouble(itChann->c_str());
			piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
			//当获取到的转速小于0时,则显示转速为0
			if (piRev[nloop]<0)
				piRev[nloop] = 0;
			++itChannRevField;
			++nloop;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char*)pValue,sizeof(DATATYPE_CHAR)*iChannNum_);
		pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_DYNPRESSCHANN);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		
	}while(pDB->More());

	KD_DeletePoint(pValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChannField.clear();
	lChannName.clear();

	return index;
}

int CTransactData::ReadProcSpecValueTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
										  CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_)
{
	CString strTable;
	string sSuffix,sPrefix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
		return -1;
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//假如为true，说明所代表测点为空值
	int index=0;
	int nloop=0;
	list<string> lChann;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum_];
	memset(pfValue,0,iChannNum_*sizeof(DATATYPE_CHAR));
	int *piRev     =new int[iChannNum_];
	memset(piRev,0,iChannNum_*sizeof(int));
	string sShowField="";
    S_ChannInfo channInfo;
	lChann=ParseChannList(pcChannBuf_,iChannNum_);
	iChannNum_=lChann.size();
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		//CString sTemp;sTemp.Format("测点名:%s",(*itChann).c_str());
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadProcTrendSpecType");
		//得到这个测点基本信息
		channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
		lChannRevField.push_back(pPlantInfo->GetRevField(channInfo._iRevType));
		sShowField.append("[");
		sShowField.append(channInfo._cChannNo);
		sShowField.append("],");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);
	//CString sTemp;sTemp.Format("测点名:%s",sShowField.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","ReadProcTrendSpecType");
	bool bRead=false;
	bool bNeedSudTbl=false;
	int iNums=0,iInterval=0,iTmp=0;
	index=0;
	E_TREND_TBL_TYPE iTblType=GE_TREND_TBLTYPE_SECOND;
	//CBHDateTime tCurrTime = CBHDateTime::GetCurrentTime();SYSTEMTIME sysTime;GetLocalTime(&sysTime);
	//CString strTemp1;strTemp1.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	int D_TRENDRECORDS = GetConfigTrendRecordsNum();
	iNums=GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
	if(iNums > 0)
	{

		iInterval=iNums/D_TRENDRECORDS;

		bRead=GetTrendRecordsEx(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl,iInterval);
		if(bRead)
		{
			index = 0;
			do
			{
				index++;
				nloop=0;
				itChannRevField = lChannRevField.begin();
				for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
				{
					channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
					pfValue[nloop]=pDB->GetDouble(channInfo._cChannNo);
					piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
					//当获取到的转速小于0时,则显示转速为0
					if (piRev[nloop]<0)
						piRev[nloop] = 0;
					++itChannRevField;
					++nloop;
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
				pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
				pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_ALLPROC);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();

			}while(pDB->More());
		}
	}
	else
	{
		bRead=GetTrendRecords(pDB,sSuffix,sShowField,GE_ALLPROC,iTblType,timeStart_,timeEnd_,bNeedSudTbl);
		if(bRead)
		{
			if (bNeedSudTbl)
				iInterval=0;
			else
			{
				iNums=pDB->GetCount();
				iInterval=iNums/D_TRENDRECORDS;
				if(iInterval>1)
					iInterval-=1;
				else
					iInterval=0;
			}
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
					{
						channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
						pfValue[nloop]=pDB->GetDouble(channInfo._cChannNo);
						piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
						//当获取到的转速小于0时,则显示转速为0
						if (piRev[nloop]<0)
							piRev[nloop] = 0;
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_ALLPROC);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
	}
	
	if(bNeedSudTbl)
	{
RECALL_READ_RECORD_PROC1:
		strTable.Format("%s%s", GetTblNamePrefix( GE_ALLPROC, iTblType), sSuffix.c_str());
		//CString sTemp;sTemp.Format("表名:%s",strTable);
		//CHZLogManage::Debug(sTemp,"CTransactData","ReadProcSpecValueTrend");
		pDB->Cmd("SELECT %s from [%s] ",sShowField.c_str(),strTable);
		pDB->Cmd(" WHERE [%s] between '%s' and '%s' ",gc_cTime, CPlantInfoBase::TimeConvertString(timeStart_), CPlantInfoBase::TimeConvertString(timeEnd_));
		pDB->Cmd(" ORDER BY [%s] ",gc_cTime);
		if (pDB->More())
		{
			iNums=pDB->GetCount();
			if ((iNums+index)<(D_TRENDRECORDS*TRENDRECORDS_COEF))
			{
				if (iTblType>GetTrendTblType(timeEnd_))
				{
					iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
					goto RECALL_READ_RECORD_PROC1;
				}
			}
			iInterval=iNums/(D_TRENDRECORDS-index);
			if(iInterval>1)
				iInterval-=1;
			else
				iInterval=0;
			iTmp=0;
			do
			{
				if(iTmp==0)
				{
					iTmp=iInterval;
					index++;
					nloop=0;
					itChannRevField = lChannRevField.begin();
					for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
					{
						channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
						pfValue[nloop]=pDB->GetDouble(channInfo._cChannNo);
						piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
						//当获取到的转速小于0时,则显示转速为0
						if (piRev[nloop]<0)
							piRev[nloop] = 0;
						++itChannRevField;
						++nloop;
					}
					pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
					pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
					pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_ALLPROC);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}
				else
					--iTmp;
			}while(pDB->More());
		}
		else
		{
			if (iTblType>GetTrendTblType(timeEnd_))
			{
				iTblType=(E_TREND_TBL_TYPE)(iTblType-1);
				goto RECALL_READ_RECORD_PROC1;
			}
		}
	}
	if (bCurr_)
	{
		strTable.Format("TREND_PROCSECOND_%s",sSuffix.c_str());
		pDB->Cmd("SELECT TOP 1 %s  FROM [%s]",sShowField.c_str(),strTable);
		pDB->Cmd(" ORDER BY [%s] DESC",gc_cTime);
		if(pDB->More())
		{
			nloop=0;
			itChannRevField = lChannRevField.begin();
			for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
			{
				channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
				pfValue[nloop]=pDB->GetDouble(channInfo._cChannNo);
				piRev[nloop] = pDB->GetLong( itChannRevField->c_str());
				//当获取到的转速小于0时,则显示转速为0
				if (piRev[nloop]<0)
					piRev[nloop] = 0;
				++itChannRevField;
				++nloop;
			}
			pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
			pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
			pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_ALLPROC);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			index++;
		}
	}
	KD_DeletePoint(pfValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChann.clear();
	//tCurrTime = CBHDateTime::GetCurrentTime();GetLocalTime(&sysTime);
	//CString strTemp11;strTemp11.Format("%s:%d", CPlantInfoBase::TimeConvertString(tCurrTime),sysTime.wMilliseconds);
	//CHZLogManage::Debug("开始得过程量趋势时间"+strTemp1+"得过程量趋势完成时间"+strTemp11,"CTransactData","ReadProcSpecValueTrend");
	return index;
}

int CTransactData::ReadProcSpecValueTrendEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
										  CBHDateTime timeStart_,CBHDateTime timeEnd_)
{
	CString strTable;
	string sSuffix,sPrefix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
	{
		return -1;
	}
	SwitchDbNameEx(pDB,sSuffix.c_str());
	//假如为true，说明所代表测点为空值
	int index=0;
	int nloop=0;
	list<string> lChann;
	list<string>::iterator itChann;
	list<string> lChannRevField;
	list<string>::iterator itChannRevField;
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[iChannNum_];
	memset(pfValue,0,iChannNum_*sizeof(DATATYPE_CHAR));
	int *piRev     =new int[iChannNum_];
	memset(piRev,0,iChannNum_*sizeof(int));
	string sShowField="";
	S_ChannInfo channInfo;
	lChann=ParseChannList(pcChannBuf_,iChannNum_);
	iChannNum_=lChann.size();
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
		lChannRevField.push_back(pPlantInfo->GetRevField(channInfo._iRevType));
		sShowField.append("[");
		sShowField.append(channInfo._cChannNo);
		sShowField.append("],");
	}
	sShowField.append(gc_cRev);
	sShowField.append(",");
	sShowField.append(gc_cRev2);
	sShowField.append(",");
	sShowField.append(gc_cRev3);
	sShowField.append(",");
	sShowField.append(gc_cRev4);
	sShowField.append(",");
	sShowField.append(gc_cRev5);
	sShowField.append(",");
	sShowField.append(gc_cTime);


	//首先查报警表
	bool bNeedSudTbl=false;
	CString strTblAlarm,strTblRegular,strWhere,strTimeStart,strTimeEnd;
	int lnNums = 0;
	strTblAlarm.Format("%s%s",GetTblNamePrefixAlarmSud(GE_ALLPROC,SEARCH_ALARM),sSuffix.c_str());

	strTimeStart= CPlantInfoBase::TimeConvertString(timeStart_);
	strTimeEnd  = CPlantInfoBase::TimeConvertString(timeEnd_);
	strWhere.Format("  [%s] between '%s' AND '%s' ",gc_cTime,strTimeStart,strTimeEnd);

	//其次查常规数据表
	//实际起始时间的表类型,即最多(记录最密)可以从那个表读取数据
	E_TREND_TBL_TYPE lnE_TREND_TBL_TYPE = GetTrendTblType(timeStart_);

	if(GE_TREND_TBLTYPE_UNKNOWN >=lnE_TREND_TBL_TYPE) 
	{
		lnE_TREND_TBL_TYPE = GE_TREND_TBLTYPE_SECOND;

	}else if(lnE_TREND_TBL_TYPE>GE_TREND_TBLTYPE_MONTH)
	{
		lnE_TREND_TBL_TYPE = GE_TREND_TBLTYPE_MONTH;
	}

	while((lnE_TREND_TBL_TYPE<=GE_TREND_TBLTYPE_MONTH))
	{
		strTblRegular.Format("%s%s",GetTblNamePrefix(GE_ALLPROC,lnE_TREND_TBL_TYPE),sSuffix.c_str());
		pDB->Cmd("SELECT count(*) FROM [%s]",strTblRegular);
		pDB->Cmd(" WHERE %s ",strWhere);

		if (pDB->More())
		{
			lnNums=pDB->GetCount();

			if (lnNums > 0)
			{
				break;
			}
		}
		else 
		{
			lnNums=0;
			// 防止超时或其它异常，
			if(pDB->GetLastError() < 0)
			{
				KD_DeletePoint(pfValue);
				KD_DeletePoint(piRev);
				return 0;
			}
		}

		lnE_TREND_TBL_TYPE = (E_TREND_TBL_TYPE)(lnE_TREND_TBL_TYPE+1);
	}

	while((lnE_TREND_TBL_TYPE >=GE_TREND_TBLTYPE_SECOND))
	{
		strTblRegular.Format("%s%s",GetTblNamePrefix(GE_ALLPROC,lnE_TREND_TBL_TYPE),sSuffix.c_str());
		pDB->Cmd("SELECT count(*) FROM [%s]",strTblRegular);
		pDB->Cmd(" WHERE %s ",strWhere);

		if (pDB->More())
		{
			lnNums=pDB->GetCount();

			if (lnNums > 0)
			{
				break;
			}
		}
		else 
		{
			lnNums=0;
			// 防止超时或其它异常，
			if(pDB->GetLastError() < 0)
			{
				KD_DeletePoint(pfValue);
				KD_DeletePoint(piRev);
				return 0;
			}
		}

		lnE_TREND_TBL_TYPE = (E_TREND_TBL_TYPE)(lnE_TREND_TBL_TYPE-1);
	}

	strWhere.Format("  [%s] between '%s' AND '%s' ORDER BY [%s] ",gc_cTime,strTimeStart,strTimeEnd,gc_cTime);

	pDB->Cmd("SELECT %s FROM [%s] union SELECT %s FROM [%s]",sShowField.c_str(),strTblAlarm,sShowField.c_str(),strTblRegular);
	pDB->Cmd(" WHERE %s ",strWhere);

	if (pDB->More())
	{
		lnNums=pDB->GetCount();
		bNeedSudTbl = false;
	}
	else 
	{
		lnNums=0;
		// 防止超时或其它异常，
		if(pDB->GetLastError() < 0)
		{
			bNeedSudTbl = false;
		}
	}

	//将联合查询结果返回
	do
	{
		index++;
		nloop=0;
		itChannRevField = lChannRevField.begin();
		for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		{
			channInfo=pPlantInfo->GetSpecProcChannInfo(*itChann);
			pfValue[nloop]=pDB->GetDouble(channInfo._cChannNo);
			piRev[nloop] = pDB->GetLong( itChannRevField->c_str() );
			//当获取到的转速小于0时,则显示转速为0
			if (piRev[nloop]<0)
				piRev[nloop] = 0;
			++itChannRevField;
			++nloop;
		}
		pFixComm->SetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*iChannNum_);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,pDB->GetTime(gc_cTime));
		pFixComm->SetItemBuf(FIELD_SERVICE_REV_BUFF,(char*)piRev,sizeof(int)*iChannNum_);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,GE_ALLPROC);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}while(pDB->More());

	KD_DeletePoint(pfValue);
	KD_DeletePoint(piRev);
	lChannRevField.clear();
	lChann.clear();

	return index;
}



int CTransactData::ReadVibChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, DATATYPE_CHAR* pfWave_, int &iWaveNum_, S_VibCharValue &vibCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_)
{
	//要提取波形数据的表单名字
	CString strTable,strShowField;
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀

	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;
	
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;
	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecVibChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	strShowField.Format(" %s,%s",
		ConsVibChannField(channInfo._cChannNo,true),
		ConsCommonField());
	pDB_->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",strShowField,sSuffix.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID_,gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));
	if(!(pDB_->More()))
	{
		//当按照精确时间查询失败时，则按照相邻的一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",strShowField,sSuffix.c_str());
		pDB_->Cmd(" WHERE %s AND [%s]=%d", strSqlWhere,gc_cEventID,iEventID_);
		if(!(pDB_->More()))
			return -2;
	}

	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();
	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;

	// 采样频率和采样点数
	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);
	
	//CString strInfo;strInfo.Format("iSmpFreq_:%d,inum:%d,iRev:%d",iSmpFreq_,inum,iRev);BHTRACE_DEBUG(strInfo);
	if (inum>1)
	{   //为了防止采样点数为0的情况下，问题异常。
		vibCharValue_=ReadVibCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);

		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);

		inum=isize/sizeof(DATATYPE_WAVE);
		//writefile(pDtWave,inum,201);
		if( inum < iWaveNum_)
			iWaveNum_ = inum;
		//strInfo.Format("inum:%d,iWaveNum_:%d",inum,iWaveNum_);BHTRACE_DEBUG(strInfo);
		//memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));

		pPlant->ConvertDTWaveToFloat(pfWave_,pDtWave,iWaveNum_,sWaveHead_);
		pPlant->ConvertFloatToDTWave(pWave_,pfWave_,iWaveNum_,sWaveHead_);
		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}

int CTransactData::ReadDynChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_,DATATYPE_CHAR* pfWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_)
{
	//要提取波形数据的表单名字
	CString strTable,strShowField;
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;
	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecDynChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	strShowField.Format(" %s,%s",
		ConsDynChannField(channInfo._cChannNo,true),
		ConsCommonField());
	//获取报警数据
	pDB_->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID_,gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));
	if(!(pDB_->More()))
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
		pDB_->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
		pDB_->Cmd(" WHERE %s AND [%s]=%d", strSqlWhere,gc_cEventID,iEventID_);
		if(!(pDB_->More()))
			return -2;
	}
	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;
	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();
	// 采样频率和采样点数
	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);

	//CString strInfo;strInfo.Format("iSmpFreq_:%d,inum:%d,iRev:%d",iSmpFreq_,inum,iRev);BHTRACE_DEBUG(strInfo);
	if (inum>1)
	{   //为了防止采样点数为0的情况下，问题异常。
		dynCharValue_=ReadDynCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);
		inum=isize/sizeof(DATATYPE_WAVE);
		//writefile(pDtWave,inum,201);
		if( inum < iWaveNum_)
			iWaveNum_ = inum;
		//strInfo.Format("inum:%d,iWaveNum_:%d",inum,iWaveNum_);BHTRACE_DEBUG(strInfo);
	//	memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));

		pPlant->ConvertDTWaveToFloat(pfWave_,pDtWave,iWaveNum_,sWaveHead_);
		pPlant->ConvertFloatToDTWave(pWave_,pfWave_,iWaveNum_,sWaveHead_);
		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}

int CTransactData::ReadVibChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, int &iWaveNum_, S_VibCharValue &vibCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_)
{
	//要提取波形数据的表单名字
	CString strTable,strShowField;
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀

	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;

	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;
	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecVibChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	strShowField.Format(" %s,%s",
		ConsVibChannField(channInfo._cChannNo,true),
		ConsCommonField());
	pDB_->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",strShowField,sSuffix.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID_,gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));
	if(!(pDB_->More()))
	{
		//当按照精确时间查询失败时，则按照相邻的一段时间查询数据
		CString strSqlWhere;
		COleDateTimeSpan timeSpan=COleDateTimeSpan( 0, 0, 0, ALARMRECORDS_QUERYTIMESPAN );
		CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
		CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
		dtStart = timeSpec_ - timeSpan;
		dtEnd   = timeSpec_ + timeSpan;
		strSqlWhere.Format(" [%s] > '%s' AND [%s] < '%s' ", gc_cTime, CPlantInfoBase::TimeConvertString(dtStart), gc_cTime, CPlantInfoBase::TimeConvertString(dtEnd) );
		//获取报警数据
		pDB_->Cmd("SELECT %s FROM [ALARM_VIB_%s] ",strShowField,sSuffix.c_str());
		pDB_->Cmd(" WHERE %s AND [%s]=%d", strSqlWhere,gc_cEventID,iEventID_);
		if(!(pDB_->More()))
			return -2;
	}

	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();
	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;

	// 采样频率和采样点数
	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);

	//CString strInfo;strInfo.Format("iSmpFreq_:%d,inum:%d,iRev:%d",iSmpFreq_,inum,iRev);BHTRACE_DEBUG(strInfo);
	if (inum>1)
	{   //为了防止采样点数为0的情况下，问题异常。
		vibCharValue_=ReadVibCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);

		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);

		inum=isize/sizeof(DATATYPE_WAVE);
		//writefile(pDtWave,inum,201);
		if( inum < iWaveNum_)
			iWaveNum_ = inum;
		//strInfo.Format("inum:%d,iWaveNum_:%d",inum,iWaveNum_);BHTRACE_DEBUG(strInfo);
		memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));

		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}

int CTransactData::ReadDynChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_)
{
	//要提取波形数据的表单名字
	CString strTable,strShowField;
	CString strTime =  CPlantInfoBase::TimeConvertString(timeSpec_);
	string sSuffix;//后缀
	CPlantInfoOffLine *pPlant = CheckCompanyInfo( sCompany_, sFactory_, sPlant_, sSuffix, pFixComm_,false);
	if ( pPlant == NULL )
		return -1;
	SwitchDbNameEx(pDB_,sSuffix.c_str());
	if ( iWaveNum_<100)
		iWaveNum_ = 513;
	string sPrefix;
	//得到这个测点报警限信息等基本信息
	S_ChannInfo channInfo = pPlant->GetSpecDynChannInfo( sChann_ );
	if (strcmp(channInfo._cChannID, "") == 0)
		return -2;
	strShowField.Format(" %s,%s",
		ConsDynChannField(channInfo._cChannNo,true),
		ConsCommonField());
	//获取报警数据
	pDB_->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s'",gc_cEventID,iEventID_,gc_cTime, CPlantInfoBase::TimeConvertString(timeSpec_));
	if(!(pDB_->More()))
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
		pDB_->Cmd("SELECT %s FROM [ALARM_DYN_%s] ",strShowField,sSuffix.c_str());
		pDB_->Cmd(" WHERE %s AND [%s]=%d", strSqlWhere,gc_cEventID,iEventID_);
		if(!(pDB_->More()))
			return -2;
	}
	//得到波形数据
	long isize = 0;
	int inum=0,iRev=0;
	int lnWavePtMax = pPlant->GetMiddleWareWavePointNumMax();
	// 采样频率和采样点数
	iSmpFreq_ = SimADO_GetSampleFrequencyEx(pDB_, channInfo);
	inum      = SimADO_GetSampleNumEx(pDB_, channInfo);
	iRev      = SimADO_GetRevEx(pDB_, channInfo);

	//CString strInfo;strInfo.Format("iSmpFreq_:%d,inum:%d,iRev:%d",iSmpFreq_,inum,iRev);BHTRACE_DEBUG(strInfo);
	if (inum>1)
	{   //为了防止采样点数为0的情况下，问题异常。
		dynCharValue_=ReadDynCharValue(channInfo._cChannNo,pDB_);
		DATATYPE_WAVE * pDtWave=new DATATYPE_WAVE[lnWavePtMax];
		isize=inum * sizeof(DATATYPE_WAVE);
		fRev_=(float)iRev;
		CString strWave;
		strWave.Format("WAVE_%s",channInfo._cChannNo);
		SimADO_SafeGetBufferEx(pDB_, strWave,  (BYTE *)pDtWave,  isize, sWaveHead_);
		inum=isize/sizeof(DATATYPE_WAVE);
		//writefile(pDtWave,inum,201);
		if( inum < iWaveNum_)
			iWaveNum_ = inum;
		//strInfo.Format("inum:%d,iWaveNum_:%d",inum,iWaveNum_);BHTRACE_DEBUG(strInfo);
		memcpy( pWave_, pDtWave, iWaveNum_*sizeof(DATATYPE_WAVE));

		KD_DeletePoint(pDtWave);
	}
	else
		iWaveNum_ = 0;
	return inum;
}

///解释报警状态，如果返回0，说明没有报警，1表示常规低报，2表示常规高报，3表示快变报警，4表示换变报警，5表示趋势报警
 //6表示自定义低报，7自定义高报。
///数字越小，越优先,即如果存在常规报警情况下，就不会向下判断，是否有快变、缓变等。
int CTransactData::ParseAlarmStatus(unsigned int iState_)
{
	return ::AlarmStatus_ParseAlarmStatus(iState_);
}

S_VibCharValue CTransactData::MaxVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_)
{
	S_VibCharValue vibValue;
	vibValue._fCharAll=max(vibValue1_._fCharAll,vibValue2_._fCharAll);
	vibValue._fCharOne=max(vibValue1_._fCharOne,vibValue2_._fCharOne);
	vibValue._fCharTwo=max(vibValue1_._fCharTwo,vibValue2_._fCharTwo);
	vibValue._fCharThree=max(vibValue1_._fCharThree,vibValue2_._fCharThree);
	vibValue._fCharHalf=max(vibValue1_._fCharHalf,vibValue2_._fCharHalf);
	vibValue._fCharRes=max(vibValue1_._fCharRes,vibValue2_._fCharRes);
	vibValue._fDC=max(vibValue1_._fDC,vibValue2_._fDC);
	vibValue._fGap=max(vibValue1_._fGap,vibValue2_._fGap);
	vibValue._fPhaseOne=max(vibValue1_._fPhaseOne,vibValue2_._fPhaseOne);
	vibValue._fPhaseTwo=max(vibValue1_._fPhaseOne,vibValue2_._fPhaseTwo);
	vibValue._fPhaseThree=max(vibValue1_._fPhaseThree,vibValue2_._fPhaseThree);
	vibValue._fPhaseHalf=max(vibValue1_._fPhaseHalf,vibValue2_._fPhaseHalf);
	return vibValue;
}

S_VibCharValue CTransactData::MinVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_)
{
	S_VibCharValue vibValue;
	vibValue._fCharAll=min(vibValue1_._fCharAll,vibValue2_._fCharAll);
	vibValue._fCharOne=min(vibValue1_._fCharOne,vibValue2_._fCharOne);
	vibValue._fCharTwo=min(vibValue1_._fCharTwo,vibValue2_._fCharTwo);
	vibValue._fCharThree=min(vibValue1_._fCharThree,vibValue2_._fCharThree);
	vibValue._fCharHalf=min(vibValue1_._fCharHalf,vibValue2_._fCharHalf);
	vibValue._fCharRes=min(vibValue1_._fCharRes,vibValue2_._fCharRes);
	vibValue._fDC=min(vibValue1_._fDC,vibValue2_._fDC);
	vibValue._fGap=min(vibValue1_._fGap,vibValue2_._fGap);
	vibValue._fPhaseOne=min(vibValue1_._fPhaseOne,vibValue2_._fPhaseOne);
	vibValue._fPhaseTwo=min(vibValue1_._fPhaseOne,vibValue2_._fPhaseTwo);
	vibValue._fPhaseThree=min(vibValue1_._fPhaseThree,vibValue2_._fPhaseThree);
	vibValue._fPhaseHalf=min(vibValue1_._fPhaseHalf,vibValue2_._fPhaseHalf);
	return vibValue;
}

S_VibCharValue CTransactData::SumVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_)
{
	S_VibCharValue vibValue;
	vibValue._fCharAll=vibValue1_._fCharAll+vibValue2_._fCharAll;
	vibValue._fCharOne=vibValue1_._fCharOne+vibValue2_._fCharOne;
	vibValue._fCharTwo=vibValue1_._fCharTwo+vibValue2_._fCharTwo;
	vibValue._fCharThree=vibValue1_._fCharThree+vibValue2_._fCharThree;
	vibValue._fCharHalf=vibValue1_._fCharHalf+vibValue2_._fCharHalf;
	vibValue._fCharRes=vibValue1_._fCharRes+vibValue2_._fCharRes;
	vibValue._fDC=vibValue1_._fDC+vibValue2_._fDC;
	vibValue._fGap=vibValue1_._fGap+vibValue2_._fGap;
	vibValue._fPhaseOne=vibValue1_._fPhaseOne+vibValue2_._fPhaseOne;
	vibValue._fPhaseTwo=vibValue1_._fPhaseOne+vibValue2_._fPhaseTwo;
	vibValue._fPhaseThree=vibValue1_._fPhaseThree+vibValue2_._fPhaseThree;
	vibValue._fPhaseHalf=vibValue1_._fPhaseHalf+vibValue2_._fPhaseHalf;
	return vibValue;
}

CString CTransactData::GetAlarmTypeDiscribe(unsigned int iAlarmType_)
{
	CString strDisb="";
	CString strCharType,strJudgeType,strChannType;
	//得到判断报警的类型描述
	switch(iAlarmType_&0xFF000000)
	{
	case D_BHKD_AT_FAST:
		strJudgeType="快变报警";
		break;
	case D_BHKD_AT_SLOW:
		strJudgeType="缓变报警";
		break;
	case D_BHKD_AT_TREND:
		strJudgeType="趋势报警";
		break;
	case D_BHKD_AT_REGULAR:
	default:
		strJudgeType="常规报警";
		break;
	}
	//得到测点类型描述
	switch(iAlarmType_&0x00FF0000)
	{
	case D_BHKD_AT_LOCATION:
		strChannType="轴心位置";
		break;
	case D_BHKD_AT_TEMP:
		strChannType="温度";
		break;
	case D_BHKD_AT_PRESS:
		strChannType="压力";
		break;
	case D_BHKD_AT_FLUX:
		strChannType="流量";
		break;
	case D_BHKD_AT_OTHERPROC:
		strChannType="其它类型测点";
		break;
	case D_BHKD_AT_RADIALVIB:
		strChannType="径向振动";
		break;
	case D_BHKD_AT_AXISVIB:
		strChannType="轴向振动";
		break;
	default:
		strChannType="径向振动";
	}
	//得到特征值类型
	switch(iAlarmType_&0x0000FF00)
	{
	case D_BHKD_AT_CHARONE:
		strCharType="1X倍频";
		break;
	case D_BHKD_AT_CHARTWO:
		strCharType="2X倍频";
		break;
	case D_BHKD_AT_CHARTHREE:
		strCharType="3X倍频";
		break;
	case D_BHKD_AT_CHARHALF:
		strCharType="1/2分频";
		break;
	case D_BHKD_AT_CHARRES:
		strCharType="残振";
		break;
	case D_BHKD_AT_CHARALL:
		strCharType="通频";
		break;
	case D_BHKD_AT_AXISDISPLACE:
		strCharType="轴位移";
		break;
	default:
		strCharType="通频";
	}
	strDisb=strChannType+strCharType+strJudgeType;
	return strDisb;
}

E_TBL_CHANNTYPE CTransactData::GetChannOneType(const CHANNTYPE &iChannType_)
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

CString  CTransactData::GetBalanceSql(int		sqlType,									  
									  int		anBalanceType,
									  CString   astrSuffix,
									  CString	lstrChannNo1,
									  CString	lstrChannNo2,
									  CString   lstrDateTimeStart,
									  CString	lstrDateTimeEnd,
									  int anBufferLen1/* = 0*/,
									  int anBufferLen2/* = 0*/)
{
	CString lstrSql;	

	CString lstrTableName;

	lstrTableName.Format(_T("%s_%s"),gc_balanceTable,astrSuffix);
	
	switch(sqlType)
	{
	case CTransactData::SELECT:
		
		if (lstrDateTimeEnd.GetLength()>0)
		{
			CString lstrSqlFormat = "select * from  [%s] where \
									([%s] between '%s' and '%s') \
									and ([%s] = %d) order by [%s] desc";

			lstrSql.Format( lstrSqlFormat,
							lstrTableName,
							gc_balancetable_datetime,
							lstrDateTimeStart,
							lstrDateTimeEnd,
							gc_balancetable_type,
							anBalanceType,
							gc_balancetable_datetime
							);

		}else
		{
			CString lstrSqlFormat = "select * from  [%s] where [%s] between  = '%s'\
									and [%s] = %d order by [%s] desc";

			lstrSql.Format( lstrSqlFormat,
							lstrTableName,
							gc_balancetable_datetime,
							lstrDateTimeStart,
							gc_balancetable_type,
							anBalanceType,
							gc_balancetable_datetime
							);
		}

		break;
	case CTransactData::SELECT_FOR_UPDATE:

		if (lstrDateTimeEnd.GetLength()>0)
		{
			CString lstrSqlFormat = "select * from  [%s] where \
									([%s] between '%s' and '%s') \
									and ([%s] = %d)";

			lstrSql.Format( lstrSqlFormat,
				lstrTableName,
				gc_balancetable_datetime,
				lstrDateTimeStart,
				lstrDateTimeEnd,
				gc_balancetable_type,
				anBalanceType
				);

		}else
		{
			CString lstrSqlFormat = "select * from  [%s] where [%s] between  = '%s'\
									and [%s] = %d ";

			lstrSql.Format( lstrSqlFormat,
				lstrTableName,
				gc_balancetable_datetime,
				lstrDateTimeStart,
				gc_balancetable_type,
				anBalanceType
				);
		}

		break;
	case CTransactData::INSERT:
		{

			if (anBalanceType>0)
			{
				CString lstrSqlFormat = "INSERT INTO [%s]  ([%s],[%s],[%s],[%s],\
										[%s],[%s],[%s],[%s] ) select '%s','%s','%s',\
										%d,isnull(max(%s), 0)+1, 0x%x, %d,%d from [%s]";

				lstrSql.Format( lstrSqlFormat,
								lstrTableName,

								gc_balanceTable_pt1,								
								gc_balanceTable_pt2,								
								gc_balancetable_datetime,							
								gc_balancetable_type,							
								gc_cID,
								gc_balancetable_data_pt_1,
								gc_balancetable_data_len_pt_1,
								gc_balancetable_data_len_pt_2,

								lstrChannNo1,
								lstrChannNo2,
								lstrDateTimeStart,
								1,
								gc_cID,
								0,
								anBufferLen1,
								anBufferLen2,

								lstrTableName
								);
			}else
			{
				CString lstrSqlFormat = "INSERT INTO [%s]  ([%s],[%s],[%s],[%s],[%s],[%s]) \
										 select '%s','%s',%d,\
										 isnull(max(%s), 0)+1,0x%x, %d from [%s]";


				lstrSql.Format( lstrSqlFormat,
								lstrTableName,

								gc_balanceTable_pt1,	
								gc_balancetable_datetime,							
								gc_balancetable_type,							
								gc_cID,
								gc_balancetable_data_pt_1,
								gc_balancetable_data_len_pt_1,

								lstrChannNo1,
								lstrDateTimeStart,
								0,
								gc_cID,
								0,
								anBufferLen1,

								lstrTableName
								);
			}
		}
		break;

	default:
		break;
	}

	return lstrSql;
}

/**接收动平衡类型
*输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
*                记录时间       FIELD_SERVICE_OFF_TIME
*                测点名         FIELD_SYSTEM_CHANN
				 测点名2        FIELD_SYSTEM_CHANN2
*                BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
				 BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
				 BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
				 BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*返回值的字段码：
*                同步状态      FIELD_SYSTEM_STATUS               1,保存成功 0，保存失败                 
*
#define     SERVICE_SERVICE_OFF_SET_BALANCE_DATA 24062    
*/



int CTransactData::SetBalanceData(ICommInterFace * pFixComm ,IDBInterFace *pDB)
{
	if ( NULL == pDB || NULL == pFixComm)
	{
		return -1;
	}
	//1.得到公司\分厂\设备\测点名\动平衡类型
	string sCompany,sFactory,sPlant;
	
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	int lnBalanceType = lpFixCom->GetLong(FIELD_SERVICE_OFF_BALANCE_TYPE);
	
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	
	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -1;
	}
	
	//2.切换到分库

	SwitchDbNameEx(pDB,sAlias.c_str());

	//3.写入数据	
	//3.0 根据侧点名，得到测点号
	
	string lstrChann1ID = lpFixCom->Get(FIELD_SYSTEM_CHANN);

	string lstrChann2ID = lpFixCom->Get(FIELD_SYSTEM_CHANN2);

	S_ChannInfo loChannInfo1 = pPlantInfo->GetSpecVibChannInfo(lstrChann1ID);

	S_ChannInfo loChannInfo2 = pPlantInfo->GetSpecVibChannInfo(lstrChann2ID);

	CString lstrDateTime = lpFixCom->Get(FIELD_SERVICE_OFF_TIME);

	CString lstrChann1NO = loChannInfo1._cChannNo;

	CString lstrChann2NO = loChannInfo2._cChannNo;

	//3.1 得到测点buffer，时间
	
	int lnChann1Len = lpFixCom->GetLong(FIELD_SERVICE_OFF_DATA_CHANN_1_LEN);

	int lnChann2Len = lpFixCom->GetLong(FIELD_SERVICE_OFF_DATA_CHANN_2_LEN);

	if (lnChann1Len == 0)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -2;
	}

	std::vector<BYTE> loBuffer;

	loBuffer.resize(lnChann1Len);

	BYTE * lpBuffer1 = &loBuffer.front();

	lpFixCom->GetItemBuf(FIELD_SERVICE_OFF_DATA_CHANN_1,(CHAR *)lpBuffer1,lnChann1Len);

	std::vector<BYTE> loBuffer2;

	BYTE * lpBuffer2 = NULL;

	if (lnChann2Len>0)
	{
		loBuffer2.resize(lnChann2Len);

		lpBuffer2 = &loBuffer2.front();

		lpFixCom->GetItemBuf(FIELD_SERVICE_OFF_DATA_CHANN_2,(CHAR *)lpBuffer2,lnChann2Len);
	}

	CString lstrSuffix ;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());


	CString lstrSql =	this->GetBalanceSql(	    SELECT_FOR_UPDATE,
													lnBalanceType,
													lstrSuffix,
													lstrChann1NO,
													lstrChann2NO,
													lstrDateTime,
													lstrDateTime);

	pDB->Cmd(lstrSql);

	if (pDB->More())
	{
		pDB->SetBuffer(gc_balancetable_data_pt_1,lpBuffer1,lnChann1Len);

		if (lnChann2Len>0)
		{
			pDB->SetBuffer(gc_balancetable_data_pt_2,lpBuffer2,lnChann2Len);
		}

		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,1);
		// lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());

		return 1;
	}




	//3.2 insert item
	lstrSql = this->GetBalanceSql(	INSERT,
											lnBalanceType,
											lstrSuffix,
											lstrChann1NO,
											lstrChann2NO,
											lstrDateTime,
											lstrDateTime,
											lnChann1Len,
											lnChann2Len);

	
	
	pDB->Cmd(lstrSql);

	if (!pDB->More())
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -3;
	}

	//4.写入buffer

	 lstrSql =	this->GetBalanceSql(	    SELECT_FOR_UPDATE,
											lnBalanceType,
											lstrSuffix,
											lstrChann1NO,
											lstrChann2NO,
											lstrDateTime,
											lstrDateTime);

	 pDB->Cmd(lstrSql);

	 if (!pDB->More(TRUE))
	 {
		 lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		 lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		 lpFixCom->CreateEndMak();
		 lpFixCom->Write(lpFixCom->GetAci());
		 return -4;
	 }

	 pDB->SetBuffer(gc_balancetable_data_pt_1,lpBuffer1,lnChann1Len);

	 if (lnChann2Len>0)
	 {
		  pDB->SetBuffer(gc_balancetable_data_pt_2,lpBuffer2,lnChann2Len);
	 }

	 lpFixCom->CreateAnswer(lpFixCom->GetFunc());
	 lpFixCom->SetItem(FIELD_SYSTEM_STATUS,1);
	// lpFixCom->CreateEndMak();
	 lpFixCom->Write(lpFixCom->GetAci());

	return 1;
}

/**返回动平衡类型
*  输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
				 记录时间开始   FIELD_SERVICE_OFF_TIME
				 记录时间结束   FIELD_SERVICE_OFF_TIMEEND
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*   返回值的字段码：
*               记录时间       FIELD_SERVICE_OFF_TIME
*               测点名         FIELD_SYSTEM_CHANN
*				测点名2        FIELD_SYSTEM_CHANN2
*               BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
*				BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
*				BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
*				BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节                     

*#define     SERVICE_SERVICE_OFF_SET_BALANCE_DATA 24063*/

int CTransactData::GetBalanceData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	if ( NULL == pDB || NULL == pFixComm)
	{
		return -1;
	}
	
	//1.得到公司\分厂\设备\测点名\动平衡类型
	string sCompany,sFactory,sPlant;

	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	int lnBalanceType = lpFixCom->GetLong(FIELD_SERVICE_OFF_BALANCE_TYPE);

	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);

	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -1;
	}

	//2.切换到分库

	SwitchDbNameEx(pDB,sAlias.c_str());

	CString lstrSuffix ;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());

	//2.得到测点号
	string lstrChann1ID = lpFixCom->Get(FIELD_SYSTEM_CHANN);

	string lstrChann2ID = lpFixCom->Get(FIELD_SYSTEM_CHANN2);

	S_ChannInfo loChannInfo1 = pPlantInfo->GetSpecVibChannInfo(lstrChann1ID);

	S_ChannInfo loChannInfo2 = pPlantInfo->GetSpecVibChannInfo(lstrChann2ID);

	CString lstrDateTime = lpFixCom->Get(FIELD_SERVICE_OFF_TIME);

	CString lstrDateTimeEnd =  lpFixCom->Get(FIELD_SERVICE_OFF_TIMEEND);

	CString lstrChann1NO = loChannInfo1._cChannNo;

	CString lstrChann2NO = loChannInfo2._cChannNo;


	//3.得到SQL

	CString lstrSql = this->GetBalanceSql(	SELECT,
											lnBalanceType,
											lstrSuffix,
											lstrChann1NO,
											lstrChann2NO,
											lstrDateTime,
											lstrDateTimeEnd);


	//4. 执行查询
	pDB->Cmd(lstrSql);

	lpFixCom->CreateAnswerMul(lpFixCom->GetFunc());

	int lnDataCount = 0;

	while(pDB->More())
	{		
		long lnBufferLength = pDB->GetLong(gc_balancetable_data_len_pt_1);

		if (lnBufferLength<=0)
		{
			continue;
		}

		std::vector<BYTE> loBuffer;

		loBuffer.resize(lnBufferLength);

		BYTE * lpBuffer = & loBuffer.front();
		CWaveHead sWaveHead;

		pDB->GetBuffer(gc_balancetable_data_pt_1,lpBuffer,lnBufferLength,sWaveHead);

		CBHDateTime  loColeDateTime = pDB->GetTime(gc_balancetable_datetime);

		lpFixCom->SetItem(FIELD_SERVICE_OFF_TIME,loColeDateTime);

		string lstrChannNo = pDB->Get(gc_balanceTable_pt1);

		S_ChannInfo  loChannInfo = pPlantInfo->GetSpecVibChannNoInfo(lstrChannNo);

		CString lstrChannId = loChannInfo._cChannID;

		lpFixCom->SetItem(FIELD_SYSTEM_CHANN,lstrChannId);

		lpFixCom->SetItem(FIELD_SERVICE_OFF_DATA_CHANN_1_LEN,lnBufferLength);

		lpFixCom->SetItemBuf(FIELD_SERVICE_OFF_DATA_CHANN_1,(CHAR *)lpBuffer,lnBufferLength);

		if (lnBalanceType>0)
		{
			 lnBufferLength = pDB->GetLong(gc_balancetable_data_len_pt_2);

			if (lnBufferLength<=0)
			{
				continue;
			}

			loBuffer.resize(lnBufferLength);

			lpBuffer = & loBuffer.front();
			CWaveHead sWaveHead;

			pDB->GetBuffer(gc_balancetable_data_pt_2,lpBuffer,lnBufferLength,sWaveHead);

			lstrChannNo = pDB->Get(gc_balanceTable_pt2);

			loChannInfo = pPlantInfo->GetSpecVibChannNoInfo(lstrChannNo);

			lstrChannId = loChannInfo._cChannID;

			lpFixCom->SetItem(FIELD_SYSTEM_CHANN2,lstrChannId);

			lpFixCom->SetItem(FIELD_SERVICE_OFF_DATA_CHANN_2_LEN,lnBufferLength);

			lpFixCom->SetItemBuf(FIELD_SERVICE_OFF_DATA_CHANN_2,(CHAR *)lpBuffer,lnBufferLength);
			
		}

		lpFixCom->Write(lpFixCom->GetAci());
		
		lpFixCom->CreateNextAnswer();

		lnDataCount++;
	}


	//lpFixCom->CreateEndMak();

	lpFixCom->CreateEndMak();
	lpFixCom->Write(lpFixCom->GetAci());


	return lnDataCount;
}

/*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	     分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          启停车类型：  FIELD_SYSTEM_TYPE
  *          试车事件名    FIELD_SYSTEM_EVENT_NAME
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  *         #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX    21077 
  */


CString CTransactData::GetSqlForSud( int sqlType, 
									 ICommInterFace * pFixComm,
									 CString & arefStringSuffix)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	if (ReadCheckCompanyInfo( sCompany,
									sFactory,
									sPlant,
									sAlias,
									pFixComm)==NULL)
	{
		return CString(_T(""));
	}

	arefStringSuffix = sAlias.c_str();

	string sTimeStart,sTimeEnd,lstrSud_Name=gc_cSUD_EVENT_Prefix;
	CString lstrSud_LOG = gc_cSUD_LOG_Prefix;
	lstrSud_Name.append(sAlias);
	lstrSud_LOG.Append(sAlias.c_str());

	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	
	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	sTimeStart = timeStart.Format(gc_cTimeFormat);//CPlantInfoBase::TimeConvertString(timeStart);
	sTimeEnd   = timeEnd.Format(gc_cTimeFormat);//CPlantInfoBase::TimeConvertString(timeEnd);
	CString lstrEventName = pFixComm->Get(FIELD_SYSTEM_EVENT_NAME);
	int lnEventId = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	

	CString lstrSql;
	CString lstrTemp;
	switch (sqlType)
	{
	case CTransactData::SELECT:
		lstrTemp.Format("SELECT sl.*,isnull(sn.[%s],'') as wangjian FROM [%s] sl LEFT JOIN  [%s] sn    ",
						gc_cEventNAME,						
						lstrSud_LOG,
						lstrSud_Name.c_str()
						);
		lstrSql.Append(lstrTemp);

		lstrTemp.Format(" ON (sn.[%s] = sl.[%s]) WHERE  (sl.[%s]<='%s') AND (sl.[%s]>='%s')   ORDER BY sl.[%s] ",	
						gc_cEventID,
						gc_cEventID,			
						gc_cStart_T,
						sTimeEnd.c_str(),
						gc_cStart_T,
						sTimeStart.c_str(),
						gc_cStart_T);
		lstrSql.Append(lstrTemp);
		break;
	case CTransactData::SEARCH_SUD_NAME:
		lstrEventName = "%%" + lstrEventName;
		lstrEventName.Append("%%");

		lstrTemp.Format("SELECT sl.*,sn.[%s] FROM [%s] sl, [%s] sn    ",
			gc_cEventNAME,			
			lstrSud_LOG,
			lstrSud_Name.c_str()
			);
		lstrSql.Append(lstrTemp);

		lstrTemp.Format(" where (   sn.[%s] = sl.[%s])\
									AND (sl.[%s]<='%s') \
									AND (sl.[%s]>='%s') \
									AND (sn.[%s] like '%s') \
									ORDER BY sl.[%s] ",	
			gc_cEventID,
			gc_cEventID,			
			gc_cStart_T,
			sTimeEnd.c_str(),
			gc_cStart_T,
			sTimeStart.c_str(),
			gc_cEventNAME,
			lstrEventName,
			gc_cStart_T
			);
		lstrSql.Append(lstrTemp);
		break;
	case CTransactData::SELECT_FOR_UPDATE:

		lstrTemp.Format("SELECT sl.* FROM [%s] sl ",			
						lstrSud_Name.c_str()
						);
		lstrSql.Append(lstrTemp);

		lstrTemp.Format(" where ( sl.[%s] = %d)\
						ORDER BY sl.[%s] ",	
						gc_cEventID,
						lnEventId,
						gc_cStart_T
						);
		lstrSql.Append(lstrTemp);
		break;

	case CTransactData::UPDATE:	

		lstrTemp.Format("update [%s] set [%s] = %d, [%s] = '%s' ,[%s]= '%s',[%s] = '%s' WHERE  [%s] = %d",			
						lstrSud_Name.c_str(),

						gc_cEventID,
						lnEventId,

						gc_cEventNAME,
						lstrEventName,

						gc_cStart_T,
						sTimeStart.c_str(),

						gc_cEnd_T,			
						sTimeEnd.c_str(),

						gc_cEventID,
						lnEventId
						);
		lstrSql.Append(lstrTemp);
		break;

	case CTransactData::INSERT:

		lstrTemp.Format("INSERT INTO [%s] ( [%s],[%s],[%s],[%s],[%s]) values ( %d,%d,'%s','%s','%s' )",			
						lstrSud_Name.c_str(),
						
						gc_cID,
						gc_cEventID,
						gc_cEventNAME,
						gc_cStart_T,
						gc_cEnd_T,

						lnEventId,
						lnEventId,
						lstrEventName,
						sTimeStart.c_str(),
						sTimeEnd.c_str()
						);
		lstrSql.Append(lstrTemp);
		break;

	}
	ASSERT(!lstrSql.IsEmpty());
	return lstrSql;
}


        
int CTransactData::GetSpecTimeRangeSudAndName( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	int nCount=-1;
	CString lstrSuffix;
	CString lstrSql = this->GetSqlForSud(CTransactData::SELECT,pFixComm,lstrSuffix);
	if (lstrSql.IsEmpty())
	{
		ASSERT(FALSE);
		return -1;
	}
	SwitchDbNameEx(pDB,lstrSuffix);

	//获取每个启停车事件下的最大最小转速
	map<int,P_MAXMINREV> loRevInfo;

	map<int,P_MAXMINREV>::iterator lpIter;

	this->GetSudSpecEventMaxMinRev(pFixComm,pDB,loRevInfo);

	pDB->Cmd(lstrSql);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	
	while(pDB->More())
	{
		int lnEventID = pDB->GetLong(gc_cEventID);

		lpIter = loRevInfo.find(lnEventID);
		if (lpIter != loRevInfo.end())
		{
			P_MAXMINREV loMaxMinRev = lpIter->second;

			pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,loMaxMinRev.first);
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,loMaxMinRev.second);
		}
		else
		{
			continue;
		}

		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,lnEventID);
		CBHDateTime  timeStart=pDB->GetTime(gc_cStart_T);
		pFixComm->SetTime(FIELD_SERVICE_OFF_TIMESTART,timeStart);
		if (0==pDB->GetLong(gc_cStop_Flag))
		{
			pFixComm->SetTime(FIELD_SERVICE_OFF_TIMEEND,timeStart);
		}
		else
		{
			pFixComm->SetTime(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cEnd_T));
		}
		CString lstrEventName = pDB->Get("wangjian").c_str();
		if (lstrEventName.IsEmpty())
		{
			lstrEventName = timeStart.Format(gc_cTimeFormat);
		}
		
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_NAME,lstrEventName);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;	
}


int CTransactData::    GetSpecTimeRangeSudSearchByName( ICommInterFace * pFixComm, 
													IDBInterFace *pDB )
{
	int nCount=-1;
	CString lstrSuffix;
	CString lstrSql = this->GetSqlForSud(CTransactData::SEARCH_SUD_NAME,pFixComm,lstrSuffix);	
	if (lstrSql.IsEmpty())
	{
		BHLOG_DEBUG(lstrSql);
		ASSERT(FALSE);
		return -1;
	}
	SwitchDbNameEx(pDB,lstrSuffix);

	//获取每个启停车事件下的最大最小转速
	map<int,P_MAXMINREV> loRevInfo;

	map<int,P_MAXMINREV>::iterator lpIter;

	this->GetSudSpecEventMaxMinRev(pFixComm,pDB,loRevInfo);

	pDB->Cmd(lstrSql);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	while(pDB->More())
	{
		int lnEventID = pDB->GetLong(gc_cEventID);

		lpIter = loRevInfo.find(lnEventID);
		if (lpIter != loRevInfo.end())
		{
			P_MAXMINREV loMaxMinRev = lpIter->second;

			pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,loMaxMinRev.first);
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV,loMaxMinRev.second);
		}
		else
		{
			continue;
		}

		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,lnEventID);
		CBHDateTime  timeStart=pDB->GetTime(gc_cStart_T);
		pFixComm->SetTime(FIELD_SERVICE_OFF_TIMESTART,timeStart);
		if (0==pDB->GetLong(gc_cStop_Flag))
		{
			pFixComm->SetTime(FIELD_SERVICE_OFF_TIMEEND,timeStart);
		}
		else
		{
			pFixComm->SetTime(FIELD_SERVICE_OFF_TIMEEND,pDB->GetTime(gc_cEnd_T));
		}
		CString lstrEventName = pDB->Get(gc_cEventNAME).c_str();
		if (lstrEventName.IsEmpty())
		{
			lstrEventName = timeStart.Format(gc_cTimeFormat);
		}
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_NAME,lstrEventName);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cType));

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;	
}

int CTransactData::UpdateRenameEvent( ICommInterFace * pFixComm, 
									 IDBInterFace *pDB )
{
	int nCount=-1;
	CString lstrSuffix;
	CString lstrSql = this->GetSqlForSud(CTransactData::SELECT_FOR_UPDATE,pFixComm,lstrSuffix);
	if (lstrSql.IsEmpty())
	{
		ASSERT(FALSE);
		return -1;
	}
	SwitchDbNameEx(pDB,lstrSuffix);
	pDB->Cmd(lstrSql);
	if(pDB->More())
	{	lstrSql = this->GetSqlForSud(CTransactData::UPDATE,pFixComm,lstrSuffix);
		 pDB->Cmd(lstrSql);	
	}else
	{
		lstrSql = this->GetSqlForSud(CTransactData::INSERT,pFixComm,lstrSuffix);
		pDB->Cmd(lstrSql);	
	}
	pDB->Exec();
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
/*	pFixComm->CreateEndMak();*/
	pFixComm->Write(pFixComm->GetAci());
	return 1;	
}

int CTransactData::GetSudSpecEventMaxMinRev(ICommInterFace * pFixComm, IDBInterFace *pDB, map<int,P_MAXMINREV> &arRevInfo)
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	if (ReadCheckCompanyInfo( sCompany,
		sFactory,
		sPlant,
		sAlias,
		pFixComm)==NULL)
	{
		return 0;
	}

	string sTimeStart,sTimeEnd;
	
	CString lstrSud_LOG = gc_cSUD_LOG_Prefix;
	CString lstrSud_Data = "SUD_";
	CString lstrWhere;
	
	lstrSud_Data.Append(sAlias.c_str());
	lstrSud_LOG.Append(sAlias.c_str());
	
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();

	timeStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	timeEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	sTimeStart = timeStart.Format();
	sTimeEnd   = timeEnd.Format();

	lstrWhere.Format("[%s] between '%s' and '%s'",gc_cDateTime,sTimeStart.c_str(),sTimeEnd.c_str());	

	CString lstrSql;

	lstrSql.Format("select [%s],max([rev]) as 'MaxREV1',min([rev]) as 'MinREV1',\
					max([rev2]) as 'MaxREV2',min([rev2]) as 'MinREV2',\
					max([rev3]) as 'MaxREV3',min([rev3]) as 'MinREV3',\
					max([rev4]) as 'MaxREV4',min([rev4]) as 'MinREV4',\
					max([rev5]) as 'MaxREV5',min([rev5]) as 'MinREV5' from %s \
					where %s group by [%s]",
					gc_cEventID,lstrSud_Data,lstrWhere,gc_cEventID);

	pDB->Cmd(lstrSql);

	int lnCount = 0;

	int lnMaxRev = -1;

	int lnMinRev = -1;

	while(pDB->More())
	{
		P_MAXMINREV loMaxMinRev;

		int lnEventID = pDB->GetLong(gc_cEventID);

		int lnIndex = 1;

		lnMaxRev = pDB->GetLong("MaxREV1");

		if (lnMaxRev < pDB->GetLong("MaxREV2"))
		{
			lnIndex = 2;
			lnMaxRev = pDB->GetLong("MaxREV2");
		}

		if (lnMaxRev < pDB->GetLong("MaxREV3"))
		{
			lnIndex = 3;
			lnMaxRev = pDB->GetLong("MaxREV3");
		}

		if (lnMaxRev < pDB->GetLong("MaxREV4"))
		{
			lnIndex = 4;
			lnMaxRev = pDB->GetLong("MaxREV4");
		}

		if (lnMaxRev < pDB->GetLong("MaxREV5"))
		{
			lnIndex = 5;
			lnMaxRev = pDB->GetLong("MaxREV5");
		}

		if (lnMaxRev < 1)
		{
			continue;
		}

		loMaxMinRev.first = lnMaxRev;

		CString lstrTemp;

		lstrTemp.Format("%s%d","MinREV",lnIndex);
		loMaxMinRev.second = pDB->GetLong(lstrTemp);

		arRevInfo.insert(make_pair(lnEventID,loMaxMinRev));

		lnCount++;
	}

	return lnCount;
}

  /*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          测点类型    FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *		     是否修改成功：  FIELD_SYSTEM_STATUS
  *#define    SERVICE_SERVICE_OFF_SAVESPECTIMEDATA    21083
  */
int CTransactData::SaveSpecTypeDataByTimeRegion(ICommInterFace *pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;

	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,lpFixCom);

	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -1;
	}

	//2.切换到分库

	SwitchDbNameEx(pDB,sAlias.c_str());

	CString lstrSuffix ;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());

	int lnChannType = lpFixCom->GetLong(FIELD_SYSTEM_CHANN_TYPE);

	CBHDateTime loStart = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CBHDateTime loEnd = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	CString lstrTable;

	E_TREND_TBL_TYPE lenTableType = GE_TREND_TBLTYPE_MONTH;

	BOOL lbRet = TRUE;

	int lnTotalUpdatedCount = 0;

	while(lenTableType > GE_TREND_TBLTYPE_UNKNOWN)
	{
		lstrTable.Format("%s%s",GetTblNamePrefix((CHANNTYPE)lnChannType, lenTableType),lstrSuffix);

		CString lstrSql;

		lstrSql.Format("update [%s] set [id] = -[id] where ([%s] between '%s' and '%s') and ([id]>0)",
			lstrTable,gc_cTime,loStart.Format(gc_cTimeFormat),loEnd.Format(gc_cTimeFormat));

		pDB->Cmd(lstrSql);

		pDB->Exec();


		lenTableType = (E_TREND_TBL_TYPE)((int)lenTableType - 1);
	}
	
	lpFixCom->CreateAnswer(lpFixCom->GetFunc());

	lpFixCom->SetItem(FIELD_SYSTEM_STATUS,lbRet);

	lpFixCom->Write(lpFixCom->GetAci());

	return lbRet;
}

 /*!
 *输入参数字段码
 *          公司ID          FIELD_SYSTEM_COMPANY
 *	        分厂名        FIELD_SYSTEM_FACTORY
 *          机组ID：        FIELD_SERVICE_PLANTID
 *          标注时刻：      FIELD_SERVICE_OFF_TIME
 *          测点：          FIELD_SERVICE_CHANN1
 *          事件开始时间：  FIELD_SERVICE_OFF_TIMESTART
 *          事件结束时间：  FIELD_SERVICE_OFF_TIMEEND
 *          标注描述：      FIELD_SERVICE_REMARK  
 *          事件ID：        FIELD_SYSTEM_EVENT_ID     删除和更改时需要
 *          操作类型：      FIELD_SYSTEM_TYPE     1:保存 2. 更改  3.删除
 *          标注类型：      FIELD_SERVICE_DATA_TYPE  0:未知 1:报警统计自动标注  2. 人工标准 
 *返回值的字段码：
 *		   是否保存成功：  FIELD_SYSTEM_STATUS
 *#define    SERVICE_SERVICE_OFF_SAVESPECMARK    21084
 */
int CTransactData::SaveSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;

	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,lpFixCom);

	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -1;
	}

	int lnOperationType = lpFixCom->GetLong(FIELD_SYSTEM_TYPE); // 1:保存 2. 更改  3.删除
     
	int lnCurEventID = lpFixCom->GetLong(FIELD_SYSTEM_EVENT_ID);

	int lnRemarkType = lpFixCom->GetLong(FIELD_SERVICE_DATA_TYPE);  //0:未知 1:报警统计自动标注  2. 人工标准 

	CBHDateTime loDateTime = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIME);

	CString lstrChann = lpFixCom->GetItem(FIELD_SERVICE_CHANN1);

	CString lstrMarkInfo = lpFixCom->GetItem(FIELD_SERVICE_REMARK);

	CBHDateTime loEventStartTime = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CBHDateTime loEventEndTime = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMEEND);


	//2.切换到分库

	SwitchDbNameEx(pDB,sAlias.c_str());

	CString lstrSuffix,lstrTable,lstrSql;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());

	lstrTable.Format("%s%s","REMARK_",lstrSuffix);

	lstrSql.Format("select top 1 %s from %s order by %s desc",gc_cEventID,lstrTable,gc_cEventID);

	pDB->Cmd(lstrSql);

	long lnEventID = 0;

	if (pDB->More())
	{
		lnEventID = pDB->GetLong(gc_cEventID);
	}

	switch(lnOperationType)
	{
	case 1://保存
		{
			lstrSql.Format("insert into %s(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) values(%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s')",  \
				lstrTable,gc_cEventID,gc_cType,gc_cCompany,gc_cFactory_Name,gc_cPlantID,gc_cChannID,gc_cTime,gc_cStart_T,gc_cEnd_T,gc_cComments,  \
				lnEventID+1,lnRemarkType,sCompany.c_str(),sFactory.c_str(),sPlant.c_str(),lstrChann,loDateTime.Format(),loEventStartTime.Format(),loEventEndTime.Format(),lstrMarkInfo);
		}
		break;
	case 2://更改
		{
           lstrSql.Format("update [%s] set %s = '%s' where %s=%d",lstrTable,gc_cComments,lstrMarkInfo,gc_cEventID,lnCurEventID);
		}
		break;
	case 3://删除
		{
			lstrSql.Format("delete from [%s] where %s=%d",lstrTable,gc_cEventID,lnCurEventID);
		}
	default:
		break;
	}


	pDB->Cmd(lstrSql);

	bool lbRet = pDB->Exec();

	lpFixCom->CreateAnswer(lpFixCom->GetFunc());

	lpFixCom->SetItem(FIELD_SYSTEM_STATUS,lbRet);

	lpFixCom->Write(lpFixCom->GetAci());

	return lbRet;
}

/*!
*输入参数字段码
*          公司ID      FIELD_SYSTEM_COMPANY
*	       分厂名      FIELD_SYSTEM_FACTORY
*          机组ID：    FIELD_SERVICE_PLANTID
*          起始索引：  FIELD_SERVICE_STARTINDEX
*          标注描述：  FIELD_SYSTEM_MARKNAME    
*返回值的字段码：
*          事件ID：    FIELD_SYSTEM_EVENT_ID
*          测点：      FIELD_SERVICE_CHANN1
*          事件开始时间：  FIELD_SERVICE_OFF_TIMESTART
*          事件结束时间：  FIELD_SERVICE_OFF_TIMEEND
*		   标注描述：  FIELD_SERVICE_REMARK 
*          标注时间：  FIELD_SERVICE_OFF_TIME
*          标注类型：      FIELD_SERVICE_DATA_TYPE  0:未知 1:报警统计自动标注  2. 人工标准 
*#define    SERVICE_SERVICE_OFF_GETSPECMARK    21085
*/
int CTransactData::GetSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant;

	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,lpFixCom);

	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->CreateEndMak();
		lpFixCom->Write(lpFixCom->GetAci());
		return -1;
	}

	CBHDateTime ldtStart = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CBHDateTime ldtEnd = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	CString lstrMark = lpFixCom->GetItem(FIELD_SERVICE_REMARK);

	int lnStartIndex = lpFixCom->GetLong(FIELD_SERVICE_STARTINDEX);

	COleDateTimeSpan lodtSpan = ldtEnd-ldtStart;

	bool lbGetAll = false;
	//请求时间间隔小于1秒，则获取当前设备所有的事件标记
	if (lodtSpan.GetTotalSeconds() < 1)
	{
		lbGetAll = true;
	}

	//2.切换到分库

	SwitchDbNameEx(pDB,sAlias.c_str());

	CString lstrSuffix,lstrTable,lstrSql;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());

	lstrTable.Format("%s%s","REMARK_",lstrSuffix);

	if (lbGetAll)
	{
		if (lstrMark.Trim().IsEmpty())
		{
			lstrSql.Format("select * from %s where %s > %d order by %s", lstrTable,gc_cEventID,lnStartIndex,gc_cEventID);
		}
		else
		{
			lstrMark = "%%" + lstrMark;

			lstrMark.Append("%%");

			lstrSql.Format("select * from %s where %s like '%s'",lstrTable,gc_cComments,lstrMark);
		}
	}
	else
	{
		if (lstrMark.Trim().IsEmpty())
		{
			lstrSql.Format("select * from %s where %s between '%s' and '%s' and %s > %d order by %s", lstrTable,gc_cTime,ldtStart.Format(),ldtEnd.Format(),gc_cEventID,lnStartIndex,gc_cEventID);
		}
		else
		{
			lstrMark = "%%" + lstrMark;

			lstrMark.Append("%%");

			lstrSql.Format("select * from %s where %s between '%s' and '%s' and %s like '%s'",lstrTable,gc_cTime,ldtStart.Format(),ldtEnd.Format(),gc_cComments,lstrMark);
		}
	}
	
	pDB->Cmd(lstrSql);

	lpFixCom->CreateAnswerMul(lpFixCom->GetFunc());

	int lnDataCount = 0;

	while(pDB->More())
	{
		int lnEventID = pDB->GetLong(gc_cEventID);

		CString lstrChannName = pDB->Get(gc_cChannID).c_str();

		CBHDateTime loDateTime = pDB->GetTime(gc_cTime);

		CBHDateTime loEventStartTime = pDB->GetTime(gc_cStart_T);

		CBHDateTime loEventEndTime = pDB->GetTime(gc_cEnd_T);

		CString lstrMarkInfo = pDB->Get(gc_cComments).c_str();

		int lnRemarkType = pDB->GetLong(gc_cType);

		lpFixCom->SetItem(FIELD_SYSTEM_EVENT_ID,lnEventID);

		lpFixCom->SetItem(FIELD_SERVICE_CHANN1,lstrChannName);
		
		lpFixCom->SetItem(FIELD_SERVICE_OFF_TIME,loDateTime);

		lpFixCom->SetItem(FIELD_SERVICE_OFF_TIMESTART,loEventStartTime);

		lpFixCom->SetTime(FIELD_SERVICE_OFF_TIMEEND,loEventEndTime);

		lpFixCom->SetItem(FIELD_SERVICE_REMARK,lstrMarkInfo);

		lpFixCom->SetItem(FIELD_SERVICE_DATA_TYPE,lnRemarkType);

		lpFixCom->Write(lpFixCom->GetAci());

		lpFixCom->CreateNextAnswer();

		lnDataCount++;
	}

	lpFixCom->CreateEndMak();

	lpFixCom->Write(lpFixCom->GetAci());

	return lnDataCount;
}

int CTransactData::GetSudTrendCount()
{
	int lnTrendCount = 2000;

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lnTrendCount = ::GetPrivateProfileInt("SYSTEM","G_SUD_NUM",2000,cAppLocation);

	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	return lnTrendCount;
}

int CTransactData::GetSudJilianWaveCount()
{
	int lnJilianRecordNum = 400;

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lnJilianRecordNum = ::GetPrivateProfileInt("SYSTEM","G_JILIAN_NUM",400,cAppLocation);

	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	return lnJilianRecordNum;
}

int CTransactData::GetSpecChannSaveTimeSudByTimeRange( ICommInterFace *pFixComm, IDBInterFace *pDB )
{
	string sCompany,sFactory,sPlant;
	string sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm);
	if ( pPlantInfo == NULL )
		return -1;

	int lnJilianRecordNum = this->GetSudJilianWaveCount();

	SwitchDbNameEx(pDB,sAlias.c_str());

	string sChann=pFixComm->GetItem(FIELD_SERVICE_OFF_CHANN1);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	CBHDateTime dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIME);
	dtStart.SetMillisecond(0);
	int iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	S_ChannInfo channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);
	if((iLines>1600)||(iLines<200))
		iLines=401;

	string sTimeSud,sAllField,sTable,sSmpFreq,sSmpNum,sRev = "REV_",sWave="WAVE_",sAllChar="ALL";
	sWave.append(channInfo._cChannNo);
	sRev.append(channInfo._cChannNo);
	sAllChar.append(channInfo._cChannNo);

	sAllField = ConsVibChannField( channInfo._cChannNo, true);
	int iSensorType = channInfo._iSensorType;//GetSpecChannSensorType(sCompany,sPlant,sChann,iRevType,pDB);

	int nCount=-1;
	sTable="SUD_";
	sTable.append(sAlias);

	std::vector<CBHDateTime> lvDatetime;

	int lnRecordCount = 0;
	pDB->Cmd("select count(%s) as RecordCount from %s where %s = %d",gc_cEventID,sTable.c_str(),gc_cEventID,iEventID);
	if(pDB->More())
	{
		lnRecordCount = pDB->GetLong("RecordCount");
	}

	if(lnRecordCount/150 > 1)
	{
		int lnInterval = lnRecordCount/150;
		CBHDateTime ldtTime;
		pDB->Cmd("select top 20 %s from %s where %s = %d order by %s desc",
			gc_cTime,sTable.c_str(),gc_cEventID,iEventID,sRev.c_str());
		while(pDB->More())
		{
			ldtTime = pDB->GetTime(gc_cTime);
			if(find(lvDatetime.begin(),lvDatetime.end(),ldtTime) == lvDatetime.end())
			{
				lvDatetime.push_back(pDB->GetTime(gc_cTime));
			}
		}

		pDB->Cmd("select top 30 %s from %s where %s = %d order by %s desc",
			gc_cTime,sTable.c_str(),gc_cEventID,iEventID,sAllChar.c_str());
		while(pDB->More())
		{
			ldtTime = pDB->GetTime(gc_cTime);
			if(find(lvDatetime.begin(),lvDatetime.end(),ldtTime) == lvDatetime.end())
			{
				lvDatetime.push_back(pDB->GetTime(gc_cTime));
			}
		}

		
		pDB->Cmd("select top 1 %s from %s where %s = %d order by %s asc",
			gc_cTime,sTable.c_str(),gc_cEventID,iEventID,gc_cTime);
		if(pDB->More())
		{
			ldtTime = pDB->GetTime(gc_cTime);
			if(find(lvDatetime.begin(),lvDatetime.end(),ldtTime) == lvDatetime.end())
			{
				lvDatetime.push_back(pDB->GetTime(gc_cTime));
			}
		}

		lnRecordCount = 150;

		while(lnRecordCount > 0)
		{
			pDB->Cmd("select top 1 %s from %s where  %s = %d and %s in\
					 (select top %d %s from %s where %s > '%s' order by %s) order by %s desc",
				gc_cTime,sTable.c_str(),gc_cEventID,iEventID,gc_cTime,
				lnInterval,gc_cTime,sTable.c_str(),gc_cTime,ldtTime.Format(gc_cTimeFormat),
				gc_cTime,gc_cTime);
			if(pDB->More())
			{
				ldtTime = pDB->GetTime(gc_cTime);
				if(find(lvDatetime.begin(),lvDatetime.end(),ldtTime) == lvDatetime.end())
				{
					lvDatetime.push_back(pDB->GetTime(gc_cTime));
				}
			}
			lnRecordCount--;
		}

		CString lstrTimeArray;

		if(lvDatetime.size() > 0)
		{
			lstrTimeArray = "'" + lvDatetime[0].Format(gc_cTimeFormat) + "'";
			for(unsigned int i = 0; i < lvDatetime.size(); i++)
			{
				lstrTimeArray = lstrTimeArray + ",'" + lvDatetime[i].Format(gc_cTimeFormat)+"'";
			}
		}

		pDB->Cmd("select %s,%s from [%s] ",
			sAllField.c_str(),
			ConsCommonField(),
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d and %s in(%s) order by [%s],[%s]",
			gc_cEventID,iEventID,gc_cTime,lstrTimeArray,gc_cTime,gc_cMicroSecond);
	}
	else
	{
		pDB->Cmd("select %s,%s from [%s] ",
			sAllField.c_str(),
			ConsCommonField(),
			sTable.c_str());
		pDB->Cmd(" WHERE [%s]=%d order by [%s],[%s]",
			gc_cEventID,iEventID,gc_cTime,gc_cMicroSecond);
	}
	
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		int ifreq,inum,irev,iquest_wavenum,istep=0,istep2=0;
		S_VibCharValue vibCharValue;
		vibCharValue = ReadVibCharValue( channInfo._cChannNo, pDB);
		inum  = SimADO_GetSampleNumEx(pDB, channInfo);
		ifreq = SimADO_GetSampleFrequencyEx(pDB, channInfo);
		irev  = SimADO_GetRevEx(pDB, channInfo);
		CBHDateTime dtTime = pDB->GetTime(gc_cTime);
		dtTime.SetMillisecond(0);
		iMicroSecond = pDB->GetLong(gc_cMicroSecond);
		if(irev< 0)
		{
			continue;
		}
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME,dtTime);
		pFixComm->SetItem(FIELD_SERVICE_OFF_PHASE, vibCharValue._fPhaseOne);
		pFixComm->SetItem(FIELD_SERVICE_OFF_AMP,  vibCharValue._fCharOne);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S, (char *)&vibCharValue, sizeof(S_VibCharValue));
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV,irev);
		DATATYPE_WAVE *pDtWave=NULL,*pDtFreq=NULL;
		float *pfWave=NULL,*pfSpectrum=NULL;
		float df;
		iquest_wavenum=iLines*2.56;
		long isize=0;
		if (inum>0)
		{
			pfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pfSpectrum=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pDtFreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			isize=inum*sizeof(DATATYPE_WAVE);
			CWaveHead sWaveHead;
			SimADO_SafeGetBufferEx(pDB, sWave.c_str(), (BYTE *)pDtWave, isize, sWaveHead);
			inum=isize/sizeof(DATATYPE_WAVE);
			pPlantInfo->ConvertDTWaveToFloat(pfWave,pDtWave,inum,sWaveHead);

			{//细化谱和正常谱分析 2007.4.7添加
				float fStartFreq=0;
				int iRtn=pPlantInfo->ZoomSpectrumAnalysis(pfWave,inum,ifreq,pfSpectrum,iLines,df,fStartFreq);
				if (iRtn<0)
				{//错误
					iLines=0;
				}
				else
				{
					if (iRtn==2)
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1);
					}
					else
					{
						if (GE_SENSORTYPE_VOTEX==iSensorType)
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,2.818);
						else
							pPlantInfo->ConvertFloatToDTWave(pDtFreq,pfSpectrum,iLines,sWaveHead,1.414);
					}
				}
			}

			pFixComm->SetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)pDtFreq,iLines*sizeof(DATATYPE_WAVE));
			//pFixComm->SetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pDtWave,iWaveNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
			pFixComm->SetItem(FIELD_SERVICE_OFF_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iLines);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,sWaveHead.m_pParam[0]);
		}
		else
		{
			pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,inum);
		}

		if(pDtFreq!=NULL) delete [] pDtFreq ;
		if(pfWave!=NULL) delete [] pfWave;
		if(pDtWave!=NULL) delete [] pDtWave;
		if(pfSpectrum!=NULL) delete [] pfSpectrum;
		nCount++;

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

vector<CBHDateTime> CTransactData::GetSpecTimeByTimeRange(
												IDBInterFace *pDB,
												string sSuffix,
												CBHDateTime timeStart_,
												CBHDateTime timeEnd_,
												CString&  strTblName_,
												E_TBL_CHANNTYPE iTblType_,
												bool bAlarmTable_
												)
{
	//查常规表
	E_TREND_TBL_TYPE lenTblType = GE_TREND_TBLTYPE_SECOND;
	bool bNeedSubTbl = false;
	bool bUseSupTbl = false;

	CString strTimeStart = timeStart_.Format(gc_cTimeFormat);
	CString strTimeEnd   = timeEnd_.Format(gc_cTimeFormat);

	int D_TRENDRECORDS = 300;

	int lnInterval = 0;

	int nNum = 0;

	CString lstrTimeArray;
	string sShowField;
	CString strWhere;

	std::vector<CBHDateTime> lvDatetime;

	strWhere.Format(" WHERE [%s] BETWEEN '%s' AND '%s'",gc_cTime,strTimeStart,strTimeEnd);

	if(!bAlarmTable_)
	{
		if(iTblType_ == E_TBL_CHANNTYPE_VIB)
		{
			nNum = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_VIBCHANN,lenTblType,timeStart_,timeEnd_,bNeedSubTbl);

			strTblName_.Format("%s%s",GetTblNamePrefix(GE_VIBCHANN,lenTblType),sSuffix.c_str());
		}
		else
		{
			nNum = GetTrendRecordsNum(pDB,sSuffix,sShowField,GE_DYNPRESSCHANN,lenTblType,timeStart_,timeEnd_,bNeedSubTbl);

			strTblName_.Format("%s%s",GetTblNamePrefix(GE_DYNPRESSCHANN,lenTblType),sSuffix.c_str());
		}

		CString strTimeArrayRegular,strTimeArrayAlarm;

		if(nNum > 0)
		{
			lnInterval = nNum/D_TRENDRECORDS;
		}

		int lnID = 0;

		if(lnInterval > 0)
		{
			pDB->Cmd("select top 1 %s from %s where %s between '%s' and '%s' order by %s",
				gc_cID,strTblName_,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
			if(pDB->More())
			{
				lnID = pDB->GetLong(gc_cID);
			}
			pDB->Cmd("select %s from %s where (%s-(%d))%%%d = 0 and (%s between '%s' and '%s')",
				gc_cTime,strTblName_,gc_cID,lnID,lnInterval,gc_cTime,strTimeStart,strTimeEnd);
			while(pDB->More())
			{
				lvDatetime.push_back(pDB->GetTime(gc_cTime));
			}
		}
		else
		{
			if(nNum > 0)
			{
				pDB->Cmd("select %s from %s where %s between '%s' and '%s' order by %s",
					gc_cTime,strTblName_,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
				while(pDB->More())
				{
					lvDatetime.push_back(pDB->GetTime(gc_cTime));
				}
			}
		}

	}
	else
	{
		pDB->Cmd("select count(%s) as RecordCount from %s %s",gc_cTime,strTblName_,strWhere);
		if(pDB->More())
		{
			nNum = pDB->GetLong("RecordCount");
		}
		if(nNum > 0)
		{
			lnInterval = nNum/D_TRENDRECORDS;
		}
		if(lnInterval > 0)
		{
			CString lstrTblName;
			int lnEventID = 1;
			map<int,CBHDateTime> loEventID;
			lstrTblName.Format("ALARM_LOG_%s",sSuffix.c_str());

			pDB->Cmd("select count(distinct %s) as RecordCount from %s where %s between '%s' and '%s'",
				gc_cEventID,lstrTblName,gc_cStart_T,
				strTimeStart,
				strTimeEnd);
			if(pDB->More())
			{
				nNum = pDB->GetLong("RecordCount");
			}

			lnInterval = nNum/D_TRENDRECORDS;
			if(lnInterval > 0)
			{
				
				pDB->Cmd("select top 1 %s from %s where %s between '%s' and '%s' order by %s",
					gc_cEventID,lstrTblName,gc_cStart_T,strTimeStart,strTimeEnd,gc_cStart_T);
				if(pDB->More())
				{
					lnEventID = pDB->GetLong(gc_cEventID) + 1;
				}
				pDB->Cmd("select distinct %s,%s from %s where (%s-(%d))%%%d = 0 and (%s between '%s' and '%s')",\
					gc_cEventID,gc_cStart_T,lstrTblName,gc_cEventID,lnEventID,lnInterval,
					gc_cStart_T,strTimeStart,strTimeEnd);
				while(pDB->More())
				{ 
					int lnTempEventID = pDB->GetLong(gc_cEventID);

					CBHDateTime loTimeStart = pDB->GetTime(gc_cStart_T);

					loEventID.insert(make_pair(lnTempEventID,loTimeStart));
				}
			}
			else
			{
				pDB->Cmd("select distinct %s,%s from %s where %s between '%s' and '%s'",\
					gc_cEventID,gc_cStart_T,lstrTblName,gc_cStart_T,strTimeStart,strTimeEnd);
				while(pDB->More())
				{
					int lnTempEventID = pDB->GetLong(gc_cEventID);

					CBHDateTime loTimeStart = pDB->GetTime(gc_cStart_T);

					loEventID.insert(make_pair(lnTempEventID,loTimeStart));
				}
			}

			if(loEventID.size() > 0)
			{
				int nGetDataCount = 1;
				if(loEventID.size() < D_TRENDRECORDS)
				{
					nGetDataCount = D_TRENDRECORDS/loEventID.size();
				}

				for(map<int,CBHDateTime>::iterator lpIter = loEventID.begin(); lpIter != loEventID.end(); ++lpIter)
				{
					pDB->Cmd("select top %d %s from %s where %s = %d and %s >= '%s'  order by %s asc ",
						nGetDataCount,gc_cTime,strTblName_,gc_cEventID,lpIter->first,gc_cDateTime,lpIter->second.Format(),gc_cTime);
					while(pDB->More())
					{
						lvDatetime.push_back(pDB->GetTime(gc_cTime));
					}
				}
			}
		}
		else
		{
			if(nNum > 0)
			{
				pDB->Cmd("select %s from %s where %s between '%s' and '%s' order by %s ",
					gc_cTime,strTblName_,gc_cTime,strTimeStart,strTimeEnd,gc_cTime);
				while(pDB->More())
				{
					lvDatetime.push_back(pDB->GetTime(gc_cTime));
				}
			}
		}
	}

	

	return lvDatetime;
}

int CTransactData::GetNormalWave( ICommInterFace *pFixComm,IDBInterFace *pDB )
{

	string sCompany_ =pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory_ =pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant_   =pFixComm->Get(FIELD_SERVICE_PLANTID);

	string sSuffix;
	CPlantInfoOffLine *pPlantInfo=CheckCompanyInfo(sCompany_,sFactory_,sPlant_,sSuffix,pFixComm);
	if(pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	string sChannID_ =pFixComm->Get(FIELD_SYSTEM_CHANN);
	int    lnAlarmID =pFixComm->GetLong(FIELD_SERVICE_ALARMID);
	CBHDateTime ltAlarmTime = pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CString lstrTable = "ALARM_LOG_";
	lstrTable.Append(sSuffix.c_str());

	SwitchDbNameEx(pDB,sSuffix.c_str());

	CString lstrWorkStatus;
	CString lstrChannNo = pPlantInfo->GetSpecVibChannInfo(sChannID_)._cChannNo;

	pDB->Cmd("SELECT WORKSTATUS FROM %s WHERE %s = %d AND %s = '%s' AND %s = '%s'",
		lstrTable,gc_cEventID,lnAlarmID,gc_cName,lstrChannNo,gc_cStart_T,ltAlarmTime.Format());

	if(pDB->More())
	{
		lstrWorkStatus = pDB->Get("WORKSTATUS").c_str();
	}


	CString lstrGroupName = pPlantInfo->GetGroupName().c_str();
	CString lstrCompanyName = pPlantInfo->GetCompanyName().c_str();
	CString lstrFactoryName = pPlantInfo->GetFactoryName().c_str();
	CString lstrPlantNo = pPlantInfo->GetPlantNo().c_str();

	CString lstrDataPath;

	CFileFind lfileFind;

	CString lstrFileName = lstrWorkStatus;

	CString lstrPath=::GetAppPath()+_T("AlarmData\\");

	const TCHAR * lpPathSplit = _T("\\");

	lstrPath.Append(lstrGroupName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrCompanyName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrFactoryName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrPlantNo);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrChannNo);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append("Wave");

	if(lstrWorkStatus.Trim().GetLength() > 0)
	{
		lstrPath.AppendFormat("\\%s*.txt",lstrWorkStatus);
	}
	else
	{
		lstrPath.AppendFormat("\\*.txt");
	}

	BOOL lbFound = lfileFind.FindFile(lstrPath);

	CString lstrDataFileName;

	while(lbFound)
	{
		lbFound = lfileFind.FindNextFile();
		if(lfileFind.IsDots())
		{
			continue;
		}
		if(!lfileFind.IsDirectory())
		{

			if(lstrWorkStatus.Trim().GetLength() > 0 )
			{
				if(lfileFind.GetFileName().Find(lstrWorkStatus) >= 0)
				{
					lstrDataFileName = lfileFind.GetFileName();
					lstrDataPath = lfileFind.GetFilePath();
					break;
				}

			}
			else
			{
				lstrDataFileName = lfileFind.GetFileName();
				lstrDataPath = lfileFind.GetFilePath();
				break;
			}

		}
	}

	lfileFind.Close();

	int lnDataLen = 0;

	if(lstrDataFileName.GetLength() <= 0)
	{
		return FALSE;
	}


	CString lstrTmp = lstrDataFileName;

	int lnPos = lstrTmp.ReverseFind('-');

	if(lnPos > 0)
	{
		lstrTmp = lstrTmp.Right(lstrTmp.GetLength() - lnPos - 1);
		lnPos = lstrTmp.Find(".txt");

		if(lnPos > 0)
		{
			lstrTmp = lstrTmp.Left(lnPos);
			lnDataLen = atoi(lstrTmp);
		}
	}

	if(lnDataLen <= 0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	vector<float> lvData;

	lvData.resize(lnDataLen);

	ifstream lf(lstrDataPath,ios::binary);
	if(lf)
	{
		lf.read((char*)&lvData.front(),lnDataLen*sizeof(float));
	}

	lf.close();

	if(lvData.size() > 0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());

		pFixComm->SetItemBuf(FIELD_SERVICE_OFF_NORMAL_WAVE,(char*)&lvData[1],(int)(lvData.size()-1)*sizeof(float));
		pFixComm->SetItem(FIELD_SERVICE_OFF_WAVE_NUM,(int)lvData.size()-1);
		pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,(int)lvData[0]);

		pFixComm->CreateEndMak();
		pFixComm->Write();
	}

	return 1;


}

/**得到异常信号波形
  *        起始索引  FIELD_SERVICE_STARTINDEX
  *返回的字段码：
  *        公司名          FIELD_SYSTEM_COMPANY
  *        分厂名          FIELD_SYSTEM_FACTORY
  *        设备名          FIELD_SERVICE_PLANTID
  *        测点名          FIELD_SYSTEM_CHANN
  *        波形            FIELD_SERVICE_OFF_WAVE
  *        波形点数        FIELD_SERVICE_OFF_WAVE_NUM            
  *        结束索引        FIELD_SERVICE_ENDINDEX
  #define    SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE         21095
  */
int CTransactData::GetErrorSignalWave( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int lnStartIndex = pFixComm->GetLong(FIELD_SERVICE_STARTINDEX);
	int lnWaveCount  = pFixComm->GetLong(FIELD_SERVICE_FREQWAVE_NUM);

	char cAppLocation[512]={0};
	GetCurrentDirectory(512,cAppLocation);
	strcat(cAppLocation,"\\ErrorSignal");
	CString lstrFile = cAppLocation;

	lstrFile.Append("\\*.txt");

	CFileFind lfileFind;

	BOOL lbFound = lfileFind.FindFile(lstrFile);

	CString lstrDataFileName;

	COleDateTime ltFileWriteTime = COleDateTime(2000,1,1,1,1,1);


	CString lstrLogFile;
	CString lstrLogFileName;

	map<COleDateTime,CString> lmTotalFile;

	while(lbFound)
	{
		lbFound = lfileFind.FindNextFile();
		if(lfileFind.IsDots())
		{
			continue;
		}
		if(!lfileFind.IsDirectory())
		{

			CTime lTime;

			lfileFind.GetLastWriteTime(lTime);

			CString lstrTime;

			lstrTime.Format("%d-%d-%d %d:%d:%d",lTime.GetYear(),lTime.GetMonth(),lTime.GetDay(),lTime.GetHour(),lTime.GetMinute(),lTime.GetSecond());


			COleDateTime ltTime;
			ltTime.SetDateTime(lTime.GetYear(),lTime.GetMonth(),lTime.GetDay(),lTime.GetHour(),lTime.GetMinute(),lTime.GetSecond());


			lstrLogFile = lfileFind.GetFilePath();
			lstrLogFileName = lfileFind.GetFileName();

			lmTotalFile.insert(make_pair(ltTime,lstrLogFile));

			if(lmTotalFile.size() >= 1000)
			{
				break;
			}

		}
	}

	lfileFind.Close();

	int lnIndex = 0;
	int lnRevCount = 0;
	if(lnStartIndex >= lmTotalFile.size())
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return 0;

	}


	CString lstrCompany,lstrFactory,lstrPlant,lstrChann;

	int lnSendCount = 0;

	int  lnTotalSize = 0;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for(map<COleDateTime,CString>::iterator it = lmTotalFile.begin(); it != lmTotalFile.end(); ++it)
	{
		if(lnIndex < lnStartIndex)
		{
			lnIndex++;
			continue;
		}

		CString lstrFileName = it->second;

		int lnPos = lstrFileName.ReverseFind('\\');

		if(lnPos > 0)
		{
			lstrFileName = lstrFileName.Right(lstrFileName.GetLength() - lnPos - 1);
		}

		if(lstrFileName.GetLength() > 0)
		{
			lnPos = lstrFileName.Find("-");

			int lnCount = 0;
			while(lnPos > 0)
			{
				if(lnCount == 0)
				{
					lstrCompany = lstrFileName.Left(lnPos);
				}
				else if(lnCount == 1)
				{
					lstrFactory = lstrFileName.Left(lnPos);
				}
				else if(lnCount == 2)
				{
					lstrPlant = lstrFileName.Left(lnPos);
				}
				else if(lnCount == 3)
				{
					lstrChann = lstrFileName.Left(lnPos);
				}
				else
				{
					break;
				}
				lnCount++;

				lstrFileName = lstrFileName.Right(lstrFileName.GetLength() - lnPos - 1);

				lnPos = lstrFileName.Find("-");
			}
		}


		CString lstrFilePath = it->second;

		if(lstrFilePath.GetLength() > 0)
		{

			ifstream lf;
			lf.open(lstrFilePath,ios_base::binary);
			vector<float> lvSourceWaveData;
			if(lf)
			{
				lf.seekg(0,ios_base::end);

				size_t lnFileSize = lf.tellg();
				vector<char> lvTemp(lnFileSize);
				vector<char> lvTemp1(lnFileSize);

				lf.seekg(0);
				lf.read((char*)&lvTemp.front(),lnFileSize);

				memcpy(&lvTemp1.front(),&lvTemp.front(),lnFileSize);

				char *pTmp=NULL;
				const char pcSep[]={"\t"};

				int lnDataCount = 0;
				pTmp=strtok((char*)&lvTemp.front(),pcSep);
				while (NULL!=pTmp)
				{
					pTmp=strtok(NULL,pcSep);
					lnDataCount++;
				}
				if(lnDataCount > 4096)
				{
					int lnInterval = lnDataCount/4096;
					lnDataCount = 0;
					pTmp=strtok((char*)&lvTemp1.front(),pcSep);
					while (NULL!=pTmp)
					{
						if(lnDataCount%lnInterval == 0)
						{
							lvSourceWaveData.push_back(atof(pTmp));
						}
						if(lvSourceWaveData.size() >= 4096)
						{
							break;
						}

						pTmp=strtok(NULL,pcSep);
						lnDataCount++;
					}

				}
				else
				{
					lvSourceWaveData.push_back(atof(pTmp));
				}
			}

			lf.close();


			pFixComm->SetItem( FIELD_SYSTEM_COMPANY,lstrCompany );
			pFixComm->SetItem( FIELD_SYSTEM_FACTORY,lstrFactory );
			pFixComm->SetItem( FIELD_SERVICE_PLANTID,lstrPlant );
			pFixComm->SetItem( FIELD_SYSTEM_CHANN,lstrChann );
			pFixComm->SetItem( FIELD_SERVICE_OFF_WAVE_NUM, (int)lvSourceWaveData.size() );
			pFixComm->SetItemBuf( FIELD_SERVICE_OFF_WAVE,(char*)&lvSourceWaveData.front(),(int)lvSourceWaveData.size()*sizeof(float));

			lnTotalSize = lnTotalSize + lvSourceWaveData.size()*sizeof(float);
			lnSendCount++;
			lnIndex++;

			if(lnSendCount >= lnWaveCount || lnTotalSize >= 5*1024*1024)
			{
				break;
			}

			pFixComm->CreateNextAnswer();
		}

	}

	pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnSendCount);
	pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,(int)lmTotalFile.size());
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return 1;

}

/**
根据设备测点设置轴心位置计算参数
*输入参数字段码
*          公司名          FIELD_SYSTEM_COMPANY
*          分厂名          FIELD_SYSTEM_FACTORY
*          设备名          FIELD_SERVICE_PLANTID
*          测点名          FIELD_SYSTEM_CHANN
*          测点类型        FIELD_SERVICE_CHANNTYPE
*          间隙电压        FIELD_SERVICE_TREND_GAP
*          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
*返回值的字段码：
*          状态代码        FIELD_SYSTEM_STATUS
*
#define    SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM         21096
*/

int CTransactData::SetOffAxesLocationParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	SwitchDbNameEx(pDB,g_strMainDbName);

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sHorChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_HOR);
	string sVerChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_VER);
	string sSuffix;

	CPlantInfoOffLine *pPlant=ReadCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);

	if ( NULL == pPlant)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	S_ChannInfo lHorChannInfo;
	S_ChannInfo lVerChannInfo;
	CAxisChannInfo lAxisChannInfo;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	lHorChannInfo._fSensitivity = pFixComm->GetDouble(FIELD_SERVICE_SENSITIVITY_HOR);

	lHorChannInfo._fGapVoltage = pFixComm->GetDouble(FIELD_SERVICE_CHAR_GAP_HOR);

	lVerChannInfo._fSensitivity = pFixComm->GetDouble(FIELD_SERVICE_SENSITIVITY_VER);

	lVerChannInfo._fGapVoltage = pFixComm->GetDouble(FIELD_SERVICE_CHAR_GAP_VER);

	lAxisChannInfo._iLocationType = pFixComm->GetLong(FIELD_SERVICE_AXIS_LOCATION_TYPE);

	lAxisChannInfo._iRotateDirection = pFixComm->GetLong(FIELD_SERVICE_ROTATE_DIRECTION);

	lAxisChannInfo._fAxisDia = pFixComm->GetDouble(FIELD_SERVICE_AXIS_DIA);

	int lnHorChannIndex = -1;

	int lnVerChannIndex = -1;

	int lnAxisChannIndex = -1;

	int lnResult = 0;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnHorChannIndex = pPlant->GetSpecVibChannIndex(sHorChannID);
		lnVerChannIndex = pPlant->GetSpecVibChannIndex(sVerChannID);
		lnAxisChannIndex = pPlant->GetSpecAxisLocationIndex(sHorChannID,sVerChannID);

		if (lnHorChannIndex < 0 || lnVerChannIndex < 0 || lnAxisChannIndex < 0)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		string lsPlantNo = pPlant->GetPlantNo();
		string lsHorChannNo = pPlant->GetSpecVibChannInfo(lnHorChannIndex)._cChannNo;
		string lsVerChannNo = pPlant->GetSpecVibChannInfo(lnVerChannIndex)._cChannNo;

		pDB->Cmd("UPDATE [%s] SET [%s] = %f,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s'",
			gc_cChannTable,gc_cSensitivity,lHorChannInfo._fSensitivity,gc_cGapVoltage,lHorChannInfo._fGapVoltage,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),gc_cChannNo,lsHorChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pDB->Cmd("UPDATE [%s] SET [%s] = %f,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s'",
			gc_cChannTable,gc_cSensitivity,lVerChannInfo._fSensitivity,gc_cGapVoltage,lVerChannInfo._fGapVoltage,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),gc_cChannNo,lsVerChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pDB->Cmd("UPDATE [%s] SET [%s] = %d,[%s] = %d,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s' AND [%s] = '%s'",
			gc_cPlantAxisTable,gc_cLocationType,lAxisChannInfo._iLocationType,
			gc_cRotate_Direction,lAxisChannInfo._iRotateDirection,gc_cAxis_Dia,lAxisChannInfo._fAxisDia,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),
			gc_cHorChann,lsHorChannNo.c_str(),gc_cVerChann,lsVerChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
		lnResult = pPlant->SetOffVibAxesLocationParam(lnHorChannIndex,lnVerChannIndex,lnAxisChannIndex,lHorChannInfo,lVerChannInfo,lAxisChannInfo);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnHorChannIndex = pPlant->GetSpecDynChannIndex(sHorChannID);
		lnVerChannIndex = pPlant->GetSpecDynChannIndex(sVerChannID);
		lnAxisChannIndex = pPlant->GetSpecDynAxisLocationIndex(sHorChannID,sVerChannID);

		if (lnHorChannIndex < 0 || lnVerChannIndex < 0 || lnAxisChannIndex < 0)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		string lsPlantNo = pPlant->GetPlantNo();
		string lsHorChannNo = pPlant->GetSpecDynChannInfo(lnHorChannIndex)._cChannNo;
		string lsVerChannNo = pPlant->GetSpecDynChannInfo(lnVerChannIndex)._cChannNo;

		pDB->Cmd("UPDATE [%s] SET [%s] = %f,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s'",
			gc_cChannTable,gc_cSensitivity,lHorChannInfo._fSensitivity,gc_cGapVoltage,lHorChannInfo._fGapVoltage,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),gc_cChannNo,lsHorChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pDB->Cmd("UPDATE [%s] SET [%s] = %f,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s'",
			gc_cChannTable,gc_cSensitivity,lVerChannInfo._fSensitivity,gc_cGapVoltage,lVerChannInfo._fGapVoltage,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),gc_cChannNo,lsVerChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		pDB->Cmd("UPDATE [%s] SET [%s] = %d,[%s] = %d,[%s] = %f WHERE [%s] = '%s' AND %s = '%s' AND %s = '%s' AND [%s] = '%s'",
			gc_cPlantAxisTable,gc_cLocationType,lAxisChannInfo._iLocationType,
			gc_cRotate_Direction,lAxisChannInfo._iRotateDirection,gc_cAxis_Dia,lAxisChannInfo._fAxisDia,
			gc_cCompany,sCompany.c_str(),gc_cPlantNo,lsPlantNo.c_str(),
			gc_cHorChann,lsHorChannNo.c_str(),gc_cVerChann,lsVerChannNo.c_str());
		if(!pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pPlant->SetOffDynAxesLocationParam(lnHorChannIndex,lnVerChannIndex,lnAxisChannIndex,lHorChannInfo,lVerChannInfo,lAxisChannInfo);


	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());
	return 1;
}
