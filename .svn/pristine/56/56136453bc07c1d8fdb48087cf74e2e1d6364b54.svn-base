#pragma once
#include <aco/BHACO.h>
#include <IServer.h>
using namespace BHAco;

class ACO;
class CRequestHandle
{
public:
	CRequestHandle(void);
	~CRequestHandle(void);
public:
	SDataPacket HandleSvr(const SDataPacket& dp_,LY::IServer * a_pServer);
private:
	 //ÑéÖ¤µÇÂ½
	SDataPacket LoginCheck(const SDataPacket& dp_);
	int CaleDataPacketSize(const SDataPacket& dp_);
private:
	  IceUtil::Monitor<IceUtil::Mutex> _monitor;
	  ACO *__aco;
	  
};
