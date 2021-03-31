/*
* Copyright (c) 2010, BH 网软件部
* All rights reserved.
* 
* 文件名称： CBHCommWrapper.cpp
* 摘    要： CFixComm 库封装类
* 作    者： 赵大力
* 修 改 者1：
* 
*/
// ZdlDbWrapper.cpp : 实现文件
//

#include <StdAfx.h>
#include <additions/BHCommWrapper.h>


CBHCommWrapper::CBHCommWrapper()
{
	this->m_pFixComm = NULL;
}

CBHCommWrapper::~CBHCommWrapper()
{
	this->m_pFixComm = NULL;
}

double	CBHCommWrapper::GetDouble( int nFixCode , int         nRow)
{

	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return 0;
	}

	return this->m_pFixComm->GetDouble(nFixCode,nRow);
};
long	CBHCommWrapper::GetLong( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return 0;
	}

	return this->m_pFixComm->GetLong(nFixCode,nRow);
};

CString CBHCommWrapper::Get( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return _T("");
	}

	return this->m_pFixComm->Get(nFixCode,nRow);
}

// 根据FIX代码取出相应的结果。
CString CBHCommWrapper::GetItem( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return _T("");
	}

	return this->m_pFixComm->GetItem(nFixCode,nRow);
};

COleDateTime CBHCommWrapper::GetTime   ( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return COleDateTime();
	}

	return this->m_pFixComm->GetTime(nFixCode,nRow);
}

bool CBHCommWrapper::GetItemBuf( int nFixCode, char *pBuf, int nSize ) 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->GetItemBuf(nFixCode,pBuf,nSize);
	
}
bool CBHCommWrapper::SetItemBuf( int nFixCode, char *pBuf, int nSize ) 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->SetItemBuf(nFixCode,pBuf,nSize);

}

// 设置某请求域浮点性数据 -- 浮点性
BOOL    CBHCommWrapper::SetItem    ( int nFixCode , double      dValue   ) 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->SetItem(nFixCode,dValue);
}

// 设置某请求域整性数据   -- 整性
BOOL    CBHCommWrapper::SetItem    ( int nFixCode , long        nValue   )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->SetItem(nFixCode,nValue);
}
BOOL    CBHCommWrapper::SetItem( int nFixCode , int         nValue   )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->SetItem(nFixCode,nValue);
}

// 设置某请求域字符串数据 -- 字符串
BOOL    CBHCommWrapper::SetItem( int nFixCode , const char *pszData  )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}
	return this->m_pFixComm->SetItem(nFixCode,pszData);
}; 

BOOL CBHCommWrapper::SetItem( int nFixCode , COleDateTime aoTime )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}

	return this->m_pFixComm->SetItem(nFixCode,aoTime);
}

BOOL   CBHCommWrapper::CreateAnswer    ( int nFunc , int nWidth )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->CreateAnswer(nFunc,nWidth);
	
};
BOOL   CBHCommWrapper::CreateAnswerMul ( int nFunc , int nWidth )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->CreateAnswerMul(nFunc,nWidth);
}
BOOL   CBHCommWrapper::CreateNextAnswer()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->CreateNextAnswer();
}
void   CBHCommWrapper::CreateEndMak()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return ;
	}

	return this->m_pFixComm->CreateEndMak();
}
//=============================================================
//   设置/得到内部的FixComm指针，可以不实现
//=============================================================
//设置内部的FixComm指针
BOOL CBHCommWrapper::SetFixComm(CFixComm * apFixComm)  
{
	if (apFixComm == NULL)
	{
		return FALSE;
	}

	if (this->m_pFixComm!=NULL)
	{
		ASSERT(FALSE);	
	}

	this->m_pFixComm = apFixComm;

	return TRUE;
}

//得到内部的FixComm指针
CFixComm * CBHCommWrapper::GetFixComm()	 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	
	}

	return this->m_pFixComm;
}

long CBHCommWrapper::GetFunc()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return -1;
	}

	return this->m_pFixComm->GetFunc();
}

ACI * CBHCommWrapper::GetAci()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return NULL;
	}

	return this->m_pFixComm->GetAci();
}


int  CBHCommWrapper::Write(ACI *pAci/* =NULL */)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}

	return this->m_pFixComm->Write(pAci);
}

ACO* CBHCommWrapper::GetAco()
{
	return NULL;
}

BOOL CBHCommWrapper::SetTime( int nFixCode , COleDateTime aoTime )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}

	return this->m_pFixComm->SetItem(nFixCode,aoTime);
}
