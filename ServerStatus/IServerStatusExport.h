


#ifdef BHXZ_EXPORT_DLL
#define BHXZ_API extern "C" __declspec(dllexport)
#else
#define BHXZ_API extern "C" __declspec(dllimport)
#endif
#ifndef BHXZ_SERVER_STATUS_API
#define  BHXZ_SERVER_STATUS_API

namespace LY
{
	typedef struct __PLANTINFO_STATUS
	{
		CString m_strVibUpdateTime;
		CString m_strDynUpdateTime;
		CString m_strProcUpdateTime;

		CString m_strVibSaveTime;
		CString m_strDynSaveTime;
		CString m_strProcSaveTime;

		int m_nVibChannCount;
		int m_nDynChannCount;
		int m_nProcChannCount;

		CString m_strGroup;
		CString m_strCompany;
		CString m_strFactory;
		CString m_strSet;
		CString m_strPlant;

		__PLANTINFO_STATUS()
		{
			this->m_nDynChannCount =0;
			this->m_nProcChannCount = 0;
			this->m_nVibChannCount =0;
		}

	} PLANTINFO_STATUS, *LP_PLANTINFO_STATUS;
	class IServerStatus
	{
	public:
		IServerStatus(void){};
		virtual ~IServerStatus(void){};
		virtual int RegistModuleComplete(char * apModuleName,int anModuleNamelength)=0;
		virtual int IsServerStartupComplete()=0;
		virtual int UpdateStatistics(PLANTINFO_STATUS & arefStatus) = 0;
	};



}

BHXZ_API LY::IServerStatus * GetServerStatus();

#endif 

