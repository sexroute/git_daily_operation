// DieselAlarm956.h : DieselAlarm956 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CDieselAlarm956App
// �йش���ʵ�ֵ���Ϣ������� DieselAlarm956.cpp
//

class CDieselAlarm956App : public CWinApp
{
public:
	CDieselAlarm956App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
