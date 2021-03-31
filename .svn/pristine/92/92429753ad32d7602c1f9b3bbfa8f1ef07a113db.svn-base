#pragma once
#include "BHACO.h"
#include "LogI.h"
using namespace BHAco;
class CDialog;
#include "RequestHandle.h"

class CRequestI: public Request
{
public:
	CRequestI(const LogIPtr&, CWnd*);
	CRequestI();
	virtual SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Current& = ::Ice::Current());
	virtual void sayHello(int, const Ice::Current&) const;
	virtual void shutdown(const Ice::Current&);
private:

	LogIPtr _log;
	void* _dialog;
private:
	string parseRemoteAddrFromConnStr(const string& strconn_);
};

class CDataPacketDebugInfo{
public:
	CDataPacketDebugInfo(const SMessageHeader& msgHeader_, const PackageDetail& packet_, const Warnings& curwarning_, const int& retcode_);
	~CDataPacketDebugInfo();
public:
	SDataPacket __datapacket;
};
