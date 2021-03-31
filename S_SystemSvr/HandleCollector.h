#pragma once
#ifndef __CHandleCollector__
#define __CHandleCollector__

extern BOOL g_bEnableDumpMonitor;
class CHandleCollector
{
private:
	CHandleCollector(void);
public:
	virtual ~CHandleCollector(void);
	static void Init();
	static void StartTrack();
	static void StopTrack();
	static void SetCheckInterval(DWORD adwMilliSeconds);
	static void SetConnLimitPerPeer(DWORD adwConnLimitPerPeer);
	static void SetEnableTrack(BOOL abEnableTrack);

};


#endif