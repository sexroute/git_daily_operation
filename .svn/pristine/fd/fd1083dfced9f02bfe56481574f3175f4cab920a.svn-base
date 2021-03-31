// XBase.h: interface for the CXBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_)
#define AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagDBFHead{ /* DBF头结构 */
   unsigned char  mark;
   unsigned char  year;
   unsigned char  month;
   unsigned char  day;
   long			  count;
   unsigned short hlen;
   unsigned short reclen;
   char  remain[20];
} DBFHEAD;
typedef struct tagDBFField{ /* DBF字段结构 */
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
	//打开DBF文件 nStyle&1=1读写 nStyle&2=2:快速写(不立即提交) nStyle&4=4,快读(不重新读记录数)
	bool Create(const char * pszFileName,const char * field,int nStyle = 0); 
	void Close(); //关闭DBF
	bool Eof();	
	bool Bof();
	int  GetNo();	//取记录号 从0开始
	int  GetCount(BOOL bRefresh=FALSE); //取记录数
	DBFHEAD		m_Head;	
public:
	int GetFCount(); //取字段数量
	int FindIndex(const char * pszName); //查找指定字段名的字段序号0 - m_nFCount-1
	bool GetFieldInfo(int nIndex,char * pszName ,BYTE * pType = NULL,int * pWidth = NULL,int * pDec = NULL,int * pStart = NULL);
	//CTime GetDBFDate();
//	bool GetFieldInfo(int nIndex,char * pszName=NULL,BYTE * pType=NULL,int * pWidth=NULL,int * pDec=NULL,int * pStart=NULL);
	//取字段信息
public:  //记录移动
	void GoBottom();
	void GoTop();
	bool Go(int nNo);
	bool Skip(int nOffset = 1);
public: //字段取值
	char *	GetText(int nIndex,char * out);           //取所有类型字段的字符值
	char *	GetText(const char * pszName,char * out); //取所有类型字段的字符值
	char *	GetDate(int nIndex,char * out);           //取D类型字段的日期字符值
	char *	GetDate(const char * pszName,char * out); //取D类型字段的日期字符值
	long	GetLong(int nIndex);			   //取N类型字段的long值
	long	GetLong(const char * pszName);     //取N类型字段的long值
	double	GetDouble(int nIndex);			   //取N类型字段的double值
	double	GetDouble(const char * pszName);   //取N类型字段的double值(有小数自动取整)
	bool	GetLogical(int nIndex);			   //取L型自动的bool值
	bool	GetLogical(const char * pszName);  //取L型自动的bool值
	bool    IsDeleted();						//判断记录是否删除
public: //字段符值
	void Set(int nIndex,const char * in);			//D与C类型有效
	void Set(const char * pszName,const char * in);	//D与C类型有效
	void Set(int nIndex,long nValue);				//N类型小数为0有效
	void Set(const char * pszName,long nValue);		//N类型小数为0有效
	void Set(int nIndex,double dValue);				//N类型小数>0有效
	void Set(const char * pszName,double dValue);	//N类型小数>0有效
	void Set(int nIndex,bool bValue=true);			//L类型有效
	void Set(const char * pszName,bool bValue=true); //L类型有效

public: //记录操作
	char *  GetData();								//取整条记录的数据指针
	void    Append(const char * pszData =NULL);     //追加记录
	void    Delete(bool bDelete = true);            //删除或回收记录
	void    Replace(const char * pszData =NULL);    //修改记录
	BOOL    Write();		//将记录写入硬盘，如果打开时加_DBF_FASTWRITE,将不立即commit
	void    Zap();  //清空数据库，只保留DBF头
public:  //记录查找
	CString GetDate( const char *pszName );
	BOOL UnLock();
	BOOL RLock();
	BOOL FLock();
	int     WriteInfo(const char *pData, int nLength);
	CString GetData( const char *pszName);
	CString GetText( const char *pszName);
	CString Get( const char *pszName);
	bool	Find(int nIndex,const char * pszValue);		//快速对分查找
	bool	Locate(int nIndex,const char * pszValue);	//逐笔查找
	bool	Locate(int nIndex,long nValue);				//逐笔查找
	bool    Locate(const char* pszName,long nValue);
	bool    Locate(const char* pszName,const char * pszValue);
    int     ReadInfo( char *pData, int nLength);
	bool    FreshHeadInfo();
private:
	//int			m_nHandle;  //文件HANDLE
	CFile      *m_pFile ;
	int			m_nStyle;       // &1=0:只读标记 &1=1:读写打开 &2=2:快速读写	
	CPtrArray	m_arField;	    // 字段定义列表
	CString		m_pszFileName;  // 文件名称
    int			m_nFCount;      // 字段数 
	int			m_nNo;		    // 记录号 从0开始
	bool		m_bBof; 
	bool		m_bEof;
	bool        m_bFLock;       // 文件锁
	bool        m_bRLock;       // 记录锁
	bool		m_bChgCount;    //是否有新记录加入而未Write
	unsigned char *	m_pszData;  //数据缓冲区指针
};

#define GetIndex FindIndex

#endif // !defined(AFX_XBASE_H__E8320B61_B391_11D3_ABD6_0010A4FEC159__INCLUDED_)
