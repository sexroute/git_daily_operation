#pragma pack (1)
#include "stdafx.h"
#include "process.h"
#include "Implement.h"
#include <MiddleWare.h>



/**�޸ļ�������м��IP��Port
*���������
*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
*           װ����ID        FIELD_SYSTEM_SET
*          �м��IP    FILELD_SYSTEM_MW_IP
*          �м��PORT  FILELD_SYSTEM_MW_PORT
*����ֵ���ֶ��룺
*          ״̬     FIELD_SYSTEM_STATUS
*          ״̬���� FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
int Svr_SysModifyMwInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB)
{
	CImplement impl;
	return impl.SysModifyMwInfo(pFixComm,pDB);
}

