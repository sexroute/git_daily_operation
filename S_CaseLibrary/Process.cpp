#pragma pack (1)
#include "stdafx.h"
#include "process.h"
#include "Implement.h"
#include <MiddleWare.h>



/**修改监测中心中间件IP和Port
*输入参数：
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          中间件IP    FILELD_SYSTEM_MW_IP
*          中间件PORT  FILELD_SYSTEM_MW_PORT
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
int Svr_SysModifyMwInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB)
{
	CImplement impl;
	return impl.SysModifyMwInfo(pFixComm,pDB);
}

