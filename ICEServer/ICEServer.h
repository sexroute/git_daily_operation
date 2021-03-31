// ICEServer.h : ICEServer DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号
#include <IServer.h>
#include <ice/Properties.h>
#include <ice/Connection.h>
#include <ice/Initialize.h>
#include "LogI.h"
// CICEServerApp
// 有关此类实现的信息，请参阅 ICEServer.cpp
//

class CICEServerApp : public CWinApp
{
public:
	CICEServerApp();

// 重写
public:
	virtual BOOL InitInstance();

	static LY::IServer * m_pServer;

	static LY::IServer * GetServer();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

class ICEServer :
	public LY::IServer
{
public:
	ICEServer(void);
	virtual ~ICEServer(void);
	virtual int Init();
	virtual int Start();
	virtual int Stop();
	virtual int RegServiceCallBackHandler(int   hInstancemain,LY::CALLBACK_PROC apfProc);
	virtual int HandleSrvData(void * apData,void * response);

	BOOL Inited() const;
	void Inited(BOOL val);

	Ice::InitializationData PropData() const;
	void PropData(Ice::InitializationData & val);

	BOOL Started() const;
	void Started(BOOL val);

	void CreateDebugInfoWindow(CString lstrInfo,BOOL abInitSucceed);

	BOOL m_bInited;
	BOOL m_bStarted;
	Ice::InitializationData __propData;
	Ice::CommunicatorPtr __communicator;
	Ice::ObjectAdapterPtr __adapter;
	LogIPtr __logI;
};


