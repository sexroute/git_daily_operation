#pragma  once

#ifdef _KDCOMMLIB_MARBLE_H_
#define _KDCOMMLIB_API_  __declspec(dllexport)
#else
#define _KDCOMMLIB_API_  __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "KDCommLibD.lib")
#else
#pragma comment(lib, "KDCommlib.lib")
#endif
#endif

#include <atlstr.h>
#include <vector>
using std::vector;

enum BHLOG_MODE_ENUM {
	BHLOG_MODE_TRACE  = 0,
	BHLOG_MODE_DEBUG,
	BHLOG_MODE_INFO,
	BHLOG_MODE_WARN,
	BHLOG_MODE_ERROR,
	BHLOG_MODE_FATAL,
	BHLOG_MODE_FILE,
	BHLOG_MODE_NUM
};

 CString _KDCOMMLIB_API_ getStringFormattedAs(const TCHAR * format, ...);
 bool _KDCOMMLIB_API_ BHLog(BHLOG_MODE_ENUM level, CString msg, CString loc);

#define STR getStringFormattedAs


#define gc_cTimeFormat          "%Y-%m-%d %H:%M:%S"

#define BHLOG(level, info) \
	do {	if (IsDebuggerPresent()||(level !=BHLOG_MODE_DEBUG)){\
	CString sLine; \
	sLine.Format("%s@%d", __FUNCTION__, __LINE__); \
	BHLog(level, info, sLine); \
	}\
	}while(0)

#define _BHLOG_TRACE(info)
#define _BHLOG_DEBUG(info)
#define _BHLOG_INFO(info)
#define _BHLOG_WARN(info)
#define _BHLOG_ERROR(info)
#define _BHLOG_FATAL(info)
#define _BHLOG_FILE(info)

#define _BHLOG_ARRAY(ArrayName, ArrayData, ArrayNum)

#define BHLOG_TRACE(info) BHLOG(BHLOG_MODE_TRACE, info)
#define BHLOG_DEBUG(info) BHLOG(BHLOG_MODE_DEBUG, info)
#define BHLOG_INFO(info)  BHLOG(BHLOG_MODE_INFO,  info)
#define BHLOG_WARN(info)  BHLOG(BHLOG_MODE_WARN,  info)
#define BHLOG_ERROR(info) BHLOG(BHLOG_MODE_ERROR, info)
#define BHLOG_FATAL(info) BHLOG(BHLOG_MODE_FATAL, info)
#define BHLOG_FILE(info)  BHLOG(BHLOG_MODE_FILE, info)

#define BHLOG_ARRAY(ArrayName, ArrayData, ArrayNum) \
	do {\
	if(ArrayNum > 0){ \
	string logStr = ArrayName; \
	logStr.append(STR("[%d]=%f", ArrayNum, (double)ArrayData[0])); \
	for(int i = 1; i < ArrayNum; i++){ \
	logStr.append(STR(",%f", (double)ArrayData[i])); \
	} \
	BHLOG_DEBUG(logStr); \
	} \
	}while(0)

#define BHLOG_ID(id) BHLOG_DEBUG(STR("%s(%d)", # id, id))

/// 得到进程当前目录
CString _KDCOMMLIB_API_  GetAppPath();

/// 得到进程当前全路径名
CString _KDCOMMLIB_API_  GetFilePathFromFullName();

#define WRITE_FATAL(e) 	if (NULL!=e)\
{\
	CString lstrErrorMessage;\
	e->GetErrorMessage(lstrErrorMessage.GetBufferSetLength(1024),255);\
	lstrErrorMessage.ReleaseBuffer();\
	BHLOG_FATAL(lstrErrorMessage);\
	e->Delete();\
}\


#define BEGIN_ERROR_HANDLE		try{ \
	try{\

#define END_ERROR_HANDLE				    }\
	catch (CMemoryException* e)\
{\
	WRITE_FATAL(e);\
}\
	catch (CFileException* e)\
{\
	WRITE_FATAL(e);\
}\
	catch (CException* e)\
{\
	WRITE_FATAL(e);\
}\
}catch(...){WRITE_FATAL_LOG;}

#define WRITE_FATAL_LOG 						{\
	ASSERT(FALSE);\
	CString lstrFatalLine ;\
	lstrFatalLine.Format(_T("****** Fatal Error ****** %s %d"),__FILE__,__LINE__);\
	BHLOG_FATAL(lstrFatalLine);\
};

extern "C"
{
	// !\name 公共函数
	//@{

	/// 喂狗
	/** 更新看门狗文件
	*	\param[in] strFile_ 看门狗文件名
	*	\return 喂狗成功标志
	*/
	bool _KDCOMMLIB_API_ Feed(const CString &strFile_);

	/// 启动自动升级程序
	/** 同时发送执行更新的消息给升级程序
	*	\return 是否启动成功
	*/
	bool _KDCOMMLIB_API_ StartUpdater(int nTryCount = 10);

    /// 版本号
    struct _KDCOMMLIB_API_ S_VERSION_NO
    {
        WORD wMain;
        WORD wSub;
        WORD wBuild;
        WORD wCompile;

        S_VERSION_NO(void);
        inline CString ToString(void);
    };

    /// 版本信息类
    class _KDCOMMLIB_API_ CBHVersion
    {
    public:
        CBHVersion(LPCTSTR szPath_ = "");

    public:
        bool GetFileVersion(S_VERSION_NO &verNo_);
        bool GetProductVersion(S_VERSION_NO &verNo_);
        bool GetFileFlags(DWORD& rdwFlags);
        bool GetFileOS(DWORD& rdwOS);
        bool GetFileType(DWORD& rdwType);
        bool GetFileSubtype(DWORD& rdwType);	

        // string info
        bool GetCompanyName(CString &strCompanyName_);
        bool GetFileDescription(CString &strFileDesc_);
        bool GetProductName(CString &strProductName_);

    protected:
        inline bool Init();
        bool GetFixedInfo(void *pBlock_, VS_FIXEDFILEINFO& rFixedInfo_);
        bool GetStringInfo(LPVOID pBlock_, LPCTSTR lpszKey, CString &strValue_);

        bool _bInit;
        CString	_strPath;
        CString	_strCompanyName;
        CString	_strProductName;
        CString	_strFileDescription;
        VS_FIXEDFILEINFO _fixFileInfo;
    };

	/// 数据锁类
	class _KDCOMMLIB_API_ CMutexRW
	{
	protected:
		HANDLE m_semReaders;
		HANDLE m_semWriters;
		long m_lnReaders;
	public:
		CMutexRW();
		virtual ~CMutexRW();

		void BeginRead();
		void EndRead();
		void BeginWrite();
		void EndWrite();
	};


	/// 文件夹浏览类
	/** 浏览文件夹对话框供用户选择目录 */
	class _KDCOMMLIB_API_ CBrowseForFolder
	{
	public:
		CBrowseForFolder(const HWND hParent = NULL, const LPITEMIDLIST pidl = NULL, const int nTitleID = 0);
		CBrowseForFolder(const HWND hParent, const LPITEMIDLIST pidl, const CString& strTitle);
		virtual ~CBrowseForFolder(void);

		/// 设置文件夹选择对话框的owner窗口
		void SetOwner(const HWND hwndOwner);
		/// 设置待浏览的目录树的根目录(NULL表示设置桌面为根目录)
		void SetRoot(const LPITEMIDLIST pidl = NULL);
		/// 获取文件夹选择对话框的标题
		CString GetTitle(void) const;
		/// 设置文件夹选择对话框的标题: 重载1
		void SetTitle(const CString& strTitle);
		/// 设置文件夹选择对话框的标题: 重载2
		bool SetTitle(const int nTitle);
		/// 获取BROWSEINFO属性ulFlags, 参见MSDN中关于BROWSEINFO::ulFlags的说明
		UINT GetFlags(void) const;
		/// 设置BROWSEINFO的ulFlags属性
		void SetFlags(const UINT ulFlags);
		// 获取已选定的文件夹路径
		CString GetSelectedFolder(void) const;
		/// 获取与选定文件夹相关联的图片
		int GetImage(void) const;
		/// 弹出目录选择对话框
		bool SelectFolder(void);

	protected:
		virtual void OnInit(void) const;
		virtual void OnSelChanged(const LPITEMIDLIST pidl) const;
		void EnableOK(const bool bEnable) const;
		void SetStatusText(const CString& strText) const;
        void SetSelection(const LPITEMIDLIST pidl) const;
        void SetSelection(const CString& strPath) const;
	private:
		static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
		typedef std::auto_ptr<char> AUTO_STR;
		AUTO_STR m_pchTitle;
		BROWSEINFO m_bi;
		char m_szSelected[MAX_PATH];
		CString m_strPath;
		HWND m_hwnd;
	};

    /// 日志级别
	enum E_LOG_LEVEL
	{
		MODE_TRACE = 0x0001,				//比较细节的信息
		MODE_DEBUG = 0x0002,				//系统流程中的细节信息
		MODE_INFO  = 0x0004,				//运行时产生的有意义的事件
		MODE_WARN  = 0x0010,				//运行时不合需要和不合预期的状态但还没有称为“错误”
		MODE_ERROR = 0x0020,				//其它运行期的错误或不是预期的事件
		MODE_FATAL = 0x0040					//非常严重的错误，导致系统中止
	};

    /// 日志类
    class _KDCOMMLIB_API_ CHZLog 
    {
    public:
        CHZLog(void);
	    virtual ~CHZLog(void);
    	
    public:

        static CHZLog * CreateLogTxt(const CString &strFileDir_, const CString &strFileName, int iLogLevel_ = MODE_ERROR | MODE_FATAL);
        static void ReleaseAllLog(void);
        static E_LOG_LEVEL GetLevelByString( const CString &strLevel );

	    virtual BOOL IsDebugEnabled() = 0;
	    virtual BOOL IsErrorEnabled() = 0;
	    virtual BOOL IsFatalEnabled() = 0;
	    virtual BOOL IsInfoEnabled() = 0;
	    virtual BOOL IsTraceEnabled() = 0;
	    virtual BOOL IsWarnEnabled() = 0;

	    virtual void Trace(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;
	    virtual void Debug(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;
	    virtual void Info(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  ) = 0;
	    virtual void Warn(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;
	    virtual void Error(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;
	    virtual void Fatal(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;

    protected:
        static std::vector<CHZLog*> s_vecLog;
	    int _iLogLevel;		// 日志级别
    };


	/// 日志管理类
	class _KDCOMMLIB_API_ CHZLogManage 
	{
	public:
		CHZLogManage(void);
		~CHZLogManage(void);

	public:
		static BOOL IsDebugEnabled();
		static BOOL IsErrorEnabled(); 
		static BOOL IsFatalEnabled();
		static BOOL IsInfoEnabled();
		static BOOL IsTraceEnabled(); 
		static BOOL IsWarnEnabled();
		static void Trace(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ); 
		static void Debug(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  ); 
		static void Info(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" );
		static void Warn(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );
		static void Error(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );
		static void Fatal(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );

		static void StartLogTxt(CString strFileDir_,CString strFileName,int iLogLevel_ = 2);
		
		static UINT GetLogger(CString strFileDir_,CString strFileName,int iLogLevel_ = 2 );
		
		static void Log(	UINT anLoggHandle,
							const CString &strMsg, 
							const CString &strModule = "", 
							const CString &strOperType = "" ,
							int iLogLevel_= 2);
		
		static void ReleaseLogger(UINT anLoggHandle);

		static void SetLogFileLimit(INT64 anFileSizeInBytes);
		static INT64 GetLogFileLimit();

		static void SetMinDiskFreeLimit(INT64 anDiskFreeLimit);
		static INT64 GetMinDiskFreeLimit();

    protected:
		static void InitLog( CString strKey = "", CString strFile = "" );
		static void ValidLog();

	protected:
		static CHZLog *m_pLog;					//日志接口
	};


    /// 异常处理方式
    enum E_EXCEPTION_HANDLE_TYPE
    {
        EH_MSG_BOX = 0x0001,           ///< 对话框报错
        EH_WRITE_LOG = 0x0002,        ///< 日志报错
        EH_WRITE_DUMP = 0x0004,     ///< 写dump文件
        EH_STATIC_EXIT = 0x0008         ///< 静默退出
    };

    /// 崩溃信息捕捉类
    class _KDCOMMLIB_API_ CBHCrashCapture
    {
    public:
        /// 加载异常处理功能
        static void Load(int iType_ = EH_WRITE_LOG | EH_WRITE_DUMP | EH_STATIC_EXIT);
        /// 卸载异常处理功能
        static void Unload(void);

    protected:
        /// 程序崩溃信息
        struct S_CRASH_INFO
        {
            DWORD64 dw64Addr;      ///< 异常发生地址
            DWORD dwCode;             ///< 异常码
            DWORD dwFlag;               ///< 异常标志
            S_CRASH_INFO(void): dw64Addr(0), dwCode(0), dwFlag(0)
            {
            }
        };
        /// 调用堆栈信息
        struct S_CALL_STACK_INFO
        {
            CString strModuleName;      ///< 模块名
            CString strMethodName;      ///< 函数名
            CString strFileName;             ///< 文件名
            DWORD dwLineNo;              ///< 代码行号
            S_CALL_STACK_INFO(void): dwLineNo(0)
            {
            }
        };
        static int s_iType;      ///< 异常处理类型
        static LPTOP_LEVEL_EXCEPTION_FILTER s_lpPrevFilter;  ///< 之前的异常处理函数指针

        /// 异常处理例程
        static long _stdcall AppCrashHandler(_EXCEPTION_POINTERS *pExceptionInfo_);
        /// 获取程序崩溃信息
        static bool GetCrashInfo(const EXCEPTION_RECORD *pRecord_, S_CRASH_INFO &crash_);
        /// 获取调用堆栈信息
        static bool GetCallStackInfo(const CONTEXT *pContext_, vector<S_CALL_STACK_INFO> &vecStack_);
        /// 生成dump文件
        static bool CreateDumpFile(_EXCEPTION_POINTERS *pExceptionInfo_, const char *szFilePath_);
    };


    /// 地址类型
    enum E_BH_ADDRESS_TYPE
    {
        ADDR_MAC,
        ADDR_IPV4,
        ADDR_IPV6
    };
    /// 地址类
    class _KDCOMMLIB_API_ CBHAddress
    {
    public:
        /// 地址类型
        enum
        {
            ADDR_MAC,
            ADDR_IPV4,
            ADDR_IPV6
        };

        CBHAddress(int iType_ = ADDR_MAC);
        CBHAddress(const CBHAddress &addr_);
        CBHAddress & operator=(const CBHAddress &addr_);

        /// 获取类型
        int GetType(void);
        /// 获取地址
        bool GetAddress(CString &strAddr_);
        bool GetAddress(vector<BYTE> &vbyAddr_);
        /// 设置地址
        bool SetAddress(const char *szAddr_);
        bool SetAddress(const vector<BYTE> &vbyAddr_);

    protected:
        BYTE _abyAddr[16];   ///< 地址信息
        int _iType;                 ///< 地址类型
        int _nLen;                  ///< 地址长度
        int _nGroups;            ///< 地址分组数
        char _chSep;             ///< 地址字符串的分隔符
        bool _bHex;              ///< 是否16进制表示
        bool _bInit;               ///< 初始化是否成功

        /// 字符串转换16进制数
        static void Str2Hex(const char *szHexStr_, vector<BYTE> &vbyHex_);
    };

    /// 网卡管理类
    class _KDCOMMLIB_API_ CBHNetworkAdapters
    {
    public:
        CBHNetworkAdapters(void);
        ~CBHNetworkAdapters(void);

        /// 刷新网卡信息
        void Refresh(void);
        /// 网卡数量
        unsigned int GetAdaptersCount(void);
        /// 获取网卡名称
        CString GetAdapterName(unsigned int iAdapter_);
        /// 获取网卡描述信息
        CString GetAdapterDescription(unsigned int iAdapter_);
        /// 获取网卡物理地址
        bool GetPhysicalAddress(unsigned int iAdapter_, CBHAddress &addrMac_);
        /// 获取IP地址数目
        unsigned int GetIPAddressesCount(unsigned int iAdapter_);
        /// 获取IP地址
        bool GetIPAddress(unsigned int iAdapter_, unsigned int iAddr_, CBHAddress &addrIP_);
        /// 获取子网掩码
        bool GetSubnetMask(unsigned int iAdapter_, unsigned int iAddr_, CBHAddress &addrSubnet_);
        /// 获取网关数目
        unsigned int GetGatewaysCount(unsigned int iAdapter_);
        /// 获取网关地址
        bool GetGatewayAddress(unsigned int iAdapter_, unsigned int iGateway_, CBHAddress &addrGateway_);

    protected:
        /// 网卡信息结构体
        struct S_BHNetworkAdapter
        {
            CString _strName;                                     ///< 网卡名称
            CString _strDescription;                             ///< 网卡描述信息
            CBHAddress _addrMac;                             ///< 网卡物理地址
            vector<CBHAddress> _vecIpAddr;             ///< IP地址列表
            vector<CBHAddress> _vecSubnetMask;    ///< 子网掩码列表
            vector<CBHAddress> _vecGateway;         ///< 网关列表
        };

        vector<S_BHNetworkAdapter> _vecAdapters;   ///< 网卡信息
    };

	//@}
}