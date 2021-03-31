//Implement.h
#include <additions/IDBInterFace.h>
#include <additions/ICommInterFace.h>
#pragma  once

#ifndef _SYSTEMSVR_IMPLEMENT_H_
#define _SYSTEMSVR_IMPLEMENT_H_

class CImplement
{
public:
	CImplement(void);
	~CImplement(void);
public:
	/**修改监测中心中间件IP和Port
	*输入参数：
	*           所在分厂       FIELD_SYSTEM_FACTORY
	*           装置名ID        FIELD_SYSTEM_SET
	*          中间件IP    FIELD_SYSTEM_MW_IP
	*          中间件PORT  FIELD_SYSTEM_MW_PORT
	*返回值的字段码：
	*          状态     FIELD_SYSTEM_STATUS
	*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
	int SysModifyMwInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);


};
#endif