#pragma once
#ifndef __MONITOR_ICE_SERVER__H__
#define __MONITOR_ICE_SERVER__H__

#ifdef ICE_MONITOR_EXPORTS
#define EXPORT_ICE_MONITOR __declspec(dllexport)
#else
#define EXPORT_ICE_MONITOR __declspec(dllimport)
#endif

class EXPORT_ICE_MONITOR CMonitorICEServer
{
protected:
	CMonitorICEServer(void);
public:
	static CMonitorICEServer* GetInstance();
	bool InitialDialog();
	bool DestroyMonitorWindow();
	void SetServerInfo(CString & astrInfo,BOOL abSucceed);
	void requestdebuginfo(const char* rhost, const int ver,const int code,int result, bool ssl, const char* time, const void* request, const void* response);
private:
    static CMonitorICEServer* pMonitor; 
	static HWND hLogWnd;
	HWND FindWindowByProcessID(DWORD dwPID_);
};
#endif