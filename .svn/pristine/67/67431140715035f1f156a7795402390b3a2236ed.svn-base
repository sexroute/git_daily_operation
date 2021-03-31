#if !defined(AFX_ACITRAN_H__INCLUDED_)
#define AFX_ACITRAN_H__INCLUDED_

typedef struct 
{

	char	*m_pData			;
	long	 m_nDataSize		;
	long	 m_nRouteID			;
	long	 m_nFunc			;		
	long	 m_nCode		    ;
	long	 m_nQuery			;
	long	 m_bIsSelf			;	//系统内部请求
	bool	 m_bReqNext			;	//
	CString	 m_sErrMsg			;
	void	*m_pCommAns			;
	void	*m_pAci				;
	void	*m_pAciSvr			;
	void	*m_pParam			;
	void	*m_pVoid			;
	

}TAciTran;

typedef struct
{
	long	 m_nRouteSndID		; //
	long	 m_nRouteID			; //##
	long	 m_nFunc			;
	long	 m_nSec				;

	BOOL	 m_bIsLast			;
	BOOL	 m_bIsMulti			;
	long	 m_nRow				;
	long	 m_nQuery			;
	BOOL	 m_bNeedQuery		;

	CString	 m_sNode			;
	CString	 m_sNote			;
	void	*m_pFuncParam		;
	void	*m_pFunc			;
	void	*m_pVoid			;

}TAciParam;
#define		NOTE_TYPE_COMM			0
#define		NOTE_TYPE_COMMON		0
#define		NOTE_TYPE_WARN			1
#define		NOTE_TYPE_STOP			2
#define		NOTE_TYPE_CUSTOM		3
#define		NOTE_TYPE_CUSTOM_WARN	4
#define		NOTE_TYPE_CUSTOM_STOP	5
#define		NOTE_TYPE_DEBUG_REC		6
#define		NOTE_TYPE_DEBUG_SEND	7

#define		DO_TYPE_ACITRAN_SEND	10
#define		DO_TYPE_ACITRAN_RECV	11

#define		INIT_TYPE_GETNEW_RECV_COMM	20
#define		INIT_TYPE_DEL_RECV_COMM		21

#define		CALLBACK_TYPE_GETROUTEID			20
#define		CALLBACK_TYPE_SETROUTEID			21
#define		CALLBACK_TYPE_ADDROUTEID			22
#define		CALLBACK_TYPE_DELROUTEID			23
#define		CALLBACK_TYPE_ADDNOTE				30
#define		CALLBACK_TYPE_SETPARAM				31
#define		CALLBACK_TYPE_SETNEXT				32
#define		CALLBACK_TYPE_GETNEXTROUTEID		33

#endif 
