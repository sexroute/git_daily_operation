#ifndef __ICommInterFace__
#define __ICommInterFace__

#include <atlstr.h>
#include <ATLComTime.h>
#include <FixCommA/FixCommA.h>
#include <FixComm.h>

#include <additions/BHDateTime.h>


class ACO;

#ifdef _PIDMS

class ACI;

#endif

class ICommInterFace
{
public:
	//=============================================================
	//   �������������� / ��ȡ��������ݣ�����ʵ��
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  )=0;
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  )=0;
	virtual CString Get		         ( int nFixCode , int         nRow=-1  )=0;
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  )=0; // ����FIX����ȡ����Ӧ�Ľ����
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  )=0;
	virtual bool GetItemBuf( int nFixCode, char *pBuf, int nSize ) =0;
	virtual bool SetItemBuf( int nFixCode, char *pBuf, int nSize ) =0;

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   )=0; // ����ĳ�����򸡵������� -- ������
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   )=0; // ����ĳ��������������   -- ����
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   )=0;
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  )=0; // ����ĳ�������ַ������� -- �ַ���
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  )=0;
	
	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1)=0;
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1)=0;
	virtual BOOL   CreateNextAnswer()=0;
	virtual void   CreateEndMak()=0;
	virtual long   GetFunc()=0;
	virtual ACO	*  GetAco()=0;
	virtual ACI	*  GetAci()=0;


	virtual int     Write(ACI *pAci=NULL)=0;  // ��������

	//=============================================================
	//   ����/�õ��ڲ���FixCommָ�룬���Բ�ʵ��
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm)=0;  //�����ڲ���FixCommָ��
	virtual BOOL        SetFixCommResponse(void * apFixComm) = 0;
	virtual void * 		GetFixComm()=0;	 //�õ��ڲ���FixCommָ��
	virtual void *      GetFixCommResponse() = 0; //�õ�Ӧ��FixCommָ��


	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  )=0; // ����ĳ�������ַ������� -- �ַ���

};



#endif