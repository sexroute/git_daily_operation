#include "stdafx.h"
#include "transactdata.h"
#include "OnlineGlobal.h"
#include "zipimpl.h"
#include "SpeedUpDownDataSave.h"
#include <MiddleWare.h>
#include "additions/DebugHelper.h"
#include "Operations.h"

CTransactData::CTransactData(void)
{
}

CTransactData::~CTransactData(void)
{
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


/**得到指定设备指定测点类型的所有测点名和测点号，通过循环依次得到
	*输入参数：   公司名    FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SYSTEM_PLANT
	*             测点类型   FIELD_SYSTEM_CHANN_TYPE
	*返回值的字段码：
	*             设备号     FIELD_SERVICE_ON_SEND_PLANTID              
	*             测点名     FIELD_SYSTEM_CHANN
	*             测点号     FIELD_SERVICE_CHANN1
	*             测点位号   FIELD_SYSTEM_CHANN_ALIAS
	*             测点单位   FIELD_SERVICE_CHANN1_UNIT
	*             测点类型    FIELD_SYSTEM_CHANN_TYPE
	*             传感器类型 FIELD_SYSTEM_SENSOR_TYPE
	*             所属汽缸号 FIELD_SERVICE_CYLINDERNO
	*             信息类型   FIELD_SYSTEM_TYPE
	*             报警限类型        FIELD_SYSTEM_EVENT_TYPE
	*             报警上限          FIELD_SYSTEM_ALARM_LOW
	*             报警上上限        FIELD_SYSTEM_ALARM_HIGH
	*             报警下限          FIELD_SYSTEM_ALARM_LOW2
	*             报警下下限        FIELD_SYSTEM_ALARM_HIGH2
	*             自定义参数模板ID          FIELD_SYSTEM_TEMPID
	*             自定义报警参数模板ID      FIELD_SYSTEM_ALARM_TEMPID
	*             数据源                    FIELD_SYSTEM_DATASOURCE
	*             部位名称          FIELD_SERVICE_SEC_NAME
	*             部位索引          FIELD_SERVICE_SEC_IDX
	*             位置名称          FIELD_SERVICE_LOC_NAME
	*             位置索引          FIELD_SERVICE_LOC_IDX
	*             测点所属列表      FIELD_SERVICE_CHANN1_LIST
	*             转速类型          FIELD_SERVICE_REV
	*             是否存在键相      FIELD_SERVICE_KEY_IF
	*             传感器灵敏度      FIELD_SERVICE_SENSITIVITY
	*             传感器偏置量      FIELD_SERVICE_SENSITIVITY1
	*             间隙电压          FIELD_SERVICE_TREND_GAP
	*             位置号            FIELD_SERVICE_LOCATION_NO
	*             测点方向          FIELD_SERVICE_MEASURE_DIRECTION
	*             数据类型          FIELD_SERVICE_DATA_TYPE
	*             直流放大倍数      FIELD_SERVICE_DC_AMPLIFY
	*             交流放大倍数      FIELD_SERVICE_AC_AMPLIFY
  *define     SERVICE_SYSTEM_GETSPECTYPECHANN           25003
  */
int CTransactData::Service_SysGetSpecTypeChann(ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	_DECLARE_PERF_MEASURE_TIME();
	_BEGIN_PERF_MEASURE_TIME();

	int iType=pFixComm->GetLong(FIELD_SYSTEM_CHANN_TYPE);

	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany ,sFactory, sPlant);
	
	if (pPlantInfo==NULL)
	{	
		return -1;
	}
	E_TBL_CHANNTYPE lnTableType = (E_TBL_CHANNTYPE)4; //全部
	
	if (iType!=0)
	{
		CHANNTYPE lnType = (CHANNTYPE)iType;

		lnTableType = GetChannOneType(lnType);

	}else
	{
		lnTableType = (E_TBL_CHANNTYPE)4; //全部
	}
	
	int lnChannNum = 0;
	S_ChannInfo loInfo;
	int nCount=0;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());


	_END_PERF_MEASURE_TIME("25003:1");
	_BEGIN_PERF_MEASURE_TIME();


	//1.取得振动测点列表
	if(lnTableType == E_TBL_CHANNTYPE_VIB || 4 == lnTableType)
	{
		lnChannNum = pPlantInfo->GetVibChannNum();

		for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vVibChann.begin();
			itVib!=pPlantInfo->_vVibChann.end();++itVib)
		{
			CVibChannParam * lpVib = (CVibChannParam *)(*itVib);				

			loInfo = lpVib->GetChannInfo();

			if (WriteBackDataForService_SysGetSpecTypeChann(pFixComm,loInfo,lpVib))
			{
				nCount++;
			}

		}
	}

	//2.取得动态测点列表
	if(lnTableType == E_TBL_CHANNTYPE_DYN || 4 == lnTableType)
	{
		lnChannNum = pPlantInfo->GetDynChannNum();

		for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vDynChann.begin();
			itVib!=pPlantInfo->_vDynChann.end();++itVib)
		{
			CDynChannParam * lpVib = (CDynChannParam *)(*itVib);				

			loInfo = lpVib->GetChannInfo();

			if (WriteBackDataForService_SysGetSpecTypeChann(pFixComm,loInfo,lpVib))
			{
				nCount++;
			}

		}
	}

	//3.取得过程量测点列表
	if(lnTableType == E_TBL_CHANNTYPE_PROC || 4 == lnTableType)
	{
		lnChannNum = pPlantInfo->GetProcChannNum();
		for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vProcChann.begin();
			itVib!=pPlantInfo->_vProcChann.end();++itVib)
		{
			CProcChannParam * lpVib = (CProcChannParam *)(*itVib);				

			loInfo = lpVib->GetChannInfo();

			if (WriteBackDataForService_SysGetSpecTypeChann(pFixComm,loInfo,lpVib))
			{
				nCount++;
			}
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	_END_PERF_MEASURE_TIME("25003:4");
	_BEGIN_PERF_MEASURE_TIME();
	return nCount;
}


/**读取所有各种测点的特征值，通过循环依次取得各个测点的所有类型特征值。
*振动通道：使用振动类型的特征值结构体；FIELD_SERVICE_CHARVALUE_S
*过程量通道：使用特征值字段 FIELD_SERVICE_TREND_VALUE
*动态通道：使用动态类型的特征值结构体 FIELD_SERVICE_CHARVALUE_S
*每循环依次可得到一个测点指定趋势特征值
*输入参数值：机组ID          FIELD_SERVICE_PLANTID
*            公司ID          FIELD_SYSTEM_COMPANY
*			 分厂名          FIELD_SYSTEM_FACTORY
*            测点buff        FIELD_SERVICE_CHANN_BUFF
*            测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*            测点ID          FIELD_SERVICE_CHANN1
*            特征值          FIELD_SERVICE_TREND_VALUE
*        特征值结构体        FIELD_SERVICE_CHARVALUE_S
*            时间            FIELD_SERVICE_TIME
*             单位           FIELD_SERVICE_CHANN1_UNIT
*      指定机组测点的转速    FIELD_SERVICE_REV
*          测点类型          FIELD_SYSTEM_CHANN_TYPE
*          报警限类型        FIELD_SYSTEM_TYPE
*          报警上限          FIELD_SYSTEM_ALARM_LOW
*          报警上上限        FIELD_SYSTEM_ALARM_HIGH
*          报警下限          FIELD_SYSTEM_ALARM_LOW2
*          报警下下限        FIELD_SYSTEM_ALARM_HIGH2
#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002
*/
int CTransactData::GetAllTypeChar(ICommInterFace* pFixComm)
{
	string sCompany;
	string sFactory;
	string sPlant;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)ReadCheckCompanyInfo(sCompany ,sFactory, sPlant, pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	int index=0;
	S_VibCharValue sVibData;
	S_DynCharValue dynCharValue;
	S_ChannInfo channInfo;
	float fRev=.0,fValue=.1;
	int iChannNum   = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes      = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff= new char[ iBytes ];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	string sUnit    = "";
	lChann          = ParseChannList(pChannBuff,iChannNum);
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if ((pPlantInfo->GetSpecChannRadialAllChar((*itChann),sVibData,sUnit))!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sVibData,sizeof(S_VibCharValue));
			channInfo=pPlantInfo->GetSpecVibChannInfo((*itChann));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,channInfo._chAlarmParam._iRegJudgeType);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			fRev=(float)sVibData._iRev;
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iChannType);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
			continue;
		}
		if ((pPlantInfo->GetSpecChannProcData((*itChann),fValue,sUnit))!=-1)
		{
			timeCurr=pPlantInfo->GetRealProcDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE, fValue);
			channInfo=pPlantInfo->GetSpecProcChannInfo((*itChann));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,channInfo._chAlarmParam._iRegJudgeType);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iChannType);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
			continue;
		}
		if ((pPlantInfo->GetSpecDynChannCharValue((*itChann),dynCharValue))!=-1)
		{
			timeCurr=pPlantInfo->GetRealDynDataDateTime();
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&dynCharValue,sizeof(S_DynCharValue));
			channInfo=pPlantInfo->GetSpecDynChannInfo((*itChann));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,channInfo._chAlarmParam._iRegJudgeType);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			//CString sTemp;sTemp.Format("channInfo._cChannID:%s,fRev:%f",channInfo._cChannID,fRev);BHTRACE_DEBUG(sTemp);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iChannType);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
			continue;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	return index;
}

/**得到指定任意多个振动测点的频谱。（测点之间“，”分开）。如果多个测点，就循环提取
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                           FIELD_SERVICE_CHANN1
*          测点的频谱数据                   FIELD_SERVICE_FREQWAVE1
*          测点实际频谱一组数据的个数       FIELD_SERVICE_FREQWAVE_NUM
*          特征值结构体   FIELD_SERVICE_CHARVALUE_S
*          振动测点频谱DF                   FIELD_SERVICE_FREQWAVE_DF
*          指定机组测点的转速               FIELD_SERVICE_REV
*          时间                             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECTRUM                20003
*/
int CTransactData::GetSpectrum(ICommInterFace * pFixComm,CCustomDB *pDB)
{
	DATATYPE_WAVE *pfreqwave=NULL;
	float         *lpfFreq = NULL;
	float df=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	int    iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	iLines=iLines<199?201:iLines;
	pfreqwave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	vector<float> lofFreq(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	lpfFreq = &lofFreq.front();
	float fRev=.0;
	S_VibCharValue vibValue;
	string sUnit="";
//	S_ChannInfo channInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	float lfScaleCoef = 10.0;

	int lnPrecisionEnable = 0;

	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		timeCurr=pPlantInfo->GetRealVibDataDateTime();
		if ((pPlantInfo->GetSpecChannVibFreq((*itChann),pfreqwave,iLines,df,lpfFreq,lfScaleCoef,lnPrecisionEnable))!=-1)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)pfreqwave,(iLines)*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			//		writefile(pfreqwave,iLines,2);
			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iLines);
			pPlantInfo->GetSpecChannRadialAllChar((*itChann),vibValue,sUnit);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibValue,sizeof(S_VibCharValue));
			pPlantInfo->GetSpecChannRev((*itChann),fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);

			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnable);
			if(lnPrecisionEnable > 0)
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)lpfFreq,(iLines)*sizeof(float));
			}
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pfreqwave)
	KD_DeletePoint(pChannBuff)
	lChann.clear();
	return iLines;
}



/**读取监测站级实时报警统计
*输入参数值：公司ID,分厂ID,设备ID;          FIELD_SYSTEM_COMPANY2
*		     
*返回值的字段码：
*		     测点名           FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_CHANN1VALUE
*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC (-1: 未知0: 自己判断报警 1: 同步自下级)
#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT           20033
*/

extern INT GetSplitStrArray(const CString& strData,const CString& strSplitFlag,CStringArray &array,BOOL SequenceAsOne);

int CTransactData::OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	int nCount=0,iChannNum=0,iList=0,iChannDetailType=0,iCharType=GE_ALLCHAR;
	unsigned int iState = 0; 
	float pfRev[5];
	float fRev=.0,fData=.0;
	string sChann = "";
	string sUnit="";
	string sChannAlias="";
	S_ChannInfo channInfo;
	CString sCompany_factory_plants=pFixComm->Get(FIELD_SYSTEM_COMPANY2);
	CString lstrSplitAll = ";";
	CString lstrSplitDetail = ",";
	CStringArray loArrays;

	int lnSize = GetSplitStrArray(sCompany_factory_plants,lstrSplitAll,loArrays,TRUE);

	lnSize = loArrays.GetSize();
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	vector<CPlantInfoOnLine *> loPlantInfos;	
	for (int i=0;i<lnSize;i++)
	{
		CStringArray loPerArray;
		CString lstrPerData = loArrays.GetAt(i);
		GetSplitStrArray(lstrPerData,lstrSplitDetail,loPerArray,TRUE);

		int lnPerSize= loPerArray.GetSize();
		if(lnPerSize!=3)
		{
			ASSERT(FALSE);
		}else
		{
			CString lstrCompany = loPerArray.GetAt(0);
			CString lstrFactory = loPerArray.GetAt(1);
			CString lstrPlant = loPerArray.GetAt(2);

			string sCompany=lstrCompany.GetBuffer(0);
			string sFactory=lstrFactory.GetBuffer(0);
			string sPlant=lstrPlant.GetBuffer(0);

			CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

			if (NULL!= pPlantInfo)
			{
				loPlantInfos.push_back(pPlantInfo);
			}
		}
	}

	lnSize = loPlantInfos.size();
	for (int i=0;i<lnSize;i++)
	{
		CAlarmPlantInfo *pPlantInfo= (CAlarmPlantInfo *)loPlantInfos[i];
		nCount +=this->OnGetSpecPlantMonitorAlarmStat(pFixComm,pDB,*pPlantInfo,i,lnSize);
	}

	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	return nCount;

}

/**读取监测站级实时报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*返回值的字段码：
*		     测点名           FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_CHANN1VALUE
*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC (-1: 未知0: 自己判断报警 1: 同步自下级)
#define     SERVICE_SERVICE_ON_MONITORALARMSTAT           20026
*/
int CTransactData::OnGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	int nCount=0,iChannNum=0,iList=0,iChannDetailType=0,iCharType=GE_ALLCHAR;
	unsigned int iState = 0; 
	float pfRev[5];
	float fRev=.0,fData=.0;
	string sChann = "";
	string sUnit="";
	string sChannAlias="";
	S_ChannInfo channInfo;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SERVICE_PLANTID);
	int lnStartIndex = pFixComm->GetLong(FIELD_SERVICE_STARTINDEX);
	int lnStopPlantNum =  pFixComm->GetLong(FIELD_SERVICE_STOP_PLANTNUM);	
	int lnStopEndIndex = INT_MAX;
	if (lnStopPlantNum>0)
	{
		lnStopEndIndex = lnStartIndex+lnStopPlantNum;
	}


	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (NULL!= pPlantInfo)
	{

		nCount +=this->OnGetSpecPlantMonitorAlarmStat(pFixComm,pDB,*pPlantInfo,0,1);
	}

	else if(NULL==pPlantInfo && (sPlant.empty()||sCompany.empty()||sFactory.empty() ))
	{
		vector<CPlantInfoOnLine *> loPlantInfos;
		int lnSize = GetAllPlantInfo(sCompany,sFactory,sPlant,loPlantInfos,PLANTINFO_ALARM);
		ASSERT(lnSize == loPlantInfos.size());
		lnSize = loPlantInfos.size();
		for (int i=0;i<lnSize;i++)
		{
			if (lnStartIndex>i)
			{
				continue;
			}
			if (i>=lnStopEndIndex)
			{
				break;
			}
			CAlarmPlantInfo *pPlantInfo= (CAlarmPlantInfo *)loPlantInfos[i];
			nCount +=this->OnGetSpecPlantMonitorAlarmStat(pFixComm,pDB,*pPlantInfo,i,lnSize);
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	return nCount;
}

int  CTransactData::OnGetSpecPlantMonitorAlarmStat(ICommInterFace *pFixComm,
												   CCustomDB *pDB,
												   CAlarmPlantInfo & arefPlantInfo,
												   int anPlantIndex,
												   int anTotalPlants)
{
	CAlarmPlantInfo * pPlantInfo = & arefPlantInfo;

	string sCompany=pPlantInfo->GetPlantInfo()._cCompany;
	string sFactory=pPlantInfo->GetPlantInfo()._cFactory;
	string sPlant=pPlantInfo->GetPlantInfo()._cPlantID;

	int nCount=0,iChannNum=0,iList=0,iChannDetailType=0,iCharType=GE_ALLCHAR;
	unsigned int iState = 0;
	float pfRev[5]={0};
	float fRev=.0,fData=.0;
	string sChann = "";
	string sUnit="";
	string sChannAlias="";
	S_ChannInfo channInfo;

	pPlantInfo->GetRealRev(pfRev,5);

	long lnAlarmId   = -1;
	long lnAlarmSync = -1;

	GetAlarmEventID(pPlantInfo, lnAlarmId, lnAlarmSync);


	int loop1=0;
	iChannNum=pPlantInfo->GetVibChannNum();
	//CString strinfo; strinfo.Format("振动通道数：%d", iChannNum);AfxMessageBox(strinfo);
	for (loop1=0;loop1<iChannNum;++loop1)
	{
		int iAlarmLevel = 0;

		if (pPlantInfo->GetSpecChannAndTypeAlarmStateEx(loop1,
			sChann,
			sChannAlias,
			sUnit,
			iList,
			fData,
			iState,
			iChannDetailType,
			iAlarmLevel,
			GE_VIBCHANN))
		{

			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannAlias.c_str());

			pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);


			int lnAlarmSatus = ParseAlarmStatus(iState);

			if(lnAlarmSatus >0 && iAlarmLevel <= 0)
			{
				if(lnAlarmSatus == 1)
				{
					iAlarmLevel = ALARM_URGENT;
				}
				else if(lnAlarmSatus ==2)
				{
					iAlarmLevel = ALARM_DANGEOUS;
				}
				else
				{
					iAlarmLevel = ALARM_POTENTIAL;
				}
			}

			if(lnAlarmSatus > 2)
			{
				
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_SEFL_LEARNING);

				if(lnAlarmSatus == 3)
				{
					lnAlarmSatus = 1;
				}
				else if(lnAlarmSatus == 5)
				{
					lnAlarmSatus = 2;
				}
				else
				{
					lnAlarmSatus = 1;
				}
				

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_THRESHOLD);
			}

			pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,iAlarmLevel);

			pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmSatus);

			channInfo = pPlantInfo->GetSpecVibChannInfo(sChann);

			if (channInfo._iRevType>5 || channInfo._iRevType<1)
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[0]);  
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[channInfo._iRevType-1]);  
			}

			pFixComm->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);

			pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,channInfo._chAlarmParam._iRegJudgeType);//报警判断类型
			pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,channInfo._chAlarmParam._bRegular);     //是否开启报警判断

			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iDataType);
			pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,channInfo._iChannType);

			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,channInfo._iCylinderNo);
			pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO,channInfo._iLocationNo);

			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory.c_str());
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());

			pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,anTotalPlants-1);
			pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,anPlantIndex);

			pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealVibDataDateTime());
			this->SetSpecPlantAlarmStateEx(pFixComm,pPlantInfo,pDB);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++nCount;
			//CString sTemp;sTemp.Format("振动通道循环：%d,转速:%.2f,特征值：%.2f,解析报警状：%d",nCount,pfRev[0],fData,ParseAlarmStatus(iState));
			//CHZLogManage::Debug(sTemp,"CTransactData","OnGetMonitorAlarmStat");
			//strinfo.Format("振动通道循环：%d", nCount);AfxMessageBox(strinfo);
		}
	}
	iChannNum=pPlantInfo->GetProcChannNum();
	//strinfo.Format("过程量通道数：%d", iChannNum);AfxMessageBox(strinfo);
	for (loop1=0;loop1<iChannNum;++loop1)
	{
		if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannDetailType,GE_ALLPROC,iCharType))
		{
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannAlias.c_str());
			pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));

			int lnAlarmStatus = ParseAlarmStatus(iState);

			int iAlarmLevel = 0;
			if(lnAlarmStatus == 1)
			{
				iAlarmLevel = ALARM_URGENT;
			}
			else if(lnAlarmStatus == 2)
			{
				iAlarmLevel = ALARM_DANGEOUS;
			}
			else if(lnAlarmStatus >2)
			{
				iAlarmLevel = ALARM_POTENTIAL;
			}
			else
			{
				iAlarmLevel = 0;
			}
			if(lnAlarmStatus > 2)
			{
				if(lnAlarmStatus == 3)
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_SEFL_LEARNING);
				}
				else if(lnAlarmStatus == 4)
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_COMP);
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_TREND);
				}
				lnAlarmStatus = 1;

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_THRESHOLD);
			}

			pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,iAlarmLevel);

			channInfo = pPlantInfo->GetSpecProcChannInfo(sChann);

			if (channInfo._iRevType>5 || channInfo._iRevType<1)
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[0]);  
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[channInfo._iRevType-1]);  
			}

			pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,channInfo._chAlarmParam._iRegJudgeType);//报警判断类型
			pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,channInfo._chAlarmParam._bRegular);     //是否开启报警判断

			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iDataType);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,channInfo._iChannType);
			pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,channInfo._iCylinderNo);
			pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO,channInfo._iLocationNo);
			pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealProcDataDateTime());

			pFixComm->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);

			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory.c_str());
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());

			pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,anTotalPlants-1);
			pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,anPlantIndex);
			this->SetSpecPlantAlarmStateEx(pFixComm,pPlantInfo,pDB);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++nCount;
			//strinfo.Format("过程量通道循环：%d", nCount);AfxMessageBox(strinfo);
		}
	}

	iChannNum=pPlantInfo->GetDynChannNum();
	for (loop1=0;loop1<iChannNum;++loop1)
	{
		if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannDetailType,GE_DYNPRESSCHANN,iCharType))
		{
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannAlias.c_str());
			pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
			int lnAlarmSatus = ParseAlarmStatus(iState);

			int  lnAlarmLevel = 0;

			if(lnAlarmSatus == 1)
			{
				lnAlarmLevel = ALARM_URGENT;
			}
			else if(lnAlarmSatus == 2)
			{
				lnAlarmLevel = ALARM_DANGEOUS;
			}
			else if(lnAlarmSatus > 2)
			{
				lnAlarmLevel = ALARM_POTENTIAL;
			}

			if(lnAlarmSatus > 2)
			{
				if(lnAlarmSatus == 3)
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_SEFL_LEARNING);
				}
				else if(lnAlarmSatus == 4)
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_COMP);
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_TREND);
				}
				lnAlarmSatus = 1;

			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_THRESHOLD);
			}



			pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmSatus);

			pFixComm->SetItem(FIELD_SERVICE_ALARM_LEVEL,lnAlarmLevel);

			channInfo = pPlantInfo->GetSpecDynChannInfo(sChann);

			if (channInfo._iRevType>5 || channInfo._iRevType<1)
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[0]);  
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[channInfo._iRevType-1]);  
			}

			pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,channInfo._chAlarmParam._iRegJudgeType);//报警判断类型
			pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,channInfo._chAlarmParam._bRegular);     //是否开启报警判断

			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iDataType);
			pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,channInfo._iChannType);
			pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,channInfo._iCylinderNo);
			pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO,channInfo._iLocationNo);
			pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());

			pFixComm->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);

			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sFactory.c_str());
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());

			pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,anTotalPlants-1);
			pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,anPlantIndex);
            this->SetSpecPlantAlarmStateEx(pFixComm,pPlantInfo,pDB);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++nCount;
		}
	}

	if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{//目前往复设备报警统计才显示轴心位置报警
		iChannNum=pPlantInfo->GetAxisPairNum();
		for (loop1=0;loop1<iChannNum;++loop1)
		{
			if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannDetailType,GE_AXISLOCATIONCHANN))
			{
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
				pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannAlias.c_str());
				pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
				int lnAlarmSatus = ParseAlarmStatus(iState);

				if(lnAlarmSatus > 2)
				{
					if(lnAlarmSatus == 3)
					{
						pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_SEFL_LEARNING);
					}
					else if(lnAlarmSatus == 4)
					{
						pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_COMP);
					}
					else
					{
						pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_MULTIDEVICE_TREND);
					}
					lnAlarmSatus = 1;

				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,(int)E_ALARM_TYPE_THRESHOLD);
				}

				pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmSatus);
				channInfo = pPlantInfo->GetSpecDynChannInfo(sChann);

				if (channInfo._iRevType>5 || channInfo._iRevType<1)
				{
					pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[0]);  
				}
				else
				{
					pFixComm->SetItem(FIELD_SERVICE_REV,pfRev[channInfo._iRevType-1]);  
				}

				pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,channInfo._chAlarmParam._iRegJudgeType);//报警判断类型
				pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,channInfo._chAlarmParam._bRegular);     //是否开启报警判断

				pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
				pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
				pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmLH);
				pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmLL);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
				pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iDataType);
				pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,channInfo._iCylinderNo);
				pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO,channInfo._iLocationIdx);
				pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());
				pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,channInfo._iChannType);
				pFixComm->SetItem(FIELD_SERVICE_CHANN2,channInfo._cChannNo);

				pFixComm->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
				pFixComm->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);
				this->SetSpecPlantAlarmState(pFixComm,pPlantInfo,pDB);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++nCount;
			}
		}
	}
	return nCount;
}

/**得到所有振动测点的五种类型的特征值，通过循环依次取得各个测点的所有类型特征值，
*每循环依次可得到一个测点的五种特征值
  *输入参数值：
  *                 公司ID   FIELD_SYSTEM_COMPANY
  *			        分厂名   FIELD_SYSTEM_FACTORY
  *                 机组ID   FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *                 测点ID         FIELD_SERVICE_CHANN1
  *                 特征值结构体   FIELD_SERVICE_CHARVALUE_S
  *                 报警值         FIELD_SYSTEM_ALARM_LOW
  *                 危险值         FIELD_SYSTEM_ALARM_HIGH
  *                 单位           FIELD_SERVICE_CHANN1_UNIT
  *          指定机组测点的转速    FIELD_SERVICE_REV
  *            时间           FIELD_SERVICE_TIME
*#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000
*/
// 得到指定机组所有振动测点所有特征值
int CTransactData::GetAllFiveChar(ICommInterFace * pFixComm)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	int ichannnum=0;
	int index,nCount=-1;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	S_VibCharValue sVibData;
    S_ChannInfo channInfo;
	float fRev=0;
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	ichannnum=pPlantInfo->GetRadialVibChannNum();
	pFixComm->CreateAnswerMul( pFixComm->GetFunc());
	for(index=0;index<ichannnum;index++)
	{
		if ((pPlantInfo->GetSpecChannRadialAllChar(index,sVibData,sUnit))!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sVibData,sizeof(S_VibCharValue));
            pFixComm->SetItem(FIELD_SERVICE_CHANN1,pPlantInfo->GetSpecIndexRadialVibChannID(index).c_str());
			channInfo=pPlantInfo->GetSpecVibChannInfo(index);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
            pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			nCount++;
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	return nCount;
}
/**读取指定振动测点的五种类型的特征值。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点buff       FIELD_SERVICE_CHANN_BUFF
*          测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数       FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          测点ID         FIELD_SERVICE_CHANN1
*          测点位号       FIELD_SYSTEM_CHANN_ALIAS
*          报警值         FIELD_SYSTEM_ALARM_LOW
*          危险值         FIELD_SYSTEM_ALARM_HIGH
*          单位           FIELD_SERVICE_CHANN1_UNIT
*          指定测点转速   FIELD_SERVICE_REV
*          时间           FIELD_SERVICE_TIME
*          测点类型       FIELD_SYSTEM_CHANN_TYPE
#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR        20001
*/
int CTransactData::GetSpecFiveChar(ICommInterFace * pFixComm)
{
	string sCompany;
	string sFactory;
	string sPlant;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)ReadCheckCompanyInfo(sCompany ,sFactory, sPlant, pFixComm);
	if (pPlantInfo==NULL)
		return -1;
	int index=0;
	S_VibCharValue sVibData;
	S_ChannInfo channInfo;
	float fRev=.0;
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
//	CString strTmp;strTmp.Format("%s,%d",pChannBuff,iBytes);AfxMessageBox(strTmp);
	list<string> lChann;
	list<string>::iterator itChann;
	string sUnit="";
	lChann=ParseChannList(pChannBuff,iChannNum);
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
	//	strTmp.Format("%s",itChann->c_str());AfxMessageBox(strTmp);
		if ((pPlantInfo->GetSpecChannRadialAllChar((*itChann),sVibData,sUnit))!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sVibData,sizeof(S_VibCharValue));
			channInfo=pPlantInfo->GetSpecVibChannInfo((*itChann));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);
		//	strTmp.Format("%s",channInfo._cChannID);AfxMessageBox(strTmp);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			//HZLOG_DEBUG(STR("Rev=%f", fRev));
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,channInfo._iChannType);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	return index;
}


/**得到指定任意多个振动测点的指定类型特征值
  *输入参数：
  *          公司ID         FIELD_SYSTEM_COMPANY
  *			 分厂名         FIELD_SYSTEM_FACTORY
  *          机组ID         FIELD_SERVICE_PLANTID
  *          特征值类型     FIELD_SERVICE_TREND_CHARTYPE
  *          测点buff     FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数     FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          测点特征值         FIELD_SERVICE_TREND_VALUE
  *          测点ID             FIELD_SERVICE_CHANN1
  *          时间               FIELD_SERVICE_TIME
  *          单位               FIELD_SERVICE_CHANN1_UNIT
  #define     SERVICE_SERVICE_ON_GETFOURSPECCHAR        20004
  */
int CTransactData::GetFourSpecChar(ICommInterFace * pFixComm)
{
   	float fData=0;
	int index=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
    char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	string sUnit="";
	lChann=ParseChannList(pChannBuff,iChannNum);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if(pPlantInfo->GetSpecChannRadialData((*itChann),fData,sUnit,GE_ALLCHAR)!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE,fData);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			index++;
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	KD_DeletePoint(pChannBuff);
	lChann.clear();
	return index;
}

/**得到指定机组的指定过程量测点类型的所有过程量测点的值。注意：如果测点类型是振动测点，则不返回任何值。
*输入参数：机组ID         FIELD_SERVICE_PLANTID
*          公司ID         FIELD_SYSTEM_COMPANY
*		   分厂名         FIELD_SYSTEM_FACTORY
*          测点类型       FIELD_SERVICE_CHANNTYPE
*返回值的字段码：
*          过程量值       FIELD_SERVICE_TREND_PROCESS
*          测点ID         FIELD_SERVICE_CHANN1
*          时间           FIELD_SERVICE_TIME
*          单位           FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETALLPROCVALUE        20005
*/
int CTransactData::GetAllProcValue(ICommInterFace * pFixComm)
{
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	//振动测点类型，直接错误返回
	if(iCharType==GE_VIBCHANN){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	//轴位移测点
	if (iCharType==GE_AXIALCHANN)
		return GetAllAxisDisplace(pFixComm);
	int index=0;
    int ichannnum=0;
	float fData;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	ichannnum=pPlantInfo->GetProcChannNum();
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//指定过程量类型的过程量测点
	for(index=0;index<ichannnum;index++)
	{
		if (pPlantInfo->GetSpecChannProcData(index,fData,sUnit,iCharType)!=-1)
		{
			timeCurr=pPlantInfo->GetRealProcDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_PROCESS,fData);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,pPlantInfo->GetSpecIndexProcChannID(index).c_str());
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	return index;
}
/**得到指定过程量测点的值。
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          过程量测点值     FIELD_SERVICE_TREND_PROCESS
*          测点ID           FIELD_SERVICE_CHANN1
*            时间           FIELD_SERVICE_TIME
*          单位             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETSPECPROCVALUE       20006
*/
int CTransactData::GetSpecProcValue(ICommInterFace * pFixComm)
{
	int index=-1;
    float fData=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	string sUnit="";
	lChann=ParseChannList(pChannBuff,iChannNum);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if ((pPlantInfo->GetSpecChannProcData((*itChann),fData,sUnit))!=-1)
		{
			index++;
			timeCurr=pPlantInfo->GetRealProcDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_PROCESS,fData);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	return index;
}

/**得到指定两个振动测点的实时波形和频谱。（如何振动测点2不设置参数，则只提取一组）
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                                FIELD_SERVICE_CHANN1
*          测点位号                              FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据                        FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据                        FIELD_SERVICE_FREQWAVE1
*          测点实时波形上一组数据的个数          FIELD_SERVICE_TIMEWAVE_NUM
*          测点实际频谱一组数据的个数            FIELD_SERVICE_FREQWAVE_NUM
*          振动测点频谱DF                        FIELD_SERVICE_FREQWAVE_DF
*          所有特征值                            FIELD_SERVICE_CHARVALUE_S
*          报警值                                FIELD_SYSTEM_ALARM_LOW
*          危险值                                FIELD_SYSTEM_ALARM_HIGH
*          单位                                  FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速                    FIELD_SERVICE_REV
*          时间                                  FIELD_SERVICE_TIME
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
*          测点起始索引                          FIELD_SERVICE_STARTINDEX
*          测点结束索引                          FIELD_SERVICE_ENDINDEX
*          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
*          自定义参数数据BUFFSIZE                FIELD_SERVICE_CHANN_BUFFSIZE
#define     SERVICE_SERVICE_ON_GETWAVE                20007
*/
int CTransactData::GetTwoWave(ICommInterFace * pFixComm)
{
	int iRtn = -1;
	DATATYPE_WAVE *pfreqwave=NULL;
	DATATYPE_WAVE *fwave=NULL;
	float * lpfWave = NULL;
	float * lpfFreq = NULL;
	float df=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	lChann=ParseChannList(pChannBuff,iChannNum);
	itChann=lChann.begin();
	iChannNum=lChann.size();
	int    iPtNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	int    iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);

	//if (iLines < 199)
	//{
	//	iLines = 201;
	//}
	//else
	//iLines=iLines<199?201:iLines;
	iPtNum=iPtNum<512?513:iPtNum;
	
	pfreqwave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	fwave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

	lpfFreq = new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
	lpfWave = new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
	float fRev=.0;
    S_VibCharValue vibValue;
	S_ChannInfo channInfo;
	string sUnit="";
	float pfCustomData[GD_CUSTOMPARAM_BUFFSIZE] = {.0};
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	_DECLARE_PERF_MEASURE_TIME();

	if ((iLines  != 0) && (iLines %2 != 0))
	{
		iLines -= 1;
	}
	
	int lnSkipFreq = pFixComm->GetLong(FIELD_SERVICE_ZOOM_STOPFREQ);

	BOOL lbSkipFreq = FALSE;

	if (lnSkipFreq>0)
	{
		lbSkipFreq = TRUE;
	}

	float lfScaleCoef =10.0;
	int lnPrecisionEnabled = 0;

	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		//CString sTemp;sTemp.Format("%d,%s, %s, %d, %d",lChann.size(),itChann->c_str(),pChannBuff,iBytes,iChannNum);
		//CHZLogManage::Debug(sTemp,"CTransactData","GetTwoWave");
		_BEGIN_PERF_MEASURE_TIME();
		int lnRealWaveCount = iPtNum;
		if ((pPlantInfo->GetSpecChannVibWaveAndFreq((*itChann),fwave,lnRealWaveCount,pfreqwave,iLines,df,lfScaleCoef,lnPrecisionEnabled,lpfWave,lpfFreq,lbSkipFreq))!=-1)
		{
			++iRtn;
			

			_END_PERF_MEASURE_TIME("GetSpecChannVibWaveAndFreq");
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
		//	strTmp.Format("%d,%d,%.2f %s",iLines,iPtNum,df,itChann->c_str());AfxMessageBox(strTmp);
			pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)fwave,lnRealWaveCount*sizeof(DATATYPE_WAVE));
			pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)pfreqwave,iLines*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);

			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
			if(lnPrecisionEnabled > 0)
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)lpfWave,lnRealWaveCount*sizeof(float));
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)lpfFreq,iLines*sizeof(float));
			}
	//		writefile(fwave,iPtNum,1);
			//		writefile(pfreqwave,iLines,2);
			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);

			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iLines);
			pPlantInfo->GetSpecChannRadialAllChar((*itChann),vibValue,sUnit);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibValue,sizeof(S_VibCharValue));
			channInfo=pPlantInfo->GetSpecVibChannInfo(*itChann);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev((*itChann),fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo._chAlarmParam._pThLearnParamF[8]._fParam4);

			//CString sTemp;sTemp.Format("ALARM_LOW:%f, ALARM_HIGH:%f",channInfo._chAlarmParam._pThLearnParamS[9]._fParam2,channInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
			//CHZLogManage::Debug(sTemp,"CTransactData","GetTwoWave");
			pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,channInfo._chAlarmParam._pThLearnParamS[9]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,channInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
			int iStartIndex,iEndIndex;
			pPlantInfo->GetSpecVibChannStartEndIndex(channInfo._cChannID,iStartIndex,iEndIndex);

			if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
			{
				if (iStartIndex<=0||iEndIndex <= 0)
				{
					iStartIndex = lnRealWaveCount/2;
					iEndIndex = lnRealWaveCount;
				}

				if (lnRealWaveCount<2*iEndIndex)
				{
					pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,lnRealWaveCount);
				}else
				{
					pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,2*iEndIndex);
				}
			}
			else
			{
				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,lnRealWaveCount);
			}
			
			pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,iStartIndex);
			pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,iEndIndex);
			//CString sTemp;sTemp.Format("iStartIndex:%d, iEndIndex:%d",iStartIndex,iEndIndex);BHTRACE_DEBUG(sTemp);
			//CHZLogManage::Debug(sTemp,"CTransactData","GetTwoWave");
			
			pPlantInfo->GetSpecRadialVibChannCustomData(channInfo._cChannID, pfCustomData,0, GD_CUSTOMPARAM_BUFFSIZE);
			
			//CString sTemp;sTemp.Format("Vib20007:pfCustomData[0]:%f, pfCustomData[1]:%f,pfCustomData[2]:%f,pfCustomData[3]:%f",pfCustomData[0], pfCustomData[1],pfCustomData[2],pfCustomData[3]);BHTRACE_DEBUG(sTemp);
			//CString stemp;stemp.Format("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",channInfo._fCustomParam[0],channInfo._fCustomParam[1],channInfo._fCustomParam[2],channInfo._fCustomParam[3],channInfo._fCustomParam[4],channInfo._fCustomParam[5],channInfo._fCustomParam[6],channInfo._fCustomParam[7],channInfo._fCustomParam[8],channInfo._fCustomParam[9]);BHTRACE_DEBUG(stemp);
			int iAllBuffSize=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
			pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)(pfCustomData),iAllBuffSize);

			//CString lsTitle;
			//lsTitle.Format("C_%s", timeCurr.Format("%Y%m%d%H%M%S"));
			//writefile(pfCustomData, GD_CUSTOMPARAM_BUFFSIZE, lsTitle);
			
			pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			
		}
		
	}
	
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(fwave);
	KD_DeletePoint(pfreqwave);
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(lpfFreq);
	KD_DeletePoint(lpfWave);
	lChann.clear();
	return iRtn;
}


/**得到某一组轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          轴心轨迹的垂直方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
*          垂直方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          轴心轨迹的水平方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          水平方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          水平报警值                       FIELD_SYSTEM_ALARM_LOW
*          水平危险值                       FIELD_SYSTEM_ALARM_HIGH
*          垂直报警值                       FIELD_SYSTEM_ALARM_LOW2
*          垂直危险值                       FIELD_SYSTEM_ALARM_HIGH2
*          水平方向测点位号                 FIELD_SYSTEM_CHANN_ALIAS
*          垂直方向测点位号                 FIELD_SYSTEM_CHANN2_ALIAS
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*          时间                             FIELD_SERVICE_TIME
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                       FIELD_SYSTEM_STATUS//1:为反进动,2为正进动

#define     SERVICE_SERVICE_ON_GETAXESTRACK           20008
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
int CTransactData::GetAxesTrack(ICommInterFace* pFixComm)
{
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	float         *pfVerWave = NULL;
	float         *pfHorWave = NULL;
	double *pWaveVerData=NULL;
	double *pWaveHorData=NULL;
	BYTE  *pBytes=NULL;
	long iInputBytes=0;
	int index=-1;
	int iSmpNum=1,iSmpFreq=1;
	float fRev=0;
    CZipImpl zipImpl;
	long iZipBytes=-1;
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
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime(1999,1,1,1,1,1);

	string sLocation   = pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);

	S_ChannInfo channInfoHor;
	S_ChannInfo channInfoVer;
	string sChannNoVer;
	string sChannNoHor;

	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	//CString sTemp;sTemp.Format("iWaveNum:%d,sLocation:%s,sChannIDHor:%s,sChannIDVer:%s",iWaveNum,sLocation.c_str(),sChannIDHor.c_str(),sChannIDVer.c_str());BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetAxesTrack");
	if (sChannIDVer.empty()||sChannIDHor.empty())
	{
		if(sLocation.empty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		CAxisChannInfo axisChannInfo=pPlantInfo->GetSpecLocationAxisChannInfo(sLocation);
		sChannNoVer = axisChannInfo._cVerChann;
		sChannNoHor = axisChannInfo._cHorChann;
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if((sChannIDHor!="")&&(sChannIDVer!=""))
	{
		std::vector<DATATYPE_WAVE> loHorWave,loVerWave;
		std::vector<float> lofHorWave,lofVerWave;
		std::vector<double> loWaveHorData,loWaveVerData;
		std::vector<BYTE> loBytes;

		channInfoVer = pPlantInfo->GetSpecVibChannInfo( sChannIDHor );
		channInfoHor = pPlantInfo->GetSpecVibChannInfo( sChannIDVer );
		sChannNoVer  = channInfoVer._cChannNo;
		sChannNoHor  = channInfoHor._cChannNo;

		timeCurr=pPlantInfo->GetRealVibDataDateTime();
		S_ChannInfo channInfo;
		S_VibCharValue vibValue;
		channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
		pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
		iSmpFreq=vibValue._iSmpFreq;
        iSmpNum=vibValue._iSmpNum;
		iWaveNum=iWaveNum>iSmpNum?iSmpNum:iWaveNum;
		iWaveNum = GetNearestPoint(iWaveNum);
		iWaveNum=iWaveNum<100?513:iWaveNum;;//防止波形点数太小时，引起的错误
		if (iWaveNum<0)
		{
			return -1;
		}
		//iWaveNum = iWaveNum-iWaveNum%2;
		iWaveNum = GetNearestPoint(iWaveNum);

		//CString sTemp;sTemp.Format("iSmpNum:%d,iSmpFreq:%d,iWaveNum:%d",iSmpNum,iSmpFreq,iWaveNum);BHTRACE_DEBUG(sTemp);
		//CHZLogManage::Debug(sTemp,"CTransactData","GetAxesTrack");
		fRev=(float)vibValue._iRev;
		loHorWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		loVerWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		lofHorWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		lofVerWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pVerWave = &loVerWave.front();
		pHorWave = &loHorWave.front();
		pfVerWave = &lofVerWave.front();
		pfHorWave = &lofHorWave.front();
		//pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		//pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		iInputBytes=iWaveNum*sizeof(DATATYPE_WAVE);
		loBytes.resize(iInputBytes);
		pBytes = &loBytes.front();

		float lfScaleCoef = 10.0;
		int   lnPrecisionEnabled = 0;

		float lfMaxValue = 0;
		float lfMinValue = 0;
        // pBytes=new BYTE[iInputBytes];

		if ((pPlantInfo->GetSpecChannVibWave(sChannIDHor,pHorWave,iWaveNum,pfHorWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1)&&(pPlantInfo->GetSpecChannVibWave(sChannIDVer,pVerWave,iWaveNum,pfVerWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1))
		{

			iInputBytes=iWaveNum*sizeof(DATATYPE_WAVE);
			
			/*
			 * 压缩时可能崩溃，直接返回数据
			 */
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerWave,iInputBytes);
			pFixComm->SetItem(FIELD_SERVICE_ZIPWAVE_BYTESNUM, iZipBytes);

			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorWave,iInputBytes);
		    pFixComm->SetItem(FIELD_SERVICE_ZIPWAVE_BYTESNUM1, iZipBytes);
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

			if(lnPrecisionEnabled > 0)
			{
				pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR_FLOAT,(char*)pfHorWave,iSmpNum*sizeof(float));
				pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER_FLOAT,(char*)pfVerWave,iSmpNum*sizeof(float));
			}

			pFixComm->SetItem(FIELD_SERVICE_AXESTRACK_WAVENUM,iWaveNum);

			pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
            pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
			S_ChannInfo channInfo;
			S_VibCharValue vibValue;
			channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);
			//垂直方向数据
			channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDVer);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,channInfo._chAlarmParam._fAlarmHH);
			pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibValue,sizeof(S_VibCharValue));
			pFixComm->SetItem(FIELD_SYSTEM_CHANN2_ALIAS,channInfo._cChannAlias);

			//转子旋向
			int iRotation =-1;
			iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);

			//进动方向
			if(iWaveNum>1)
			{
				int iPression = 0;
				int iOrbitShape=0;
				loWaveHorData.resize(iWaveNum);
				loWaveVerData.resize(iWaveNum);
				pWaveVerData = &loWaveVerData.front();
				pWaveHorData = &loWaveHorData.front();
				//pWaveVerData=new double[iWaveNum];
				//pWaveHorData=new double[iWaveNum];
				pPlantInfo->ConvertFloatToDouble(pWaveVerData,pfVerWave,iWaveNum);
				pPlantInfo->ConvertFloatToDouble(pWaveHorData,pfHorWave,iWaveNum);
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
				{	/*CHZLogManage::Error("计算轴心进动方向出错","S_OnlineSev","CTransactData::GetAxesTrack");*/}
				if (iPression==0)//如果计算为不确定则为正进动
				{	iPression=2;}
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,iPression);
				//CString sTemp;sTemp.Format("iRotation:%d,iPression:%d,iOrbitShape:%d",iRotation,iPression,iOrbitShape);BHTRACE_DEBUG(sTemp);
			}

			//水平方向数据
			

			index++;

		}
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");

		index= -1;
	}

	pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
	pFixComm->Write(pFixComm->GetAci());

	return index;
}

/**得到指定机组指定一组测点轴心位置值。
*输入参数：
*            公司ID               FIELD_SYSTEM_COMPANY
*			 分厂名               FIELD_SYSTEM_FACTORY
*            机组ID               FIELD_SERVICE_PLANTID
  *          轴承位置描述     FIELD_SYSTEM_AXISLOCATION
  *          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
  *          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*返回值的字段码：
*            X方向振动测点位置  FIELD_SERVICE_AXESLOCATION_HOR
*            Y方向振动测点位置  FIELD_SERVICE_AXESLOCATION_VER
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009
*/
int CTransactData::GetAxisLocation(ICommInterFace* pFixComm)
{
	float fX=0,fY=0;
	int index=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	string sLocation=pFixComm->GetItem(FIELD_SYSTEM_AXISLOCATION);
	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
    if(sLocation!="")
	{
	   if (pPlantInfo->GetSpecChannAxisLocationData(sLocation,fX,fY))
	   {
		   timeCurr=pPlantInfo->GetRealVibDataDateTime();
           pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,fX);
		   pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,fY);
		   pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
		   index=0;
	   }
	}
	else if(!((sChannIDVer.empty())||(sChannIDHor.empty())))
	{
		if (pPlantInfo->GetSpecChannAxisLocationData(sChannIDHor,sChannIDVer,fX,fY))
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_HOR,fX);
			pFixComm->SetItem(FIELD_SERVICE_AXESLOCATION_VER,fY);
			//CString sTmp;sTmp.Format("fX:%f,fY:%f",fX,fY);BHTRACE_DEBUG(sTmp);
			//CHZLogManage::Debug( sTmp, "CTransactData","GetAxisLocation");
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			index=0;
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		index= -1;
	}
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**得到指定机组指定轴位移测点值
*输入参数值：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID         FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
*          间隙电压       FIELD_SERVICE_TREND_GAP
*          直流量         FIELD_SERVICE_TREND_DC
*          测点ID         FIELD_SERVICE_CHANN1
*          测点位号       FIELD_SYSTEM_CHANN_ALIAS
*          报警值         FIELD_SYSTEM_ALARM_LOW
*          危险值         FIELD_SYSTEM_ALARM_HIGH
*          单位           FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速    FIELD_SERVICE_REV
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECAXISDISPLACE    20011
*/
int CTransactData::GetSpecAxisDisplaceValue(ICommInterFace* pFixComm)
{
	float fData=0;
	int index=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	lChann=ParseChannList(pChannBuff,iChannNum);
    S_ChannInfo channInfo;
	string sUnit="";
	float fRev=.0;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		if(pPlantInfo->GetSpecChannAxisData((*itChann),fData,sUnit,GE_AXIALDISPLACE)!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_AXISDISPLACE,fData);
			pPlantInfo->GetSpecChannAxisData((*itChann),fData,sUnit,GE_DCCHAR);
			pFixComm->SetItem(FIELD_SERVICE_TREND_DC,fData);
			pPlantInfo->GetSpecChannAxisData((*itChann),fData,sUnit,GE_GAP);
			pFixComm->SetItem(FIELD_SERVICE_TREND_GAP,fData);
			channInfo=pPlantInfo->GetSpecVibChannInfo(*itChann);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(channInfo._cChannID,fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			index++;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	return index;
}

/**得到指定机组所有轴位移测点值，通过循环依次得到所有轴位移值
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			   分厂名   FIELD_SYSTEM_FACTORY
*            机组ID   FIELD_SERVICE_PLANTID
*返回值的字段码：
*            轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
*            测点ID         FIELD_SERVICE_CHANN1
*          报警值         FIELD_SYSTEM_ALARM_LOW
*          危险值         FIELD_SYSTEM_ALARM_HIGH
*          单位           FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速    FIELD_SERVICE_REV
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012
*/
int CTransactData::GetAllAxisDisplace(ICommInterFace* pFixComm)
{
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int ichannnum=pPlantInfo->GetAxialVibChannNum();
	int nCount=-1;
	float fData=.0;
	float fRev=.0;
	S_ChannInfo channInfo;
	string sChann,sUnit="";
    pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for(int loop=0;loop<ichannnum;++loop)
	{
		if (pPlantInfo->GetSpecChannAxisData(loop,fData,sUnit,GE_AXIALDISPLACE)!=-1)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pFixComm->SetItem(FIELD_SERVICE_TREND_AXISDISPLACE, fData);
			sChann=pPlantInfo->GetSpecIndexAxisVibChannID(loop);
			channInfo=pPlantInfo->GetSpecVibChannInfo(sChann);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,channInfo._cChannID);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			pPlantInfo->GetSpecChannRev(sChann,fRev);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->Write  ( pFixComm->GetAci() );
			pFixComm->CreateNextAnswer();
			++nCount;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	return nCount;
}

/**得到用户所观测机组范围内所有机组的测点运行状态，通过循环依次得到所有报警的测点,如果不需要通道信息则把FIELD_SERVICE_IFCHANN设为-1
*输入参数值：
*            用户ID            FIELD_SYSTEM_USEID
*            是否需要通道       FIELD_SERVICE_IFCHANN
*返回值字段码：
*            公司ID             FIELD_SYSTEM_COMPANY
*			 分厂名             FIELD_SYSTEM_FACTORY
*            报警的机组ID       FIELD_SERVICE_PLANTID
*            报警的测点ID       FIELD_SERVICE_CHANN1
*            报警值             FIELD_SERVICE_CHANN1VALUE
#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
*/
int CTransactData::GetAllChannelState(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sUserID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
    bool bChann=true;
	if(pFixComm->GetLong(FIELD_SERVICE_IFCHANN)==-1)
		bChann=false;
	string sFactory="",sPlant;
	string sCompany,sGroup;
	int index=0,nCount=-1,iNum=0,iloop=0;
	int iLevel=-1;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pDB->Cmd("SELECT * FROM [%s] ",gc_cUserTable);
	pDB->Cmd(" WHERE %s='%s'",gc_cUserName,sUserID.c_str());
	if(pDB->More())
	{
       iLevel=pDB->GetLong(gc_cUserLevel);
	   sFactory=pDB->Get(gc_cFactory_Name);
	   sCompany=pDB->Get(gc_cCompany);
	   sGroup=pDB->Get(gc_cGroup);
 	}
	else
		goto QUIT_CLEAN;
	if ((iLevel==GE_LOCAL_OPERATOR)||(iLevel==GE_LOCAL_ENGINEER)||(iLevel==GE_LOCAL_ADMIN))
	{
		if ((sGroup.empty())||(sCompany.empty())||(sFactory.empty()))
			goto QUIT_CLEAN;
		nCount=GetSpecFactoryChannAlarmState(sGroup,sCompany,sFactory,pFixComm,bChann);
	}
	else if ((iLevel==GE_GROUP_ADMIN)||(iLevel==GE_GROUP_EXPERT)||(iLevel==GE_GROUP_OPERATOR)||(iLevel==GE_GROUP_ENGINEER))
	{
		if(sGroup.empty())
			goto QUIT_CLEAN;
        nCount=GetSpecGroupChannAlarmState(sGroup,pFixComm,pDB,bChann);
	}
	else
	{
		if ((sGroup.empty())||(sCompany.empty()))
			goto QUIT_CLEAN;
		nCount=GetSpecCompanyChannAlarmState(sGroup,sCompany,pFixComm,bChann);
	}
QUIT_CLEAN:
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


// 得到指定设备的所有测点特征值，振动测点需要指定测点特征类型
/**得到指定机组所有测点特征值，通过循环依次得到所有值。注：对于振动测点来说，指定那种特征类型就给那一种
  *输入参数值：
  *            公司ID         FIELD_SYSTEM_COMPANY
  *			   分厂名         FIELD_SYSTEM_FACTORY
  *            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
  *            设备号buff     FIELD_SERVICE_CHANN_BUFF
  *            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
  *            设备号个数     FIELD_SERVICE_CHANN_NUM
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
  *            时间           FIELD_SERVICE_TIME
  #define     SERVICE_SERVICE_ON_GETALLCHANNVALUE    20014
 */
int CTransactData::GetAllChannValue(ICommInterFace * pFixComm, CCustomDB * pDB)
{
	string sPlant[4];
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	int iPlantNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pPlantBuff=new char[iBytes];
	ZeroMemory(pPlantBuff,iBytes);
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pPlantBuff,iBytes);
	list<string> lPlant;
	list<string>::iterator itPlant;
	lPlant=ParseChannList(pPlantBuff,iPlantNum);
	iPlantNum=lPlant.size();
    CAlarmPlantInfo *pPlantInfo=NULL;
	CRealPlantInfo *pPlant=NULL;
	string sChann="";
	string sChannAlias;
	float fData;
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
		pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,(*itPlant),PLANTINFO_ALARM);
		pPlant=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,(*itPlant),PLANTINFO_REAL);
		if (pPlantInfo!=NULL && pPlant!=NULL)
		{
   			int loop1=0;
			iChannNum=pPlantInfo->GetRadialVibChannNum();
			S_VibCharValue vibCharValue;
			string sUnit;
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,
															  sChann,
															  sChannAlias,
															  sUnit,
															  iList,
															  fData,
															  iState,
															  iChannType,
															  GE_VIBCHANN,
															  iCharType))
				{
					timeCurr=pPlantInfo->GetRealVibDataDateTime();
					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
				
					int lnAlarmStatus = ParseAlarmStatus(iState);
					if(lnAlarmStatus > 2)
					{
						if(lnAlarmStatus == 5)
						{
							lnAlarmStatus = 2;
						}
						else
						{
							lnAlarmStatus = 1;
						}
						
					}
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,lnAlarmStatus);

					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
					float pfRev[5]={.0,.0,.0,.0,.0};
					pPlantInfo->GetRealRev( pfRev, 5);
					pFixComm->SetItem(FIELD_SERVICE_REV, pfRev[0]);
					pFixComm->SetItem(FIELD_SERVICE_REV2,pfRev[1]);
					pFixComm->SetItem(FIELD_SERVICE_REV3, pfRev[2]);
					pFixComm->SetItem(FIELD_SERVICE_REV4, pfRev[3]);
					pFixComm->SetItem(FIELD_SERVICE_REV5, pfRev[4]);
					
					if(pPlant->GetSpecChannRadialAllChar(sChann,vibCharValue,sUnit) > -1)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibCharValue,sizeof(S_VibCharValue));
					}

					pFixComm->SetItem(FIELD_SERVICE_DISK_FULL,g_bIsDiskFull);

					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}
			iChannNum=pPlantInfo->GetAxialVibChannNum();
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannType,GE_AXIALCHANN,GE_AXIALDISPLACE))
				{
					timeCurr=pPlantInfo->GetRealVibDataDateTime();
					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
					pFixComm->SetItem(FIELD_SERVICE_DISK_FULL,g_bIsDiskFull);

					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}

			iChannNum=pPlantInfo->GetDynChannNum();
			S_DynCharValue dynCharValue;
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannType,GE_DYNPRESSCHANN))
				{
					timeCurr=pPlantInfo->GetRealDynDataDateTime();
					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
					pFixComm->SetItem(FIELD_SERVICE_DISK_FULL,g_bIsDiskFull);

					if(pPlant->GetSpecDynChannCharValue(sChann,dynCharValue) > -1)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&dynCharValue,sizeof(S_DynCharValue));
					}
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}

			iChannNum=pPlantInfo->GetProcChannNum();
			for (loop1=0;loop1<iChannNum;++loop1)
			{
				if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,sChannAlias,sUnit,iList,fData,iState,iChannType,GE_ALLPROC))
				{

					timeCurr=pPlantInfo->GetRealProcDataDateTime(sChann);
					pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,ParseAlarmStatus(iState));
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
					pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,iList);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
					pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
					pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
					pFixComm->SetItem(FIELD_SERVICE_DISK_FULL,g_bIsDiskFull);

					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					++nCount;
				}
			}
            //得到整机效率
			//float fEfficiency = .0;
			//{
			//	pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
			//	pFixComm->SetItem(FIELD_SERVICE_CHANN1,"整机效率");
			//	pFixComm->SetItem(FIELD_SERVICE_CHANN1_STATUS,0);
			//	pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,"");
			//	pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST,-100);
			//	pFixComm->SetItem(FIELD_SERVICE_CHANN1VALUE,fEfficiency);
			//	pFixComm->SetItem(FIELD_SERVICE_CHANNTYPE,iChannType);
			//	pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			//	pFixComm->Write(pFixComm->GetAci());
			//	pFixComm->CreateNextAnswer();
			//}
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	KD_DeletePoint(pPlantBuff);
 	return nCount;
}


/**通过分厂名得到下面设备的报警状态,并且得到指定机组的转速
  *输入参数值：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			   分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID   FIELD_SERVICE_PLANTID
  *返回值字段码：
  *            报警的机组ID         FIELD_SERVICE_PLANTID
  *            指定设备的转速       FIELD_SERVICE_REV
  *            指定机组的转速2      FIELD_SERVICE_OFF_REV2
  #define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017
  */
int CTransactData::GetSubCorpState(ICommInterFace * pFixComm, CCustomDB * pDB)
{
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	int index=0,nCount=0,iNum=0,iloop=0;
	float pfRev[2];
	bool bFlagStop=false;
	string sGroup;
 //   CString strtmp; strtmp.Format("%s,%s",strPlantID.c_str(),strSubCorp.c_str());   AfxMessageBox(strtmp);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//传递转速
	CAlarmPlantInfo *pPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);
	if (pPlantInfo!=NULL)
	{
		sGroup=pPlantInfo->GetGroupName();
		pPlantInfo->GetRealRev(pfRev,2);
		pFixComm->SetItem(FIELD_SERVICE_REV, pfRev[0]);
		pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,pfRev[1]);
	}
	if (sCompany=="")
		nCount=GetAllPlantChannAlarmState(pFixComm,false);
	else
	{
		if (sFactory=="")
			nCount=GetSpecCompanyChannAlarmState(sGroup,sCompany,pFixComm,false);
	    else
			nCount=GetSpecFactoryChannAlarmState(sGroup,sCompany,sFactory,pFixComm,false);
	}
	if(nCount==0)
	{//说明没有设备报警
	   //添加下面两句，是为传递转速
    	pFixComm->Write( pFixComm->GetAci() );
	    pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}
/**得到指定指定机组和指定可以做启停车的测点的波特图数据，一次只能得到一组数据，如果没有指定测点，则发送所有测点的数据
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
  *返回值的字段码：
  *          测点号           FIELD_SERVICE_CHANN1
  *          相位：           FIELD_SERVICE_TREND_ONECHAR
  *          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
  *          转速：           FIELD_SERVICE_REV
  *          时间             FIELD_SERVICE_TIME
  *          对应时间的微秒   FIELD_SERVICE_MICROSECOND
  *          开停车状态       FIELD_SERVICE_SUD_STATUS
  *          开停车类型       FIELD_SERVICE_SUD_TYPE
  *          启停车事件ID     FIELD_SYSTEM_EVENT_ID
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
#define     SERVICE_SERVICE_ON_GETBODE                20018
*/

int CTransactData::GetBode(ICommInterFace * pFixComm, CCustomDB * pDB)
{
	if (G_TEMP_ONLINE)
	{
        return CTransactData::GetBodeForTemp(pFixComm,pDB);
	}
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_SUD);
	if(pSudPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
    float fRev=0;
	float fPhase=0,fAmp=0;
	int iSmpNum=0,iSmpFreq=0;
	int index=-1;
	int iMicroSecond=0;
	E_SUD_TYPE iType;
	int iStatus=0;
	int iEventID=-1;
	S_VibCharValue vibCharValue;
	string sUnit = "";
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
//	CString strTmp;strTmp.Format("%d",iChannNum);AfxMessageBox(strTmp);
	string sChann;
	static volatile ULONGLONG lnMaxDateTime = 0;
	if (iChannNum==0)
	{
		iChannNum=pSudPlantInfo->GetRadialVibChannNum();
		for (int loop=0;loop<iChannNum;++loop)
		{
			if (pSudPlantInfo->GetSpecChannIndex(loop,sChann,fRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType)!=-1)
			{
				iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
				
				pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
				pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

				SYSTEMTIME loSystemTime = {0};

				int lnIMicroSecond = 0;

				pSudPlantInfo->GetCurrDataTime(lnIMicroSecond).GetAsSystemTime(loSystemTime);
				
				if(iStatus>0)
				{
					FILETIME loFileTime = {0};

					SystemTimeToFileTime(&loSystemTime,&loFileTime);			

					ULARGE_INTEGER lnTemp;

					lnTemp.HighPart = loFileTime.dwHighDateTime;

					lnTemp.LowPart = loFileTime.dwLowDateTime;

					if (lnTemp.QuadPart >= lnMaxDateTime)
					{
						lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
					}else
					{
						iStatus = 0;
					}
				}

				if (iStatus>0)
				{
					CBHDateTime ldtTmp = pSudPlantInfo->GetCurrDataTime(iMicroSecond);
					ldtTmp.SetMillisecond(iMicroSecond);
					pSudPlantInfo->GetSpecChannRadialAllChar( sChann, vibCharValue, sUnit);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
					pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
					pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
					pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
					pFixComm->SetItem(FIELD_SERVICE_TIME,ldtTmp);
					pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibCharValue,sizeof(S_VibCharValue));
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++index;
			}
		}
	}
	else
	{
		for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		{//AfxMessageBox("ds");
			if (pSudPlantInfo->GetSpecChannIndex((*itChann),fRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType)!=-1)
		//	if (1)
			{//AfxMessageBox("ds2");
				iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
				pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
				pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

				SYSTEMTIME loSystemTime = {0};
				
				int lnIMicroSecond = 0;

				pSudPlantInfo->GetCurrDataTime(lnIMicroSecond).GetAsSystemTime(loSystemTime);

				if (iStatus >0)
				{
					FILETIME loFileTime = {0};

					SystemTimeToFileTime(&loSystemTime,&loFileTime);			

					ULARGE_INTEGER lnTemp;

					lnTemp.HighPart = loFileTime.dwHighDateTime;

					lnTemp.LowPart = loFileTime.dwLowDateTime;

					if (lnTemp.QuadPart >= lnMaxDateTime)
					{
						lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
					}else
					{
						iStatus = 0;
					}
				}
				if (iStatus>0)
				{
					CBHDateTime ldtTmp = pSudPlantInfo->GetCurrDataTime(iMicroSecond);
					ldtTmp.SetMillisecond(iMicroSecond);
					pSudPlantInfo->GetSpecChannRadialAllChar((*itChann), vibCharValue, sUnit);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
					pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
					pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
					pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
					pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
					pFixComm->SetItem(FIELD_SERVICE_TIME,ldtTmp);
					pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibCharValue,sizeof(S_VibCharValue));
					//CString strTemp;
					//strTemp.Format("%.2f,1X:%.2f",fRev,vibCharValue._fCharAll);
					//CHZLogManage::Debug(strTemp,"CTransactData","GetBode");
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++index;
			}
		}
	}
    pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pChannBuff);
	lChann.clear();
	return index;
}
/**得到指定指定机组和指定可以做启停车的测点瀑布图，一次只能得到一组波形频谱
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点号           FIELD_SERVICE_CHANN1
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*          相位：           FIELD_SERVICE_TREND_ONECHAR
*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
*          转速：           FIELD_SERVICE_REV
*          频谱数据         FIELD_SERVICE_FREQWAVE1
*          频谱数据个数     FIELD_SERVICE_FREQWAVE_NUM
*          频谱DF           FIELD_SERVICE_FREQWAVE_DF
*          时间             FIELD_SERVICE_TIME
*          对应时间的微秒   FIELD_SERVICE_MICROSECOND
*          开停车状态       FIELD_SERVICE_SUD_STATUS
*          开停车类型       FIELD_SERVICE_SUD_TYPE
*          启停车事件ID     FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_GETWATERFALL            20019
*/
int CTransactData::GetWaterFall(ICommInterFace * pFixComm, CCustomDB * pDB)
{
	if (G_TEMP_ONLINE)
	{
		return CTransactData::GetWaterFallForTemp(pFixComm,pDB);
	}
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	iLines=iLines<199?401:iLines;

	CSudPlantInfo *pSudPlantInfo=(CSudPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_SUD);
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if(pSudPlantInfo==NULL)
    {
        pFixComm->Write(pFixComm->GetAci());
        return -1;
    }

	float iRev=0;
	int iSmpNum=0,iSmpFreq=0;
	int iRealNum=(iLines-1)*2.56;
	float fAmp=.0,fPhase=.0;
	DATATYPE_WAVE *pfFreqWave=NULL,*pfWave=NULL;
	float *lpfWave = NULL,*lpfFreq = NULL;
	float df=.0;
	int index=pSudPlantInfo->GetSpecChannIndex(sChann,iRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType);
//	int index=0;
	E_SUD_TYPE iType;
	int iStatus=0;
	int iEventID=-1;
	int iMicroSecond=0;
	//CString strTmp;strTmp.Format("%d,%d,%f,%f,%f,%d",iEventID,iLines,iRev,fAmp,fPhase,pSudPlantInfo->GetCurrDataTime(iMicroSecond).GetYear());AfxMessageBox(strTmp);

	if (index==-1)
		goto GETWATERFALL_CLEAN;
	iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
	pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
	pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

	static volatile ULONGLONG lnMaxDateTime = 0;

	SYSTEMTIME loSystemTime = {0};

	int lnIMicroSecond = 0;

	pSudPlantInfo->GetCurrDataTime(lnIMicroSecond).GetAsSystemTime(loSystemTime);
	
	/*
	if(iStatus>0)
		{
			FILETIME loFileTime = {0};
	
			SystemTimeToFileTime(&loSystemTime,&loFileTime);			
	
			ULARGE_INTEGER lnTemp;
	
			lnTemp.HighPart = loFileTime.dwHighDateTime;
	
			lnTemp.LowPart = loFileTime.dwLowDateTime;
	
			if (lnTemp.QuadPart >= lnMaxDateTime)
			{
				lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
			}else
			{
				iStatus = 0;
			}
		}*/
	
	if (iStatus>0)
	{
		float lfScaleCoef = 10.0;
		int lnPrecisionEnabled = 0; 
		iRealNum=iSmpNum>iRealNum?iRealNum:iSmpNum;
		if (iRealNum<256)
			goto GETWATERFALL_CLEAN;
		else
		{
			if (iRealNum/2 <=0)
			{
				return -1;
			}
			pfFreqWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pfWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			lpfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			lpfFreq=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];

			if (pSudPlantInfo->GetSpecChannVibWaveAndFreq(sChann,pfWave,iRealNum,pfFreqWave,iLines,df,lfScaleCoef,lnPrecisionEnabled,lpfWave,lpfFreq)==-1)
				goto GETWATERFALL_CLEAN;
		}
		pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
		pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
		pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iLines);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);
		pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)pfFreqWave,(iLines)*sizeof(DATATYPE_WAVE));
	    pFixComm->SetItem(FIELD_SERVICE_TIME,pSudPlantInfo->GetCurrDataTime(iMicroSecond));
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)lpfWave,iLines*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)lpfFreq,iLines*sizeof(float));
		}

		
//		CString strTmp;
//		strTmp.Format("%d,%d,%f,%f,%f,%d",iEventID,iLines,iRev,fAmp,fPhase,pSudPlantInfo->GetCurrDataTime(iMicroSecond).GetYear());AfxMessageBox(strTmp);
	}
GETWATERFALL_CLEAN:
	pFixComm->Write(pFixComm->GetAci());
	if (pfFreqWave!=NULL)
		delete [] pfFreqWave;
	if (pfWave!=NULL)
	    delete [] pfWave;
	KD_DeletePoint(lpfWave);
	KD_DeletePoint(lpfFreq);
	return index;
}

/**得到指定机组振动测点的细化频谱分析和波形。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点             FIELD_SERVICE_CHANN1
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
*          波形数据个数     FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
*          状态             FIELD_SYSTEM_STATUS
*          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020
*/
int CTransactData::GetZoomSpec(ICommInterFace * pFixComm,CCustomDB *pDB)
{
	int nErrorCode=-1;
	string strInfo="";
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	long iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	float fStartFreq=pFixComm->GetDouble(FIELD_SERVICE_ZOOM_STARTFREQ);
	int iStopFreq=pFixComm->GetLong(FIELD_SERVICE_ZOOM_STOPFREQ);
	int iPtNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	DATATYPE_WAVE *pfZoomSpm=NULL;
	DATATYPE_WAVE *pfWave=NULL;
	float         *lpfWave=NULL;
	float         *lpfFreq=NULL;
	float df;
	vector<float> lofWave;
	vector<float> lofFreq;

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_REAL);
	iLines=iLines>0?iLines:201;
	iPtNum=iPtNum>50?iPtNum:513;

	pFixComm->CreateAnswer(pFixComm->GetFunc());
    if ((pPlantInfo==NULL)||sChann.empty())
	{
		nErrorCode=-100;
		goto CLEAN_GETZOOMSPEC;
	}
	pfZoomSpm=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pfWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

	lofWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	lofFreq.resize(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	lpfWave = &lofWave.front();
	lpfFreq = &lofFreq.front();

	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;

	nErrorCode=pPlantInfo->GetSpecChannZoomSpec(sChann,pfZoomSpm,iLines,fStartFreq,iStopFreq,df,pfWave,iPtNum,lfScaleCoef,lnPrecisionEnabled,lpfWave,lpfFreq);
	if(nErrorCode==0)
	{
		pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)pfWave,iPtNum*sizeof(DATATYPE_WAVE));
		pFixComm->SetItemBuf(FIELD_SERVICE_ZOOMSPECTRUM,(char *)pfZoomSpm,iLines*sizeof(DATATYPE_WAVE));
		pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iPtNum);
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,iLines);
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_STARTFREQ,fStartFreq);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)lpfWave,iPtNum*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)lpfFreq,iLines*sizeof(float));
		}
	}
CLEAN_GETZOOMSPEC:
	if(pfZoomSpm!=NULL) delete [] pfZoomSpm;
	if(pfWave!=NULL) delete [] pfWave;
	switch(nErrorCode)
	{
	case 0:
         strInfo="";
		 break;
	case -1:
         strInfo="细化分辨率大于未细化的分辨率";
		 break;
	 case -2:
         strInfo="细化倍数超过范围（即超过20倍）";
		 break;
	 case -3:
         strInfo="起始频率大于终止频率或者设置参数有误";
		 break;
	 case -4:
	 case -5:
         strInfo="分析出错";
		 break;
	 case -100:
		 strInfo="设备或测点信息有误";
		 break;
	 default:
		 strInfo="未知错误";
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,nErrorCode);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strInfo.c_str());
	pFixComm->Write(pFixComm->GetAci());
	return nErrorCode;
}
/**得到指定指定机组和测点的相关信息
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*			 分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点号           FIELD_SERVICE_CHANN1
*返回值的字段码：
*          额定转速：       FIELD_SERVICE_REV
*          实时转速：       FIELD_SERVICE_REV2
*          采样频率         FIELD_SERVICE_SAMPLE_FREQ
*          采样频率         FIELD_SERVICE_SAMPLE_NUM
#define     SERVICE_SERVICE_ON_GETSPECCHANNINFO           20021
*/
int CTransactData::GetSpecChannInfo(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iSmpFreq=0,iSmpNum=0;
	float fRatingRev=0,fRealRev=0;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_REAL);
	if ((pPlantInfo==NULL)||(sChann.empty()))
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pPlantInfo->GetRealSmpInfo(iSmpNum,iSmpFreq);
	fRatingRev=pPlantInfo->GetSpecChannRev(sChann,fRealRev);
	pFixComm->SetItem(FIELD_SERVICE_REV,fRatingRev);
	pFixComm->SetItem(FIELD_SERVICE_REV2,fRealRev);
	pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
	pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM,iSmpNum);
	pFixComm->Write(pFixComm->GetAci());
	return iSmpNum;
}

/**读取指定设备振动测点的指定倍频的特征值和相位，
*如果指定测点了，就只取指定测点的特征值和相位
*如果没有指定测点，则获得所有测点通过循环依次取得各个测点的对应倍频特征值和相位，每循环依次可得到一个测点指定趋势特征值和相位
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID      FIELD_SERVICE_PLANTID
*            测点ID      FIELD_SERVICE_CHANN1
*            特征值类型  FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*            测点ID      FIELD_SERVICE_CHANN1
*            指定特征值  FIELD_SERVICE_TREND_VALUE
*            对应的相位  FIELD_SERVICE_TREND_PHASE
*            转速：           FIELD_SERVICE_REV
#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022
*/
int CTransactData::GetAllSpecCharAndPhase(ICommInterFace* pFixComm,CCustomDB* pDB)
{
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_REAL);
	if(pPlantInfo==NULL)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	float fRev=0;
	float fPhase=0,fAmp=0;
	int iSmpNum=0,iSmpFreq=0;
	int index=-1;
	string sUnit="";
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (sChann.empty())
	{
		int iChannNum=pPlantInfo->GetRadialVibChannNum();
		for (int loop=0;loop<iChannNum;++loop)
		{
			if (pPlantInfo->GetSpecChannRadialData(loop,fAmp,fPhase,fRev,sUnit,iCharType)!=-1)
			{
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
				pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
				pFixComm->SetItem(FIELD_SERVICE_TREND_PHASE,fPhase);
				pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE,fAmp);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++index;
			}
		}
	}
	else
	{
		if (pPlantInfo->GetSpecChannRadialData(sChann,fAmp,fPhase,fRev,sUnit,iCharType)!=-1)
		{
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TREND_PHASE,fPhase);
			pFixComm->SetItem(FIELD_SERVICE_TREND_VALUE,fAmp);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sUnit.c_str());
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++index;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return index;
}

/**得到指定公司和分厂所有机组的转速。如果指定机组只有一个转速，则只取FIELD_SERVICE_REV的值
*输入参数：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            设备号buff     FIELD_SERVICE_CHANN_BUFF
*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            设备号个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          机组ID          FIELD_SERVICE_PLANTID
*          指定机组的转速  FIELD_SERVICE_REV
*          指定机组的转速2 FIELD_SERVICE_OFF_REV2
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETREV                 20010*/
int CTransactData::GetPlantRev(ICommInterFace *pFixComm)
{

	int nCount=0;
	float pfRev[5];
	string sPlant[4];
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int iPlantNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pPlantBuff=new char[iBytes];
	ZeroMemory(pPlantBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pPlantBuff,iBytes);
	list<string> lPlant;
	list<string>::iterator itPlant;
	lPlant=ParseChannList(pPlantBuff,iPlantNum);
	iPlantNum=lPlant.size();
	CRealPlantInfo *pPlantInfo=NULL;
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (itPlant=lPlant.begin();itPlant!=lPlant.end();++itPlant)
	{
		pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,(*itPlant),PLANTINFO_REAL);
		if (pPlantInfo!=NULL)
		{
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			pPlantInfo->GetRealRev( pfRev, 5);
			pFixComm->SetItem(FIELD_SERVICE_PLANTID,itPlant->c_str());
			pFixComm->SetItem(FIELD_SERVICE_REV, pfRev[0]);
			pFixComm->SetItem(FIELD_SERVICE_OFF_REV2,pfRev[1]);
            pFixComm->SetItem(FIELD_SERVICE_REV3, pfRev[2]);
			pFixComm->SetItem(FIELD_SERVICE_REV4, pfRev[3]);
			pFixComm->SetItem(FIELD_SERVICE_REV5, pfRev[4]);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			nCount++;
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci() );
	KD_DeletePoint(pPlantBuff);
	lPlant.clear();
	return nCount;
}


/**得到指定任意个动态压力测点示功图信息。循环得到各个测点的数据
*输入参数值：
*            公司ID         FIELD_SYSTEM_COMPANY
*			 分厂名         FIELD_SYSTEM_FACTORY
*            机组ID         FIELD_SERVICE_PLANTID
*            测点buff       FIELD_SERVICE_CHANN_BUFF
*            测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数       FIELD_SERVICE_CHANN_NUM
*            示功图类型     FIELD_SYSTEM_TYPE       P-V,P-a,P-t
*返回值的字段码：
*          测点号           FIELD_SERVICE_CHANN1
*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
*          波形数据         FIELD_SERVICE_TIMEWAVE1
*          X坐标数据        FIELD_SERVICE_TIMEWAVE2
*          所有特征值       FIELD_SERVICE_CHARVALUE_S
*          采样频率         FIELD_SERVICE_SAMPLE_FREQ
*          采样点数         FIELD_SERVICE_SAMPLE_NUM
*          报警值           FIELD_SYSTEM_ALARM_LOW
*          危险值           FIELD_SYSTEM_ALARM_HIGH
*          单位             FIELD_SERVICE_CHANN1_UNIT
*          指定测点的转速   FIELD_SERVICE_REV
*          时间             FIELD_SERVICE_TIME
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_ON_GETPVDATA               20015
*/
int CTransactData::GetPVData(ICommInterFace *pFixComm,CCustomDB *pDB)
{
//	CHZLogManage::Info("dsf0","CTransactData::GetPVData","CTransactData::GetPVData");
	string sCompany,sFactory,sPlant;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo*)ReadCheckCompanyInfo(sCompany,sFactory,sPlant,pFixComm,true,PLANTINFO_REAL);
	if(NULL==pPlantInfo)
		return -1;
	int iPvType=pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator it;
    //解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	int iSmpNum=1,iSmpFreq=1;
	string sChann;
	S_DynCharValue sDynCharValue;
	S_ChannInfo sChannInfo;
	float fRev = .1;
	if( iChannNum <1 )
		//CHZLogManage::Error("没有通道选择", "Online", "CTransactData获取示功图实时数据");
		iChannNum=0;
	int iStartIndex = 0, iEndIndex = 0;
	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;

	float lfMaxValue = 0;
	float lfMinValue = 0;

	DATATYPE_WAVE *pWave=NULL;
	DATATYPE_WAVE *pXCoor= NULL;
	float         *pfWave = NULL;

	pWave= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pXCoor= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

	vector<float> loWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	pfWave = &loWave.front();

	float pfCustomData[GD_CUSTOMPARAM_BUFFSIZE] = {.0};

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (it=lChann.begin();it!=lChann.end();++it)
	{
     	sChann = *it;
		if(pPlantInfo->GetSpecDynChannCharValue(sChann,sDynCharValue) > -1)
		{
			++iChannNum;
			sChannInfo=pPlantInfo->GetSpecDynChannInfo(sChann);
			int iCylinderNo=sChannInfo._iCylinderNo;//得到对应测点气缸的编号
			if (pPlantInfo->GetSpecDynChannStartEndIndex(sChann,iStartIndex,iEndIndex)<0)
		         CHZLogManage::Error("没有得到指定测点的周期起始和结束索引","Online","CTransactData::GetPVData");
			//CString strInfo;strInfo.Format("得到的周期起始和结束索引%d,%d 汽缸号%d",iStartIndex, iEndIndex,iCylinderNo);
			//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
			iSmpNum=sDynCharValue._iSmpNum;
			iSmpFreq=sDynCharValue._iSmpFreq;
			//strInfo.Format("iSmpNum:%d,iSmpFreq:%d",iSmpNum, iSmpFreq);BHTRACE_DEBUG(strInfo);
			//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
			
			if (iSmpNum<0)
			{
				KD_DeletePoint(pChannBuff);
				KD_DeletePoint(pXCoor);
				KD_DeletePoint(pWave);
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				return -1;
			}
			
			if(pPlantInfo->GetSpecDynChannWave(sChann,pWave,iSmpNum,pfWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)<0)
				CHZLogManage::Error("没有读到示功图数据","Online","CTransactData::GetPVData");
			//writefile(pWave, iSmpNum, 111);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannInfo._cChannAlias);
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
			fRev=(float)sDynCharValue._iRev;
			iStartIndex=0;
			int iPeriodSmpNums = iEndIndex-iStartIndex;
			if (iPvType == GE_PVTYPE_Pt)
			{///假如画p-t图，或者周期点数大于采样点数，则不按周期点数画，而按照全部实际的波形点数画
				pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)pWave,iSmpNum*sizeof(DATATYPE_WAVE));

				if(lnPrecisionEnabled > 0)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iSmpNum*sizeof(float));
				}

				for (int nloop=0; nloop<iSmpNum;nloop++)
					pXCoor[nloop] = (float)nloop;
			}
			else
			{
				if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iSmpNum))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)pWave,iSmpNum*sizeof(DATATYPE_WAVE));
					iSmpNum = pPlantInfo->GetPVXCoorData(iSmpFreq, iSmpNum, iPvType, fRev,iCylinderNo, pXCoor);
					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iSmpNum*sizeof(float));
					}
				}
				else
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pWave[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
					iSmpNum = pPlantInfo->GetPVXCoorData( iSmpFreq, iEndIndex - iStartIndex, iPvType,iCylinderNo,pXCoor);
					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,(iEndIndex-iStartIndex)*sizeof(float));
					}
				}
			}
			//writefile(pXCoor,iSmpNum,111);
			//strInfo.Format("iSmpNum:%d",iSmpNum);BHTRACE_DEBUG(strInfo);
			//CHZLogManage::Debug( strInfo, "CTransactData","GetPVData");
			int iAllBuffSize=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
			pPlantInfo->GetSpecDynChannCustomData(sChannInfo._cChannID, pfCustomData, 0, GD_CUSTOMPARAM_BUFFSIZE);

			pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pfCustomData,iAllBuffSize);
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
			pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pXCoor, iSmpNum * sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_SAMPLE_NUM, iSmpNum);
			pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ, iSmpFreq);
			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sDynCharValue,sizeof(S_DynCharValue));
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,sChannInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,sChannInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sChannInfo._cUnit);
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam4);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			//writefile(pXCoor,iSmpNum,222);
			//    CString strInfo;strInfo.Format("%.2f,%.2f,%.2f",sDynCharValue.GetSpecTypeData(0),sDynCharValue.GetSpecTypeData(1),sDynCharValue.GetSpecTypeData(2))	;
			//	CHZLogManage::Info( strInfo, "Online", "CTransactData::GetPVData");
			
		}
	}
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pXCoor);
	KD_DeletePoint(pWave);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iChannNum;

}

/**得到指定任意多个沉降量测点的实时波形、频谱和所有特征值。（测点之间“，”分开）。如果多个测点，就循环提取
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                  FIELD_SERVICE_CHANN1
*          测点别名                FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据          FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据          FIELD_SERVICE_FREQWAVE1
*          测点波形数据的点数      FIELD_SERVICE_TIMEWAVE_NUM
*          测点频谱数据的点数      FIELD_SERVICE_FREQWAVE_NUM
*          振动测点频谱DF          FIELD_SERVICE_FREQWAVE_DF
*          所有特征值              FIELD_SERVICE_CHARVALUE_S
*          报警值                  FIELD_SYSTEM_ALARM_LOW
*          危险值                  FIELD_SYSTEM_ALARM_HIGH
*          单位                    FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速      FIELD_SERVICE_REV
*          时间                    FIELD_SERVICE_TIME
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
*          测点起始索引                          FIELD_SERVICE_STARTINDEX
*          测点结束索引                          FIELD_SERVICE_ENDINDEX
*          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
*          自定义参数数据BUFFSIZE                FIELD_SERVICE_CHANN_BUFFSIZE
#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
*/
int CTransactData::GetRodSinkData(ICommInterFace* pFixComm,CCustomDB *pDB)
{
	string sCompany,sFactory,sPlant,sChann="";
	int iPtNums=0,iLines=0,iChannNum=0,iBytes=0;

    CRealPlantInfo *pPlantInfo=(CRealPlantInfo*)ReadCheckCompanyInfo(sCompany,sFactory,sPlant,pFixComm,true,PLANTINFO_REAL);
	if(NULL==pPlantInfo)
		return -1;
	iPtNums=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	//CString sTemp;sTemp.Format("%d",iPtNums);
	//CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
	iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	iLines    = iLines > 200 ? iLines:401;
	if (iLines >3200 )
		iLines = 401;
	iChannNum = pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	iBytes    = pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator it;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	DATATYPE_WAVE *pWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	DATATYPE_WAVE *pWaveNotDC=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	DATATYPE_WAVE *pFreq=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	float *lpfWave = NULL;
	float *lpfFreq = NULL;
	float *lpfWaveNotDC=NULL;

	vector<float> lofWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lofFreq(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> lofWaveNotDC(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	lpfWave = &lofWave.front();
	lpfFreq = &lofFreq.front();
	lpfWaveNotDC = & lofWaveNotDC.front();

	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;



	S_DynCharValue sDynCharValue;
	S_ChannInfo sChannInfo;
	float fDf=.1;
	float pfCustomData[GD_CUSTOMPARAM_BUFFSIZE]={.0};

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
    for (it=lChann.begin();it!=lChann.end();++it)
	{
		  sChann = *it;
	      if(pPlantInfo->GetSpecDynChannCharValue(sChann,sDynCharValue) > -1)
		  {
			  ++iChannNum;
			  int iSmpNum=sDynCharValue._iSmpNum;
			  iLines=iSmpNum/2.56+1;//动态活塞杆沉降不可选分析线数，由采样点数计算获得
			 
			  sChannInfo=pPlantInfo->GetSpecDynChannInfo(sChann);
			  pPlantInfo->GetSpecDynChannWaveAndFreq(sChann,pWave,iPtNums,pFreq,iLines,fDf,lfScaleCoef,lnPrecisionEnabled,lpfWave,lpfFreq);
			  pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
			  pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,sChannInfo._cChannAlias);
			  pPlantInfo->SubtractWaveDC(pWave,pWaveNotDC,iPtNums);
			  pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pWaveNotDC,iPtNums*sizeof(DATATYPE_WAVE));
			  pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char*)pFreq,iLines*sizeof(DATATYPE_WAVE));

			  pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
			  if(lnPrecisionEnabled > 0)
			  {
				  pPlantInfo->SubtractWaveDC(lpfWave,lpfWaveNotDC,iPtNums);				 
				  pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)lpfWaveNotDC,iPtNums*sizeof(float));
				  pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)lpfFreq,iLines*sizeof(float));

			  }
			  //pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iPtNums);
			  pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iLines);
              pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,fDf);
			  pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sDynCharValue,sizeof(S_DynCharValue));
			  pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,sChannInfo._chAlarmParam._fAlarmHL);
			  pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,sChannInfo._chAlarmParam._fAlarmHH);
			  pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,sChannInfo._cUnit);
			  fDf=(float)sDynCharValue._iRev;
			  pFixComm->SetItem(FIELD_SERVICE_REV,fDf);
			  pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());
			  pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
			  pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
			  pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
			  pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam4);
			  pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,sChannInfo._chAlarmParam._pThLearnParamS[9]._fParam2);
			  pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,sChannInfo._chAlarmParam._pThLearnParamS[9]._fParam1);
			  pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);			 
			  int iStartIndex,iEndIndex;
			  pPlantInfo->GetSpecDynChannStartEndIndex(sChannInfo._cChannID,iStartIndex,iEndIndex);

			  if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
			  {
				  int lnWaveNumPeriod = iEndIndex-iStartIndex;
				  if (iPtNums<iEndIndex)
				  {
					  pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iPtNums);
				  }else
				  {
					  pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,lnWaveNumPeriod);
				  }
			  }
			  else
			  {
				  pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iPtNums);
			  }
			 
              pFixComm->SetItem(FIELD_SERVICE_STARTINDEX,iStartIndex);
		      pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,iEndIndex);
			  //CString sTemp;sTemp.Format("iStartIndex:%d, iEndIndex:%d",iStartIndex,iEndIndex);BHTRACE_DEBUG(sTemp);
			  //CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
			  int iAllBuffSize=GD_CUSTOMPARAM_BUFFSIZE*sizeof(float);
			  pPlantInfo->GetSpecDynChannCustomData(sChannInfo._cChannID, pfCustomData, 0, GD_CUSTOMPARAM_BUFFSIZE);
			  //CString sTemp;sTemp.Format("Dyn20016:pfCustomData[0]:%f, pfCustomData[1]:%f,pfCustomData[2]:%f,pfCustomData[3]:%f",pfCustomData[0], pfCustomData[1],pfCustomData[2],pfCustomData[3]);BHTRACE_DEBUG(sTemp);
			  pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)(pfCustomData),iAllBuffSize);
			  pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iAllBuffSize);
			  //CString sTemp;sTemp.Format("ALARM_LOW:%f, ALARM_HIGH:%f,iStartIndex:%d,iEndIndex:%d",sChannInfo._chAlarmParam._pThLearnParamS[9]._fParam2,sChannInfo._chAlarmParam._pThLearnParamS[9]._fParam1,iStartIndex,iEndIndex);
			  //CHZLogManage::Debug(sTemp,"CTransactData","GetRodSinkData");
			  pFixComm->Write(pFixComm->GetAci());
			  pFixComm->CreateNextAnswer();
			 
		  }
	}
	lChann.clear();
	KD_DeletePoint(pChannBuff);
	KD_DeletePoint(pWave);
	KD_DeletePoint(pWaveNotDC);
	 KD_DeletePoint(pFreq);
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
    return iChannNum;
}

/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其波形和特征参数
*如果测点为空就不会返回对应的数据
*输入参数值：
*          公司ID                     FIELD_SYSTEM_COMPANY
*	       分厂名                     FIELD_SYSTEM_FACTORY
*          机组ID                     FIELD_SERVICE_PLANTID
*          振动测点buff               FIELD_SERVICE_CHANN_BUFF
*          振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*          振动测点个数               FIELD_SERVICE_CHANN_NUM
*          动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*          动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*          动态测点个数               FIELD_SERVICE_CHANN_NUM2
*          活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
*          活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*          活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
*返回字段：
*          波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
*          采样频率                   FIELD_SERVICE_SAMPLE_FREQ
*          测点名称                   FIELD_SERVICE_CHANN1
*          测点波形                   FIELD_SERVICE_TIMEWAVE1
*          测点特征值                 FIELD_SERVICE_CHARVALUE_S
*          横坐标                     FIELD_SERVICE_FREQWAVE2
*          转速                       FIELD_SERVICE_REV
*          时间                       FIELD_SERVICE_TIME
*          吸气阀开启相位角           FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角           FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
*/
int CTransactData::OnGetRcMultiParamData(ICommInterFace* pFixComm,CCustomDB* pDB)
{
	int nCount=0;
	float fRev = 0.1;
	string sCompany,sFactory,sPlant,sVibChann,sDynChann,sSinkChann,sUnit;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo*)ReadCheckCompanyInfo(sCompany,sFactory,sPlant,pFixComm,true,PLANTINFO_REAL);
	if(NULL==pPlantInfo)
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

	int iVibSmpNum=1;
	int iDynSmpNum=1;
	int iDynSmpFreq=1;
	int iVibSmpFreq=1;
	int iStartIndex,iEndIndex,iCylinderNo,iTmp1,iTmp2,iTmp3,iRealNums = 0;

	pPlantInfo->GetRealSmpInfo(iVibSmpNum,iVibSmpFreq,true);
	_HZLOG_DEBUG(STR("VibSampleNum=%d, VibSampleFreq=%d",
				iVibSmpNum, iVibSmpFreq));

	pPlantInfo->GetDynRealSmpInfo(iDynSmpNum,iDynSmpFreq,true);
	_HZLOG_DEBUG(STR("DynSampleNum=%d, DynSampleFreq=%d",
				iDynSmpNum, iDynSmpFreq));

	// 动态测点的采样点数取动态/振动二者较小的
	iDynSmpNum=iDynSmpNum>iVibSmpNum?iDynSmpNum:iVibSmpNum;
	_HZLOG_DEBUG(STR("DynSmpNum=%d", iDynSmpNum));

	DATATYPE_WAVE *pDynWave =new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	DATATYPE_WAVE * pXCoor = new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	DATATYPE_WAVE *pWaveNotDC =new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

	vector<float> loDynWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> loXCoor(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	vector<float> loWaveNotDC(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

	float *pfDynWave = &loDynWave.front();
	float *pfXCoor = &loXCoor.front();
	float *pfWaveNotDC = &loWaveNotDC.front();


	S_DynCharValue sDynCharValue;
	S_VibCharValue sVibCharValue;
	S_ChannInfo sChannInfo;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;
	float lfMaxValue = 0;
	float lfMinValue = 0;
    //振动测点
	for (itVib=lVibChann.begin();itVib!=lVibChann.end();++itVib){

		sVibChann = *itVib;
		pPlantInfo->GetSpecVibChannStartEndIndex(sVibChann,iStartIndex,iEndIndex);
		pPlantInfo->GetSpecChannRev(sVibChann,fRev);
		iCylinderNo=pPlantInfo->GetSpecVibChannInfo(sVibChann)._iCylinderNo;

		int iPeriodSmpNums = iEndIndex-iStartIndex;
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

		if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE  || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL )
		{
			int iPeriodSmpNums = iEndIndex;
			if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iVibSmpNum))
				iRealNums = pPlantInfo->GetPVXCoorData( iVibSmpFreq, iVibSmpNum, GE_PVTYPE_Pa, fRev,iCylinderNo,pXCoor);//返回的是一个周期的点数
			else
				iRealNums = pPlantInfo->GetPVXCoorData(iVibSmpFreq,iEndIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
		}
		else
		{
			if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iVibSmpNum))
				iRealNums = pPlantInfo->GetPVXCoorData( iVibSmpFreq, iVibSmpNum, GE_PVTYPE_Pa, fRev,iCylinderNo,pXCoor);//返回的是一个周期的点数
			else
				iRealNums = pPlantInfo->GetPVXCoorData(iVibSmpFreq,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
			_HZLOG_DEBUG(STR("CylinderNo=%d RealSmpNum=%d, VibSmpNum=%d, DynSmpNum=%d",
				iCylinderNo, iRealNums, iVibSmpNum, iDynSmpNum));
		}
		
		


		if(pPlantInfo->GetSpecChannRadialAllChar(sVibChann,sVibCharValue,sUnit) > -1)
		{
			++nCount;
			iTmp3=iVibSmpNum;
			pPlantInfo->GetSpecChannVibWave(sVibChann,pDynWave,iTmp3,pfDynWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue);
			sChannInfo=pPlantInfo->GetSpecVibChannInfo(sVibChann);
			_HZLOG_DEBUG(STR("iTmp3=%d", iTmp3));
			if (iTmp3>0)
			{
				
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);

				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sVibChann.c_str());
				if (pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iVibSmpNum))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pDynWave,iRealNums*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)pXCoor, iRealNums*sizeof(DATATYPE_WAVE));

					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,iRealNums*sizeof(float));

					}
				}
				else
				{
					if(pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || pPlantInfo->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pDynWave[0]),(iEndIndex)*sizeof(DATATYPE_WAVE));
						pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)&(pXCoor[0]), (iEndIndex)*sizeof(DATATYPE_WAVE));

						if(lnPrecisionEnabled > 0)
						{
							pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,iEndIndex*sizeof(float));

						}
					}
					else
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pDynWave[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
						pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));

						if(lnPrecisionEnabled > 0)
						{
							pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,(iEndIndex-iStartIndex)*sizeof(float));

						}
					}
					
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sVibCharValue,sizeof(S_VibCharValue));
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam4);

				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealNums);
				pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iVibSmpFreq);
				pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
				pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealVibDataDateTime());
				pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			//	writefile(pDynWave,iRealNums,3);
		}

	}

	//动态压力
	for(itDyn=lDynChann.begin(); itDyn!=lDynChann.end(); ++itDyn)
	{
		sDynChann=*itDyn;
		pPlantInfo->GetSpecDynChannStartEndIndex(sDynChann,iStartIndex,iEndIndex);
		pPlantInfo->GetSpecChannRev(sDynChann,fRev);
		iCylinderNo=pPlantInfo->GetSpecDynChannInfo(sDynChann)._iCylinderNo;
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iDynSmpNum))
			iRealNums = pPlantInfo->GetPVXCoorData( iDynSmpFreq, iDynSmpNum, GE_PVTYPE_Pa, fRev,iCylinderNo,pXCoor);//返回的是一个周期的点数
		else
			iRealNums = pPlantInfo->GetPVXCoorData(iDynSmpFreq,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
		//CString strInfo;strInfo.Format("dyn:%d,%d,%d,%d,%d,%",iStartIndex,iEndIndex,iDynSmpNum,iCylinderNo,iRealNums,iDynSmpFreq);
		//CHZLogManage::Debug(strInfo, "Online", "CTransactData::OnGetRcMultiParamData");
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
		if(pPlantInfo->GetSpecDynChannCharValue(sDynChann,sDynCharValue) > -1)
		{
			++nCount;
			iTmp1=iDynSmpNum;
			pPlantInfo->GetSpecDynChannWave(sDynChann,pDynWave,iTmp1,pfDynWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue);
			iStartIndex=0,iEndIndex=0;
			sChannInfo=pPlantInfo->GetSpecDynChannInfo(sDynChann);
			if (iTmp1>0)
			{
				
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sDynChann.c_str());
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
				if (pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iDynSmpNum))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pDynWave,iRealNums*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)pXCoor, iRealNums*sizeof(DATATYPE_WAVE));
					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,iRealNums*sizeof(float));

					}
				}
				else
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pDynWave[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));

					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,(iEndIndex-iStartIndex)*sizeof(float));

					}
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sDynCharValue,sizeof(S_DynCharValue));
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam4);

				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealNums);
				pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iDynSmpFreq);
				pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
				pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());
				pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();

			}
		}
	}

	//活塞杆沉降量
	for (itSink=lSinkChann.begin();itSink!=lSinkChann.end();++itSink)
	{
		sSinkChann=*itSink;
		pPlantInfo->GetSpecDynChannStartEndIndex(sSinkChann,iStartIndex,iEndIndex);
		pPlantInfo->GetSpecChannRev(sSinkChann,fRev);
		iCylinderNo=pPlantInfo->GetSpecDynChannInfo(sSinkChann)._iCylinderNo;
		int iPeriodSmpNums = iEndIndex-iStartIndex;
		if ((pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex)) || (iPeriodSmpNums>iDynSmpNum))
			iRealNums = pPlantInfo->GetPVXCoorData( iDynSmpFreq, iDynSmpNum, GE_PVTYPE_Pa, fRev,iCylinderNo,pXCoor);//返回的是一个周期的点数
		else
			iRealNums = pPlantInfo->GetPVXCoorData(iDynSmpFreq,iEndIndex - iStartIndex,GE_PVTYPE_Pa,iCylinderNo,pXCoor);
		//CString strInfo;strInfo.Format("sink:%d,%d,%d,%d,%d,%",iStartIndex,iEndIndex,iDynSmpNum,iCylinderNo,iRealNums,iDynSmpFreq);
		//CHZLogManage::Debug(strInfo, "Online", "CTransactData::OnGetRcMultiParamData");
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
		if(pPlantInfo->GetSpecDynChannCharValue(sSinkChann,sDynCharValue) > -1)
		{
			++nCount;
			iTmp2=iDynSmpNum;
			pPlantInfo->GetSpecDynChannWave(sSinkChann,pDynWave,iTmp2,pfDynWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue);
			pPlantInfo->SubtractWaveDC(pDynWave,pWaveNotDC,iRealNums);
			sChannInfo=pPlantInfo->GetSpecDynChannInfo(sSinkChann);
			if (iTmp2>0)
			{
				
				pFixComm->SetItem(FIELD_SERVICE_CHANN1,sSinkChann.c_str());
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
				if (pPlantInfo->IsNotDynPeriodIndex(iStartIndex,iEndIndex) || (iPeriodSmpNums>iDynSmpNum))
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)pWaveNotDC,iRealNums*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)pXCoor, iRealNums*sizeof(DATATYPE_WAVE));
					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,iRealNums*sizeof(float));

					}

				}
				else
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char*)&(pWaveNotDC[iStartIndex]),(iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
					pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2,(char *)&(pXCoor[iStartIndex]), (iEndIndex-iStartIndex)*sizeof(DATATYPE_WAVE));
					if(lnPrecisionEnabled > 0)
					{
						pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char*)pfDynWave,(iEndIndex-iStartIndex)*sizeof(float));

					}
				}
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&sDynCharValue,sizeof(S_DynCharValue));
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,sChannInfo._chAlarmParam._pThLearnParamF[8]._fParam4);

				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iRealNums);
				pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iDynSmpFreq);
				pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
				pFixComm->SetItem(FIELD_SERVICE_TIME,pPlantInfo->GetRealDynDataDateTime());
				pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	lVibChann.clear();
	lDynChann.clear();
	lSinkChann.clear();
	KD_DeletePoint(pDynWave);
	KD_DeletePoint(pWaveNotDC);
	KD_DeletePoint(pXCoor);
	KD_DeletePoint(pChannBuff);
    KD_DeletePoint(pChannBuff2);
	KD_DeletePoint(pChannBuff3);
	return nCount;
}

/**读取集团级实时报警统计数据
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名          FIELD_SYSTEM_FACTORY
*返回值的字段码：
*		     设备数          FIELD_SERVICE_PLANT_NUM
*		     报警设备数      FIELD_SERVICE_ALARM_PLANTNUM
*		     运行设备数      FIELD_SERVICE_RUN_PLANTNUM
*		     停车设备数      FIELD_SERVICE_STOP_PLANTNUM
#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
*/
int CTransactData::OnGetGroupAlarmStat(ICommInterFace* pFixComm,CCustomDB* pDB)
{
	SwichDbName(pDB,g_strMainDbName);
	int nCount=0,iPlantNum,iAlarmPlantNum,iRunPlantNum,iStopPlantNum;
	bool bChann = false;
	string sGroup;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	S_PlantNumStatics iPlantNums;
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cGroup,gc_cCompanyTable);
	pDB->Cmd(" WHERE %s='%s'",gc_cCompany,sCompany.c_str());
	if(pDB->More())
	{
		sGroup=pDB->Get(gc_cGroup);
	}
	nCount=GetSpecCompanyAlarmStatics(sGroup,sCompany,iPlantNums);
	//CString sTemp;sTemp.Format("集团名:%s,nCount:%d,设备数:%d,报警设备数:%d,运行设备数:%d,停车设备数:%d",sGroup.c_str(),nCount,iPlantNums._iPlantNum,iPlantNums._iAlarmPlantNum,iPlantNums._iRunPlantNum,iPlantNums._iStopPlantNum);
	//CHZLogManage::Debug(sTemp,"CTransactData","OnGetGroupAlarmStat");
	iPlantNum=iPlantNums._iPlantNum;
	iAlarmPlantNum=iPlantNums._iAlarmPlantNum;
	iRunPlantNum=iPlantNums._iRunPlantNum;
	iStopPlantNum=iPlantNums._iStopPlantNum;
    if (nCount <0)
    {
		iPlantNum = 0;
		iAlarmPlantNum = 0;
		iRunPlantNum = 0;
		iStopPlantNum = 0;
    }
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,iPlantNum);
	pFixComm->SetItem(FIELD_SERVICE_ALARM_PLANTNUM,iAlarmPlantNum);
	pFixComm->SetItem(FIELD_SERVICE_RUN_PLANTNUM,iRunPlantNum);
	pFixComm->SetItem(FIELD_SERVICE_STOP_PLANTNUM,iStopPlantNum);
	pFixComm->Write( pFixComm->GetAci() );
	return nCount;
}

/**读取设备实时报警统计
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		      分厂名          FIELD_SYSTEM_FACTORY
*		      设备名          FIELD_SERVICE_PLANTID
*返回值的字段码：
*		     公司名           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*		     设备名           FIELD_SERVICE_PLANTID
*		     测点数           FIELD_SERVICE_CHANN_NUM
*		     报警状态         FIELD_SERVICE_ALARM_STATUS
*		     启停车状态       FIELD_SERVICE_SUD_STATUS 
*		     转速1              FIELD_SERVICE_REV
*		     报警ID             FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC (-1: 未知0: 自己判断报警 1: 同步自下级)
#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT          20025
*/
int CTransactData::OnGetCompanyAlarmStat(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	

	if (NULL == g_pCompanyPoolAlarm)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -1;
	}

	int nCount = 0;

	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->Get(FIELD_SERVICE_PLANTID);
	
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	
	if (sCompany.empty())
	{
		nCount += SetCompanyAlarmState(g_pCompanyPoolAlarm, "", pFixComm, pDB);

		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return nCount;
	}

	CFactoryInfoPool * lpFactoryPool = g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany);
	if (NULL == lpFactoryPool)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -2;
	}

	if (sFactory.empty())
	{
		nCount += SetFactoryAlarmState(lpFactoryPool, "", pFixComm, pDB);

		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return nCount;
	}


	CPlantInfoPool * lpPlantInfoPool = lpFactoryPool->GetSpecInfoPool(sFactory);
	if (NULL == lpPlantInfoPool)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -3;
	}

	nCount += SetPlantAlarmState(lpPlantInfoPool, sPlant, pFixComm, pDB);

    pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci());

	return nCount;
}

bool CTransactData::IsRun(CAlarmPlantInfo *pPlantInfo_)
{
	float fRev[5];
	pPlantInfo_->GetRealRev(fRev,5);
		if (fRev[0]>10.0)//获取到的转速大于10时，即认为是开车状态
      return true;
	return false;
}

int CTransactData::SetSpecPlantAlarmState(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_)
{
	if (pPlantInfo_ ==NULL)
	{
		return -1;
	}
	bool bStopped = false;
	bool bAlarm = false;
	int iChannNum = 0;
	float pfRev[5];
	//float fRev = .0;
	string sChann = "";
	pFixComm_->SetItem(FIELD_SYSTEM_COMPANY, pPlantInfo_->GetCompanyName().c_str());
	pFixComm_->SetItem(FIELD_SYSTEM_FACTORY, pPlantInfo_->GetFactoryName().c_str());
	pFixComm_->SetItem(FIELD_SERVICE_PLANTID,pPlantInfo_->GetPlantID().c_str());
	pFixComm_->SetItem(FIELD_SERVICE_PLANT_TYPE, pPlantInfo_->GetPlantInfo()._iMachineType);
	iChannNum = pPlantInfo_->GetVibChannNum() + pPlantInfo_->GetProcChannNum() + pPlantInfo_->GetDynChannNum();
	pFixComm_->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum);
	
	S_PlantInfo loPlantInfo = pPlantInfo_->GetPlantInfo();
	/*bAlarm 返回 正常0, 低报1, 高报2*/
	int lnAlarmStatus = pPlantInfo_->GetPlantAlarmStateEx(); 
	pFixComm_->SetItem(FIELD_SERVICE_ALARM_STATUS, lnAlarmStatus);
	pFixComm_->SetItem(FIELD_SYSTEM_COUNTRY, pPlantInfo_->Country().GetBuffer(0));
	pPlantInfo_->GetRealRev(pfRev,5);
	pFixComm_->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, pPlantInfo_->PlantDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_SET_DISPLAY, pPlantInfo_->SetDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY, pPlantInfo_->FactoryDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY, pPlantInfo_->CompanyDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_GROUP_DISPLAY, pPlantInfo_->GroupDisplayName());

	int lnProcJudgedStatus = pPlantInfo_->GetProcSUDStatus();
	int lnVibJudgedStatus = pPlantInfo_->GetVibJudgeRunStatus();
	COleDateTimeSpan loTimeSpan =COleDateTime::GetCurrentTime() -  pPlantInfo_->TimeLastUpdateTime();

	CBHDateTime loUpdateTime = pPlantInfo_->GetRealVibDataDateTime();

	int lnRev = 0;//pfRev[0];
	
	//取5级转速中的最大转速作为开停车判断参开转速
	for (int lnRevIndex = 0; lnRevIndex < 5; lnRevIndex++)
	{
		int lnRevTemp = (int)pfRev[lnRevIndex];
		
		if (lnRevIndex == 0)
		{
			lnRev = (int)pfRev[0];
			
			continue;
		}
		
		if (lnRev < lnRevTemp)
		{
			lnRev = lnRevTemp;
		}
	}

	for(int i = 0; i < 5; i++)
	{
		if(lnRev < pfRev[i])
		{
			lnRev = pfRev[i];
		}
	}

	bool lbSudStatus;
	int lbSystemNotConnected = 0;//断网状态：0表示未断网，1表示断网
	int lnSudType = -1;//-1表示不在启停车状态中
	int lnStatusValid = 2;//1：表示状态有效，2：表示状态无效

	lnSudType = pPlantInfo_->GetSudStatus(lbSudStatus);

	if (lbSudStatus && lnRev > g_rev_threshold) // 起停车状态中
	{
		switch (lnSudType) 
		{
		case GE_SUD_START:
			break;
		case GE_SUD_STOP:
			break;
		case GE_SUD_UNKNOWN:
			break;
		default:
			break;
		}
		lnStatusValid = 1;
	}
	else 
	{
		lnSudType = -1;

		if (pPlantInfo_->IsAllChannUpdate())
		{
			lnStatusValid = 1;
		}
		else if ( pPlantInfo_->IsHasChannNotUpdate() && pPlantInfo_->IsPlantUpdateOverTime())
		{
			lnStatusValid = 1;
		}

		int lnTimeElapsed = loTimeSpan.GetTotalHours();

		if (lnTimeElapsed >= g_nDefaultNetOffTimeSpan)
		{
			bStopped = false;
			lbSystemNotConnected = 1;
			lnStatusValid = 2;
		}

		if (lbSystemNotConnected == 0)
		{
			if(lnRev > 0)
			{
				if(lnRev > g_rev_threshold)
				{
					bStopped = false;
				}
				else
				{
					bStopped = true;
				}
			}
			else
			{
				if (lnVibJudgedStatus > 0)//无线测点:1:开车 2：停车 0：未知
				{
					if(lnVibJudgedStatus == 1)
					{
						bStopped = false;
					}
					else
					{
						bStopped = true;
					}
				}
				else if (lnProcJudgedStatus > 0) //过程量参与了判断
				{
					if (lnProcJudgedStatus==2)
					{
						if (lnAlarmStatus<=0)
						{
							bStopped = true;

						}else
						{
							bStopped = false;
						}

					}else
					{
						bStopped = false;
					}
				}
				else
				{
					if ( (lnRev<1) && lnAlarmStatus <=0)
					{
						bStopped = true;
					}
				}
			}
		}//if (lbSystemNotConnected == 1)
	}//if (lbSudStatus)


	if (!g_bOpenNetOffStatus)
	{
		if (lbSystemNotConnected == 1)
		{
			bStopped = true;
		}

		lbSystemNotConnected = -1;
	}

	pFixComm_->SetItem(FIELD_SERVICE_SUD_STATUS, bStopped);
	pFixComm_->SetItem(FIELD_SERVICE_SUD_TYPE,lnSudType);
	pFixComm_->SetItem(FIELD_SYSTEM_STATUS, lbSystemNotConnected);
	pFixComm_->SetItem(FIELD_SERVICE_DATA_TYPE,lnStatusValid);
	//CString sTemp;sTemp.Format("测点数:%d,转速:%.2f",iChannNum,pfRev[0]);
	//CHZLogManage::Debug(sTemp,"CTransactData","SetSpecPlantAlarmState");
	pFixComm_->SetItem(FIELD_SERVICE_REV, lnRev);
	pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_REV, pfRev[0]);
	pFixComm_->SetItem(FIELD_SERVICE_REV2, pfRev[1]);
	pFixComm_->SetItem(FIELD_SERVICE_REV3, pfRev[2]);
	pFixComm_->SetItem(FIELD_SERVICE_REV4, pfRev[3]);
	pFixComm_->SetItem(FIELD_SERVICE_REV5,pfRev[4]);
	pFixComm_->SetItem(FIELD_SERVICE_TIME,loUpdateTime);

	long lnAlarmId   = -1;
	long lnAlarmSync = -1;

	GetAlarmEventID(pPlantInfo_, lnAlarmId, lnAlarmSync);

    pFixComm_->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
	pFixComm_->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);


	zdlTraceLine(_T("%s:%s:%s:%s 报警id:%d(同步:%d) 是否停车:%d 是否报警: %d\r\n"),
						pPlantInfo_->GetGroupName().c_str(),
						pPlantInfo_->GetCompanyName().c_str(),
						pPlantInfo_->GetFactoryName().c_str(),
						pPlantInfo_->GetPlantID().c_str(),
						lnAlarmId,
						lnAlarmSync,
						bStopped,
						lnAlarmStatus);



	return  1;
}

int CTransactData::SetSpecPlantAlarmStateEx(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_)
{
	if (pPlantInfo_ ==NULL)
	{
		return -1;
	}
	bool bStopped = false;
	bool bAlarm = false;
	int iChannNum = 0;
	float pfRev[5];
	//float fRev = .0;
	string sChann = "";
	pFixComm_->SetItem(FIELD_SYSTEM_COMPANY, pPlantInfo_->GetCompanyName().c_str());
	pFixComm_->SetItem(FIELD_SYSTEM_FACTORY, pPlantInfo_->GetFactoryName().c_str());
	pFixComm_->SetItem(FIELD_SERVICE_PLANTID,pPlantInfo_->GetPlantID().c_str());
	pFixComm_->SetItem(FIELD_SERVICE_PLANT_TYPE, pPlantInfo_->GetPlantInfo()._iMachineType);
	iChannNum = pPlantInfo_->GetVibChannNum() + pPlantInfo_->GetProcChannNum() + pPlantInfo_->GetDynChannNum();
	pFixComm_->SetItem(FIELD_SERVICE_CHANN_NUM,iChannNum);

	S_PlantInfo loPlantInfo = pPlantInfo_->GetPlantInfo();
	/*bAlarm 返回 正常0, 低报1, 高报2*/
	int lnAlarmStatus = pPlantInfo_->GetPlantAlarmStateEx(); 
	pFixComm_->SetItem(FIELD_SERVICE_ALARM_STATUS, lnAlarmStatus);
	pFixComm_->SetItem(FIELD_SYSTEM_COUNTRY, pPlantInfo_->Country().GetBuffer(0));
	pPlantInfo_->GetRealRev(pfRev,5);
	pFixComm_->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, pPlantInfo_->PlantDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_SET_DISPLAY, pPlantInfo_->SetDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY, pPlantInfo_->FactoryDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY, pPlantInfo_->CompanyDisplayName());
	pFixComm_->SetItem(FIELD_SYSTEM_GROUP_DISPLAY, pPlantInfo_->GroupDisplayName());

	int lnProcJudgedStatus = pPlantInfo_->GetProcSUDStatus();
	int lnVibJudgedStatus = pPlantInfo_->GetVibJudgeRunStatus();
	COleDateTimeSpan loTimeSpan =COleDateTime::GetCurrentTime() -  pPlantInfo_->TimeLastUpdateTime();

	int lnRev = 0;//pfRev[0];

	//取5级转速中的最大转速作为开停车判断参开转速
	for (int lnRevIndex = 0; lnRevIndex < 5; lnRevIndex++)
	{
		int lnRevTemp = (int)pfRev[lnRevIndex];

		if (lnRevIndex == 0)
		{
			lnRev = (int)pfRev[0];

			continue;
		}

		if (lnRev < lnRevTemp)
		{
			lnRev = lnRevTemp;
		}
	}

	for(int i = 0; i < 5; i++)
	{
		if(lnRev < pfRev[i])
		{
			lnRev = pfRev[i];
		}
	}

	bool lbSudStatus;
	int lbSystemNotConnected = 0;//断网状态：0表示未断网，1表示断网
	int lnSudType = -1;//-1表示不在启停车状态中
	int lnStatusValid = 2;//1：表示状态有效，2：表示状态无效

	lnSudType = pPlantInfo_->GetSudStatus(lbSudStatus);

	if (lbSudStatus) // 起停车状态中
	{
		switch (lnSudType) 
		{
		case GE_SUD_START:
			break;
		case GE_SUD_STOP:
			break;
		case GE_SUD_UNKNOWN:
			break;
		default:
			break;
		}
		lnStatusValid = 1;
	}
	else 
	{
		lnSudType = -1;

		if (pPlantInfo_->IsAllChannUpdate())
		{
			lnStatusValid = 1;
		}
		else if ( pPlantInfo_->IsHasChannNotUpdate() && pPlantInfo_->IsPlantUpdateOverTime())
		{
			lnStatusValid = 1;
		}

		int lnTimeElapsed = loTimeSpan.GetTotalHours();

		if (lnTimeElapsed >= g_nDefaultNetOffTimeSpan)
		{
			bStopped = false;
			lbSystemNotConnected = 1;
			lnStatusValid = 2;
		}

		if (lbSystemNotConnected == 0)
		{
			if(lnRev > 0)
			{
				if(lnRev > g_rev_threshold)
				{
					bStopped = false;
				}
				else
				{
					bStopped = true;
				}
			}
			else
			{
				if (lnVibJudgedStatus > 0)//无线测点:1:开车 2：停车 0：未知
				{
					if(lnVibJudgedStatus == 1)
					{
						bStopped = false;
					}
					else
					{
						bStopped = true;
					}
				}
				else if (lnProcJudgedStatus > 0) //过程量参与了判断
				{
					if (lnProcJudgedStatus==2)
					{
						if (lnAlarmStatus<=0)
						{
							bStopped = true;

						}else
						{
							bStopped = false;
						}

					}else
					{
						bStopped = false;
					}
				}
				else
				{
					if ( (lnRev<1) && lnAlarmStatus <=0)
					{
						bStopped = true;
					}
				}
			}
		}//if (lbSystemNotConnected == 1)
	}//if (lbSudStatus)


	if (!g_bOpenNetOffStatus)
	{
		if (lbSystemNotConnected == 1)
		{
			bStopped = true;
		}

		lbSystemNotConnected = -1;
	}

	pFixComm_->SetItem(FIELD_SERVICE_SUD_STATUS, bStopped);
	pFixComm_->SetItem(FIELD_SERVICE_SUD_TYPE,lnSudType);
	pFixComm_->SetItem(FIELD_SYSTEM_STATUS, lbSystemNotConnected);
	pFixComm_->SetItem(FIELD_SERVICE_DATA_TYPE,lnStatusValid);
	//CString sTemp;sTemp.Format("测点数:%d,转速:%.2f",iChannNum,pfRev[0]);
	//CHZLogManage::Debug(sTemp,"CTransactData","SetSpecPlantAlarmState");
	//pFixComm_->SetItem(FIELD_SERVICE_REV, lnRev);

	long lnAlarmId   = -1;
	long lnAlarmSync = -1;

	GetAlarmEventID(pPlantInfo_, lnAlarmId, lnAlarmSync);

	pFixComm_->SetItem(FIELD_SERVICE_ALARMID, lnAlarmId);
	pFixComm_->SetItem(FIELD_SERVICE_ALARM_SYNC, lnAlarmSync);


	zdlTraceLine(_T("%s:%s:%s:%s 报警id:%d(同步:%d) 是否停车:%d 是否报警: %d\r\n"),
		pPlantInfo_->GetGroupName().c_str(),
		pPlantInfo_->GetCompanyName().c_str(),
		pPlantInfo_->GetFactoryName().c_str(),
		pPlantInfo_->GetPlantID().c_str(),
		lnAlarmId,
		lnAlarmSync,
		bStopped,
		lnAlarmStatus);



	return  1;
}

int CTransactData::SetPlantAlarmState(CPlantInfoPool * apPlantPool,
					   std::string asPlant,
					   ICommInterFace *  apFixComm, 
					   CCustomDB * apDB)
{
	if (NULL == apFixComm || NULL == apDB)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (NULL == apPlantPool)
	{
		ASSERT(FALSE);
		return 0;
	}

	int lnCount = 0;

	CAlarmPlantInfo * lpPlantInfo = NULL;

	if (asPlant.empty())
	{
		std::string lsPlant;

		for (int i = 0; i < apPlantPool->GetInfoNum(); ++i)
		{
			lpPlantInfo = (CAlarmPlantInfo *)apPlantPool ->GetSpecInfoPool(i, lsPlant);

			if (lpPlantInfo != NULL)
			{
				if (SetSpecPlantAlarmState( apFixComm, lpPlantInfo, apDB)> -1)
				{
					apFixComm->Write(apFixComm->GetAci());
					apFixComm->CreateNextAnswer();

					++lnCount;
				}
			}
		}
	}
	else
	{
		lpPlantInfo = (CAlarmPlantInfo *)apPlantPool ->GetSpecInfoPool(asPlant);

		if (lpPlantInfo != NULL)
		{
			if (SetSpecPlantAlarmState( apFixComm, lpPlantInfo, apDB)> -1)
			{
				apFixComm->Write(apFixComm->GetAci());
				apFixComm->CreateNextAnswer();

				++lnCount;
			}
		}
	}

	return lnCount;
}

int CTransactData::SetFactoryAlarmState(CFactoryInfoPool * apFactoryPool,
									  std::string asFactory,
									  ICommInterFace *  apFixComm, 
									  CCustomDB * apDB)
{
	if (NULL == apFixComm || NULL == apDB)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (NULL == apFactoryPool)
	{
		ASSERT(FALSE);
		return 0;
	}

	int lnCount = 0;

	CPlantInfoPool * lpPlantPool;

	if (asFactory.empty())
	{
		std::string lsFactory;

		for (int i = 0; i < apFactoryPool->GetInfoNum(); ++i)
		{
			lpPlantPool = apFactoryPool->GetSpecInfoPool(i, lsFactory);
			if (lpPlantPool != NULL)
			{
				lnCount += SetPlantAlarmState(lpPlantPool, "", apFixComm, apDB);
			}
		}
	}
	else
	{
		lpPlantPool = apFactoryPool->GetSpecInfoPool(asFactory);
		if (lpPlantPool != NULL)
		{
			lnCount += SetPlantAlarmState(lpPlantPool, "", apFixComm, apDB);
		}
	}

	return lnCount;
}

int CTransactData::SetCompanyAlarmState(CCompanyInfoPool * apCompanyPool,
										std::string asCompany,
										ICommInterFace *  apFixComm, 
										CCustomDB * apDB)
{
	if (NULL == apFixComm || NULL == apDB)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (NULL == apCompanyPool)
	{
		ASSERT(FALSE);
		return 0;
	}

	int lnCount = 0;

	CFactoryInfoPool * lpFactoryPool;

	if (asCompany.empty())
	{
		std::string lsCompany;

		for (int i = 0; i < apCompanyPool->GetInfoNum(); ++i)
		{
			lpFactoryPool = apCompanyPool->GetSpecInfoPool(i, lsCompany);
			if (lpFactoryPool != NULL)
			{
				lnCount += SetFactoryAlarmState(lpFactoryPool, "", apFixComm, apDB);
			}
		}
	}
	else
	{
		lpFactoryPool = apCompanyPool->GetSpecInfoPool(asCompany);
		if (lpFactoryPool != NULL)
		{
			lnCount += SetFactoryAlarmState(lpFactoryPool, "", apFixComm, apDB);
		}
	}

	return lnCount;
}

void CTransactData::GetAlarmEventID(CAlarmPlantInfo *apPlantInfo, long & anAlarmId, long &anAlarmSync)
{
	if (NULL == apPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}

	long lnDetectedAlarmID = apPlantInfo->_iEventID;
	long lnSyncedAlarmID   = apPlantInfo->_iSyncID;

	if (lnSyncedAlarmID > 0)
	{
		anAlarmId   = lnSyncedAlarmID;
		anAlarmSync = 1;
	}
	else
	{
		anAlarmId   = lnDetectedAlarmID;
		anAlarmSync = 0;
	}
}


int CTransactData::GetAllPlantChannAlarmState(ICommInterFace *pFixComm_,bool bChann_/* =false */)
{
	int nCount=0;
    int iCompanyNum=g_pCompanyPoolAlarm->GetInfoNum();
	string sCompany="";
	for (int loop=0;loop<iCompanyNum;++loop)
	{
		if(g_pCompanyPoolAlarm->GetSpecInfoPool(loop,sCompany)!=NULL)
			nCount+=GetSpecCompanyChannAlarmState(sCompany,sCompany,pFixComm_,bChann_);
	}
	return nCount;
}

int CTransactData::GetSpecCompanyChannAlarmState(string sGroup_,string sCompany_,ICommInterFace *pFixComm_,bool bChann_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
  	int iFactory=pFactoryInfoPool->GetInfoNum();
	string sFactory="";
	for (int loop=0;loop<iFactory;++loop)
	{
		if(pFactoryInfoPool->GetSpecInfoPool(loop,sFactory)!=NULL)
		{
			nCount+=GetSpecFactoryChannAlarmState(sGroup_,sCompany_,sFactory,pFixComm_,bChann_);
		}
	}
   return nCount;
}

int CTransactData::GetSpecFactoryChannAlarmState(string sGroup_,string sCompany_,string sFactory_,ICommInterFace *pFixComm_,bool bChann_/* =false */)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
    pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
	CPlantInfoPool *pPlantInfoPool=pFactoryInfoPool->GetSpecInfoPool(sFactory_);
	if (pPlantInfoPool==NULL) return 0;

	CAlarmPlantInfo *pPlantInfo=NULL;
    int iPlantNum=pPlantInfoPool->GetInfoNum();
	string sPlant="";
	string sChann="";
	int iChannNum=0;
	float fData=.0;
	unsigned int iState=0;

	for (int loop=0;loop<iPlantNum;++loop)
	{
		pPlantInfo=(CAlarmPlantInfo *)pPlantInfoPool->GetSpecInfoPool(loop,sPlant);
		if (pPlantInfo!=NULL)
		{
			if (pPlantInfo->GetPlantAlarmState())
			{
				++nCount;
				//CString sTemp;sTemp.Format("%d",nCount);AfxMessageBox(sTemp);
				if (bChann_&&(pFixComm_!=NULL))
				{
					int loop1=0;
					iChannNum=pPlantInfo->GetVibChannNum();
					for (loop1=0;loop1<iChannNum;++loop1)
					{
						if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,fData,iState,GE_VIBCHANN))
						{
							pFixComm_->SetItem(FIELD_SYSTEM_COMPANY,sCompany_.c_str());
							pFixComm_->SetItem(FIELD_SYSTEM_FACTORY,sFactory_.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());
						    pFixComm_->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
						    pFixComm_->Write(pFixComm_->GetAci());
						    pFixComm_->CreateNextAnswer();
						}
					}
					iChannNum=pPlantInfo->GetProcChannNum();
					for (loop1=0;loop1<iChannNum;++loop1)
					{
						if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,fData,iState,GE_ALLPROC))
						{
							pFixComm_->SetItem(FIELD_SYSTEM_COMPANY,sCompany_.c_str());
							pFixComm_->SetItem(FIELD_SYSTEM_FACTORY,sFactory_.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
							pFixComm_->Write(pFixComm_->GetAci());
							pFixComm_->CreateNextAnswer();
						}
					}
					iChannNum=pPlantInfo->GetDynChannNum();
					for (loop1=0;loop1<iChannNum;++loop1)
					{
						if (pPlantInfo->GetSpecChannAndTypeAlarmState(loop1,sChann,fData,iState,GE_DYNPRESSCHANN))
						{
							pFixComm_->SetItem(FIELD_SYSTEM_COMPANY,sCompany_.c_str());
							pFixComm_->SetItem(FIELD_SYSTEM_FACTORY,sFactory_.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
							pFixComm_->SetItem(FIELD_SERVICE_CHANN1VALUE,fData);
							pFixComm_->Write(pFixComm_->GetAci());
							pFixComm_->CreateNextAnswer();
						}
					}

				}
				else
				{
					if (pFixComm_!=NULL)
					{
						pFixComm_->SetItem(FIELD_SYSTEM_COMPANY,sCompany_.c_str());
						pFixComm_->SetItem(FIELD_SYSTEM_FACTORY,sFactory_.c_str());
						pFixComm_->SetItem(FIELD_SERVICE_PLANTID,sPlant.c_str());
						pFixComm_->Write(pFixComm_->GetAci());
                        pFixComm_->CreateNextAnswer();
					}
				}
			}
        }
	}
    return nCount;
}

int CTransactData::GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,CCustomDB *pDB_,bool bChann_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int nCount=0;
	list<string> listCompany;
	string sCompany;
	pDB_->Cmd("SELECT * FROM [%s] ",gc_cCompanyTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cGroup,sGroup_.c_str());
	while (pDB_->More())
	{
		sCompany=pDB_->Get(gc_cCompany);
		if (!sCompany.empty())
		  listCompany.push_back(sCompany);
	}
	list<string>::iterator it=listCompany.begin();
	for (;it!=listCompany.end();++it)
	{
		nCount+=GetSpecCompanyChannAlarmState(sGroup_,(*it),pFixComm_,bChann_);
	}
	return nCount;
}

int CTransactData::GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,IDBInterFace *pDB_,bool bChann_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int nCount=0;
	list<string> listCompany;
	string sCompany;
	pDB_->Cmd("SELECT * FROM [%s] ",gc_cCompanyTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cGroup,sGroup_.c_str());
	while (pDB_->More())
	{
		sCompany=pDB_->Get(gc_cCompany);
		if (!sCompany.empty())
			listCompany.push_back(sCompany);
	}
	list<string>::iterator it=listCompany.begin();
	for (;it!=listCompany.end();++it)
	{
		nCount+=GetSpecCompanyChannAlarmState(sGroup_,(*it),pFixComm_,bChann_);
	}
	return nCount;
}

list<string> CTransactData::ParseChannList(char *pChannBuf_,const int &iNum_)
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

///解释报警状态，如果返回0，说明没有报警，1表示常规低报，2表示常规高报，3表示快变报警，4表示缓变报警，5表示趋势报警
//6表示自定义低报，7自定义高报。
///数字越小，越优先,即如果存在常规报警情况下，就不会向下判断，是否有快变、缓变等。
int ParseAlarmStatus(unsigned int iState_)
{
	return ::AlarmStatus_ParseAlarmStatus(iState_);
}

CPlantInfoOnLine* CTransactData::ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_,const PLANTINFOTYPE &plantInfoType_)
{
	sCompany_=pFixComm_->Get(FIELD_SYSTEM_COMPANY);
	sFactory_=pFixComm_->Get(FIELD_SYSTEM_FACTORY);
	sPlant_=pFixComm_->Get(FIELD_SERVICE_PLANTID);
	return CheckCompanyInfo(sCompany_,sFactory_,sPlant_,pFixComm_,bRtn_,plantInfoType_);
}

CPlantInfoOnLine* CTransactData::CheckCompanyInfo(const string &sCompany_,const string &sFactory_,const string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_,const PLANTINFOTYPE &plantInfoType_)
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
	CPlantInfoOnLine *pPlantInfo=GetSpecCompanyFactoryPlant(sCompany_,sFactory_,sPlant_,plantInfoType_);
	if ((NULL==pPlantInfo)&&(bRtn_))
	{
		pFixComm_->CreateAnswer(pFixComm_->GetFunc());
		pFixComm_->Write(pFixComm_->GetAci());
	}
	return pPlantInfo;
}


int CTransactData::GetSpecGroupAlarmStatics(string sGroup_,CCustomDB *pDB_, S_PlantNumStatics &plantNumStatics_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int nCount=0;
	list<string> listCompany;
	string sCompany;
	pDB_->Cmd("SELECT * FROM [%s] ",gc_cCompanyTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cGroup,sGroup_.c_str());
	while (pDB_->More())
	{
		sCompany=pDB_->Get(gc_cCompany);
		if (!sCompany.empty())
			listCompany.push_back(sCompany);
	}
	list<string>::iterator it=listCompany.begin();
	S_PlantNumStatics iPlantNums;
	for (;it!=listCompany.end();++it)
	{
		nCount+=GetSpecCompanyAlarmStatics(sGroup_,(*it),plantNumStatics_);
		iPlantNums = iPlantNums + plantNumStatics_;
	}
	plantNumStatics_ = iPlantNums;
	return nCount;
}

int CTransactData::GetSpecGroupAlarmStatics(string sGroup_,IDBInterFace *pDB_, S_PlantNumStatics &plantNumStatics_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int nCount=0;
	list<string> listCompany;
	string sCompany;
	pDB_->Cmd("SELECT * FROM [%s] ",gc_cCompanyTable);
	pDB_->Cmd(" WHERE [%s]='%s'",gc_cGroup,sGroup_.c_str());
	while (pDB_->More())
	{
		sCompany=pDB_->Get(gc_cCompany);
		if (!sCompany.empty())
			listCompany.push_back(sCompany);
	}
	list<string>::iterator it=listCompany.begin();
	S_PlantNumStatics iPlantNums;
	for (;it!=listCompany.end();++it)
	{
		nCount+=GetSpecCompanyAlarmStatics(sGroup_,(*it),plantNumStatics_);
		iPlantNums = iPlantNums + plantNumStatics_;
	}
	plantNumStatics_ = iPlantNums;
	return nCount;
}

int CTransactData::GetSpecCompanyAlarmStatics(string sGroup_,string sCompany_,S_PlantNumStatics &plantNumStatics_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
	int iFactory=pFactoryInfoPool->GetInfoNum();
	string sFactory="";
	S_PlantNumStatics iPlantNums;
	for (int loop=0;loop<iFactory;++loop)
	{
		if(pFactoryInfoPool->GetSpecInfoPool(loop,sFactory)!=NULL)
		{
			nCount+=GetSpecFactoryAlarmStatics(sGroup_,sCompany_,sFactory,plantNumStatics_);
			iPlantNums = iPlantNums + plantNumStatics_;
		}
	}
	plantNumStatics_ = iPlantNums;
	return nCount;
}

int CTransactData::GetSpecFactoryAlarmStatics(string sGroup_,string sCompany_,string sFactory_,S_PlantNumStatics &plantNumStatics_)
{
	int nCount=0;
	CFactoryInfoPool *pFactoryInfoPool=NULL;
	pFactoryInfoPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
	if (pFactoryInfoPool==NULL) return 0;
	CPlantInfoPool *pPlantInfoPool=pFactoryInfoPool->GetSpecInfoPool(sFactory_);
	if (pPlantInfoPool==NULL)   return 0;

	CAlarmPlantInfo *pPlantInfo=NULL;
	int iPlantNum=pPlantInfoPool->GetInfoNum();
	S_PlantNumStatics plantNumsStatics;
	plantNumsStatics._iPlantNum = iPlantNum;
	string sPlant="";
	string sChann="";

	float fData=.0;
	unsigned int iState=0;
	int iRunPlantNums = 0;

	for (int loop=0;loop<iPlantNum;++loop)
	{
		pPlantInfo=(CAlarmPlantInfo *)pPlantInfoPool->GetSpecInfoPool(loop,sPlant);
		if (IsRun(pPlantInfo))
			iRunPlantNums ++;
		if (pPlantInfo!=NULL)
		{
			if (pPlantInfo->GetPlantAlarmState())
				++nCount;
		}
	}
	plantNumsStatics._iAlarmPlantNum = nCount;
	plantNumsStatics._iRunPlantNum   = iRunPlantNums;
	plantNumsStatics._iStopPlantNum  = iPlantNum-iRunPlantNums;
	plantNumStatics_ = plantNumsStatics;
	return nCount;
}

/**得到指定机组指定一组测点的活塞杆轴心位置轨迹
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名            FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          活塞杆位置描述      FIELD_SYSTEM_AXISLOCATION
*          水平方向测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数          FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          水平方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          垂直方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_VER
*          实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
*          水平方向测点特征值  FIELD_SERVICE_CHARVALUE_S
*          垂直方向测点特征值  FIELD_SERVICE_CHARVALUE_S2
*          时间                FIELD_SERVICE_TIME
*		   转速1               FIELD_SERVICE_REV
*		   轴心轨迹圆点X坐标   FIELD_SERVICE_AXIS_COORX
*		   轴心轨迹圆点Y坐标   FIELD_SERVICE_AXIS_COORY
*		   报警椭圆水平轴半径  FIELD_SERVICE_HOR_RADIUS
*		   报警椭圆垂直轴半径  FIELD_SERVICE_VER_RADIUS
#define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION         20027
*/
int CTransactData::OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	int index=-1;
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	DATATYPE_WAVE *pVerAxesWave=NULL;
	DATATYPE_WAVE *pHorAxesWave=NULL;

	float         *pfVerWave = NULL;
	float         *pfHorWave = NULL;
	float         *pfVerAxesWave = NULL;
	float         *pfHorAxesWave = NULL;
	long iInputBytes=0;
	int iSmpNum=0,iSmpFreq=0;
	int iStartIndex = 0,iEndIndex = 0 ,iPeriodNum = 0;
	float fRev=0;
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
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime(1999,1,1,1,1,1);
	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	string sChannIDHor = pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer = pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	//CString strTmp;strTmp.Format("1处:sChannVer：%s,sChannHor：%s",sChannIDVer.c_str(),sChannIDHor.c_str());
	//CHZLogManage::Debug( strTmp, "CTransactData","OnGetRodSinkAxesLocation");
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	CAxisChannInfo axisChannInfo;
	axisChannInfo=pPlantInfo->GetSpecDynChannAxisChannInfo(sChannIDHor,sChannIDVer);
	if (sChannIDVer.empty()||sChannIDHor.empty())
	{
		axisChannInfo=pPlantInfo->GetSpecLocationAxisChannInfo(sLocation);
		S_ChannInfo sHorChannInfo = pPlantInfo->GetSpecDynChannInfo(axisChannInfo._cHorChann);
		S_ChannInfo sVerChannInfo = pPlantInfo->GetSpecDynChannInfo(axisChannInfo._cVerChann);
		sChannIDHor=sHorChannInfo._cChannID;
		sChannIDVer=sVerChannInfo._cChannID;
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if((sChannIDHor!="")&&(sChannIDVer!=""))
	{
		axisChannInfo = pPlantInfo->GetSpecDynChannAxisChannInfo(sChannIDHor, sChannIDVer);
		S_ChannInfo channInfo;
		S_DynCharValue dynValue;
		channInfo=pPlantInfo->GetSpecDynChannInfo(sChannIDHor);
		pPlantInfo->GetSpecDynChannCharValue(channInfo._cChannID,dynValue);
        iSmpFreq=dynValue._iSmpFreq;
		iSmpNum=dynValue._iSmpNum;
		iWaveNum=iWaveNum>iSmpNum?iSmpNum:iWaveNum;
		pPlantInfo->GetSpecDynChannStartEndIndex(sChannIDVer,iStartIndex,iEndIndex);
	    iPeriodNum = iEndIndex - iStartIndex;
		iWaveNum = iWaveNum>iPeriodNum?iPeriodNum:iWaveNum;
		//CString strInfo;strInfo.Format("%s,%s,%d,%d",sChannIDHor.c_str(),sChannIDVer.c_str(),iPeriodNum,iWaveNum);
		//CHZLogManage::Debug( strInfo, "CTransactData","OnGetRodSinkAxesLocation");
		timeCurr=pPlantInfo->GetRealDynDataDateTime();
		pPlantInfo->GetSpecChannRev(sChannIDVer,fRev);
		if (iWaveNum<=0)
		{
			return -1;
		}
		pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pVerAxesWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pHorAxesWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		vector<float> loVerWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		vector<float> loHorWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		vector<float> loVerAxesWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		vector<float> loHorAxesWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);

		pfVerWave = &loVerWave.front();
		pfHorWave = &loHorWave.front();
		pfVerAxesWave = &loVerAxesWave.front();
		pfHorAxesWave = &loHorAxesWave.front();

		iInputBytes=iWaveNum*sizeof(DATATYPE_WAVE);
		//CString strTmp;strTmp.Format("2处:sChannVer：%s,sChannHor：%s",sChannIDVer.c_str(),sChannIDHor.c_str());
		//CHZLogManage::Debug( strTmp, "CTransactData","OnGetRodSinkAxesLocation");
		float lfScaleCoef = 10.0;
		int   lnPrecisionEnabled = 0;
		float lfMaxValue = 0;
		float lfMinValue = 0;

		if ((pPlantInfo->GetSpecDynChannWave(sChannIDHor,pHorWave,iWaveNum,pfHorWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1)&&(pPlantInfo->GetSpecDynChannWave(sChannIDVer,pVerWave,iWaveNum,pfVerWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1))
		{

			iInputBytes=iWaveNum*sizeof(DATATYPE_WAVE);
			/*pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerWave,iInputBytes);
			pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorWave,iInputBytes);*/
			if(sLocation!="")
			{
				if (pPlantInfo->GetSpecDynChannAxisLocationData(sLocation,iWaveNum,pHorWave,pVerWave,pHorAxesWave,pVerAxesWave))
				{
					timeCurr=pPlantInfo->GetRealDynDataDateTime();
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerAxesWave,iInputBytes);
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorAxesWave,iInputBytes);
				}
			}
			else if(!((sChannIDVer.empty())||(sChannIDHor.empty())))
			{
				if (pPlantInfo->GetSpecDynChannAxisLocationData(sChannIDHor,sChannIDVer,iWaveNum,pHorWave,pVerWave,pHorAxesWave,pVerAxesWave))
				{
					timeCurr=pPlantInfo->GetRealDynDataDateTime();
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pVerAxesWave,iInputBytes);
					pFixComm->SetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pHorAxesWave,iInputBytes);
				}
			}
			//CString strInfo;strInfo.Format("%d,%d,%d,%d",pVerAxesWave[0],pVerAxesWave[1],pHorAxesWave[0],pHorAxesWave[1]);
			//CHZLogManage::Debug( strInfo, "CTransactData","OnGetRodSinkAxesLocation");
		}
		pFixComm->SetItem(FIELD_SERVICE_AXESTRACK_WAVENUM,iWaveNum);
		//水平方向特征值数据
		channInfo=pPlantInfo->GetSpecDynChannInfo(sChannIDHor);
		pPlantInfo->GetSpecDynChannCharValue(channInfo._cChannID,dynValue);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&dynValue,sizeof(S_DynCharValue));
		//strInfo.Format("%.2f,%.2f",dynValue[0],dynValue[1]);
		//CHZLogManage::Debug( strInfo, "CTransactData","OnGetRodSinkAxesLocation");
		//垂直方向特征值数据
		channInfo=pPlantInfo->GetSpecDynChannInfo(sChannIDVer);
		pPlantInfo->GetSpecDynChannCharValue(channInfo._cChannID,dynValue);
		pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&dynValue,sizeof(S_DynCharValue));
		//strInfo.Format("%.2f,%.2f,%.2f",dynValue[0],dynValue[1],dynValue[2]);
		//CHZLogManage::Debug( strInfo, "CTransactData","OnGetRodSinkAxesLocation");
		pFixComm->SetItem(FIELD_SERVICE_AXIS_COORX,axisChannInfo._axisAlarmParam._thLearnParamS._fParam3);
	    pFixComm->SetItem(FIELD_SERVICE_AXIS_COORY,axisChannInfo._axisAlarmParam._thLearnParamS._fParam4);
		pFixComm->SetItem(FIELD_SERVICE_HOR_RADIUS,axisChannInfo._axisAlarmParam._thLearnParamS._fParam1);
		pFixComm->SetItem(FIELD_SERVICE_VER_RADIUS,axisChannInfo._axisAlarmParam._thLearnParamS._fParam2);
		//CString sTemp;sTemp.Format("fX:%f,fY:%f,fHorAxis:%f,fVerAxis:%fn",axisChannInfo._axisAlarmParam._thLearnParamS._fParam3,axisChannInfo._axisAlarmParam._thLearnParamS._fParam4,axisChannInfo._axisAlarmParam._thLearnParamS._fParam1,axisChannInfo._axisAlarmParam._thLearnParamS._fParam2);BHTRACE_DEBUG(sTemp);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
		index++;
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		index= -1;
	}
	pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
	pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pHorAxesWave);
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pVerAxesWave);
	return index;
}

/**往复式系统应力监测，得到指定机组指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		   分厂名          　　　  FIELD_SYSTEM_FACTORY
*          机组ID          　　　  FIELD_SERVICE_PLANTID
*          汽缸号                  FIELD_SERVICE_CYLINDERNO
*          缸体振动测点            FIELD_SERVICE_CHANN1
*返回值的字段码：
*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*          振动波形数据   　　　　 FIELD_SERVICE_FREQWAVE2
*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
*		   特征值结构体		       FIELD_SERVICE_CHARVALUE_S
*		   特征值结构体2		   FIELD_SERVICE_CHARVALUE_S2
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
#define     SERVICE_SERVICE_ON_GETDYNSTRESSWAVE              20028
*/
int CTransactData::OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int index = -1;
	string sCompany,sFactory,sPlant,sChann1,sChann2,sStressChann;
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo*)ReadCheckCompanyInfo(sCompany,sFactory,sPlant,pFixComm,true,PLANTINFO_REAL);
	if(NULL==pPlantInfo)
		return -1;
	int iCylinderNo=pFixComm->GetLong(FIELD_SERVICE_CYLINDERNO);
	string sChannVib=pFixComm->GetItem(FIELD_SERVICE_CHANN1);

	if(iCylinderNo < 1)
	{
		BHLOG_ERROR(STR("%d: 未设置缸号 %s|%s|%s",
			SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
			sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

		return -1;
	}

	DATATYPE_WAVE *pWave1=NULL;
	DATATYPE_WAVE *pWave2=NULL;
	DATATYPE_WAVE *pRodLoadWave=NULL;
	float *pfStressWave=NULL;
	float *pfInertiaWave=NULL;
	float *pfRodLoadWave=NULL;
	DATATYPE_WAVE *pVibWave=NULL;
	float         *pfVibWave=NULL;

	CString lstrVibChann = sChannVib.c_str();

	lstrVibChann.Trim();

	int lnVibChannIndex = -1;

	if (lstrVibChann.IsEmpty())
	{
		int lnVibChannNum = pPlantInfo->GetVibChannNum();

		for (int i =0;i<lnVibChannNum;i++)
		{
			S_ChannInfo loSChannInfo = pPlantInfo->GetSpecVibChannInfo(i);

			if (iCylinderNo == loSChannInfo._iCylinderNo)
			{
				lnVibChannIndex = i;
				sChannVib = loSChannInfo._cChannID;

				break;
			}
		}

		if(lnVibChannIndex < 0)
		{
			BHLOG_ERROR(STR("%d: %d缸未组态振动测点 %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				iCylinderNo,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -1;
		}

		if(sChannVib.empty())
		{
			BHLOG_ERROR(STR("%d: %d缸未组态振动测点 %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				iCylinderNo,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -1;
		}
	}
	else
	{
		lnVibChannIndex = pPlantInfo->GetSpecVibChannIndex(lstrVibChann.GetString());
		if(lnVibChannIndex < 0)
		{
			BHLOG_ERROR(STR("%d: 振动测点%s不存在 %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				sChannVib.c_str(),
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -1;
		}
	}

	S_VibCharValue vibCharValue;
	string sUnit;

	if (pPlantInfo->GetDynStressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sStressChann)>0)
	{
		//如果有动态应力测点、则直接从数采获得活塞杆应力波形数据

		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0)
		{
			BHLOG_ERROR(STR("%d: %d缸未找到两个动态压力测点 %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				iCylinderNo,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -2;
		}

		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;
		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1;
		int iStartIndex = 0, iEndIndex = 0;
		S_DynCharValue dynCharValue;
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;

		float lfScaleCoef = 10.0;
		int lnPrecisionEnabled = 0;
		float lfMaxValue = 0;
		float lfMinValue = 0;

		pPlantInfo->GetSpecDynChannCharValue(channInfo1._cChannID,dynCharValue);
		loDynCharValueWriteBack1 = dynCharValue;
		pPlantInfo->GetSpecDynChannCharValue(channInfo2._cChannID,loDynCharValueWriteBack2);

		iSmpFreq=dynCharValue._iSmpFreq;
		iSmpNum=dynCharValue._iSmpNum;
		if (iSmpNum<=0)
		{
			BHLOG_ERROR(STR("%d: 测点%s采样点数不正确(%d) %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				sChann1.c_str(),
				iSmpNum,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -1;
		}
		pWave1=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pWave2= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pRodLoadWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfStressWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfInertiaWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfRodLoadWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pVibWave= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

		vector<float> lofVibWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		pfVibWave = &lofVibWave.front();
		if((pPlantInfo->GetSpecDynChannWave(sChann1,pWave1,iSmpNum,pfStressWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1) && (pPlantInfo->GetSpecDynChannWave(sChann2,pWave2,iSmpNum,pfInertiaWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1) && (pPlantInfo->GetSpecDynChannWave(sStressChann,pRodLoadWave,iSmpNum,pfRodLoadWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1))
		{
			//得到活塞杆负荷波形
			//pPlantInfo->ConvertDTWaveToFloat(pfRodLoadWave,pRodLoadWave,iSmpNum);
			if (pPlantInfo->GetSpecDynChannStartEndIndex(sChann1,iStartIndex,iEndIndex)!=-1)
			{
				int iPeriodSmpNums = iEndIndex-iStartIndex;
				iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
				//计算获得惯性力波形
				if (fAngle1-fAngle2>0)
					iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
				else
					iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
				//计算获得的惯性力波形数据由N转化为KN
				for (int nloop=0;nloop<iSmpNum;nloop++)
					pfInertiaWave[nloop]=pfInertiaWave[nloop]/1000.0;
                //计算获得气体压力波形
				for (int nloop=0;nloop<iSmpNum;nloop++)
					pfStressWave[nloop]=pfRodLoadWave[nloop]-pfInertiaWave[nloop];

				////获得振动测点的波形
				if (pPlantInfo->GetSpecChannVibWave(sChannVib,pVibWave,iSmpNum,pfVibWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)<0)
				{
					CHZLogManage::Error("没有读到振动波形数据","Online","CTransactData::OnGetDynStressWave");
				}

				if(pPlantInfo->GetSpecChannRadialAllChar(lnVibChannIndex, vibCharValue, sUnit) < 0)
				{
					BHLOG_ERROR(STR("%d: 没有读取到测点%s的特征值 %s|%s|%s",
						SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
						sChannVib.c_str(),
						sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));
				}
				float lfScaleCoef = 0;
				if(pPlantInfo->_vDynChann.size() > 0)
				{
					lfScaleCoef = pPlantInfo->GetDynDataWaveCoef(1,0);
				}
				else
				{
					if(pPlantInfo->_vVibChann.size() > 0)
					{
						lfScaleCoef = pPlantInfo->GetVibDataWaveCoef(1,0);
					}
				}
				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pVibWave, iSmpNum * sizeof(DATATYPE_WAVE));
				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
				if(lnPrecisionEnabled > 0)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)pfVibWave,iSmpNum*sizeof(float));
				}
				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);

				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&loDynCharValueWriteBack1,sizeof(S_DynCharValue));
				pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&loDynCharValueWriteBack2,sizeof(S_DynCharValue));

				pFixComm->SetItemBuf(FIELD_SYSTEM_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));

				pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
				pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());


				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam1);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam2);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam3);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo1._chAlarmParam._pThLearnParamF[8]._fParam4);


				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM5,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam1);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM6,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam2);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM7,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam3);
				pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM8,channInfo2._chAlarmParam._pThLearnParamF[8]._fParam4);

				CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();				
				timeCurr=pPlantInfo->GetRealVibDataDateTime();
				pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
				pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);

				pFixComm->Write(pFixComm->GetAci());

				index=1;
				//writefile(pfRodLoadWave,iSmpNum,101);
				//writefile(pfStressWave,iSmpNum,102);
				//writefile(pfInertiaWave,iSmpNum,103);
				//writefile(pVibWave,iSmpNum,104);
				//writefile(pRodLoadWave,iSmpNum,105);
			}
			else
			{
				CHZLogManage::Error("没有得到指定测点的周期起始和结束索引","Online","CTransactData::OnGetDynStressWave");
				index = -4;
				goto QUIT_CLEAR;
			}
		}
		else
		{
			CHZLogManage::Error("没有读到动态压力波形数据","Online","CTransactData::OnGetDynStressWave");
			index = -3;
			goto QUIT_CLEAR;
		}
	}
	else
	{
		//如果没有动态应力测点、则根据两个动态压力测点计算获得活塞杆应力波形数据
		if (pPlantInfo->GetDynPressChann(pDB,sCompany,sFactory,sPlant,iCylinderNo,sChann1,sChann2)<0)
		{
			BHLOG_ERROR(STR("%d: %d缸未找到两个动态压力测点 %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				iCylinderNo,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -2;
		}
		//得到动态压力测点与键相的差角
		S_ChannInfo channInfo1,channInfo2;
		channInfo1=pPlantInfo->GetSpecDynChannInfo(sChann1);
		channInfo2=pPlantInfo->GetSpecDynChannInfo(sChann2);
		float fAngle1=channInfo1._chAlarmParam._pThLearnParamF[8]._fParam5;
		float fAngle2=channInfo2._chAlarmParam._pThLearnParamF[8]._fParam5;

		//获得动态压力测点的波形
		int iSmpNum=1,iSmpFreq=1;
		int iStartIndex = 0, iEndIndex = 0;

		float lfScaleCoef = 10.0;
		int lnPrecisionEnabled = 0;
		float lfMaxValue = 0;
		float lfMinValue = 0;

		S_DynCharValue dynCharValue;
		S_DynCharValue loDynCharValueWriteBack1;
		S_DynCharValue loDynCharValueWriteBack2;

		pPlantInfo->GetSpecDynChannCharValue(channInfo1._cChannID,dynCharValue);
		pPlantInfo->GetSpecDynChannCharValue(channInfo2._cChannID,loDynCharValueWriteBack2);
		iSmpFreq=dynCharValue._iSmpFreq;
		iSmpNum=dynCharValue._iSmpNum;
		loDynCharValueWriteBack1 = dynCharValue;
		if (iSmpNum<=0)
		{
			BHLOG_ERROR(STR("%d: 测点%s采样点数不正确(%d) %s|%s|%s",
				SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
				sChann1.c_str(),
				iSmpNum,
				sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));

			return -1;
		}
		pWave1=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pWave2= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfStressWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfInertiaWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pfRodLoadWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pVibWave= new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

		vector<float> lofVibWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
		pfVibWave = &lofVibWave.front();

		if((pPlantInfo->GetSpecDynChannWave(sChann1,pWave1,iSmpNum,pfStressWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1) && (pPlantInfo->GetSpecDynChannWave(sChann2,pWave2,iSmpNum,pfInertiaWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1))
		{
			if (pPlantInfo->GetSpecDynChannStartEndIndex(sChann1,iStartIndex,iEndIndex)!=-1)
			{
				int iPeriodSmpNums = iEndIndex-iStartIndex;
				iPeriodSmpNums = iPeriodSmpNums>iSmpNum?iSmpNum:iPeriodSmpNums;
				//计算气体压力和惯性力波形
				if (fAngle1-fAngle2>0)
					iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave1,pWave2,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
				else
					iSmpNum=pPlantInfo->GetStrssAndInertiaWave(pWave2,pWave1,iSmpFreq,iCylinderNo,iPeriodSmpNums,pfStressWave,pfInertiaWave);
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
				if (pPlantInfo->GetSpecChannVibWave(sChannVib,pVibWave,iSmpNum,pfVibWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)<0)
				{
					CHZLogManage::Error("没有读到振动波形数据","Online","CTransactData::OnGetDynStressWave");
				}

				if(pPlantInfo->GetSpecChannRadialAllChar(lnVibChannIndex, vibCharValue, sUnit) < 0)
				{
					BHLOG_ERROR(STR("%d: 没有读取到测点%s的特征值 %s|%s|%s",
						SERVICE_SERVICE_ON_GETDYNSTRESSWAVE,
						sChannVib.c_str(),
						sCompany.c_str(), sFactory.c_str(), sPlant.c_str()));
				}

				float lfScaleCoef = 0;
				if(pPlantInfo->_vDynChann.size() > 0)
				{
					lfScaleCoef = pPlantInfo->GetDynDataWaveCoef(1,0);
				}
				else
				{
					if(pPlantInfo->_vVibChann.size() > 0)
					{
						lfScaleCoef = pPlantInfo->GetVibDataWaveCoef(1,0);
					}
				}

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE1, (char *)pfStressWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_TIMEWAVE2, (char *)pfInertiaWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE1, (char *)pfRodLoadWave, iSmpNum * sizeof(float));
				pFixComm->SetItemBuf( FIELD_SERVICE_FREQWAVE2, (char *)pVibWave, iSmpNum * sizeof(DATATYPE_WAVE));
				pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM, iSmpNum);

				pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
				if(lnPrecisionEnabled > 0)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char*)pfVibWave,iSmpNum*sizeof(float));
				}


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
				timeCurr=pPlantInfo->GetRealVibDataDateTime();
				pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
				
				pFixComm->SetItem(FIELD_SYSTEM_CHANN,sChann1.c_str());
				pFixComm->SetItem(FIELD_SYSTEM_CHANN2,sChann2.c_str());
				pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
				pFixComm->Write(pFixComm->GetAci());
				index=1;
			}
			else
			{
				CHZLogManage::Error("没有得到指定测点的周期起始和结束索引","Online","CTransactData::OnGetDynStressWave");
				index = -4;
				goto QUIT_CLEAR;
			}
		}
		else
		{
			CHZLogManage::Error("没有读到动态压力波形数据","Online","CTransactData::OnGetDynStressWave");
			index = -3;
			goto QUIT_CLEAR;
		}
	}
QUIT_CLEAR:
	KD_DeletePoint(pWave1);
	KD_DeletePoint(pWave2);
	KD_DeletePoint(pRodLoadWave);
	KD_DeletePoint(pfStressWave);
	KD_DeletePoint(pfInertiaWave);
	KD_DeletePoint(pfRodLoadWave);
	KD_DeletePoint(pVibWave);
	return index;
}

/**得到某一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*		   分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数             FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          指定机组测点的转速             FIELD_SERVICE_REV
*          转子旋向：                     FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*          水平方向特征值                 FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                 FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
*/
int CTransactData::OnGetPurifyAxesTrack(ICommInterFace* pFixComm,CCustomDB *pDB)
{
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	float         *pfVerWave=NULL;
	float         *pfHorWave=NULL;
	double *pVerWaveData=NULL;
	double *pHorWaveData=NULL;

	vector<float> lofVerWave;
	vector<float> lofHorWave;
	int index=-1;
	int iSmpNum=0,iSmpFreq=0;
	int iRev=0;
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
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime(1999,1,1,1,1,1);
	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	string sChannIDHor=pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer=pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	//CString sTemp;sTemp.Format("iWaveNum:%d,sLocation:%s,sChannIDHor:%s,sChannIDVer:%s",iWaveNum,sLocation.c_str(),sChannIDHor.c_str(),sChannIDVer.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","GetAxesTrack");
	if (sChannIDVer.empty()||sChannIDHor.empty())
	{
		CAxisChannInfo axisChannInfo=pPlantInfo->GetSpecLocationAxisChannInfo(sLocation);
		sChannIDVer=axisChannInfo._cVerChann;
		sChannIDHor=axisChannInfo._cHorChann;
	}
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	#if 0
	{
		CString sLog;
		sLog.Format("sChannIDHor=%s, sChannIDVer=%s", sChannIDHor.c_str(), sChannIDVer.c_str());
		BHTRACE_DEBUG(sLog);
	}
	#endif

	if((sChannIDHor!="")&&(sChannIDVer!=""))
	{
		timeCurr=pPlantInfo->GetRealVibDataDateTime();
		S_ChannInfo channInfo;
		channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
		S_VibCharValue vibCharValue;
		string sUnit="";
		pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibCharValue,sUnit);
		{
		#if 0
			CString sLog;
			sLog.Format("%s VibCharValue(ac=%f,1c=%f,2c=%f,3c=%f,hc=%f,dc=%f,gap=%f,rc=%f,1p=%f,2p=%f,3p=%f,hp=%f,rev=%d,SmpFreq=%d,SmpNum=%d)",
			channInfo._cChannID,
			vibCharValue.GetSpecTypeData(GE_ALLCHAR),
			vibCharValue.GetSpecTypeData(GE_ONECHAR),
			vibCharValue.GetSpecTypeData(GE_TWOCHAR),
			vibCharValue.GetSpecTypeData(GE_THREECHAR),
			vibCharValue.GetSpecTypeData(GE_HALFCHAR),
			vibCharValue.GetSpecTypeData(GE_DCCHAR),
			vibCharValue.GetSpecTypeData(GE_GAP),
			vibCharValue.GetSpecTypeData(GE_RESCHAR),
			vibCharValue.GetSpecTypeData(GE_PHASECHARONE),
			vibCharValue.GetSpecTypeData(GE_PHASECHARTWO),
			vibCharValue.GetSpecTypeData(GE_PHASECHARTHREE),
			vibCharValue.GetSpecTypeData(GE_PHASECHARHALF),
			vibCharValue._iRev,
			vibCharValue._iSmpFreq,
			vibCharValue._iSmpNum
			);
			BHTRACE_DEBUG(sLog);
		#endif
		}
		iSmpFreq=vibCharValue._iSmpFreq;
		iSmpNum=vibCharValue._iSmpNum;
		iRev=vibCharValue._iRev;
		float fRev=(float)iRev;
		iWaveNum=iWaveNum>iSmpNum?iSmpNum:iWaveNum;
		if (iWaveNum <=0)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		float lfScaleCoef = 10.0;
		int lnPrecisionEnabled = 0;
		float lfMaxValue = 0;
		float lfMinValue = 0;
		//CString sTemp;sTemp.Format("iSmpNum:%d,iWaveNum:%d",iSmpNum,iWaveNum);BHTRACE_DEBUG(sTemp);
		pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];

		lofHorWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		lofVerWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);

		pfHorWave = &lofHorWave.front();
		pfVerWave = &lofVerWave.front();


		if ((pPlantInfo->GetSpecChannVibWave(sChannIDHor,pHorWave,iWaveNum,pfHorWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1)&&(pPlantInfo->GetSpecChannVibWave(sChannIDVer,pVerWave,iWaveNum,pfVerWave,lfScaleCoef,lnPrecisionEnabled,lfMaxValue,lfMinValue)!=-1))

		{
			if (iWaveNum>1)
			{

				pVerWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
				pHorWaveData=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
				pPlantInfo->ConvertFloatToDouble(pVerWaveData,pfVerWave,iWaveNum);
				pPlantInfo->ConvertFloatToDouble(pHorWaveData,pfHorWave,iWaveNum);
				//writefile(pHorWaveData,iWaveNum,1);
				//writefile(pVerWaveData,iWaveNum,2);
				//获得提纯轴心轨迹的各个谐波数据
				double f0 = (double)iRev/60;

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
					//CHZLogManage::Error("计算轴心进动方向出错","S_OnlineSev","CTransactData::OnGetPurifyAxesTrack");
				}
				if (iPression==0)//如果计算为不确定则为正进动
				{
					iPression=2;
				}

				double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
				int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM;
				vector<SSigParam> vSigParamV,vSigParamH;
				vSigParamV.resize(MLcNum+MHcNum);
				vSigParamH.resize(MLcNum+MHcNum);
				CSigMath calcAxesData;


				try
				{
					index = calcAxesData.SCorbitpurifie_Rp(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth,vSigParamV,vSigParamH);
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				{
				#if 0
				//HZLOG_ERROR(STR("iWaveNum=%d", iWaveNum));
				//HZLOG_ERROR(STR("f0=%f", f0));
				//HZLOG_ERROR(STR("iSmpFreq=%d", iSmpFreq));
				HZLOG_DEBUG(STR("%d = calcAxesData.SCorbitpurifie_Rp(pVerWaveData,pHorWaveData,iWaveNum=%d,f0=%f,iSmpFreq=%d,MHcNum=%d,AHcth=%f,MLcNum=%d,ALcth=%f,vSigParamV,vSigParamH))",
				index,
				iWaveNum,
				f0,
				iSmpFreq,
				MHcNum,
				AHcth,
				MLcNum,
				ALcth));
				HZLOG_ARRAY("iWaveNum", pVerWaveData, iWaveNum);
				HZLOG_ARRAY("pHorWaveData", pHorWaveData, iWaveNum);
				#endif
				}

				//CString sTemp;sTemp.Format("index:%d,iWaveNum:%d,f0:%f,iSmpFreq:%d,MHcNum:%d,AHcth:%f,MLcNum:%d,ALcth:%f",index,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth);BHTRACE_DEBUG(sTemp);
				if (index>-1)
				{
					for (int iNum =0;iNum<vSigParamH.size();iNum++)
					{
						if (vSigParamH[iNum]._fFreq!=0 && vSigParamH[iNum]._fAmp!=0)
						{
							++index;
							pFixComm->SetItem(FIELD_SERVICE_VERFREQ,vSigParamV[iNum]._fFreq);
							pFixComm->SetItem(FIELD_SERVICE_VERAMP,vSigParamV[iNum]._fAmp);
							pFixComm->SetItem(FIELD_SERVICE_VERPHASE,vSigParamV[iNum]._fPhase);
							pFixComm->SetItem(FIELD_SERVICE_HORFREQ,vSigParamH[iNum]._fFreq);
							pFixComm->SetItem(FIELD_SERVICE_HORAMP,vSigParamH[iNum]._fAmp);
							pFixComm->SetItem(FIELD_SERVICE_HORPHASE,vSigParamH[iNum]._fPhase);

							pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
							{
							#if 0
								CString sLog;
								sLog.Format("%d) VFreq=%f, VAmp=%f, VPhase=%f",
									iNum,
									vSigParamV[iNum]._fFreq,
									vSigParamV[iNum]._fAmp,
									vSigParamV[iNum]._fPhase);
								BHTRACE_DEBUG(sLog);

								sLog.Format("%d) HFreq=%f, HAmp=%f, HPhase=%f",
									iNum,
									vSigParamH[iNum]._fFreq,
									vSigParamH[iNum]._fAmp,
									vSigParamH[iNum]._fPhase);
								BHTRACE_DEBUG(sLog);
							#endif
							}

							//转子旋向
							int iRotation =-1;
							iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
							pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);

							pFixComm->SetItem(FIELD_SYSTEM_STATUS,iPression);
							//CString sTemp;sTemp.Format("iRotation:%d,iPression:%d,iOrbitShape:%d",iRotation,iPression,iOrbitShape);BHTRACE_DEBUG(sTemp);

							S_ChannInfo    channInfo;
							S_VibCharValue vibValue;
							string         sUnit;

							/* 水平方向特征值 */
							channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
							pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
							pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));

							#if 0
							{
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
							}
							#endif

							/* 垂直方向特征值 */
							channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDVer);
							pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
							pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibValue,sizeof(S_VibCharValue));

							#if 0
							{
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
							}
							#endif

							pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);

							pFixComm->Write(pFixComm->GetAci());
							pFixComm->CreateNextAnswer();
						}
					}
				}

			}
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		index= -1;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWaveData);
	KD_DeletePoint(pHorWaveData);
	return index;
}

/**得到指定振动测点的实时波形和倒谱。（如何振动测点2不设置参数，则只提取一组）
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          测点名                                FIELD_SERVICE_CHANN1
*          测点位号                              FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据                        FIELD_SERVICE_TIMEWAVE1
*          测点的倒谱数据                        FIELD_SERVICE_FREQWAVE1
*          测点实时波形上一组数据的个数          FIELD_SERVICE_TIMEWAVE_NUM
*          测点实际倒谱一组数据的个数            FIELD_SERVICE_FREQWAVE_NUM
*          振动测点倒谱DF                        FIELD_SERVICE_FREQWAVE_DF
*          所有特征值                            FIELD_SERVICE_CHARVALUE_S
*          报警值                                FIELD_SYSTEM_ALARM_LOW
*          危险值                                FIELD_SYSTEM_ALARM_HIGH
*          单位                                  FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速                    FIELD_SERVICE_REV
*          时间                                  FIELD_SERVICE_TIME
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_ON_GETCEPSTRUM          20030
*/
int CTransactData::OnGetCepstrum(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	int iRtn = -1;
	DATATYPE_WAVE *fwave=NULL;
	double *pfreqwave=NULL;
	float *pfWave = NULL;
	float df=.0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL){
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	lChann=ParseChannList(pChannBuff,iChannNum);
	itChann=lChann.begin();
	iChannNum=lChann.size();
	int    iPtNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	if (iPtNum<2)
	{
		return -1;
	}
	iPtNum=iPtNum<512?513:iPtNum;

	fwave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
	pfreqwave=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
	vector<float> loWave(GD_MIDDLEWARE_WAVE_POINT_MAX,0);
	pfWave = &loWave.front();
	std::vector<float> loDataWriteBack;
	loDataWriteBack.resize(iPtNum/2);
	int iCepsNums=0;
	float fRev=.0;
	S_VibCharValue vibValue;
	S_ChannInfo channInfo;
	string sUnit="";
	float lfScaleCoef = 10.0;
	int lnPrecisionEnabled = 0;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	int lnDataLength = 0;
	for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
	{
		CString sTemp;sTemp.Format("%d,%s, %s, %d, %d",lChann.size(),itChann->c_str(),pChannBuff,iBytes,iChannNum);
		if ((pPlantInfo->GetSpecChannVibWaveAndCepstrum((*itChann),fwave,iPtNum,pfreqwave,iCepsNums,df,lfScaleCoef,lnPrecisionEnabled,pfWave))!=-1)
		{
			for (int iBuffer =0;iBuffer<loDataWriteBack.size();iBuffer++)
			{
				loDataWriteBack[iBuffer] = pfreqwave[iBuffer];
			}
			++iRtn;
			timeCurr=pPlantInfo->GetRealVibDataDateTime();
			//	strTmp.Format("%d,%d,%.2f %s",iLines,iPtNum,df,itChann->c_str());AfxMessageBox(strTmp);
// 			lnDataLength += iPtNum*sizeof(DATATYPE_WAVE);
// 			lnDataLength += iCepsNums*sizeof(float);
// 			lnDataLength += sizeof(S_VibCharValue);
// 			lnDataLength += 20;
// 			lnDataLength += 100;
// 			lnDataLength += 8*sizeof(float);
			pFixComm->SetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)fwave,iPtNum*sizeof(DATATYPE_WAVE));
			pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
			
			if(lnPrecisionEnabled > 0)
			{
				vector<float> lvFreqWave(loDataWriteBack.size());

				for (int iBuffer =0;iBuffer<loDataWriteBack.size();iBuffer++)
				{
					lvFreqWave[iBuffer] = loDataWriteBack[iBuffer]/10.0;
				}

				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)pfWave,iPtNum*sizeof(float));
				pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_FREQ1,(char *)&(lvFreqWave.front()),iCepsNums*sizeof(float));
				
			}

			pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)&(loDataWriteBack.front()),iCepsNums*sizeof(float));
			pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
			//writefile(fwave,iPtNum,1);
			//writefile(pfreqwave,iCepsNums,2);
			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);
			pFixComm->SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iPtNum);
			pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iCepsNums);
			sTemp.Format("iPtNum:%d,iCepsNums:%d",iPtNum,iCepsNums);
			pPlantInfo->GetSpecChannRadialAllChar((*itChann),vibValue,sUnit);

			pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibValue,sizeof(S_VibCharValue));
			channInfo=pPlantInfo->GetSpecVibChannInfo(*itChann);
			pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,channInfo._cChannAlias);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,channInfo._chAlarmParam._fAlarmHL);
			pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,channInfo._chAlarmParam._fAlarmHH);
			pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,channInfo._cUnit);
			fRev=(float)vibValue._iRev;
			pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
			pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,channInfo._chAlarmParam._pThLearnParamF[8]._fParam1);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM2,channInfo._chAlarmParam._pThLearnParamF[8]._fParam2);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM3,channInfo._chAlarmParam._pThLearnParamF[8]._fParam3);
			pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM4,channInfo._chAlarmParam._pThLearnParamF[8]._fParam4);
			lnDataLength += ((CFixComm*)pFixComm->GetFixComm())->m_nSendSize;
			pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}

	zdlTraceLine(_T("DataSize:%d"),((CFixComm*)pFixComm->GetFixComm()));
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(fwave);
	KD_DeletePoint(pfreqwave);
	KD_DeletePoint(pChannBuff)
	lChann.clear();
	return iRtn;
}

/**得到指定一对振动测点的实时全谱和全谱瀑布图。
*输入参数：
*          公司ID                           FIELD_SYSTEM_COMPANY
*		   分厂名                           FIELD_SYSTEM_FACTORY
*          机组ID                           FIELD_SERVICE_PLANTID
*          轴承位置描述                     FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID               FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID               FIELD_SERVICE_CHANNID_VER
*          波形点数                         FIELD_SERVICE_TIMEWAVE_NUM
*          是否应用新算法                   FIELD_SERVICE_FUNCTION_TYPE
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          时间                             FIELD_SERVICE_TIME
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031
*/
int CTransactData::OnGetFullSpectrum(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	DATATYPE_WAVE *pVerWave=NULL;
	DATATYPE_WAVE *pHorWave=NULL;
	float         *pfVerWave=NULL;
	float         *pfHorWave=NULL;

	double *pVerWaveData=NULL;
	double *pHorWaveData=NULL;

	vector<float> lofVerWave;
	vector<float> lofHorWave;

	int index=-1;
	int iSmpNum=0,iSmpFreq=0;
	float fRev=.0;
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
	string sUnit="";
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	string sLocation=pFixComm->Get(FIELD_SYSTEM_AXISLOCATION);
	string sChannIDHor=pFixComm->GetItem(FIELD_SERVICE_CHANNID_HOR);
	string sChannIDVer=pFixComm->GetItem(FIELD_SERVICE_CHANNID_VER);
	int iWaveNum=pFixComm->GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
	//CString sTemp;sTemp.Format("iWaveNum:%d,sLocation:%s,sChannIDHor:%s,sChannIDVer:%s",iWaveNum,sLocation.c_str(),sChannIDHor.c_str(),sChannIDVer.c_str());
	//CHZLogManage::Debug(sTemp,"CTransactData","GetAxesTrack");
	if (sChannIDVer.empty()||sChannIDHor.empty())
	{
		CAxisChannInfo axisChannInfo=pPlantInfo->GetSpecLocationAxisChannInfo(sLocation);
		sChannIDVer=axisChannInfo._cVerChann;
		sChannIDHor=axisChannInfo._cHorChann;
	}

	//是否按照新的算法FullSpectrumII计算，默认按照FullSpectrum计算
	int lnSigMathType = pFixComm->GetLong(FIELD_SERVICE_FUNCTION_TYPE);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if((sChannIDHor!="")&&(sChannIDVer!=""))
	{
		timeCurr=pPlantInfo->GetRealVibDataDateTime();
		string sUnit;
		S_ChannInfo channInfo;
		S_VibCharValue vibValue;
		channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDHor);
		pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
        iSmpFreq=vibValue._iSmpFreq;
		iSmpNum=vibValue._iSmpNum;
		fRev=(float)vibValue._iRev;
		pPlantInfo->GetRealSmpInfo(iSmpNum,iSmpFreq);
		iWaveNum=iWaveNum>iSmpNum?iSmpNum:iWaveNum;
		if (iWaveNum<=1)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
		pVerWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pHorWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
		lofHorWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);
		lofVerWave.resize(GD_MIDDLEWARE_WAVE_POINT_MAX);

		pfHorWave = &lofHorWave.front();
		pfVerWave = &lofVerWave.front();

		float lfScaleCoef = 10.0;
		int   lnPrecisionEnable = 0;

		float lfMaxValue = 0;
		float lfMinValue = 0;
		
		if ((pPlantInfo->GetSpecChannVibWave(sChannIDHor,pHorWave,iWaveNum,pfHorWave,lfScaleCoef,lnPrecisionEnable,lfMaxValue,lfMinValue)!=-1)&&(pPlantInfo->GetSpecChannVibWave(sChannIDVer,pVerWave,iWaveNum,pfVerWave,lfScaleCoef,lnPrecisionEnable,lfMaxValue,lfMinValue)!=-1))
		{
			if (iWaveNum>1)
			{
				pVerWaveData=new double[iWaveNum];
				pHorWaveData=new double[iWaveNum];
				pPlantInfo->ConvertFloatToDouble(pVerWaveData,pfVerWave,iWaveNum);
				pPlantInfo->ConvertFloatToDouble(pHorWaveData,pfHorWave,iWaveNum);
				//writefile(pHorWaveData,iWaveNum,1);
				//writefile(pVerWaveData,iWaveNum,2);
				double f0 = (double)fRev/60;
				double AHcth=GD_HIGHFCTH,ALcth=GD_LOWFCTH;
				int MHcNum=GD_HIGHFCNUM,MLcNum=GD_LOWFCNUM,iFullSpecInfoSize=0;
				vector<SSigParam> mapFullSpecInfo;
				mapFullSpecInfo.resize(2*(MLcNum+MHcNum));
				//获得全谱信息相关频率和幅值数据
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
						if(lnSigMathType == 1)
						{
							index = calcFullSpecInfo.FullSpectrumII(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,vFullSpecX,vFullSpecY);	
						}
						else
						{
							index = calcFullSpecInfo.FullSpectrum(pVerWaveData,pHorWaveData,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth,mapFullSpecInfo);
						}
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

							//CString sTemp;sTemp.Format("index:%d,iWaveNum:%d,f0:%f,iSmpFreq:%d,MHcNum:%d,AHcth:%f,MLcNum:%d,ALcth:%f",index,iWaveNum,f0,iSmpFreq,MHcNum,AHcth,MLcNum,ALcth);BHTRACE_DEBUG(sTemp);
					if(lnSigMathType == 1)
					{
						iFullSpecInfoSize = vFullSpecX.size();
						if (index>-1)
						{
							for (int iNum =0;iNum<vFullSpecX.size();iNum++)
							{
								++index;
								vFullInfo.push_back(make_pair(vFullSpecX[iNum],vFullSpecY[iNum]));
							}
						}
					}
					else
					{
						iFullSpecInfoSize = mapFullSpecInfo.size();

						if (index > -1)
						{
							for (int iNum = 0; iNum < mapFullSpecInfo.size();iNum++)
							{
								if (mapFullSpecInfo[iNum]._fFreq != 0 && mapFullSpecInfo[iNum]._fAmp != 0)
								{
									++index;

									vFullInfo.push_back(make_pair(mapFullSpecInfo[iNum]._fFreq,mapFullSpecInfo[iNum]._fAmp));
								}
							}
						}
					}
				}
				int iFullInfoSize = vFullInfo.size();

				while ((iFullInfoSize*sizeof(FreqAmp)) >= 90*1000)
				{
					vFullInfo.pop_back();
					iFullInfoSize =vFullInfo.size();
				}

				if (iFullInfoSize > 0)
				{
					pFixComm->SetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)&vFullInfo[0],iFullInfoSize*sizeof(FreqAmp));
					pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,iFullInfoSize);
					timeCurr=pPlantInfo->GetRealVibDataDateTime();
					pFixComm->SetItem(FIELD_SERVICE_TIME,timeCurr);
					pFixComm->SetItem(FIELD_SERVICE_SAMPLE_FREQ,iSmpFreq);
					pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
					//转子旋向
					int iRotation =-1;
					iRotation= pPlantInfo->GetSpecChannsAxisRotation(sChannIDHor,sChannIDVer);
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,iRotation);
					//水平方向数据
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibValue,sizeof(S_VibCharValue));
					//垂直方向数据
					channInfo=pPlantInfo->GetSpecVibChannInfo(sChannIDVer);
					pPlantInfo->GetSpecChannRadialAllChar(channInfo._cChannID,vibValue,sUnit);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *)&vibValue,sizeof(S_VibCharValue));
					pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
				}else
				{
					pFixComm->SetItem(FIELD_CODE,-1);
				}

			}
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_CODE,-1);
		pFixComm->SetItem(FIELD_MESSAGE,"输入参数有空值");
		index= -1;
	}
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pVerWave);
	KD_DeletePoint(pHorWave);
	KD_DeletePoint(pVerWaveData);
	KD_DeletePoint(pHorWaveData);
	return index;
}

/** 获取异常数据检测结果
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *            分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            测点                   FIELD_SYSTEM_CHANN
  *            异常时间               FIELD_SERVICE_TIME
  *            异常类别               FIELD_SYSTEM_TYPE
  *            异常原因               FIELD_SYSTEM_STATUS_DISCRIPTION
  *\description
               异常类别(FIELD_SYSTEM_TYPE)字段类型： GE_ABNORMAL_DATA_CATEGORY
                                     包括(越界数据、趋势波动的转速，不合理数据)
 #define     SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD       20032
  */
int CTransactData::GetAbnormalDataRecord(ICommInterFace *pFixComm,CCustomDB * pDB)
{
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	/*
	Loop Variables:
		c : Company
		f : Factory
		p : Plant
	*/
	string sCompanyName;
	string sFactoryName;
	string sPlantName;

	CFactoryInfoPool * pFactoryInfo = NULL;
	CPlantInfoPool   * pPlantInfo   = NULL;

	CRealPlantInfo   *  pPlant;

	for(int c = 0; c < g_pCompanyPoolReal->GetInfoNum();c++){

		pFactoryInfo =
			g_pCompanyPoolReal->GetSpecInfoPool(c, sCompanyName);

		if (NULL == pFactoryInfo)
		{
			continue;
		}

		for(int f = 0; pFactoryInfo != NULL && f < pFactoryInfo->GetInfoNum(); f++){
			pPlantInfo = pFactoryInfo->GetSpecInfoPool(f, sFactoryName);

			if (NULL == pPlantInfo)
			{
				continue;
			}

			for(int p = 0; pPlantInfo != NULL && p < pPlantInfo->GetInfoNum(); p++){
				pPlant = (CRealPlantInfo *)pPlantInfo->GetSpecInfoPool(p, sPlantName);

				if(pPlant == NULL){
					HZLOG_ERROR(STR("公司[%s]分厂[%s]设备[%s/%s]异常",
						pPlant->GetCompanyName().c_str(),
						pPlant->GetFactoryName().c_str(),
						pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
					continue;
				}

				S_ChannInfo  channInfo;
				CBHDateTime currTime;

				/* 振动测点 */
				CVibChannParam* pVibChann = NULL;

				S_VibCharValue  vibChar;

				vector<float> realRev;
				vector<float> lastRev;

				realRev.resize(PLANT_REV_NUM);
				lastRev.resize(PLANT_REV_NUM);

				if (realRev.size() == 0|| lastRev.size() ==0)
				{
					return -1;
				}

				pPlant->GetRealRev(&realRev[0], PLANT_REV_NUM);
				pPlant->GetLastRev(&lastRev[0], PLANT_REV_NUM);

				int errorType = AbnormalData_IsRevValid(&lastRev[0], &realRev[0], PLANT_REV_NUM, pPlant);
				if(errorType != GE_DATA_VALUE_NO_ERROR){
					currTime = pPlant->GetLastRevTime();

					pFixComm->SetItem(FIELD_SYSTEM_COMPANY,  pPlant->GetCompanyName().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_FACTORY,  pPlant->GetFactoryName().c_str());
					pFixComm->SetItem(FIELD_SERVICE_PLANTID, pPlant->GetPlantID().c_str());
					//pFixComm->SetItem(FIELD_SYSTEM_CHANN, channInfo._cChannID);
					pFixComm->SetItem(FIELD_SERVICE_TIME, currTime);
					pFixComm->SetItem(FIELD_SYSTEM_TYPE,  errorType);

					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,
						AbnormalData_GetLastError().c_str());

					pFixComm->Write( pFixComm->GetAci() );
					pFixComm->CreateNextAnswer();
				}
				
				for(int i = 0; i < pPlant->_vVibChann.size(); i++){
					pVibChann = (CVibChannParam*)pPlant->_vVibChann[i];

					if (NULL == pVibChann)
					{
						continue;
					}

					channInfo = pVibChann->GetChannInfo();

					currTime  = pPlant->GetRealVibDataDateTime();

					string lstrUnit;
					
					pPlant->GetSpecChannRadialAllChar(channInfo._cChannID,vibChar,lstrUnit);
					
					//vibChar   = pVibChann->GetCharValueEx();

					int errorType = AbnormalData_IsCharValid(&vibChar, &channInfo, pPlant);
					pPlant->GetRealVibDataDateTime();

					if(errorType != GE_DATA_VALUE_NO_ERROR){
						pFixComm->SetItem(FIELD_SYSTEM_COMPANY,  pPlant->GetCompanyName().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_FACTORY,  pPlant->GetFactoryName().c_str());
						pFixComm->SetItem(FIELD_SERVICE_PLANTID, pPlant->GetPlantID().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_CHANN, channInfo._cChannID);
						pFixComm->SetItem(FIELD_SERVICE_TIME, currTime);
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,  errorType);

						pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,
							AbnormalData_GetLastError().c_str());

						pFixComm->Write( pFixComm->GetAci() );
						pFixComm->CreateNextAnswer();
					}

				}

				/* 动态测点 */
				CDynChannParam * pDynChann = NULL;


				S_DynCharValue dynChar;

				for(int i = 0; i < pPlant->_vDynChann.size(); i++){
					pDynChann = (CDynChannParam*)pPlant->_vDynChann[i];

					if (NULL == pDynChann)
					{
						continue;
					}

					channInfo =   pDynChann->GetChannInfo();
					//dynChar   = pDynChann->GetCharValueEx();
					
					pPlant->GetSpecDynChannCharValue(channInfo._cChannID,dynChar);

					currTime  = pPlant->GetRealDynDataDateTime();

					int errorType = AbnormalData_IsCharValid(&dynChar, &channInfo, pPlant);
					if(errorType != GE_DATA_VALUE_NO_ERROR){
						pFixComm->SetItem(FIELD_SYSTEM_COMPANY,  pPlant->GetCompanyName().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_FACTORY,  pPlant->GetFactoryName().c_str());
						pFixComm->SetItem(FIELD_SERVICE_PLANTID, pPlant->GetPlantID().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_CHANN, channInfo._cChannID);
						pFixComm->SetItem(FIELD_SERVICE_TIME, currTime);
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,  errorType);

						pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,
							AbnormalData_GetLastError().c_str());

						pFixComm->Write( pFixComm->GetAci() );
						pFixComm->CreateNextAnswer();
					}
				}

				/* 过程量测点 */
				CProcChannParam * pProcChann = NULL;

				float procChar = 0;

				for(int i = 0; i < pPlant->_vProcChann.size(); i++){
					pProcChann = (CProcChannParam*)pPlant->_vProcChann[i];

					if (NULL == pProcChann)
					{
						continue;
					}

					channInfo = pProcChann->GetChannInfo();
					procChar = pProcChann->GetData();

					int errorType = AbnormalData_IsCharValid(procChar, &channInfo, pPlant);
					if(errorType != GE_DATA_VALUE_NO_ERROR){
						pFixComm->SetItem(FIELD_SYSTEM_COMPANY,  pPlant->GetCompanyName().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_FACTORY,  pPlant->GetFactoryName().c_str());
						pFixComm->SetItem(FIELD_SERVICE_PLANTID, pPlant->GetPlantID().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_CHANN, channInfo._cChannID);
						pFixComm->SetItem(FIELD_SERVICE_TIME, currTime);
						pFixComm->SetItem(FIELD_SYSTEM_TYPE,  errorType);

						pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,
							AbnormalData_GetLastError().c_str());

						pFixComm->Write( pFixComm->GetAci() );
						pFixComm->CreateNextAnswer();
					}
				}
			}
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return 0;
}

/**得到监测中心的设备信息表
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SYSTEM_PLANT
*返回的字段码：
*             测点号     FIELD_SYSTEM_CHANN
*             索引       FIELD_SYSTEM_STATUS
*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*             单位       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
*/

int CTransactData::SysGetMachineInfoType(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	SwichDbName(pDB,g_strMainDbName);
	int nCount=0;
	string sCompany  = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory  = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant    = pFixComm->Get(FIELD_SYSTEM_PLANT);

	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany ,sFactory, sPlant);

	if (pPlantInfo==NULL)
	{	
		return -1;
	}

	if (g_pMachIneInfoTypeCollection == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vVibChann.begin();
		 itVib!= pPlantInfo->_vVibChann.end();itVib++)
	{
		CChannInfoBase * pInfo = (CChannInfoBase *)(*itVib);		
		S_ChannInfo loInfo = pInfo->GetChannInfo();

		MachIneInfoTypeCollection::iterator lpit = g_pMachIneInfoTypeCollection->find(loInfo._iInfoType);

		if (lpit!= g_pMachIneInfoTypeCollection->end())
		{
			MachIneInfoTypeVo  & lrefVo = lpit->second;

			for (int i =0;i<lrefVo.size();i++)
			{
				S_Machineinfotype & lrefInfoType = lrefVo[i];

				pFixComm->SetItem(FIELD_SYSTEM_CHANN,loInfo._cChannID);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,lrefInfoType.m_nIndex);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,lrefInfoType.m_strComment);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,lrefInfoType.m_strUnit);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				nCount++;
			}
		}
	}


	for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vDynChann.begin();
		itVib!= pPlantInfo->_vDynChann.end();itVib++)
	{
		CChannInfoBase * pInfo = (CChannInfoBase *)(*itVib);		
		S_ChannInfo loInfo = pInfo->GetChannInfo();

		MachIneInfoTypeCollection::iterator lpit = g_pMachIneInfoTypeCollection->find(loInfo._iInfoType);

		if (lpit!= g_pMachIneInfoTypeCollection->end())
		{
			MachIneInfoTypeVo  & lrefVo = lpit->second;

			for (int i =0;i<lrefVo.size();i++)
			{
				S_Machineinfotype & lrefInfoType = lrefVo[i];

				pFixComm->SetItem(FIELD_SYSTEM_CHANN,loInfo._cChannID);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,lrefInfoType.m_nIndex);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,lrefInfoType.m_strComment);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,lrefInfoType.m_strUnit);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				nCount++;
			}
		}


	}

	for (V_PCHANNINFO::iterator itVib=pPlantInfo->_vProcChann.begin();
		itVib!= pPlantInfo->_vProcChann.end();itVib++)
	{
		CChannInfoBase * pInfo = (CChannInfoBase *)(*itVib);		
		S_ChannInfo loInfo = pInfo->GetChannInfo();

		MachIneInfoTypeCollection::iterator lpit = g_pMachIneInfoTypeCollection->find(loInfo._iInfoType);

		if (lpit!= g_pMachIneInfoTypeCollection->end())
		{
			MachIneInfoTypeVo  & lrefVo = lpit->second;

			for (int i =0;i<lrefVo.size();i++)
			{
				S_Machineinfotype & lrefInfoType = lrefVo[i];

				pFixComm->SetItem(FIELD_SYSTEM_CHANN,loInfo._cChannID);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,lrefInfoType.m_nIndex);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,lrefInfoType.m_strComment);
				pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,lrefInfoType.m_strUnit);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				nCount++;
			}
		}


	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

int CTransactData::GetBodeForTemp( ICommInterFace * pFixComm, CCustomDB * pDB )
{
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	CRealPlantInfo *pSudPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_REAL);
    CSudPlantInfo *pPlantInfo = (CSudPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_SUD);
	if(pSudPlantInfo==NULL ||(pPlantInfo != NULL && pPlantInfo->GetSudFinishStatus() == true))
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	int iChannNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iBytes=pFixComm->GetLong(FIELD_SERVICE_CHANN_BUFFSIZE)+1;
	char *pChannBuff=new char[iBytes];
	ZeroMemory(pChannBuff,iBytes);
	pFixComm->GetItemBuf(FIELD_SERVICE_CHANN_BUFF,pChannBuff,iBytes);
	list<string> lChann;
	list<string>::iterator itChann;
	//解析测点
	lChann=ParseChannList(pChannBuff,iChannNum);
	iChannNum=lChann.size();
	float fRev=0;
	float fPhase=0,fAmp=0;
	int iSmpNum=0,iSmpFreq=0;
	int index=-1;
	int iMicroSecond=0;
	E_SUD_TYPE iType;
	int iStatus=0;
	int iEventID=-1;
	S_VibCharValue vibCharValue;
	string sUnit = "";
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	//	CString strTmp;strTmp.Format("%d",iChannNum);AfxMessageBox(strTmp);
	string sChann;
	static volatile ULONGLONG lnMaxDateTime = 0;
	if (iChannNum==0)
	{
		iChannNum=pSudPlantInfo->GetRadialVibChannNum();
		for (int loop=0;loop<iChannNum;++loop)
		{
			if (pSudPlantInfo->GetSpecChannIndex(loop,sChann,fRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType)!=-1)
			{
				iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
				pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
				pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

				SYSTEMTIME loSystemTime = {0};

				int lnIMicroSecond = 0;

				pSudPlantInfo->GetRealVibDataDateTime().GetAsSystemTime(loSystemTime);

				if(iStatus>0)
				{
					FILETIME loFileTime = {0};

					SystemTimeToFileTime(&loSystemTime,&loFileTime);			

					ULARGE_INTEGER lnTemp;

					lnTemp.HighPart = loFileTime.dwHighDateTime;

					lnTemp.LowPart = loFileTime.dwLowDateTime;

					if (lnTemp.QuadPart >= lnMaxDateTime)
					{
						lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
					}else
					{
						iStatus = 0;
					}
				}

				if (iStatus>0)
				{
					pSudPlantInfo->GetSpecChannRadialAllChar( sChann, vibCharValue, sUnit);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,sChann.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
					pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
					pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
					pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
					pFixComm->SetItem(FIELD_SERVICE_TIME,pSudPlantInfo->GetRealVibDataDateTime());
					pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibCharValue,sizeof(S_VibCharValue));
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++index;
			}
		}
	}
	else
	{
		for (itChann=lChann.begin();itChann!=lChann.end();++itChann)
		{//AfxMessageBox("ds");
			if (pSudPlantInfo->GetSpecChannIndex((*itChann),fRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType)!=-1)
				//	if (1)
			{//AfxMessageBox("ds2");
				iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
				pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
				pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

				SYSTEMTIME loSystemTime = {0};

				int lnIMicroSecond = 0;

				pSudPlantInfo->GetRealVibDataDateTime().GetAsSystemTime(loSystemTime);

				if (iStatus >0)
				{
					FILETIME loFileTime = {0};

					SystemTimeToFileTime(&loSystemTime,&loFileTime);			

					ULARGE_INTEGER lnTemp;

					lnTemp.HighPart = loFileTime.dwHighDateTime;

					lnTemp.LowPart = loFileTime.dwLowDateTime;

					if (lnTemp.QuadPart >= lnMaxDateTime)
					{
						lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
					}else
					{
						iStatus = 0;
					}
				}
				if (iStatus>0)
				{
					pSudPlantInfo->GetSpecChannRadialAllChar((*itChann), vibCharValue, sUnit);
					pFixComm->SetItem(FIELD_SERVICE_CHANN1,itChann->c_str());
					pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
					pFixComm->SetItem(FIELD_SERVICE_REV,fRev);
					pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
					pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
					pFixComm->SetItem(FIELD_SERVICE_TIME,pSudPlantInfo->GetRealVibDataDateTime());
					pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
					pFixComm->SetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char*)&vibCharValue,sizeof(S_VibCharValue));
					//CString strTemp;
					//strTemp.Format("%.2f,1X:%.2f",fRev,vibCharValue._fCharAll);
					//CHZLogManage::Debug(strTemp,"CTransactData","GetBode");
				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				++index;
			}
		}
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	KD_DeletePoint(pChannBuff);
	lChann.clear();
	return index;
}


/**得到临时在线指定指定机组和指定可以做启停车的测点瀑布图，一次只能得到一组波形频谱
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点号           FIELD_SERVICE_CHANN1
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*          相位：           FIELD_SERVICE_TREND_ONECHAR
*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
*          转速：           FIELD_SERVICE_REV
*          频谱数据         FIELD_SERVICE_FREQWAVE1
*          频谱数据个数     FIELD_SERVICE_FREQWAVE_NUM
*          频谱DF           FIELD_SERVICE_FREQWAVE_DF
*          时间             FIELD_SERVICE_TIME
*          对应时间的微秒   FIELD_SERVICE_MICROSECOND
*          开停车状态       FIELD_SERVICE_SUD_STATUS
*          开停车类型       FIELD_SERVICE_SUD_TYPE
*          启停车事件ID     FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_GETWATERFALL            20019
*/
int CTransactData::GetWaterFallForTemp(ICommInterFace * pFixComm, CCustomDB * pDB)
{
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlantID=pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sChann=pFixComm->GetItem(FIELD_SERVICE_CHANN1);
	int iLines=pFixComm->GetLong(FIELD_SERVICE_ZOOM_LINES);
	int iCharType=pFixComm->GetLong(FIELD_SERVICE_TREND_CHARTYPE);
	iLines=iLines<199?401:iLines;

	CRealPlantInfo *pSudPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_REAL);
	CSudPlantInfo *pPlantInfo = (CSudPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlantID,PLANTINFO_SUD);
	
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	
   if((pSudPlantInfo==NULL ) || (pPlantInfo != NULL && pPlantInfo->GetSudFinishStatus() == true))
    { 
        pFixComm->Write(pFixComm->GetAci());
        return -1;
    }

	float iRev=0;
	int iSmpNum=0,iSmpFreq=0;
	int iRealNum=(iLines-1)*2.56;
	float fAmp=.0,fPhase=.0;
	DATATYPE_WAVE *pfFreqWave=NULL,*pfWave=NULL;
	float *lpfWave = NULL,*lpfFreq = NULL;
	float df=.0;
	int index=pSudPlantInfo->GetSpecChannIndex(sChann,iRev,fPhase,fAmp,iSmpNum,iSmpFreq,iCharType);
//	int index=0;
	E_SUD_TYPE iType;
	int iStatus=0;
	int iEventID=-1;
	int iMicroSecond=0;
	//CString strTmp;strTmp.Format("%d,%d,%f,%f,%f,%d",iEventID,iLines,iRev,fAmp,fPhase,pSudPlantInfo->GetCurrDataTime(iMicroSecond).GetYear());AfxMessageBox(strTmp);

	if (index==-1)
		goto GETWATERFALL_CLEAN;
	iStatus=(pSudPlantInfo->GetStatusType(iType,iEventID))?1:0;
	pFixComm->SetItem(FIELD_SERVICE_SUD_STATUS,iStatus);
	pFixComm->SetItem(FIELD_SERVICE_SUD_TYPE,iType);

	static volatile ULONGLONG lnMaxDateTime = 0;

	SYSTEMTIME loSystemTime = {0};

	int lnIMicroSecond = 0;

	pSudPlantInfo->GetRealVibDataDateTime().GetAsSystemTime(loSystemTime);
	
	/*
	if(iStatus>0)
		{
			FILETIME loFileTime = {0};
	
			SystemTimeToFileTime(&loSystemTime,&loFileTime);			
	
			ULARGE_INTEGER lnTemp;
	
			lnTemp.HighPart = loFileTime.dwHighDateTime;
	
			lnTemp.LowPart = loFileTime.dwLowDateTime;
	
			if (lnTemp.QuadPart >= lnMaxDateTime)
			{
				lnMaxDateTime = (volatile LONGLONG)lnTemp.QuadPart;			
			}else
			{
				iStatus = 0;
			}
		}*/
	
	if (iStatus>0)
	{
		float lfScaleCoef = 10.0;
		int lnPrecisionEnabled = 0;

		iRealNum=iSmpNum>iRealNum?iRealNum:iSmpNum;
		if (iRealNum<256)
			goto GETWATERFALL_CLEAN;
		else
		{
			if (iRealNum/2 <=0)
			{
				return -1;
			}
			pfFreqWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			pfWave=new DATATYPE_WAVE[GD_MIDDLEWARE_WAVE_POINT_MAX];
			lpfWave=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			lpfFreq=new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
			if (pSudPlantInfo->GetSpecChannVibWaveAndFreq(sChann,pfWave,iRealNum,pfFreqWave,iLines,df,lfScaleCoef,lnPrecisionEnabled,lpfWave,lpfFreq)==-1)
				goto GETWATERFALL_CLEAN;
		}
		pFixComm->SetItem(FIELD_SERVICE_REV,iRev);
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
		pFixComm->SetItem(FIELD_SERVICE_TREND_ONECHAR,fPhase);
		pFixComm->SetItem(FIELD_SERVICE_TREND_OVERALLCHAR,fAmp);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_NUM,iLines);
		pFixComm->SetItem(FIELD_SERVICE_FREQWAVE_DF,df);
		pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)pfFreqWave,(iLines)*sizeof(DATATYPE_WAVE));
	    pFixComm->SetItem(FIELD_SERVICE_TIME,pSudPlantInfo->GetRealVibDataDateTime());
		pFixComm->SetItem(FIELD_SERVICE_MICROSECOND,iMicroSecond);
		pFixComm->SetItem(FIELD_SERVICE_WAVE_COEF,lfScaleCoef);
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO,lnPrecisionEnabled);
		if(lnPrecisionEnabled > 0)
		{
			pFixComm->SetItemBuf(FIELD_SERVICE_SEND_FLOAT_WAVE1,(char *)lpfWave,iLines*sizeof(float));
			pFixComm->SetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)lpfFreq,iLines*sizeof(float));
		}
//		CString strTmp;
//		strTmp.Format("%d,%d,%f,%f,%f,%d",iEventID,iLines,iRev,fAmp,fPhase,pSudPlantInfo->GetCurrDataTime(iMicroSecond).GetYear());AfxMessageBox(strTmp);
	}
GETWATERFALL_CLEAN:
	pFixComm->Write(pFixComm->GetAci());
	if (pfFreqWave!=NULL)
		delete [] pfFreqWave;
	if (pfWave!=NULL)
	    delete [] pfWave;
	KD_DeletePoint(lpfWave);
	KD_DeletePoint(lpfFreq);
	return index;
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
  *#define    SERVICE_SERVICE_ON_SAVESPECTIMEDATA    21080
  */
int CTransactData::SaveSpecTypeDataByTimeRegion(ICommInterFace *pFixComm, IDBInterFace *pDB)
{
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	if (pPlantInfo==NULL)
	{
		lpFixCom->CreateAnswer(lpFixCom->GetFunc());
		lpFixCom->SetItem(FIELD_SYSTEM_STATUS,0);
		lpFixCom->Write(lpFixCom->GetAci());
		lpFixCom->CreateEndMak();
		return -1;
	}

	//2.切换到分库


	CString lstrSuffix ;

	lstrSuffix.Format(_T("%s%s"),pPlantInfo->GetCompanyAlias().c_str(),pPlantInfo->GetPlantNo().c_str());

	SwitchDbNameEx(pDB,lstrSuffix);

	int lnChannType = lpFixCom->GetLong(FIELD_SYSTEM_CHANN_TYPE);

	CBHDateTime loStart = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CBHDateTime loEnd = lpFixCom->GetTime(FIELD_SERVICE_OFF_TIMEEND);

	CString lstrTable,lstrSaveTable;

	E_TREND_TBL_TYPE lenTableType = GE_TREND_TBLTYPE_DAY,lenSaveTblType;

	BOOL lbRet = TRUE;

	int lnTotalUpdatedCount = 0;

	BOOL lbRecordExist = FALSE;
	
	while(lenTableType > GE_TREND_TBLTYPE_UNKNOWN)
	{
		lstrTable.Format("%s%s",GetTblNamePrefix((CHANNTYPE)lnChannType, lenTableType),lstrSuffix);

		CString lstrSql;

		pDB->Cmd("select top 1 %s from %s where (%s between '%s' and '%s') and %s >= %d",
			gc_cTime,lstrTable,gc_cTime,loStart.Format(gc_cTimeFormat),loEnd.Format(gc_cTimeFormat),gc_cID,0);
		if(pDB->More())
		{
			
			lstrSaveTable = lstrTable;
			lbRecordExist = true;
			lenSaveTblType = lenTableType;
			std::pair<CBHDateTime,CBHDateTime> lpTime(loStart,loEnd);
			pPlantInfo->SaveSpecTime(pDB,lpTime,lstrSaveTable,0);
			
		}
		lenTableType = (E_TREND_TBL_TYPE)((int)lenTableType - 1);
	}


	lpFixCom->CreateAnswer(lpFixCom->GetFunc());

	lpFixCom->SetItem(FIELD_SYSTEM_STATUS,lbRet);

	lpFixCom->Write(lpFixCom->GetAci());

	return lbRet;
}

/*! 通过测点类型和表类型确定表前缀
 \param[in]  iChannType_: 振动 动态 过程量
 \param[in]  iTblType_:   月，日，时，秒
 \return 表前缀
 */
CString CTransactData::GetTblNamePrefix(const CHANNTYPE &iChannType_, E_TREND_TBL_TYPE &iTblType_/* =GE_TREND_TBLTYPE_SECOND */)
{
	CString strSuffix="";


	switch(GetChannOneType(iChannType_))
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

/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
	*输入参数：  
   *         公司名             FIELD_SYSTEM_COMPANY
   *         分厂名             FIELD_SYSTEM_FACTORY
   *         机组名             FIELD_SYSTEM_PLANT
   *         测点名             FIELD_SYSTEM_CHANN
   *         测点类型：         FIELD_SERVICE_CHANNTYPE
   *         是否常规报警       FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
   *         报警限类型         FIELD_SYSTEM_ALARM_TYPE
   *         报警上限           FIELD_SYSTEM_ALARM_HIGH
   *         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
   *         报警下限           FIELD_SYSTEM_ALARM_LOW
   *         报警下下限         FIELD_SYSTEM_ALARM_LOW2
   *         修改报警限主机名   FIELD_SYSTEM_HOST_NAME
   *         修改报警限主机IP   FIELD_SYSTEM_CLIENT_IP
   *         修改报警限用户名   FIELD_SYSTEM_USER_NAME
 
   *返回值的字段码：
   *         返回状态       FIELD_SYSTEM_STATUS
   #define   SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION 20044
*/

int CTransactData::ChangeSpecChannAlarmLimitation( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	SwitchDbNameEx(pDB,g_strMainDbName);

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	CRealPlantInfo *pRealPlantInfo = (CRealPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if (NULL == pAlarmPlantInfo || NULL == pRealPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	string sChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN);

	string sHostName = pFixComm->GetItem(FIELD_SYSTEM_HOST_NAME);

	string sClientIP = pFixComm->GetItem(FIELD_SYSTEM_CLIENT_IP);

	string sUserName = pFixComm->GetItem(FIELD_SYSTEM_USER_NAME);

	CChannAlarmParam lchannAlarmParam;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	bool lbJudgeAlarm = (bool)pFixComm->GetLong(FIELD_SERVICE_DATA_TYPE);

	int  lnAlarmType  = pFixComm->GetLong(FIELD_SERVICE_ALARM_TYPE);

	double ldbAlarmHighHigh = pFixComm->GetDouble(FIELD_SYSTEM_ALARM_HIGH2);

	double ldbAlarmHigh     = pFixComm->GetDouble(FIELD_SYSTEM_ALARM_HIGH);

	double ldbAlarmLow      = pFixComm->GetDouble(FIELD_SYSTEM_ALARM_LOW);

	double ldbAlarmLowLow   = pFixComm->GetDouble(FIELD_SYSTEM_ALARM_LOW2);

	string lstrAlarmMode;

	switch(lnAlarmType)
	{
	case E_ALARMCHECK_LOWPASS:
		lstrAlarmMode = "低通";
		break;
	case E_ALARMCHECK_HIGHPASS:
		lstrAlarmMode = "高通";
		break;
	case E_ALARMCHECK_BANDPASS:
		lstrAlarmMode = "带通";
		break;
	case E_ALARMCHECK_BANDSTOP:
		lstrAlarmMode = "带阻";
		break;
	default:
		lstrAlarmMode = "低通";

	}

	int lnIndexAlarm = -1,lnIndexReal = -1;

	int lnResult = 0;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cAlarmParam,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",\
		gc_cCompany,sCompany.c_str(),gc_cPlantNo,pRealPlantInfo->GetPlantNo().c_str(),\
		gc_cChannID,sChannID.c_str());
	if (pDB->More(TRUE))
	{
		long nSize = sizeof(CChannAlarmParam);
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cAlarmParam,  (BYTE*)&lchannAlarmParam,  nSize, sWaveHead);
		lchannAlarmParam._bRegular = lbJudgeAlarm;
		lchannAlarmParam._iRegJudgeType = lnAlarmType;
		lchannAlarmParam._fAlarmHH = ldbAlarmHighHigh;
		lchannAlarmParam._fAlarmHL = ldbAlarmHigh;
		lchannAlarmParam._fAlarmLH = ldbAlarmLow;
		lchannAlarmParam._fAlarmLL = ldbAlarmLowLow;
		if(!pDB->SetBuffer(gc_cAlarmParam,(BYTE*)&lchannAlarmParam,nSize,false))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		
		Operations::Log(STR("服务器:%s,%s用户:%s修改%s公司%s分厂%s设备%s报警限。是否报警:%d,报警方式:%s,高高限:%.2f,高限:%.2f,下限:%.2f,下下限:%.2f",
			sHostName.c_str(), sClientIP.c_str(), sUserName.c_str(),sCompany.c_str(),
			sFactory.c_str(),sPlant.c_str(),sChannID.c_str(),lbJudgeAlarm,lstrAlarmMode.c_str(),
			ldbAlarmHighHigh,ldbAlarmHigh,ldbAlarmLow,ldbAlarmLowLow));
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecVibChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecVibChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->ChangeVibAlarmLimitation(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->ChangeVibAlarmLimitation(lnIndexReal,lchannAlarmParam);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecDynChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecDynChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->ChangeDynAlarmLimitation(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->ChangeDynAlarmLimitation(lnIndexReal,lchannAlarmParam);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_PROC)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecProcChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecProcChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->ChangeProcAlarmLimitation(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->ChangeProcAlarmLimitation(lnIndexReal,lchannAlarmParam);

	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());
	return 0;
}


/**获取指定设备指定测点的报警信息
*输入参数：  
*         公司名         FIELD_SYSTEM_COMPANY
*         分厂名         FIELD_SYSTEM_FACTORY
*         机组名         FIELD_SERVICE_PLANTID
*         测点名         FIELD_SYSTEM_CHANN
*         测点类型：     FIELD_SERVICE_CHANNTYPE
*返回值的字段码：
*         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
*         报警限类型     FIELD_SYSTEM_ALARM_TYPE
*         报警上限           FIELD_SYSTEM_ALARM_HIGH
*         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
*         报警下限           FIELD_SYSTEM_ALARM_LOW
*         报警下下限         FIELD_SYSTEM_ALARM_LOW2
*         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
#define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045
*/
int CTransactData::GetSpecChannAlarmLimitation( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	ICommInterFace * lpFixCom = pFixComm;

	ASSERT(lpFixCom);

	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	if (NULL == pAlarmPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	string sChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN);

	CChannAlarmParam lchannAlarmParam;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	int lnIndexAlarm = -1;

	int lnResult = FALSE;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecVibChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetVibAlarmLimitation(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecDynChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetDynAlarmLimitation(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_PROC)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecProcChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetProcAlarmLimitation(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	lnResult = TRUE;

	pFixComm->SetItem(FIELD_SERVICE_DATA_TYPE, lchannAlarmParam._bRegular);
	pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE, lchannAlarmParam._iRegJudgeType);
	pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2, lchannAlarmParam._fAlarmHH);
	pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH, lchannAlarmParam._fAlarmHL);
	pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW, lchannAlarmParam._fAlarmLH);
	pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2, lchannAlarmParam._fAlarmLL);

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());

	return 0;
}

typedef std::vector<S_AlarmState> AlarmStateList_t;
typedef std::vector<S_AlarmStateEx> AlarmStateExList_t;

struct S_PlantStatus
{
	CString      _sCompany;
	CString      _sFactory;
	CString      _sPlant;

	int          _nAlarmID;
	AlarmStateExList_t  _VibAlarmStatus;
	AlarmStateExList_t  _DynAlarmStatus;
	AlarmStateExList_t  _ProcAlarmStatus;
	AlarmStateList_t    _AxisPairAlarmStatus;
	
	int          _nSUDStatus;
	int          _nSUDType;

	int Parse( char* apBuffer, int anLength, CString &asError )
	{
		ASSERT(apBuffer);
		ASSERT(anLength > 0);

		char* lpBuffer = apBuffer;
		const char* lpEnd = lpBuffer + anLength;

		int lnLength = 0;

		{
			lnLength = *(BYTE*)lpBuffer;
			lpBuffer++;

			if (lnLength < 1) {
				ASSERT(FALSE);
				asError = STR("公司名长度错误: %d", lnLength);
				return -5;
			}

			if (lpBuffer + lnLength >= lpEnd) {
				ASSERT(FALSE);
				asError = _T("公司名长度越界");
				return -6;
			}

			this->_sCompany = CString(lpBuffer, lnLength);
			lpBuffer += lnLength;
		}

		{
			lnLength = *(BYTE*)lpBuffer;
			lpBuffer++;

			if (lnLength < 1) {
				ASSERT(FALSE);
				asError = STR("分厂名长度错误: %d", lnLength);
				return -7;
			}

			if (lpBuffer + lnLength >= lpEnd) {
				ASSERT(FALSE);
				asError = _T("分厂名长度越界");
				return -8;
			}

			this->_sFactory = CString(lpBuffer, lnLength);
			lpBuffer += lnLength;
		}

		{
			lnLength = *(BYTE*)lpBuffer;
			lpBuffer++;

			if (lnLength < 1) {
				ASSERT(FALSE);
				asError = STR("设备名长度错误: %d", lnLength);
				return -9;
			}

			if (lpBuffer + lnLength >= lpEnd) {
				ASSERT(FALSE);
				asError = _T("设备名长度越界");
				return -10;
			}

			this->_sPlant   = CString(lpBuffer, lnLength);
			lpBuffer += lnLength;
		}

		this->_nAlarmID     = *(int*)lpBuffer;
		lpBuffer+=sizeof(int);

		int lnChannCount = 0;

		{
			lnChannCount = *(int*)lpBuffer;
			lpBuffer+=sizeof(int);

			if (lnChannCount < 0) {
				ASSERT(FALSE);
				asError = STR("振动测点数目错误(%d)", lnChannCount);
				return -11;
			}

			if (lnChannCount > 0) {
				lnLength = lnChannCount*sizeof(S_AlarmStateEx);
				if (lpBuffer + lnLength >= lpEnd) {
					ASSERT(FALSE);
					asError = _T("振动测点报警状态越界");
					return -12;
				}

				this->_VibAlarmStatus.resize(lnChannCount);
				memcpy(&this->_VibAlarmStatus[0], lpBuffer, lnLength);
				lpBuffer+= lnLength;
			}
		}

		{
			lnChannCount = *(int*)lpBuffer;
			lpBuffer+=sizeof(int);

			if (lnChannCount < 0) {
				ASSERT(FALSE);
				asError = STR("动态测点数目错误(%d)", lnChannCount);
				return -13;
			}

			if (lnChannCount > 0) {
				lnLength = lnChannCount*sizeof(S_AlarmStateEx);
				if (lpBuffer + lnLength >= lpEnd) {
					ASSERT(FALSE);
					asError = _T("动态测点报警状态越界");
					return -14;
				}

				this->_DynAlarmStatus.resize(lnChannCount);
				memcpy(&this->_DynAlarmStatus[0], lpBuffer, lnLength);
				lpBuffer+= lnLength;
			}
		}

		{
			lnChannCount = *(int*)lpBuffer;
			lpBuffer+=sizeof(int);

			if (lnChannCount < 0) {
				ASSERT(FALSE);
				asError = STR("过程量测点数目错误(%d)", lnChannCount);
				return -15;
			}

			if (lnChannCount > 0) {
				lnLength = lnChannCount*sizeof(S_AlarmStateEx);
				if (lpBuffer + lnLength >= lpEnd) {
					ASSERT(FALSE);
					asError = _T("过程量测点报警状态越界");
					return -16;
				}

				this->_ProcAlarmStatus.resize(lnChannCount);
				memcpy(&this->_ProcAlarmStatus[0], lpBuffer, lnLength);
				lpBuffer+= lnLength;
			}
		}

		{
			lnChannCount = *(int*)lpBuffer;
			lpBuffer+=sizeof(int);

			if (lnChannCount < 0) {
				ASSERT(FALSE);
				asError = STR("测点对数目错误(%d)", lnChannCount);
				return -17;
			}

			if (lnChannCount > 0) {
				lnLength = lnChannCount*sizeof(S_AlarmState);
				if (lpBuffer + lnLength >= lpEnd) {
					ASSERT(FALSE);
					asError = _T("测点对报警状态越界");
					return -18;
				}

				this->_AxisPairAlarmStatus.resize(lnChannCount);
				memcpy(&this->_AxisPairAlarmStatus[0], lpBuffer, lnLength);
				lpBuffer+= lnLength;
			}
		}

		this->_nSUDStatus  = *(int*)lpBuffer;
		lpBuffer+=sizeof(int);

		this->_nSUDType  = *(int*)lpBuffer;
		lpBuffer+=sizeof(int);

		return lpBuffer - apBuffer;
	}

	void Dump()
	{
		BHLOG_DEBUG(STR("公司=%s 分厂=%s 设备=%s 报警ID=%d 启停机状态=%d",
			this->_sCompany,
			this->_sFactory,
			this->_sPlant,
			this->_nAlarmID,
			this->_nSUDStatus));
	}
};

typedef std::vector<S_PlantStatus> PlantStatusList_t;


namespace PlantStatus
{
	enum E_PLANT_STATUS_PROTOCOL_VERSION {
		VERSION_SYNC_PLANT_STATUS = 1,
	};

	int Parse(ICommInterFace * pFixComm, OUT PlantStatusList_t& aPlantStatusList, CString& asError)
	{
		 ASSERT(pFixComm);

		long lnPlantCount = pFixComm->GetLong(FIELD_SERVICE_PLANT_NUM);

		if (lnPlantCount < 1) {
			asError = "设备数目不正确";
			return -2;
		}

		int lnLength = pFixComm->GetLong(FIELD_SERVICE_LEN);
		if (lnLength < 1) {
			asError = "状态数据长度错误";
			return -3;
		}

		/*
		FIELD_SYSTEM_STATUS Format Specification:
		FIELD_SYSTEM_STATUS := 设备状态+
		设备状态 := 公司名 分厂名 设备名 报警状态 启停机状态
		报警状态 := AlarmID 振动测点报警状态 动态测点报警状态 过程量测点报警状态 测点对报警状态
		启停机状态 := SudStatus SudType
		测点报警状态 := 数目 数据
		名:= 长度 数据
		*/

		std::vector<char> loBuffer(lnLength);
		if(!pFixComm->GetItemBuf(FIELD_SYSTEM_STATUS, &loBuffer[0], lnLength))
		{
			asError = _T("读取状态数据失败");
			return -4;
		}

		aPlantStatusList.reserve(lnPlantCount);

		char* lpBuffer = &loBuffer[0];

		S_PlantStatus loPlantAlarmSudStatus;

		while(lnLength > 0)
		{
			int lnSize = loPlantAlarmSudStatus.Parse(lpBuffer, lnLength, asError);

			if (lnSize < 0) {
				return lnSize;
			}

			aPlantStatusList.push_back(loPlantAlarmSudStatus);

			lpBuffer += lnSize;
			lnLength -= lnSize;
		}

		 if(lpBuffer != (&loBuffer[0] + loBuffer.size()))
		 {
			 asError = _T("状态数据格式错误");
			 return -5;
		 }

		return TRUE;
	}
}

int CTransactData::SkipSpecPlantStatus(S_PlantStatus* apStatus, CString& asError)
{
	ASSERT(apStatus);

	string sCompany = apStatus->_sCompany;
	string sFactory = apStatus->_sFactory;
	string sPlant   = apStatus->_sPlant;

	CAlarmPlantInfo *lpPlantInfo = (CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany, sFactory, sPlant, PLANTINFO_ALARM);
	if (NULL == lpPlantInfo)
	{
		ASSERT(FALSE);
		return 0;
	}

	bool lbSudStatus = apStatus->_nSUDStatus;
	int  lnSudType   = apStatus->_nSUDType;

	lpPlantInfo->SetSudStatus(lbSudStatus, (E_SUD_TYPE)lnSudType);

	if (lpPlantInfo->_iSyncID >= 0)
	{
		lpPlantInfo->_iSyncID = -2;
	}

	return 1;
}

/*
#define   SERVICE_SERVICE_ON_SET_PLANT_STATUS 24041
*/
int CTransactData::SetSpecPlantStatus(S_PlantStatus* apStatus, CString& asError)
{
	ASSERT(apStatus);

	string sCompany = apStatus->_sCompany;
	string sFactory = apStatus->_sFactory;
	string sPlant   = apStatus->_sPlant;

	CAlarmPlantInfo *lpPlantInfo = (CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany, sFactory, sPlant, PLANTINFO_ALARM);
	if (NULL == lpPlantInfo)
	{
		return 0;
	}

	bool lbSudStatus = apStatus->_nSUDStatus;
	int  lnSudType   = apStatus->_nSUDType;

	lpPlantInfo->SetSudStatus(lbSudStatus, (E_SUD_TYPE)lnSudType);

	int iVibNum = apStatus->_VibAlarmStatus.size();
	if(iVibNum > 0)
	{
		lpPlantInfo->SetVibChannStateEx((S_AlarmStateEx*)&apStatus->_VibAlarmStatus[0], iVibNum);
	}

	int iDynNum = apStatus->_DynAlarmStatus.size();
	if (iDynNum > 0)
	{
		lpPlantInfo->SetDynChannStateEx((S_AlarmStateEx*)&apStatus->_DynAlarmStatus[0], iDynNum);
	}

	int iProcNum = apStatus->_ProcAlarmStatus.size();
	if(iProcNum > 0)
	{
		lpPlantInfo->SetProcChannStateEx((S_AlarmStateEx*)&apStatus->_ProcAlarmStatus[0], iProcNum);
	}

	int iAxisPairNum = apStatus->_AxisPairAlarmStatus.size();
	if (iAxisPairNum>0)
	{
		lpPlantInfo->SetAxisLocationState((S_AlarmState*)&apStatus->_AxisPairAlarmStatus[0], iAxisPairNum);
	}

	lpPlantInfo->_iSyncID = apStatus->_nAlarmID;

	return iVibNum + iDynNum + iProcNum + iAxisPairNum;
}

int CTransactData::SetPlantStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	PlantStatusList_t loPlantStatusList;

	int lnResult = -1;
	CString lsError;

	long lnVersion = pFixComm->GetLong(FIELD_SYSTEM_VERSION);

	if (PlantStatus::VERSION_SYNC_PLANT_STATUS == lnVersion) {
		lnResult = PlantStatus::Parse(pFixComm, loPlantStatusList, lsError);
	} else {
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -1);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, _T("状态数据同步版本不支持"));
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	if (lnResult < 0) {
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, lnResult);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, lsError);
		pFixComm->Write(pFixComm->GetAci());

		return lnResult;
	}

	if (isLocalJudgeAlarm())
	{
		for (PlantStatusList_t::iterator it = loPlantStatusList.begin();
			it != loPlantStatusList.end();
			++it)
		{
			SkipSpecPlantStatus(&*it, lsError);
		}

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		return -2;
	}

	int lnCount = 0;

	for (PlantStatusList_t::iterator it = loPlantStatusList.begin();
		it != loPlantStatusList.end();
		++it) 
	{
		lnResult = SetSpecPlantStatus(&*it, lsError);
		if(lnResult < 0)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, lnResult);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, lsError);
			pFixComm->Write(pFixComm->GetAci());

			return lnResult;
		}

		if (lnResult > 0)
		{
			lnCount++;
		}
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, lnCount);
	pFixComm->Write(pFixComm->GetAci());

	return lnCount;
}

/*
#define   SERVICE_SERVICE_ON_GET_PLANT_STATUS 24042
*/
int CTransactData::GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CAlarmPlantInfo* apPlantInfo)
{
	ASSERT(apPlantInfo);

	AlarmStateExList_t loStatusBuffer;

	long lnEventID = -1;
	long lnSyncStatus = -1;


	GetAlarmEventID(apPlantInfo,lnEventID,lnSyncStatus);

	apFixComm->SetItem(FIELD_SYSTEM_COMPANY, apPlantInfo->GetCompanyName().c_str());
	apFixComm->SetItem(FIELD_SYSTEM_FACTORY, apPlantInfo->GetFactoryName().c_str());
	apFixComm->SetItem(FIELD_SERVICE_PLANTID, apPlantInfo->GetPlantID().c_str());

	apFixComm->SetItem(FIELD_SERVICE_ALARMID, lnEventID);

	int lnChannCount = apPlantInfo->GetVibChannNum();
	if (lnChannCount > 0) {
		loStatusBuffer.resize(lnChannCount);

		S_AlarmStateEx *lpState = &loStatusBuffer[0];

		apPlantInfo->GetVibChannStateEx(lpState, lnChannCount);
		if(lnChannCount > 0)
		{
			apFixComm->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM, lnChannCount);
			apFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLVIBSTATE, (char*)lpState, lnChannCount*sizeof(S_AlarmStateEx));
		}
	}

	lnChannCount = apPlantInfo->GetDynChannNum();
	if (lnChannCount > 0) {
		loStatusBuffer.resize(lnChannCount);

		S_AlarmStateEx *lpState = &loStatusBuffer[0];

		apPlantInfo->GetDynChannStateEx(lpState, lnChannCount);
		if (lnChannCount > 0)
		{
			apFixComm->SetItem(FIELD_SERVICE_ON_SEND_DYNCHANNNUM, lnChannCount);
			apFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLDYNSTATE, (char*)lpState, lnChannCount*sizeof(S_AlarmStateEx));
		}
	}

	lnChannCount = apPlantInfo->GetProcChannNum();
	if (lnChannCount > 0) {
		loStatusBuffer.resize(lnChannCount);
		S_AlarmStateEx *lpState = &loStatusBuffer[0];

		apPlantInfo->GetProcChannStateEx(lpState, lnChannCount);
		if (lnChannCount > 0)
		{
			apFixComm->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM, lnChannCount);
			apFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCSTATE, (char*)lpState, lnChannCount*sizeof(S_AlarmStateEx));
		}
	}


	lnChannCount=apPlantInfo->GetAxisPairNum();
	if(lnChannCount>0)
	{
		AlarmStateList_t loAxisBuffer;

		loAxisBuffer.resize(lnChannCount);
		S_AlarmState *lpState = &loAxisBuffer[0];
		apPlantInfo->GetAxisLocationState(lpState,lnChannCount);

		if (lnChannCount > 0)
		{
			apFixComm->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,lnChannCount);
			apFixComm->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE,(char*)lpState,lnChannCount*sizeof(S_AlarmState));
		}
	}

	int lnSudType = -1;
	bool lbSudStatus = false;

	lnSudType = apPlantInfo->GetSudStatus(lbSudStatus);

	apFixComm->SetItem(FIELD_SERVICE_SUD_STATUS, lbSudStatus);
	apFixComm->SetItem(FIELD_SERVICE_SUD_TYPE, lnSudType);

	apFixComm->Write(apFixComm->GetAci());
	apFixComm->CreateNextAnswer();

	return 1;
}

int CTransactData::GetSpecPlantStatus(ICommInterFace*  apFixComm, 
									  IDBInterFace* apDB,
									  CPlantInfoPool* apPlantPool)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apPlantPool);

	int lnCount = 0;

	CAlarmPlantInfo * lpPlantInfo = NULL;

	std::string lsPlant;

	for (int i = 0; i < apPlantPool->GetInfoNum(); ++i)
	{
		lpPlantInfo = (CAlarmPlantInfo *)apPlantPool->GetSpecInfoPool(i, lsPlant);
		if (NULL == lpPlantInfo) {
			ASSERT(FALSE);
			return -1;
		}

		int lnResult = GetSpecPlantStatus(apFixComm, apDB, lpPlantInfo);
		if (lnResult < 1) {
			return lnResult;
		}

		++lnCount;
	}

	return lnCount;
}


int CTransactData::GetSpecPlantStatus(ICommInterFace*  apFixComm, 
									  IDBInterFace* apDB,
									  CPlantInfoPool* apPlantPool,
									  const std::string& asPlant)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apPlantPool);


	CAlarmPlantInfo * lpPlantInfo = NULL;

	lpPlantInfo = (CAlarmPlantInfo *)apPlantPool->GetSpecInfoPool(asPlant);
	if (NULL == lpPlantInfo) {
		ASSERT(FALSE);
		return -1;
	}

	int lnResult = GetSpecPlantStatus(apFixComm, apDB, lpPlantInfo);
	if (lnResult < 1) {
		return lnResult;
	}

	return 1;
}


int CTransactData::GetSpecPlantStatus(ICommInterFace* apFixComm, 
									  IDBInterFace* apDB,
									  CFactoryInfoPool* apFactoryPool)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apFactoryPool);

	int lnCount = 0;

	CPlantInfoPool * lpPlantPool = NULL;

	std::string lsFactory;

	for (int i = 0; i < apFactoryPool->GetInfoNum(); ++i)
	{
		lpPlantPool = apFactoryPool->GetSpecInfoPool(i, lsFactory);
		if (NULL == lpPlantPool) {
			ASSERT(FALSE);
			return -1;
		}

		int lnResult = GetSpecPlantStatus(apFixComm, apDB, lpPlantPool);

		if (lnResult < 0) {
			return lnResult;
		}

		lnCount += lnResult;
	}

	return lnCount;
}

int CTransactData::GetSpecPlantStatus(ICommInterFace* apFixComm, 
									  IDBInterFace* apDB,
									  CFactoryInfoPool* apFactoryPool,
									  const std::string& asFactory)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apFactoryPool);

	CPlantInfoPool * lpPlantPool = NULL;

	lpPlantPool = apFactoryPool->GetSpecInfoPool(asFactory);
	if (NULL == lpPlantPool) {
		ASSERT(FALSE);
		return -1;
	}

	int lnCount = GetSpecPlantStatus(apFixComm, apDB, lpPlantPool);
	if (lnCount < 0) {
		return lnCount;
	}

	return lnCount;
}

int CTransactData::GetSpecPlantStatus(ICommInterFace*  apFixComm,
									  IDBInterFace* apDB,
									  CCompanyInfoPool* apCompanyPool)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apCompanyPool);

	int lnCount = 0;

	CFactoryInfoPool * lpFactoryPool = NULL;

	std::string lsCompany;

	for (int i = 0; i < apCompanyPool->GetInfoNum(); ++i)
	{
		lpFactoryPool = apCompanyPool->GetSpecInfoPool(i, lsCompany);
		if (NULL == lpFactoryPool) {
			ASSERT(FALSE);
			return -1;
		}

		int lnResult = GetSpecPlantStatus(apFixComm, apDB, lpFactoryPool);
		if (lnResult < 0) {
			return lnResult;
		}

		lnCount += lnResult;
	}

	return lnCount;
}

int CTransactData::GetSpecPlantStatus(ICommInterFace*  apFixComm,
									  IDBInterFace* apDB,
									  CCompanyInfoPool* apCompanyPool,
									  const std::string& asCompany)
{
	ASSERT(apFixComm);
	ASSERT(apDB);
	ASSERT(apCompanyPool);

	CFactoryInfoPool * lpFactoryPool = apCompanyPool->GetSpecInfoPool(asCompany);
	if (NULL == lpFactoryPool) {
		ASSERT(FALSE);
		return -1;
	}

	int lnCount = GetSpecPlantStatus(apFixComm, apDB, lpFactoryPool);

	return lnCount;
}

int CTransactData::GetPlantStatus( ICommInterFace * pFixComm, IDBInterFace *apDB )
{
	if (NULL == g_pCompanyPoolAlarm)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -1;
	}

	int lnCount = 0;

	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->Get(FIELD_SERVICE_PLANTID);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	if (sCompany.empty())
	{
		lnCount = GetSpecPlantStatus(pFixComm, apDB, g_pCompanyPoolAlarm);

		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return lnCount;
	}

	CFactoryInfoPool * lpFactoryPool = g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany);
	if (NULL == lpFactoryPool)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -2;
	}

	if (sFactory.empty())
	{
		lnCount = GetSpecPlantStatus(pFixComm, apDB, lpFactoryPool);

		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return lnCount;
	}


	CPlantInfoPool * lpPlantInfoPool = lpFactoryPool->GetSpecInfoPool(sFactory);
	if (NULL == lpPlantInfoPool)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write( pFixComm->GetAci());

		return -3;
	}

	lnCount = GetSpecPlantStatus(pFixComm, apDB, lpPlantInfoPool, sPlant);

	pFixComm->CreateEndMak();
	pFixComm->Write( pFixComm->GetAci());

	return lnCount;
}

/**获取指定设备的无线测点状态
*输入参数：  
*         公司名         FIELD_SYSTEM_COMPANY
*         分厂名         FIELD_SYSTEM_FACTORY
*         机组名         FIELD_SERVICE_PLANTID
*返回值的字段码：
*         测点名         FIELD_SYSTEM_CHANN
*         是否在线       FIELD_SYSTEM_ONLINE 
*         是否正常       FIELD_SYSTEM_NORMAL
*         数据更新状态   FIELD_SYSTEM_DATAUPDATE
*         电池电量       FIELD_SYSTEM_BATTERY_POWER

#define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047
*/
int CTransactData::GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	ICommInterFace * lpFixCom = pFixComm;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	
	CRealPlantInfo *pPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);
	
	if (NULL == pPlantInfo)
	{
		lpFixCom->Write(lpFixCom->GetAci());
		
		return -1;
	}

	lpFixCom->CreateAnswerMul(lpFixCom->GetFunc());

	int lnChannNum = 0;

	string lsChannID;

	int lnChannOnline = -1, lnChannNormal = -1, int lnDataUpdate = -1, int lnBatteryPower = -1;

	int lnRetDataCount = 0;

	bool lbGetDataSucc = false;

	lnChannNum = pPlantInfo->GetVibChannNum();

	for (int lnVibChannIndex = 0; lnVibChannIndex < lnChannNum;lnVibChannIndex++)
	{
		lbGetDataSucc = pPlantInfo->GetSpecWireLessChannStatus(lnVibChannIndex,\
			                                                        lsChannID,\
																	lnChannOnline,\
																	lnChannNormal,\
																	lnDataUpdate,\
																	lnBatteryPower,\
																	GE_VIBCHANN);

		if (lbGetDataSucc)
		{
			lpFixCom->SetItem(FIELD_SYSTEM_CHANN,lsChannID.c_str());

			lpFixCom->SetItem(FIELD_SYSTEM_ONLINE,lnChannOnline);

			lpFixCom->SetItem(FIELD_SYSTEM_NORMAL,lnChannNormal);

			lpFixCom->SetItem(FIELD_SYSTEM_DATAUPDATE,lnDataUpdate);

			lpFixCom->SetItem(FIELD_SYSTEM_BATTERY_POWER,lnBatteryPower);

			lpFixCom->Write(lpFixCom->GetAci());

			lpFixCom->CreateNextAnswer();

			lnRetDataCount++;
		}
	}

	lnChannNum = pPlantInfo->GetProcChannNum();

	for (int lnProcChannIndex = 0; lnProcChannIndex < lnChannNum;lnProcChannIndex++)
	{
		lbGetDataSucc = pPlantInfo->GetSpecWireLessChannStatus(lnProcChannIndex,\
																lsChannID,\
																lnChannOnline,\
																lnChannNormal,\
																lnDataUpdate,\
																lnBatteryPower,\
																GE_TEMPCHANN);

		if (lbGetDataSucc)
		{
			lpFixCom->SetItem(FIELD_SYSTEM_CHANN,lsChannID.c_str());

			lpFixCom->SetItem(FIELD_SYSTEM_ONLINE,lnChannOnline);

			lpFixCom->SetItem(FIELD_SYSTEM_NORMAL,lnChannNormal);

			lpFixCom->SetItem(FIELD_SYSTEM_DATAUPDATE,lnDataUpdate);

			lpFixCom->SetItem(FIELD_SYSTEM_BATTERY_POWER,lnBatteryPower);

			lpFixCom->Write(lpFixCom->GetAci());

			lpFixCom->CreateNextAnswer();

			lnRetDataCount++;
		}
	}

	lpFixCom->CreateEndMak();

	lpFixCom->Write(lpFixCom->GetAci());
	
	return lnRetDataCount;
}
/*!
*输入参数字段码
*
*返回值的字段码：
*          数据采集时间：       FIELD_SERVICE_TIME

#define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
*/

int CTransactData::GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	CBHDateTime loDateTime = CBHDateTime::GetCurrentTime();

	pFixComm->SetItem(FIELD_SERVICE_TIME,loDateTime);

	pFixComm->Write(pFixComm->GetAci());

	return 1;
}

/*!
*输入参数字段码
*
*返回值的字段码：
*          通知状态：  FIELD_SYSTEM_STATUS
#define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
*/
int CTransactData::NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	CBHDateTime loDateTime = CBHDateTime::GetCurrentTime();

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);

	pFixComm->Write(pFixComm->GetAci());

	return -1;
}

/**中间件中获取概貌图信息接口
*输入参数：  
*         机组概貌图类型ID：         FIELD_SYSTEM_PLANT_GRAPHTYPE
*         机组设备类型：             FIELD_SERVICE_PLANT_TYPE
*         
*返回值的字段码：
*         概貌图图片大小             FIELD_SERVICE_CHANN_BUFFSIZE
*         概貌图图片内容             FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF
*         
*         概貌图信息框大小           FIELD_SYSTEM_GRAPH_LISTBOX_BUFFERSIZE
*         概貌图信息框内容           FIELD_SYSTEM_GRAPH_LISTBOX_BUFFER
*
#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION 20048
*/
int nDeviceKindId =0;
int CTransactData::GetPlantGraphInformationS( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	char pFileName[MAX_PATH];   
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName);   

	CString TmpStrCurrentPath;
	TmpStrCurrentPath.Format("%s",pFileName);

	CString strDB = TmpStrCurrentPath+"\\Graph.mdb";

	CString str2007DB = TmpStrCurrentPath + "\\Graph.accdb";

	if (_access(strDB,0) == -1 && _access(str2007DB,0) == -1)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("20048:数据库Graph.mdb不存在"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	//机组概貌图类型ID
	int a_nTestLxId = pFixComm->GetLong(FIELD_SYSTEM_PLANT_GRAPHTYPE);
	int a_nPlantType = pFixComm->GetLong(FIELD_SERVICE_PLANT_TYPE);
	string a_strCompany = pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string a_strFactory = pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string a_strPlant   = pFixComm->GetItem(FIELD_SYSTEM_PLANT);

	CPlantInfoOnLine *pPlantInfo = (CRealPlantInfo *)GetSpecCompanyFactoryPlant(a_strCompany,a_strFactory,a_strPlant,PLANTINFO_REAL);

	int lnGraphID = this->GetDefaultGraphID(a_nPlantType);

	if(pPlantInfo)
	{
		lnGraphID = this->GetDefaultGraphID(pPlantInfo->GetPlantInfo()._iMachineType);
	}

	//数据库连接
	_ConnectionPtr        m_pConnection;

	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		//  CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=" + strDB + ";";
		CString strConnect;
		if(_access(str2007DB,0) == 0)
		{
			strConnect = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};uid=;pwd=;DBQ=" + str2007DB + ";";
		}
		else
		{
			strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=" + strDB + ";";
		}

		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("数据库连接失败,确认数据库Graph.mdb是否在当前路径下"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());

	    return -1;
	}

	try
	{
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance("ADODB.Recordset");

		CString strSql;
		strSql.Format("select DeviceKind_id, fontSize from Test_Lx where id = %d", a_nTestLxId);
		pRecordset->Open(_bstr_t(strSql), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		if (pRecordset->adoEOF)
		{
			pRecordset->Close();

			strSql.Format("select DeviceKind_id, fontSize from Test_Lx where id = %d", lnGraphID);
			pRecordset->Open(_bstr_t(strSql), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
			if (pRecordset->adoEOF)
			{
				pRecordset->Close();

				CString lstrErrorLog;
				lstrErrorLog.Format(_T("该设备未配置好"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->Write(pFixComm->GetAci());

				return -1;
			}
			
		}


		_variant_t vtDeviceKindId = pRecordset->GetCollect("DeviceKind_id");
		_variant_t vtFontSize = pRecordset->GetCollect("FontSize");
		pRecordset->Close();
		nDeviceKindId = (vtDeviceKindId.vt != VT_NULL) ? vtDeviceKindId.lVal : 0;
		if (nDeviceKindId == 0)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("该设备未配置好"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		int lnFontSize = (vtFontSize.vt != VT_NULL) ? vtFontSize.lVal : 9;


		strSql.Format("select tx from DeviceKind where id = %d", nDeviceKindId);
		pRecordset->Open(_variant_t(strSql), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);

		if (pRecordset->adoEOF)
		{
			pRecordset->Close();

			CString lstrErrorLog;
			lstrErrorLog.Format(_T("该设备未配置好"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}
        long lnRealSize  = pRecordset->GetFields()->GetItem("tx")->ActualSize;

	    if (lnRealSize <= 0)
		{
		    pRecordset->Close();

			CString lstrErrorLog;
			lstrErrorLog.Format(_T("该设备未配置好"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());

		    return -1;
		}

	    //获得概貌图图片buffer
		_variant_t vtBlob = pRecordset->GetFields()->GetItem("tx")->GetChunk(lnRealSize);
		
		pRecordset->Close();

		pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	    pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHFONT_SIZE,lnFontSize);

		BYTE *pBuffer = new BYTE[lnRealSize];
		
		memset(pBuffer,0,lnRealSize);
		
		char* pBuf = NULL;
		
		SafeArrayAccessData(vtBlob.parray, (void **)&pBuf);
		
		memcpy(pBuffer, pBuf,(lnRealSize));
		
		SafeArrayUnaccessData(vtBlob.parray);

		//获取信息框信息
		vector<structChannel> loAllListData;
		structChannel structChannelData;

		//不同信息框的ID：标题框-1；时间框0；测点信息框1；转速框2 
		try
		{
			CString strSql;
			strSql.Format("select typeid,ord, XPos, YPos, Width, Height, comment from channel \
						  where (typeid = %d or typeid = %d or typeid = %d or typeid = %d) and TestLx_id = %d \
						  order by ord", -1,0,1,2, a_nTestLxId);
			if(m_pConnection.GetInterfacePtr() == NULL)
			{
				KD_DeletePoint(pBuffer);
				return -1;
			}

			pRecordset->Open(_variant_t(strSql), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		}
		catch(_com_error e)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("读取数据失败"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			pFixComm->Write(pFixComm->GetAci());

			KD_DeletePoint(pBuffer);
			return -1;
		}

		while (!pRecordset->adoEOF)
		{
			structChannelData.m_nType = pRecordset->GetCollect("typeid").lVal;
			structChannelData.m_nOrd = pRecordset->GetCollect("ord").lVal;
			structChannelData.m_nXPos = pRecordset->GetCollect("XPos").lVal;
			structChannelData.m_nYPos = pRecordset->GetCollect("YPos").lVal;
			structChannelData.m_nWidth = pRecordset->GetCollect("Width").lVal;
			structChannelData.m_nHeight = pRecordset->GetCollect("Height").lVal;

			if (pRecordset->GetCollect("comment").vt == VT_NULL)
			{
				CString lstrTemp = "";
				strcpy(structChannelData.m_strComments,lstrTemp);
			}
			else
			{
				CString lstrTemp = (char*)_bstr_t(pRecordset->GetCollect("comment"));
				strcpy(structChannelData.m_strComments,lstrTemp);
			}

			loAllListData.push_back(structChannelData);

			pRecordset->MoveNext();
		}

		pRecordset->Close();

		//传送信息框信息
		pFixComm->SetItem(FIELD_SYSTEM_GRAPH_LISTBOX_BUFFERSIZE,int(((int)loAllListData.size())*sizeof(structChannel)));

		pFixComm->SetItemBuf(FIELD_SYSTEM_GRAPH_LISTBOX_BUFFER,(char *)(&loAllListData.front()),(int)loAllListData.size()*sizeof(structChannel));

		pFixComm->SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,lnRealSize);

		//每次发包大小90kb限制........
		int lnPakageSize =90000;

		int lnTotalBufferSize = 0;

		int lnTempBufferSize = lnRealSize;

		int lnSendBufferSize = lnRealSize;

        while (lnTotalBufferSize < lnRealSize)
        {
			if (lnTempBufferSize > lnPakageSize)
			{
				lnSendBufferSize = lnPakageSize;
			}
			else
			{
				lnSendBufferSize = lnTempBufferSize;
			}

			BYTE * lpSendBuffer = new BYTE[lnSendBufferSize];

			memset(lpSendBuffer,0,lnSendBufferSize);

            memcpy(lpSendBuffer, &pBuffer[lnTotalBufferSize], lnSendBufferSize);

			pFixComm->SetItem(FIELD_SERVICE_PACKAGE_EVERYTIME_BUFFSIZE,lnSendBufferSize);

			pFixComm->SetItemBuf(FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF,(char*)lpSendBuffer,lnSendBufferSize);

            lnTotalBufferSize += lnSendBufferSize;

			lnTempBufferSize -= lnPakageSize;

			pFixComm->Write(pFixComm->GetAci());

			pFixComm->CreateNextAnswer();

			delete []lpSendBuffer;

			lpSendBuffer = NULL;
        }

	    delete []pBuffer;
		pBuffer =NULL;
	}
	catch (_com_error& e)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("读取数据失败"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}
	pFixComm->CreateEndMak();

	pFixComm->Write(pFixComm->GetAci());
	
	return 1;
}

/**中间件中获取概貌图信息接口
*输入参数：  
*         机组概貌图                 FIELD_SERVICE_BHCLIENT_GROUPHINDEX
*         机组概貌图类型ID：         FIELD_SYSTEM_PLANT_GRAPHTYPE
*         
*返回值的字段码：
*         客户端的部位号             FIELD_SERVICE_GROUPH_ACCESS_NUMBER
*         报警类型判断               FIELD_SERVICE_GRAPH_ALARM_TYPE
*         
*         低报low_alarm_xoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_XOFFSET
*         低报low_alarm_yoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_YOFFSET
*         高报low_alarm_xoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_XOFFSET
*         高报low_alarm_yoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_YOFFSET
*
*         低报图片buffer大小         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
*         低报图片buffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
*         高报图片buffer大小         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
*         高报图片buffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
*
#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO 20049
*/
int CTransactData::GetPlantGraphPositonAndAlarmInfo( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	char pFileName[MAX_PATH];   
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName);   

	CString TmpStrCurrentPath;
	TmpStrCurrentPath.Format("%s",pFileName);

	CString strDB = TmpStrCurrentPath+"\\Graph.mdb";

	CString str2007DB = TmpStrCurrentPath + "\\Graph.accdb";

	if (_access(strDB,0) == -1 && _access(str2007DB,0) == -1)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("20049:数据库Graph.mdb不存在"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		pFixComm->CreateAnswer(pFixComm->GetFunc());

		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	long nSize = 0;
	//机组概貌图,区分概貌图，装配图和原理图
	int a_nGrouphindex =pFixComm->GetLong(FIELD_SERVICE_BHCLIENT_GROUPHINDEX);

	//机组概貌图类型ID
	int a_nTestLxId = pFixComm->GetLong(FIELD_SYSTEM_PLANT_GRAPHTYPE);

	//数据库连接
	_ConnectionPtr        m_pConnection;

	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		//CString strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=" + strDB + ";";
		CString strConnect;
		if(_access(str2007DB,0) == 0)
		{
			strConnect = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};uid=;pwd=;DBQ=" + str2007DB + ";";
		}
		else
		{
			strConnect="DRIVER={Microsoft Access Driver (*.mdb)};uid=;pwd=;DBQ=" + strDB + ";";
		}
		m_pConnection->Open((_bstr_t)strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("数据库连接失败,确认数据库Graph.mdb是否在当前路径下"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		pFixComm->CreateAnswer(pFixComm->GetFunc());

		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	_RecordsetPtr pRecordset;
	pRecordset.CreateInstance("ADODB.Recordset");
	
	//获取部位位置信息
    map<int, int> mapAccessToClient;
    try
	{
		CString strSql;
		strSql.Format("select image_subsystem_number, bhclient_subsystem_number from subsystem_mapping where bhclient_device_kind = %d", a_nTestLxId);
		pRecordset->Open((LPTSTR)(LPCTSTR)strSql, m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);

		if (pRecordset->adoEOF)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("该设备部位信息未配置好"),__FILE__,__LINE__);
			//CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		while (!pRecordset->adoEOF)
		{
			int bhclient_subsystem_number = pRecordset->GetCollect("bhclient_subsystem_number");
	        
			int nKey = pRecordset->GetCollect("image_subsystem_number");
			
			mapAccessToClient[nKey] = bhclient_subsystem_number;
	    
			pRecordset->MoveNext();
		}
		pRecordset->Close();

		pFixComm->CreateAnswerMul(pFixComm->GetFunc());

		////查询部位号信息
		strSql.Format("select number, low_alarm_xoffset, low_alarm_yoffset, low_alarm_image, hight_alarm_xoffset, hight_alarm_yoffset, high_alarm_image  from image_subsystem where image_id = %d", nDeviceKindId);
		pRecordset->Open((LPTSTR)(LPCTSTR)strSql, m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		//查询数据库得到所有部位号和报警数据
		while (!pRecordset->adoEOF)
		{
			int lnNumber = 0;

			int nImageSubSystemID = pRecordset->GetCollect("number");  //access的部位号
			map<int, int>::iterator it2 = mapAccessToClient.find(nImageSubSystemID);
			if (it2 == mapAccessToClient.end())                        //找不到就默认为相同
			{
				lnNumber = nImageSubSystemID;
			}
			else
			{
				lnNumber = it2->second;                                
			}	

            //分两次传送，低报和高报，保证每次传送的数据包不至于过大
			for (int lnAlarmIndex = 0; lnAlarmIndex < 2; lnAlarmIndex++)
			{
				pFixComm->SetItem(FIELD_SERVICE_GROUPH_ACCESS_NUMBER,lnNumber);  //bhclient端的部位号
				pFixComm->SetItem(FIELD_SERVICE_GRAPH_ALARM_TYPE,lnAlarmIndex);  //判断报警类型
				int m_nXOffset = 0;
				int m_nYOffset = 0;
				_variant_t vtBlob;
			
				if (lnAlarmIndex == 0)
				{
					m_nXOffset = pRecordset->GetCollect("low_alarm_xoffset");
					m_nYOffset = pRecordset->GetCollect("low_alarm_yoffset");
					pFixComm->SetItem(FIELD_SERVICE_GROUPH_LOW_ALARM_XOFFSET,m_nXOffset);
					pFixComm->SetItem(FIELD_SERVICE_GROUPH_LOW_ALARM_YOFFSET,m_nYOffset);			

					nSize = pRecordset->GetFields()->GetItem("low_alarm_image")->ActualSize;
					if (nSize > 0)
					{
						vtBlob = pRecordset->GetFields()->GetItem("low_alarm_image")->GetChunk(nSize);
						if (vtBlob.vt == (VT_ARRAY | VT_UI1))
						{
							char* pBuf = NULL;
							SafeArrayAccessData(vtBlob.parray, (void**)&pBuf);
							SafeArrayUnaccessData(vtBlob.parray);
							if (pBuf != NULL)
							{
								pFixComm->SetItem(FIELD_SERVICE_GROUPH_LOW_ALARM_BUFFSIZE,nSize);
								pFixComm->SetItemBuf(FIELD_SYSTEM_GROUPH_LOW_ALARM_BUFF,(char*)pBuf,(nSize));
							}
						}
					}
				}
				else if(lnAlarmIndex == 1)
				{
					m_nXOffset = pRecordset->GetCollect("hight_alarm_xoffset");
					m_nYOffset = pRecordset->GetCollect("hight_alarm_yoffset");
					pFixComm->SetItem(FIELD_SERVICE_GROUPH_HIG_ALARM_XOFFSET,m_nXOffset);
					pFixComm->SetItem(FIELD_SERVICE_GROUPH_HIG_ALARM_YOFFSET,m_nYOffset);

					nSize = pRecordset->GetFields()->GetItem("high_alarm_image")->ActualSize;
					if (nSize > 0)
					{
						vtBlob = pRecordset->GetFields()->GetItem("high_alarm_image")->GetChunk(nSize);
						if (vtBlob.vt == (VT_ARRAY | VT_UI1))
						{
							char* pBuf = NULL;
							SafeArrayAccessData(vtBlob.parray, (void**)&pBuf);
							SafeArrayUnaccessData(vtBlob.parray);
							if (NULL != pBuf)
							{
								pFixComm->SetItem(FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE,nSize);
								pFixComm->SetItemBuf(FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF,(char*)pBuf,(nSize));
							}
						}
					}
				}

				pFixComm->Write(pFixComm->GetAci());

				pFixComm->CreateNextAnswer();
			}

			pRecordset->MoveNext();	
		}
		pRecordset->Close();
	}
    catch (_com_error& e)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("读取数据失败"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		pFixComm->Write(pFixComm->GetAci());
		
		return -1;
	}

	pFixComm->CreateEndMak();

    pFixComm->Write(pFixComm->GetAci());
    
	return nSize;

}

int CTransactData::GetDefaultGraphID(int nPlantType_)
{
	int lnGraphID = -1;

	switch(nPlantType_)
	{
	case GE_MACHINETYPE_RC:
		lnGraphID = RECT_GRAPH_ID;
		break;
	case GE_MACHINETYPE_COMPR:
	case GE_MACHINETYPE_FAN:
	case GE_MACHINETYPE_COMPR1:
	case GE_MACHINETYPE_SMOKESTEAM:
		lnGraphID = ROTATION_GRAPH_ID;
		break;
	case GE_MACHINETYPE_WINDPEQ:
		lnGraphID = WINDPOWER_GRAPH_ID;
		break;
	case GE_MACHINETYPE_PUMP:
	case GE_MACHINETYPE_KEYPUMP:
	case GE_MACHINETYPE_ALL:
		lnGraphID = PUMP_GRAPH_ID;
		break;
	case GE_MACHINETYPE_TURB:
		lnGraphID = TURBINE_GRAPH_ID;
		break;
	case GE_MACHINETYPE_DISEL:
		lnGraphID = DIESEL_GRAPH_ID;
		break;
	case GE_MACHINETYPE_ICE:
		lnGraphID = ICE_GRAPH_ID;
		break;
	default:
		lnGraphID = ROTATION_GRAPH_ID;
		break;

	}
	return lnGraphID;

}

int CTransactData::GetLogFileContent( ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	CString lstrFilePath = pFixComm->GetItem(FIELD_SERVICE_FILE_PATH);
	CString lstrFileName = pFixComm->GetItem(FIELD_SERVICE_FILE_NAME);

	char cAppLocation[512]={0};
	GetCurrentDirectory(512,cAppLocation);
	strcat(cAppLocation,"\\Log");
	CString lstrFile = cAppLocation;

	lstrFileName.MakeUpper();
	if(lstrFileName.Find("VIBMAX") >= 0)
	{
		lstrFile.AppendFormat("\\%s*.log",lstrFileName);
	}
	else
	{
		lstrFile.AppendFormat("\\%s*.txt",lstrFileName);
	}

	CFileFind lfileFind;

	BOOL lbFound = lfileFind.FindFile(lstrFile);

	CString lstrDataFileName;

	COleDateTime ltFileWriteTime = COleDateTime(2000,1,1,1,1,1);

	COleDateTime ltCurrTime = COleDateTime::GetCurrentTime();

	COleDateTimeSpan ltTimeSpan = COleDateTimeSpan(30,0,0,0);

	COleDateTime ltLastTime = ltCurrTime - ltTimeSpan;


	CString lstrLogFile;
	CString lstrLogFileName;

	map<CString,CString> lmLogFile;

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

			if(lstrFileName.Find("VIBMAX") >= 0)
			{
				if(ltTime > ltLastTime)
				{
					ltFileWriteTime = ltTime;
					lstrLogFile = lfileFind.GetFilePath();
					lstrLogFileName = lfileFind.GetFileName();
					lmLogFile.insert(make_pair(lstrLogFileName,lstrLogFile));
				}
			}
			else
			{
				if(ltFileWriteTime < ltTime)
				{
					ltFileWriteTime = ltTime;
					lstrLogFile = lfileFind.GetFilePath();
					lstrLogFileName = lfileFind.GetFileName();
					lmLogFile.clear();
					lmLogFile.insert(make_pair(lstrLogFileName,lstrLogFile));
				}
			}
			
			

		}
	}


	lfileFind.Close();

	const int lnReadSize = 1024000;

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	int lnFileCount = 0;

	for(map<CString,CString>::iterator it = lmLogFile.begin(); it != lmLogFile.end(); ++it)
	{
		ifstream lf;
		lstrLogFile = it->second;
		lstrLogFileName = it->first;

		lf.open(lstrLogFile,ios_base::binary);

		if(lf)
		{
			lf.seekg(0,ios_base::end);

			size_t lnFileSize = lf.tellg();

			int lnTotalSize = 0;

			bool lbReadFinish = false;

			int lnShouldReadSize = 0;

			while(true)
			{
				char *pData = NULL;

				if(lnTotalSize == 0)
				{
					lf.seekg(0);
					if(lnFileSize <= lnReadSize)
					{
						lnShouldReadSize = lnFileSize+1;
					}
					else
					{
						lnShouldReadSize = lnReadSize;
					}

				}
				else
				{
					lf.seekg(lnTotalSize);
					if(lnTotalSize + lnReadSize <= lnFileSize)
					{
						lnShouldReadSize = lnReadSize;
					}
					else
					{
						lnShouldReadSize = lnFileSize - lnTotalSize + 1;

					}
				}

				pData = new char[lnShouldReadSize];

				memset(pData,0,lnShouldReadSize);

				lf.read(pData,lnShouldReadSize-1);

				lnTotalSize += (lnShouldReadSize-1);

				pFixComm->SetItemBuf(FIELD_SERVICE_FILE_CONTENT,pData,lnShouldReadSize);

				pFixComm->SetItem(FIELD_SERVICE_FILE_NAME,lstrLogFileName);

				pFixComm->SetItem(FIELD_SERVICE_DB_DATA_FILE_SIZE,lnShouldReadSize);

				KD_DeletePoint(pData);

				if(lnTotalSize >= lnFileSize)
				{
					lnFileCount++;

					if(lnFileCount < lmLogFile.size())
					{
						pFixComm->Write(pFixComm->GetAci());

						pFixComm->CreateNextAnswer();	
					}
					break;
				}
				else
				{
					pFixComm->Write(pFixComm->GetAci());

					pFixComm->CreateNextAnswer();
				}


			}


		}

		lf.close();
	}

	pFixComm->CreateEndMak();

	pFixComm->Write(pFixComm->GetAci());
	return 1;
}

int CTransactData::GetChannGradeThreshold( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	ICommInterFace * lpFixCom = pFixComm;

	ASSERT(lpFixCom);

	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->GetItem(FIELD_SERVICE_PLANTID);

	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	if (NULL == pAlarmPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	string sChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN);

	CChannAlarmParam lchannAlarmParam;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	int lnIndexAlarm = -1;

	int lnResult = FALSE;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecVibChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetVibGradeThreshold(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecDynChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetDynGradeThreshold(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_PROC)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecProcChannIndex(sChannID);

		if (!pAlarmPlantInfo->GetProcGradeThreshold(lnIndexAlarm, lchannAlarmParam))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	lnResult = TRUE;

	pFixComm->SetItem(FIELD_SERVICE_GRADE_ONE_THRESHOLD, lchannAlarmParam._fAlarm1);
	pFixComm->SetItem(FIELD_SERVICE_GRADE_TWO_THRESHOLD, lchannAlarmParam._fAlarm2);
	pFixComm->SetItem(FIELD_SERVICE_GRADE_THREE_THRESHOLD, lchannAlarmParam._fAlarm3);

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());

	return 0;
}

int CTransactData::SetChannGradeThreshold( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	SwitchDbNameEx(pDB,g_strMainDbName);

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	CAlarmPlantInfo *pAlarmPlantInfo=(CAlarmPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_ALARM);

	CRealPlantInfo *pRealPlantInfo = (CRealPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if (NULL == pAlarmPlantInfo || NULL == pRealPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	string sChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN);


	CChannAlarmParam lchannAlarmParam;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	double ldbGradeOneThreshold = pFixComm->GetDouble(FIELD_SERVICE_GRADE_ONE_THRESHOLD);

	double ldbGradeTwoThreshold = pFixComm->GetDouble(FIELD_SERVICE_GRADE_TWO_THRESHOLD);

	double ldbGradeThreeThreshold = pFixComm->GetDouble(FIELD_SERVICE_GRADE_THREE_THRESHOLD);

	int lnIndexAlarm = -1,lnIndexReal = -1;

	int lnResult = 0;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cAlarmParam,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",\
		gc_cCompany,sCompany.c_str(),gc_cPlantNo,pRealPlantInfo->GetPlantNo().c_str(),\
		gc_cChannID,sChannID.c_str());
	if (pDB->More(TRUE))
	{
		long nSize = sizeof(CChannAlarmParam);
		CWaveHead lsWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cAlarmParam,  (BYTE*)&lchannAlarmParam,  nSize,lsWaveHead);
		lchannAlarmParam._fAlarm1 = ldbGradeOneThreshold;
		lchannAlarmParam._fAlarm2 = ldbGradeTwoThreshold;
		lchannAlarmParam._fAlarm3 = ldbGradeThreeThreshold;
		if(!pDB->SetBuffer(gc_cAlarmParam,(BYTE*)&lchannAlarmParam,nSize,false))
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecVibChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecVibChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->SetVibGradeThreshold(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->SetVibGradeThreshold(lnIndexReal,lchannAlarmParam);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecDynChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecDynChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->SetDynGradeThreshold(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->SetDynGradeThreshold(lnIndexReal,lchannAlarmParam);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_PROC)
	{
		lnIndexAlarm = pAlarmPlantInfo->GetSpecProcChannIndex(sChannID);
		lnIndexReal = pRealPlantInfo->GetSpecProcChannIndex(sChannID);

		if (lnIndexAlarm<0 || lnIndexReal < 0 || lnIndexAlarm != lnIndexReal)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pAlarmPlantInfo->SetProcGradeThreshold(lnIndexAlarm,lchannAlarmParam);

		lnResult = pRealPlantInfo->SetProcGradeThreshold(lnIndexReal,lchannAlarmParam);

	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());
	return 0;
}

int CTransactData::GetAxesLocationParam( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	ICommInterFace * lpFixCom = pFixComm;

	ASSERT(lpFixCom);

	string sCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant   = pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sVerChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_VER);
	string sHorChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_HOR);


	CRealPlantInfo *pRealPlantInfo=(CRealPlantInfo *)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if (NULL == pRealPlantInfo)
	{
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	S_ChannInfo lcHorChannInfo;
	S_ChannInfo lcVerChannInfo;

	CAxisChannInfo lcAxisChannInfo;

	CHANNTYPE lnChannType = (CHANNTYPE)pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);

	E_TBL_CHANNTYPE lenTableChannType = GetChannOneType(lnChannType);

	int lnIndexHorChann = -1;
	int lnIndexVerChann = -1;
	int lnIndexAxisChann = -1;


	int lnResult = FALSE;

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if(lenTableChannType == E_TBL_CHANNTYPE_VIB)
	{
		lnIndexHorChann = pRealPlantInfo->GetSpecVibChannIndex(sHorChannID);
		lnIndexVerChann = pRealPlantInfo->GetSpecVibChannIndex(sVerChannID);

		lnIndexAxisChann = pRealPlantInfo->GetSpecAxisLocationIndex(sHorChannID,sVerChannID);

		if(lnIndexHorChann < 0 || lnIndexVerChann < 0 || lnIndexAxisChann < 0 || lnIndexAxisChann >= pRealPlantInfo->_vAxisChann.size())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lcHorChannInfo = pRealPlantInfo->GetSpecVibChannInfo(lnIndexHorChann);
		lcVerChannInfo = pRealPlantInfo->GetSpecVibChannInfo(lnIndexVerChann);
		lcAxisChannInfo = pRealPlantInfo->_vAxisChann[lnIndexAxisChann]->GetChannInfo();

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnIndexHorChann = pRealPlantInfo->GetSpecDynChannIndex(sHorChannID);
		lnIndexVerChann = pRealPlantInfo->GetSpecDynChannIndex(sVerChannID);

		lnIndexAxisChann = pRealPlantInfo->GetSpecDynAxisLocationIndex(sHorChannID,sVerChannID);

		if(lnIndexHorChann < 0 || lnIndexVerChann < 0 || lnIndexAxisChann < 0 || lnIndexAxisChann >= pRealPlantInfo->_vAxisChann.size())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lcHorChannInfo = pRealPlantInfo->GetSpecDynChannInfo(lnIndexHorChann);
		lcVerChannInfo = pRealPlantInfo->GetSpecDynChannInfo(lnIndexVerChann);
		lcAxisChannInfo = pRealPlantInfo->_vAxisChann[lnIndexAxisChann]->GetChannInfo();

	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}

	lnResult = TRUE;

	pFixComm->SetItem(FIELD_SERVICE_CHAR_GAP_HOR,lcHorChannInfo._fGapVoltage);
	pFixComm->SetItem(FIELD_SERVICE_CHAR_GAP_VER,lcVerChannInfo._fGapVoltage);
	pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY_HOR,lcHorChannInfo._fSensitivity);
	pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY_VER,lcVerChannInfo._fSensitivity);
	pFixComm->SetItem(FIELD_SERVICE_AXIS_LOCATION_TYPE,lcAxisChannInfo._iLocationType);
	pFixComm->SetItem(FIELD_SERVICE_ROTATE_DIRECTION,lcAxisChannInfo._iRotateDirection);
	pFixComm->SetItem(FIELD_SERVICE_AXIS_DIA,lcAxisChannInfo._fAxisDia);

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
	pFixComm->Write(pFixComm->GetAci());

	return 0;
}

int CTransactData::SetAxesLocationParam( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	string sAlias;

	ICommInterFace * lpFixCom = pFixComm;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant= pFixComm->GetItem(FIELD_SERVICE_PLANTID);
	string sVerChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_VER);
	string sHorChannID = pFixComm->GetItem(FIELD_SYSTEM_CHANN_HOR);

	CRealPlantInfo *pRealPlantInfo = (CRealPlantInfo*)GetSpecCompanyFactoryPlant(sCompany,sFactory,sPlant,PLANTINFO_REAL);

	if ( NULL == pRealPlantInfo)
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
		lnHorChannIndex = pRealPlantInfo->GetSpecVibChannIndex(sHorChannID);
		lnVerChannIndex = pRealPlantInfo->GetSpecVibChannIndex(sVerChannID);
		lnAxisChannIndex = pRealPlantInfo->GetSpecAxisLocationIndex(sHorChannID,sVerChannID);

		if (lnHorChannIndex < 0 || lnVerChannIndex < 0 || lnAxisChannIndex < 0)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pRealPlantInfo->SetVibAxesLocationParam(lnHorChannIndex,lnVerChannIndex,lnAxisChannIndex,lHorChannInfo,lVerChannInfo,lAxisChannInfo);

	}
	else if(lenTableChannType == E_TBL_CHANNTYPE_DYN)
	{
		lnHorChannIndex = pRealPlantInfo->GetSpecDynChannIndex(sHorChannID);
		lnVerChannIndex = pRealPlantInfo->GetSpecDynChannIndex(sVerChannID);
		lnAxisChannIndex = pRealPlantInfo->GetSpecDynAxisLocationIndex(sHorChannID,sVerChannID);

		if (lnHorChannIndex < 0 || lnVerChannIndex < 0 || lnAxisChannIndex < 0)
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnResult);
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		lnResult = pRealPlantInfo->SetDynAxesLocationParam(lnHorChannIndex,lnVerChannIndex,lnAxisChannIndex,lHorChannInfo,lVerChannInfo,lAxisChannInfo);


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