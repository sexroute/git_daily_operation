#include "StdAfx.h"
#include "bhacoi.h"
#include <IceUtil/IceUtil.h>
#include <ice/Connection.h>
#include "RequestHandle.h"
#include "aco/SrvDef.h"
#include <string>
#include "kdcommlib.h"
#include "MonitorICEServer.h"
using namespace BHAco;
using namespace std;

static CRequestHandle*  _g_requesthandle = new CRequestHandle;

CRequestI::CRequestI(const int & log, CWnd* dialog) :_log(log)
{
	_dialog = dialog;
	string str = "CRequestI constuction";
	this->m_pServer = NULL;
	BHLOG_DEBUG(str.c_str());
}

CRequestI::CRequestI()
{
	string str = "CRequestI constuction 2";
	this->m_pServer = NULL;
	BHLOG_DEBUG(str.c_str());
}

CRequestI::CRequestI( LY::IServer* apServer )
{
	string str = "CRequestI constuction 3";
	this->m_pServer = apServer;
	BHLOG_DEBUG(str.c_str());
}

SDataPacket CRequestI::RequestData(const ::BHAco::SDataPacket& dp_, const ::Ice::Current& ic_) 
{
	char cc[100];
    SYSTEMTIME stimecur;
	_ULARGE_INTEGER  starttime,endtime;
	bool bmonitor = true;	
	bool bdebug   = true;
    SDataPacket *request = NULL, *response = NULL;
	
	if (bmonitor)
	{
		GetSystemTime(&stimecur);
		SystemTimeToFileTime(&stimecur, (FILETIME*)&starttime);
		if (bdebug)
		{
			request = new SDataPacket(dp_);
			//request->msgHeader = dp_.msgHeader;
            //request->packet = dp_.packet;
			//request->curWarnings = dp_.curWarnings;
			//request->nRetCode = dp_.nRetCode;
		}
	}
	///handle request service
    SDataPacket &dp =_g_requesthandle->HandleSvr(dp_,this->m_pServer);
    
	if (bmonitor)
	{
		GetSystemTime(&stimecur);
		SystemTimeToFileTime(&stimecur,(FILETIME*)&endtime);
		float  dft= (endtime.QuadPart - starttime.QuadPart)/10000.0;
	
		FILETIME LocalFileTime;
		FileTimeToLocalFileTime((FILETIME*)&starttime, &LocalFileTime);
		FileTimeToSystemTime(&LocalFileTime, &stimecur);

		sprintf(cc,"%d-%d-%d %d:%d:%d(%.2f ms)",stimecur.wYear,stimecur.wMonth, stimecur.wDay, stimecur.wHour, stimecur.wMinute, stimecur.wSecond, dft);

		int servicecode;
		string shost = parseRemoteAddrFromConnStr(ic_.con->toString());
		servicecode = dp_.msgHeader.nServiceCode;
		int rows = dp.packet.size();
		if (bdebug)
		{
			response = new SDataPacket(dp);	
    	}
		CMonitorICEServer::GetInstance()->requestdebuginfo(shost.c_str(),0,servicecode,rows,false,cc,request,response);
	}
	
	
	

	//sprintf(cc,"%d size:%d end threadid %d,%ds, %d",dp.msgHeader.nServiceCode,dp.msgHeader.bodylength, __threadid(),dt.GetSecond(),this);	
	//_log->message(cc);
	return dp;
}

void CRequestI::sayHello(int delay, const Ice::Current& tt) const
{
//	tt.con->toString();
	if(delay != 0)
	{
		IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(delay));
	}
	BHLOG_DEBUG("Hello World!");
}

void CRequestI::shutdown(const Ice::Current&)
{
	if(_dialog)
	{
		BHLOG_DEBUG("Shutting down...");
		((CDialog*)_dialog)->EndDialog(0);
		_dialog = 0;
	}
}

string CRequestI::parseRemoteAddrFromConnStr(const string& strconn_)
{
//standand format:  "local address = 127.0.0.1:8000remote address = 127.0.0.1:3518";
	string strTmp = strrchr(strconn_.c_str(),'=');
	strTmp.erase(0,1);
	int ind = strTmp.find(':');
	strTmp.erase(ind,strTmp.size() -ind);
	return strTmp;
}

CDataPacketDebugInfo::CDataPacketDebugInfo(const SMessageHeader& msgHeader_, const PackageDetail& packet_, const Warnings& curwarning_, const int& retcode_)
{
	__datapacket.curWarnings.clear();
	__datapacket.packet.clear();
   __datapacket.msgHeader = msgHeader_;
   __datapacket.curWarnings = curwarning_;
   __datapacket.nRetCode = retcode_;
   __datapacket.packet  = packet_;
}

CDataPacketDebugInfo::~CDataPacketDebugInfo()
{
	__datapacket.curWarnings.clear();
   for (PackageDetail::iterator it = __datapacket.packet.begin(); it!= __datapacket.packet.end(); ++it)
   	   it->clear();
   __datapacket.packet.clear();   
}