#include "stdafx.h"
#ifndef _M_BHKD_SERVER_SIDE_
#include ".\fakecomm.h"

#define  CHECK_REQUEST_STATUS {if(this->m_oRequest.size()==0)\
							  {\
								return FALSE;\
							  }\
							  if ((this->m_noRequestDataIndex>=this->m_oRequest.size()))\
							  {\
								  return FALSE;\
							  };\
							  if (this->m_noRequestDataIndex<0)\
							  {\
							      return FALSE;\
							  }};

#define  CHECK_RESPONSE_STATUS {if(this->m_oResponse.size()==0)\
							  {\
							  return FALSE;\
							  }\
							  if ((this->m_noResponseDataIndex>=this->m_oResponse.size()))\
							  {\
							  return FALSE;\
							  };\
							  if (this->m_noResponseDataIndex<0)\
							  {\
							  return FALSE;\
							  }};

#define  CHECK_REQUEST_STATUS_DATE {if(this->m_oRequest.size()==0)\
							  {\
							    return CBHDateTime();\
							  }\
							  if ((this->m_noRequestDataIndex>=this->m_oRequest.size()))\
							  {\
							  return CBHDateTime();\
							  };\
							  if (this->m_noRequestDataIndex<0)\
							  {\
							  return CBHDateTime();\
							  }};

#define  CHECK_RESPONSE_STATUS_DATE {if(this->m_oResponse.size()==0)\
							  {\
							  return CBHDateTime();\
							  }\
							  if ((this->m_noResponseDataIndex>=this->m_oResponse.size()))\
							  {\
							  return CBHDateTime();\
							  };\
							  if (this->m_noResponseDataIndex<0)\
							  {\
							  return CBHDateTime();\
							  }};

#define  CHECK_REQUEST_STATUS_CSTRING {if(this->m_oRequest.size()==0)\
									{\
									return CString();\
									}\
									if ((this->m_noRequestDataIndex>=this->m_oRequest.size()))\
									{\
									return CString();\
									};\
									if (this->m_noRequestDataIndex<0)\
									{\
									return CString();\
									}};

#define  CHECK_RESPONSE_STATUS_CSTRING {if(this->m_oResponse.size()==0)\
							  {\
							  return CString();\
							  }\
							  if ((this->m_noResponseDataIndex>=this->m_oResponse.size()))\
							  {\
							  return CString();\
							  };\
							  if (this->m_noResponseDataIndex<0)\
							  {\
							  return CString();\
							  }};

CFakeComm::CFakeComm(void)
{
	this->m_noRequestDataIndex = -1;
	this->m_noResponseDataIndex = -1;
	this->m_nFuncCode = 0;
	this->m_nCommMode = REQUEST;
	this->m_enFixCommType = EN_FIXCOMM_TYPE_FAKE;
}

CFakeComm::~CFakeComm(void)
{
	this->ResetRequestData();
	this->ReestResponseData();
}



bool CFakeComm::More()
{
	BEGIN_ERROR_HANDLE;
	
	Do(this);
	
	END_ERROR_HANDLE;
	
	if ((this->m_noResponseDataIndex+1)<= this->m_oResponse.size() && this->m_oResponse.size()>0)
	{
		this->m_noResponseDataIndex++;
		return true;
	}

	return false;
}

void CFakeComm::CreateHead( int nFunc , int nWidth/*=-1*/ )
{
	this->m_nCommMode = REQUEST;
	this->ResetRequestData();
	this->m_noRequestDataIndex++;
	FixCommRow loRow;
	this->m_oRequest.push_back(loRow);
}

void CFakeComm::ResetRequestData()
{
	this->m_oRequest.clear();
	this->m_noRequestDataIndex = -1;
	return;
}

BOOL CFakeComm::CreateAnswer( int nFunc , int nWidth /*=-1*/ )
{
	this->m_nCommMode = RESPONSE;
	this->ReestResponseData();
	return TRUE;
}

BOOL CFakeComm::CreateAnswerMul( int nFunc , int nWidth /*=-1*/ )
{
	this->m_nCommMode = RESPONSE;
	this->ReestResponseData();
	return TRUE;
}

BOOL CFakeComm::CreateNextAnswer()
{
	this->m_nCommMode = RESPONSE;
	this->m_noResponseDataIndex++;
	FixCommRow loRow;
	this->m_oResponse.push_back(loRow);
	return TRUE;
}

long CFakeComm::GetFunc()
{
	return this->m_nFuncCode;
}



void CFakeComm::ReestResponseData()
{
	this->m_nCommMode = RESPONSE;
	this->m_oResponse.clear();
	this->m_noResponseDataIndex = -1;
}

ACO	* CFakeComm::GetAco()
{
	return NULL;
}

ACI	* CFakeComm::GetAci()
{
	return NULL;

}

int CFakeComm::Write( ACI *pAci/*=NULL*/ )
{
	return NULL;

}

BOOL CFakeComm::SetFixComm( void * apFixComm )
{
	this->m_pFixComReqeust = apFixComm;
	
	return (this->m_pFixComReqeust!= NULL);
}

BOOL CFakeComm::SetFixCommResponse( void * apFixComm )
{
	this->m_pFixComResponse = apFixComm;

	return (this->m_pFixComResponse!= NULL);
}

void * CFakeComm::GetFixComm()
{
	return this->m_pFixComReqeust;
}

void * CFakeComm::GetFixCommResponse()
{
	return this->m_pFixComResponse;
}

BOOL CFakeComm::SetTime( int nFixCode , CBHDateTime aoTime )
{
	
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];
		lrefRow.m_oDateTimeValues[nFixCode] = aoTime;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		lrefRow.m_oDateTimeValues[nFixCode] = aoTime;
	}

	return TRUE;
}

EN_FIXCOMM_TYPE CFakeComm::GetFixCommType()
{
	return this->m_enFixCommType;
}

BOOL CFakeComm::SetItem( int nFixCode , double dValue )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];
		lrefRow.m_oDblValues[nFixCode] = dValue;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		lrefRow.m_oDblValues[nFixCode] = dValue;
	}

	return TRUE;
}

BOOL CFakeComm::SetItem( int nFixCode , long nValue )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];
		lrefRow.m_oIntValues[nFixCode] = nValue;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		lrefRow.m_oIntValues[nFixCode] = nValue;
	}

	return TRUE;
}

BOOL CFakeComm::SetItem( int nFixCode , int nValue )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];
		lrefRow.m_oIntValues[nFixCode] = nValue;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		lrefRow.m_oIntValues[nFixCode] = nValue;
	}

	return TRUE;
}

BOOL CFakeComm::SetItem( int nFixCode , const char *pszData )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];
		CString lstrData;
		if (NULL != pszData)
		{
			lstrData = pszData;
			
		}
		lrefRow.m_oCStringValues[nFixCode] = lstrData;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		CString lstrData;
		if (NULL != pszData)
		{
			lstrData = pszData;			
		}
		lrefRow.m_oCStringValues[nFixCode] = lstrData;
	}

	return TRUE;
}

BOOL CFakeComm::SetItem( int nFixCode , CBHDateTime aoTime )
{
	return this->SetTime(nFixCode,aoTime);
}

bool CFakeComm::SetItemBuf( int nFixCode, char *pBuf, int nSize )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];		
		if ((nSize>0 )&& (pBuf!= NULL))
		{
			_COMM_BUFFER loBuffer;
			lrefRow.m_oBufferValues[nFixCode] = loBuffer;
			
			_COMM_BUFFER & lrefBuffer = lrefRow.m_oBufferValues[nFixCode];
			lrefBuffer.resize(nSize);
			BYTE * lpBuffer = &lrefBuffer.front();
			memcpy(lpBuffer,pBuf,nSize);

		}

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		if ((nSize>0 )&& (pBuf!= NULL))
		{
			_COMM_BUFFER loBuffer;
			lrefRow.m_oBufferValues[nFixCode] = loBuffer;

			_COMM_BUFFER & lrefBuffer = lrefRow.m_oBufferValues[nFixCode];
			lrefBuffer.resize(nSize);
			BYTE * lpBuffer = &lrefBuffer.front();
			memcpy(lpBuffer,pBuf,nSize);
		}
	}

	return TRUE;
}

bool CFakeComm::GetItemBuf( int nFixCode, char *pBuf, int nSize )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];		
		if ((nSize>0 )&& (pBuf!= NULL))
		{
			if (lrefRow.m_oBufferValues.find(nFixCode) == lrefRow.m_oBufferValues.end())
			{
				return false;
			}
			

			_COMM_BUFFER & lrefBuffer = lrefRow.m_oBufferValues[nFixCode];
			BYTE * lpBuffer = &lrefBuffer.front();
			
			int lnActualSize = nSize;
			if (lnActualSize>lrefBuffer.size())
			{
				lnActualSize = lrefBuffer.size();
			}
			
			memcpy(pBuf,lpBuffer,lnActualSize);
		}

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];
		if ((nSize>0 )&& (pBuf!= NULL))
		{
			if (lrefRow.m_oBufferValues.find(nFixCode) == lrefRow.m_oBufferValues.end())
			{
				return false;
			}


			_COMM_BUFFER & lrefBuffer = lrefRow.m_oBufferValues[nFixCode];
			BYTE * lpBuffer = &lrefBuffer.front();

			int lnActualSize = nSize;
			if (lnActualSize>lrefBuffer.size())
			{
				lnActualSize = lrefBuffer.size();
			}

			memcpy(pBuf,lpBuffer,lnActualSize);
		}
	}

	return TRUE;
}

CBHDateTime CFakeComm::GetTime( int nFixCode , int nRow/*=-1 */ )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS_DATE;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];

		CBHDateTime loDateTime;

		std::map<int,CBHDateTime>::iterator lpIt = lrefRow.m_oDateTimeValues.find(nFixCode);
		
		if (lpIt!= lrefRow.m_oDateTimeValues.end())
		{
			loDateTime = lpIt->second;
		}

		return loDateTime;

	}else
	{
		CHECK_RESPONSE_STATUS_DATE;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];

		CBHDateTime loDateTime;

		std::map<int,CBHDateTime>::iterator lpIt = lrefRow.m_oDateTimeValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oDateTimeValues.end())
		{
			loDateTime = lpIt->second;
		}

		return loDateTime;
	}

	return CBHDateTime();
}

CString CFakeComm::GetItem( int nFixCode , int nRow/*=-1 */ )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS_CSTRING;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];

		CString lstrDateTime;

		std::map<int,CString>::iterator lpIt = lrefRow.m_oCStringValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oCStringValues.end())
		{
			lstrDateTime = lpIt->second;
		}

		return lstrDateTime;

	}else
	{
		CHECK_RESPONSE_STATUS_CSTRING;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];

		CString lstrDateTime;

		std::map<int,CString>::iterator lpIt = lrefRow.m_oCStringValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oCStringValues.end())
		{
			lstrDateTime = lpIt->second;
		}

		return lstrDateTime;
	}

	return CString();
}

CString CFakeComm::Get( int nFixCode , int nRow/*=-1 */ )
{
	return this->GetItem(nFixCode,nRow);
}

long CFakeComm::GetLong( int nFixCode , int nRow/*=-1 */ )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];

		int lnValue;

		std::map<int,int>::iterator lpIt = lrefRow.m_oIntValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oIntValues.end())
		{
			lnValue = lpIt->second;
		}

		return lnValue;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];

		int lnValue;

		std::map<int,int>::iterator lpIt = lrefRow.m_oIntValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oIntValues.end())
		{
			lnValue = lpIt->second;
		}

		return lnValue;
	}

	return 0;
}

double CFakeComm::GetDouble( int nFixCode , int nRow/*=-1 */ )
{
	if (this->m_nCommMode ==  REQUEST)
	{
		CHECK_REQUEST_STATUS;

		FixCommRow & lrefRow = this->m_oRequest[this->m_noRequestDataIndex];

		double ldblValue;

		std::map<int,double>::iterator lpIt = lrefRow.m_oDblValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oDblValues.end())
		{
			ldblValue = lpIt->second;
		}

		return ldblValue;

	}else
	{
		CHECK_RESPONSE_STATUS;

		FixCommRow & lrefRow = this->m_oResponse[this->m_noResponseDataIndex];

		double ldblValue;

		std::map<int,double>::iterator lpIt = lrefRow.m_oDblValues.find(nFixCode);

		if (lpIt!= lrefRow.m_oDblValues.end())
		{
			ldblValue = lpIt->second;
		}

		return ldblValue;
	}

	return 0;
}

void CFakeComm::CreateEndMak()
{

}


 BOOL CreateFakeFixComm(CFakeComm ** appFixComm)
{
	if (NULL == appFixComm)
	{
		return FALSE;
	}

	* appFixComm = new CFakeComm();

	return (* appFixComm!= NULL);
}

 BOOL DeleteFakeFixComm(CFakeComm * appFixComm)
{
	if (NULL == appFixComm)
	{
		return FALSE;
	}

	delete appFixComm;

	return TRUE;
}
#endif
