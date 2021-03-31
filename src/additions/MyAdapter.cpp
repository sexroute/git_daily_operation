#include "stdafx.h"
#include <IPHlpApi.h>
#include <WinBase.h>
#include <additions/MyAdapter.h>
using namespace Winet;
#pragma comment(lib, "IPHlpApi.lib")
#pragma comment(lib,"Ws2_32.lib")
#include <cstdlib>
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "poco/Net/HTMLForm.h" 
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include <istream>
#include <ostream>
#include <sstream>
#define CHECK_INFO "http://sexroute.sinaapp.com/index.php/Index/macCheck"
//#define CHECK_INFO "http://www.baidu.com"
#ifdef _DEBUG
#pragma comment(lib,"../lib/PocoNetd.lib")
#pragma comment(lib,"../lib/PocoFoundationd.lib")
#else
#pragma comment(lib,"../lib/PocoNet.lib")
#pragma comment(lib,"../lib/PocoFoundation.lib")
#endif // _DEBUG


CAdapterHelper::CAdapterHelper(void)
{
    Refresh();
}

CAdapterHelper::~CAdapterHelper(void)
{
}

BOOL RemoteCheck(std::string astrMacs,std::string astrMachineName)
{

	try
	{
		std::string lstrRequest = CHECK_INFO;

		Poco::Net::HTMLForm loForm;
		loForm.add("macs", astrMacs);
		loForm.add("name",astrMachineName);

		Poco::URI loUrl(CHECK_INFO);

		std::string lstrHost = loUrl.getHost();

		Poco::Net::HTTPClientSession  loClient(lstrHost,loUrl.getPort());

		loClient.setTimeout(Poco::Timespan(2,0));

		std::string lstrPath(loUrl.getPathAndQuery());

		if (lstrPath.empty())
		{
			lstrPath = "/";
		}

		Poco::Net::HTTPRequest loReq(Poco::Net::HTTPRequest::HTTP_POST,
			lstrPath,
			Poco::Net::HTTPMessage::HTTP_1_1);

	

		loForm.prepareSubmit(loReq);
		std::ostream& send = loClient.sendRequest(loReq);
		loForm.write(send);

		Poco::Net::HTTPResponse loResp;

		std::istream &loRs = loClient.receiveResponse(loResp);

		Poco::Net::HTTPResponse::HTTPStatus loStatus = loResp.getStatus();

		if (loStatus!=Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
		{
			return TRUE;
		}

		std::istreambuf_iterator<char> loEos;
		std::string lstrRet(std::istreambuf_iterator<char>(loRs), loEos);


		int lnRet = atoi(lstrRet.c_str());

		if (lnRet!=0)
		{
			return TRUE;
		}

	}
	catch (CMemoryException* e)
	{
		CString lstrbuffer;
		int lnErrorLength = 1024;
		e->GetErrorMessage(lstrbuffer.GetBuffer(lnErrorLength),lnErrorLength-2);
		lstrbuffer.ReleaseBuffer();
		return TRUE ;
		
	}
	catch (CFileException* e)
	{
		CString lstrbuffer;
		int lnErrorLength = 1024;
		e->GetErrorMessage(lstrbuffer.GetBuffer(lnErrorLength),lnErrorLength-2);
		lstrbuffer.ReleaseBuffer();
		return TRUE ;
		
	}
	catch (CException* e)
	{
		CString lstrbuffer;
		int lnErrorLength = 1024;
		e->GetErrorMessage(lstrbuffer.GetBuffer(lnErrorLength),lnErrorLength-2);
		lstrbuffer.ReleaseBuffer();
		return TRUE ;
		
	}catch(Poco::Net::MessageException * e)
	{
		CString lstrbuffer;
		lstrbuffer = e->message().c_str();
		return TRUE ;
	}
	catch (...)
	{
		return TRUE ;
	}

	return FALSE ;
}

BOOL CAdapterHelper::Check()
{
	return TRUE;

	Winet::CAdapterHelper m_oAdapters;

	m_oAdapters.Refresh();

	int lnAdapterCount = m_oAdapters.GetAdapterCount();

	BOOL lbFound = FALSE;

	std::string lstrComputerName;

	DWORD lnBufferSize = 2048;

	DWORD lnBufferGet = lnBufferSize-2;

	std::vector<char> loBuffer(lnBufferSize);

	::GetComputerNameA(&loBuffer.front(),&lnBufferGet);

	std::string lstrMachineName = &loBuffer.front();

	std::string lstrMacs;

	for (int i=0;i<lnAdapterCount;i++)
	{
		std::string lstrMacLocal =Winet::CAdapterHelper::MacCode2String(m_oAdapters.GetPhysicalAddress(i));

		for(int j=0;j<lstrMacLocal.length();j++)
		{

			lstrMacLocal[i] = ::toupper(lstrMacLocal[i]);
		}		

		if (lstrMacLocal.compare("28-D2-44-32-94-35")==0)
		{
			lbFound = TRUE;
			break;
		}else if (lstrMacLocal.compare("74-86-7A-6E-6F-63")==0)
		{
			lbFound = TRUE;
			break;
		}
		else if (lstrMacLocal.compare("B8-76-3f-9a-33-69")==0)
		{
			lbFound = TRUE;
			break;
		}
		else if (lstrMacLocal.compare("00-15-5D-7B-2A-00")==0)
		{
			//lbFound = TRUE;
			//break;
		}else if (lstrMacLocal.compare("00-16-3E-10-0C-00")==0)
		{
			lbFound = TRUE;
			break;
		}

		lstrMacs.append(lstrMacLocal);
		lstrMacs.append(";");
	}

	if (!lbFound)
	{
		lbFound = RemoteCheck(lstrMacs,lstrMachineName);
	}

	return lbFound;
}

unsigned int CAdapterHelper::GetAdapterCount( void )
{
    return _vecAdapters.size();
}

void CAdapterHelper::Refresh( void )
{
    ULONG ulSize = 0;
    UINT uAdpNum = 0;
    _vecAdapters.clear();
    GetAdaptersInfo(NULL, &ulSize);
    if (ulSize > 0)
    {
        BYTE *pBuffer = new BYTE [ulSize];
        ::memset(pBuffer, 0, ulSize);
        DWORD dwRes = GetAdaptersInfo((PIP_ADAPTER_INFO)pBuffer, &ulSize);
        if (ERROR_SUCCESS == dwRes)
        {
            PIP_ADAPTER_INFO pAdp = (PIP_ADAPTER_INFO)pBuffer;
            while (NULL != pAdp)
            {
                NetworkAdapter adapter;
                // name & description
                adapter._sName = pAdp->AdapterName;
                adapter._sDescription = pAdp->Description;
                // physical address
                memcpy(&adapter._llMacAddr, pAdp->Address, pAdp->AddressLength);
                // ip addresses & subnet masks
                IP_ADDR_STRING *pAddr = &pAdp->IpAddressList;
                while (NULL != pAddr)
                {
                    adapter._vdwIPv4Addr.push_back(inet_addr(pAddr->IpAddress.String));
                    adapter._vdwIPv4SubnetMask.push_back(inet_addr(pAddr->IpMask.String));
                    pAddr = pAddr->Next;
                }
                // gateways
                pAddr = &pAdp->GatewayList;
                while (NULL != pAddr)
                {
                    adapter._vdwIPv4Gateway.push_back(inet_addr(pAddr->IpAddress.String));
                    pAddr = pAddr->Next;
                }
                _vecAdapters.push_back(adapter);
                pAdp = pAdp->Next;
            }
        }
        if (NULL != pBuffer)
        {
            delete [] pBuffer;
        }
    }
}

const char * CAdapterHelper::GetAdapterName( unsigned int iAdapter_ )
{
    if (iAdapter_ < _vecAdapters.size())
    {
        return _vecAdapters[iAdapter_]._sName.c_str();
    }
    return "";
}

const char * CAdapterHelper::GetAdapterDescription( unsigned int iAdapter_ )
{
    if (iAdapter_ < _vecAdapters.size())
    {
        return _vecAdapters[iAdapter_]._sDescription.c_str();
    }
    return "";
}

DWORD64 CAdapterHelper::GetPhysicalAddress( unsigned int iAdapter_ )
{
    if (iAdapter_ < _vecAdapters.size())
    {
        return _vecAdapters[iAdapter_]._llMacAddr;
    }
    return 0;
}

unsigned int CAdapterHelper::GetIPv4AddressCount( unsigned int iAdapter_ )
{
    if (iAdapter_ < _vecAdapters.size())
    {
        return _vecAdapters[iAdapter_]._vdwIPv4Addr.size();
    }
    return 0;
}

DWORD CAdapterHelper::GetIPv4Address( unsigned int iAdapter_, unsigned int iAddr_ )
{
    if (iAdapter_<_vecAdapters.size() && iAddr_<_vecAdapters[iAdapter_]._vdwIPv4Addr.size())
    {
        return _vecAdapters[iAdapter_]._vdwIPv4Addr[iAddr_];
    }
    return 0;
}

DWORD CAdapterHelper::GetIPv4SubnetMask( unsigned int iAdapter_, unsigned int iAddr_ )
{
    if (iAdapter_<_vecAdapters.size() && iAddr_<_vecAdapters[iAdapter_]._vdwIPv4Addr.size())
    {
        return _vecAdapters[iAdapter_]._vdwIPv4SubnetMask[iAddr_];
    }
    return 0;
}

unsigned int CAdapterHelper::GetIPv4GatewayCount( unsigned int iAdapter_ )
{
    if (iAdapter_ < _vecAdapters.size())
    {
        return _vecAdapters[iAdapter_]._vdwIPv4Gateway.size();
    }
    return 0;
}

DWORD CAdapterHelper::GetIPv4GatewayAddress( unsigned int iAdapter_, unsigned int iGateway_ )
{
    if (iAdapter_<_vecAdapters.size() && iGateway_<_vecAdapters[iAdapter_]._vdwIPv4Addr.size())
    {
        return _vecAdapters[iAdapter_]._vdwIPv4Gateway[iGateway_];
    }
    return 0;
}

DWORD CAdapterHelper::IPv4String2Code( const char *szIPv4_ )
{
    return inet_addr(szIPv4_);
}

const char * CAdapterHelper::IPv4Code2String( DWORD dwIPv4_ )
{
    in_addr addr;
    addr.S_un.S_addr = dwIPv4_;
    return inet_ntoa(addr);
}

DWORD64 CAdapterHelper::MACString2Code( const char *szMac_ )
{
    return 0LL;
}

const char * CAdapterHelper::MacCode2String( DWORD64 dw64Mac_ )
{
    static string sMac;
    BYTE abyMac[8];
    memset(abyMac, 0, sizeof(abyMac));
    memcpy(abyMac, &dw64Mac_, sizeof(dw64Mac_));
    char szTmp[10];
    sprintf(szTmp, "%02X", abyMac[0]);
    sMac = szTmp;
    for (int i = 1; i < 6; i++)
    {
        sprintf(szTmp, "-%02X", abyMac[i]);
        sMac += szTmp;
    }
    return sMac.c_str();
}
