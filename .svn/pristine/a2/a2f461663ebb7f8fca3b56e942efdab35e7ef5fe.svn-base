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
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类


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

CBHDateTime CBHCommWrapper::GetTime   ( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return CBHDateTime();
	}

	CString strTime = this->m_pFixComm->GetItem(nFixCode);
	
	if(strTime.Trim().GetLength()>=12)
	{
		CBHDateTime dtTime;

		dtTime.ParseDateTime(strTime);

		return dtTime;
	}
	else
	{
		return CBHDateTime(this->m_pFixComm->GetTime(nFixCode,nRow));
	}
	
	
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
BOOL CBHCommWrapper::SetFixComm(void * apFixComm)  
{
	if (apFixComm == NULL)
	{
		return FALSE;
	}

	if (this->m_pFixComm!=NULL)
	{
		ASSERT(FALSE);	
	}

	this->m_pFixComm = (CFixComm *)apFixComm;

	return TRUE;
}

BOOL CBHCommWrapper::SetFixCommResponse(void * apFixComm)
{
	return this->SetFixComm(apFixComm);
}
//得到内部的FixComm指针
void * CBHCommWrapper::GetFixComm()	 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	
	}

	return this->m_pFixComm;
}

void * CBHCommWrapper::GetFixCommResponse()
{

	return this->GetFixComm();
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

BOOL CBHCommWrapper::SetTime( int nFixCode , CBHDateTime aoTime )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}

	return this->m_pFixComm->SetItem(nFixCode,aoTime);
}

BOOL CBHCommWrapper::SetItem( int nFixCode , CBHDateTime aoTime )
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}

	return this->m_pFixComm->SetItem(nFixCode,aoTime.Format("%Y-%m-%d %H:%M:%S"));
}




CBHCommWrapperA::CBHCommWrapperA()
{
	this->m_pFixComm = NULL;
	this->m_pFixCommResponse = NULL;
}

CBHCommWrapperA::~CBHCommWrapperA()
{
	this->m_pFixComm = NULL;
	this->m_pFixCommResponse = NULL;
}

double	CBHCommWrapperA::GetDouble( int nFixCode , int         nRow)
{

	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return 0;
	}	

	return this->m_pFixComm->GetDouble(nFixCode);
};
long	CBHCommWrapperA::GetLong( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return 0;
	}

	return this->m_pFixComm->GetInt(nFixCode);
};

CString CBHCommWrapperA::Get( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return _T("");
	}

	return this->m_pFixComm->GetString(nFixCode).c_str();
}

// 根据FIX代码取出相应的结果。
CString CBHCommWrapperA::GetItem( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return _T("");
	}

	return this->m_pFixComm->GetString(nFixCode).c_str();
};

CBHDateTime CBHCommWrapperA::GetTime   ( int nFixCode , int         nRow)
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return CBHDateTime();
	}

	CString strTime = this->m_pFixComm->GetString(nFixCode).c_str();
	if(strTime.Trim().GetLength()>=12)
	{
		CBHDateTime dtTime;

		dtTime.ParseDateTime(strTime);

		return dtTime;
	}
	else
	{
		struct tm tmVal;
		tmVal = this->m_pFixComm->GetDateTime( nFixCode );
		CBHDateTime dtVal(tmVal.tm_year, tmVal.tm_mon, tmVal.tm_mday, tmVal.tm_hour, tmVal.tm_min, tmVal.tm_sec);


		return dtVal;
	}
	
}

bool CBHCommWrapperA::GetItemBuf( int nFixCode, char *pBuf, int nSize ) 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

   bool lbRet = m_pFixComm->GetBytesEx(nFixCode,pBuf,nSize);

   return lbRet;	

}
bool CBHCommWrapperA::SetItemBuf( int nFixCode, char *pBuf, int nSize ) 
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}



	this->m_pFixCommResponse->SetBytes( nFixCode, pBuf, nSize);
	
	return true;
}

// 设置某请求域浮点性数据 -- 浮点性
BOOL    CBHCommWrapperA::SetItem    ( int nFixCode , double      dValue   ) 
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}
	
	
	this->m_pFixCommResponse->SetDouble(nFixCode,dValue);

	return TRUE;
}

// 设置某请求域整性数据   -- 整性
BOOL    CBHCommWrapperA::SetItem    ( int nFixCode , long        nValue   )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	 this->m_pFixCommResponse->SetInt(nFixCode,nValue);

	 return  TRUE;
}
BOOL    CBHCommWrapperA::SetItem( int nFixCode , int         nValue   )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	 this->m_pFixCommResponse->SetInt(nFixCode,nValue);

	 return TRUE;
}

// 设置某请求域字符串数据 -- 字符串
BOOL    CBHCommWrapperA::SetItem( int nFixCode , const char *pszData  )
{
	if (this->m_pFixCommResponse==NULL || NULL == pszData)
	{
		ASSERT(FALSE);

		return FALSE;
	}
	 this->m_pFixCommResponse->SetString(nFixCode,pszData);

	 return TRUE;
}; 

BOOL   CBHCommWrapperA::CreateAnswer    ( int nFunc , int nWidth )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	this->m_pFixCommResponse->Clear();
	this->m_pFixCommResponse->InsertRow();

	return TRUE;

};
BOOL   CBHCommWrapperA::CreateAnswerMul ( int nFunc , int nWidth )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	this->m_pFixCommResponse->Clear();
	this->m_pFixCommResponse->InsertRow();

	return TRUE;
}
BOOL   CBHCommWrapperA::CreateNextAnswer()
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	this->m_pFixCommResponse->InsertRow();

	return TRUE;
}
void   CBHCommWrapperA::CreateEndMak()
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return ;
	}

	this->m_pFixCommResponse->SetRetCode(1);

	return ;
}
//=============================================================
//   设置/得到内部的FixComm指针，可以不实现
//=============================================================
//设置内部的FixComm指针
BOOL CBHCommWrapperA::SetFixComm(void * apFixComm)  
{
	if (apFixComm == NULL)
	{
		return FALSE;
	}

	if (this->m_pFixComm!=NULL)
	{
		ASSERT(FALSE);	
	}

	this->m_pFixComm = (ACO *)apFixComm;

	return TRUE;
}

BOOL CBHCommWrapperA::SetFixCommResponse(void * apFixComm)
{
	if (apFixComm == NULL)
	{
		return FALSE;
	}

	if (this->m_pFixCommResponse!=NULL)
	{
		ASSERT(FALSE);	
	}

	this->m_pFixCommResponse = (ACO *)apFixComm;

	return TRUE;
}
//得到内部的FixComm指针
void * CBHCommWrapperA::GetFixComm()	 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	
	}

	return this->m_pFixComm;
}
void * CBHCommWrapperA::GetFixCommResponse()
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);	
	}

	return this->m_pFixCommResponse;
}

long CBHCommWrapperA::GetFunc()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return -1;
	}

	return  this->m_pFixComm->GetServiceCode();;
}

ACI * CBHCommWrapperA::GetAci()
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);	

		return NULL;
	}	
	return NULL;
}


int  CBHCommWrapperA::Write(ACI *pAci/* =NULL */)
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);	

		return 0;
	}
	
	return 1;
}

ACO* CBHCommWrapperA::GetAco()
{

	return NULL;
}

BOOL CBHCommWrapperA::SetItem( int nFixCode , CBHDateTime aoTime )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}
	return this->SetTime(nFixCode,aoTime);
}

BOOL CBHCommWrapperA::SetTime( int nFixCode , CBHDateTime aoTime )
{
	if (this->m_pFixCommResponse==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}


	this->m_pFixCommResponse->SetString(nFixCode,aoTime.Format("%Y-%m-%d %H:%M:%S"));

	return TRUE;
	
}





