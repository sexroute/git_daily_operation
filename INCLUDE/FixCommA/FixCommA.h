#ifndef _FIXCOMMA_H_BH_
#define _FIXCOMMA_H_BH_

#include <Windows.h>
#include <atlcomtime.h>
#include <string>
using namespace std;
#ifdef FIXCOMMA_EXPORTS
#define EXPORT_CFIXCOMMA __declspec(dllexport)
#else
#define EXPORT_CFIXCOMMA __declspec(dllimport)
#ifdef _DEBUG
#pragma  comment(lib,"FixCommAD.lib")
#else
#pragma  comment(lib,"FixCommA.lib")
#endif
#endif
class ACOBase;
class EXPORT_CFIXCOMMA CFixCommA
{
public:
	CFixCommA();
	virtual ~CFixCommA();
	void SetCompressLevel(int level);
	bool SetItem( int nFixCode , COleDateTime tValue   );
	COleDateTime GetTime( int nFixCode , int  nRow=-1  );
	bool    GetItemBuf( int nFixCode, unsigned char *pBuf, int nSize );
	bool    GetItemBuf( int nFixCode, char *pBuf, int nSize );
	bool	HaveItem( int nFixCode, int  nRow=-1  );
	
	///����ֵtrue���������false���޽��������һ�ε��ó���falseʱ��Ҳ�������м���������⣩��
	bool	More();

	///��һ�ε���ʱ���������true��ʾ�����м��ͨѶ�ɹ������ܴ����޽���������Ի�ȡ���ǰ���������GetCount�ж��Ƿ��н����
	///��������ʱ������false��ʾû�н��
	bool    MoreEx();
	
	void	SetErrMsg( const char *pszMsg);

	string  GetRetNote();
	long	GetRetCode();
	long	GetFunc();
	void	CreateEndMak();
	//=============================================================
	//   �������̺���      < ��Ҫ���ڿͻ��˱�� >
	//=============================================================
	bool    Link( const char *pAddr_, const int &iPort_ );      // ���ӷ�����/Զ�̷�����
    string GetLastErrMsg();                                    // ȡ��ϵͳ������Ϣ
	long    GetCode ();                                         // ȡ��ϵͳ�����룬����ͨѶ���� ҵ�����. ���������㡣
	void    CreateHead ( int nFunc);                            // �����������룬�������(��ѡ��������÷���ʱ�����ж������)
	bool    Close();                                            // �ر��������������
	long    GetCount();                                         // ȡ�����ؼ�¼��
	bool    Empty();                                            // ��ս������ݻ���
	void    SetTimeOut( long nMaxSec );                         // ����ͨѶ��ʱ����	


	//=============================================================
	//   �������̺���      < ֻ���ڷ���˱�� >
	//=============================================================
	bool   CreateAnswer    ( int nFunc , int nWidth =-1);
	bool   CreateAnswerMul ( int nFunc , int nWidth =-1);
	bool   CreateNextAnswer();

	//=============================================================
	//   �������������� / ��ȡ���������
	//=============================================================
	double	GetDouble  ( int nFixCode , int         nRow=-1  );
	float	CFixCommA::GetFloat(const int &nFixCode ,const int &nRow = -1 );
	long	GetLong    ( int nFixCode , int         nRow=-1  );
	string  Get   ( int nFixCode , int         nRow=-1  );
	string  GetItem    ( int nFixCode , int         nRow=-1  ); // ����FIX����ȡ����Ӧ�Ľ����
	// ȡ�����ݺ��� , ���nRow=-1,���ص�ǰ�н������

	bool    SetItem( int nFixCode , double      dValue   ); // ����ĳ�����򸡵������� -- ������
	bool    SetItem( int nFixCode , long        nValue   ); // ����ĳ��������������   -- ����
	bool    SetItem( int nFixCode , int         nValue   );
	bool    SetItem( int nFixCode , const char *pszData  ); // ����ĳ�������ַ������� -- �ַ���
	bool	SetItemBuf( int nFixCode, char *pBuf, int nSize );
private:
	ACOBase *_pAco;
};
#endif//_FIXCOMMA_H_BH_
