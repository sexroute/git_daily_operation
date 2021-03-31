/**
 * @file ACO.cpp
 * @brief Advanced Communication Object
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 108 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/src/ACO.cpp $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 15, 2004
 *
 * $LastChangedDate: 2008-02-23 14:04:09 +0800 (Sat, 23 Feb 2008) $
 */

#include "FixCommA/ACOA.h"
#include "aco/ACO.h"
//#include "aco/ACOConfig.h"


bool ACOA::Absolute(const unsigned int row)
{ 
  return _pAco->Absolute(row);
}

bool ACOA::More()
{
/*	if (_bSent && _bRecieved )
		return _pAco->Next();
	else
	{
		if (_bSent)
			_bRecieved = _pAco->Receive();
		else
		{
			_bSent = _pAco->Send(true);
			if ( ! _bSent ) 
				return false;
			_bRecieved = _pAco->Receive();
			if (!_bRecieved)
			{
				return false;
			}
		}
		return _bRecieved;
	}*/
	return _pAco->More();
}

bool ACOA::MoreEx(){
	return _pAco->MoreEx();
}

bool ACOA::First()
{
  return _pAco->First();
}


bool ACOA::GetBoolean(const unsigned int nFieldCode)
{
   return _pAco->GetBoolean( nFieldCode );
}

char ACOA::GetByte(const unsigned int nFieldCode)
{
   return _pAco->GetByte( nFieldCode );
}

std::vector<unsigned char> ACOA::GetBytes(const unsigned int nFieldCode){
  return _pAco->GetBytesEx( nFieldCode );
}

int ACOA::GetBytesEx( const unsigned int nFieldCode,char * apbuffer,int anBufferSize )
{
	return _pAco->GetBytesEx(nFieldCode,apbuffer,anBufferSize);
}


std::string ACOA::GetDate(const unsigned int nFieldCode){
 return _pAco->GetDate( nFieldCode );
}

double ACOA::GetDouble(const unsigned int nFieldCode){
  return _pAco->GetDouble( nFieldCode );
}

float ACOA::GetFloat(const unsigned int nFieldCode){
  return _pAco->GetFloat( nFieldCode );
}


int ACOA::GetInt(const unsigned int nFieldCode){
 return _pAco->GetInt( nFieldCode );
}

unsigned int ACOA::GetRow(){
  return _pAco->GetRow();
}

short ACOA::GetShort(const unsigned int nFieldCode){
 return _pAco->GetShort( nFieldCode );
}

std::string ACOA::GetString(const unsigned int nFieldCode){
  return _pAco->GetString( nFieldCode );
}


  
unsigned int ACOA::GetUInt(const unsigned int nFieldCode){
 return _pAco->GetUInt(nFieldCode);
}
  

void ACOA::InsertRow(const unsigned int rowsize)
{
	_bSent = false;
	_bRecieved = false;
	_pAco->InsertRow( rowsize );
}

bool ACOA::IsAfterLast(){
  return _pAco->IsAfterLast();
}

bool ACOA::IsBeforeFirst(){
  // There is no before first conception in the stl's container
  return _pAco->IsBeforeFirst();
}

bool ACOA::IsFirst(){
  return _pAco->IsFirst();
}

bool ACOA::IsLast(){
   return _pAco->IsLast();
}

bool ACOA::Last(){
   return _pAco->Last();
}

bool ACOA::Next(){
   return _pAco->Next();
}

bool ACOA::Previous(){
   return _pAco->Previous();
}

bool ACOA::Relative(const int rows){
   return _pAco->Relative( rows );
}


void ACOA::SetBoolean(const unsigned int nFieldCode,const bool x){
  _pAco->SetBoolean( nFieldCode, x );
}

void ACOA::SetByte(const unsigned int nFieldCode,const char x){
 _pAco->SetByte( nFieldCode, x );
}

void ACOA::SetBytes(const unsigned int nFieldCode,const char* x,const unsigned int size){
   _pAco->SetBytes( nFieldCode, x, size );
}

void ACOA::SetDate(const unsigned int nFieldCode,const char* x){
 _pAco->SetDate( nFieldCode, x );
}

void ACOA::SetDouble(const unsigned int nFieldCode,const double x){
   _pAco->SetDouble( nFieldCode, x );
}

void ACOA::SetFloat(const unsigned int nFieldCode,const float x){
 _pAco->SetFloat( nFieldCode, x );
}

void ACOA::SetInt(const unsigned int nFieldCode,const int x){
 _pAco->SetInt(nFieldCode, x);
}

void ACOA::SetShort(const unsigned int nFieldCode,const short x){
 _pAco->SetShort( nFieldCode, x );
}

void ACOA::SetString(const unsigned int nFieldCode,const char* x){
 _pAco->SetString( nFieldCode, x );
}

void ACOA::SetUInt(const unsigned int nFieldCode,const unsigned int x){
   _pAco->SetUInt( nFieldCode, x );  
}
  
bool ACOA::HasField(const unsigned int nFieldCode,const int nRowIndex){
   return _pAco->HasField( nFieldCode, nRowIndex );
}

bool ACOA::HasField(const unsigned int nFieldCode){
  return _pAco->HasField( nFieldCode );
}

unsigned int ACOA::GetServiceCode(){
  return _pAco->GetServiceCode();
}

void ACOA::SetServiceCode(const unsigned int code)
{
  _pAco->SetServiceCode( code );
  _bRecieved = false;
  _bSent     = false;
}

unsigned int ACOA::GetVersion(){
  return _pAco->GetVersion();
}

void ACOA::SetVersion(const unsigned int version){
 _pAco->SetVersion( version );
}

bool ACOA::Connect(unsigned short port,const char* address)
{
	_pAco->SetServiceCode(1000);
	_pAco->SetTimeout(10);
    return _pAco->Connect( port, address );
}

bool ACOA::Clear(){
	return _pAco->Clear();
}

bool ACOA::Close(){
   return _pAco->Close();
}

int ACOA::GetRetCode(){
  return _pAco->GetRetCode();
}

void ACOA::SetRetCode(const int code){
  _pAco->SetRetCode( code );
}

unsigned int ACOA::GetFieldCount(){
  return _pAco->GetFieldCount();
}
  
unsigned int ACOA::GetFieldCount(const unsigned int row){
  return _pAco->GetFieldCount( row );
}

unsigned int ACOA::GetRowCount(){
  return _pAco->GetRowCount();
}

ACOA::ACOA(bool ssl)
{
  _pAco  = new ACO(ssl);
  _pUsrProfile = new userprofile;
  _bSent = false;
  _bRecieved = false;
  strcpy(_pUsrProfile->username, "admin");
  strcpy(_pUsrProfile->passwd, "nw passwd");
  strcpy(_pUsrProfile->license, "license");
  strcpy(_pUsrProfile->domain, "kaida.com.cn");
  _pUsrProfile->sessionid[0]= 0;
  _pUsrProfile->logged      = false;
  _pUsrProfile->compress    = true;
  _pUsrProfile->compressionalgorithm= DEFLATION_COMPRESS;
  _pUsrProfile->compressionlevel    = DEFAULT_COMPRESSION;
  _pUsrProfile->version             = SRV_VERSION_1;  //0
  _pAco->SetUserProfile(*_pUsrProfile);
}

ACOA::~ACOA(){
   delete _pAco;
   delete _pUsrProfile;
}

bool ACOA::Send(){
  return _pAco->Send(true);
}

void ACOA::SetTimeout(const int timeoutSec){
  _pAco->SetTimeout( timeoutSec );
}


bool ACOA::Receive()
{
   return _pAco->Receive();
}

std::string ACOA::GetRemoteIP() const{
  return _pAco->GetRemoteIP();
}

struct tm ACOA::GetTime(const int &vid){
	return _pAco->GetDateTime(vid);
};

void ACOA::SetUsrPwd( const char *pcUsr_, const char *pcPwd_)
{
	strcpy( _pUsrProfile->username, pcUsr_);
	strcpy( _pUsrProfile->passwd, pcPwd_);
}