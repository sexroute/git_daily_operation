// S_Processor.h : S_Processor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CS_ProcessorApp
// �йش���ʵ�ֵ���Ϣ������� S_Processor.cpp
//

class CS_ProcessorApp : public CWinApp
{
public:
	CS_ProcessorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
