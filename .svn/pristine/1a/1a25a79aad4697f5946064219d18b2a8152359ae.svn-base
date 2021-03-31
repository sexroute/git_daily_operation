#pragma once
#ifndef __CHandleCollector__
#define __CHandleCollector__
#include <map>
#include <vector>
#include "ntdll.h"
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
void GetHandlesMap(DWORD adwProcessId,std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> & arefMap);
void GetHandlesVo(std::vector<NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> & arefMap);
CString GetHandleTypeName(HANDLE hHandle);
#endif