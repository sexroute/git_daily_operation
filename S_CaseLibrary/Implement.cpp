#include "stdafx.h"
#include <algorithm>
#include "implement.h"

#include <fstream>
#include <json/json.h>
#include <MiddleWare.h>
#include <additions/DebugHelper.h>
#include <additions/IDBInterFace.h>
#include <additions/ICommInterFace.h>


CImplement::CImplement(void)
{
}

CImplement::~CImplement(void)
{
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
int CImplement::SysModifyMwInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB)
{
   SwitchDbNameEx(&pDB,g_strMainDbName);
   string sCompany=pFixComm.Get(FIELD_SYSTEM_COMPANY);
   string sFactory=pFixComm.Get(FIELD_SYSTEM_FACTORY);
   string sSetID=pFixComm.Get(FIELD_SYSTEM_SET);
   string strIP=pFixComm.Get(FIELD_SYSTEM_MW_IP);
   int iPort=pFixComm.GetLong(FIELD_SYSTEM_MW_PORT);
   string strMwID="";
   int iStatus=-1;

   pFixComm.CreateAnswer(pFixComm.GetFunc());
   pDB.Cmd("SELECT %s from %s ",gc_cSetID,gc_cSetTable);
   pDB.Cmd(" %s ",strMwID.c_str());
   if(pDB.More())
	   strMwID=pDB.Get(0,FALSE).GetBuffer(0);
   else
	   goto CL_SYSMODIFYMWINFO;

   pDB.Cmd("UPDATE %s SET %s='%s',%s=%d ",gc_cSetTable,gc_cCenterMw_IP,strIP.c_str(),gc_cCenterMw_Port,iPort);
   pDB.Cmd(" WHERE %s='%s'",gc_cSetID,strMwID.c_str());
   if (pDB.Exec())
	   iStatus=0;

CL_SYSMODIFYMWINFO:
    pFixComm.SetItem(FIELD_SYSTEM_STATUS,iStatus);
	pFixComm.Write(pFixComm.GetAci());
	return iStatus;
}

