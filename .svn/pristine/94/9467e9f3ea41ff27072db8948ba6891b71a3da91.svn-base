#include "stdafx.h"
#include "trandatarecandsend.h"
#include "off_globe.h"
#include "additions/DebugHelper.h"

CTranDataRecAndSend::CTranDataRecAndSend(void)
{
}

CTranDataRecAndSend::~CTranDataRecAndSend(void)
{
}

/**申请指定公司、分厂、设备的启停车事件ID
*输入参数字段码：
*             公司ID         FIELD_SYSTEM_COMPANY
*			  分厂名         FIELD_SYSTEM_FACTORY
*             机组ID：       FIELD_SERVICE_PLANTID
*             开始时间       FIELD_SERVICE_OFF_TIMESTART
*返回值的字段码：
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_OFF_REQSUDEVENTID        24008
*/
int CTranDataRecAndSend::ReqSudEventID(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	//AfxMessageBox("dsf");
	string sCompany,sFactory,sPlant,sAlias;
	long lEventID=-2;
	string sTable;
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	string sTime;
	if (NULL==ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm,false))
		goto QUIT_REQSUDEVENTID;
	SwitchDbNameEx(pDB,sAlias.c_str());

	sTable="SUD_LOG_";
	sTable.append(sAlias);

    dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);

	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cEventID, sTable.c_str());
	pDB->Cmd(" WHERE [%s]='%s'", 
		gc_cStart_T, dtStart.Format(gc_cTimeFormat));
	if(pDB->More())
	{
		lEventID = pDB->GetLong(gc_cEventID);
		if (lEventID > 0)
		{
			goto QUIT_REQSUDEVENTID;
		}
	}

	sTime= CPlantInfoBase::TimeConvertString(dtStart);
	//得到目前最大事件ID
	pDB->Cmd("SELECT TOP 1 [%s] FROM [%s] ",gc_cEventID,sTable.c_str());
	pDB->Cmd(" ORDER BY [%s] DESC",gc_cEventID);
//	AfxMessageBox(pDB->GetSql());
	if (pDB->More())
		lEventID=pDB->GetLong(gc_cEventID)+1;
	else
		lEventID=1;
	//插入启停车事件
	pDB->Cmd("INSERT INTO [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s])",sTable.c_str(),gc_cID,gc_cEventID,gc_cStart_T,gc_cEnd_T,gc_cStop_Flag,gc_cSynFlag,gc_cType);
	pDB->Cmd("  VALUES(%d,%d,'%s','%s',0,0,%d)",lEventID,lEventID,sTime.c_str(),sTime.c_str(),GE_SUD_UNKNOWN);
	if (!(pDB->Exec()))
	{
		lEventID=-1;
	}

QUIT_REQSUDEVENTID:
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,lEventID);
	pFixComm->Write(pFixComm->GetAci());
	return lEventID;
}

/**通知指定启停车事件的启停车过程结束。如果成功返回1，失败返回-1。删除临时表中这个事件的数据
  *输入参数字段码：
  *             公司ID        FIELD_SYSTEM_COMPANY
  *			    分厂名        FIELD_SYSTEM_FACTORY
  *             机组ID：      FIELD_SERVICE_PLANTID
  *             启停车类型     FIELD_SERVICE_SUD_TYPE
  *             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
  *             结束时间      FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_NOTICESUDSTOP        24009
  */
int CTranDataRecAndSend::NoticeSudStop(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sAlias;
	if (NULL==ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm))
		return -1;
	SwitchDbNameEx(pDB,sAlias.c_str());
	string sTable="SUD_LOG_";
	sTable.append(sAlias);
	long lEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
	dtEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	string sTime= CPlantInfoBase::TimeConvertString(dtEnd);
	int iRtn=-1;

	pDB->Cmd(" UPDATE [%s] SET ",sTable.c_str());
	pDB->Cmd("[%s]='%s',[%s]=1,[%s]=%d ",gc_cEnd_T,sTime.c_str(),gc_cStop_Flag,gc_cType,iType);
	pDB->Cmd(" WHERE [%s]=%d",gc_cEventID,lEventID);
	if (pDB->Exec())
	{
		iRtn=1;
	}
	
	string sTableName = "SUD_EVENT_";
	sTableName.append(sAlias);
	pDB->Cmd(" UPDATE [%s] SET ",sTableName.c_str());
	pDB->Cmd("[%s]='%s' ",gc_cEnd_T,sTime.c_str());
	pDB->Cmd(" WHERE [%s]=%d",gc_cEventID,lEventID);
	if (pDB->Exec())
	{
		
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iRtn);
	pFixComm->Write(pFixComm->GetAci());

	return iRtn;
}

/**启停车结束后，发送指定机组的振动启停车数据到中间件
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			  分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             起始索引        FIELD_SERVICE_ON_SEND_STARTID
*             启停车标志位    FIELD_SERVICE_SUD_STATUS
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
*             指定机组的转速  FIELD_SERVICE_REV
*             指定机组的转速2 FIELD_SERVICE_REV2
*             指定机组的转速3  FIELD_SERVICE_REV3
*             指定机组的转速4  FIELD_SERVICE_REV4
*             指定机组的转速5  FIELD_SERVICE_REV5
*             特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据        FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*             采样频率        FIELD_SERVICE_SAMPLE_FREQ
*             采样点数        FIELD_SERVICE_SAMPLE_NUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
*            对应的时间       FIELD_SERVICE_TIME
*            对应时间的微秒   FIELD_SERVICE_MICROSECOND
*            启停车类型       FIELD_SERVICE_SUD_TYPE
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECSUDDATA        24010
*/
int CTranDataRecAndSend::RecSudData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;
	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
	if (iLen<=0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pFixComm->Write(pFixComm->GetAci());
		return -1;
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
	long iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	CBHDateTime dTime=CBHDateTime::GetCurrentTime();
	dTime=pFixComm->GetTime(FIELD_SERVICE_TIME);
	bool bNewAssit=true;
	int iRealAllNum=0;
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;
	std::vector<S_OldVibCharValue> loOldVibValue;
	loOldVibValue.resize(iLen);
	std::vector<S_VibCharValue> loVibValue;
	loVibValue.resize(iLen);

	S_OldVibCharValue *pOldVibValue = &loOldVibValue.front();
	S_VibCharValue    *pVibValue    = &loVibValue.front();
	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldVibValue,iLen*sizeof(S_OldVibCharValue));
		pPlantInfo->ChangeVibCharValueToNew(pVibValue,pOldVibValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
	}
	else
	{
		iRealAllNum=iRealNum;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibValue,iLen*sizeof(S_VibCharValue));
	}

	for (int i=0;i<iLen;i++)
	{
		if (!IsVibCharValid(&pVibValue[i]))
		{
			BHLOG_ERROR(STR("特征值不正确，不保存启停车数据"));
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}
	}
	//CString sTemp;sTemp.Format("iSmpNum:%d,iRealNum:%d,iRealAllNum:%d",iSmpNum,iRealNum,iRealAllNum);BHTRACE_DEBUG(sTemp);
	if (iRealAllNum<=0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
		pFixComm->Write(pFixComm->GetAci());
		return -2;
	}
	std::vector<DATATYPE_WAVE> loByteBuf;
	loByteBuf.resize(iRealAllNum);
	DATATYPE_WAVE *pByteBuf = &loByteBuf.front();
	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));
	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
	//得到自定义参数模板ID和自定参数BUFF数据
	string sTempletID="";
	sTempletID=pFixComm->GetItem(FIELD_SYSTEM_TEMPID);
	std::vector<S_CustomParamData> loCustomParam;
	loCustomParam.resize(iLen);
	S_CustomParamData *pfCustomParom = &loCustomParam.front();
	memset(pfCustomParom,0,iLen*sizeof(S_CustomParamData));
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)pfCustomParom,iLen*sizeof(S_CustomParamData));
	int iStatus=-2;
	if (dTime<CBHDateTime(2000,1,1,1,1,1))
		dTime=CBHDateTime::GetCurrentTime();
	//保存数据
	if (iRealAllNum>1 )
    {
		if(!G_TEMP_ONLINE)
        {
            iStatus=pPlantInfo->SaveSudData(iStartIndex,iLen,pVibValue,pByteBuf,pfCustomParom,sTempletID,iEventID,dTime,iMicroSecond,iRealAllNum,pDB);
        }
    }
    else
	{
		BHLOG_ERROR(STR("Offline:采样点数为0，不接收保存启停车数据"));
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**@brief 发送阈值的信息、学习参数、学习数据，6种特征值一起发。返回-1表示失败，返回大于0表示成功
  *输入参数字段码
  *           公司ID           FIELD_SYSTEM_COMPANY
  *			  分厂名           FIELD_SYSTEM_FACTORY
  *           机组ID：         FIELD_SERVICE_PLANTID
  *           通道类型         FIELD_SERVICE_CHANNTYPE
  *           通道索引         FIELD_SERVICE_ON_SEND_STARTID
  *           阈值ID          FIELD_SERVICE_THRESHOLD_ID
  *           特征值个数       FIELD_SERVICE_CHANN_NUM
  *           学习数据的BUFF   FIELD_SERVICE_ON_SEND_VIBWAVE    //数据依次为不同特征值数组链接起来.数组长度=FIELD_SERVICE_LEN*FIELD_SERVICE_CHANN_NUM
  *           学习数据的长度   FIELD_SERVICE_LEN
  *           阈值的BUFF       FIELD_SERVICE_THRESHOLD_BUFF     //结构体长度为FIELD_SERVICE_CHANN_NUM
  *           学习参数的BUFF   FIELD_SERVICE_LEARNPARAM_BUFF    //结构体长度为FIELD_SERVICE_CHANN_NUM
  *           报警类型         FIELD_SERVICE_ALARM_TYPE
  *           学习数据的开始时间  FIELD_SERVICE_OFF_TIMESTART
  *           学习数据的结束时间  FIELD_SERVICE_OFF_TIMEEND
  *           触发门限学习的类型  FIELD_SYSTEM_TYPE
  *返回值的字段码
  *           成功标识            FIELD_SYSTEM_STATUS
  #define    SERVICE_SERVICE_OFF_SAVETHINFO         24014
  */
int CTranDataRecAndSend::SaveThInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-1;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;
    //读取相关信息
	DATATYPE_WAVE *pWave=NULL;
    int iChannType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iChannIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iThID=pFixComm->GetLong(FIELD_SERVICE_THRESHOLD_ID);
	int iCharNum=pFixComm->GetLong(FIELD_SERVICE_CHANN_NUM);
	int iLearnDataLen=pFixComm->GetLong(FIELD_SERVICE_LEN);

    iCharNum=6;//针对当前的快变报警，不取特征值数目，临时设置为6
    long iDataBufBytes=iCharNum*iLearnDataLen*sizeof(float);
	char *pDataBuf=new char[iDataBufBytes];
    pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,pDataBuf,iDataBufBytes);

    long iThBytes=iCharNum*sizeof(CThreshold);//阈值ID BUF的长度
	CThreshold *pThBuf=new CThreshold[iThBytes];//阈值ID BUF
	pFixComm->GetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF,(char*)pThBuf,iThBytes);

	long iAllBytes=iCharNum*sizeof(CThresholdLearnParam);
	CThresholdLearnParam *pThLearnParamBuf=new CThresholdLearnParam[iCharNum];
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char*)pThLearnParamBuf,iAllBytes);

	unsigned int iAlarmType=pFixComm->GetLong(FIELD_SERVICE_ALARM_TYPE);
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
    dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	dtEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
    int iType=pFixComm->GetLong(FIELD_SYSTEM_TYPE);

	//CString strTemp;
	//strTemp.Format("%d,%d,%d,%d,%d",iThID,iChannIndex,iChannType,iType,iCharNum);
	//CHZLogManage::Debug(strTemp,"CTranDataRecAndSend","SaveThInfo");
	//进行阈值信息的保存
	iStatus=pPlantInfo->SaveThresholdInfo(pDB,iThID,iChannIndex,dtStart,dtEnd,iChannType,iType,pDataBuf,iLearnDataLen,pThBuf,pThLearnParamBuf,iAlarmType,iCharNum);

	//strTemp.Format("阈值信息保存成功标识：%d",iStatus);
	//CHZLogManage::Debug(strTemp,"CTranDataRecAndSend","SaveThInfo");

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());

	delete [] pDataBuf;
	delete [] pThBuf;
    delete [] pThLearnParamBuf;
	return iStatus;//如果成功一定要返回大于0
}

/**通知指定快变报警事件的报警过程结束。如果成功返回1，失败返回-1
  *输入参数字段码：
  *             公司ID          FIELD_SYSTEM_COMPANY
  * 		  分厂名          FIELD_SYSTEM_FACTORY
  *             机组ID：        FIELD_SERVICE_PLANTID
  *             通道类型        FIELD_SERVICE_CHANNTYPE
  *             通道索引        FIELD_SERVICE_ON_SEND_STARTID
  *             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
  *           新阈值ID        FIELD_SERVICE_THRESHOLD_ID
  *             结束时间        FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP        24015
  */
int CTranDataRecAndSend::NoticeFastAlarmStop(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-1;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;
	int iChannIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iThIDNew=pFixComm->GetLong(FIELD_SERVICE_THRESHOLD_ID);
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	iStatus=pPlantInfo->UpdateVibAlarmStatus(iChannIndex,iEventID,dt,iThIDNew,pDB);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**发送报警数据到中间件保存
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
*             一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
*             采样点数         FIELD_SERVICE_SAMPLE_NUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
*             对应的时间        FIELD_SERVICE_TIME
*             对应时间的微秒    FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECFASTALARMDATA        24016
*/
int  CTranDataRecAndSend::RecFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-2;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;

	if (isLocalJudgeAlarm())
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		return -2;
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
	CBHDateTime dTime=CBHDateTime::GetCurrentTime();
	dTime=pFixComm->GetTime(FIELD_SERVICE_TIME);

	if(iMicroSecond > 0)
	{
		SYSTEMTIME st;

		if(dTime.GetAsSystemTime(st))
		{
			if (st.wMilliseconds != iMicroSecond)
			{
				dTime.SetMillisecond(iMicroSecond);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	bool bNewAssit=true;
	int iRealAllNum=0;
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;

	if (!bNewAssit)
	{
		if (iRealNum < 1)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealNum));
			pFixComm->Write(pFixComm->GetAci());

			return -4;
		}
	}

	std::vector<S_OldVibCharValue> loOldVibCharValue;
	loOldVibCharValue.resize(iLen);
	
	std::vector<S_VibCharValue> loVibCharValue;
	loVibCharValue.resize(iLen);

	S_OldVibCharValue *pOldVibValue = &loOldVibCharValue.front();
	S_VibCharValue *pVibValue = &loVibCharValue.front();

	if (bNewAssit==false)
	{
		iRealAllNum=iRealNum*iLen;
		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldVibValue,iLen*sizeof(S_OldVibCharValue));
			pPlantInfo->ChangeVibCharValueToNew(pVibValue,pOldVibValue,pfRev,iSmpNum,iSmpFreq,iRealNum,iLen,iStartIndex);
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
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pVibValue,iLen*sizeof(S_VibCharValue));
	}

	bool bExistOnlineChann = false;

	for (int i=0;i<iLen;++i)
	{
	
		S_VibCharValue * lpCharVal = pVibValue+i;

		int iChannIndex = iStartIndex + i;
		S_ChannInfo& loChannInfo = pPlantInfo->GetSpecVibChannInfo(iChannIndex);

		if (loChannInfo._iDataSource == GE_DATA_SOURCE_ONLINE)
		{
			if (lpCharVal->_iSmpNum < 1)
			{
				CString lsErrLog;

				lsErrLog.Format("SERVICE_SERVICE_OFF_RECFASTALARMDATA(%d)测点数%s采样点数不正确(%d)", 
					SERVICE_SERVICE_OFF_RECFASTALARMDATA,
					loChannInfo._cChannID, lpCharVal->_iSmpNum);

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, lsErrLog);
				pFixComm->Write(pFixComm->GetAci());

				BHLOG_ERROR(lsErrLog);

				return -5;
			}

			bExistOnlineChann = true;
		}
	}

	if (bExistOnlineChann)
	{
		if (iRealNum < 1)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealNum));
			pFixComm->Write(pFixComm->GetAci());

			return -4;
		}
	}

	if (bExistOnlineChann)
	{
		if (iRealAllNum < 1)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-5);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("总波形点数不正确:%d", iRealAllNum));
			pFixComm->Write(pFixComm->GetAci());

			return -5;
		}
	}

	//CString sTemp;sTemp.Format("24016:iSmpFreq,%d,iSmpNum:%d,iRealNum:%d,iRealAllNum:%d,bNewAssit:%d",iSmpFreq,iSmpNum,iRealNum,iRealAllNum,bNewAssit==true?1:0);BHTRACE_DEBUG(sTemp);
	//得到波形数据
	DATATYPE_WAVE *pByteBuf=NULL;

	std::vector<DATATYPE_WAVE> loByteBuf;
	
	if (iRealAllNum > 0)
	{
		
		loByteBuf.resize(iRealAllNum);
		pByteBuf = &loByteBuf.front();
		memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
	}

	int *lpHead = NULL;
	std::vector<int> loWaveHead;

	int lnWaveHeadLen = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEHEAD_LENGTH);
	if(lnWaveHeadLen > 0)
	{
		if(lnWaveHeadLen == (iLen * sizeof(int)))
		{
			loWaveHead.resize(lnWaveHeadLen);

			lpHead = &loWaveHead[0];

			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_WAVEHEAD_BUFFER,(char *)lpHead,lnWaveHeadLen);
		}
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
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)pfCustomParom,iLen*sizeof(S_CustomParamData));
	/*
	for(int i = 0; i < iLen; i++)
	{
		for(int j = 0; j < GD_CUSTOMPARAM_BUFFSIZE; j++)
		{
			CString sTemp;sTemp.Format("振动自定义参数:%d,%f",j,pfCustomParom->pfCustomParom[i * GD_CUSTOMPARAM_BUFFSIZE + j]);BHTRACE_DEBUG(sTemp);
		}
	}
	*/
	//CString strTmp;strTmp.Format("%d,%d,%d,%d,%d,%d",iStartIndex,iLen,iEventID,iSmpFreq,iSmpNum,iRealNum);
	//CHZLogManage::Debug(strTmp,"CTranDataRecAndSend","RecFastAlarmData");
	//保存数据

	try
	{
		iStatus=pPlantInfo->SaveVibFAlarmData(pDB, iStartIndex,iLen,pVibValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dTime,iMicroSecond,iRealAllNum,lpHead);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**保存报警后新学习的阈值ID，如果数组长度小于1表示失败
  *输入参数字段码
  *           公司ID          FIELD_SYSTEM_COMPANY
  *			  分厂名          FIELD_SYSTEM_FACTORY
  *           机组ID：        FIELD_SERVICE_PLANTID
  *           通道个数        FIELD_SERVICE_CHANN_NUM
  *           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
  *返回值的字段码：
  *           成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID   24017
  */
int  CTranDataRecAndSend::SaveAlarmedNewThID(ICommInterFace *pFixComm,CCustomDB *pDB)
{
	int iStatus=-1;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;
	return iStatus;
}

/**添加一条完整的报警日志.失败返回值小于0，成功为0
  *输入参数字段码
  *           公司ID          FIELD_SYSTEM_COMPANY
  *			分厂名          FIELD_SYSTEM_FACTORY
  *           机组ID：        FIELD_SERVICE_PLANTID
  *           通道ID          FIELD_SERVICE_CHANN1
  *           日志ID          FIELD_SYSTEM_ID
  *           快变报警事件ID  FIELD_SYSTEM_EVENT_ID
  *           通道类型        FIELD_SERVICE_CHANNTYPE
  *           报警类型        FIELD_SERVICE_ALARM_TYPE
  *           开始时间        FIELD_SERVICE_OFF_TIMESTART
  *           结束时间        FIELD_SERVICE_OFF_TIMEEND
  *           阈值ID          FIELD_SERVICE_THRESHOLD_ID
  *           新阈值ID        FIELD_SERVICE_ON_SEND_STARTID
  *返回值的字段码：
  *           成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_ADDALARMLOG                24018
  */
int CTranDataRecAndSend::AddAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-1;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();

	if (isLocalJudgeAlarm())
	{

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("中心级判断报警, 不接收装置级报警"));
		pFixComm->Write(pFixComm->GetAci());

		return -2;
	}

	int iLogID=pFixComm->GetLong(FIELD_SYSTEM_ID);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	unsigned int iAlarmType=pFixComm->GetLong(FIELD_SERVICE_ALARM_TYPE);
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
	dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	dtEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
	int iThID=pFixComm->GetLong(FIELD_SERVICE_THRESHOLD_ID);
	int iThIDNew=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	string sChannID=pFixComm->Get(FIELD_SERVICE_CHANN1);
	int iAlarmLevel = pFixComm->GetLong(FIELD_SERVICE_ALARM_LEVEL);
	int iStopFlag  = pFixComm->GetLong(FIELD_SERVICE_STOP_FLAG);
    try
	{
		iStatus=pPlantInfo->AddAlarmLog(sChannID,iEventID,iLogID,dtStart,dtEnd,iChannType,iAlarmType,iThID,iThIDNew,pDB,iAlarmLevel,iStopFlag);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog); 	
	}
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**新增报警日志，报警开始添加。如果成功返回1，失败返回-1
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			  分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             通道类型        FIELD_SERVICE_CHANNTYPE
*             通道索引        FIELD_SERVICE_ON_SEND_STARTID
*             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
*             报警类型        FIELD_SERVICE_ALARM_TYPE
*             起始时间        FIELD_SERVICE_OFF_TIMESTART
*             阈值ID          FIELD_SERVICE_THRESHOLD_ID
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG        24019
*/
int CTranDataRecAndSend::AddFastAlarmStartLog(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-1;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;
	int iChannType=pFixComm->GetLong(FIELD_SERVICE_CHANNTYPE);
	int index=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	unsigned int iAlarmType=pFixComm->GetLong(FIELD_SERVICE_ALARM_TYPE);
	CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
	dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	int iThID=pFixComm->GetLong(FIELD_SERVICE_THRESHOLD_ID);
	string sChannID="";
	if (iChannType==GE_VIBCHANN)
		sChannID=pPlantInfo->GetSpecVibChannInfo(index)._cChannID;
	else
		sChannID=pPlantInfo->GetSpecProcChannInfo(index)._cChannID;
	iStatus=pPlantInfo->AddAlarmLog(sChannID,iEventID,iEventID,dtStart,dtStart,iChannType,iAlarmType,iThID,iThID,pDB);

	CString strTemp;strTemp.Format("%d",iStatus);
	CHZLogManage::Debug(strTemp,"CTranDataRecAndSend","AddFastAlarmStartLog");

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}


/**添加一条完整的启停车日志。如果成功返回启停车事件ID，失败返回-1
*输入参数字段码：
*             公司ID        FIELD_SYSTEM_COMPANY
*			        分厂名        FIELD_SYSTEM_FACTORY
*             机组ID：      FIELD_SERVICE_PLANTID
*             启停车类型    FIELD_SERVICE_SUD_TYPE
*             开始时间      FIELD_SERVICE_OFF_TIMESTART
*             结束时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*             启停车事件ID: FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_OFF_ADDWHOLESUDLOG             24053
*/
int CTranDataRecAndSend::AddWholeSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
   string sCompany,sFactory,sPlant,sSuffix;
   CPlantInfoOffLine *pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sSuffix,pFixComm);
   if (NULL==pPlantInfo)
      return -1;
   int iType=pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);
   CBHDateTime dtStart=CBHDateTime::GetCurrentTime();
   CBHDateTime dtEnd=CBHDateTime::GetCurrentTime();
   dtStart=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
   dtEnd=pFixComm->GetTime(FIELD_SERVICE_OFF_TIMEEND);
   int iEventID=-1;
   iEventID=pPlantInfo->AddWholeSudLog(pDB,(E_SUD_TYPE)iType,dtStart,dtEnd);
   pFixComm->CreateAnswer(pFixComm->GetFunc());
   pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,iEventID);
   pFixComm->Write(pFixComm->GetAci());
   return iEventID;
}

/**发送动态报警数据到中间件保存
*输入参数字段码：
*             公司ID           FIELD_SYSTEM_COMPANY
*			  分厂名           FIELD_SYSTEM_FACTORY
*             机组ID：         FIELD_SERVICE_PLANTID
*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             动态报警事件ID   FIELD_SYSTEM_EVENT_ID
*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
*             总的波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
*             对应的时间        FIELD_SERVICE_TIME
*             对应时间的微秒    FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECDYNALARMDATA         24055
*/
int CTranDataRecAndSend::ReceiveDynAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iStatus=-2;
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo=NULL;
	pPlantInfo=ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;

	if (isLocalJudgeAlarm())
	{

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		return -2;
	}

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);
	BOOL bNewAssit = FALSE;
	//判断是否为新版本的发送助手
	if (iSmpFreq>0 && iSmpNum>0)
		bNewAssit=false;
	else
		bNewAssit=true;

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

	if (bNewAssit==false)
	{
		if (iRealAllNum>GD_MIDDLEWARE_WAVE_POINT)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealAllNum));
			pFixComm->Write(pFixComm->GetAci());

			return -4;
		}
	}
	else
	{
		if (iRealAllNum>GD_MIDDLEWARE_WAVE_POINT * iLen)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", iRealAllNum));
			pFixComm->Write(pFixComm->GetAci());

			return -4;
		}
	}

	float pfRev[5];
	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	std::vector<S_OldDynCharValue> loOldDynCharValue;
	loOldDynCharValue.resize(iLen);
    
	std::vector<S_DynCharValue> loDynCharValue;
	loDynCharValue.resize(iLen);

	S_OldDynCharValue *pOldDynCharValue = &loOldDynCharValue.front();
	S_DynCharValue    *pDynCharValue = &loDynCharValue.front();

	if (bNewAssit==false)
	{
		iRealAllNum=iRealAllNum*iLen;
		try
		{
			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pOldDynCharValue,iLen*sizeof(S_OldDynCharValue));
			pPlantInfo->ChangeDynCharValueToNew(pDynCharValue,pOldDynCharValue,pfRev,iSmpNum,iSmpFreq,iRealAllNum,iLen,iStartIndex);
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
		iRealAllNum=iRealAllNum;
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)pDynCharValue,iLen*sizeof(S_DynCharValue));
	}
	
	long  iEventID=pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	int   iMicroSecond=pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);
	CBHDateTime dTime=CBHDateTime::GetCurrentTime();
	dTime=pFixComm->GetTime(FIELD_SERVICE_TIME);
	if(iMicroSecond > 0)
	{
		SYSTEMTIME st;

		if(dTime.GetAsSystemTime(st))
		{
			if (st.wMilliseconds != iMicroSecond)
			{
				dTime.SetMillisecond(iMicroSecond);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	for (int i=0;i<iLen;i++)
	{
		if (!IsDynCharValid(&pDynCharValue[i]))
		{

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

	int *lpHead = NULL;
	std::vector<int> loWaveHead;

	int lnWaveHeadLen = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEHEAD_LENGTH);
	if(lnWaveHeadLen > 0)
	{
		if(lnWaveHeadLen == (iLen * sizeof(int)))
		{
			loWaveHead.resize(lnWaveHeadLen);

			lpHead = &loWaveHead[0];

			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_WAVEHEAD_BUFFER,(char *)lpHead,lnWaveHeadLen);
		}
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
	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)pfCustomParom,iLen*sizeof(S_CustomParamData));
	//保存数据
	iStatus=pPlantInfo->SaveDynAlarmData(pDB,iStartIndex,iLen,pDynCharValue,puiType,pByteBuf,pfCustomParom,sTempletID,iEventID,dTime,iMicroSecond,iRealAllNum,lpHead);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}


/** 发送过程量报警数据到中间件模块
*输入参数字段码：
*                公司ID                 FIELD_SYSTEM_COMPANY
*                分厂名                 FIELD_SYSTEM_FACTORY
*                机组ID                 FIELD_SERVICE_PLANTID
*                报警类型               FIELD_SERVICE_ALARM_TYPE
*                报警事件ID             FIELD_SYSTEM_EVENT_ID
*                对应的时间             FIELD_SERVICE_TIME
*                过程量测点号           FIELD_SYSTEM_CHANN
*                过程量测点类型         FIELD_SYSTEM_CHANN_TYPE
*                过程量特征值           FIELD_SERVICE_CHANN1VALUE
*                转速                   FIELD_SERVICE_REV
*
*返回值的字段码：
*                成功标识               FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECPROCALARMDATA           24056
*/
int CTranDataRecAndSend::ReceiveProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadAndCheckCompanyInfo(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo){
		return -1;
	}

	if (isLocalJudgeAlarm())
	{

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "中心级判断报警, 不接收装置级报警");
		pFixComm->Write(pFixComm->GetAci());

		return -2;
	}

	int iStatus = -2;

	int lnAlarmType = pFixComm->GetLong(FIELD_SERVICE_ALARM_TYPE);
	int lnEventID   = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	CBHDateTime loDateTime = pFixComm->GetTime(FIELD_SERVICE_TIME);

	int iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	if(iMicroSecond > 0)
	{
		SYSTEMTIME st;

		if(loDateTime.GetAsSystemTime(st))
		{
			if (st.wMilliseconds != iMicroSecond)
			{
				loDateTime.SetMillisecond(iMicroSecond);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	CString lsChannNo    = pFixComm->Get(FIELD_SYSTEM_CHANN);
	int     lnChannType  = pFixComm->GetLong(FIELD_SYSTEM_CHANN_TYPE);
	float   lfChannValue = pFixComm->GetDouble(FIELD_SERVICE_CHANN1VALUE);
	int     lnRev        = pFixComm->GetLong(FIELD_SERVICE_REV);

	iStatus = pPlantInfo->SaveProcAlarmData(pDB, lnAlarmType, lnEventID, loDateTime, iMicroSecond,lsChannNo, lnChannType, lfChannValue, lnRev);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, iStatus);
	pFixComm->Write(pFixComm->GetAci());

	return iStatus;
}


/**请求离线巡检数据同步，返回离线测点索引和个数，及同步时间
*输入参数字段码：
*                公司              FIELD_SYSTEM_COMPANY
*			     分厂名            FIELD_SYSTEM_FACTORY
*                设备/机组         FIELD_SYSTEM_PLANTID
*返回值的字段码：
*                上次同步时间      FIELD_SERVICE_TIME
*                离线测点起始索引  FIELD_SERVICE_ON_SEND_STARTID
*                离线测点个数      FIELD_SERVICE_CHANN_NUM
*                同步响应          FIELD_SYSTEM_STATUS
（FIELD_SYSTEM_SUCCESS 可以同步/ FIELD_SYSTEM_FAILURE 异常，无法同步）
*
#define     SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC 24060
*/
int CTranDataRecAndSend::RequestOfflineSync(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany  = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory  = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant    = pFixComm->Get(FIELD_SERVICE_PLANTID);

	if ((sPlant.empty())||(sCompany.empty())||(sFactory.empty()))
	{
	    CString sLog;
		sLog.Format("设备名：%s,分厂名：%s,公司名：%s，有空值",
 			sPlant.c_str(),sFactory.c_str(),sCompany.c_str());

		CHZLogManage::Error(sLog,"CTranDataRecAndSend","RequestOfflineSync");

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, FIELD_SYSTEM_FAILURE);
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}

	string sKey = sCompany;
	sKey.append(sPlant);

	BOOL lbLockGot = FALSE;

	CPlantInfoOffLine *pPlantInfo = NULL;

	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{
			BHLOG_DEBUG(STR(_T("Thread ID:%d"),::GetCurrentThreadId()));

			MAP_PLANTINFO::iterator it = g_mapPlantInfo.find(sKey);

			if (it==g_mapPlantInfo.end())
			{
				CString sLog;
				sLog.Format("指定 设备名：%s,分厂名：%s,公司名：%s，不存在",
					sPlant.c_str(),sFactory.c_str(),sCompany.c_str());
				CHZLogManage::Error(sLog,"CTranDataRecAndSend","RequestOfflineSync");

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS, FIELD_SYSTEM_FAILURE);
				pFixComm->Write(pFixComm->GetAci());
				g_MapLock.ReleaseReaderLock();
				return -2;
			}

			pPlantInfo = it->second;
		}

	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
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
		}

	}

	if ( NULL == pPlantInfo)
	{
		return -2;
	}

	/* 切换至分库
	*/
	string sDataDBName;
	sDataDBName = pPlantInfo->GetCompanyAlias();
	sDataDBName.append(pPlantInfo->GetPlantNo());

	SwitchDbNameEx(pDB, sDataDBName.c_str());

	int procChannNum        = pPlantInfo->_vProcChann.size();
	int offlineProcChannNum = pPlantInfo->_vOfflineChann.size();


	CString sOfflineDataTblName;
	sOfflineDataTblName.Format("%s_%s", gc_cOfflineTable_Prefix, sDataDBName.c_str());

	CBHDateTime lastSyncTime;
	lastSyncTime.ParseDateTime("2000-01-01 00:00:00");

	pDB->Cmd("SELECT TOP 1 [%s] FROM %s", gc_cDateTime, sOfflineDataTblName);
	pDB->Cmd(" ORDER BY %s DESC", gc_cDateTime);
	if(pDB->More())
	  lastSyncTime = pDB->GetTime(gc_cDateTime);

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SERVICE_TIME, lastSyncTime);
	pFixComm->SetItem(FIELD_SERVICE_ON_SEND_STARTID, procChannNum - offlineProcChannNum);
	pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM, offlineProcChannNum);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,FIELD_SYSTEM_SUCCESS);
	pFixComm->Write(pFixComm->GetAci());
	return FIELD_SYSTEM_SUCCESS;
}


/**接收离线巡检数据
*输入参数字段码：
*                公司名        FIELD_SYSTEM_COMPANY
*                分厂名        FIELD_SYSTEM_FACTORY
*                设备号        FIELD_SERVICE_PLANTID
*                采集时间      FIELD_SERVICE_TIME
*                测点          FIELD_SYSTEM_CHANN  (离线中的ID, 在线的No 如1T1, 1T2)
*                转速          FIELD_SERVICE_REV
*                特征值        FIELD_SERVICE_OFF_DATA( float)
*返回值的字段码：
*                同步状态      FIELD_SYSTEM_STATUS
                                （FIELD_SYSTEM_SUCCESS/ FIELD_SYSTEM_FAILURE）
*
#define     SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA            24061
*/
int CTranDataRecAndSend::SyncOfflineData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany  = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory  = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant    = pFixComm->Get(FIELD_SERVICE_PLANTID);

	if ((sPlant.empty())||(sCompany.empty())||(sFactory.empty()))
	{
	    CString sLog;
		sLog.Format("设备名：%s,分厂名：%s,公司名：%s，有空值",
 			sPlant.c_str(),sFactory.c_str(),sCompany.c_str());

		CHZLogManage::Error(sLog,"CTranDataRecAndSend","RequestOfflineSync");

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, FIELD_SYSTEM_FAILURE);
		pFixComm->Write(pFixComm->GetAci());

		return -1;
	}


	string sKey = sCompany;
	sKey.append(sPlant);

	BOOL lbLockGot = FALSE;

	CPlantInfoOffLine *pPlantInfo = NULL;



	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{
			MAP_PLANTINFO::iterator it = g_mapPlantInfo.find(sKey);
			if (it==g_mapPlantInfo.end())
			{
				CString sLog;
				sLog.Format("指定 设备名：%s,分厂名：%s,公司名：%s，不存在",
					sPlant.c_str(),sFactory.c_str(),sCompany.c_str());
				CHZLogManage::Error(sLog,"CTranDataRecAndSend","RequestOfflineSync");

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS, FIELD_SYSTEM_FAILURE);
				pFixComm->Write(pFixComm->GetAci());
				g_MapLock.ReleaseReaderLock();
				return -2;
			}

			 pPlantInfo = it->second;
		}
	}
	catch (...)
	{
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
		}

	}

	if (pPlantInfo == NULL)
	{
		return -2;
	}



	S_ChannInfo sChannInfo;
	map<string, S_ChannInfo> offlineChannInfo;
	COfflineChannParam *pParam;

	for(V_PCHANNINFO::iterator it = pPlantInfo->_vOfflineChann.begin();
		it != pPlantInfo->_vOfflineChann.end();
		++it)
	{
		pParam = (COfflineChannParam *)(*it);
		sChannInfo = pParam->GetChannInfo();

		offlineChannInfo[sChannInfo._cChannID] = sChannInfo;
	}

	/* 切换至分库
	*/
	string sDataDBName;
	sDataDBName = pPlantInfo->GetCompanyAlias();
	sDataDBName.append(pPlantInfo->GetPlantNo());

	SwitchDbNameEx(pDB, sDataDBName.c_str());
	
	string sChannName = pFixComm->Get(FIELD_SYSTEM_CHANN);
	string sChannNo;

	CString sOfflineDataTblName;
	sOfflineDataTblName.Format("%s_%s", gc_cOfflineTable_Prefix, sDataDBName.c_str());
	
	


	map<string, S_ChannInfo>::iterator itChann = offlineChannInfo.find(sChannName);
	if(itChann == offlineChannInfo.end()){
		// 不是已配置的离线测点
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS, FIELD_SYSTEM_FAILURE);
		pFixComm->Write(pFixComm->GetAci());

		return -3;
	}
	


	
	sChannNo = itChann->second._cChannNo;

	int maxID = 0;

	pDB->Cmd("SELECT TOP 1 [%s] FROM %s", gc_cID, sOfflineDataTblName);
	pDB->Cmd(" ORDER BY %s DESC", gc_cID);
	if(pDB->More())
	{
		maxID = pDB->GetLong(gc_cID);
	}

	maxID++;

	bool syncStatus;

	CBHDateTime daqTime = pFixComm->GetTime(FIELD_SERVICE_TIME);

	pDB->Cmd("SELECT [%s] from %s", gc_cID, sOfflineDataTblName);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",
		gc_cChannNo,  sChannNo.c_str(),
		gc_cDateTime, daqTime.Format(gc_cTimeFormat));

	int operateID = -1;
	if(pDB->More())
	{
		operateID = pDB->GetLong(gc_cID);
	}

	int lnData = (int)(pFixComm->GetDouble(FIELD_SERVICE_REV));



	if(operateID > 0) {
		pDB->Cmd("UPDATE %s", sOfflineDataTblName);
		pDB->Cmd(" SET [%s]=%d, [%s]=%f",
			gc_cRev, lnData,
			gc_cAll, pFixComm->GetDouble(FIELD_SERVICE_OFF_DATA));
		pDB->Cmd(" WHERE [%s]=%d",
			gc_cID, operateID);
	}
	else {
		pDB->Cmd("INSERT INTO %s", sOfflineDataTblName);
		pDB->Cmd(" ([%s], [%s], [%s], [%s], [%s])",
			gc_cID, gc_cChannNo, gc_cDateTime, gc_cRev, gc_cAll);
		pDB->Cmd(" VALUES(%d, '%s', '%s', %f, %f)",
			maxID,
			sChannNo.c_str(),
			daqTime.Format(gc_cTimeFormat),
			pFixComm->GetDouble(FIELD_SERVICE_REV),
			pFixComm->GetDouble(FIELD_SERVICE_OFF_DATA));
	}



	syncStatus = pDB->Exec();	

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS, syncStatus);
	pFixComm->Write(pFixComm->GetAci());



	return syncStatus;
}


/*            公司ID                 FIELD_SYSTEM_COMPANY
 *			 分厂名                 FIELD_SYSTEM_FACTORY
 *            机组ID                 FIELD_SERVICE_PLANTID
 */
CPlantInfoOffLine * CTranDataRecAndSend::ReadAndCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,string &sAlias_,ICommInterFace *pFixComm_,bool bRtn_)
{
	CString strInfo;
	sCompany_=pFixComm_->Get(FIELD_SYSTEM_COMPANY);
	sFactory_=pFixComm_->Get(FIELD_SYSTEM_FACTORY);
	sPlant_=pFixComm_->Get(FIELD_SERVICE_PLANTID);
	if ((sPlant_.empty())||(sCompany_.empty())||(sFactory_.empty()))
	{
		strInfo.Format("设备名：%s,分厂名：%s,公司名：%s，有空值",sPlant_.c_str(),sFactory_.c_str(),sCompany_.c_str());
		CHZLogManage::Error(strInfo,"CTranDataRecAndSend","ReadAndCheckCompanyInfo");
		if (bRtn_)
		{
			pFixComm_->CreateAnswer(pFixComm_->GetFunc());
			pFixComm_->SetItem(FIELD_SYSTEM_STATUS,-1);
			pFixComm_->Write(pFixComm_->GetAci());
		}
		return NULL;
	}

	string sConn=sCompany_;
	sConn.append(sPlant_);

	BOOL lbLockGot = FALSE;

	CPlantInfoOffLine * lpRet = NULL;

	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{

			MAP_PLANTINFO::iterator it;
			it=g_mapPlantInfo.find(sConn);
			if (it==g_mapPlantInfo.end())
			{
				strInfo.Format("指定 设备名：%s,分厂名：%s,公司名：%s，不存在",sPlant_.c_str(),sFactory_.c_str(),sCompany_.c_str());
				CHZLogManage::Error(strInfo,"CTranDataRecAndSend","ReadAndCheckCompanyInfo");
				if (bRtn_)
				{
					pFixComm_->CreateAnswer(pFixComm_->GetFunc());
					pFixComm_->SetItem(FIELD_SYSTEM_STATUS,-1);
					pFixComm_->Write(pFixComm_->GetAci());
				}
				g_MapLock.ReleaseReaderLock();
				return NULL;
			}
			sAlias_=it->second->GetCompanyAlias();

			sAlias_.append(it->second->GetPlantNo());

			lpRet = it->second;
		}
	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
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
		}

	}

	return lpRet;
}


CPlantInfoOffLine * CTranDataRecAndSend::ReadAndCheckCompanyInfoEx(string &sCompany_,string &sFactory_,string &sPlant_,string &sAlias_,ICommInterFace *pFixComm_,bool bRtn_)
{
	CString strInfo;
	sCompany_=pFixComm_->Get(FIELD_SYSTEM_COMPANY);
	sFactory_=pFixComm_->Get(FIELD_SYSTEM_FACTORY);
	sPlant_=pFixComm_->Get(FIELD_SERVICE_PLANTID);
	if ((sPlant_.empty())||(sCompany_.empty())||(sFactory_.empty()))
	{
		strInfo.Format("设备名：%s,分厂名：%s,公司名：%s，有空值",sPlant_.c_str(),sFactory_.c_str(),sCompany_.c_str());
		CHZLogManage::Error(strInfo,"CTranDataRecAndSend","ReadAndCheckCompanyInfo");
		if (bRtn_)
		{
			pFixComm_->CreateAnswer(pFixComm_->GetFunc());
			pFixComm_->SetItem(FIELD_SYSTEM_STATUS,-1);
			pFixComm_->Write(pFixComm_->GetAci());
		}
		return NULL;
	}

	string sConn=sCompany_;
	sConn.append(sPlant_);

	BOOL lbLockGot = FALSE;

	CPlantInfoOffLine * lpRet = NULL;

	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{

			MAP_PLANTINFO::iterator it;
			it=g_mapPlantInfo.find(sConn);
			if (it==g_mapPlantInfo.end())
			{
				strInfo.Format("指定 设备名：%s,分厂名：%s,公司名：%s，不存在",sPlant_.c_str(),sFactory_.c_str(),sCompany_.c_str());
				CHZLogManage::Error(strInfo,"CTranDataRecAndSend","ReadAndCheckCompanyInfo");
				if (bRtn_)
				{
					pFixComm_->CreateAnswer(pFixComm_->GetFunc());
					pFixComm_->SetItem(FIELD_SYSTEM_STATUS,-1);
					pFixComm_->Write(pFixComm_->GetAci());
				}
				g_MapLock.ReleaseReaderLock();
				return NULL;
			}
			sAlias_=it->second->GetCompanyAlias();

			sAlias_.append(it->second->GetPlantNo());

			lpRet = it->second;
		}
	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
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
		}

	}

	return lpRet;
}

/** 启停车结束后，发送指定机组的启停车振动测点数据到中间件
 * #define     SERVICE_SERVICE_OFF_SUD_VIB_DATA                         24110
*/
int  CTranDataRecAndSend::RecSudVibData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iRet=-1;

	std::string sCompany,sFactory,sPlant,sAlias;

	CPlantInfoOffLine *pPlantInfo = ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);

	if (iLen < 1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("测点数目不正确:%d", iLen));
		pFixComm->Write(pFixComm->GetAci());

		return -3;
	}

	// 特征值
	std::vector<S_VibCharValue> loCharValBuffer(iLen);
	S_VibCharValue *lpChar = &loCharValBuffer[0];

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)lpChar,iLen*sizeof(S_VibCharValue));

	bool bExistOnlineChann = false;

	for (int i=0;i<iLen;++i)
	{
		const S_VibCharValue& loCharVal = loCharValBuffer[i];

		int lnChannIndex = iStartIndex + i;
		const S_ChannInfo& loChannInfo = pPlantInfo->GetSpecVibChannInfo(lnChannIndex);

		if (GE_DATA_SOURCE_ONLINE == loChannInfo._iDataSource)
		{
			if (loCharVal._iSmpNum < 1)
			{
				CString lsErrLog;

				lsErrLog.Format("SERVICE_SERVICE_OFF_SUD_VIB_DATA(%d)测点数%s采样点数不正确(%d)", 
					SERVICE_SERVICE_OFF_SUD_VIB_DATA,
					loChannInfo._cChannID, loCharVal._iSmpNum);

				pFixComm->CreateAnswer(pFixComm->GetFunc());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, lsErrLog);
				pFixComm->Write(pFixComm->GetAci());

				BHLOG_ERROR(lsErrLog);

				return -3;
			}

			bExistOnlineChann = true;
		}
	}

	// 波形数据
	int lnWaveNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	if (bExistOnlineChann)
	{
		if (lnWaveNum < 1)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", lnWaveNum));
			pFixComm->Write(pFixComm->GetAci());

			return -4;
		}
	}

	DATATYPE_WAVE *lpWave = NULL;
	std::vector<DATATYPE_WAVE> loWaveBuffer(lnWaveNum);

	int *lpHead = NULL;
	std::vector<int> loWaveHead;

	if (lnWaveNum > 0)
	{
		lpWave = &loWaveBuffer[0];

		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)lpWave,lnWaveNum*sizeof(DATATYPE_WAVE));

		int lnWaveHeadLen = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEHEAD_LENGTH);
		if(lnWaveHeadLen > 0)
		{
			if(lnWaveHeadLen == (iLen * sizeof(int)))
			{
				loWaveHead.resize(lnWaveHeadLen);

				lpHead = &loWaveHead[0];

				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_WAVEHEAD_BUFFER,(char *)lpHead,lnWaveHeadLen);
			}
		}
	}

	int lnSudType = pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);

	// 自定义参数
	std::string lsTemplate = pFixComm->GetItem(FIELD_SYSTEM_TEMPID);

	std::vector<S_CustomParamData> loParamBuffer(iLen);
	S_CustomParamData* lpParam = &loParamBuffer[0];

	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)lpParam,iLen*sizeof(S_CustomParamData));

	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);

	float pfRev[5];

	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	long  iEventID = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	CBHDateTime dTime  = pFixComm->GetTime(FIELD_SERVICE_TIME);
	int   iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	try
	{
		iRet=pPlantInfo->SaveVibSudData(pDB, iEventID, dTime, iMicroSecond, 
			iStartIndex, iLen, lpChar, lpWave, lnWaveNum, lpParam, lsTemplate,
			iSmpNum, iSmpFreq, pfRev, lpHead);
	}
	catch (...)
	{
		ASSERT(FALSE);

		BHLOG_ERROR(STR("Database Exception: "));

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("保存启停车振动数据异常"));
		pFixComm->Write(pFixComm->GetAci());

		return -4;
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iRet);
	pFixComm->Write(pFixComm->GetAci());

	return iRet;
}

/** 启停车结束后，发送指定机组的启停车动态测点数据到中间件
 * #define     SERVICE_SERVICE_OFF_SUD_DYN_DATA                         24111
*/
int  CTranDataRecAndSend::RecSudDynData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int iRet=-1;

	std::string sCompany,sFactory,sPlant,sAlias;

	CPlantInfoOffLine *pPlantInfo = ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo)
		return -1;

	int iStartIndex=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_STARTID);
	int iLen=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_DYNCHANNNUM);

	if (iLen < 1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-3);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("测点数目不正确:%d", iLen));
		pFixComm->Write(pFixComm->GetAci());

		return -3;
	}

	// 特征值
	std::vector<S_DynCharValue> loCharValBuffer(iLen);
	S_DynCharValue *lpChar = &loCharValBuffer[0];

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char*)lpChar,iLen*sizeof(S_DynCharValue));

	// 波形数据
	int lnWaveNum=pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
	if (lnWaveNum < 1)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("波形点数不正确:%d", lnWaveNum));
		pFixComm->Write(pFixComm->GetAci());

		return -4;
	}

	std::vector<DATATYPE_WAVE> loWaveBuffer(lnWaveNum);
	DATATYPE_WAVE *lpWave = &loWaveBuffer[0];

	int *lpHead = NULL;
	std::vector<int> loWaveHead;

	int lnWaveHeadLen = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEHEAD_LENGTH);
	if(lnWaveHeadLen > 0)
	{
		if(lnWaveHeadLen == (iLen * sizeof(int)))
		{
			loWaveHead.resize(lnWaveHeadLen);

			lpHead = &loWaveHead[0];

			pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_WAVEHEAD_BUFFER,(char *)lpHead,lnWaveHeadLen);
		}
	}

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)lpWave,lnWaveNum*sizeof(DATATYPE_WAVE));

	int lnSudType = pFixComm->GetLong(FIELD_SERVICE_SUD_TYPE);

	// 自定义参数
	std::string lsTemplate = pFixComm->GetItem(FIELD_SYSTEM_TEMPID);

	std::vector<S_CustomParamData> loParamBuffer(iLen);
	S_CustomParamData* lpParam = &loParamBuffer[0];

	pFixComm->GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)lpParam,iLen*sizeof(S_CustomParamData));

	int iSmpNum=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_NUM);
	int iSmpFreq=pFixComm->GetLong(FIELD_SERVICE_SAMPLE_FREQ);

	float pfRev[5];

	pfRev[0]=pFixComm->GetDouble(FIELD_SERVICE_REV);
	pfRev[1]=pFixComm->GetDouble(FIELD_SERVICE_REV2);
	pfRev[2]=pFixComm->GetDouble(FIELD_SERVICE_REV3);
	pfRev[3]=pFixComm->GetDouble(FIELD_SERVICE_REV4);
	pfRev[4]=pFixComm->GetDouble(FIELD_SERVICE_REV5);

	long  iEventID = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);

	CBHDateTime dTime  = pFixComm->GetTime(FIELD_SERVICE_TIME);
	int   iMicroSecond = pFixComm->GetLong(FIELD_SERVICE_MICROSECOND);

	try
	{
		iRet=pPlantInfo->SaveDynSudData(pDB, iEventID, dTime, iMicroSecond, 
			iStartIndex, iLen, lpChar, lpWave, lnWaveNum, lpParam, lsTemplate,
			iSmpNum, iSmpFreq, pfRev, lpHead);
	}
	catch (...)
	{
		ASSERT(FALSE);

		BHLOG_ERROR(STR("Database Exception: "));

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-4);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, STR("保存启停车动态数据异常"));
		pFixComm->Write(pFixComm->GetAci());

		return -4;
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iRet);
	pFixComm->Write(pFixComm->GetAci());

	return iRet;
}

/** 启停车结束后，发送指定机组的启停车过程量测点数据到中间件
 * #define     SERVICE_SERVICE_OFF_SUD_PROC_DATA                        24113
*/
int  CTranDataRecAndSend::RecSudProcData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sAlias;
	CPlantInfoOffLine *pPlantInfo = ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sAlias,pFixComm,true);
	if (NULL==pPlantInfo){
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
	if (((iProcChannNum+iStartIndex)> pPlantInfo->_vProcChann.size())||(iProcChannNum<1)||(iStartIndex<0))
	{
		CString lstrLogData;
		lstrLogData.Format(_T("24057 接口，数采参数不正确，FIELD_SERVICE_ON_SEND_PROCCHANNNUM:%d, %s,%d"),iProcChannNum, __FILE__,__LINE__);
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

	int iStatus = pPlantInfo->SaveProcSudData(pDB, 
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

namespace SUD
{

inline bool SyncEventName(IDBInterFace *apDB, const CString& aSudEventTable,
						 int aEventID, const CString& aEventName, const CBHDateTime& aStartTime)
{
	CString lsStartTime = aStartTime.Format(DATE_TIME_FORMAT);

	apDB->Cmd("SELECT [%s],[%s] FROM %s", gc_cEventNAME, gc_cStart_T, aSudEventTable);
	apDB->Cmd(" WHERE [%s]=%d", gc_cEventID, aEventID);
	if (apDB->More())
	{
		CString lsDbEventName = apDB->Get(gc_cEventNAME).c_str();
		CBHDateTime loDbStartTime = apDB->GetTime(gc_cStart_T);

		CString lsUpdateSnippet;

		if (lsDbEventName != aEventName)
		{
			ASSERT(FALSE);
			lsUpdateSnippet.Format("%s='%s'", gc_cEventNAME, aEventName);
		}

		if (loDbStartTime != aStartTime)
		{
			ASSERT(FALSE);

			if (!lsUpdateSnippet.IsEmpty())
			{
				lsUpdateSnippet.AppendChar(',');
			}

			lsUpdateSnippet.AppendFormat("%s='%s'", gc_cStart_T, lsStartTime);
		}

		if (!lsUpdateSnippet.IsEmpty())
		{
			apDB->Cmd("UPDATE %s SET %s", aSudEventTable, lsUpdateSnippet);
			apDB->Cmd(" WHERE [%s]=%d", gc_cEventID, aEventID);

			if (!(apDB->Exec()))
			{
				ASSERT(FALSE);

				return false;
			}
		}
	}
	else
	{
		apDB->Cmd("INSERT INTO [%s]([%s],[%s],[%s],[%s],[%s])", aSudEventTable,
			gc_cID,gc_cEventID,gc_cEventNAME,gc_cStart_T,gc_cEnd_T);
		apDB->Cmd("  VALUES(%d,%d,'%s','%s','%s')",
			aEventID,aEventID, aEventName, lsStartTime, lsStartTime);
		if (!(apDB->Exec()))
		{
			ASSERT(FALSE);

			return false;
		}
	}

	return true;
}

}

/**启停车结束后， 同步指定公司、分厂、设备的启停车事件ID
* #define     SERVICE_SERVICE_OFF_SUD_ADD_LOG                          24114
*/
int  CTranDataRecAndSend::RecSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	string sCompany,sFactory,sPlant,sDbSuffix;

	int lnRetEventID = -1;

	if (NULL==ReadAndCheckCompanyInfoEx(sCompany,sFactory,sPlant,sDbSuffix,pFixComm,false))
	{
		lnRetEventID = -1;

		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,lnRetEventID);
		pFixComm->Write(pFixComm->GetAci());

		return lnRetEventID;
	}

	SwitchDbNameEx(pDB, sDbSuffix.c_str());


	CString lsSudLogTable;
	lsSudLogTable.Format("SUD_LOG_%s", sDbSuffix.c_str());

	CString lsSudEventTable;
	lsSudEventTable.Format("SUD_EVENT_%s", sDbSuffix.c_str());

	const int lnRequestEventID = pFixComm->GetLong(FIELD_SYSTEM_EVENT_ID);
	const CString lsRequestEventName = pFixComm->Get(FIELD_SYSTEM_EVENT_NAME);

	const CBHDateTime dtStart = pFixComm->GetTime(FIELD_SERVICE_OFF_TIMESTART);
	const CString lsStartTime = CPlantInfoBase::TimeConvertString(dtStart);

	if (lnRequestEventID < 0)
	{
		lnRetEventID = -2;

		goto __QUIT_REC_SUD_LOG__;
	}


	pDB->Cmd("SELECT [%s] FROM [%s] ", gc_cEventID, lsSudLogTable);
	pDB->Cmd(" WHERE [%s]='%s'", gc_cStart_T, lsStartTime);
	if(pDB->More())
	{
		lnRetEventID = pDB->GetLong(gc_cEventID);

		if (!lsRequestEventName.IsEmpty())
		{
			SUD::SyncEventName(pDB, lsSudEventTable, lnRetEventID, lsRequestEventName, dtStart);
		}

		goto __QUIT_REC_SUD_LOG__;
	}
	else
	{
		pDB->Cmd("SELECT [%s] FROM [%s] ", gc_cStart_T, lsSudLogTable);
		pDB->Cmd(" WHERE [%s]=%d", gc_cEventID, lnRequestEventID);
		if(pDB->More())
		{
			CBHDateTime loDbTime = pDB->GetTime(gc_cStart_T);

			if(loDbTime == dtStart)
			{
				if (!lsRequestEventName.IsEmpty())
				{
					SUD::SyncEventName(pDB, lsSudEventTable, lnRequestEventID, lsRequestEventName, dtStart);
				}

				lnRetEventID = lnRequestEventID;

				goto __QUIT_REC_SUD_LOG__;
			}

			int lnMaxId = 0;

			pDB->Cmd("SELECT TOP 1 [%s] FROM [%s] ",gc_cEventID, lsSudLogTable);
			pDB->Cmd(" ORDER BY [%s] DESC",gc_cEventID);
			if (pDB->More())
				lnMaxId = pDB->GetLong(gc_cEventID)+1;
			else
				lnMaxId = 1;

			pDB->Cmd("INSERT INTO [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s])", lsSudLogTable, 
				gc_cID,gc_cEventID,gc_cStart_T,gc_cEnd_T,gc_cStop_Flag,gc_cSynFlag,gc_cType);
			pDB->Cmd("  VALUES(%d,%d,'%s','%s',0,0,%d)",
				lnMaxId,lnMaxId,lsStartTime,lsStartTime,GE_SUD_UNKNOWN);
			if (!pDB->Exec())
			{
				lnRetEventID = -1;
				goto __QUIT_REC_SUD_LOG__;
			}

			if (!lsRequestEventName.IsEmpty())
			{
				SUD::SyncEventName(pDB, lsSudEventTable, lnMaxId, lsRequestEventName, dtStart);
			}

			lnRetEventID = lnMaxId;
			goto __QUIT_REC_SUD_LOG__;
		}
	}


	// 启停车日志
	pDB->Cmd("INSERT INTO [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s])", lsSudLogTable, 
		gc_cID,gc_cEventID,gc_cStart_T,gc_cEnd_T,gc_cStop_Flag,gc_cSynFlag,gc_cType);
	pDB->Cmd("  VALUES(%d,%d,'%s','%s',0,0,%d)",
		lnRequestEventID,lnRequestEventID,lsStartTime,lsStartTime,GE_SUD_UNKNOWN);
	if (!(pDB->Exec()))
	{
		lnRetEventID=-1;
		goto __QUIT_REC_SUD_LOG__;
	}

	// 启停车事件
	if (!lsRequestEventName.IsEmpty())
	{
		SUD::SyncEventName(pDB, lsSudEventTable, lnRequestEventID, lsRequestEventName, dtStart);
	}


	lnRetEventID = lnRequestEventID;

__QUIT_REC_SUD_LOG__:

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_EVENT_ID,lnRetEventID);
	pFixComm->Write(pFixComm->GetAci());

	return lnRetEventID;
}

	/**临时在线需求，读取或者设置多个正在采集的设备,e
	读取逻辑：如果没有输入（公司、分厂、设备名中任一个长度为零，或者在设备池中没有对应的设备），则返回所有设备的采集状态，如果是多个，客户端应以while(fixcom.more())的方式读取
	设置逻辑：如果有输入（公司、分厂、设备名中任一个长度均不为零，且在设备池中有对应的设备），则返回所有设备的采集状态，以及上次设置的时间，客户端应以while(fixcom.more())的方式读取
	中间件重启后以上次设置的为准，首次启动，均不采集
	*输入参数字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME
					 设置状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
	*                
	*返回值的字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME 
					返回状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
									

	#define     SERVICE_SERVICE_TEMP_SAMPLE_PLANTS 24070 
	*/
typedef struct __TEMP_SAMPLE_PLANT_SAMPLE_INFO
{
	CString m_strCompany;
	CString m_strFactory;
	CString m_strPlant;
	int		m_nStatus;
} TEMP_SAMPLE_PLANT_SAMPLE_INFO, * P_TEMP_SAMPLE_PLANT_SAMPLE_INFO;
int  CTranDataRecAndSend::Service_temp_sample_plants(ICommInterFace * pFixComm,IDBInterFace *pDB)
{

	CString sCompany_ =pFixComm->Get(FIELD_SYSTEM_COMPANY);
	CString sFactory_ =pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CString sPlant_   =pFixComm->Get(FIELD_SERVICE_PLANTID);
	int		lnStatus   =pFixComm->GetLong(FIELD_SYSTEM_STATUS);
	
	BOOL lbLockGot = FALSE; 
	BOOL lbWrite = FALSE;
	BOOL lbRead = FALSE;
	if (!((sPlant_.IsEmpty())||(sCompany_.IsEmpty())||(sFactory_.IsEmpty())))
	{

		try
		{
		string sConn=sCompany_;
		sConn.append(sPlant_);
		lbLockGot = g_MapLock.AcquireWriterLock();


			if (lbLockGot)
			{
				CPlantInfoOffLine *pPlantInfo = NULL;
				MAP_PLANTINFO::iterator it;
				it=g_mapPlantInfo.find(sConn);
				if (it!=g_mapPlantInfo.end())
				{
					pPlantInfo = it->second;	

					if (pPlantInfo!= NULL)
					{
						pPlantInfo->IsSample(lnStatus);
						lbWrite = TRUE;
					}
				}

			}

		}catch(...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		} 

		if (lbLockGot)
		{
			try
			{
				g_MapLock.ReleaseWriterLock();
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			lbLockGot = FALSE;

		}
	}else
	{
		lbRead = TRUE;
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	if (lbWrite)
	{
		SyncSampleRecords();

	}else if(lbRead)
	{
		//读取
		std::vector<TEMP_SAMPLE_PLANT_SAMPLE_INFO> lvoSampleInfo;
		try
		{
			lbLockGot = g_MapLock.AcquireReaderLock();

			if (lbLockGot)
			{
				MAP_PLANTINFO::iterator it;
				for (it = g_mapPlantInfo.begin();it!=g_mapPlantInfo.end();it++)
				{
					CPlantInfoOffLine *pPlantInfo = it->second; 

					ASSERT(NULL!= pPlantInfo);
					if (NULL != pPlantInfo)
					{
						TEMP_SAMPLE_PLANT_SAMPLE_INFO loSampleInfo; 
						loSampleInfo.m_strCompany = pPlantInfo->GetCompanyName().c_str();
						loSampleInfo.m_strFactory = pPlantInfo->GetFactoryName().c_str();
						loSampleInfo.m_strPlant = pPlantInfo->GetPlantID().c_str();
						loSampleInfo.m_nStatus = pPlantInfo->IsSample();
						lvoSampleInfo.push_back(loSampleInfo);
					}
				}			
			}

		}catch(...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
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
			}

			lbLockGot = FALSE;
		}

		if (lvoSampleInfo.size()>0)
		{
			for (int i =0;i<lvoSampleInfo.size();i++)
			{
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,lvoSampleInfo[i].m_strCompany);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,lvoSampleInfo[i].m_strFactory);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID,lvoSampleInfo[i].m_strPlant);
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,lvoSampleInfo[i].m_nStatus);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return 1;
}


/*
int CTranDataRecAndSend::ReceiveSudVibValue(ICommInterFace * pFixComm,CCustomDB *pDB)
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

	S_OldVibCharValue *pOldVibCharValue=new S_OldVibCharValue[iLen];
	ZeroMemory(pOldVibCharValue,sizeof(S_OldVibCharValue)*iLen);

	S_VibCharValue *pVibCharValue=new S_VibCharValue[iLen];
	ZeroMemory(pVibCharValue,sizeof(S_VibCharValue)*iLen);

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
				zdlTraceLine(lstrLogData);

				DATATYPE_WAVE *pByteBuf=new DATATYPE_WAVE[iRealAllNum];
				pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));

				delete[] pByteBuf;
				return -1;
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

	DATATYPE_WAVE *pByteBuf=new DATATYPE_WAVE[iRealAllNum];

	ASSERT(pVibCharValue->_iSmpNum!=0);

	memset(pByteBuf,0,iRealAllNum*sizeof(DATATYPE_WAVE));

	long iAllBytes=0;
	long iUnZipBytes=0;
	BYTE *pZipByte=NULL;

	CBHDateTime loTimeUpdate = CBHDateTime::GetCurrentTime();

	iAllBytes=pFixComm->GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
	if (iAllBytes>0)
	{
		CZipImpl zipImpl;
		pZipByte=new BYTE[iAllBytes];
		memset(pZipByte,0,iAllBytes);
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pZipByte,iAllBytes);
		if (zipImpl.UnZip(pZipByte,iAllBytes,(BYTE*)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE),iUnZipBytes))
		{
			ASSERT(FALSE);
			bFlag=true;
		}else
		{
			ASSERT(FALSE);
		}
		KD_DeletePoint(pZipByte);

	}
	else
	{
		pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)pByteBuf,iRealAllNum*sizeof(DATATYPE_WAVE));
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
		pPlantInfo->UpdateVibData(pByteBuf,pVibCharValue,iRealAllNum,iLen,loTimeUpdate,iStartIndex,bEnd);

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
			pAlarmPlantInfo->UpdateVibData( pByteBuf,
				pVibCharValue,
				iRealAllNum,
				iLen,
				loTimeUpdate,
				iStartIndex,
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


		}
	}
	KD_DeletePoint(pOldVibCharValue);
	KD_DeletePoint(pVibCharValue);
	KD_DeletePoint(pByteBuf);

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

*/
