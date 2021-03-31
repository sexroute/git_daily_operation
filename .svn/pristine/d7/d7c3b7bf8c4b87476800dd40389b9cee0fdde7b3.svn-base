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

#include "aco/ACOImpl.h"
#include "aco/ACO.h"
#include <Windows.h>

#ifdef WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

bool ACO::Absolute(const unsigned int row){
  return _pAcoImpl->Absolute(row);
}


void ACO::ClearWarnings(){
  _pAcoImpl->ClearWarnings();
}

bool ACO::First(){  
  return _pAcoImpl->First();
}

bool ACO::GetBoolean(const unsigned int nFieldCode){
   return _pAcoImpl->GetBoolean(nFieldCode);
}

char ACO::GetByte(const unsigned int nFieldCode){
	return _pAcoImpl->GetByte(nFieldCode);
}

std::vector<unsigned char> ACO::GetBytesEx(const unsigned int nFieldCode)
{
	return _pAcoImpl->GetBytesEx(nFieldCode);
}

int ACO::GetBytesEx( const unsigned int nFieldCode,char * apBuffer,int anBufferSize )
{
	return _pAcoImpl->GetBytesEx(nFieldCode,apBuffer,anBufferSize);
}


std::vector<char> ACO::GetBytes(const unsigned int nFieldCode)
{
	return _pAcoImpl->GetBytes(nFieldCode);
}

std::string ACO::GetDate(const unsigned int nFieldCode){
	return _pAcoImpl->GetDate(nFieldCode);
}

tm ACO::GetDateTime(const unsigned int nFieldCode){
	return _pAcoImpl->GetDateTime(nFieldCode);
}

double ACO::GetDouble(const unsigned int nFieldCode){
	return _pAcoImpl->GetDouble(nFieldCode);
}

float ACO::GetFloat(const unsigned int nFieldCode){
	return _pAcoImpl->GetFloat(nFieldCode);
}


int ACO::GetInt(const unsigned int nFieldCode){
	return _pAcoImpl->GetInt(nFieldCode);
}

unsigned int ACO::GetRow(){
  return _pAcoImpl->GetRow();
}

short ACO::GetShort(const unsigned int nFieldCode){
	return _pAcoImpl->GetShort(nFieldCode);
}

std::string ACO::GetString(const unsigned int nFieldCode){
	return _pAcoImpl->GetString(nFieldCode);
}


unsigned int ACO::GetUInt(const unsigned int nFieldCode){
     return _pAcoImpl->GetUInt(nFieldCode);
}
  
std::list<std::string> ACO::GetWarnings(){
  return _pAcoImpl->GetWarnings();
}

void ACO::InsertRow(const unsigned int rowsize){
   _pAcoImpl->InsertRow(rowsize);
}

bool ACO::IsFirst(){
  return _pAcoImpl->IsFirst();
}

bool ACO::IsLast(){
  return _pAcoImpl->IsLast();
}

bool ACO::IsAfterLast(){
	return _pAcoImpl->IsAfterLast();
}

bool ACO::IsBeforeFirst(){
	return _pAcoImpl->IsBeforeFirst();
}

bool ACO::Last(){
  return _pAcoImpl->Last();
}

bool ACO::Next(){
	return _pAcoImpl->Next();
}

bool ACO::Previous(){
  return _pAcoImpl->Previous();
}

bool ACO::Relative(const int rows){
   return _pAcoImpl->Relative(rows);
}

void ACO::SetBoolean(const unsigned int nFieldCode,const bool x){
  _pAcoImpl->SetBoolean(nFieldCode,x);
}

void ACO::SetByte(const unsigned int nFieldCode,const char x){
_pAcoImpl->SetByte(nFieldCode, x);
}

void ACO::SetBytes(const unsigned int nFieldCode,const char* x,const unsigned int size)
{
  _pAcoImpl->SetBytes(nFieldCode, x,size);
}

void ACO::SetDate(const unsigned int nFieldCode,const char* x){
  _pAcoImpl->SetDate(nFieldCode,x);
}

void ACO::SetDouble(const unsigned int nFieldCode,const double x){
   _pAcoImpl->SetDouble(nFieldCode,x);
}

void ACO::SetFloat(const unsigned int nFieldCode,const float x){
  _pAcoImpl->SetFloat(nFieldCode, x);
}

void ACO::SetInt(const unsigned int nFieldCode,const int x){
   _pAcoImpl->SetInt(nFieldCode,x);
}

void ACO::SetShort(const unsigned int nFieldCode,const short x){
 _pAcoImpl->SetShort(nFieldCode, x);
}

void ACO::SetString(const unsigned int nFieldCode,const char* x){
  _pAcoImpl->SetString(nFieldCode,x);
}

void ACO::SetUInt(const unsigned int nFieldCode,const unsigned int x){
   _pAcoImpl->SetUInt(nFieldCode,x);  
}
  
void ACO::SetWarnings(const std::list<std::string>& warnings){
  _pAcoImpl->SetWarnings(warnings);
}

void ACO::AddWarning(const std::string& warning){
  _pAcoImpl->AddWarning(warning);
}


bool ACO::HasField(const unsigned int nFieldCode){
	return _pAcoImpl->HasField(nFieldCode);
}

bool ACO::HasField(const unsigned int nFieldCode,const int nRowIndex){
    return _pAcoImpl->HasField(nFieldCode,nRowIndex);
}

userprofile ACO::GetUserProfile(){
	return _pAcoImpl->GetUserProfile();
}

void ACO::SetUserProfile(const userprofile& profile){
	_pAcoImpl->SetUserProfile(profile);
}

unsigned int ACO::GetServiceCode(){
  return _pAcoImpl->GetServiceCode();
}

void ACO::SetServiceCode(const unsigned int code){
///重置发送和接受状态
	_bSentSuc = false;
	_bRecievedSuc = false;

	_pAcoImpl->SetServiceCode(code);
}

unsigned int ACO::GetVersion(){
  return _pAcoImpl->GetVersion();
}

void ACO::SetVersion(const unsigned int version){
   _pAcoImpl->SetVersion(version);
}

bool ACO::Connect(unsigned short port,const char* address){	
  return _pAcoImpl->Connect(port, address);
}

bool ACO::Clear(){
	return _pAcoImpl->Clear();
}

bool ACO::Close(){
	return _pAcoImpl->Close();
}

int ACO::GetRetCode(){
  return _pAcoImpl->GetRetCode();
}

void ACO::SetRetCode(const int code){
  _pAcoImpl->SetRetCode(code);
}

unsigned int  ACO::GetFieldCount(){
	return _pAcoImpl->GetFieldCount();
}

unsigned int ACO::GetFieldCount(const unsigned int row){
   return _pAcoImpl->GetFieldCount(row);
}

unsigned int ACO::GetRowCount(){
//	TTrace::DebugEx(100,"%s ",__FUNCTION__);
  return _pAcoImpl->GetRowCount();
}

ACO::ACO(bool ssl)
{
 _pAcoImpl = new ACOImpl(ssl);
 _bSentSuc = false;
 _bRecievedSuc = false;
}

ACO::~ACO(){
	if (_pAcoImpl)
          delete _pAcoImpl;
	_pAcoImpl = NULL;
}


bool ACO::More(){
	bool bMore = MoreEx();
	return  (bMore&&GetRowCount());	
}

bool ACO::MoreEx(){

	if (_bSentSuc && _bRecievedSuc )
		return _pAcoImpl->Next();
	else
	{
        _bSentSuc = Send(true);
		if (_bSentSuc)
			_bRecievedSuc = _pAcoImpl->Receive();
		else
			return false;
		return _bRecievedSuc;
	}
	return _bRecievedSuc;
}

bool ACO::Send(bool bReSend_){
    return _pAcoImpl->Send(bReSend_);
}


void ACO::SetTimeout(const int timeoutSec){
 _pAcoImpl->SetTimeout(timeoutSec);
}

bool ACO::Receive(){
  return _pAcoImpl->Receive();
}
std::string ACO::GetRemoteIP() const{
    return _pAcoImpl->GetRemoteIP();
}

ACOImpl* ACO::GetAcoImpl(){
	return _pAcoImpl;
}