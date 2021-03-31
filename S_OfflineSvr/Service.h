
typedef struct Service_Module{
	DWORD		Ver					;  	   // �汾
	char		*pszAuthor			;	   // ����˵��
	char		*pszDescription		;      // ģ��˵��
	char		*pszModuleName		;	   // ģ������
	char		*pszModuleTypeName	;	   // ģ����������
	char 	    *pszBuildDate		;      // ��������
	DWORD		dwInputType   		;      // ����������������Լ��
	void		*pInputPointer		;	   // �������� [in/out]
	void		*pOutputPointer		;	   // �������� [in/out]
	DWORD		dwSize				;	   // �������ݵĴ�С [in]
	HWND		hParentWnd			;	   // ������	[in]
	HINSTANCE	hDllInst			;	   // Dll���	[in]	

	CFixComm	*pFixComm			;
	CCustomDB	*pDB				;
	char		*pszNote			;
	long		*pCode				;
    
	bool (*Config	 )( struct Service_Module * pModule ) ;	    //���ú���
	bool (*Init		 )( struct Service_Module * pModule ) ;		//��ʼ������
	bool (*Quit		 )( struct Service_Module * pModule ) ;		//�˳�����
	bool (*Run		 )( struct Service_Module * pModule ) ;		//ִ�к���

	bool (*Do		 )( struct Service_Module * pModule ) ;		//��������
	bool (*DoNext    )( struct Service_Module * pModule ) ;		//��������
	bool (*DoReserve )( struct Service_Module * pModule ) ;     //Ԥ������
} Service_Module ;

//��������
bool Config	  ( struct Service_Module * pModule ) ;	//���ú���
bool Init	  ( struct Service_Module * pModule ) ;	//��ʼ������
bool Quit	  ( struct Service_Module * pModule ) ;	//�˳�����
bool Run	  ( struct Service_Module * pModule ) ;	//ִ�к���
bool Do		  ( struct Service_Module * pModule ) ;		//��������
bool DoNext   ( struct Service_Module * pModule ) ;		//��������
bool DoReserve( struct Service_Module * pModule ) ;     //Ԥ������

//�ӿڶ���
Service_Module *Service();
