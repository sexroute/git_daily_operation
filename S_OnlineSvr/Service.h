
typedef struct Service_Module{
	DWORD		Ver					;  	   // 版本
	char		*pszAuthor			;	   // 作者说明
	char		*pszDescription		;      // 模块说明
	char		*pszModuleName		;	   // 模块名称
	char		*pszModuleTypeName	;	   // 模块类型名称
	char 	    *pszBuildDate		;      // 编译日期
	DWORD		dwInputType   		;      // 输入数据数据类型约定
	void		*pInputPointer		;	   // 输入数据 [in/out]
	void		*pOutputPointer		;	   // 输入数据 [in/out]
	DWORD		dwSize				;	   // 输入数据的大小 [in]
	HWND		hParentWnd			;	   // 父窗口	[in]
	HINSTANCE	hDllInst			;	   // Dll句柄	[in]	

	CFixComm	*pFixComm			;
	CCustomDB	*pDB				;
	char		*pszNote			;
	long		*pCode				;
    
	bool (*Config	 )( struct Service_Module * pModule ) ;	    //设置函数
	bool (*Init		 )( struct Service_Module * pModule ) ;		//初始化函数
	bool (*Quit		 )( struct Service_Module * pModule ) ;		//退出函数
	bool (*Run		 )( struct Service_Module * pModule ) ;		//执行函数

	bool (*Do		 )( struct Service_Module * pModule ) ;		//操作函数
	bool (*DoNext    )( struct Service_Module * pModule ) ;		//操作函数
	bool (*DoReserve )( struct Service_Module * pModule ) ;     //预留函数
} Service_Module ;

//函数定义
bool Config	  ( struct Service_Module * pModule ) ;	//设置函数
bool Init	  ( struct Service_Module * pModule ) ;	//初始化函数
bool Quit	  ( struct Service_Module * pModule ) ;	//退出函数
bool Run	  ( struct Service_Module * pModule ) ;	//执行函数
bool Do		  ( struct Service_Module * pModule ) ;		//操作函数
bool DoNext   ( struct Service_Module * pModule ) ;		//操作函数
bool DoReserve( struct Service_Module * pModule ) ;     //预留函数

//接口定义
Service_Module *Service();
