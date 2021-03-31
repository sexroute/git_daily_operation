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

/**ͨ��������ͺʹ��������͵õ�����ֵ����
*���������
*             �豸����    FIELD_SYSTEM_TYPE
*             �������    FIELD_SYSTEM_CHANN_TYPE
*             ����������  FIELD_SYSTEM_SENSOR_TYPE
*����ֵ���ֶ��룺
*             ����         FIELD_SYSTEM_STATUS
*             ��Ӧ����     FIELD_SYSTEM_STATUS_DISCRIPTION
*             ����ֵ��λ   FIELD_SERVICE_CHANN1_UNIT
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

/**�޸ļ�������м��IP��Port
*���������
 *          ��˾ID      FIELD_SYSTEM_COMPANY
*           ���ڷֳ�    FIELD_SYSTEM_FACTORY
*           װ����ID    FIELD_SYSTEM_SET
*           �м��IP    FIELD_SYSTEM_MW_IP
*           �м��PORT  FIELD_SYSTEM_MW_PORT
*����ֵ���ֶ��룺
*           ״̬     FIELD_SYSTEM_STATUS
*           ״̬���� FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**�õ���������м��IP��Port
*           ��˾ID        FIELD_SYSTEM_COMPANY
*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
*           װ����ID        FIELD_SYSTEM_SET
*����ֵ���ֶ��룺
*          �м��IP    FIELD_SYSTEM_MW_IP
*          �м��PORT  FIELD_SYSTEM_MW_PORT
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

/**�޸ķ��͵�������ĵ�ʱ�������Ƿ���״̬
 *           ��˾ID        FIELD_SYSTEM_COMPANY
*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
*           װ����ID        FIELD_SYSTEM_SET
*          ����ʱ����    FIELD_SYSTEM_SENDC_INTERVAL
*          �Ƿ���״̬    FIELD_SYSTEM_SENDC_IF
*          ������ͣ�����ݼ��  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          ���ͱ������ݼ��    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
*����ֵ���ֶ��룺
*          ״̬     FIELD_SYSTEM_STATUS
*          ״̬���� FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**�õ����͵�������ĵ�ʱ�������Ƿ���
*         ��˾ID        FIELD_SYSTEM_COMPANY
*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
*           װ����ID        FIELD_SYSTEM_SET
*����ֵ���ֶ��룺
*          ����ʱ����    FIELD_SYSTEM_SENDC_INTERVAL
*          �Ƿ���״̬    FIELD_SYSTEM_SENDC_IF
*          ������ͣ�����ݼ��  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          ���ͱ������ݼ��    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
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

/**�õ�װ�ü��м����Ϣ
*���ص��ֶ��룺
*         �ֳ�����      FIELD_SYSTEM_FACTORY
*         ��˾ID        FIELD_SYSTEM_COMPANY
*         װ����ID      FIELD_SYSTEM_SET
*         ��Ϣ�ṹ��    FIELD_SYSTEM_SETSVR_SETINFO
*         �ṹ�峤��    FIELD_SERVICE_LEN
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

/**�õ�װ�ü��м�����������豸��ϸ��Ϣ
  *        �豸��ʼ����  FIELD_SERVICE_STARTINDEX
  *���ص��ֶ��룺
  *        �豸��Ϣ      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        �汾          FIELD_SYSTEM_VERSION
  *        �豸��Ϣ      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        ��ʾ����      FIELD_SERVICE_GROUP_VISIBLE
  *        ��ʾ��˾      FIELD_SERVICE_COMPANY_VISIBLE
  *        ��ʾ�ֳ�      FIELD_SERVICE_FACTORY_VISIBLE
  *        ��ʾװ��      FIELD_SERVICE_SET_VISIBLE
  *        ��ʾ�豸      FIELD_SERVICE_PLANT_VISIBLE
  *        ��������      FIELD_SYSTEM_GROUP
  *        ��������      FIELD_SYSTEM_COUNTRY
  *        �豸����      FIELD_SERVICE_ENDINDEX
  *        �豸��Ŀ      FIELD_SERVICE_PLANT_NUM
  *        �豸��ʾ����  FIELD_SYSTEM_PLANT_DISPLAY
  *        ERP����       FIELD_SYSTEM_PLANT_ERP_CODE
  *        �豸��ʶ      FIELD_SERVICE_DEVICE_ID
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

	//�õ��м����ʾ����װ������

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

/**�õ�װ�ü��м��������ָ���豸�Ĳ����ϸ��Ϣ
* �����ֶ��룺
*        ��˾ID        FIELD_SYSTEM_COMPANY
*		 �ֳ���        FIELD_SYSTEM_FACTORY
*        �豸�ţ�      FIELD_SYSTEM_PLANT
*���ص��ֶ��룺
*        �����Ϣ  FIELD_SYSTEM_SETSVR_CHANNINFO
*        ����      FIELD_SYSTEM_SETSVR_INFONUM
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
/**�õ������û���Ϣ
  *�����ֶ��룺
  *�����ֶ��룺
  *            �û���         FIELD_SYSTEM_USEID
  *            ����           FIELD_SYSTEM_PASSWORD
  *            �û�����       FIELD_SYSTEM_USER_LEVEL
  *            ���ڷֳ�       FIELD_SYSTEM_FACTORY
  *            װ����ID       FIELD_SYSTEM_SET
  *            �Ƿ����װ��   FIELD_SYSTEM_BROWSE_SET
  *            ˢ��ʱ�� FIELD_SYSTEM_REFRESHTIME
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
	//�������Ƿ��и��������Ƿ����Ӽ���IDֵ�ж�
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

	//�����û���ȡ�豸
	UserInfo_t loUserInfo;

	UserManager::GetUserInfo(lstrUserName,loUserInfo);

	IdSet_t loCacheMachineIDList;

	if (loUserInfo.UserId == -1)//����
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

				//���˳��
				CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

				//��ֹ����λ
				lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

				//�����ǰ��˳��
				lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

				//����������λ
				lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

				//�����ر���λ
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

				//���˳��
				CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

				//��ֹ����λ
				lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

				//�����ǰ��˳��
				lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

				//����������λ
				lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
				pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

				//�����ر���λ
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
					//װ�ü�ID��Ϣ
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


					//���˳��
					CString lstrValue = GetValueByKey(goMapFireSequece,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_FIRE_SEQUECE,lstrValue);

					//��ֹ����λ
					lstrValue = GetValueByKey(goMapTDCPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_TDC_PHASE,lstrValue);

					//�����ǰ��˳��
					lstrValue = GetValueByKey(goMapFireAdvancedAnglePhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE,lstrValue);

					//����������λ
					lstrValue = GetValueByKey(goMapValveOpenPhase,loPlantInfo.CompanyName,loPlantInfo.FactoryName,loPlantInfo.PlantID);
					pFixComm->SetItem(FIELD_SYSTEM_VALVE_OPEN_PHASE,lstrValue);

					//�����ر���λ
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
	//					//װ�ü�ID��Ϣ
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

/**�õ�ָ���ֳ������л���λ�ź����IP��Ϣ��ͨ��ѭ�����εõ����л���λ�š����ֱ�����ӷ��ʵĹ�˾������������˾��IP��PortΪ�գ����ֱ�����ӷ��ʵ���װ�ü�����˾��װ�ü�IP��Port��Ϊ��
*�����������˾��     FIELD_SYSTEM_COMPANY
*          �ֳ���     FIELD_SYSTEM_FACTORY
*����ֵ���ֶ��룺
*          ��˾��     FIELD_SYSTEM_COMPANY
*          �ֳ���     FIELD_SYSTEM_FACTORY
*          �����     FIELD_SYSTEM_PLANT
*          ��òͼ���� FIELD_SYSTEM_PLANT_GRAPHTYPE
*          ���ż�IP   FIELD_SYSTEM_GROUP_MW_IP
*          ���ż�Port FIELD_SYSTEM_GROUP_MW_PORT
*          ��˾��IP   FIELD_SYSTEM_COMPANY_MW_IP
*          ��˾��Port FIELD_SYSTEM_COMPANY_MW_PORT
*          װ�ü�IP   FIELD_SYSTEM_MW_IP
*          װ�ü�Port FIELD_SYSTEM_MW_PORT
*          װ�ü��м��ID   FIELD_SYSTEM_SET
*          �豸����   FIELD_SYSTEM_TYPE
*          ����Դ     FIELD_SYSTEM_DATASOURCE
*          ��ʾ����   FIELD_SYSTEM_PLANT_DISPLAY
*          װ����ʾ����FIELD_SYSTEM_SET_DISPLAY
*          ERP����    FIELD_SYSTEM_PLANT_ERP_CODE
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
*/
int CImplement::SysGetPlantIPofSubCorp(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
/*
	if(!SwitchDbNameEx(pDB,g_strMainDbName)){
		HZLOG_DEBUG(STR("�л����ݿ�%sʧ��", g_strMainDbName));

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
				//װ�ü�ID��Ϣ
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

/**�õ�������ĵ��豸��Ϣ��
*���������   ��˾��    FIELD_SYSTEM_COMPANY
*             �ֳ���     FIELD_SYSTEM_FACTORY
*             �����     FIELD_SYSTEM_PLANT
*���ص��ֶ��룺
*             ����     FIELD_SYSTEM_CHANN
*             ����       FIELD_SYSTEM_STATUS
*             ��Ӧ����   FIELD_SYSTEM_STATUS_DISCRIPTION
*             ��λ       FIELD_SERVICE_CHANN1_UNIT
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

/**�õ�������ĵ���Ϣ������
* �����ֶ��룺
*���ص��ֶ��룺
*        ��Ϣ����      FIELD_SYSTEM_TYPE
*        ��Ӧ����      FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**���ݲ����Ϣ����,�õ�������ĵ���Ϣ������
* �����ֶ��룺
*        ��Ϣ����   FIELD_SYSTEM_TYPE
*���ص��ֶ��룺
*        ����       FIELD_SYSTEM_STATUS
*        ��Ӧ����   FIELD_SYSTEM_STATUS_DISCRIPTION
*        ��λ       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
*/
int CImplement::SysGetInfoTypeDetailBaseType(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0,iInfoType=0;
   	iInfoType = pFixComm->GetLong(FIELD_SYSTEM_TYPE);
	//��ѯ�豸��Ϣ��������ֵ��������Ӧ������ֵ�����������͵�λ
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

/**�޸�ָ���豸ָ�����ı�����Ϣ�����Ϊ-1����ʾû���޸ĳɹ���Ϊ1��ʾ�޸ĳɹ�
*���������   ��˾��         FIELD_SYSTEM_COMPANY
*             �ֳ���         FIELD_SYSTEM_FACTORY
*             ������         FIELD_SYSTEM_PLANT
*             �����         FIELD_SYSTEM_CHANN
*             �Ƿ񳣹汨��   FIELD_SYSTEM_STATUS //1Ϊ�жϱ�����0Ϊ���жϱ���
*             ����������     FIELD_SYSTEM_EVENT_TYPE
*             ��������       FIELD_SYSTEM_ALARM_LOW
*             ����������     FIELD_SYSTEM_ALARM_HIGH
*             ��������       FIELD_SYSTEM_ALARM_LOW2
*             ����������     FIELD_SYSTEM_ALARM_HIGH2
*����ֵ���ֶ��룺
*             ����״̬       FIELD_SYSTEM_STATUS
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

/**�õ�������ĵ��Զ������ģ���
* �����ֶ��룺
*���ص��ֶ��룺
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         �Զ������ģ��ID       FIELD_SYSTEM_TEMPID
*         �Զ�����Ϣ����         FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**�õ�������ĵ��Զ��������Ϣ��
*�������ֵ��
*���ز���ֵ��
*         ����ID                 FIELD_SYSTEM_STATUS
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         �߼�����ģ��ID         FIELD_SYSTEM_TEMPID
*         ��������               FIELD_SYSTEM_DATA
*         ��λ����               FIELD_SYSTEM_UNIT
*         �źŷ�����ʽ           FIELD_SYSTEM_SIGNAL_TYPE
*         ����ʼ��               FIELD_SYSTEM_SEG_STARTNO
*         ����                   FIELD_SYSTEM_SEG_NUM
*         ����                   FIELD_SYSTEM_SEG_NAME
*         ������ʼֵ             FIELD_SYSTEM_START_VALUE
*         ���ݽ���ֵ             FIELD_SYSTEM_END_VALUE
*         ����ֵ����             FIELD_SYSTEM_CHAR_TYPE
*         Ԥ��������             FIELD_SYSTEM_TYPE
*         ����Ƶ����ʼֵ         FIELD_SYSTEM_BAND_STARTVALUE
*         ����Ƶ������ֵ         FIELD_SYSTEM_BAND_ENDVALUE
*         Ԥ�������K             FIELD_SERVICE_PREPROC_K
*         Ԥ�������B             FIELD_SERVICE_PREPROC_B
*         ��λ                   FIELD_SERVICE_UNIT
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

/**�õ�������ĵ��Զ��������������ģ���
*�������ֵ��
*���ز���ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         �Զ������ģ��ID         FIELD_SYSTEM_TEMPID
*         �Զ��屨��ģ��ID         FIELD_SYSTEM_ALARM_TEMPID
*         �Զ��屨������           FIELD_SERVICE_ALARM_DISCRIPTION
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

/**�õ�������ĵ��Զ����������������Ϣ��
*�������ֵ��
*���ز���ֵ��
*         ����ID                 FIELD_SYSTEM_STATUS
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         �Զ��屨������ģ��ID   FIELD_SYSTEM_ALARM_TEMPID
*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
*         ����                   FIELD_SYSTEM_SEG_NUM
*         ����                   FIELD_SYSTEM_SEG_NAME
*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
*         ��������               FIELD_SERVICE_ALARM_LOW
*         ��������               FIELD_SERVICE_ALARM_HIGH
*         ������                 FIELD_SYSTEM_ALARM_GROUP
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

/**�õ�������ĵ�ͨ���Զ����������������Ϣ��
*�������ֵ��
*���ز���ֵ��
*         ����ID                 FIELD_SYSTEM_STATUS
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         ����                 FIELD_SYSTEM_CHANN
*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
*         ����                   FIELD_SYSTEM_SEG_NUM
*         ����                   FIELD_SYSTEM_SEG_NAME
*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
*         ��������               FIELD_SERVICE_ALARM_LOW
*         ��������               FIELD_SERVICE_ALARM_HIGH
*         ������                 FIELD_SYSTEM_ALARM_GROUP
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

/**�õ�ָ����˾���ֳ���װ��ID���Զ��������Ϣ
*�������ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID                FIELD_SYSTEM_SET
*         �豸��                FIELD_SERVICE_PLANTID
*���ز���ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         ����ID                 FIELD_SYSTEM_STATUS
*         �߼�����ģ��ID         FIELD_SYSTEM_TEMPID
*         ��������               FIELD_SYSTEM_DATA
*         ��λ����               FIELD_SYSTEM_UNIT
*         �źŷ�����ʽ           FIELD_SYSTEM_SIGNAL_TYPE
*         ����ʼ��               FIELD_SYSTEM_SEG_STARTNO
*         ����                   FIELD_SYSTEM_SEG_NUM
*         ����                   FIELD_SYSTEM_SEG_NAME
*         ������ʼֵ             FIELD_SYSTEM_START_VALUE
*         ���ݽ���ֵ             FIELD_SYSTEM_END_VALUE
*         ����ֵ����             FIELD_SYSTEM_CHAR_TYPE
*		  ����ֵ��λ			 FIELD_SERVICE_CHANN1_UNIT
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
				case GE_ENVSPECCF1:			///��Ȧ����Ƶ��         m/s2
				case GE_ENVSPECCF2:			///��Ȧ����Ƶ��         m/s2
				case GE_ENVSPECCF3:			///���ּ�����Ƶ��       m/s2
				case GE_ENVSPECCF4:			///����������Ƶ��       m/s2
				case GE_USER_AENVFREQ2:		///�����תƵ			m/s2
				case GE_GEARBOXSHAFTCF1:	///������I��תƵ		m/s2
				case GE_GEARBOXSHAFTCF2:	///������I��תƵ		m/s2
				case GE_GEARBOXSHAFTCF3:	///������I��תƵ		m/s2
				case GE_GEARBOXSHAFTCF4:	///������I��תƵ		m/s2
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

/**�õ�ָ����˾���ֳ���װ��ID���Զ����������������Ϣ
*�������ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*���ز���ֵ��
*         ����ID                 FIELD_SYSTEM_STATUS
*         �Զ��屨������ģ��ID   FIELD_SYSTEM_ALARM_TEMPID
*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
*         ����                   FIELD_SYSTEM_SEG_NUM
*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
*         ��������               FIELD_SERVICE_ALARM_LOW
*         ��������               FIELD_SERVICE_ALARM_HIGH
*         ������                 FIELD_SYSTEM_ALARM_GROUP
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

/**�õ�����������õ��Զ�����в�����
* �����ֶ��룺
*���ص��ֶ��룺
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         �������ID             FIELD_SYSTEM_STATUS
*         �������               FIELD_SYSTEM_SEG_NAME
*         �������               FIELD_SYSTEM_TYPE
*         ������Ŀ               FIELD_SYSTEM_SEG_NUM
*         ����ֱ��               FIELD_SYSTEM_START_VALUE
*         �ھ�                   FIELD_SYSTEM_END_VALUE
*         ѹ����                 FIELD_SERVICE_ANGLEPARAM1
*         ���ֱ��               FIELD_SYSTEM_BAND_STARTVALUE
*         �ھ�                   FIELD_SYSTEM_BAND_ENDVALUE
*         ��������               FIELD_SYSTEM_AXISCHANN_NUM
*         ��д���               FIELD_SYSTEM_CHAR_TYPE
*         ͬ��״̬��ʶ           FIELD_SYSTEM_SENDC_IF
*         ��ע                   FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**�õ�������ĵĳ����������
*�������ֵ��
*���ز���ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         ����������ID           FIELD_SYSTEM_STATUS
*         ���������             FIELD_SYSTEM_TYPE
*         ����������             FIELD_SYSTEM_SEG_NAME
*         ���ǳ���������ʽ       FIELD_SYSTEM_CHAR_TYPE
*         ̫���ֳ���             FIELD_SERVICE_PLANT_NUM
*         �����ָ���             FIELD_SERVICE_ALARM_PLANTNUM
*         �����ֳ���             FIELD_SERVICE_RUN_PLANTNUM
*         ����ϵ����             FIELD_SERVICE_STOP_PLANTNUM
*         ���ּ���               FIELD_SYSTEM_USER_LEVEL
*         ����Z12                FIELD_SYSTEM_GEARNUMZ12
*         ����Z21                FIELD_SYSTEM_GEARNUMZ21
*         ����Z22                FIELD_SYSTEM_GEARNUMZ22
*         ����Z31                FIELD_SYSTEM_GEARNUMZ31
*         ����Z32                FIELD_SYSTEM_GEARNUMZ32
*         ����Z41                FIELD_SYSTEM_GEARNUMZ41
*         ����Z42                FIELD_SYSTEM_GEARNUMZ42
*         ����Z51                FIELD_SYSTEM_GEARNUMZ51
*         ����Z52                FIELD_SYSTEM_GEARNUMZ52
*         ����Z61                FIELD_SYSTEM_GEARNUMZ61
*         ͬ��״̬��ʶ           FIELD_SYSTEM_SENDC_IF
*         ��ע                   FIELD_SYSTEM_STATUS_DISCRIPTION
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

/**�õ�������ĵ�λ�ò�����
*�������ֵ��
*���ز���ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         λ������ID             FIELD_SYSTEM_STATUS
*         λ������               FIELD_SYSTEM_SEG_NAME
*         ��λ����ID             FIELD_SYSTEM_CHANN_TYPE
*         �������ID             FIELD_SYSTEM_TYPE
*         ��б�����             FIELD_SYSTEM_CHAR_TYPE
*         ���                   FIELD_SYSTEM_SEG_NUM
*         ��ע                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         λ�ú�                 FIELD_SERVICE_LOCATION_NO
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

/**�õ�������ĵĲ�λ������
*�������ֵ��
*���ز���ֵ��
*         ��˾��                 FIELD_SYSTEM_COMPANY
*         �ֳ���                 FIELD_SYSTEM_FACTORY
*         װ����ID               FIELD_SYSTEM_SET
*         �豸��                 FIELD_SYSTEM_PLANT
*         ��λ����ID             FIELD_SYSTEM_STATUS
*         ��λ����               FIELD_SYSTEM_SEG_NAME
*         ����������ID           FIELD_SYSTEM_TYPE
*         ��ע                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         ��������               FIELD_SERVICE_FUNCTION_TYPE
*         ��λ����               FIELD_SERVICE_SECTION_TYPE
*         ��ǰλ��               FIELD_SERVICE_LOC_IDX
*         ǰһ��λ               FIELD_SERVICE_PREV_SEC_IDX
*         ǰһλ��               FIELD_SERVICE_PREV_LOC_IDX
*         ��λͼƬ               FIELD_SERVICE_PREV_SECTION_PIC
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


/** ��ȡ���������·���Ͱ汾��
  *��������ֶ��룺��
  *����ֵ���ֶ��룺(n)
  *            ������                 FIELD_SERVICE_MODULE_NAME
  *            �ļ���                 FIELD_SERVICE_FILE_NAME
  *            ·��                   FIELD_SERVICE_FILE_PATH
  *            �汾��                 FIELD_SERVICE_FILE_VERSION
  *\instance ��:
      [JSON]
      "MiddleWare" : {
         "POSITION" : "E:\\Project\\lib\\S_SystemSvr.dll",
         "POSITION1" : "E:\\Project\\lib\\S_OnlineSvr.dll",
         "POSITION2" : "E:\\Project\\lib\\S_OfflineSvr.dll",
         "S_OfflineSvr.dll" : "3.5.0",
         "S_OnlineSvr.dll" : "3.5.0",
         "S_SystemSvr.dll" : "3.5.0"
      },

      [�м��ͨ�����ݸ�ʽ]
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
		HZLOG_ERROR(STR("�޷����ļ�%s", statusFile));
		return -1;
	}

	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_BASIC_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
		                statusFile,
		                DAEMON_STATUS_BASIC_INFO));
		return -3;
	}

	Json::Value basicInfo = root[DAEMON_STATUS_BASIC_INFO];

	if(!basicInfo.isMember(DAEMON_STATUS_SOFTWARE_VERSION)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s]û��ģ��",
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
					HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s].[%s].%s�޶�Ӧ���ļ�·��",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str()));

					/* ���ļ�·��, ���� */
					continue;
				}

				size_t sepratorLoction = filePath.rfind('\\');
				if(string::npos == sepratorLoction){
					HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s].[%s].%s��Ӧ���ļ�·��%s��ʽ����",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str(),
					filePath.c_str()));

					/* ��ʽ����, ���� */
					continue;
				}

				std::string fileName = filePath.substr(sepratorLoction + 1);
				if(fileName.empty()){
					HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s].[%s].%s��Ӧ���ļ�����",
					statusFile,
					DAEMON_STATUS_BASIC_INFO,
					DAEMON_STATUS_SOFTWARE_VERSION,
					moduleName.c_str(),
					it->c_str()));

					/* �޷��������ļ���, ���� */
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
				HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s].[%s]��û���ļ�%s��·����Ϣ",
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

/**�õ��м���İ汾��
*�������ֵ��
*���ز���ֵ��
*         �汾��                 FIELD_SERVICE_FILE_VERSION
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


/** ��ȡ�������ݿ�״̬
  *��������ֶ��룺��
  *����ֵ���ֶ��룺(n)
  *     ��������Ԥ�Ʊ���ʱ��,��λ ��(AlarmDataSavingTime)     FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME
  *     ��������Ԥ�Ʊ���ʱ��(DataSavingTime)                  FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME
  *     ��������������Ԥ�Ʊ���ʱ��(TrendDataSavingTime)       FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME
  *     ���ݿ���Ŀ(DBNum)                                     FIELD_SERVICE_DB_NUM
  *     ���ݿ��ļ����,��0, �����ݿ��ļ��� 1�������ļ�1, ...  FIELD_SERVICE_DB_INDEX
  *     �����ļ���(DBDataFileName)                            FIELD_SERVICE_DB_DATA_FILE_NAME
  *     �����ļ���С����λ GB(DBDataFileSize)                 FIELD_SERVICE_DB_DATA_FILE_SIZE
  *     ��־�ļ���(DBLogFileName)                             FIELD_SERVICE_DB_LOG_FILE_NAME
  *     ��־�ļ���С����λ GB(DBLogFileSize)                  FIELD_SERVICE_DB_LOG_FILE_SIZE
  *     ���ݿ���(DBName)                                      FIELD_SERVICE_DB_NAME
  *     ���ݿ����ͣ�0��ʾ���⣬1��ʾ�ֿ�(DBType)              FIELD_SERVICE_DB_TYPE
  *     ���ݿ����ڴ���ʣ��ռ䣬��λΪGB(LeftDiskSpace)       FIELD_SERVICE_FREE_DISK_SPACE
  *     ���ݿ����ڴ���ʣ��ռ��(LeftDiskSpaceRate)           FIELD_SERVICE_FREE_DISK_SPACE_RATIO
  *     ǰһ���������ı�CSV��ʽ(TheFastIncTableName)      FIELD_SERVICE_FAST_INC_TABLE
  *     ���ݿ����ڴ����ܿռ䣬��λΪGB(TotalDiskSpace)        FIELD_SERVICE_TOTAL_DISK_SPACE
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
		HZLOG_ERROR(STR("�޷����ļ�%s", statusFile));
		return -1;
	}

	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_DB_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
		                statusFile,
		                DAEMON_STATUS_DB_INFO));
		return -3;
	}

	Json::Value DBInfo = root[DAEMON_STATUS_DB_INFO];

	double AlarmDataSavingTime         = -1;
	double AlarmTrendSavingTime        = -1;
	double AlarmTrendAndDataSavingTime = -1;

	if(!DBInfo.isMember(DAEMON_STATUS_DB_STORING_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
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
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
		                statusFile,
		                DAEMON_STATUS_DB_DISK_INFO));

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -5;
	}

	Json::Value diskInfo = DBInfo[DAEMON_STATUS_DB_DISK_INFO];

	if(!diskInfo.isMember(DAEMON_STATUS_DB_NUM)){
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s]��û��%s",
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s]��û��%s",
			statusFile,
			DAEMON_STATUS_DB_INFO,
			DAEMON_STATUS_DB_DISK_INFO,
			DAEMON_STATUS_DB_NUM));

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -7;
	}

	if(!diskInfo.isMember(DAEMON_STATUS_DB_MAIN)){
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s]��û��%s",
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
			HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s].[%s]��û��%s",
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


/** ��ȡ�м������״̬
  *��������ֶ��룺��
  *����ֵ���ֶ��룺(1)
  *            ���ݿ�״̬             FIELD_SERVICE_DB_STATUS
  *            �������ݵĹ�˾��       FIELD_SYSTEM_COMPANY
  *            �������ݵķֳ���       FIELD_SYSTEM_FACTORY
  *            �������ݵ��豸��Ŀ     FIELD_SERVICE_DATA_RCV_PLANT_NUM
  *            �������ݵ��豸״̬     FIELD_SERVICE_DATA_RCV_STATUS
  *            �������ݵ��豸��Ŀ     FIELD_SERVICE_DATA_SAVE_PLANT_NUM
  *            �������ݵ��豸״̬     FIELD_SERVICE_DATA_SAVE_STATUS
  *            ͬ�����ݵ��豸��Ŀ     FIELD_SERVICE_DATA_SYNC_PLANT_NUM
  *            ͬ�����ݵ��豸״̬     FIELD_SERVICE_DATA_SYNC_STATUS
  *
  *\description ����˵����
  *            ���ݿ�״̬,     ���� GE_DB_STATUS
  *            �������ݵ��豸״̬������ BUFFER, BHMWDataRcvStatus[]
  *            �������ݵ��豸״̬������ BUFFER, BHMWDataSaveStatus[]
  *            ͬ�����ݵ��豸״̬������ BUFFER, BHMWDataSyncStatus[]
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
		HZLOG_ERROR(STR("�޷����ļ�%s", statusFile));
		return -1;
	}

	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_MW_STATUS)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
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
	  ��ΪcompanyName �� factoryName��itemList, �ʼ���������Ŀʱ���2
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s]��û��%s",
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s]��û��%s",
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, [%s]��û��%s",
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
		HZLOG_ERROR(STR("�ػ�ϵͳ��־�ļ�%s�м�¼����, ��˾���ֳ����쳣 (%s, %s)",
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
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼ȱʧ, [%s]��û�м�¼",
			statusFile,
			DAEMON_STATUS_MW_DATA_RECEIVING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_RCV_STATUS,
		          (char *)&dataRcvStatus[0],
		          dataRcvStatus.size() * sizeof(BHKD::BHMWDataRcvStatus));
		}

	pFixComm->SetItem(FIELD_SERVICE_DATA_SAVE_PLANT_NUM, (int)dataSaveStatus.size());
	if(dataSaveStatus.size() > 0){
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼ȱʧ, [%s]��û�м�¼",
			statusFile,
			DAEMON_STATUS_MW_DATA_SAVING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_SAVE_STATUS,
		          (char *)&dataSaveStatus[0],
		          dataSaveStatus.size() * sizeof(BHKD::BHMWDataSaveStatus));
	}

	pFixComm->SetItem(FIELD_SERVICE_DATA_SYNC_PLANT_NUM, (int)dataSyncStatus.size());
	if(dataSyncStatus.size() > 0){
		HZLOG_WARN(STR("�ػ�ϵͳ��־�ļ�%s�м�¼ȱʧ, [%s]��û�м�¼",
			statusFile,
			DAEMON_STATUS_MW_DATA_SENDING_STATUS));
		pFixComm->SetItemBuf(FIELD_SERVICE_DATA_SYNC_STATUS,
		          (char *)&dataSyncStatus[0],
		          dataSyncStatus.size() * sizeof(BHKD::BHMWDataSyncStatus));
	}


	pFixComm->Write(pFixComm->GetAci());

	return dataRcvStatus.size() + dataSaveStatus.size() + dataSyncStatus.size();
}

/** ��ȡ����Ӳ��״̬
  *�������ֵ����
  *����ֵ���ֶ��룺(n)
  *            ���ɿ�����             FIELD_SERVICE_DAQ_CARD_INDEX
  *            ���ɿ�����             FIELD_SERVICE_DAQ_CARD_NUM
  *            ���ɿ��߼�����         FIELD_SERVICE_DAQ_CARD_NAME
  *            ʹ��AIͨ��(CSV)        FIELD_SERVICE_DAQ_USED_AI_CHANNEL
  *            ʹ��CIͨ��(CSV)        FIELD_SERVICE_DAQ_USED_CI_CHANNEL
  *            ����״��(�ַ�������)   FIELD_SERVICE_DAQ_CARD_STATUS
  *            ��ȡ����Ӳ��״̬ʱ��   FIELD_SERVICE_OFF_TIME
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
		HZLOG_ERROR(STR("�޷����ļ�%s, �����ؽ�%s\n", statusFile));
		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -1;
	}

	_HZLOG_DEBUG(STR("Parsing %s...", statusFile));


	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	_HZLOG_DEBUG(STR("Closed %s", statusFile));

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s, �����ؽ�\n",
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
		HZLOG_WARN(STR("�޷���ȡ�ļ�%s�ϴθ���ʱ��, �����ؽ�",
			statusFile));

		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -3;
	}

	std::string sLastUpdateTime =
		root.get(DAQ_LAST_UPDATE_TIME, "").asString();
	CBHDateTime tLastUpdateTime;
	if(!tLastUpdateTime.ParseDateTime(sLastUpdateTime.c_str())){
		HZLOG_WARN(STR("�޷���ȡDAQ״̬����ʱ��, ����״̬δ֪",
			statusFile));

		verifyDAQHWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -4;
	}


	int cardCount = root.get(DAQ_HW_STATUS_CARD_COUNT, 0).asInt();

	if(cardCount < 0){
		HZLOG_ERROR(STR("���ɿ���Ŀ(%d)��Ч\n", cardCount));
		::SetNullReturnFixcommEx(pFixComm);
		return -5;
	}

	if(!root.isMember(DAQ_HW_STATUS_CARD_INFO)){
		HZLOG_ERROR(STR("û�����ɿ�״̬��¼"));
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
			HZLOG_ERROR(STR("û�����ɿ�%s��״̬��¼\n", cardId.c_str()));

			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_INDEX, i);
			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_NUM, cardCount);

			pFixComm->SetItem(FIELD_SERVICE_DAQ_CARD_STATUS,
			          STR("û�����ɿ�%s��״̬��¼\n",
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

/** ��ȡ�������״̬
  *�������ֵ����
  *����ֵ���ֶ��룺(1)
  *            �汾��                                   FIELD_SERVICE_DAQ_SW_VERSION
  *            �ɼ�ģʽ                                 FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            ����ʱ��                                 FIELD_SERVICE_TIME
  *            ���һ�������ݷ���ʱ��                 FIELD_SERVICE_DAQ_VIB_TIME
  *            ���һ�ζ�̬���ݷ���ʱ��                 FIELD_SERVICE_DAQ_DYN_TIME
  *            ���һ�ι��������ݷ���ʱ��               FIELD_SERVICE_DAQ_PROC_TIME
  *            ���ռ�϶��ѹ״̬                         FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            ���һ�ζ������ݷ��Ϳ�ʼʱ��             FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            ���һ�ζ������ݷ��ͽ���ʱ��             FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  *            ��ȡ�������״̬ʱ��                     FIELD_SERVICE_OFF_TIME
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
		HZLOG_ERROR(STR("�޷����ļ�%s, �����ؽ�%s\n", statusFile));
		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -1;
	}

	_HZLOG_DEBUG(STR("Parsing %s...", statusFile));

	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	_HZLOG_DEBUG(STR("Closed %s", statusFile));

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s, �����ؽ�\n",
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
		HZLOG_WARN(STR("�޷���ȡ�ļ�%s�ϴθ���ʱ��, �����ؽ�",
			statusFile));

		verifyDAQSWStautsFile();
		::SetNullReturnFixcommEx(pFixComm);
		return -3;
	}

	std::string sLastUpdateTime =
		root.get(DAQ_LAST_UPDATE_TIME, "").asString();
	CBHDateTime tLastUpdateTime;
	if(!tLastUpdateTime.ParseDateTime(sLastUpdateTime.c_str())){
		HZLOG_WARN(STR("�޷���ȡDAQ״̬����ʱ��, ����״̬δ֪",
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


/** ��ȡ��������̬Ϣ
  * �������ֵ����
  * ����ֵ���ֶ��룺(1)
  *          ��������(������0��ʾδ����)                FIELD_SERVICE_CONFIG_ALARM
  *          ���ӵ����ݿ�                               FIELD_SERVICE_CONFIG_CONNECTED_DB
  *          ������������(������0��ʾδ����)            FIELD_SERVICE_CONFIG_DATA_SAVING
  *          ͬ����������(������0��ʾδ����)            FIELD_SERVICE_CONFIG_DATA_SYNC
  *          �м����������(0��ʾװ�ü���1��ʾ���ļ�)   FIELD_SERVICE_CONFIG_SERVER_TYPE
  *          ͳ����Ϣ��˾��Ŀ                           FIELD_SERVICE_COMPANY_NUM
  *          ͳ����Ϣ�豸��Ŀ                           FIELD_SERVICE_PLANT_NUM
  *          ͳ����Ϣ�����Ŀ                           FIELD_SERVICE_CHANN_NUM
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
		HZLOG_ERROR(STR("�޷����ļ�%s", statusFile));
		return -1;
	}

	/* ��ȡ״̬�ļ� */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		HZLOG_ERROR(STR("�޷������ļ�%s, %s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	if(!root.isMember(DAEMON_STATUS_BASIC_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
		                statusFile,
		                DAEMON_STATUS_BASIC_INFO));
		return -3;
	}

	Json::Value basicInfo = root[DAEMON_STATUS_BASIC_INFO];

	if(!basicInfo.isMember(DAEMON_STATUS_CONFIG_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
		                statusFile,
		                DAEMON_STATUS_CONFIG_INFO));
		return -4;
	}

	if(!basicInfo.isMember(DAEMON_STATUS_STATISTIC_INFO)){
		HZLOG_ERROR(STR("�ػ�ϵͳ��־%s��û��%s",
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
		_snprintf( pStrInfo,sizeof(pStrInfo), "û��ָ�� %s ��˾ %s �豸 ",sCompany_.c_str(), sPlantID_.c_str());
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
		_snprintf( pStrInfo,sizeof(pStrInfo), "û��ָ�� %s ��˾ %s �豸 ",sCompany_.c_str(), sPlantID_.c_str());
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
