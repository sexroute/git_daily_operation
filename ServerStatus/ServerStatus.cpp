#include "stdafx.h"
#include "IServerStatusExport.h"
#include "http_parser.h"
#include "kdcommlib.h"
#include <map>
#include <vector>
#include <afxmt.h>
#include "Utility.h"
#include "additions/BHDateTime.h"
#include "HandleCollector.h"
CCriticalSection * g_pLock						= NULL;
static LY::IServerStatus * g_pServer = NULL;
char cAppLocation[5000]={0};
BOOL g_bServerStarted = FALSE;
class ServerStatusImp;
typedef struct __CLIENT_INFO
{
	CString m_strData;
	SOCKET m_oClient;
	ServerStatusImp * m_pThis;
}CLIENT_INFO;


class ServerStatusImp : public LY::IServerStatus
{
public:
	ServerStatusImp()
	{
		//1.read setting file
		GetCurrentDirectory(4999,cAppLocation);
		strcat(cAppLocation,"\\DseSvr.ini");

		//2.init module name
		g_pLock->Lock();
		BEGIN_ERROR_HANDLE;
		
		for (int i=1;i<100;i++)
		{
			CString lstrModulekey;
			lstrModulekey.Format(_T("MODULE%d"),i);
			CString lstrValue;
			int lnDataLength = 1024;
			GetPrivateProfileString(_T("MODULE"),lstrModulekey,_T(""),lstrValue.GetBufferSetLength(lnDataLength),lnDataLength,cAppLocation);
			lstrValue.ReleaseBuffer();
			lstrValue = lstrValue.Trim();
			int lnPos = lstrValue.ReverseFind('\\');
			if (lnPos>0)
			{
				lstrValue = lstrValue.Mid(lnPos+1);
			}
			if (lstrValue.IsEmpty())
			{
				break;
			}

			lstrValue = lstrValue.MakeLower();

			//regist
			this->m_oModules[lstrValue] = 0;
		}
		
		END_ERROR_HANDLE;
		g_pLock->Unlock();

		int lnData = GetPrivateProfileInt(_T("SYSTEM"),_T("PORT_SERVER_INFO"),18001,cAppLocation);
		CString lstrData;lstrData.Format(_T("%d"),lnData);
		WritePrivateProfileString(_T("SYSTEM"),_T("PORT_SERVER_INFO"),lstrData,cAppLocation);
		this->m_nServerInfoPort = lnData;

		lnData = GetPrivateProfileInt(_T("SYSTEM"),_T("PORT_SERVER_STATUS"),18002,cAppLocation);
		lstrData.Format(_T("%d"),lnData);
		WritePrivateProfileString(_T("SYSTEM"),_T("PORT_SERVER_STATUS"),lstrData,cAppLocation);
		this->m_nServerStatusPort = lnData;

		

		this->m_bServerComplete = FALSE;
		this->m_bRegenerateForm = FALSE;
	};
	virtual ~ServerStatusImp(){};
	std::map<CString,int> m_oModules;
	int m_nServerStatusPort;
	int m_nServerInfoPort;

	static UINT ClientProcForInfo(LPVOID apData)
	{
		if (NULL == apData)
		{
			return 0;
		}

		SOCKET loClient = *((SOCKET *)apData);
		char lnData[2] = {0};
		CString lstrRequested;
		int lnBufferLength = 1;
		int lnRet = recv(loClient,lnData,lnBufferLength,0);
		while (lnRet>0)
		{
			lstrRequested.Append(lnData);
			lnRet = recv(loClient,lnData,lnBufferLength,0);
		}
		BHLOG_DEBUG(lstrRequested);
		shutdown(loClient,SD_BOTH);
		closesocket(loClient);
		return 0;
	}
	
	static UINT ServerProcForInfo(LPVOID apData)
	{
		if (NULL == apData)
		{
			return 0;
		}
		g_pLock->Lock();
		g_bServerStarted = TRUE;
		g_pLock->Unlock();

		WSADATA wsa;

		SOCKET s;
		SOCKADDR_IN sAddr;

		USHORT port;


		STARTUPINFO si;
		ServerStatusImp * lpThis = (ServerStatusImp* )apData;
		port = lpThis->m_nServerInfoPort;//Set listening port

		memset( &si, 0, sizeof( si ) );
		si.cb = sizeof( si );
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

		sAddr.sin_addr.s_addr = INADDR_ANY;
		sAddr.sin_port =  (port >> 8) | (port << 8);
		sAddr.sin_family = AF_INET;

		WSAStartup( 0x0202, &wsa );

		s = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
		int lnRet = bind( s, (LPSOCKADDR)&sAddr, sizeof( sAddr ) );

		if (SOCKET_ERROR == lnRet)
		{
			return -1;
		}

		lnRet = listen( s, 5 );

		if (SOCKET_ERROR == lnRet)
		{
			return -1;
		}

		CString lstrDataWriteBack;

		g_pLock->Lock();
		std::map<CString,int>::iterator lpIt = lpThis->m_oModules.begin();
		for (lpIt=lpThis->m_oModules.begin();lpIt!=lpThis->m_oModules.end();lpIt++)
		{
			CString lstrTemp;
			lstrTemp.Format(_T("Module:%s Status:%d | "),lpIt->first,lpIt->second);
			lstrDataWriteBack += lstrTemp;
		}
		g_pLock->Unlock();

		CString lstrHttpHeader = "HTTP/1.1 200 OK\r\nCache-Control: private\r\nContent-Type: text/plain; charset=utf-8\r\nVary: Accept-Encoding\r\nServer: Microsoft-IIS/7.5\r\nConnection: close";
		CString lstrDataLengthFormat = "\r\nContent-Length: %d";
		CString lstrDataLength = "";

		while(TRUE)
		{
			SOCKADDR_IN loClientAddr ={0}; // 定义一个客户端得地址结构作为参数  

			int lnAddr_length=sizeof(loClientAddr);  

			//closesocket(loClient);

			BHLOG_DEBUG(STR(_T("Server Port: %d"),port));

			SOCKET loClient = WSAAccept(	s, 
				(SOCKADDR*)&loClientAddr, 
				&lnAddr_length, 
				NULL, 
				0);

			if (INVALID_SOCKET == loClient)
			{
				continue;
			}
			std::vector<CHAR> loCommand(5);


			CString lstrData;


			CString lstrHandleInfo;

			std::vector<NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> loHandles;
			GetHandlesVo(loHandles);
			std::map<int,int> loHandleTypeStatistics;
			std::map<int ,CString> loHandleTypeNameMap;
			for (int i=0;i<loHandles.size();i++)
			{

				NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX * lpHandleInfo = &loHandles[i];
				CString lstrTemp;
				CString lstrObjectTypeName;
				std::map<int ,CString>::iterator lpTypeName= loHandleTypeNameMap.find(lpHandleInfo->ObjectTypeNumber);
				if (lpTypeName== loHandleTypeNameMap.end())
				{
					lstrObjectTypeName = GetHandleTypeName((HANDLE)lpHandleInfo->Handle);
					if (!lstrObjectTypeName.IsEmpty())
					{
						loHandleTypeNameMap[lpHandleInfo->ObjectTypeNumber] = lstrObjectTypeName;
					}

				}else
				{
					lstrObjectTypeName = lpTypeName->second;
				}
				lstrTemp.Format(_T("Handle:%08x | Type:%d | TypeName:%s \r\n"),lpHandleInfo->Handle,lpHandleInfo->ObjectTypeNumber,lstrObjectTypeName);
				lstrHandleInfo.Append(lstrTemp);
				loHandleTypeStatistics[lpHandleInfo->ObjectTypeNumber]++;
			}
			CString lstrHandleStatistics;
			lstrHandleStatistics.Append(lstrDataWriteBack);
			lstrHandleStatistics.Append("\r\n-------------------------------------2. Handle Statistics ---------------------------------\r\n");
			CString lstrTotal;
			lstrTotal.Format(_T("Total handle count %d"),loHandles.size());
			for (std::map<int,int>::iterator lpIt = loHandleTypeStatistics.begin();lpIt!=loHandleTypeStatistics.end();lpIt++)
			{
				std::map<int ,CString>::iterator lpTypeName= loHandleTypeNameMap.find(lpIt->first);
				CString lstrTypeName = _T("Unkown");
				if (lpTypeName!= loHandleTypeNameMap.end())
				{
					lstrTypeName = lpTypeName->second;
				}
				CString lstrTemp;
				lstrTemp.Format(_T("Type:%d Count:%d TypeName:%s\r\n"),lpIt->first,lpIt->second,lstrTypeName);
				lstrHandleStatistics.Append(lstrTemp);
			}
			lstrHandleStatistics.Append("\r\n-------------------------------------3. Handle detail ---------------------------------\r\n");
			lstrHandleStatistics.Append(lstrHandleInfo);

			lstrDataLength.Format(lstrDataLengthFormat,lstrHandleStatistics.GetLength());

			CString lstrHttpData;
			lstrHttpData.Append(lstrHttpHeader);
			lstrHttpData.Append(lstrDataLength);
			lstrHttpData.Append(_T("\r\n"));
			lstrHttpData.Append(_T("\r\n"));
			lstrHttpData.Append(lstrHandleStatistics);
			send(loClient,lstrHttpData.GetBuffer(0),lstrHttpData.GetLength(),0);
			Sleep(1000);
			shutdown(loClient,SD_BOTH);
			closesocket(loClient);
		}
		g_bServerStarted = FALSE;

		return 0;
	}


	std::map<CString,LY::PLANTINFO_STATUS> m_oPlantStatus;
	BOOL m_bRegenerateForm;
	virtual int UpdateStatistics(LY::PLANTINFO_STATUS & arefStatus)
	{
		g_pLock->Lock();
		BEGIN_ERROR_HANDLE;

		CString lstrKey = arefStatus.m_strGroup
			+arefStatus.m_strCompany
			+arefStatus.m_strFactory
			+arefStatus.m_strSet
			+arefStatus.m_strPlant;
		std::map<CString,LY::PLANTINFO_STATUS>::iterator lpFind = this->m_oPlantStatus.find(lstrKey);
		if (lpFind== this->m_oPlantStatus.end())
		{
			m_oPlantStatus[lstrKey] = arefStatus;
		}else
		{
			if (!arefStatus.m_strDynSaveTime.IsEmpty())
			{
				lpFind->second.m_strDynSaveTime = arefStatus.m_strDynSaveTime;
			}

			if (!arefStatus.m_strDynUpdateTime.IsEmpty())
			{
				lpFind->second.m_strDynUpdateTime = arefStatus.m_strDynUpdateTime;
			}

			if (!arefStatus.m_strVibSaveTime.IsEmpty())
			{
				lpFind->second.m_strVibSaveTime = arefStatus.m_strVibSaveTime;
			}

			if (!arefStatus.m_strVibUpdateTime.IsEmpty())
			{
				lpFind->second.m_strVibUpdateTime = arefStatus.m_strVibUpdateTime;
			}

			if (!arefStatus.m_strProcUpdateTime.IsEmpty())
			{
				lpFind->second.m_strProcUpdateTime = arefStatus.m_strProcUpdateTime;
			}

			if (!arefStatus.m_strProcSaveTime.IsEmpty())
			{
				lpFind->second.m_strProcSaveTime = arefStatus.m_strProcSaveTime;
			}


		}
		
		this->m_bRegenerateForm = TRUE;
		END_ERROR_HANDLE;
		g_pLock->Unlock();
		return 0;
	}
	
	CString m_strFormData;
	CString GetStatsForm(int anRtLimit =0,int anHisLimit =0,int anDumpProc = 0)
	{
		if (anRtLimit<0)
		{
			anRtLimit = 0;
		}

		if (anHisLimit <0)
		{
			anHisLimit = 0;
		}
		CString lstrData;
		g_pLock->Lock();
		//if (m_bRegenerateForm)
		{
			CString lstrHeaderFormat = "HTTP/1.1 200 OK\r\nCache-Control: private\r\nContent-Type: text/html; charset=gb2312\r\nVary: Accept-Encoding\r\nServer: Microsoft-IIS/7.5\r\nContent-Length: %d\r\nConnection: close\r\n\r\n";
			CString lstrHeader;
		
			m_strFormData =		"<html><head>";
			m_strFormData += "<!-- CSS goes in the document HEAD or added to your external stylesheet -->\
								<style type=\"text/css\">\
								table.imagetable {\
									font-family: verdana,arial,sans-serif;\
									font-size:11px;\
				color:#333333;\
									border-width: 1px;\
									border-color: #999999;\
									border-collapse: collapse;\
								}\
								table.imagetable th {\
				background:#b5cfd2 url('cell-blue.jpg');\
									border-width: 1px;\
				padding: 8px;\
									border-style: solid;\
									border-color: #999999;\
								}\
								table.imagetable td {\
				background:#dcddc0 url('cell-grey.jpg');\
									border-width: 1px;\
				padding: 8px;\
									border-style: solid;\
									border-color: #999999;\
								}\
								</style></head><body>";

			m_strFormData += "<table class=\"imagetable\">\
									<tr>\
									<th>序号</th><th>保存错误</th><th>实时错误</th><th>公司</th><th>分厂</th><th>设备</th><th>实时振动数据更新时间</th><th>实时动态数据更新时间</th><th>实时过程量数据更新时间</th><th>振动数据上次保存时间</th><th>动态数据上次保存时间</th><th>过程量数据上次保存时间</th>\
									</tr>";
			std::map<CString,LY::PLANTINFO_STATUS>::iterator lpIt = this->m_oPlantStatus.begin();
			USES_CONVERSION;
			CBHDateTime loDateTimeNow = CBHDateTime::GetCurrentTimeEx();
			COleDateTimeSpan loDateSpan = COleDateTimeSpan(0,0,0,anRtLimit);
			COleDateTimeSpan loDateSpanHis = COleDateTimeSpan(0,0,0,anHisLimit);
			CString lstrNoData = _T("没有更新");
			CString lstrNoNeedData = _T("没有测点");
			int lnCount = 0;
			for (lpIt=this->m_oPlantStatus.begin();lpIt!=this->m_oPlantStatus.end();lpIt++)
			{
				BOOL lbShouldDump = FALSE;
				BOOL lbHist = FALSE;
				BOOL lbRt = FALSE;
				if (anHisLimit>0 || anRtLimit>0)
				{
					if (anHisLimit>0)
					{
						CBHDateTime loDateTimeTest;
						COleDateTimeSpan loDiff;


						if (lpIt->second.m_nVibChannCount>0&& (!lbShouldDump))
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strVibSaveTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpanHis  )
							{
								lbShouldDump = TRUE;
								lbHist = TRUE;
							}
						}


						if (lpIt->second.m_nDynChannCount>0&& (!lbShouldDump))
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strDynSaveTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpanHis  )
							{
								lbShouldDump = TRUE;
								lbHist = TRUE;
							}
						}

						if (lpIt->second.m_nProcChannCount>0 && (!lbShouldDump) && (anDumpProc))
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strProcSaveTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpanHis  )
							{
								lbShouldDump = TRUE;
								lbHist = TRUE;
							}
						}
					}//if (anHisLimit>0)


					if (anRtLimit>0)
					{
						CBHDateTime loDateTimeTest;
						COleDateTimeSpan loDiff;


						if (lpIt->second.m_nVibChannCount>0)
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strVibUpdateTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpan  )
							{
								lbShouldDump = TRUE;
								lbRt = TRUE;
							}
						}


						if (lpIt->second.m_nDynChannCount>0)
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strDynUpdateTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpan  )
							{
								lbShouldDump = TRUE;
								lbRt = TRUE;
							}
						}

						if (lpIt->second.m_nProcChannCount>0  && (anDumpProc))
						{
							loDateTimeTest.ParseDateTime(lpIt->second.m_strProcUpdateTime);
							loDiff = loDateTimeNow - loDateTimeTest ;
							if (loDiff> loDateSpan  )
							{
								lbShouldDump = TRUE;
								lbRt = TRUE;
							}
						}
					}//if (anHisLimit>0)

				}else//if (anHisLimit>0 || anRtLimit>0)
				{
					lbShouldDump = TRUE;
				}

				if (!lbShouldDump)
				{
					continue;
				}
				
				CString lstrPerLine;
				lstrPerLine += "<tr>\r\n";

				CString lstrNo;
				lstrNo.Format(_T("%d"),++lnCount);

				lstrPerLine += "<td >";
				lstrPerLine += lstrNo;
				lstrPerLine += "</td>";

				if (lbHist)
				{
					lstrPerLine += "<td style='background:#ff00c0'>";
					lstrPerLine +=  "是";
					lstrPerLine += "</td>";
				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine +=  "否";
					lstrPerLine += "</td>";
				}

				if (lbRt)
				{
					lstrPerLine += "<td style='background:#ffc0c0'>";
					lstrPerLine +=  "是";
					lstrPerLine += "</td>";
				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine +=  "否";
					lstrPerLine += "</td>";
				}

				lstrPerLine += "<td>";
				lstrPerLine +=  lpIt->second.m_strCompany;
				lstrPerLine += "</td>";

				lstrPerLine += "<td>";
				lstrPerLine += lpIt->second.m_strFactory;
				lstrPerLine += "</td>";

				lstrPerLine += "<td>";
				lstrPerLine += lpIt->second.m_strPlant;
				lstrPerLine += "</td>";

				if (lpIt->second.m_nVibChannCount>0)
				{
					if (lpIt->second.m_strVibUpdateTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strVibUpdateTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}


				if (lpIt->second.m_nDynChannCount>0)
				{
					if (lpIt->second.m_strDynUpdateTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strDynUpdateTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}


				if (lpIt->second.m_nProcChannCount>0)
				{
					if (lpIt->second.m_strProcUpdateTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strProcUpdateTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}


				if (lpIt->second.m_nVibChannCount>0)
				{
					if (lpIt->second.m_strVibSaveTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strVibSaveTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}


				if (lpIt->second.m_nDynChannCount>0)
				{
					if (lpIt->second.m_strDynSaveTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strDynSaveTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}


				if (lpIt->second.m_nProcChannCount>0)
				{
					if (lpIt->second.m_strProcSaveTime.CompareNoCase("1970-01-01 01:01:01.001"))
					{
						lstrPerLine += "<td>";
						lstrPerLine += lpIt->second.m_strProcSaveTime;
						lstrPerLine += "</td>";
					}else
					{
						lstrPerLine += "<td>";
						lstrPerLine += lstrNoData;
						lstrPerLine += "</td>";
					}

				}else
				{
					lstrPerLine += "<td>";
					lstrPerLine += lstrNoNeedData;
					lstrPerLine += "</td>";
				}		

				lstrPerLine += "</tr>\r\n";

				m_strFormData += lstrPerLine;
			}
			m_strFormData +=  "</body>\
				</table>   ";
			m_strFormData +=  "</body>\
				</html>   ";

			lstrHeader.Format(lstrHeaderFormat,m_strFormData.GetLength());
			m_strFormData = lstrHeader + m_strFormData;

		}
		
		lstrData  = m_strFormData;

		g_pLock->Unlock();

		return lstrData;
	}

	static BOOL  IsPortAvailable(int anPort)
	{

		unsigned short lusPortTest = anPort;

		long lSockTest = (long)socket(AF_INET, SOCK_STREAM, 0);

		if(0 > lSockTest)
		{
			return FALSE;
		}

		//绑定本地地址
		struct sockaddr_in  serverAddr;
		memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));     
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = ::inet_addr(_T("127.0.0.1"));
		serverAddr.sin_port = htons(lusPortTest);

		if (0 > bind ((SOCKET)lSockTest, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
		{
			(void)closesocket((UINT)lSockTest);

			return FALSE;
		}

		(void)closesocket((UINT)lSockTest);

		return TRUE;
	}

	static BOOL DetectAvailablePort(int & anPort)
	{
		if (IsPortAvailable(anPort))
		{
			return TRUE;
		}

		for (int i =25000;i<26000;i++)
		{
			if (IsPortAvailable(i))
			{
				anPort = i;

				return TRUE;
			}
		}

		return FALSE;
	}

	static int on_headers_complete(http_parser* apParser)
	{
		if (NULL == apParser)
		{
			return S_FALSE;
		}

		return S_OK;
	}

	static int on_message_complete(http_parser* apParser)
	{
		if (NULL == apParser)
		{
			return S_FALSE;
		}

		return S_OK;
	}
	
	static int on_url(http_parser* apParser, const char *at, size_t length)
	{
		if (NULL == apParser)
		{
			return S_FALSE;
		}
		BOOL lbIsHead = FALSE;
		
		if (apParser->method == HTTP_HEAD)
		{
			lbIsHead = TRUE;
		}
		

		http_parser_url  loUrlParser = {0};	
		http_parser_url * u = &loUrlParser;
		http_parser_parse_url(at,length,lbIsHead,&loUrlParser);
		
		char part[512] = {0};
		int i=0;
		for (i = 0; i < UF_MAX; i++) 
		{
			if (u->field_set & (1 << i)) {
				memcpy(part, at + u->field_data[i].off,
					u->field_data[i].len);
				part[u->field_data[i].len] = '\0';
			} else
				part[0] = '\0';

			TRACE("\tfield %u: off: %u len: %u part: \"%s\"\n", i,
				u->field_data[i].off, u->field_data[i].len, part);
		}
		int lnRtLimit = 0;
		int lnHisLimit = 0;
		int lnDumpProc = 0;
		//path
		if (u->field_set & (1<<UF_QUERY))
		{
			memcpy(part, at + u->field_data[UF_QUERY].off,
				u->field_data[UF_QUERY].len);
			part[u->field_data[UF_QUERY].len] = '\0';
			CString lstrQuery = CUtility::UrlDecode(part);

			CStringArray loParameters;
			CString lstrSplit = _T("&");
			CString lstrSplit2 = _T("=");
			CUtility::GetSplitStrArray(lstrQuery,lstrSplit,loParameters);

			for (int i=0;i<loParameters.GetCount();i++)
			{
				CString lstrPerParameter = loParameters.GetAt(i);
				CStringArray loPerParameters;
				CUtility::GetSplitStrArray(lstrPerParameter,lstrSplit2,loPerParameters);
				if (loPerParameters.GetCount()>1)
				{
					if (loPerParameters.GetAt(0).CompareNoCase("rt")==0)
					{
						lnRtLimit = _ttoi(loPerParameters.GetAt(1));
					}else if (loPerParameters.GetAt(0).CompareNoCase("his")==0)
					{
						lnHisLimit = _ttoi(loPerParameters.GetAt(1));
					}else if (loPerParameters.GetAt(0).CompareNoCase("proc"))
					{
						lnDumpProc = 1;
					}
				}

				

			}		 
			
		}//if (u->field_set && (1<<UF_QUERY))

		CLIENT_INFO * lpClient = (CLIENT_INFO *)apParser->data;

		if (NULL!=lpClient)
		{
			if (NULL!= lpClient->m_pThis)
			{
				CString lstrWriteBack = lpClient->m_pThis->GetStatsForm(lnRtLimit,lnHisLimit,lnDumpProc);
				SOCKET loClient = lpClient->m_oClient;
				send(loClient,lstrWriteBack.GetBuffer(0),lstrWriteBack.GetLength(),0);
			}
		}
	
		return S_OK;
	}

	static int on_header_value(http_parser* apParser, const char *at, size_t length)
	{
		if (NULL == apParser)
		{
			return S_FALSE;
		}

		return S_OK;
	}

	static int on_header_field(http_parser* apParser, const char *at, size_t length)
	{
		if (NULL == apParser)
		{
			return S_FALSE;
		}

		return S_OK;
	}


	static UINT ClientProcForStats(LPVOID apData)
	{
		if (NULL == apData )
		{
			return 0;
		}

		CLIENT_INFO loInfo;

		CLIENT_INFO * lpClient = (CLIENT_INFO *)apData;
		if (lpClient->m_pThis == NULL)
		{
			return 0;
		}
		g_pLock->Lock();
		loInfo = *lpClient;
		delete lpClient;
		g_pLock->Unlock();
		std::vector<CHAR> loBuffer(40960);
		int lnReadCount = recv(loInfo.m_oClient,&loBuffer.front(),loBuffer.size(),0);
		if(lnReadCount< 0)  
		{
			closesocket(loInfo.m_oClient);
			return 0;
		}
	

		ServerStatusImp * lpThis =loInfo.m_pThis;
		CString	lstrDataWriteBack = lpThis->GetStatsForm();


		http_parser_settings settings= {0};
		settings.on_url = ServerStatusImp::on_url;
		settings.on_header_field = ServerStatusImp::on_header_field;
		settings.on_headers_complete = ServerStatusImp::on_headers_complete;
		settings.on_message_complete = ServerStatusImp::on_message_complete;
		settings.on_header_value =  ServerStatusImp::on_header_value;
		settings.on_header_field =  ServerStatusImp::on_header_field;
	

		http_parser loParser ={0} ;
		http_parser * lpParser = &loParser;
		http_parser_init(lpParser, HTTP_REQUEST);
		loParser.data = (void *)&loInfo;
		int parsed = http_parser_execute(
			lpParser, &settings, &loBuffer.front(), lnReadCount);
		send(loInfo.m_oClient,loInfo.m_strData.GetBuffer(0),loInfo.m_strData.GetLength(),0);
		closesocket(loInfo.m_oClient);

	
	}

	static UINT ServerProcForStats(LPVOID apData)
	{
		if (NULL == apData)
		{
			return 0;
		}
		g_pLock->Lock();
		g_bServerStarted = TRUE;
		g_pLock->Unlock();

		WSADATA wsa;

		SOCKET s;
		SOCKADDR_IN sAddr;

		int port;


		STARTUPINFO si;
		ServerStatusImp * lpThis = (ServerStatusImp* )apData;
		port = lpThis->m_nServerStatusPort;//Set listening port

		DetectAvailablePort(port);

		memset( &si, 0, sizeof( si ) );
		si.cb = sizeof( si );
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

		sAddr.sin_addr.s_addr = INADDR_ANY;
		sAddr.sin_port =  (port >> 8) | (port << 8);
		sAddr.sin_family = AF_INET;

		WSAStartup( 0x0202, &wsa );

		s = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
		int lnRet = bind( s, (LPSOCKADDR)&sAddr, sizeof( sAddr ) );

		if (SOCKET_ERROR == lnRet)
		{
			DWORD ldwError = GetLastError();
			
			return -1;
		}

		lnRet = listen( s, 5 );

		if (SOCKET_ERROR == lnRet)
		{
			return -1;
		}

		CString lstrDataWriteBack;

		

		while(TRUE)
		{
			SOCKADDR_IN loClientAddr ={0}; // 定义一个客户端得地址结构作为参数  

			int lnAddr_length=sizeof(loClientAddr);  

			//closesocket(loClient);

			BHLOG_DEBUG(STR(_T("Server Port: %d"),port));

			SOCKET loClient = WSAAccept(	s, 
				(SOCKADDR*)&loClientAddr, 
				&lnAddr_length, 
				NULL, 
				0);

			if (INVALID_SOCKET == loClient)
			{
				continue;
			}
			std::vector<CHAR> loCommand(5);


	
			CLIENT_INFO * lpInfo = new CLIENT_INFO();
			lpInfo->m_oClient = loClient;
			lpInfo->m_strData = lstrDataWriteBack;
			lpInfo->m_pThis = lpThis;
			CWinThread * lpThread = AfxBeginThread(ClientProcForStats,lpInfo,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,0);
			
			if (NULL!=lpThread)
			{
				lpThread->m_bAutoDelete = TRUE;
				lpThread->ResumeThread();
				//CloseHandle(lpThread->m_hThread);
			}
		}

		g_bServerStarted = FALSE;

		return 0;
	}
	BOOL m_bServerComplete ;
	 int IsServerStartupComplete()
	{
		if (this->m_bServerComplete)
		{
			return TRUE;

		}
		BOOL lbServerStartFinished = TRUE;
		g_pLock->Lock();
		std::map<CString,int>::iterator lpIt = m_oModules.begin();
		for (lpIt=m_oModules.begin();lpIt!=m_oModules.end();lpIt++)
		{
			if (lpIt->second !=1)
			{
				lbServerStartFinished = FALSE;
				break;
			}
		}
		this->m_bServerComplete = lbServerStartFinished;
		g_pLock->Unlock();

		return lbServerStartFinished;
	}
	 int RegistModuleComplete(char * apModuleName,int anModuleNamelength)
	{
		//1.parse parameter
		if (NULL == apModuleName || anModuleNamelength <=0 || anModuleNamelength>=1024)
		{
			return FALSE;
		}
		char lpBuffer[1024] = {0};
		strncpy(lpBuffer,apModuleName,anModuleNamelength);
		CString lstrModuleName =lpBuffer;
		lstrModuleName = lstrModuleName.MakeLower();
			
		//2.regist Module
		BOOL lbServerStartFinished = TRUE;
		g_pLock->Lock();
		BEGIN_ERROR_HANDLE;
		
		std::map<CString,int>::iterator lpIt = m_oModules.find(lstrModuleName);
		if (lpIt == m_oModules.end())
		{
			g_pLock->Unlock();
			return FALSE;
		}

		m_oModules[lstrModuleName] = 1; //complete
	
		END_ERROR_HANDLE;
		g_pLock->Unlock();

		//2. start server
		if (this->IsServerStartupComplete())
		{
			this->StartServerStatusProc();
		}
		return TRUE;
	};

	void StartServerStatusProc()
	{
		BOOL lbShouldStartServer = FALSE;
		g_pLock->Lock();
		if (!g_bServerStarted)
		{
			lbShouldStartServer = TRUE;
		}		
		g_pLock->Unlock();

		if (lbShouldStartServer)
		{


			CWinThread* lpThread = AfxBeginThread(ServerStatusImp::ServerProcForStats,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			lpThread->m_bAutoDelete = TRUE;
			lpThread->ResumeThread();

			lpThread = AfxBeginThread(ServerStatusImp::ServerProcForInfo,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			lpThread->m_bAutoDelete = TRUE;
			lpThread->ResumeThread();

		}

	}



};
BHXZ_API LY::IServerStatus * GetServerStatus()
{
	if (NULL == g_pLock)
	{
		g_pLock = new CCriticalSection();
	}
	g_pLock->Lock();
	if (NULL ==  g_pServer)
	{
		g_pServer = new ServerStatusImp();
	}
	g_pLock->Unlock();

	return g_pServer;
}



