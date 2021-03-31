/**
* @file ACOImpl.cpp
* @brief Advanced Communication Object
* @author Xiaoguang Sun
*
* $LastChangedRevision: 108 $
*
* $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/src/ACOImpl.cpp $
*
* $LastChangedBy: solaris $
*
* Created:     Jul. 15, 2004
*
* $LastChangedDate: 2008-02-23 14:04:09 +0800 (Sat, 23 Feb 2008) $
*/

#include "aco/SrvDef.h"
#include "aco/ACOImpl.h"
#include "aco/debug.h"
#include <aco/TDateTime.h>
#include <Ice/Ice.h>

#ifdef WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

#define  D_ProtocolSign "ICEP"


static void aco_init(){
	

}

static void aco_fini(){
	
}

#ifdef WIN32
#include <windows.h>
BOOL WINAPI DllMain(HINSTANCE hinstDLL,
					DWORD fdwReason,
					LPVOID lpvReserved){

						switch(fdwReason)
						{
						case DLL_PROCESS_ATTACH:
							//TTrace::DebugEx(100,"Attach errorno:%d",errno);
							aco_init();
							//TTrace::DebugEx(100,"AttachEnd errorno:%d",errno);
							break;
						case DLL_PROCESS_DETACH:
							aco_fini();
							//TTrace::DebugEx(100,"Detach");
							break;
						default:
							break;
						}

						return TRUE;
					}
#else
__attribute__((constructor)) void __aco_constructor(){
	aco_init();
}

__attribute__((destructor)) void __aco_destructor(){
	aco_fini();
}
#endif

bool ACOImpl::Absolute(const unsigned int row){
	__current_row= _dataPacket.packet.begin();
	__end_row=  _dataPacket.packet.end();
	for(unsigned int rowindex=0;
		rowindex<row&&__current_row!=__end_row;++rowindex){
			// attention please: 
			// null loop will be removed by the compiler's optimization.
			// so the next line must not be moved into for's sub clause.
			++__current_row; 
		}

		return true;
}

void ACOImpl::AfterLast(){
	__current_row=_dataPacket.packet.end();
}

void ACOImpl::ClearWarnings(){
	_dataPacket.curWarnings.clear();
}

bool ACOImpl::First(){
	__current_row=_dataPacket.packet.begin();
	return true;
}


bool ACOImpl::GetBoolean(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode, FTBool) < 0)
			return false;
		else
		{
			SFieldContent& fc = (*__current_row)[nFieldCode];
			return fc.iValue>0 ? true:false;
		}
	}
	catch(std::exception& ex){
		return false;
	}
	catch(...){
		return 0;
	}
}

char ACOImpl::GetByte(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode,FTChar) < 0)
			return 0;
		else
		{
			const SFieldContent& sfc = (SFieldContent)((*__current_row)[nFieldCode]);
			if (sfc.buf.size()>0)
			   return sfc.buf[0];
			return 0;
		}
	}
	catch(std::exception& ex){
		return 0;
	}
	catch(...){
		return 0;
	}
}

std::vector<unsigned char> ACOImpl::GetBytesEx(const unsigned int nFieldCode)
{
	try
	{	
		if (checkvariantrowfield(__current_row, nFieldCode,FTBytes) < 0)
			return std::vector<unsigned char>();
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.buf;
		}
	}
	catch(std::exception& ex){
		return std::vector<unsigned char>();
	}
	catch(...){
		return std::vector<unsigned char>();
	}
}

std::vector<char> ACOImpl::GetBytes(const unsigned int nFieldCode)
{
	vector<unsigned char> vBuf =  GetBytesEx(nFieldCode);
	vector<char> vChar;
	vChar.assign(vBuf.size(),0);
	for (int ind = 0; ind < vBuf.size(); ++ind)
	   vChar[ind] = (char)vBuf[ind];	
    return vChar;
}

std::string ACOImpl::GetDate(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode, FTDateTime) < 0)
		{
           if (checkvariantrowfield(__current_row, nFieldCode, FTString) < 0)
			   return std::string();
		}
		SFieldContent& fc =(*__current_row)[nFieldCode];
		return fc.strValue;
	}
	catch(std::exception& ex){
		return std::string();
	}
	catch(...){
		return std::string();
	}
}

tm ACOImpl::GetDateTime(const unsigned int nFieldCode){
	string str = GetDate(nFieldCode);
	return ParseDateTime(str.c_str());	
}

double ACOImpl::GetDouble(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode, FTFloat) < 0)
			return .0;
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.fValue;
		}
	}
	catch(std::exception& ex){
		return .0;
	}
	catch(...){
		return 0;
	}
}

float ACOImpl::GetFloat(const unsigned int nFieldCode){
	try{
		if (checkvariantrowfield(__current_row, nFieldCode, FTFloat) < 0)
			return .0;
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.fValue;
		}
	}
	catch(std::exception& ex){
		return .0;
	}
	catch(...){
		return 0;
	}
}


int ACOImpl::GetInt(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode,FTInt) < 0)
			return 0;
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.iValue;
		}
	}
	catch(std::exception& ex){
		//TRACE("%s,%s start %d ex\n", __FUNCTION__, __FILE__, nFieldCode);
		return 0;
	}
	catch(...){
		//TRACE("%s,%s start %d ex...\n", __FUNCTION__, __FILE__, nFieldCode);
		return 0;
	}
}

unsigned int ACOImpl::GetRow(){
	return std::distance(__current_row,_dataPacket.packet.begin())+1;
}

short ACOImpl::GetShort(const unsigned int nFieldCode){
	try{	  
		if (checkvariantrowfield(__current_row, nFieldCode,FTInt) < 0)
			return 0;
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.fValue;
		}
	}
	catch(std::exception& ex){
		return 0;
	}
	catch(...){
		return 0;
	}
}

std::string ACOImpl::GetString(const unsigned int nFieldCode){
	try{	
		if (checkvariantrowfield(__current_row, nFieldCode,FTString) < 0)
			return std::string();
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.strValue;
		}
	}
	catch(std::exception& ex){
		TRACE("%s,%s start %d ex\n", __FUNCTION__, __FILE__, nFieldCode);
		return std::string();
	}
	catch(...){
		TRACE("%s,%s start %d ...\n", __FUNCTION__, __FILE__, nFieldCode);
		return std::string();
	}
}

unsigned int ACOImpl::GetUInt(const unsigned int nFieldCode){
	try{
		if (checkvariantrowfield(__current_row, nFieldCode,FTInt) < 0)
			return 0;
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			return fc.iValue;
		}
	}
	catch(std::exception& ex){
		return 0;
	}
	catch(...){
		return 0;
	}
}

std::list<std::string> ACOImpl::GetWarnings(){
	list<string> warnings;
	for (int ind = 0; ind < _dataPacket.curWarnings.size(); ++ind)
		warnings.push_back(_dataPacket.curWarnings[ind]);
	return warnings;
}
void ACOImpl::InsertRow(){
            InsertRow(1);
}

void ACOImpl::InsertRow(const unsigned int rowsize){
	//variantrow row(rowsize);
	variantrow row;
	_dataPacket.packet.push_back(row);
	__current_row = __end_row = _dataPacket.packet.end();
	--__current_row;
}

bool ACOImpl::IsAfterLast(){
	return __current_row==_dataPacket.packet.end();
}

bool ACOImpl::IsBeforeFirst(){
	// There is no before first conception in the stl's container
	return true;
}

bool ACOImpl::IsFirst(){
	return __current_row==_dataPacket.packet.begin();
}

bool ACOImpl::IsLast(){
	if(_dataPacket.packet.size()==0)
		return true;

	__end_row=_dataPacket.packet.end();
	bool result=__current_row==--__end_row;
	return result;
}

bool ACOImpl::Last(){
	__current_row=_dataPacket.packet.end();
	--__current_row;
	return true;
}

bool ACOImpl::Next(){
	if(IsLast())
		return false;
	++__current_row;
	return true;
}

bool ACOImpl::Previous(){
	if(IsFirst())
		return false;
	--__current_row;
	return true;
}

bool ACOImpl::Relative(const int rows){
	if(rows<0){
		cursor __first_row=_dataPacket.packet.begin();
		for(int offset=0;offset>rows&&__current_row!=__first_row;--offset)
			--__current_row;
	}
	else{
		__end_row=_dataPacket.packet.end();
		for(unsigned int offset=0;offset<rows&&__current_row!=__end_row;++offset)
			++__current_row;
	}
	return true;
}

void ACOImpl::SetBoolean(const unsigned int nFieldCode,const bool x){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	SFieldContent sfc;
	sfc.fType = FTBool;
	sfc.iValue = x? 1:0;
	AddField(nFieldCode,sfc);
}

void ACOImpl::SetByte(const unsigned int nFieldCode,const unsigned char& x){
	SFieldContent sfc;
	sfc.fType = FTChar;
	sfc.buf.clear();
	sfc.buf.push_back(x);
	AddField(nFieldCode,sfc);
}

void ACOImpl::SetBytes(const unsigned int nFieldCode,const char* x,const unsigned int size){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	
// 	SFieldContent sfc;
// 	sfc.fType = FTBytes;
// 	sfc.buf.clear();
// 	sfc.buf.assign(size,0);
// 	for(unsigned int offset=0;offset<size;++offset)
// 		sfc.buf[offset]=x[offset];
// 	AddField(nFieldCode,sfc);
	if ((NULL == x )|| (size<=0))
	{
		return;
	}	
	SFieldContent & arefSf = (*__current_row)[nFieldCode] ;
	arefSf.buf.clear();
	arefSf.buf.assign(size,0);
	arefSf.fType = FTBytes;
	unsigned char * lpBuffer = &(arefSf.buf.front());
	memcpy(lpBuffer,x,size);
}

void ACOImpl::SetDateTime(const unsigned int nFieldCode,const tm& x_){
 //格式为：2010-8-9 10:11:2
	char cTime[100]={0};
	sprintf(cTime,("%d-%d-%d %d:%d:%d"),x_.tm_year,x_.tm_mon,x_.tm_mday,x_.tm_hour,x_.tm_min,x_.tm_sec);
    SetDateTime(nFieldCode,cTime);
}

void ACOImpl::SetDateTime(const unsigned int nFieldCode,const char* x){
	SFieldContent sfc;
	sfc.fType = FTDateTime;
	sfc.strValue = x;
	AddField(nFieldCode,sfc);
}


void ACOImpl::AddField(const unsigned int nFieldCode,const SFieldContent& sfc_)
{
	(*__current_row)[nFieldCode] = sfc_;
}

void ACOImpl::SetDate(const unsigned int nFieldCode,const char* x){

	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	SetDateTime(nFieldCode,x);
}

void ACOImpl::SetDouble(const unsigned int nFieldCode,const double x){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	float f1 = x;
	SetFloat(nFieldCode,x);
}

void ACOImpl::SetFloat(const unsigned int nFieldCode,const float x){
	SFieldContent sfc;
	sfc.fType = FTFloat;
	sfc.fValue = x;
	AddField(nFieldCode,sfc);
}

void ACOImpl::SetInt(const unsigned int nFieldCode,const int x){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	SFieldContent sfc;
	sfc.fType = FTInt;
	sfc.iValue = x;
	AddField(nFieldCode,sfc);
}

void ACOImpl::SetShort(const unsigned int nFieldCode,const short x){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	int i1 = x;
	SetInt(nFieldCode,i1);
}

void ACOImpl::SetString(const unsigned int nFieldCode,const char* x){
	// if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	SFieldContent sfc;
	sfc.fType = FTString;
	sfc.strValue = x;
	AddField(nFieldCode,sfc);
}


void ACOImpl::SetUInt(const unsigned int nFieldCode,const unsigned int x){
	//  if(__current_row->size()<nFieldCode+1)
	//   __current_row->resize(nFieldCode+1);
	int i1 =x;
    SetInt(nFieldCode,i1);
}

void ACOImpl::SetWarnings(const std::list<std::string>& warnings){
	_dataPacket.curWarnings.clear();
	_dataPacket.curWarnings.insert(_dataPacket.curWarnings.begin(),warnings.begin(),warnings.end());
}

void ACOImpl::AddWarning(const std::string& warning){
	_dataPacket.curWarnings.push_back(warning);
}

bool ACOImpl::HasField(const unsigned int nFieldCode,const int nRowIndex){
	cursor __cursor_tmp=__current_row;
	Absolute(nRowIndex);
	bool result=HasField(nFieldCode);
	__current_row=__cursor_tmp;
	return result;
}

bool ACOImpl::HasField(const unsigned int nFieldCode){
	return __current_row->find(nFieldCode) != __current_row->end();
}

userprofile ACOImpl::GetUserProfile(){
	return __user_profile;
}

void ACOImpl::SetUserProfile(const userprofile& userprofile){
	__user_profile=userprofile;
	_dataPacket.msgHeader.nVersion=userprofile.version;
}

unsigned int ACOImpl::GetServiceCode(){
	return _dataPacket.msgHeader.nServiceCode;
}

void ACOImpl::SetServiceCode(const unsigned int code){
	_dataPacket.msgHeader.nServiceCode=code;
	Clear();
	ClearWarnings();
}

unsigned int ACOImpl::GetVersion(){
	return _dataPacket.msgHeader.nVersion;
}

void ACOImpl::SetVersion(const unsigned int version){
	_dataPacket.msgHeader.nVersion = version;
	//__sock_header.version=version;
	//__user_profile.version=version;
}

bool ACOImpl::Reconnect(unsigned short port,const char* address){
/*	__sock_stream.close();
//	ACE_Time_Value tm;tm.msec(10);
//	ACE_OS::sleep(tm);
//	ACE_SOCK_Connector conn;
//	memcpy(&__connector,&conn,sizeof( ACE_SOCK_Connector));
//	ACE_SOCK_Stream stream;
//	memcpy(&__sock_stream,&stream,sizeof( ACE_SOCK_Stream));
	__active_connection = false;
	__remote_addr.set(port,address);
	if(__connector.connect(__sock_stream,__remote_addr,&__timeout_value)<0)
	{

		return false;
	}
	__sock_stream.get_local_addr(__local_addr);
	if(__use_ssl){
		if(!SSLNegotiate()){
			__sock_stream.close();
			return false;
		}
	}
	if(!Login()){
		__sock_stream.close();
		return false;
	}
	__active_connection = true;*/
	return true;
}

bool ACOImpl::Connect(unsigned short port,const char* address){
	///marble modify 
//	TTrace::DebugEx(100,"%s start,%d", __FUNCTION__,errno); 
	__active_connection = false;
	if (__active_connection == true)
	{//判断是否需要重新连接		
		if ((_iPort!= port)||(strcmp( address, _sIPAddr.c_str()) != 0))
			Close();
		else
			return true;
	}
	_iPort = port;
	_sIPAddr = address;
	try
	{

		//request:tcp -h 192.168.1.113 -p 10000
		char conninfo[100]={0};
        sprintf(conninfo,"request:tcp -h %s -p %d",_sIPAddr.c_str(), _iPort);

		int argc = 0;
		Ice::InitializationData initData;
		initData.properties = Ice::createProperties();
		//initData.properties->load("config.client");
		initData.properties->setProperty("Request.Proxy",conninfo);


		ZeroMemory(conninfo,100*sizeof(char));
		sprintf(conninfo,"%d", 102400);
		initData.properties->setProperty("Ice.MessageSizeMax",conninfo);


		__communicator = Ice::initialize(argc, 0, initData);
		Ice::ObjectPrx obj = __communicator->propertyToProxy("Request.Proxy");
		RequestPrx proxy = RequestPrx::uncheckedCast(obj);

		Ice::ObjectPrx proxyObj = proxy->ice_twoway();
		proxyObj = proxyObj->ice_secure(false);
		proxyObj = proxyObj->ice_timeout(__timeout_value*1000);
		__connProxy = RequestPrx::checkedCast(proxyObj);
		if (__connProxy != NULL)
		{
			if(Login())
				__active_connection = true;
		}
		
	}
	catch(const IceUtil::Exception& ex)
	{
		this->Close();
		ostringstream ostr;
		ostr << ex;
		string s = ostr.str();
		__communicator = NULL;
		__connProxy = NULL;
		__active_connection = false;
	}
	return __active_connection;
}

bool ACOImpl::Close()
{
	__active_connection=false;

	Clear();
    
	try
	{
		if (__communicator != NULL)
			__communicator->destroy();
	}
	catch(const IceUtil::Exception&){

	}
	catch(...){

	}
	__communicator = NULL;
	__connProxy = NULL;

	__ret_code = ACO_RET_CODE_NO_ERROR;
	_dataPacket.msgHeader.bodylength = 0;
	_dataPacket.msgHeader.nServiceCode= 0;
	_dataPacket.msgHeader.sessionid = "ICEP";
	return true;
}

int ACOImpl::GetRetCode(){
	return _dataPacket.nRetCode;
}

void ACOImpl::SetRetCode(const int code){
	_dataPacket.nRetCode = code;
}

unsigned int ACOImpl::GetFieldCount(){
	if (__current_row == NULL)
	  return 0;	
	return __current_row->size();
}


unsigned int ACOImpl::GetFieldCount(const unsigned int row){
	cursor __cursor=_dataPacket.packet.begin();
	__end_row=_dataPacket.packet.end();
	for(unsigned int rowindex=0;
		rowindex<row&&__current_row!=__end_row;++rowindex){
			// attention please: 
			// null loop will be removed by the compiler's optimization.
			// so the next line must not be moved into for's sub clause.
			++__cursor; 
		}
	return __cursor->size();
}

unsigned int ACOImpl::GetRowCount(){
	return _dataPacket.packet.size();
}

ACOImpl::ACOImpl(bool ssl):
__active_connection(false)
{
	__timeout_value = 10;
	_dataPacket.msgHeader.bodylength = 0;
	_dataPacket.msgHeader.nServiceCode = 0;
	_dataPacket.msgHeader.nVersion = 0;
	_dataPacket.msgHeader.sessionid = D_ProtocolSign;
	__nReconnects = 0;
    __connProxy = NULL;

	//TTrace::DebugEx(100,"--------------%s errorno:%d",__FUNCTION__,errno);
}

ACOImpl::~ACOImpl(){
	if(__active_connection)
	{	
		Close();
	}
	Clear();
	//  ClearAllObjects();
}

bool ACOImpl::Send(bool bResend_ ){
	bool bSuc = Send();
	return bSuc;
}

bool ACOImpl::Send(){
	try{
		SDataPacket dp = _dataPacket;
		dp.msgHeader.nVersion = 0;
		dp.msgHeader.bodylength = GetDataPacketSize();
		_dataPacket = __connProxy->RequestData(dp);
	}
	catch(const IceUtil::Exception& ex)
	{
		handleException(ex);
		return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}
/*
bool ACOImpl::Send(const ACE_OutputCDR& header,const ACE_Message_Block& body){
	if(__use_ssl){
		SSL_write(__ssl_connection,
			header.begin()->rd_ptr(),
			header.total_length());

		SSL_write(__ssl_connection,
			body.rd_ptr(),
			body.total_length());
	}
	else{
		iovec iov[2];
		iov[0].iov_base=header.begin()->rd_ptr();
		iov[0].iov_len=header.total_length();
		iov[1].iov_base=body.rd_ptr();
		iov[1].iov_len=body.total_length();
		size_t ited = 0;
		int ii = __sock_stream.sendv_n(iov,2,&__timeout_value,&ited);
//		TRACE("send %d, %d\n",ii, ited);
//		TTrace::DebugEx(100,"%s send %d, %d\n",__FUNCTION__,ii, ited);
		if (ii < 0)
			return false;
	}
	return true;
}*/

void ACOImpl::SetTimeout(const int timeoutSec){
	__timeout_value = timeoutSec;
}

bool ACOImpl::Clear(){
//	TRACE("%s clear  %d \n", __FUNCTION__,__threadid());
	ClearWarnings();
	_dataPacket.packet.clear();
	__current_row=_dataPacket.packet.begin();
	SetRetCode(ACO_RET_CODE_NO_ERROR);
	return true;
}

#define SERVICE_SYSTEM_LOGIN 1000
bool ACOImpl::Login(){
	// prepare for login data;
    _dataPacket.msgHeader.sessionid = D_ProtocolSign;
	_dataPacket.msgHeader.bodylength  = 0;
	_dataPacket.msgHeader.nServiceCode = SERVICE_SYSTEM_LOGIN;
	if(!Send())
		return false;
	if(!Receive())
		return false;
    if (_dataPacket.nRetCode == 1)
       return true;
	return false;
}

bool ACOImpl::ReceiveHeader(){
	// change timeout policy

	//TTrace::DebugEx(100,"%s %d, errorno:%d",__FUNCTION__, rc, errno);
	return false;
}

bool ACOImpl::ReceiveBody(){
	
	return false;
}

bool ACOImpl::Receive(){
	__current_row = _dataPacket.packet.begin();
	return true;



	int ii = errno; 
	errno = 0;
	if(!ReceiveHeader())
	{
		ASSERT(FALSE);
		return false;
	}
	bool bRtn = ReceiveBody();
	if (!bRtn)
	{
		ASSERT(FALSE);
	}
	return bRtn;
}
std::string ACOImpl::GetRemoteIP() const{
	return _sIPAddr;
}

unsigned short ACOImpl::GetRemotePort() const{
	return _iPort;
}

int ACOImpl::variantrowclone(const variantrow& source_, variantrow& des_){
	int nCount = 0;
	des_.clear();
	for (variantrow::const_iterator it = source_.begin(); it != source_.end(); ++it){
               unsigned int field = it->first;
			   variantfield item = it->second;
			   des_.insert(make_pair(field,item));
			   ++nCount;
	}
	return nCount;
}

int ACOImpl::resultsetclone(const variantresultset& source_, variantresultset& des_){
	int nCount = 0;
    des_.clear();
	for (variantresultset::const_iterator it = source_.begin(); it != source_.end(); ++it) {
		variantrow row;
		variantrowclone(*it,row);
		des_.push_back(row);
		++nCount;
	}    
	return nCount;
}

int ACOImpl::GetResultset(SDataPacket& des_){
	des_ = _dataPacket;
	return des_.packet.size();
}

int ACOImpl::SetResultset(const SDataPacket& des_){
	Clear();
	//int nCount = resultsetclone(des_,_dataPacket.packet);
	_dataPacket = des_;
	__current_row  = __end_row = _dataPacket.packet.end();
	int nCount = _dataPacket.packet.size() ;
	if ( nCount > 0)
	 __current_row = _dataPacket.packet.begin();
	return nCount;
}

void ACOImpl::handleException(const IceUtil::Exception& e)
{
	try
	{
		e.ice_throw();
	}
	catch(const Ice::NoEndpointException&)
	{
		printf("The proxy does not support the current configuration\n");
	}
	catch(const IceUtil::Exception& ex)
	{
		ostringstream ostr;
		ostr << ex;
		string s = ostr.str();
		printf("\n%s\n",s.c_str());
	}
}

int ACOImpl::checkvariantrowfield(const cursor  row_, const unsigned int &nfield_,const EFieldType& ft_){
	int ii = 0;
	unsigned int isize = 0;
	try{
		if (row_ != NULL){
			ii = 1;
			isize = row_->size();
			if ( isize >0 && isize < 100000){
				for (variantrow::const_iterator it = row_->begin(); it != row_->end(); ++it){
					unsigned int fi = it->first;		
					if (fi == nfield_)
					{
						const SFieldContent& fc = it->second;
						if(fc.fType == ft_)
							return 0;
						else 
							return -1;
					}
				}
			}
			else
			{
          TRACE("%s,%s   %d ... %d,%d \n", __FUNCTION__, __FILE__, nfield_, ii, isize);
			}
		}
	/*	for (variantrow::const_iterator it = row_->begin(); it != row_->end(); ++it){
            ii = 2;            
			unsigned int fi = it->first;		
			ii = 3;
			if (fi == nfield_)
			   return 0;
		}*/
	}
	catch (...){
		TRACE("%s,%s  exception %d ... %d,%ud \n", __FUNCTION__, __FILE__, nfield_, ii, isize);
		return -1;
	}
	return -1;
}

unsigned int ACOImpl::GetDataPacketSize(){
	return GetDataPacketSize(_dataPacket);
}

unsigned int ACOImpl::GetDataPacketSize(const SDataPacket& dp_){
	unsigned int size = 0;
	
	///计算数据包的字节数
	int rows = dp_.packet.size();
	int rowsize = 0;
	const PackageDetail& pd = dp_.packet;
	for (int ind = 0; ind < rows; ++ind)
	{
		rowsize  = 0;
		const RowDetail& rd = pd[ind];
		for (RowDetail::const_iterator it = rd.begin(); it != rd.end(); ++it)
		{
			const SFieldContent& fd = it->second;
			//字段码字节+ SFieldContent字节
			rowsize += 4 + sizeof(EFieldType) + fd.strValue.size() + 4 + 4 + fd.buf.size();
		}
		size += rowsize;
	}
	//添加消息头的字节数
	size += sizeof(SMessageHeader);

    //添加消息头的警告信息的字节数
	for (vector<string>::const_iterator it = dp_.curWarnings.begin(); it!= dp_.curWarnings.end(); ++it){
		size += it->size();
	}

	//添加消息报中剩余的字节数
	size += 4;
	return size;
}

tm ACOImpl::ParseDateTime(const string& sDateTime_){
	tm tm1;
    tm1.tm_year = 2000;
	tm1.tm_mon = 1;
	tm1.tm_mday = 1;
	tm1.tm_hour = 1;
	tm1.tm_min = 1;
	tm1.tm_sec = 1;
	if (sDateTime_.size()< 12)
	   return tm1;	
    TDateTime tdt;
	if (tdt.ParseDateTimeString(sDateTime_.c_str()))
	{
		tm1.tm_year = tdt.sYear;
		tm1.tm_mon  = tdt.sMonth;
		tm1.tm_mday = tdt.sDay;
	}
	tm1.tm_hour = tdt.sHour;
	tm1.tm_min  = tdt.sMinute;
	tm1.tm_sec  = tdt.sSecond;		
//	TTrace::DebugEx(100,"time %s, parese:%d-%d-%d %d:%d:%d", sDateTime_.c_str(), tm1.tm_year,tm1.tm_mon, tm1.tm_mday, tm1.tm_hour,tm1.tm_min, tm1.tm_sec);
	return tm1;
} 

int ACOImpl::GetBytesEx( const unsigned int nFieldCode,char *  apBuffer,int anBufferSize )
{
	try
	{	
		if (NULL == apBuffer||anBufferSize<0)
		{
			return 0;
		}
		

		if (checkvariantrowfield(__current_row, nFieldCode,FTBytes) < 0)
		{
			return 0;
		}
		else
		{
			SFieldContent& fc =(*__current_row)[nFieldCode];
			
			int lnBuffersize = anBufferSize;

			if (lnBuffersize> fc.buf.size())
			{
				lnBuffersize = fc.buf.size();
			}

			memcpy(apBuffer,&(fc.buf.front()),lnBuffersize);

			return lnBuffersize;
		}
	}
	catch(std::exception& ex)
	{
		return 0;
	}
	catch(...)
	{		
		return 0;
	}
}
