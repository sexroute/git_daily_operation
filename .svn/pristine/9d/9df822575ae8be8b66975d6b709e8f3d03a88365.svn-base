
#include <winsock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
class HandleWatchDog
{
public:
	HandleWatchDog()
	{
		this->m_phHandle = NULL;
		this->m_bAutoClose = TRUE;
	};
	~HandleWatchDog()
	{
		if (this->m_phHandle!=NULL)
		{
			try
			{
				if (this->m_bAutoClose)
				{
					try
					{
						if ((*this->m_phHandle!=NULL) && (*this->m_phHandle!= INVALID_HANDLE_VALUE))
						{
							BOOL lbRet = CloseHandle(*this->m_phHandle);
						}


					}catch(...)
					{

					}
				}
			}
			catch (...)
			{

			}

		}
	}
	void SetHandle(const HANDLE &ahHandle)
	{
		this->m_phHandle = (HANDLE *)&ahHandle;
	}

	void SetEnableAutoCloseHandle(BOOL abFalse)
	{
		this->m_bAutoClose = abFalse;
	}

	BOOL  GetEnableAutoCloseHandle()
	{
		return this->m_bAutoClose;
	}
private:
	HANDLE * m_phHandle;
	BOOL m_bAutoClose;

};
class CRemoteSesrver
{
public:
	static BOOL  IsPortAvailable(int anPort);
	static BOOL DetectAvailablePort(int & anPort);
	static int CRemoteSesrver::replace_IAT(const char *pDllName,
		const char *pApiName,
		bool bReplace,
		DWORD * apOriginalFunc,
		DWORD apFakeFunc);

	static BOOL CRemoteSesrver::DisableStandardAbort71();
	static BOOL CRemoteSesrver::StartDebugger();

};