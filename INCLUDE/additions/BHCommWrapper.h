#ifndef _BHCommWrapper_
#define _BHCommWrapper_
#include "ICommInterFace.h"
#include "aco/ACO.h"

class CBHCommWrapper : public ICommInterFace
{
public:
	//=============================================================
	//   �������������� / ��ȡ��������ݣ�����ʵ��
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  );
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  );
	virtual CString Get		         ( int nFixCode , int         nRow=-1  );
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  ); // ����FIX����ȡ����Ӧ�Ľ����
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  );

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   ); // ����ĳ�����򸡵������� -- ������
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   ); // ����ĳ��������������   -- ����
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   );
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  ); // ����ĳ�������ַ������� -- �ַ���
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  );


	virtual bool	GetItemBuf( int nFixCode, char *pBuf, int nSize ) ;
	virtual bool	SetItemBuf( int nFixCode, char *pBuf, int nSize ) ;

	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateNextAnswer();
	virtual void   CreateEndMak();

	virtual long   GetFunc();
	virtual ACI	*  GetAci();
	virtual ACO	*  GetAco();
	virtual int    Write                                  (ACI *pAci=NULL);  // ��������

	//=============================================================
	//   ����/�õ��ڲ���FixCommָ�룬���Բ�ʵ��
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm);  //�����ڲ���FixCommָ��
	virtual BOOL        SetFixCommResponse(void * apFixComm);
	virtual void * 		GetFixComm();
	virtual void *      GetFixCommResponse();

	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  ); // ����ĳ�������ַ������� -- �ַ���
	CBHCommWrapper();
	virtual ~CBHCommWrapper();
private:
	CFixComm * m_pFixComm;
	int m_nFixCommType;
};

class CBHCommWrapperA : public ICommInterFace
{
public:
	//=============================================================
	//   �������������� / ��ȡ��������ݣ�����ʵ��
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  );
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  );
	virtual CString Get		         ( int nFixCode , int         nRow=-1  );
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  ); // ����FIX����ȡ����Ӧ�Ľ����
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  );

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   ); // ����ĳ�����򸡵������� -- ������
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   ); // ����ĳ��������������   -- ����
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   );
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  ); // ����ĳ�������ַ������� -- �ַ���
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  );

	virtual bool	GetItemBuf( int nFixCode, char *pBuf, int nSize ) ;
	virtual bool	SetItemBuf( int nFixCode, char *pBuf, int nSize ) ;

	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateNextAnswer();
	virtual void   CreateEndMak();

	virtual long   GetFunc();
	virtual ACI	*  GetAci();
	virtual ACO	*  GetAco();
	virtual int    Write                                  (ACI *pAci=NULL);  // ��������

	//=============================================================
	//   ����/�õ��ڲ���FixCommָ�룬���Բ�ʵ��
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm);  //�����ڲ���FixCommָ��
	virtual BOOL        SetFixCommResponse(void * apFixComm);
	virtual void * 		GetFixComm();	
	virtual void *      GetFixCommResponse();
    
	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  ); // ����ĳ�������ַ������� -- �ַ���

	CBHCommWrapperA();
	virtual ~CBHCommWrapperA();
private:
	ACO * m_pFixComm;
	ACO * m_pFixCommResponse;
	int m_nFixCommType;
};

#endif
