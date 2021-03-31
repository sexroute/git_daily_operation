// CustomDB.h: interface for the CCustomDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMDB_H__9084DBDF_E8F6_415E_951C_DF1C739345A5__INCLUDED_)
#define AFX_CUSTOMDB_H__9084DBDF_E8F6_415E_951C_DF1C739345A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "simado.h"
//class AFX_CLASS_EXPORT CCustomDB : public CSimADO
class AFX_EXT_CLASS CCustomDB : public CSimADO
{
public:
	BOOL	Login( const char *pszSvr , const char *pszID, const char *pszPwd , const char *pszDB);
	BOOL	Logout();
	BOOL	ExsitsTable( const char *pszDB , const char *pszTable);	
	//BOOL	NewPass    ( const char *pszPwd,const char *pszID=NULL , const char *pszOldPwd=NULL);
	//BOOL	GetNode    ( CString &sNet , CString &sAddr);
	CCustomDB();
	virtual ~CCustomDB();
    //CString  GetEncodePwd();
	//CString  GetPwd();
	//CString  GetID();
	//CString  GetOperYYB();
	//void     SetOperYYB( const char *pszYYB);
	void	 SetStandUser( bool bStand );
public:
	bool IsLogin();
    void Close();	

	int     m_nModule  ;
	int     m_nLoginID ;
	CString m_sID      ; // 柜员ID<含柜员所属营业部>
	CString m_sIDPwd   ; // 柜员密码
	CString m_sIDName  ; // 柜员姓名
	CString m_sYYBBH   ; // 发生营业部编号
	CString m_sYYBMC   ; // 营业部名称<柜员营业部名称>
	
	CString m_sNode    ; // 本机网卡地址
	CString m_sPCName  ; // 本机名称
	CString m_sProgName; // 程序名称
    CString m_sIniFile;
    int  mCode					;
private:
    bool    m_bLogin	;
	bool	m_bStandUser;
protected:
	void GetLoginInfo(CString &m_sDbUser, CString &m_sDbPwd);
};
#endif // !defined(AFX_CUSTOMDB_H__9084DBDF_E8F6_415E_951C_DF1C739345A5__INCLUDED_)
