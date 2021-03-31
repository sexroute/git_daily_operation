#include "StdAfx.h"
#include ".\requesthandle.h"
#include <map>
#include "aco\SrvDef.h"
#include "aco/ACO.h"
#include "aco/ACOImpl.h"
#include "dbconnman.h"
#include "hcom/modman.h"
#include "hcom/logger.h"
using namespace std;
CRequestHandle::CRequestHandle(void)
{
	__aco = new ACO;
}

CRequestHandle::~CRequestHandle(void)
{
	delete __aco;
}

SDataPacket CRequestHandle::HandleSvr(const SDataPacket& dp_)
{
	// IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_monitor);
   SDataPacket dp;
   dp.msgHeader.nServiceCode = dp_.msgHeader.nServiceCode;
   dp.msgHeader.nVersion     = dp_.msgHeader.nVersion;
   int iRetCode = -1;
   switch(dp_.msgHeader.nServiceCode)
   {
   case SERVICE_SYSTEM_LOGIN:
	   dp = LoginCheck(dp_);
	   break;
   default:
	   {
		   ACO* aco = new ACO();
		   try
		   {
			   aco->GetAcoImpl()->SetResultset(dp_);
			   dbdriver* dbconn = dbconnman::instance()->getdbconn(__threadid());
			   haslogger* logger=haslogger::instance();
			   if (dbconn)
			   {
				   service_entry_parameter __param={aco,dbconn,logger,NULL,NULL};
				   iRetCode = execute(aco->GetVersion(),aco->GetServiceCode(),__param);
				   aco->GetAcoImpl()->GetResultset(dp);
			   }
			   else
			   {
                 TTrace::DebugEx(100,"CRequestHandle::HandleSvr,database conn is null");
			   }
		   }
		   catch(IceUtil::Exception &ex_)
		   {
              TTrace::DebugEx(100,"CRequestHandle::HandleSv Ice Exception");
		   }
		   catch(...)
		   {
              TTrace::DebugEx(100,"CRequestHandle::HandleSv Unknown Exception");
		   }
		   delete aco;
	   }
	   break;
   }
   dp.msgHeader.bodylength = CaleDataPacketSize(dp);
   dp.msgHeader.nVersion = 0;
   return dp;
}

SDataPacket CRequestHandle::LoginCheck(const SDataPacket& dp_)
{
	SDataPacket dp;
	dp.packet.clear();
	dp.curWarnings.clear();
	dp.msgHeader.nServiceCode = dp_.msgHeader.nServiceCode;
	if (dp_.msgHeader.sessionid == "ICEP")
		dp.nRetCode = 1;
	else
		dp.nRetCode = -1;
	return dp;
}

int CRequestHandle::CaleDataPacketSize(const SDataPacket& dp_)
{
	int size = 0;
	const PackageDetail& pd = dp_.packet;
	int rows = pd.size();
	int rowsize = 0;
	for (int ind = 0; ind < rows; ++ind)
	{
		rowsize  = 0;
		const RowDetail rd = pd[ind];
		for (RowDetail::const_iterator it = rd.begin(); it != rd.end(); ++it)
		{
			const SFieldContent& fd = it->second;
			rowsize += 4 + 9 + fd.strValue.size() + fd.buf.size();
		}
		size += rowsize;
	}
	//添加功能码 和尺寸大小变量所占的字节
	size += 8;
	return size;
}