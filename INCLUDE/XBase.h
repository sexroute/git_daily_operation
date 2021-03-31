// XBase.h: interface for the CXBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_)
#define AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagDBFHead{ /* DBFͷ�ṹ */
   unsigned char  mark;
   unsigned char  year;
   unsigned char  month;
   unsigned char  day;
   long			  count;
   unsigned short hlen;
   unsigned short reclen;
   char  remain[20];
} DBFHEAD;
typedef struct tagDBFField{ /* DBF�ֶνṹ */
   char name[11];
   char type;
   unsigned short offs;
   char remain[2];
   unsigned char width;
   unsigned char dec;
   char remain1[14];
} DBFFIELD;

#define _DBF_FORWRITE		1
#define _DBF_FASTWRITE		2
#define _DBF_FASTREAD		4

class AFX_EXT_CLASS CXBase  
{
public:
	CXBase();
	virtual ~CXBase();
public:
	bool Use(const char * pszFileName,int nStyle = 0); 
	//��DBF�ļ� nStyle&1=1��д nStyle&2=2:����д(�������ύ) nStyle&4=4,���(�����¶���¼��)
	bool Create(const char * pszFileName,const char * field,int nStyle = 0); 
	void Close(); //�ر�DBF
	bool Eof();	
	bool Bof();
	int  GetNo();	//ȡ��¼�� ��0��ʼ
	int  GetCount(BOOL bRefresh=FALSE); //ȡ��¼��
	DBFHEAD		m_Head;	
public:
	int GetFCount(); //ȡ�ֶ�����
	int FindIndex(const char * pszName); //����ָ���ֶ������ֶ����0 - m_nFCount-1
	bool GetFieldInfo(int nIndex,char * pszName ,BYTE * pType = NULL,int * pWidth = NULL,int * pDec = NULL,int * pStart = NULL);
	//CTime GetDBFDate();
//	bool GetFieldInfo(int nIndex,char * pszName=NULL,BYTE * pType=NULL,int * pWidth=NULL,int * pDec=NULL,int * pStart=NULL);
	//ȡ�ֶ���Ϣ
public:  //��¼�ƶ�
	void GoBottom();
	void GoTop();
	bool Go(int nNo);
	bool Skip(int nOffset = 1);
public: //�ֶ�ȡֵ
	char *	GetText(int nIndex,char * out);           //ȡ���������ֶε��ַ�ֵ
	char *	GetText(const char * pszName,char * out); //ȡ���������ֶε��ַ�ֵ
	char *	GetDate(int nIndex,char * out);           //ȡD�����ֶε������ַ�ֵ
	char *	GetDate(const char * pszName,char * out); //ȡD�����ֶε������ַ�ֵ
	long	GetLong(int nIndex);			   //ȡN�����ֶε�longֵ
	long	GetLong(const char * pszName);     //ȡN�����ֶε�longֵ
	double	GetDouble(int nIndex);			   //ȡN�����ֶε�doubleֵ
	double	GetDouble(const char * pszName);   //ȡN�����ֶε�doubleֵ(��С���Զ�ȡ��)
	bool	GetLogical(int nIndex);			   //ȡL���Զ���boolֵ
	bool	GetLogical(const char * pszName);  //ȡL���Զ���boolֵ
	bool    IsDeleted();						//�жϼ�¼�Ƿ�ɾ��
public: //�ֶη�ֵ
	void Set(int nIndex,const char * in);			//D��C������Ч
	void Set(const char * pszName,const char * in);	//D��C������Ч
	void Set(int nIndex,long nValue);				//N����С��Ϊ0��Ч
	void Set(const char * pszName,long nValue);		//N����С��Ϊ0��Ч
	void Set(int nIndex,double dValue);				//N����С��>0��Ч
	void Set(const char * pszName,double dValue);	//N����С��>0��Ч
	void Set(int nIndex,bool bValue=true);			//L������Ч
	void Set(const char * pszName,bool bValue=true); //L������Ч

public: //��¼����
	char *  GetData();								//ȡ������¼������ָ��
	void    Append(const char * pszData =NULL);     //׷�Ӽ�¼
	void    Delete(bool bDelete = true);            //ɾ������ռ�¼
	void    Replace(const char * pszData =NULL);    //�޸ļ�¼
	BOOL    Write();		//����¼д��Ӳ�̣������ʱ��_DBF_FASTWRITE,��������commit
	void    Zap();  //������ݿ⣬ֻ����DBFͷ
public:  //��¼����
	CString GetDate( const char *pszName );
	BOOL UnLock();
	BOOL RLock();
	BOOL FLock();
	int     WriteInfo(const char *pData, int nLength);
	CString GetData( const char *pszName);
	CString GetText( const char *pszName);
	CString Get( const char *pszName);
	bool	Find(int nIndex,const char * pszValue);		//���ٶԷֲ���
	bool	Locate(int nIndex,const char * pszValue);	//��ʲ���
	bool	Locate(int nIndex,long nValue);				//��ʲ���
	bool    Locate(const char* pszName,long nValue);
	bool    Locate(const char* pszName,const char * pszValue);
    int     ReadInfo( char *pData, int nLength);
	bool    FreshHeadInfo();
private:
	//int			m_nHandle;  //�ļ�HANDLE
	CFile      *m_pFile ;
	int			m_nStyle;       // &1=0:ֻ����� &1=1:��д�� &2=2:���ٶ�д	
	CPtrArray	m_arField;	    // �ֶζ����б�
	CString		m_pszFileName;  // �ļ�����
    int			m_nFCount;      // �ֶ��� 
	int			m_nNo;		    // ��¼�� ��0��ʼ
	bool		m_bBof; 
	bool		m_bEof;
	bool        m_bFLock;       // �ļ���
	bool        m_bRLock;       // ��¼��
	bool		m_bChgCount;    //�Ƿ����¼�¼�����δWrite
	unsigned char *	m_pszData;  //���ݻ�����ָ��
};

#define GetIndex FindIndex

#endif // !defined(AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_)
