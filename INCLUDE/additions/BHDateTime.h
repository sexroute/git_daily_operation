#pragma once

#include "atlcomtime.h"

#define G_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

class CBHDateTime :
	public COleDateTime
{
public:
	CBHDateTime(void);
	CBHDateTime(const COleDateTime & dt);
	CBHDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nMillisecond = 0);
	
	~CBHDateTime(void);

public:
	CBHDateTime& CBHDateTime::operator=(const COleDateTime& dt) throw();

public:
	static CBHDateTime GetCurrentTime() throw();
	static CBHDateTime GetCurrentTimeEx() throw();

public:
	CString Format(DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) const;
	CString Format(LPCTSTR lpszFormat, bool bAppendMillsec = true) const;
	BOOL ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT) throw( );

public:
	BOOL SetMillisecond(int nMilliseconds) throw( );
	int GetMillisecond( ) const throw( );

	int SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nMillisecond = 0) throw();
	BOOL GetAsSystemTime(/*out*/SYSTEMTIME& sysTime) const throw();

public:
	static const double ONEMILLISECOND;
	static const char * DATETIMEFORMAT;

public:
	/*
	 PRB: VariantTimeToSystemTime and SystemTimeToVariantTime Functions Ignore Milliseconds Value of System Time
	   http://support.microsoft.com/kb/297463
	*/
	static BOOL VariantTimeToSystemTime (const double adDatetime, /*out*/SYSTEMTIME *apSystemtime);
	static BOOL SystemTimeToVariantTime (const SYSTEMTIME *apSystemtime, /*out*/double *apDatetime);

};


inline CString CBHDateTime::Format(DWORD dwFlags /* = 0 */, LCID lcid /* = LANG_USER_DEFAULT */) const
{
	if (GetStatus() == null)
		return _T("");

	CString lsDatetime;

	if (GetStatus() == invalid)
	{
		if(lsDatetime.LoadString(ATL_IDS_DATETIME_INVALID))
			return lsDatetime;

		return szInvalidDateTime;
	}

	SYSTEMTIME st;
	
	if(CBHDateTime::GetAsSystemTime(st))
	{
		COleDateTime loDateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

		lsDatetime = loDateTime.Format(G_TIME_FORMAT);

		lsDatetime.AppendFormat(".%03d", st.wMilliseconds);
		
	}

	return lsDatetime;
}

inline CString CBHDateTime::Format( LPCTSTR lpszFormat, bool bAppendMillsec/* = true*/) const
{
	if (GetStatus() == null)
		return _T("");

	CString lsDatetime;

	if (GetStatus() == invalid)
	{
		if(lsDatetime.LoadString(ATL_IDS_DATETIME_INVALID))
			return lsDatetime;

		return szInvalidDateTime;
	}

	
	SYSTEMTIME st;

	if(CBHDateTime::GetAsSystemTime(st))
	{
		COleDateTime loDateTime(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

		lsDatetime = loDateTime.Format(lpszFormat);

		if (bAppendMillsec)
		{
			lsDatetime.AppendFormat(".%03d", st.wMilliseconds);
		}

	}
	
	return lsDatetime;
}