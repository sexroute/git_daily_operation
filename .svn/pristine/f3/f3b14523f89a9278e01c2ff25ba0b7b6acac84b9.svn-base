#pragma once
#ifndef __MONITOR_ICE_SERVER__H__
#define __MONITOR_ICE_SERVER__H__

class __declspec(dllexport) CMonitorICEServer
{
public:
	CMonitorICEServer(void);
public:
	static CMonitorICEServer* GetInstance();
	bool InitialDialog();
	bool DestroyMonitorWindow();
	void requestdebuginfo(const char* rhost, const int ver,const int code,int result, bool ssl, const char* time, const void* request, const void* response);
	void SetServerInfo(CString & astrInfo,BOOL abSucceed);

private:
    static CMonitorICEServer* pMonitor; 
	static HWND hLogWnd;
	CDialog* pDlg;
	HWND FindWindowByProcessID(DWORD dwPID_);
};
#endif