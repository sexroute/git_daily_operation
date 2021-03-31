#pragma once
#include "aco/BHACO.h"
#include <string>
#include <IServer.h>
using namespace BHAco;
class CDialog;
#include "RequestHandle.h"

class CRequestI: public Request
{
public:
	
	CRequestI(LY::IServer* apServer);
	CRequestI();
	virtual SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Current& = ::Ice::Current());
	virtual void sayHello(int, const Ice::Current&) const;
	virtual void shutdown(const Ice::Current&);
private:
	CRequestI(const int&, CWnd*);
	int _log;
	void* _dialog;
	LY::IServer * m_pServer;
private:
	std::string parseRemoteAddrFromConnStr(const std::string& strconn_);
};

class CDataPacketDebugInfo{
public:
	CDataPacketDebugInfo(const SMessageHeader& msgHeader_, const PackageDetail& packet_, const Warnings& curwarning_, const int& retcode_);
	~CDataPacketDebugInfo();
public:
	SDataPacket __datapacket;
	
};
