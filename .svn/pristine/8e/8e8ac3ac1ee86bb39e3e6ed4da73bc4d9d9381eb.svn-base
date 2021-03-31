#include "stdafx.h"
#include <algorithm>
#include "implement.h"
#include "SysGlobal.h"
#include <fstream>
#include <json/json.h>
#include <MiddleWare.h>
#include <additions/DebugHelper.h>

CImplement::CImplement(void)
{
}

CImplement::~CImplement(void)
{
}

/**通过测点类型和传感器类型得到特征值描述
*输入参数：
*             设备类型    FIELD_SYSTEM_TYPE
*             测点类型    FIELD_SYSTEM_CHANN_TYPE
*             传感器类型  FIELD_SYSTEM_SENSOR_TYPE
*返回值的字段码：
*             索引         FIELD_SYSTEM_STATUS
*             对应描述     FIELD_SYSTEM_STATUS_DISCRIPTION
*             特征值单位   FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
*/
int CImplement::GetCharTypeDesc(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iChannType   = pFixComm->GetLong(FIELD_SYSTEM_CHANN_TYPE);
	int iSensorType  = pFixComm->GetLong(FIELD_SYSTEM_SENSOR_TYPE);
	int iMachineType = pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	int iType=-1;
	int nCount=-1;
	iType = GetCharTypeDescKey( iMachineType, iChannType, iSensorType );
	pDB->Cmd("select * from [%s] ",gc_cMachInfoTypeTable);
	pDB->Cmd(" where [%s]=%d ",gc_cInfoType,iType);
	pDB->Cmd(" order by [%s] ",gc_cIndex);
//	AfxMessageBox(pDB->GetSql());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cIndex));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
	    pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,pDB->Get(gc_cUnit).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**修改监测中心中间件IP和Port
*输入参数：
 *          公司ID      FIELD_SYSTEM_COMPANY
*           所在分厂    FIELD_SYSTEM_FACTORY
*           装置名ID    FIELD_SYSTEM_SET
*           中间件IP    FIELD_SYSTEM_MW_IP
*           中间件PORT  FIELD_SYSTEM_MW_PORT
*返回值的字段码：
*           状态     FIELD_SYSTEM_STATUS
*           状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
int CImplement::SysModifyMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
   SwitchDbNameEx(pDB,g_strMainDbName);
   string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
   string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
   string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
   string strIP=pFixComm->Get(FIELD_SYSTEM_MW_IP);
   int iPort=pFixComm->GetLong(FIELD_SYSTEM_MW_PORT);
   int iMWType=pFixComm->GetLong(FIELD_SYSTEM_MW_TYPE);
   string strMwID="";
   int iStatus=-1;

   pFixComm->CreateAnswer(pFixComm->GetFunc());
   pDB->Cmd("SELECT %s from %s ",gc_cSetID,gc_cSetTable);
   pDB->Cmd(" %s ",SetSetFactoryCond(sCompany,sFactory,sSetID).c_str());
   if(pDB->More())
	   strMwID=pDB->Get(gc_cSetID).c_str();
   else
	   goto CL_SYSMODIFYMWINFO;

   pDB->Cmd("UPDATE %s SET %s='%s',%s=%d,%s=%d ",gc_cSetTable,
	   gc_cCenterMw_IP,strIP.c_str(),
	   gc_cCenterMw_Port,iPort,
	   gc_cMwType,iMWType);
   pDB->Cmd(" WHERE %s='%s'",gc_cSetID,strMwID.c_str());
   if (pDB->Exec())
	   iStatus=0;

CL_SYSMODIFYMWINFO:
    pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**得到监测中心中间件IP和Port
*           公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*返回值的字段码：
*          中间件IP    FIELD_SYSTEM_MW_IP
*          中间件PORT  FIELD_SYSTEM_MW_PORT
#define     SERVICE_SYSTEM_GET_MWINFO                      25021
*/
int CImplement::SysGetMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
  SwitchDbNameEx(pDB,g_strMainDbName);
  int iStatus=-1;
  string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
  string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
  string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
  pDB->Cmd("SELECT %s,%s,ISNULL(%s,0) AS %s from %s ",
	  gc_cCenterMw_IP,
	  gc_cCenterMw_Port,
	  gc_cMwType,gc_cMwType,
	  gc_cSetTable);
 // pDB->Cmd(" %s ",SetSetFactoryCond(sCompany,sFactory,sSetID).c_str());
  pDB->Cmd("  where [%s]=0",gc_cID);
  pFixComm->CreateAnswer(pFixComm->GetFunc());
  if (pDB->More())
  {
	  iStatus=0;
	  pFixComm->SetItem(FIELD_SYSTEM_MW_IP,pDB->Get(gc_cCenterMw_IP).c_str());
      pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,pDB->GetLong(gc_cCenterMw_Port));
	  pFixComm->SetItem(FIELD_SYSTEM_MW_TYPE,pDB->GetLong(gc_cMwType));
  }
  pFixComm->Write(pFixComm->GetAci());
  return iStatus;
}

/**修改发送到监测中心的时间间隔和是否发送状态
 *           公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
*          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022
*/
int CImplement::SysModifySendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
	int  iInterval=pFixComm->GetLong(FIELD_SYSTEM_SENDC_INTERVAL);
	int iSendif=pFixComm->GetLong(FIELD_SYSTEM_SENDC_IF);
	int iSundSendIt=pFixComm->GetLong(FIELD_SYSTEM_SUD_SENDC_INTERVAL);
	int iAlarmSendIt=pFixComm->GetLong(FIELD_SYSTEM_ALARM_SENDC_INTERVAL);
	string strMwID="";
	int iStatus=-1;

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pDB->Cmd("SELECT %s from %s ",gc_cSetID,gc_cSetTable);
	pDB->Cmd(" %s ",SetSetFactoryCond(sCompany,sFactory,sSetID).c_str());
	if(pDB->More())
		strMwID=pDB->Get(gc_cSetID);
	else
		goto CL_SYSMODIFYSENDINFO;

	pDB->Cmd("UPDATE %s SET %s=%d,%s=%d,%s=%d,%s=%d ",gc_cSetTable,gc_cSend_Interval,iInterval,gc_cIf_Send,iSendif,gc_cAlarm_Send_Interval,iAlarmSendIt,gc_cSud_Send_Interval,iSundSendIt);
	pDB->Cmd(" WHERE %s='%s'",gc_cSetID,strMwID.c_str());
	if (pDB->Exec())
		iStatus=0;

CL_SYSMODIFYSENDINFO:
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**得到发送到监测中心的时间间隔和是否发送
*         公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*返回值的字段码：
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
*          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
#define     SERVICE_SYSTEM_GET_SENDINFO                     25023
*/
int CImplement::SysGetSendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iStatus=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
	pDB->Cmd("SELECT * from %s ",gc_cSetTable);
//	pDB->Cmd(" %s ",SetSetFactoryCond(sCompany,sFactory,sSetID).c_str());
	pDB->Cmd("  where [%s]=0",gc_cID);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		iStatus=0;
		pFixComm->SetItem(FIELD_SYSTEM_SENDC_INTERVAL,pDB->GetLong(gc_cSend_Interval));
		pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,pDB->GetLong(gc_cIf_Send));
		pFixComm->SetItem(FIELD_SYSTEM_SUD_SENDC_INTERVAL,pDB->GetLong(gc_cSud_Send_Interval));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_SENDC_INTERVAL,pDB->GetLong(gc_cAlarm_Send_Interval));
	}
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**得到装置级中间件信息
*返回的字段码：
*         分厂名字      FIELD_SYSTEM_FACTORY
*         公司ID        FIELD_SYSTEM_COMPANY
*         装置名ID      FIELD_SYSTEM_SET
*         信息结构体    FIELD_SYSTEM_SETSVR_SETINFO
*         结构体长度    FIELD_SERVICE_LEN
#define    SERVICE_SYSTEM_GET_SETSVRINFO                     25024
*/
int CImplement::SysGetSetSvrInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iStatus=-1;
	pDB->Cmd("SELECT * from [%s]",gc_cSetTable);
	S_SetInfo sSetInfo;
	int iLen=sizeof(S_SetInfo);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		iStatus++;
        sSetInfo.GetSetInfo(pDB);
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,sSetInfo._cFactory);
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sSetInfo._cCompany);
		pFixComm->SetItem(FIELD_SYSTEM_SET,sSetInfo._cSetID);
        pFixComm->SetItemBuf(FIELD_SYSTEM_SETSVR_SETINFO,(char *)&sSetInfo,iLen);
		pFixComm->SetItem(FIELD_SERVICE_LEN,iLen);
		pFixComm->SetItem(FIELD_SYSTEM_MW_TYPE, pDB->GetLong(gc_cMwType));
		pFixComm->SetItem(FIELD_SYSTEM_TM_MW_TYPE, pDB->GetLong(gc_cTm_MwType));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**得到装置级中间件服务器的设备详细信息
  *        设备起始索引  FIELD_SERVICE_STARTINDEX
  *返回的字段码：
  *        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        版本          FIELD_SYSTEM_VERSION
  *        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        显示集团      FIELD_SERVICE_GROUP_VISIBLE
  *        显示公司      FIELD_SERVICE_COMPANY_VISIBLE
  *        显示分厂      FIELD_SERVICE_FACTORY_VISIBLE
  *        显示装置      FIELD_SERVICE_SET_VISIBLE
  *        显示设备      FIELD_SERVICE_PLANT_VISIBLE
  *        集团名称      FIELD_SYSTEM_GROUP
  *        国家名称      FIELD_SYSTEM_COUNTRY
  *        设备索引      FIELD_SERVICE_ENDINDEX
  *        设备数目      FIELD_SERVICE_PLANT_NUM
  *        设备显示名称  FIELD_SYSTEM_PLANT_DISPLAY
  *        ERP编码       FIELD_SYSTEM_PLANT_ERP_CODE
  *        设备标识      FIELD_SERVICE_DEVICE_ID
  #define    SERVICE_SYSTEM_GET_SETSVRPLANTINFO                25025
  */
int CImplement::SysGetSetSvrPlantInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);

	bool bNotEmpty=false;
	
	int iSize=sizeof(S_PlantInfo);

	int lnPlantIndex = 0;

	int lnMaxPlantCount = 150;

	int lnRealSetPlantCount = 0;

	int lnStartIndex = pFixComm->GetLong(FIELD_SERVICE_STARTINDEX);

	//得到中间件标示名（装置名）

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	for (FactoryDataTypeEx::iterator lpIt = g_ListPlantInfoEx.begin();
		 lpIt != g_ListPlantInfoEx.end();
		 lpIt++)
	{
		if (lnPlantIndex >= lnStartIndex)
		{
			S_PlantInfo & sPlantInfo = lpIt->_plantInfo;
			pFixComm->SetItem(FIELD_SYSTEM_VERSION,sPlantInfo._sVersion);
			pFixComm->SetItem(FIELD_SERVICE_LEN,iSize);
			pFixComm->SetItemBuf(FIELD_SYSTEM_SETSVR_PLANTINFO,(char *)&sPlantInfo,iSize);
			pFixComm->SetItem(FIELD_SERVICE_GROUP_VISIBLE, lpIt->_bGroupVisible);
			pFixComm->SetItem(FIELD_SERVICE_COMPANY_VISIBLE, lpIt->_bCompanyVisible);
			pFixComm->SetItem(FIELD_SERVICE_FACTORY_VISIBLE, lpIt->_bFactoryVisible);
			pFixComm->SetItem(FIELD_SERVICE_SET_VISIBLE, lpIt->_bSetVisible);
			pFixComm->SetItem(FIELD_SERVICE_PLANT_VISIBLE, lpIt->_bPlantVisible);
			pFixComm->SetItem(FIELD_SYSTEM_GROUP, lpIt->m_strGroup);
			pFixComm->SetItem(FIELD_SYSTEM_COUNTRY,lpIt->m_strContry);
			pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
			pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,lnMaxPlantCount);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY,lpIt->m_strPlantDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,lpIt->m_strSetDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,lpIt->m_strErpCode);
			pFixComm->SetItem(FIELD_SERVICE_DEVICE_ID, lpIt->m_DeviceId);
			
			pFixComm->SetItem(FIELD_SYSTEM_GROUP_DISPLAY,lpIt->m_strGroupDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY,lpIt->m_strCompanyDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY,lpIt->m_strFactoryDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_GROUP_SORTNO,lpIt->m_nGroupSortNO);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY_SORTNO,lpIt->m_nCompanySortNO);
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY_SORTNO,lpIt->m_nFactorySortNO);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_SORTNO,lpIt->m_nPlantSortNO);
			pFixComm->SetItem(FIELD_SYSTEM_SET_SORTNO,lpIt->m_nSetSortNO);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,lpIt->m_strCompanyAlias);

			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();

			lnRealSetPlantCount++;
		}

		lnPlantIndex++;

		if (lnRealSetPlantCount >= lnMaxPlantCount)
		{
			pFixComm->CreateEndMak();
			pFixComm->Write(pFixComm->GetAci());

			return lnPlantIndex;
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return lnPlantIndex;
}

/**得到装置级中间件服务器指定设备的测点详细信息
* 输入字段码：
*        公司ID        FIELD_SYSTEM_COMPANY
*		 分厂名        FIELD_SYSTEM_FACTORY
*        设备号：      FIELD_SYSTEM_PLANT
*返回的字段码：
*        测点信息  FIELD_SYSTEM_SETSVR_CHANNINFO
*        个数      FIELD_SYSTEM_SETSVR_INFONUM
#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN            25026
*/
int CImplement::SysGetSetSvrSpecPlantChann(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iStatus=-1;
	S_ChannInfo sChannInfo;
	string strPlantID=pFixComm->Get(FIELD_SYSTEM_PLANT);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int iStuctSize=sizeof(S_ChannInfo);
	string strTmp=GetPlantNo(sCompany, sFactory, strPlantID, pDB);

	pDB->Cmd("SELECT * FROM %s",gc_cChannTable);
    if (!(strPlantID.empty()))
	{
		pDB->Cmd(" WHERE %s='%s' AND %s='%s' AND %s='%s' ",gc_cPlantNo,strTmp.c_str(),gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());
		pDB->Cmd(" ORDER BY [%s]",gc_cID);
	}
//	AfxMessageBox(pDB->GetSql());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		++iStatus;
		sChannInfo.GetChannInfo(pDB);
		S_ChannInfo * lpInfo = &sChannInfo;
		pFixComm->SetItemBuf(FIELD_SYSTEM_SETSVR_CHANNINFO,(char *)&sChannInfo,iStuctSize);
		pFixComm->SetItem(FIELD_SERVICE_LEN,iStuctSize);
		
		pFixComm->Write(pFixComm->GetAci());
        pFixComm->CreateNextAnswer();
	}
//	strTmp.Format("%s,%s,%s,%s",sChannInfo._cCompany,sChannInfo._cFactory,sChannInfo._cPlantID,sChannInfo._cChannID);	AfxMessageBox(strTmp);
	pFixComm->CreateEndMak();
    pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}
/**得到所有用户信息
  *输入字段码：
  *返回字段码：
  *            用户名         FIELD_SYSTEM_USEID
  *            密码           FIELD_SYSTEM_PASSWORD
  *            用户级别       FIELD_SYSTEM_USER_LEVEL
  *            所在分厂       FIELD_SYSTEM_FACTORY
  *            装置名ID       FIELD_SYSTEM_SET
  *            是否浏览装置   FIELD_SYSTEM_BROWSE_SET
  *            刷新时间 FIELD_SYSTEM_REFRESHTIME
  #define    SERVICE_SYSTEM_GET_ALLUSERINFO               25027
  */
int CImplement::SysGetAllUserInfo(ICommInterFace * pFixComm,IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iStatus=-1;
	pDB->Cmd("SELECT * from %s",gc_cUserTable);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_USEID,pDB->Get(gc_cUserName).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PASSWORD,pDB->Get(gc_cPassWord).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,pDB->GetLong(gc_cUserLevel));
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_BROWSE_SET,pDB->GetLong(gc_cBrowseSet));
		pFixComm->SetItem(FIELD_SYSTEM_REFRESHTIME,pDB->GetLong(gc_cRefreshTime));
        pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		iStatus++;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

string CImplement::SetSetFactoryCond(string sCompany_,string sFactory_,string sSetID_)
{
   bool bFirst=true;
   string sSql="";
   if (!(sFactory_.empty()))
   {
        sSql=" WHERE ";
		sSql.append(gc_cFactory_Name);
		sSql.append(" = '");
		sSql.append(sFactory_);
		sFactory_.append("' ");
		bFirst=false;
   }
   if (!(sCompany_.empty()))
   {
	   if (bFirst)
	   {
		   sSql=" WHERE ";
	   }
	   else
	   {
		   sSql.append(" AND ");
		   sSql.append(gc_cCompany);
		   sSql.append(" = '");
		   sSql.append(sCompany_);
		   sFactory_.append("' ");
	   }
   }
   if (!(sSetID_.empty()))
   {
	   if (bFirst)
	   {
		    sSql=" WHERE ";
	   }
	   else
	   {
		   sSql.append(" AND ");
		   sSql.append(gc_cSetID);
		   sSql.append(" = '");
		   sSql.append(sSetID_);
		   sFactory_.append("' ");
	   }
   }
   return sSql;
}


int CImplement::GetSetID(IDBInterFace * pDB_,string sSetID_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int iRtn=0;
	pDB_->Cmd(" select [%s] from [%s] ",gc_cID,gc_cSetTable);
	pDB_->Cmd(" where [%s]='%s'",gc_cSetID,sSetID_.c_str());
	if (pDB_->More())
		iRtn=pDB_->GetLong(gc_cID);
	else
		iRtn=0;
	return iRtn;
}

int CImplement::CheckSetSvrLevel(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int iRtn=GE_SRV_TYPE_SET;
	pDB_->Cmd(" select DISTINCT [%s] from [%s] ",gc_cID,gc_cSetTable);
	if (pDB_->More())
	{
		int nCount=pDB_->GetCount();
		switch(nCount)
		{
		case 1:
			iRtn=GE_SRV_TYPE_SET;
			break;
		case 2:
			iRtn=GE_SRV_TYPE_COMPANY;
			break;
		case 3:
			iRtn=GE_SRV_TYPE_GROUP;
			break;
		}

		if (nCount>3)
		{
			iRtn=GE_SRV_TYPE_GROUP;			
		}
	}
	return iRtn;
}

int CImplement::CheckSpecSetIDLevel(IDBInterFace *pDB_,string sSetID_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
    int iRtn=GE_SRV_TYPE_SET;
	int iSvrLevel=CheckSetSvrLevel(pDB_);
	//根据其是否有父级和其是否有子级、ID值判断
	pDB_->Cmd(" select [%s],[%s],[%s] from [%s] ",gc_cID,gc_cParent_ID,gc_cChild_Flag,gc_cSetTable);
	pDB_->Cmd(" where [%s]='%s' ",gc_cSetID,sSetID_.c_str());
	if (pDB_->More())
	{
		int iID=pDB_->GetLong(gc_cID);
	//	CString strParent=pDB_->Get(gc_cParent_ID);
	//	int iChildFlag=pDB_->GetLong(gc_cChild_Flag);
		if(iSvrLevel==GE_SRV_TYPE_COMPANY)
		{
			if (iID==0)
	    		iRtn=GE_SRV_TYPE_COMPANY;
            else
				iRtn=GE_SRV_TYPE_SET;
		}
		else if(iSvrLevel==GE_SRV_TYPE_GROUP)
		{

			if(iID==0)
				iRtn=GE_SRV_TYPE_GROUP;
			else if(iID==1)
				iRtn=GE_SRV_TYPE_COMPANY;
			else
				iRtn=GE_SRV_TYPE_SET;
		}
		else
			iRtn=GE_SRV_TYPE_SET;
	}
	return iRtn;
}

int CImplement::SysGetPlantIPofSubCorpEx(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int lnPlantIndex = 0;

	int nMaxPlantCount = 150;

	int lnRealSetPlantCount = 0;

	int nStartIndex = pFixComm->GetLong(FIELD_SERVICE_STARTINDEX);
	
	CString lstrUserName = pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString lstrVersion = pFixComm->GetItem(FIELD_SYSTEM_VERSION);



	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	//根据用户获取设备
	UserInfo_t loUserInfo;

	UserManager::GetUserInfo(lstrUserName,loUserInfo);

	IdSet_t loCacheMachineIDList;

	if (loUserInfo.UserId == -1)//匿名
	{
		loCacheMachineIDList = UserManager::GetTotalMachineIDList();	
	}
	else
	{
		UserManager::GetMachineIDList(loUserInfo.UserId, loCacheMachineIDList);
	}

	for (IdSet_t::iterator lpIter = loCacheMachineIDList.begin();
		 lpIter != loCacheMachineIDList.end();
		 ++lpIter)
	{
		int lnMachineID = *lpIter;

		PlantInfo_t loPlantInfo;

		UserManager::GetSpecMachineIDPlantInfo(lnMachineID,loPlantInfo);

		int lnPlantDatasource = DatasourceHelper::GetSpecVersionPlantDatasource(loPlantInfo.DataSource, lstrVersion);

		MAP_SETIPINFO::iterator itMap;
		string sSetID;
		if (g_iSvrLevel==GE_SRV_TYPE_SET)
		{
			if (lnPlantIndex>=nStartIndex)
			{
				pFixComm->SetItem(FIELD_SYSTEM_GROUP,loPlantInfo.GroupName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,loPlantInfo.CompanyName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loPlantInfo.FactoryName);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_SET,loPlantInfo.SetID);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loPlantInfo.GraphType);
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,loPlantInfo.MachineType);
				pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,loPlantInfo.Db_Id);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, loPlantInfo.PlantNameDisplay);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,loPlantInfo.PlantErpCode);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_DISPLAY,loPlantInfo.GroupDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY,loPlantInfo.CompanyDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY,loPlantInfo.FactoryDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_SORTNO,loPlantInfo.GroupSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_SORTNO,loPlantInfo.CompanySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_SORTNO,loPlantInfo.FactorySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_SORTNO,loPlantInfo.PlantSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,loPlantInfo.CompanyAlias);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID2,loPlantInfo.PlantNo);
				sSetID=loPlantInfo.SetID;
				itMap=g_mapSetInfo.find(sSetID);
				if (itMap!=g_mapSetInfo.end())
				{
					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetTmIP().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetTmPort());
					pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,itMap->second.GetSetDisplay().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_SET_SORTNO,itMap->second.GetSetSortNO());
				}

				//点火顺序
				CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

				//上止点相位
				lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

				//点火提前角顺序
				lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

				//气阀开启相位
				lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

				//气阀关闭相位
				lstrValue = GetValueByKey(goMapValveClosePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_CLOSE_PHASE,lstrValue);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				lnRealSetPlantCount++;
			}
			lnPlantIndex++;

			if (lnRealSetPlantCount >=nMaxPlantCount)
			{
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				return lnPlantIndex;
			}
		}
		else if(g_iSvrLevel==GE_SRV_TYPE_COMPANY)
		{
			if (lnPlantIndex>=nStartIndex)
			{
				pFixComm->SetItem(FIELD_SYSTEM_GROUP,loPlantInfo.GroupName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,loPlantInfo.CompanyName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loPlantInfo.FactoryName);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loPlantInfo.GraphType);
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,loPlantInfo.MachineType);
				pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,loPlantInfo.Db_Id);
				pFixComm->SetItem(FIELD_SYSTEM_SET,loPlantInfo.SetID);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, loPlantInfo.PlantNameDisplay);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,loPlantInfo.PlantErpCode);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_DISPLAY,loPlantInfo.GroupDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY,loPlantInfo.CompanyDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY,loPlantInfo.FactoryDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_SORTNO,loPlantInfo.GroupSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_SORTNO,loPlantInfo.CompanySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_SORTNO,loPlantInfo.FactorySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_SORTNO,loPlantInfo.PlantSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,loPlantInfo.CompanyAlias);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID2,loPlantInfo.PlantNo);
				sSetID=loPlantInfo.SetID;
				itMap=g_mapSetInfo.find(sSetID);
				if (itMap!=g_mapSetInfo.end())
				{
					pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,itMap->second.GetSetDisplay().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());
					pFixComm->SetItem(FIELD_SYSTEM_SET_SORTNO,itMap->second.GetSetSortNO());
				}

				//点火顺序
				CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

				//上止点相位
				lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

				//点火提前角顺序
				lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

				//气阀开启相位
				lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

				//气阀关闭相位
				lstrValue = GetValueByKey(goMapValveClosePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_CLOSE_PHASE,lstrValue);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
				lnRealSetPlantCount++;

			}
			lnPlantIndex++;

			if (lnRealSetPlantCount >=nMaxPlantCount)
			{

				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				return lnPlantIndex;
			}
		}
		else
		{
			if (lnPlantIndex>=nStartIndex)
			{
				pFixComm->SetItem(FIELD_SYSTEM_GROUP,loPlantInfo.GroupName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,loPlantInfo.CompanyName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loPlantInfo.FactoryName);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loPlantInfo.GraphType);
				pFixComm->SetItem(FIELD_SYSTEM_TYPE,loPlantInfo.MachineType);
				pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,loPlantInfo.Db_Id);
				pFixComm->SetItem(FIELD_SYSTEM_SET,loPlantInfo.SetID);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, loPlantInfo.PlantNameDisplay);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,loPlantInfo.PlantErpCode);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_DISPLAY,loPlantInfo.GroupDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_DISPLAY,loPlantInfo.CompanyDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_DISPLAY,loPlantInfo.FactoryDisplayName);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP_SORTNO,loPlantInfo.GroupSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_SORTNO,loPlantInfo.CompanySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY_SORTNO,loPlantInfo.FactorySortNO);
				pFixComm->SetItem(FIELD_SYSTEM_PLANT_SORTNO,loPlantInfo.PlantSortNO);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,loPlantInfo.CompanyAlias);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID2,loPlantInfo.PlantNo);
				sSetID=loPlantInfo.SetID;
				itMap=g_mapSetInfo.find(sSetID);
				lnRealSetPlantCount++;
				if (itMap!=g_mapSetInfo.end())
				{
					//装置级ID信息
					pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,itMap->second.GetSetDisplay().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());
					pFixComm->SetItem(FIELD_SYSTEM_SET_SORTNO,itMap->second.GetSetSortNO());

					CString lstrCompanyIP = itMap->second.GetParentIP().c_str();
					int lnCompanyPort = itMap->second.GetParentPort();

					sSetID=itMap->second.GetParentSetID().c_str();
					itMap=g_mapSetInfo.find(sSetID);

					if (itMap!=g_mapSetInfo.end())
					{
						sSetID=itMap->second.GetParentSetID().c_str();

						itMap=g_mapSetInfo.find(sSetID);

						if(itMap!=g_mapSetInfo.end())
						{
							CString lstrGroupIP = itMap->second.GetTmIP().c_str();
							int		lnGroupIP   = itMap->second.GetTmPort();

							if (lstrGroupIP.GetLength() == 0)
							{
								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,lstrCompanyIP);
								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,lnCompanyPort);	

							}else
							{
								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,itMap->second.GetTmIP().c_str());
								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,itMap->second.GetTmPort());
							}

						}else
						{
							pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,lstrCompanyIP);
							pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,lnCompanyPort);		
						}
					}


					//点火顺序
					CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

					//上止点相位
					lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

					//点火提前角顺序
					lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

					//气阀开启相位
					lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

					//气阀关闭相位
					lstrValue = GetValueByKey(goMapValveClosePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_VALVE_CLOSE_PHASE,lstrValue);

				}
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
			lnPlantIndex++;

			if (lnRealSetPlantCount >=nMaxPlantCount)
			{
				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());
				return lnPlantIndex;
			}
		}
	}

	//for (std::map<CString,FactoryDataTypeEx>::iterator lpIt = g_FactoryData.begin();
	//	 lpIt != g_FactoryData.end();
	//	 lpIt ++)
	//{
	//	list<S_PlantInfoEx>& lPlantInfo = lpIt->second;
	//	
	//	string sSetID;
	//	list<S_PlantInfoEx>::iterator it;
	//	MAP_SETIPINFO::iterator itMap;

	//	if (g_iSvrLevel==GE_SRV_TYPE_SET)
	//	{
	//		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
	//		{
	//			S_PlantInfo lpInfo = it->_plantInfo;
	//			if (lnPlantIndex>=nStartIndex)
	//			{
	//				pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
	//				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,lpInfo._cCompany);
	//				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,lpInfo._cFactory);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT,lpInfo._cPlantID);
	//				pFixComm->SetItem(FIELD_SYSTEM_SET,lpInfo._cSetID);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,lpInfo._iGraphType);
	//				pFixComm->SetItem(FIELD_SYSTEM_TYPE,lpInfo._iMachineType);
	//				pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lpInfo._iDataSource);
	//				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,lpInfo.m_nDbId);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
	//				sSetID=lpInfo._cSetID;
	//				itMap=g_mapSetInfo.find(sSetID);
	//				if (itMap!=g_mapSetInfo.end())
	//				{
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetTmIP().c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetTmPort());
	//				}
	//				pFixComm->Write(pFixComm->GetAci());
	//				pFixComm->CreateNextAnswer();
	//				lnRealSetPlantCount++;
	//			}
	//			lnPlantIndex++;

	//			if (lnRealSetPlantCount >=nMaxPlantCount)
	//			{
	//				pFixComm->CreateEndMak();
	//				pFixComm->Write(pFixComm->GetAci());
	//				return lnPlantIndex;
	//			}
	//		}
	//	}
	//	else if(g_iSvrLevel==GE_SRV_TYPE_COMPANY)
	//	{
	//		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
	//		{
	//			S_PlantInfo lpInfo = it->_plantInfo;

	//			if (lnPlantIndex>=nStartIndex)
	//			{
	//				pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
	//				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,lpInfo._cCompany);
	//				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,lpInfo._cFactory);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT,lpInfo._cPlantID);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,lpInfo._iGraphType);
	//				pFixComm->SetItem(FIELD_SYSTEM_TYPE,lpInfo._iMachineType);
	//				pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lpInfo._iDataSource);
	//				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,lpInfo.m_nDbId);
	//				pFixComm->SetItem(FIELD_SYSTEM_SET,lpInfo._cSetID);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
	//				sSetID=lpInfo._cSetID;
	//				itMap=g_mapSetInfo.find(sSetID);
	//				if (itMap!=g_mapSetInfo.end())
	//				{
	//					pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
	//					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());
	//				}
	//				pFixComm->Write(pFixComm->GetAci());
	//				pFixComm->CreateNextAnswer();
	//				lnRealSetPlantCount++;

	//			}
	//			lnPlantIndex++;

	//			if (lnRealSetPlantCount >=nMaxPlantCount)
	//			{
	//				
	//				pFixComm->CreateEndMak();
	//				pFixComm->Write(pFixComm->GetAci());
	//				return lnPlantIndex;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
	//		{
	//			S_PlantInfo lpInfo = it->_plantInfo;

	//			if (lnPlantIndex>=nStartIndex)
	//			{
	//				pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
	//				pFixComm->SetItem(FIELD_SYSTEM_COMPANY,lpInfo._cCompany);
	//				pFixComm->SetItem(FIELD_SYSTEM_FACTORY,lpInfo._cFactory);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT,lpInfo._cPlantID);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,lpInfo._iGraphType);
	//				pFixComm->SetItem(FIELD_SYSTEM_TYPE,lpInfo._iMachineType);
	//			pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lpInfo._iDataSource);
	//				pFixComm->SetItem(FIELD_SERVICE_ENDINDEX,lnPlantIndex);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,nMaxPlantCount);
	//				pFixComm->SetItem(FIELD_SERVICE_PLANT_DBTYPE,lpInfo.m_nDbId);
	//				pFixComm->SetItem(FIELD_SYSTEM_SET,lpInfo._cSetID);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
	//				pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
	//				sSetID=lpInfo._cSetID;
	//				itMap=g_mapSetInfo.find(sSetID);
	//				lnRealSetPlantCount++;
	//				if (itMap!=g_mapSetInfo.end())
	//				{
	//					//装置级ID信息
	//					pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
	//					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
	//					pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());

	//					CString lstrCompanyIP = itMap->second.GetParentIP().c_str();
	//					int lnCompanyPort = itMap->second.GetParentPort();

	//					sSetID=itMap->second.GetParentSetID().c_str();
	//					itMap=g_mapSetInfo.find(sSetID);

	//					if (itMap!=g_mapSetInfo.end())
	//					{
	//						sSetID=itMap->second.GetParentSetID().c_str();

	//						itMap=g_mapSetInfo.find(sSetID);

	//						if(itMap!=g_mapSetInfo.end())
	//						{
	//							CString lstrGroupIP = itMap->second.GetTmIP().c_str();
	//							int		lnGroupIP   = itMap->second.GetTmPort();

	//							if (lstrGroupIP.GetLength() == 0)
	//							{
	//								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,lstrCompanyIP);
	//								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,lnCompanyPort);	

	//							}else
	//							{
	//								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,itMap->second.GetTmIP().c_str());
	//								pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,itMap->second.GetTmPort());
	//							}

	//						}else
	//						{
	//							pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,lstrCompanyIP);
	//							pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,lnCompanyPort);		
	//						}
	//					}
	//					

	//				}
	//				pFixComm->Write(pFixComm->GetAci());
	//				pFixComm->CreateNextAnswer();
	//			}
	//			lnPlantIndex++;

	//			if (lnRealSetPlantCount >=nMaxPlantCount)
	//			{
	//				pFixComm->CreateEndMak();
	//				pFixComm->Write(pFixComm->GetAci());
	//				return lnPlantIndex;
	//			}
	//		}
	//	}
	//}

	//

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return lnPlantIndex;

}

/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
*输入参数：公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*返回值的字段码：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
*          集团级IP   FIELD_SYSTEM_GROUP_MW_IP
*          集团级Port FIELD_SYSTEM_GROUP_MW_PORT
*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
*          装置级IP   FIELD_SYSTEM_MW_IP
*          装置级Port FIELD_SYSTEM_MW_PORT
*          装置级中间件ID   FIELD_SYSTEM_SET
*          设备类型   FIELD_SYSTEM_TYPE
*          数据源     FIELD_SYSTEM_DATASOURCE
*          显示名称   FIELD_SYSTEM_PLANT_DISPLAY
*          装置显示名称FIELD_SYSTEM_SET_DISPLAY
*          ERP编码    FIELD_SYSTEM_PLANT_ERP_CODE
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
*/
int CImplement::SysGetPlantIPofSubCorp(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
/*
	if(!SwitchDbNameEx(pDB,g_strMainDbName)){
		HZLOG_DEBUG(STR("切换数据库%s失败", g_strMainDbName));

		::SetNullReturnFixcommEx(pFixComm);
		return -1;
	}*/


	int nCount=0;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CString lstrCompany = sCompany.c_str();
	CString lstrFactory = sFactory.c_str();
	CString lstrKey = lstrCompany +_T(":")+lstrFactory;
	list<S_PlantInfoEx>& lPlantInfo = g_FactoryData[lstrKey];

	CString lstrVersion = pFixComm->Get(FIELD_SYSTEM_VERSION);
	
	bool lbVersionDeprecated = DatasourceHelper::IsVersionDeprecated(lstrVersion);

/*
	S_PlantInfo plantInfo;
	int iDbID=GetCompanyDBID(pDB,sCompany);
	if (iDbID>0)
		pDB->Cmd("select * from [%s_%d] ",gc_cMachineTable,iDbID);
	else
		pDB->Cmd("select * from [%s] ",gc_cMachineTable);
	pDB->Cmd(" where [%s]='%s' ",gc_cCompany,sCompany.c_str());
	if(!(sFactory.empty()))
		pDB->Cmd(" AND [%s]='%s' ",gc_cFactory_Name,sFactory.c_str());
	pDB->Cmd(" order by [%s]",gc_cSetID);
	while (pDB->More())
	{
       plantInfo.GetPlantInfo(pDB);
	   lPlantInfo.push_back(plantInfo);
	}*/
	
	string sSetID;
	list<S_PlantInfoEx>::iterator it;
	MAP_SETIPINFO::iterator itMap;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	if (g_iSvrLevel==GE_SRV_TYPE_SET)
	{
		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
		{
			S_PlantInfo& loInfo = it->_plantInfo;

			int lnPlantDatasource = DatasourceHelper::GetSpecVersionPlantDatasource(loInfo._iDataSource, lbVersionDeprecated);

			pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loInfo._cFactory);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT,loInfo._cPlantID);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loInfo._iGraphType);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,loInfo._iMachineType);
			pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,it->m_strSetDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
			sSetID=loInfo._cSetID;
			itMap=g_mapSetInfo.find(sSetID);
			if (itMap!=g_mapSetInfo.end())
			{
				pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetTmIP().c_str());
				pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetTmPort());
			}
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		    nCount++;
		}
	}
	else if(g_iSvrLevel==GE_SRV_TYPE_COMPANY)
	{
		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
		{
			S_PlantInfo& loInfo = it->_plantInfo;

			int lnPlantDatasource = DatasourceHelper::GetSpecVersionPlantDatasource(loInfo._iDataSource, lbVersionDeprecated);

			pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loInfo._cFactory);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT,loInfo._cPlantID);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loInfo._iGraphType);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,loInfo._iMachineType);
			pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,it->m_strSetDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
			sSetID=loInfo._cSetID;
            itMap=g_mapSetInfo.find(sSetID);
			if (itMap!=g_mapSetInfo.end())
			{
				pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
				pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
				pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());
			}
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			nCount++;
		}
	}
	else
	{
		for (it=lPlantInfo.begin();it!=lPlantInfo.end();++it)
		{
			S_PlantInfo& loInfo = it->_plantInfo;

			int lnPlantDatasource = DatasourceHelper::GetSpecVersionPlantDatasource(loInfo._iDataSource, lbVersionDeprecated);

			pFixComm->SetItem(FIELD_SYSTEM_GROUP,it->m_strGroup);
			pFixComm->SetItem(FIELD_SYSTEM_COMPANY,sCompany.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_FACTORY,loInfo._cFactory);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT,loInfo._cPlantID);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,loInfo._iGraphType);
			pFixComm->SetItem(FIELD_SYSTEM_TYPE,loInfo._iMachineType);
			pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_DISPLAY, it->m_strPlantDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_SET_DISPLAY,it->m_strSetDisplayName);
			pFixComm->SetItem(FIELD_SYSTEM_PLANT_ERP_CODE,it->m_strErpCode);
			sSetID=loInfo._cSetID;
			itMap=g_mapSetInfo.find(sSetID);
			if (itMap!=g_mapSetInfo.end())
			{
				//装置级ID信息
				pFixComm->SetItem(FIELD_SYSTEM_SET,sSetID.c_str());
				pFixComm->SetItem(FIELD_SYSTEM_MW_IP,itMap->second.GetSetIP().c_str());
				pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,itMap->second.GetSetPort());
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_IP,itMap->second.GetParentIP().c_str());
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY_MW_PORT,itMap->second.GetParentPort());
				sSetID=itMap->second.GetParentSetID().c_str();
				itMap=g_mapSetInfo.find(sSetID);
				if (itMap!=g_mapSetInfo.end())
				{
					sSetID=itMap->second.GetParentSetID().c_str();
					itMap=g_mapSetInfo.find(sSetID);
					if(itMap!=g_mapSetInfo.end())
					{
					//	CString strTmp;strTmp.Format("%s,%s",sSetID.c_str(),itMap->second.GetSetID().c_str());  AfxMessageBox(strTmp);
						pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_IP,itMap->second.GetTmIP().c_str());
						pFixComm->SetItem(FIELD_SYSTEM_GROUP_MW_PORT,itMap->second.GetTmPort());
					}
				}
			}
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			nCount++;
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
    return nCount;
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
int CImplement::SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	string sCompany  = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory  = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant    = pFixComm->Get(FIELD_SYSTEM_PLANT);
	string sPlantNo = CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	pDB->Cmd("select [%s].[%s], [%s].[%s], [%s].[%s], [%s].[%s] from [%s],[%s]", gc_cChannTable, gc_cChannID,
																				 gc_cMachInfoTypeTable, gc_cIndex,
																				 gc_cMachInfoTypeTable, gc_cComment,
																				 gc_cMachInfoTypeTable, gc_cUnit,
																				 gc_cChannTable, gc_cMachInfoTypeTable);
	pDB->Cmd(" where [%s]='%s' and [%s]='%s' and [%s]='%s' and [%s].[%s]= [%s].[%s]", gc_cCompany, sCompany.c_str(),
																					  gc_cFactory_Name, sFactory.c_str(),
																			          gc_cPlantNo, sPlantNo.c_str(),
																			          gc_cChannTable, gc_cInfoType,
																					  gc_cMachInfoTypeTable, gc_cInfoType);
	pDB->Cmd(" ORDER BY 1, 2");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		//CString strTmp;strTmp.Format("%s,%d,%s,%d",pDB->Get(gc_cComment),pDB->GetLong(gc_cIndex),pDB->Get(2),pDB->GetLong(1));
		//AfxMessageBox(strTmp);
	    pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cIndex));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,pDB->Get(gc_cUnit).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的信息描述表
* 输入字段码：
*返回的字段码：
*        信息类型      FIELD_SYSTEM_TYPE
*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
*/
int CImplement::SysGetInfoTypeDetail(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] ",gc_cInfoTypeDetail);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cInfoType));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**根据测点信息类型,得到监测中心的信息描述表
* 输入字段码：
*        信息类型   FIELD_SYSTEM_TYPE
*返回的字段码：
*        索引       FIELD_SYSTEM_STATUS
*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*        单位       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
*/
int CImplement::SysGetInfoTypeDetailBaseType(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0,iInfoType=0;
   	iInfoType = pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	//查询设备信息表获得特征值类型所对应的特征值索引，描述和单位
	pDB->Cmd("select [%s],[%s],[%s] from [%s] ",gc_cIndex,gc_cComment,gc_cUnit,gc_cMachInfoTypeTable);
	pDB->Cmd(" where [%s]=%d ",gc_cInfoType,iInfoType);
	pDB->Cmd(" ORDER BY [%s]",gc_cIndex);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cIndex));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,pDB->Get(gc_cUnit).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
*输入参数：   公司名         FIELD_SYSTEM_COMPANY
*             分厂名         FIELD_SYSTEM_FACTORY
*             机组名         FIELD_SYSTEM_PLANT
*             测点名         FIELD_SYSTEM_CHANN
*             是否常规报警   FIELD_SYSTEM_STATUS //1为判断报警，0为不判断报警
*             报警限类型     FIELD_SYSTEM_EVENT_TYPE
*             报警上限       FIELD_SYSTEM_ALARM_LOW
*             报警上上限     FIELD_SYSTEM_ALARM_HIGH
*             报警下限       FIELD_SYSTEM_ALARM_LOW2
*             报警下下限     FIELD_SYSTEM_ALARM_HIGH2
*返回值的字段码：
*             返回状态       FIELD_SYSTEM_STATUS
#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115
*/
int CImplement::SysModifySpecChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iStatus=-1;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->Get(FIELD_SYSTEM_PLANT);
	string sChann=pFixComm->Get(FIELD_SYSTEM_CHANN);
	int iIsAlarm=pFixComm->GetLong(FIELD_SYSTEM_STATUS);
	bool bRegular=iIsAlarm==1?true:false;
	int iType=pFixComm->GetLong(FIELD_SYSTEM_EVENT_TYPE);
	float fAlarmLow=pFixComm->GetDouble(FIELD_SYSTEM_ALARM_LOW);
	float fAlarmHigh=pFixComm->GetDouble(FIELD_SYSTEM_ALARM_HIGH);
	float fAlarmLow2=pFixComm->GetDouble(FIELD_SYSTEM_ALARM_LOW2);
	float fAlarmHigh2=pFixComm->GetDouble(FIELD_SYSTEM_ALARM_HIGH2);

	CChannAlarmParam cAlarmParam;
	long nSize = sizeof(CChannAlarmParam);
	string sPlantNo = CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	pDB->Cmd("SELECT [%s] FROM [%s] ",gc_cAlarmParam,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cPlantNo,sPlantNo.c_str(),gc_cChannID,sChann.c_str());
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More(TRUE))
	{
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cAlarmParam,  (BYTE*)&cAlarmParam,  nSize, sWaveHead);
		cAlarmParam._bRegular=bRegular;
		cAlarmParam._iRegJudgeType=iType;
		cAlarmParam._fAlarmHL=fAlarmLow;
		cAlarmParam._fAlarmHH=fAlarmHigh;
		cAlarmParam._fAlarmLH=fAlarmLow2;
		cAlarmParam._fAlarmLL=fAlarmHigh2;
		pDB->SetBuffer(gc_cAlarmParam,(BYTE*)&cAlarmParam,nSize);
		iStatus=1;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	}
	else
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm->Write(pFixComm->GetAci());
	return iStatus;
}

/**得到监测中心的自定义参数模板表
* 输入字段码：
*返回的字段码：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID       FIELD_SYSTEM_TEMPID
*         自定义信息描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRTEMPLET      25116
*/
int CImplement::SysGetSvrTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cTemplet_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT, pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,pDB->Get(gc_cTempletID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cParam_Comment).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的自定义参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
*         预处理类型             FIELD_SYSTEM_TYPE
*         包络频带起始值         FIELD_SYSTEM_BAND_STARTVALUE
*         包络频带结束值         FIELD_SYSTEM_BAND_ENDVALUE
*         预处理参数K             FIELD_SERVICE_PREPROC_K
*         预处理参数B             FIELD_SERVICE_PREPROC_B
*         单位                   FIELD_SERVICE_UNIT
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM      25117
*/
int CImplement::SysGetCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cCustom_Param_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cID));
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT, pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,pDB->Get(gc_cTempletID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_DATA,pDB->GetLong(gc_cData_Type));
		pFixComm->SetItem(FIELD_SYSTEM_UNIT,pDB->GetLong(gc_cUnit_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,pDB->GetLong(gc_cSignal_Ana_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_STARTNO,pDB->GetLong(gc_cSeg_Start_NO));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cSeg_Num));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cSegName).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_START_VALUE,pDB->GetDouble(gc_cData_Start_Value));
		pFixComm->SetItem(FIELD_SYSTEM_END_VALUE,pDB->GetDouble(gc_cData_End_Value));
		pFixComm->SetItem(FIELD_SYSTEM_CHAR_TYPE,pDB->GetLong(gc_cChar_Type));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cPreProc_Type));
		pFixComm->SetItem(FIELD_SYSTEM_BAND_STARTVALUE,pDB->GetDouble(gc_cBand_Start_Value));
		pFixComm->SetItem(FIELD_SYSTEM_BAND_ENDVALUE,pDB->GetDouble(gc_cBand_End_Value));
		pFixComm->SetItem(FIELD_SERVICE_PREPROC_K, pDB->GetDouble(gc_cPreProc_K));
		pFixComm->SetItem(FIELD_SERVICE_PREPROC_B, pDB->GetDouble(gc_cPreProc_B));
		pFixComm->SetItem(FIELD_SERVICE_UNIT, pDB->Get(gc_cUnit).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的自定义参数报警参数模板表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID         FIELD_SYSTEM_TEMPID
*         自定义报警模板ID         FIELD_SYSTEM_ALARM_TEMPID
*         自定义报警描述           FIELD_SERVICE_ALARM_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET      25118
*/
int CImplement::SysGetSvrAlarmTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cAlarm_Templet_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT, pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,pDB->Get(gc_cTempletID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_TEMPID,pDB->Get(gc_cAlarm_TempletID).c_str());
		pFixComm->SetItem(FIELD_SERVICE_ALARM_DISCRIPTION,pDB->Get(gc_cAlarm_Param_Comment).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119
*/
int CImplement::SysGetSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cAlarm_Param_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cID));
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT, pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_TEMPID,pDB->Get(gc_cAlarm_TempletID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cJudge_Type));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,pDB->GetLong(gc_cAlarm_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_STARTNO,pDB->GetLong(gc_cAlarm_Start_SegNO));
        pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cSeg_Num));
        pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cSegName).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,pDB->GetLong( gc_cIF_Alarm));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,pDB->GetDouble(gc_cAlarm_LTH));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,pDB->GetDouble(gc_cAlarm_HTH));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_GROUP,pDB->GetLong(gc_cAlarm_Group));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的通道自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         测点号                 FIELD_SYSTEM_CHANN
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM     25120
*/
int CImplement::SysGetSvrChannAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cChann_Alarm_Param_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cID));
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantNo).c_str());
	    pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cJudge_Type));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,pDB->GetLong(gc_cAlarm_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_STARTNO,pDB->GetLong(gc_cAlarm_Start_SegNO));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cSeg_Num));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cSegName).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,pDB->GetLong( gc_cIF_Alarm));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,pDB->GetDouble(gc_cAlarm_LTH));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,pDB->GetDouble(gc_cAlarm_HTH));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_GROUP,pDB->GetLong(gc_cAlarm_Group));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定公司、分厂和装置ID的自定义参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID                FIELD_SYSTEM_SET
*         设备名                FIELD_SERVICE_PLANTID
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         索引ID                 FIELD_SYSTEM_STATUS
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
*		  特征值单位			 FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121
*/
int CImplement::SysGetSpecSvrCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
	string sPlantID = pFixComm->Get(FIELD_SERVICE_PLANTID);

	CString lsCondion;

	if (!sCompany.empty())
	{
		if (!lsCondion.IsEmpty())
		{
			lsCondion.Append(" and ");
		}

		lsCondion.AppendFormat("%s='%s'", gc_cCompany, sCompany.c_str());
	}

	if (!sFactory.empty())
	{
		if (!lsCondion.IsEmpty())
		{
			lsCondion.Append(" and ");
		}

		lsCondion.AppendFormat("%s='%s'", gc_cFactory_Name, sFactory.c_str());
	}


	if (!sSetID.empty())
	{
		if (!lsCondion.IsEmpty())
		{
			lsCondion.Append(" and ");
		}

		lsCondion.AppendFormat("%s='%s'", gc_cSetID, sSetID.c_str());
	}


	if (!sPlantID.empty())
	{
		pDB->Cmd("SELECT %s FROM %s WHERE %s='%s'", 
			gc_cPlantNo,
			gc_cMachineTable, 
			gc_cPlantID, sPlantID.c_str());
		if (!pDB->More())
		{
			pFixComm->CreateAnswerMul(pFixComm->GetFunc());
			pFixComm->CreateEndMak();
			pFixComm->Write(pFixComm->GetAci());
			return -1;
		}

		if (!lsCondion.IsEmpty())
		{
			lsCondion.Append(" and ");
		}

		lsCondion.AppendFormat("%s='%s'", gc_cPlantNo, pDB->Get(gc_cPlantNo).c_str());
	}

	pDB->Cmd("select * from [%s] ", gc_cCustom_Param_Table);
	if (!lsCondion.IsEmpty())
	{
		pDB->Cmd(" where %s ", lsCondion);
	}
	pDB->Cmd(" order by [%s]",gc_cID);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	///[TBD]
	CString strTempID ="";
	int idxCustomData= -1;
	while (pDB->More())
	{
		nCount++;

		pFixComm->SetItem(FIELD_SYSTEM_COMPANY, pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,   pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,     pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,   pDB->Get(gc_cPlantNo).c_str());
	
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cID));
		strTempID = pDB->Get(gc_cTempletID).c_str();
		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,strTempID);
		pFixComm->SetItem(FIELD_SYSTEM_DATA,pDB->GetLong(gc_cData_Type));
		pFixComm->SetItem(FIELD_SYSTEM_UNIT,pDB->GetLong(gc_cUnit_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SIGNAL_TYPE,pDB->GetLong(gc_cSignal_Ana_Type));
		idxCustomData = pDB->GetLong(gc_cSeg_Start_NO);
		pFixComm->SetItem(FIELD_SYSTEM_SEG_STARTNO, idxCustomData);
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cSeg_Num));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cSegName).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_START_VALUE,pDB->GetDouble(gc_cData_Start_Value));
		pFixComm->SetItem(FIELD_SYSTEM_END_VALUE,pDB->GetDouble(gc_cData_End_Value));
		pFixComm->SetItem(FIELD_SYSTEM_CHAR_TYPE,pDB->GetLong(gc_cChar_Type));
		CString strUnit = "";
		if (strTempID.CompareNoCase(gc_cTemplateID_WINDPUMP) == 0)
		{
			switch(idxCustomData)
			{
				case GE_VF_HALF:
				case GE_VF_FIVE:
				case GE_VF_ONEX:
				case GE_VF_TWO:
				case GE_VF_THREE:
				case GE_VF_FOUR:
				case GE_VF_BLADE:
					strUnit ="mm/s";
				break;
				case GE_ENVSPECCF1:			///内圈特征频率         m/s2
				case GE_ENVSPECCF2:			///外圈特征频率         m/s2
				case GE_ENVSPECCF3:			///保持架特征频率       m/s2
				case GE_ENVSPECCF4:			///滚动体特征频率       m/s2
				case GE_USER_AENVFREQ2:		///输出轴转频			m/s2
				case GE_GEARBOXSHAFTCF1:	///齿轮箱I轴转频		m/s2
				case GE_GEARBOXSHAFTCF2:	///齿轮箱I轴转频		m/s2
				case GE_GEARBOXSHAFTCF3:	///齿轮箱I轴转频		m/s2
				case GE_GEARBOXSHAFTCF4:	///齿轮箱I轴转频		m/s2
					strUnit ="m/s2";
				break;
				case  GE_USER_AENVFREQ1:
					strUnit ="m/s2";
				break;
				default:
				"";
			}
		}
		else
		{
			
			try
			{
				strUnit = pDB->Get(gc_cUnit).c_str();
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
				strUnit = "";
			}


			if(strUnit.TrimRight().GetLength() == 0)
			{	
				if (strTempID.CompareNoCase(gc_cTemplateID_SINK) == 0)
				{
					strUnit = "um";

				}else
				{
					strUnit = "m/s2";
				}				
			}
		}
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,strUnit);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定公司、分厂和装置ID的自定义参数报警参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM       25122
*/
int CImplement::SysGetSpecSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sSetID=pFixComm->Get(FIELD_SYSTEM_SET);
	pDB->Cmd("select * from [%s] ",gc_cAlarm_Param_Table);
	pDB->Cmd(" where %s='%s' and %s='%s' and %s='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cSetID,sSetID.c_str());
	pDB->Cmd(" order by [%s]",gc_cID);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cID));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_TEMPID,pDB->Get(gc_cAlarm_TempletID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cJudge_Type));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_TYPE,pDB->GetLong(gc_cAlarm_Type));
		zdlTraceLine(_T("************** %d*************\r\n"),pDB->GetLong(gc_cAlarm_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_STARTNO,pDB->GetLong(gc_cAlarm_Start_SegNO));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cSeg_Num));
		pFixComm->SetItem(FIELD_SYSTEM_IF_ALARM,pDB->GetLong( gc_cIF_Alarm));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_LOW,pDB->GetDouble(gc_cAlarm_LTH));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_HIGH,pDB->GetDouble(gc_cAlarm_HTH));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_GROUP,pDB->GetLong(gc_cAlarm_Group));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心配置的自定义轴承参数表
* 输入字段码：
*返回的字段码：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         轴承索引ID             FIELD_SYSTEM_STATUS
*         轴承名称               FIELD_SYSTEM_SEG_NAME
*         轴承类型               FIELD_SYSTEM_TYPE
*         滚珠数目               FIELD_SYSTEM_SEG_NUM
*         滚珠直径               FIELD_SYSTEM_START_VALUE
*         节径                   FIELD_SYSTEM_END_VALUE
*         压力角                 FIELD_SERVICE_ANGLEPARAM1
*         外壳直径               FIELD_SYSTEM_BAND_STARTVALUE
*         内径                   FIELD_SYSTEM_BAND_ENDVALUE
*         滚珠排数               FIELD_SYSTEM_AXISCHANN_NUM
*         轴承代码               FIELD_SYSTEM_CHAR_TYPE
*         同步状态标识           FIELD_SYSTEM_SENDC_IF
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123
*/
int CImplement::SysGetSetSvrCustomBearingTbl(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cCustom_Bearing_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cBearing_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cBearing_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cBearing_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cElements_Num));
		pFixComm->SetItem(FIELD_SYSTEM_START_VALUE,pDB->GetDouble(gc_cElement_Diameter));
		pFixComm->SetItem(FIELD_SYSTEM_END_VALUE,pDB->GetDouble(gc_cPith_Diameter));
		pFixComm->SetItem(FIELD_SERVICE_ANGLEPARAM1,pDB->GetDouble(gc_cContact_Angle));
		pFixComm->SetItem(FIELD_SYSTEM_BAND_STARTVALUE,pDB->GetDouble(gc_cHousing_Diameter));
		pFixComm->SetItem(FIELD_SYSTEM_BAND_ENDVALUE,pDB->GetDouble( gc_cBore_Diameter));
		pFixComm->SetItem(FIELD_SYSTEM_AXISCHANN_NUM,pDB->GetLong(gc_cRows_Num));
		pFixComm->SetItem(FIELD_SYSTEM_CHAR_TYPE,pDB->GetLong(gc_cBearing_Code));
		pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,pDB->GetLong(gc_cIs_Sync));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的齿轮箱参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         齿轮箱索引ID           FIELD_SYSTEM_STATUS
*         齿轮箱类别             FIELD_SYSTEM_TYPE
*         齿轮箱名称             FIELD_SYSTEM_SEG_NAME
*         行星齿轮驱动方式       FIELD_SYSTEM_CHAR_TYPE
*         太阳轮齿数             FIELD_SERVICE_PLANT_NUM
*         行星轮个数             FIELD_SERVICE_ALARM_PLANTNUM
*         行星轮齿数             FIELD_SERVICE_RUN_PLANTNUM
*         银河系齿数             FIELD_SERVICE_STOP_PLANTNUM
*         齿轮级数               FIELD_SYSTEM_USER_LEVEL
*         齿数Z12                FIELD_SYSTEM_GEARNUMZ12
*         齿数Z21                FIELD_SYSTEM_GEARNUMZ21
*         齿数Z22                FIELD_SYSTEM_GEARNUMZ22
*         齿数Z31                FIELD_SYSTEM_GEARNUMZ31
*         齿数Z32                FIELD_SYSTEM_GEARNUMZ32
*         齿数Z41                FIELD_SYSTEM_GEARNUMZ41
*         齿数Z42                FIELD_SYSTEM_GEARNUMZ42
*         齿数Z51                FIELD_SYSTEM_GEARNUMZ51
*         齿数Z52                FIELD_SYSTEM_GEARNUMZ52
*         齿数Z61                FIELD_SYSTEM_GEARNUMZ61
*         同步状态标识           FIELD_SYSTEM_SENDC_IF
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124
*/
int CImplement::SysGetSetSvrGearBoxTbl(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cGear_Box_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cGearBox_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cGearBox_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cGearBox_Name).c_str());
        pFixComm->SetItem(FIELD_SYSTEM_CHAR_TYPE,pDB->Get(gc_cDriver_Type).c_str());
		pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,pDB->GetLong(gc_cGear_Num_Sun));
		pFixComm->SetItem(FIELD_SERVICE_ALARM_PLANTNUM,pDB->GetLong(gc_cPlanet_Num));
		pFixComm->SetItem(FIELD_SERVICE_RUN_PLANTNUM,pDB->GetLong(gc_cGear_Num_Planet));
		pFixComm->SetItem(FIELD_SERVICE_STOP_PLANTNUM,pDB->GetLong(gc_cGear_Num_Galaxy));
		pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,pDB->GetLong(gc_cGearBox_Level));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ12,pDB->GetLong(gc_cGear_Num_12));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ21,pDB->GetLong(gc_cGear_Num_21));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ22,pDB->GetLong(gc_cGear_Num_22));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ31,pDB->GetLong(gc_cGear_Num_31));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ32,pDB->GetLong(gc_cGear_Num_32));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ41,pDB->GetLong(gc_cGear_Num_41));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ42,pDB->GetLong(gc_cGear_Num_42));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ51,pDB->GetLong(gc_cGear_Num_51));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ52,pDB->GetLong(gc_cGear_Num_52));
		pFixComm->SetItem(FIELD_SYSTEM_GEARNUMZ61,pDB->GetLong(gc_cGear_Num_61));
		pFixComm->SetItem(FIELD_SYSTEM_SENDC_IF,pDB->GetLong(gc_cIs_Sync));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的位置参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         位置索引ID             FIELD_SYSTEM_STATUS
*         位置名称               FIELD_SYSTEM_SEG_NAME
*         部位索引ID             FIELD_SYSTEM_CHANN_TYPE
*         轴承索引ID             FIELD_SYSTEM_TYPE
*         轴承表类型             FIELD_SYSTEM_CHAR_TYPE
*         轴号                   FIELD_SYSTEM_SEG_NUM
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         位置号                 FIELD_SERVICE_LOCATION_NO
#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125
*/
int CImplement::SysGetSetSvrLocationTbl(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cLocation_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cLocation_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cLocation_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,pDB->GetLong(gc_cSection_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cBearing_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_CHAR_TYPE,pDB->GetLong(gc_cBearingTbl_Type));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NUM,pDB->GetLong(gc_cShaft_Number));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO, pDB->GetLong(gc_cLocation_NO));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的部位参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         部位索引ID             FIELD_SYSTEM_STATUS
*         部位名称               FIELD_SYSTEM_SEG_NAME
*         齿轮箱索引ID           FIELD_SYSTEM_TYPE
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         功能类型               FIELD_SERVICE_FUNCTION_TYPE
*         部位类型               FIELD_SERVICE_SECTION_TYPE
*         当前位置               FIELD_SERVICE_LOC_IDX
*         前一部位               FIELD_SERVICE_PREV_SEC_IDX
*         前一位置               FIELD_SERVICE_PREV_LOC_IDX
*         部位图片               FIELD_SERVICE_PREV_SECTION_PIC
#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126
*/
int CImplement::SysGetSetSvrSectionTbl(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pDB->Cmd("select * from [%s] t",gc_cSection_Table);
	pDB->Cmd(" where exists (select * from machine m where t.COMPANY=m.COMPANY AND t.PLANTNO=m.PLANTNO)");
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY,pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantNo).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,pDB->GetLong(gc_cSection_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_SEG_NAME,pDB->Get(gc_cSection_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,pDB->GetLong(gc_cGearBox_IDX));
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,pDB->Get(gc_cComment).c_str());
		pFixComm->SetItem(FIELD_SERVICE_FUNCTION_TYPE, pDB->GetLong(gc_cFunction_Type));
		pFixComm->SetItem(FIELD_SERVICE_SECTION_TYPE, pDB->GetLong(gc_cSection_Type));
		pFixComm->SetItem(FIELD_SERVICE_LOC_IDX, pDB->GetLong(gc_cLocation_IDX));
		pFixComm->SetItem(FIELD_SERVICE_PREV_SEC_IDX, pDB->GetLong(gc_cPrev_Sec_IDX));
		pFixComm->SetItem(FIELD_SERVICE_PREV_LOC_IDX, pDB->GetLong(gc_cPrev_Loc_IDX));
		pFixComm->SetItem(FIELD_SERVICE_SECTION_PIC, pDB->GetLong(gc_cSection_Pic));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/** 获取各个程序的路径和版本号
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            程序名                 FIELD_SERVICE_MODULE_NAME
  *            文件名                 FIELD_SERVICE_FILE_NAME
  *            路径                   FIELD_SERVICE_FILE_PATH
  *            版本号                 FIELD_SERVICE_FILE_VERSION
  *\instance 例:
      [JSON]
      "MiddleWare" : {
         "POSITION" : "E:\\Project\\lib\\S_SystemSvr.dll",
         "POSITION1" : "E:\\Project\\lib\\S_OnlineSvr.dll",
         "POSITION2" : "E:\\Project\\lib\\S_OfflineSvr.dll",
         "S_OfflineSvr.dll" : "3.5.0",
         "S_OnlineSvr.dll" : "3.5.0",
         "S_SystemSvr.dll" : "3.5.0"
      },

      [中间件通信数据格式]
      "MiddleWare"
      "S_OfflineSvr.dll"
      "E:\\Project\\lib\\S_OfflineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_OnlineSvr.dll"
      "E:\\Project\\lib\\S_OnlineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_SystemSvr.dll"
      "E:\\Project\\lib\\S_SystemSvr.dll"
      "3.5.0"
 #define     SERVICE_SYSTEM_GET_MODULE_VERSION                 25029
  */
int CImplement::SysGetModuleVersion(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAEMON_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s", statusFile));
		return -1;
	}

	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_BASIC_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_BASIC_INFO));
		return -3;
	}

	Json::Value basicInfo = root[DAEMON_STATUS_BASIC_INFO];

	if(!basicInfo.isMember(DAEMON_STATUS_SOFTWARE_VERSION)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_SOFTWARE_VERSION));
		return -4;
	}

	Json::Value softwareVersion = basicInfo[DAEMON_STATUS_SOFTWARE_VERSION];

	Json::Value::Members moduleList = softwareVersion.getMemberNames();

	std::string moduleName;

	std::map<std::string, std::string> mapFileNameWithPosition;
	std::map<std::string, std::string> mapFileNameWithVersion;

	Json::Value versionAndPathInfo;
	Json::Value::Members infoList;

	string filePath;

	if(moduleList.size() < 1){
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s]没有模块",
			statusFile,
			DAEMON_STATUS_BASIC_INFO,
			DAEMON_STATUS_SOFTWARE_VERSION));

		return -5;
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	for(Json::Value::Members::iterator it = moduleList.begin();
		it != moduleList.end();
		++it)
	{
		moduleName = *it;

		versionAndPathInfo = softwareVersion[moduleName];

		mapFileNameWithPosition.clear();
		mapFileNameWithVersion.clear();

		infoList = versionAndPathInfo.getMemberNames();
		for(vector<string>::iterator it = infoList.begin();
			it != infoList.end();
			++it)
		{
			if(it->find(DAEMON_STATUS_FILE_PATH) != string::npos){

				filePath = versionAndPathInfo.get(*it, "").asString();
				if(filePath.empty()) {
					HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s].[%s].%s无对应的文件路径",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str()));

					/* 无文件路径, 忽略 */
					continue;
				}

				size_t sepratorLoction = filePath.rfind('\\');
				if(string::npos == sepratorLoction){
					HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s].[%s].%s对应的文件路径%s格式错误",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str(),
					filePath.c_str()));

					/* 格式错误, 忽略 */
					continue;
				}

				std::string fileName = filePath.substr(sepratorLoction + 1);
				if(fileName.empty()){
					HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s].[%s].%s对应的文件名空",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str()));

					/* 无法解析出文件名, 忽略 */
					continue;
				}

				mapFileNameWithPosition.insert(make_pair(fileName, filePath));
			}
			else{
				mapFileNameWithVersion.insert(
					make_pair(
						*it,
						versionAndPathInfo.get(*it, "").asString()
						)
					);
			}

		}



		for(map<string, string>::iterator it = mapFileNameWithVersion.begin();
			it != mapFileNameWithVersion.end();
			++it)
		{
			map<string, string>::iterator itFilePath = mapFileNameWithPosition.find(it->first);
			if(itFilePath != mapFileNameWithPosition.end()){
				filePath = itFilePath->second.c_str();
			}
			else{
				HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s].[%s]中没有文件%s的路径信息",
				statusFile,
				DAEMON_STATUS_BASIC_INFO,
				DAEMON_STATUS_SOFTWARE_VERSION,
				moduleName.c_str(),
				it->first.c_str()));

				filePath = "";
			}

			pFixComm->SetItem(FIELD_SERVICE_MODULE_NAME,  moduleName.c_str());
			pFixComm->SetItem(FIELD_SERVICE_FILE_NAME,    it->first.c_str());
			pFixComm->SetItem(FIELD_SERVICE_FILE_VERSION, it->second.c_str());
			pFixComm->SetItem(FIELD_SERVICE_FILE_PATH,    filePath.c_str());

			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return moduleList.size();
}

/**得到中间件的版本号
*输入参数值：
*返回参数值：
*         版本号                 FIELD_SERVICE_FILE_VERSION
#define    SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION      25127
*/


namespace Version
{
	#pragma comment(lib, "Version.lib")

	bool GetModuleProductVersion(CString &sVersion)
	{
		CString lsExecPath;
		if (!::GetModuleFileName(NULL, lsExecPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			return false;
		}

		lsExecPath.ReleaseBuffer();

		CString lsSystemDll;
		
		lsSystemDll.Format("%s\\%s",
			lsExecPath.Left(lsExecPath.ReverseFind('\\')),
			"S_SystemSvr.dll");

		DWORD  dwSize = ::GetFileVersionInfoSize(lsSystemDll, 0);
		if (dwSize < 1)
		{
			return false;
		}

		std::vector<char> loBuffer;
		loBuffer.resize(dwSize);

		if(!::GetFileVersionInfo(lsSystemDll, NULL, loBuffer.size(), &loBuffer.front()))
		{
			return false;
		}

		VS_FIXEDFILEINFO *lpFileInfo = NULL;
		if (!::VerQueryValue(&loBuffer.front(), _T("\\"), (LPVOID *)&lpFileInfo, (PUINT)&dwSize))
		{
			return false;
		}

		sVersion.Format("%d.%d.%d.%d", 
			HIWORD(lpFileInfo->dwProductVersionMS),
			LOWORD(lpFileInfo->dwProductVersionMS),
			HIWORD(lpFileInfo->dwProductVersionLS),
			LOWORD(lpFileInfo->dwProductVersionLS));

		return true;
	}
}

 int CImplement::SysGetMiddlewareVersion(ICommInterFace *pFixComm,IDBInterFace *pDB)
 {
	 if (pFixComm)
	 {
		 static CString g_sVersion;

		 if (g_sVersion.IsEmpty())
		 {
			 if (!Version::GetModuleProductVersion(g_sVersion))
			 {
				 ASSERT(FALSE);
				 g_sVersion = "4.5.1";
			 }
		 }


		 try
		 {
			 pFixComm->CreateAnswerMul(pFixComm->GetFunc());	
			 pFixComm->SetItem(FIELD_SERVICE_FILE_VERSION,g_sVersion);
			 pFixComm->SetItem(FIELD_SERVICE_TIME,CBHDateTime::GetCurrentTime());

			 SYSTEMTIME loTime={0};
			 ::GetLocalTime(&loTime);
			 pFixComm->SetItemBuf(FIELD_SERVICE_MICROSECOND,(char *)&loTime,sizeof(loTime));

			 pFixComm->SetItem(FIELD_SYSTEM_TEMPID,G_TEMP_ONLINE);
			 pFixComm->Write(pFixComm->GetAci());	
			 pFixComm->CreateEndMak();	
			 pFixComm->Write(pFixComm->GetAci());	
		     return 4;
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
	 return -1;
 }


/** 获取各个数据库状态
  *输入参数字段码：无
  *返回值的字段码：(n)
  *     报警数据预计保存时间,单位 天(AlarmDataSavingTime)     FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME
  *     趋势数据预计保存时间(DataSavingTime)                  FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME
  *     报警和趋势数据预计保存时间(TrendDataSavingTime)       FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME
  *     数据库数目(DBNum)                                     FIELD_SERVICE_DB_NUM
  *     数据库文件编号,即0, 主数据库文件， 1分数据文件1, ...  FIELD_SERVICE_DB_INDEX
  *     数据文件名(DBDataFileName)                            FIELD_SERVICE_DB_DATA_FILE_NAME
  *     数据文件大小，单位 GB(DBDataFileSize)                 FIELD_SERVICE_DB_DATA_FILE_SIZE
  *     日志文件名(DBLogFileName)                             FIELD_SERVICE_DB_LOG_FILE_NAME
  *     日志文件大小，单位 GB(DBLogFileSize)                  FIELD_SERVICE_DB_LOG_FILE_SIZE
  *     数据库名(DBName)                                      FIELD_SERVICE_DB_NAME
  *     数据库类型，0表示主库，1表示分库(DBType)              FIELD_SERVICE_DB_TYPE
  *     数据库所在磁盘剩余空间，单位为GB(LeftDiskSpace)       FIELD_SERVICE_FREE_DISK_SPACE
  *     数据库所在磁盘剩余空间比(LeftDiskSpaceRate)           FIELD_SERVICE_FREE_DISK_SPACE_RATIO
  *     前一天增长最快的表，CSV格式(TheFastIncTableName)      FIELD_SERVICE_FAST_INC_TABLE
  *     数据库所在磁盘总空间，单位为GB(TotalDiskSpace)        FIELD_SERVICE_TOTAL_DISK_SPACE
  #define     SERVICE_SYSTEM_GET_DB_STATUS                      25030
  */
int CImplement::SysGetDBStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAEMON_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s", statusFile));
		return -1;
	}

	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_DB_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_DB_INFO));
		return -3;
	}

	Json::Value DBInfo = root[DAEMON_STATUS_DB_INFO];

	double AlarmDataSavingTime         = -1;
	double AlarmTrendSavingTime        = -1;
	double AlarmTrendAndDataSavingTime = -1;

	if(!DBInfo.isMember(DAEMON_STATUS_DB_STORING_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_DB_STORING_INFO));
		nRet = -4;
	}
	else {
		Json::Value DBStoringInfo = DBInfo[DAEMON_STATUS_DB_STORING_INFO];

		AlarmDataSavingTime         = DBStoringInfo.get(
		           DAEMON_STATUS_ALARM_DATA_ESTIMATE_SVAE_TIME, -1).asDouble();
		AlarmTrendSavingTime        = DBStoringInfo.get(
		           DAEMON_STATUS_ALARM_TREND_ESTIMATE_SVAE_TIME, -1).asDouble();
		AlarmTrendAndDataSavingTime = DBStoringInfo.get(
		           DAEMON_STATUS_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME, -1).asDouble();
	}

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	pFixComm->SetItem(FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME,
	                  AlarmDataSavingTime);
	pFixComm->SetItem(FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME,
	                  AlarmTrendSavingTime);
	pFixComm->SetItem(FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME,
	                  AlarmTrendAndDataSavingTime);

	int DBNum   = 0;
	int DBIndex = -1;

	string DBDataFileName;
	double DBDataFileSize = 0;
	string DBLogFileName;
	double DBLogFileSize  = 0;

	string DBName;
	int DBType = -1;

	double freeDiskSpace      = 0;
	double freeDiskSpaceRatio = 0;

	string fastestIncTables;
	double totalDiskSpace = 0;

	if(!DBInfo.isMember(DAEMON_STATUS_DB_DISK_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_DB_DISK_INFO));

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -5;
	}

	Json::Value diskInfo = DBInfo[DAEMON_STATUS_DB_DISK_INFO];

	if(!diskInfo.isMember(DAEMON_STATUS_DB_NUM)){
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s]中没有%s",
			statusFile,
			DAEMON_STATUS_DB_INFO,
			DAEMON_STATUS_DB_DISK_INFO,
			DAEMON_STATUS_DB_NUM));

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -6;
	}

	DBNum = diskInfo.get(DAEMON_STATUS_DB_NUM, 0).asInt();

	if(DBNum < 1){
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s]中没有%s",
			statusFile,
			DAEMON_STATUS_DB_INFO,
			DAEMON_STATUS_DB_DISK_INFO,
			DAEMON_STATUS_DB_NUM));

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -7;
	}

	if(!diskInfo.isMember(DAEMON_STATUS_DB_MAIN)){
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s]中没有%s",
			statusFile,
			DAEMON_STATUS_DB_INFO,
			DAEMON_STATUS_DB_DISK_INFO,
			DAEMON_STATUS_DB_MAIN));

		return -8;
	}

	Json::Value mainDB = diskInfo[DAEMON_STATUS_DB_MAIN];

	DBDataFileName = mainDB.get(
		DAEMON_STATUS_DB_DATA_FILE_NAME, "").asString();
	DBDataFileSize = mainDB.get(
		DAEMON_STATUS_DB_DATA_FILE_SIZE, 0).asDouble();

	DBLogFileName = mainDB.get(
		DAEMON_STATUS_DB_LOG_FILE_NAME, "").asString();
	DBLogFileSize = mainDB.get(
		DAEMON_STATUS_DB_LOG_FILE_SIZE, 0).asDouble();

	DBName = mainDB.get(
		DAEMON_STATUS_DB_NAME, "").asString();
	DBType = mainDB.get(
		DAEMON_STATUS_DB_TYPE, -1).asInt();

	freeDiskSpace = mainDB.get(
		DAEMON_STATUS_FREE_DISK_SPACE, 0).asDouble();
	freeDiskSpaceRatio = mainDB.get(
		DAEMON_STATUS_FREE_DISK_SPACE_RATIO, 0).asDouble();

	fastestIncTables = mainDB.get(
		DAEMON_STATUS_FAST_INC_TABLE, 0).asString();
	totalDiskSpace = mainDB.get(
		DAEMON_STATUS_TOTAL_DISK_SPACE, 0).asDouble();

	pFixComm->SetItem(FIELD_SERVICE_DB_NUM, DBNum);
	pFixComm->SetItem(FIELD_SERVICE_DB_INDEX, 0);

	pFixComm->SetItem(FIELD_SERVICE_DB_DATA_FILE_NAME, DBDataFileName.c_str());
	pFixComm->SetItem(FIELD_SERVICE_DB_DATA_FILE_SIZE, DBDataFileSize);

	pFixComm->SetItem(FIELD_SERVICE_DB_LOG_FILE_NAME,  DBLogFileName.c_str());
	pFixComm->SetItem(FIELD_SERVICE_DB_LOG_FILE_SIZE,  DBLogFileSize);

	pFixComm->SetItem(FIELD_SERVICE_DB_NAME,  DBName.c_str());
	pFixComm->SetItem(FIELD_SERVICE_DB_TYPE,  DBType);

	pFixComm->SetItem(FIELD_SERVICE_FREE_DISK_SPACE,       freeDiskSpace);
	pFixComm->SetItem(FIELD_SERVICE_FREE_DISK_SPACE_RATIO, freeDiskSpaceRatio);

	pFixComm->SetItem(FIELD_SERVICE_FAST_INC_TABLE,   fastestIncTables.c_str());
	pFixComm->SetItem(FIELD_SERVICE_TOTAL_DISK_SPACE, totalDiskSpace);

	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();


	for(int i = 1; i < DBNum; i++){
		string subDBNode = getStringFormattedAs("%s%d", DAEMON_STATUS_DB_SUB, i);

		if(!diskInfo.isMember(subDBNode)){
			HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s].[%s]中没有%s",
				statusFile,
				DAEMON_STATUS_DB_INFO,
				DAEMON_STATUS_DB_DISK_INFO,
				subDBNode.c_str()));

			nRet = -8 - i;
		} else {
			Json::Value subDB = diskInfo[subDBNode];

			DBDataFileName = subDB.get(
				DAEMON_STATUS_DB_DATA_FILE_NAME, "").asString();
			DBDataFileSize = subDB.get(
				DAEMON_STATUS_DB_DATA_FILE_SIZE, 0).asDouble();

			DBLogFileName = subDB.get(
				DAEMON_STATUS_DB_LOG_FILE_NAME, "").asString();
			DBLogFileSize = subDB.get(
				DAEMON_STATUS_DB_LOG_FILE_SIZE, 0).asDouble();

			DBName = subDB.get(
				DAEMON_STATUS_DB_NAME, "").asString();
			DBType = subDB.get(
				DAEMON_STATUS_DB_TYPE, -1).asInt();

			freeDiskSpace = subDB.get(
				DAEMON_STATUS_FREE_DISK_SPACE, 0).asDouble();
			freeDiskSpaceRatio = subDB.get(
				DAEMON_STATUS_FREE_DISK_SPACE_RATIO, 0).asDouble();

			fastestIncTables = subDB.get(
				DAEMON_STATUS_FAST_INC_TABLE, 0).asString();
			totalDiskSpace = subDB.get(
				DAEMON_STATUS_TOTAL_DISK_SPACE, 0).asDouble();

			pFixComm->SetItem(FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME,
			                  AlarmDataSavingTime);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME,
			                  AlarmTrendSavingTime);
			pFixComm->SetItem(FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME,
			                  AlarmTrendAndDataSavingTime);

			pFixComm->SetItem(FIELD_SERVICE_DB_NUM, DBNum);
			pFixComm->SetItem(FIELD_SERVICE_DB_INDEX, i);

			pFixComm->SetItem(FIELD_SERVICE_DB_DATA_FILE_NAME, DBDataFileName.c_str());
			pFixComm->SetItem(FIELD_SERVICE_DB_DATA_FILE_SIZE, DBDataFileSize);

			pFixComm->SetItem(FIELD_SERVICE_DB_LOG_FILE_NAME,  DBLogFileName.c_str());
			pFixComm->SetItem(FIELD_SERVICE_DB_LOG_FILE_SIZE,  DBLogFileSize);

			pFixComm->SetItem(FIELD_SERVICE_DB_NAME,  DBName.c_str());
			pFixComm->SetItem(FIELD_SERVICE_DB_TYPE,  DBType);

			pFixComm->SetItem(FIELD_SERVICE_FREE_DISK_SPACE,       freeDiskSpace);
			pFixComm->SetItem(FIELD_SERVICE_FREE_DISK_SPACE_RATIO, freeDiskSpaceRatio);

			pFixComm->SetItem(FIELD_SERVICE_FAST_INC_TABLE,   fastestIncTables.c_str());
			pFixComm->SetItem(FIELD_SERVICE_TOTAL_DISK_SPACE, totalDiskSpace);

			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
		}
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return nRet;
}


/** 获取中间件运行状态
  *输入参数字段码：无
  *返回值的字段码：(1)
  *            数据库状态             FIELD_SERVICE_DB_STATUS
  *            接收数据的公司名       FIELD_SYSTEM_COMPANY
  *            接收数据的分厂名       FIELD_SYSTEM_FACTORY
  *            接收数据的设备数目     FIELD_SERVICE_DATA_RCV_PLANT_NUM
  *            接收数据的设备状态     FIELD_SERVICE_DATA_RCV_STATUS
  *            保存数据的设备数目     FIELD_SERVICE_DATA_SAVE_PLANT_NUM
  *            保存数据的设备状态     FIELD_SERVICE_DATA_SAVE_STATUS
  *            同步数据的设备数目     FIELD_SERVICE_DATA_SYNC_PLANT_NUM
  *            同步数据的设备状态     FIELD_SERVICE_DATA_SYNC_STATUS
  *
  *\description 类型说明：
  *            数据库状态,     类型 GE_DB_STATUS
  *            接收数据的设备状态，类型 BUFFER, BHMWDataRcvStatus[]
  *            保存数据的设备状态，类型 BUFFER, BHMWDataSaveStatus[]
  *            同步数据的设备状态，类型 BUFFER, BHMWDataSyncStatus[]
  #define     SERVICE_SYSTEM_GET_SERVER_STATUS                  25031
  */
namespace{
	/**
	 itemlist = company + factory + n * (plantName + vib + dyn + proc)
	 */
	inline int get_plant_num(int itemlist_size)
	{
		return (itemlist_size - 2) / 4;
	}
}
int CImplement::SysGetServerStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAEMON_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s", statusFile));
		return -1;
	}

	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_MW_STATUS)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_MW_STATUS));
		return -3;
	}

	string companyName;
	string factoryName;

	Json::Value MWStatus = root[DAEMON_STATUS_MW_STATUS];

	Json::Value status;
	Json::Value::Members itemList;

	BHKD::BHMWDataRcvStatus  rcvData;
	BHKD::BHMWDataSaveStatus saveData;
	BHKD::BHMWDataSyncStatus syncData;

	vector<BHKD::BHMWDataRcvStatus>  dataRcvStatus;
	vector<BHKD::BHMWDataSaveStatus> dataSaveStatus;
	vector<BHKD::BHMWDataSyncStatus> dataSyncStatus;

	int nRet = -3;
	/*
	  因为companyName 和 factoryName在itemList, 故计算数据数目时需减2
	*/
	if(MWStatus.isMember(DAEMON_STATUS_MW_DATA_RECEIVING_STATUS)){
		status = MWStatus[DAEMON_STATUS_MW_DATA_RECEIVING_STATUS];

		companyName = status.get(DAEMON_STATUS_MW_COMPANY, "").asString();
		factoryName = status.get(DAEMON_STATUS_MW_FACTORY, "").asString();

		itemList = status.getMemberNames();
		int nPlantNum = get_plant_num(itemList.size());
		for(int i = 0; i < nPlantNum; i++){
			if(0 == i){
				::strncpy(rcvData.plantName,
				        status.get(DAEMON_STATUS_MW_PLANT, "").asCString(),
				        BHKD::D_BHKD_PLANTID_LEN-1);
				rcvData.vibStatus = status.get(DAEMON_STATUS_MW_VIBDATA, 0).asInt();
				rcvData.dynStatus = status.get(DAEMON_STATUS_MW_DYNDATA, 0).asInt();
				rcvData.procStatus = status.get(DAEMON_STATUS_MW_PRCODATA, 0).asInt();
			}else{
				::strncpy(rcvData.plantName,
				        status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_PLANT, i), "").asCString(),
				        BHKD::D_BHKD_PLANTID_LEN-1);
				rcvData.vibStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_VIBDATA, i), 0).asInt();
				rcvData.dynStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_DYNDATA, i), 0).asInt();
				rcvData.procStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_PRCODATA, i), 0).asInt();
			}

			dataRcvStatus.push_back(rcvData);
		}
	}
	else {
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s]中没有%s",
			statusFile,
			DAEMON_STATUS_MW_STATUS,
			DAEMON_STATUS_MW_DATA_RECEIVING_STATUS));

			nRet--;
	}



	if(MWStatus.isMember(DAEMON_STATUS_MW_DATA_SAVING_STATUS)){
		status = MWStatus[DAEMON_STATUS_MW_DATA_SAVING_STATUS];

		companyName = status.get(DAEMON_STATUS_MW_COMPANY, "").asString();
		factoryName = status.get(DAEMON_STATUS_MW_FACTORY, "").asString();

		itemList = status.getMemberNames();
		int nPlantNum = get_plant_num(itemList.size());
		for(int i = 0; i < nPlantNum; i++){
			if(0 == i){
				::strncpy(saveData.plantName,
				          status.get(DAEMON_STATUS_MW_PLANT, "").asCString(),
				          BHKD::D_BHKD_PLANTID_LEN-1);
				saveData.alarmStatus = status.get(DAEMON_STATUS_MW_ALARAMDATA, 0).asInt();
				saveData.trendStatus = status.get(DAEMON_STATUS_MW_TRENDDATA, 0).asInt();
				saveData.sudStatus   = status.get(DAEMON_STATUS_MW_SUDDATA, 0).asInt();
			}else{
				::strncpy(saveData.plantName,
				          status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_PLANT, i), "").asCString(),
				          BHKD::D_BHKD_PLANTID_LEN-1);
				saveData.alarmStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_ALARAMDATA, i), 0).asInt();
				saveData.trendStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_TRENDDATA, i), 0).asInt();
				saveData.sudStatus   = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_SUDDATA, i), 0).asInt();
			}

			dataSaveStatus.push_back(saveData);
		}
	}
	else {
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s]中没有%s",
			statusFile,
			DAEMON_STATUS_MW_STATUS,
			DAEMON_STATUS_MW_DATA_SAVING_STATUS));

			nRet--;
	}


	if(MWStatus.isMember(DAEMON_STATUS_MW_DATA_SENDING_STATUS)){
		status = MWStatus[DAEMON_STATUS_MW_DATA_SENDING_STATUS];

		companyName = status.get(DAEMON_STATUS_MW_COMPANY, "").asString();
		factoryName = status.get(DAEMON_STATUS_MW_FACTORY, "").asString();

		itemList = status.getMemberNames();

		int nPlantNum = get_plant_num(itemList.size());
		for(int i = 0; i < nPlantNum; i++){
			if(0 == i){
				::strncpy(syncData.plantName,
				        status.get(DAEMON_STATUS_MW_PLANT, "").asCString(),
				        BHKD::D_BHKD_PLANTID_LEN-1);
				::strncpy(syncData.alarmStatus,
				          status.get(DAEMON_STATUS_MW_ALARAMDATA, "").asCString(),
				          BHKD::D_BHKD_STATUS_LEN-1);
				syncData.trendStatus = status.get(DAEMON_STATUS_MW_TRENDDATA, 0).asInt();
				::strncpy(syncData.sudStatus,
				          status.get(DAEMON_STATUS_MW_SUDDATA, "").asCString(),
				          BHKD::D_BHKD_STATUS_LEN-1);
			}else{
				::strncpy(syncData.plantName,
				          status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_PLANT, i), "").asCString(),
				          BHKD::D_BHKD_PLANTID_LEN-1);
				::strncpy(syncData.alarmStatus,
				          status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_ALARAMDATA, i), "").asCString(),
				          BHKD::D_BHKD_STATUS_LEN-1);
				syncData.trendStatus = status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_TRENDDATA, i), 0).asInt();
				::strncpy(syncData.sudStatus,
				          status.get(getStringFormattedAs("%s%d",
				                DAEMON_STATUS_MW_SUDDATA, i), "").asCString(),
				          BHKD::D_BHKD_STATUS_LEN-1);
			}

			dataSyncStatus.push_back(syncData);
		}
	}
	else {
		HZLOG_WARN(STR("守护系统日志文件%s中记录错误, [%s]中没有%s",
			statusFile,
			DAEMON_STATUS_MW_STATUS,
			DAEMON_STATUS_MW_DATA_SENDING_STATUS));

		nRet--;
	}

	GE_DB_STATUS dbStatus = GE_DB_FUNC_ERROR;

	pDB->ClearSql();
	pDB->Cmd("SELECT 1 as status");
	if(pDB->More() && pDB->GetLong("status") == 1){
		dbStatus = GE_DB_FUNC_OK;
	}

	if(companyName.empty() || factoryName.empty()){
		HZLOG_ERROR(STR("守护系统日志文件%s中记录错误, 公司，分厂名异常 (%s, %s)",
			statusFile,
			companyName.c_str(),
			factoryName.c_str()));

		nRet--;
		return nRet;
	}


	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pFixComm->SetItem(FIELD_SERVICE_DB_STATUS, dbStatus);

	pFixComm->SetItem(FIELD_SYSTEM_COMPANY, companyName.c_str());
	pFixComm->SetItem(FIELD_SYSTEM_FACTORY, factoryName.c_str());

	pFixComm->SetItem(FIELD_SERVICE_DATA_RCV_PLANT_NUM, (int)dataRcvStatus.size());
	if(dataRcvStatus.size() > 0){
		HZLOG_WARN(STR("守护系统日志文件%s中记录缺失, [%s]中没有记录",
			statusFile,
			DAEMON_STATUS_MW_DATA_RECEIVING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_RCV_STATUS,
		          (char *)&dataRcvStatus[0],
		          dataRcvStatus.size() * sizeof(BHKD::BHMWDataRcvStatus));
		}

	pFixComm->SetItem(FIELD_SERVICE_DATA_SAVE_PLANT_NUM, (int)dataSaveStatus.size());
	if(dataSaveStatus.size() > 0){
		HZLOG_WARN(STR("守护系统日志文件%s中记录缺失, [%s]中没有记录",
			statusFile,
			DAEMON_STATUS_MW_DATA_SAVING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_SAVE_STATUS,
		          (char *)&dataSaveStatus[0],
		          dataSaveStatus.size() * sizeof(BHKD::BHMWDataSaveStatus));
	}

	pFixComm->SetItem(FIELD_SERVICE_DATA_SYNC_PLANT_NUM, (int)dataSyncStatus.size());
	if(dataSyncStatus.size() > 0){
		HZLOG_WARN(STR("守护系统日志文件%s中记录缺失, [%s]中没有记录",
			statusFile,
			DAEMON_STATUS_MW_DATA_SENDING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_SYNC_STATUS,
		          (char *)&dataSyncStatus[0],
		          dataSyncStatus.size() * sizeof(BHKD::BHMWDataSyncStatus));
	}


	pFixComm->Write(pFixComm->GetAci());

	return dataRcvStatus.size() + dataSaveStatus.size() + dataSyncStatus.size();
}

/** 获取数采硬件状态
  *输入参数值：无
  *返回值的字段码：(n)
  *            数采卡索引             FIELD_SERVICE_DAQ_CARD_INDEX
  *            数采卡数量             FIELD_SERVICE_DAQ_CARD_NUM
  *            数采卡逻辑名称         FIELD_SERVICE_DAQ_CARD_NAME
  *            使用AI通道(CSV)        FIELD_SERVICE_DAQ_USED_AI_CHANNEL
  *            使用CI通道(CSV)        FIELD_SERVICE_DAQ_USED_CI_CHANNEL
  *            工作状况(字符串描述)   FIELD_SERVICE_DAQ_CARD_STATUS
  *            获取数采硬件状态时间   FIELD_SERVICE_OFF_TIME
  #define     SERVICE_SYSTEM_GET_DAQ_HW_STATUS                  25032
  */
int CImplement::SysGetHWDAQStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_HW_STATUS_FILE;

	_HZLOG_DEBUG(STR("Reading +r %s...", statusFile));

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s, 尝试重建%s\n", statusFile));
		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -1;
	}

	_HZLOG_DEBUG(STR("Parsing %s...", statusFile));


	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	_HZLOG_DEBUG(STR("Closed %s", statusFile));

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s, 尝试重建\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -2;
	}

	std::string sFileModifiedTime =
		root.get(FILE_MODIFIED_TIME, "").asString();
	CBHDateTime tFileModifiedTime;
	if(!tFileModifiedTime.ParseDateTime(sFileModifiedTime.c_str())){
		HZLOG_WARN(STR("无法读取文件%s上次更新时间, 尝试重建",
			statusFile));

		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -3;
	}

	std::string sLastUpdateTime =
		root.get(DAQ_LAST_UPDATE_TIME, "").asString();
	CBHDateTime tLastUpdateTime;
	if(!tLastUpdateTime.ParseDateTime(sLastUpdateTime.c_str())){
		HZLOG_WARN(STR("无法读取DAQ状态更新时间, 数采状态未知",
			statusFile));

		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -4;
	}


	int cardCount = root.get(DAQ_HW_STATUS_CARD_COUNT, 0).asInt();

	if(cardCount < 0){
		HZLOG_ERROR(STR("数采卡数目(%d)无效\n", cardCount));
		::SetNullReturnFixcommEx(pFixComm);
		return -5;
	}

	if(!root.isMember(DAQ_HW_STATUS_CARD_INFO)){
		HZLOG_ERROR(STR("没有数采卡状态记录"));
		::SetNullReturnFixcommEx(pFixComm);
		return -6;
	}

	Json::Value cardInfo = root[DAQ_HW_STATUS_CARD_INFO];

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	std::string  cardId;
	Json::Value  card;
	for(int i = 0; i < cardCount; i++){
		cardId  = getStringFormattedAs("card%d", i);
		if(!cardInfo.isMember(cardId)){
			HZLOG_ERROR(STR("没有数采卡%s的状态记录\n", cardId.c_str()));

			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_INDEX, i);
			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_NUM, cardCount);

			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_STATUS,
			          STR("没有数采卡%s的状态记录\n",
			                  cardId.c_str()));
		}
		else {

			card = cardInfo[cardId];

			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_INDEX,
			                  card.get(DAQ_HW_STATUS_CARD_INDEX, i).asInt());
			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_NUM, cardCount);
			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_NAME,
			                  card.get(DAQ_HW_STATUS_CARD_NAME, cardId).asCString());
			pFixComm->SetItem(FIELD_SERVICE_DAQ_USED_AI_CHANNEL,
			                  card.get(DAQ_HW_STATUS_USED_AI_CHANNELS, "").asCString());
			pFixComm->SetItem(FIELD_SERVICE_DAQ_USED_CI_CHANNEL,
			                  card.get(DAQ_HW_STATUS_USED_CI_CHANNELS, "").asCString());
			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_STATUS,
			                  card.get(DAQ_HW_STATUS_STATUS, "").asCString());
		}

		pFixComm->SetItem(FIELD_SERVICE_OFF_TIME, tLastUpdateTime);

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	_HZLOG_DEBUG(STR("Return %d", cardCount));

	return cardCount;
}

/** 获取数采软件状态
  *输入参数值：无
  *返回值的字段码：(1)
  *            版本号                                   FIELD_SERVICE_DAQ_SW_VERSION
  *            采集模式                                 FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            启动时间                                 FIELD_SERVICE_TIME
  *            最近一次振动数据发送时间                 FIELD_SERVICE_DAQ_VIB_TIME
  *            最近一次动态数据发送时间                 FIELD_SERVICE_DAQ_DYN_TIME
  *            最近一次过程量数据发送时间               FIELD_SERVICE_DAQ_PROC_TIME
  *            接收间隙电压状态                         FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            最近一次断网数据发送开始时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            最近一次断网数据发送结束时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  *            获取数采软件状态时间                     FIELD_SERVICE_OFF_TIME
  #define     SERVICE_SYSTEM_GET_DAQ_SW_STATUS                  25033
  */
int CImplement::SysGetDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_SW_STATUS_FILE;

	_HZLOG_DEBUG(STR("Reading +r %s...", statusFile));

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s, 尝试重建%s\n", statusFile));
		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -1;
	}

	_HZLOG_DEBUG(STR("Parsing %s...", statusFile));

	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	_HZLOG_DEBUG(STR("Closed %s", statusFile));

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s, 尝试重建\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -2;
	}

	std::string sFileModifiedTime =
		root.get(FILE_MODIFIED_TIME, "").asString();
	CBHDateTime tFileModifiedTime;
	if(!tFileModifiedTime.ParseDateTime(sFileModifiedTime.c_str())){
		HZLOG_WARN(STR("无法读取文件%s上次更新时间, 尝试重建",
			statusFile));

		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -3;
	}

	std::string sLastUpdateTime =
		root.get(DAQ_LAST_UPDATE_TIME, "").asString();
	CBHDateTime tLastUpdateTime;
	if(!tLastUpdateTime.ParseDateTime(sLastUpdateTime.c_str())){
		HZLOG_WARN(STR("无法读取DAQ状态更新时间, 数采状态未知",
			statusFile));

		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -4;
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pFixComm->SetItem(FIELD_SERVICE_DAQ_SW_VERSION,
	                  root.get(DAQ_SW_STATUS_VERSION, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_ACQUIRE_TYPE,
	                  root.get(DAQ_SW_STATUS_ACQUIRE_TYPE, 0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_TIME,
	                  root.get(DAQ_SW_STATUS_APP_START_TIME, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_VIB_TIME,
	                  root.get(DAQ_SW_STATUS_RCV_VIB_TIME, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_DYN_TIME,
	                  root.get(DAQ_SW_STATUS_RCV_DYN_TIME, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_PROC_TIME,
	                  root.get(DAQ_SW_STATUS_RCV_PROC_TIME, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_GAP_RCV_STATUS,
	                  root.get(DAQ_SW_STATUS_RCV_GAP_STATUS, 0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME,
	                  root.get(DAQ_SW_STATUS_LATEST_OFFLINE_SEND_BEGIN, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME,
	                  root.get(DAQ_SW_STATUS_LATEST_OFFLINE_SEND_END, "").asCString());
	pFixComm->SetItem(FIELD_SERVICE_OFF_TIME, tLastUpdateTime);

	pFixComm->Write(pFixComm->GetAci());

	_HZLOG_DEBUG(STR("Return"));

	return 0;
}


/** 获取基本的组态息
  * 输入参数值：无
  * 返回值的字段码：(1)
  *          报警设置(秒数，0表示未设置)                FIELD_SERVICE_CONFIG_ALARM
  *          连接的数据库                               FIELD_SERVICE_CONFIG_CONNECTED_DB
  *          保存数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SAVING
  *          同步数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SYNC
  *          中间件服务器型(0表示装置级，1表示中心级)   FIELD_SERVICE_CONFIG_SERVER_TYPE
  *          统计信息公司数目                           FIELD_SERVICE_COMPANY_NUM
  *          统计信息设备数目                           FIELD_SERVICE_PLANT_NUM
  *          统计信息测点数目                           FIELD_SERVICE_CHANN_NUM
  #define     SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO              25034
*/
int CImplement::SysGetBasicConfigInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAEMON_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件%s", statusFile));
		return -1;
	}

	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("无法解析文件%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_BASIC_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_BASIC_INFO));
		return -3;
	}

	Json::Value basicInfo = root[DAEMON_STATUS_BASIC_INFO];

	if(!basicInfo.isMember(DAEMON_STATUS_CONFIG_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_CONFIG_INFO));
		return -4;
	}

	if(!basicInfo.isMember(DAEMON_STATUS_STATISTIC_INFO)){
		HZLOG_ERROR(STR("守护系统日志%s中没有%s",
		                statusFile,
		                DAEMON_STATUS_STATISTIC_INFO));
		return -5;
	}

	Json::Value configInfo    = basicInfo[DAEMON_STATUS_CONFIG_INFO];
	Json::Value statisticInfo = basicInfo[DAEMON_STATUS_STATISTIC_INFO];

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pFixComm->SetItem(FIELD_SERVICE_CONFIG_ALARM,
	          configInfo.get(DAEMON_STATUS_CONFIG_ALARM,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_CONFIG_CONNECTED_DB,
	          configInfo.get(DAEMON_STATUS_CONFIG_CONNECTED_DB,
	                         0).asCString());
	pFixComm->SetItem(FIELD_SERVICE_CONFIG_DATA_SAVING,
	          configInfo.get(DAEMON_STATUS_CONFIG_DATA_SAVE,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_CONFIG_DATA_SYNC,
	          configInfo.get(DAEMON_STATUS_CONFIG_DATA_SYNC,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_CONFIG_SERVER_TYPE,
	          configInfo.get(DAEMON_STATUS_CONFIG_SERVER_TYPE,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_COMPANY_NUM,
	          statisticInfo.get(DAEMON_STATUS_STATISTIC_COMPANY_NUM,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_PLANT_NUM,
	          statisticInfo.get(DAEMON_STATUS_STATISTIC_PLANT_NUM,
	                         0).asInt());
	pFixComm->SetItem(FIELD_SERVICE_CHANN_NUM,
	          statisticInfo.get(DAEMON_STATUS_STATISTIC_CHANN_NUM,
	                         0).asInt());

	pFixComm->Write(pFixComm->GetAci());

	return nRet;
}



int CImplement::GetCompanyDBID(IDBInterFace *pDB_,string sCompany_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	int iRtn=0;
	pDB_->Cmd("select [%s] from [%s] ",gc_cDB_ID,gc_cCompanyTable);
	pDB_->Cmd(" where [%s]='%s'",gc_cCompany,sCompany_.c_str());
	if (pDB_->More())
		iRtn=pDB_->GetLong(gc_cDB_ID);
   return iRtn;
}

int CImplement::GetCharTypeDescKey(const int &iMachineType_, const int &iChannType_, const int &iSensorType_)
{
   if ( (iChannType_ == GE_VIBCHANN) && (iSensorType_ == GE_SENSORTYPE_ACCELERATE) )
     return GE_VIBCHARTYPE_A;
   if ( (iChannType_ == GE_DYNPRESSCHANN ))
     return GE_DYNCHARTYPE;
   if ( iChannType_ == GE_RODSINKCHANN )
	 return GE_RODSINKCHARTYPE;
   return GE_VIBCHARTYPE_A;
}

string CImplement::GetPlantNo(const string &sCompany_, const string &sFactory_, const string &sPlantID_, IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	string sPlantNo= "";
    pDB_->Cmd(" select [%s] from [%s] ",gc_cPlantNo, gc_cMachineTable);
	pDB_->Cmd(" where [%s] = '%s' and [%s] = '%s' and [%s]= '%s'",gc_cCompany, sCompany_.c_str(), gc_cFactory_Name, sFactory_.c_str(), gc_cPlantID, sPlantID_.c_str());
	if (pDB_->More())
	   sPlantNo = pDB_->Get(gc_cPlantNo);
	else
	{
		char pStrInfo[1024];
		_snprintf( pStrInfo,sizeof(pStrInfo), "没有指定 %s 公司 %s 设备 ",sCompany_.c_str(), sPlantID_.c_str());
		CHZLogManage::Error( pStrInfo, "CImplement", "GetPlantNo");
	}
	return sPlantNo;
}

CString CImplement::GetSetID2( string &sCompany_,  string &sFactory_,  string &sPlantID_, IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CString sPlantNo= "";
	pDB_->Cmd(" select [%s] from [%s] ",gc_cSetID, gc_cMachineTable);
	pDB_->Cmd(" where [%s] = '%s' and [%s] = '%s' and [%s]= '%s'",gc_cCompany, sCompany_.c_str(), gc_cFactory_Name, sFactory_.c_str(), gc_cPlantID, sPlantID_.c_str());
	if (pDB_->More())
		sPlantNo = pDB_->Get(gc_cSetID).c_str();
	else
	{
		char pStrInfo[1024];
		_snprintf( pStrInfo,sizeof(pStrInfo), "没有指定 %s 公司 %s 设备 ",sCompany_.c_str(), sPlantID_.c_str());
		CHZLogManage::Error( pStrInfo, "CImplement", "GetPlantNo");
	}
	return sPlantNo;
}

CString CImplement::GetValueByKey( map<CString,CString> &aoMapData,CString astrCompany,CString astrFactory,CString astrPlant )
{
	CString lstrKey;

	CString lstrValue = "";

	lstrKey.Format("%s|%s|%s",astrCompany,astrFactory,astrPlant);

	map<CString,CString>::iterator lpIter = aoMapData.find(lstrKey);
	if(lpIter != aoMapData.end())
	{
		lstrValue = lpIter->second;
	}

	return lstrValue;
}
