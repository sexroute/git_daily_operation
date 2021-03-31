/*
* Copyright (c) 2010, BH �������
* All rights reserved.
* 
* �ļ����ƣ� CBHCommWrapper.cpp
* ժ    Ҫ�� CFixComm ���װ��
* ��    �ߣ� �Դ���
* �� �� ��1��
* 
*/
// ZdlDbWrapper.cpp : ʵ���ļ�
//
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����


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

// ����FIX����ȡ����Ӧ�Ľ����
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

// ����ĳ�����򸡵������� -- ������
BOOL    CBHCommWrapper::SetItem    ( int nFixCode , double      dValue   ) 
{
	if (this->m_pFixComm==NULL)
	{
		ASSERT(FALSE);

		return FALSE;
	}

	return this->m_pFixComm->SetItem(nFixCode,dValue);
}

// ����ĳ��������������   -- ����
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

// ����ĳ�������ַ������� -- �ַ���
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
//   ����/�õ��ڲ���FixCommָ�룬���Բ�ʵ��
//=============================================================
//�����ڲ���FixCommָ��
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
//�õ��ڲ���FixCommָ��
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

// ����FIX����ȡ����Ӧ�Ľ����
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

// ����ĳ�����򸡵������� -- ������
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

// ����ĳ��������������   -- ����
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

// ����ĳ�������ַ������� -- �ַ���
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
//   ����/�õ��ڲ���FixCommָ�룬���Բ�ʵ��
//=============================================================
//�����ڲ���FixCommָ��
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
//�õ��ڲ���FixCommָ��
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





