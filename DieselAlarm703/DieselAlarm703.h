// DieselAlarm703.h : DieselAlarm703 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CDieselAlarm703App
// �йش���ʵ�ֵ���Ϣ������� DieselAlarm703.cpp
//

class CDieselAlarm703App : public CWinApp
{
public:
	CDieselAlarm703App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
