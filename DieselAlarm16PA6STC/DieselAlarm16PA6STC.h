// DieselAlarm16PA6STC.h : DieselAlarm16PA6STC DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CDieselAlarm16PA6STCApp
// �йش���ʵ�ֵ���Ϣ������� DieselAlarm16PA6STC.cpp
//

class CDieselAlarm16PA6STCApp : public CWinApp
{
public:
	CDieselAlarm16PA6STCApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
