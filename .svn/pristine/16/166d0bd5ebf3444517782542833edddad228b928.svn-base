#include "additions/BHDateTime.h"

#include <math.h>

const double CBHDateTime::ONEMILLISECOND  = 1.1574074074074074e-8;

const char * CBHDateTime::DATETIMEFORMAT = "%Y-%m-%d %H:%M:%S";

CBHDateTime::CBHDateTime(void) : 
	COleDateTime()
{
}

CBHDateTime::CBHDateTime( const COleDateTime & dt )
{
	*this = dt;
}

CBHDateTime::CBHDateTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nMillisecond /*= 0*/ )
{
	this->SetDateTime(nYear,nMonth,nDay,nHour,nMin,nSec,nMillisecond);
}
CBHDateTime::~CBHDateTime(void)
{
}

CBHDateTime& CBHDateTime::operator=(const COleDateTime& dt) throw()
{
	this->m_dt = dt.m_dt;
	this->m_status = dt.m_status;

	return *this;
}

BOOL CBHDateTime::VariantTimeToSystemTime (const double adDatetime,
										   /*out*/SYSTEMTIME *apSystemtime)
{
	if (NULL == apSystemtime)
	{
		return FALSE;
	}

	BOOL lbRet = TRUE;

	DWORD lwMillSecond = 0;

	double hours = (adDatetime  - (int) adDatetime) * 24;
	double minutes = (hours - (int)hours) * 60;
	double seconds = (minutes - (int)minutes) * 60;
	double milliseconds = (seconds - (int)seconds) * 1000;

	if (milliseconds < 0.5 || milliseconds > 999.4)
	{
		lwMillSecond = 0;
	}
	else
	{
		lwMillSecond = (WORD) floor(milliseconds + 0.5);
	}

	double dbTempTime = adDatetime;
	
	if(lwMillSecond > 0)
	{
		dbTempTime = adDatetime - CBHDateTime::ONEMILLISECOND*1000 / 2;
	}
	
	if (!::VariantTimeToSystemTime(dbTempTime, apSystemtime))
	{
		return lbRet;
	}

	apSystemtime->wMilliseconds = lwMillSecond;

	return TRUE;
}


BOOL CBHDateTime::SystemTimeToVariantTime( const SYSTEMTIME * apSystemtime,
										  /*out*/double *apDatetime )
{
	if (NULL == apSystemtime || NULL == apDatetime)
	{
		return FALSE;
	}

	SYSTEMTIME st;
	::ZeroMemory(&st, sizeof(SYSTEMTIME));

	st.wYear = apSystemtime->wYear;
	st.wMonth = apSystemtime->wMonth;
	st.wDay = apSystemtime->wDay;
	st.wHour = apSystemtime->wHour;
	st.wMinute = apSystemtime->wMinute;
	st.wSecond = apSystemtime->wSecond;

	double dt;
	if(!::SystemTimeToVariantTime(&st, &dt))
	{
		return FALSE;
	}

	*apDatetime = dt +  (CBHDateTime::ONEMILLISECOND * apSystemtime->wMilliseconds);

	return TRUE;
}

CBHDateTime CBHDateTime::GetCurrentTime()
{
	return COleDateTime::GetCurrentTime();
}

CBHDateTime CBHDateTime::GetCurrentTimeEx()
{
	CBHDateTime dt;

	SYSTEMTIME st;

	::GetLocalTime(&st);

	BOOL result = CBHDateTime::SystemTimeToVariantTime(&st, &dt.m_dt);

	//	m_nMiniSecond = st.wMilliseconds;

	if (result)
	{
		dt.m_status = valid;
	}
	else
	{
		dt.m_status = invalid;
	}

	return dt;
}

int CBHDateTime::SetDateTime( int nYear, int nMonth, int nDay,
							 int nHour, int nMin, int nSec,
							 int nMillisecond /*= 0*/ ) throw()
{
	SYSTEMTIME st;
	::ZeroMemory(&st, sizeof(SYSTEMTIME));

	st.wYear = WORD(nYear);
	st.wMonth = WORD(nMonth);
	st.wDay = WORD(nDay);
	st.wHour = WORD(nHour);
	st.wMinute = WORD(nMin);
	st.wSecond = WORD(nSec);
	st.wMilliseconds = WORD(nMillisecond);

	BOOL lbRet = CBHDateTime::SystemTimeToVariantTime(&st, &m_dt);

	if (lbRet)
	{
		m_status = valid;
	}
	else
	{
		m_status = invalid;
	}

	return m_status;
}

BOOL CBHDateTime::ParseDateTime( LPCTSTR lpszDate,
								DWORD dwFlags /*= 0*/,
								LCID lcid /*= LANG_USER_DEFAULT*/ ) throw( )
{
	if (NULL == lpszDate)
	{
		return FALSE;
	}

	CString lsDateTime = lpszDate;

	int lnMillisecond = 0;

	int pos = lsDateTime.ReverseFind('.');
	if (pos >= 0)
	{
		lnMillisecond = ::atoi(lsDateTime.Mid(pos+1, 3));

		lsDateTime = lsDateTime.Left(pos);
	}

	if(!COleDateTime::ParseDateTime(lsDateTime, dwFlags, lcid))
	{
		return FALSE;
	}

	if (lnMillisecond)
	{
		SetMillisecond(lnMillisecond);
	}

	return TRUE;
}

BOOL CBHDateTime::GetAsSystemTime(SYSTEMTIME& sysTime) const throw()
{
	if (GetStatus() != valid)
	{
		return false;
	}

	return  CBHDateTime::VariantTimeToSystemTime(m_dt, &sysTime);
}

int CBHDateTime::GetMillisecond() const throw( )
{
	SYSTEMTIME st;
	return GetAsSystemTime(st) ? st.wMilliseconds : COleDateTime::error;
}

BOOL CBHDateTime::SetMillisecond(int nMilliseconds) throw( )
{
	if (nMilliseconds < 0 || nMilliseconds > 999)
	{

		return FALSE;
	}
	
	SYSTEMTIME st;
	if (!CBHDateTime::GetAsSystemTime(st))
	{
		return FALSE;
	}

	st.wMilliseconds = nMilliseconds;

	BOOL lbRet = CBHDateTime::SystemTimeToVariantTime(&st, &m_dt);

	if (lbRet)
	{
		m_status = valid;
	}
	else
	{
		m_status = invalid;
	}

	return lbRet;
}

