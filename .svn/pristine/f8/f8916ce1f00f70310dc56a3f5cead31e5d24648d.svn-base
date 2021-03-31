#pragma once
#include <CaseLibrary/MyLog.h>

#ifdef _PIDMS
#include <aco/ACO.h>
#include "PIDMSCommWrapper.h"
#endif

#include <additions/ICommInterFace.h>
#include <vector>
#include <map>
#include <string>


using std::vector;
using std::map;
using std::string;

#define DATE_TIME_FORMAT			"%Y-%m-%d %H:%M:%S"			// 时间标准格式
#define MAX_SQL_LENGTH				1024											// SQL语句最大长度
#define SQL_LENGTH(sql_str)			(int)::strlen(sql_str)+10				// 实际需要的SQL语句长度：针对DBAdaptor::Cmd()的缺陷

// 删除动态内存指针(单值)
#define DELETE_POINTER(ptr)           if (ptr)\
														{\
															delete ptr;\
															ptr = NULL;\
														}
///< 删除动态内存指针(数组)
#define DELETE_BUFFER(ptr)             if (ptr)\
												        {\
													        delete [] ptr;\
													        ptr = NULL;\
												        }

/// buffer类型
typedef vector<unsigned char> BUFFER;

/// 数据接口
struct S_DATA_INTERFACE 
{
	map<int, int> _mapInteger;				///< 整型数据
	map<int, double> _mapDouble;		///< 浮点型数据
	map<int, string> _mapString;			///< 字符串数据
	map<int, BUFFER> _mapBuffer;		///< buffer数据

	void Trace()
	{
		return ;
		CMyLog::Trace_Log("\n");
		for (map<int,int>::const_iterator it=_mapInteger.begin();it!=_mapInteger.end();++it)
		{
			if (it == _mapInteger.begin())
			{
				CMyLog::Trace_Log("integer data:\n");
			}
			char buf[1024];
			sprintf(buf,"字段号：%d，字段值：%d \n",it->first,it->second);
			CMyLog::Trace_Log(buf);
		}
		for (map<int,double>::const_iterator it=_mapDouble.begin();it!=_mapDouble.end();++it)
		{
			if (it == _mapDouble.begin())
			{
				CMyLog::Trace_Log("double data:\n");
			}
			char buf[1024];
			sprintf(buf,"字段号：%d，字段值：%f \n",it->first,it->second);
			CMyLog::Trace_Log(buf);
		}
		for (map<int,string>::const_iterator it=_mapString.begin();it!=_mapString.end();++it)
		{
			if (it == _mapString.begin())
			{
				CMyLog::Trace_Log("string data:\n");
			}
			char buf[1024];
			sprintf(buf,"字段号：%d，字段值：%s \n",it->first,it->second.c_str());
			CMyLog::Trace_Log(buf);
		}
		for (map<int,BUFFER>::const_iterator it=_mapBuffer.begin();it!=_mapBuffer.end();it++)
		{
			if (it == _mapBuffer.begin())
			{
				CMyLog::Trace_Log("buffer data:\n");
			}
			char buf[1024];
			sprintf(buf,"字段号：%d，字段值：%s \n",it->first,"&it->second[0]");
			CMyLog::Trace_Log(buf);
		}
	}
};

#ifdef _PIDMS

static bool OutputResultData(ICommInterFace & pFixComm, const vector<S_DATA_INTERFACE> &vecData_)
{
	ACO* pAco_ = pFixComm.GetAco();
	if (!pAco_)
		return false;
	pAco_->Clear();
	for (vector<S_DATA_INTERFACE>::const_iterator itData = vecData_.begin(); itData != vecData_.end(); ++itData)
	{
		pAco_->InsertRow();
		// 输出整型数据
		for (map<int, int>::const_iterator itMapInt = itData->_mapInteger.begin(); itMapInt != itData->_mapInteger.end(); ++itMapInt)
			pAco_->SetInt(itMapInt->first, itMapInt->second);
		// 输出浮点型数据
		for (map<int, double>::const_iterator itMapDbl = itData->_mapDouble.begin(); itMapDbl != itData->_mapDouble.end(); ++itMapDbl)
			pAco_->SetDouble(itMapDbl->first, itMapDbl->second);
		// 输出字符串数据
		for (map<int, string>::const_iterator itMapStr = itData->_mapString.begin(); itMapStr != itData->_mapString.end(); ++itMapStr)
			pAco_->SetString(itMapStr->first, itMapStr->second.c_str());
		// 输出buffer数据
		for (map<int, BUFFER>::const_iterator itMapBuff = itData->_mapBuffer.begin(); itMapBuff != itData->_mapBuffer.end(); ++itMapBuff)
		{
			if (itMapBuff->second.empty())
				pAco_->SetBytes(itMapBuff->first, NULL, 0);
			else
				pAco_->SetBytes(itMapBuff->first, (char*)(&(itMapBuff->second[0])), itMapBuff->second.size());
		}
	}
	pAco_->SetRetCode(pAco_->GetRowCount());
	return true;
}
static bool OutputResultData(ACO *pAco_, const vector<S_DATA_INTERFACE> &vecData_)
{
	if (!pAco_)
		return false;
	pAco_->Clear();
	for (vector<S_DATA_INTERFACE>::const_iterator itData = vecData_.begin(); itData != vecData_.end(); ++itData)
	{
		pAco_->InsertRow();
		// 输出整型数据
		for (map<int, int>::const_iterator itMapInt = itData->_mapInteger.begin(); itMapInt != itData->_mapInteger.end(); ++itMapInt)
			pAco_->SetInt(itMapInt->first, itMapInt->second);
		// 输出浮点型数据
		for (map<int, double>::const_iterator itMapDbl = itData->_mapDouble.begin(); itMapDbl != itData->_mapDouble.end(); ++itMapDbl)
			pAco_->SetDouble(itMapDbl->first, itMapDbl->second);
		// 输出字符串数据
		for (map<int, string>::const_iterator itMapStr = itData->_mapString.begin(); itMapStr != itData->_mapString.end(); ++itMapStr)
			pAco_->SetString(itMapStr->first, itMapStr->second.c_str());
		// 输出buffer数据
		for (map<int, BUFFER>::const_iterator itMapBuff = itData->_mapBuffer.begin(); itMapBuff != itData->_mapBuffer.end(); ++itMapBuff)
		{
			if (itMapBuff->second.empty())
				pAco_->SetBytes(itMapBuff->first, NULL, 0);
			else
				pAco_->SetBytes(itMapBuff->first, (char*)(&(itMapBuff->second[0])), itMapBuff->second.size());
		}
	}
	pAco_->SetRetCode(pAco_->GetRowCount());
	return true;
}
#else
static bool OutputResultData(ICommInterFace & pFixComm, const vector<S_DATA_INTERFACE> &vecData_)
{
	pFixComm.CreateAnswerMul(pFixComm.GetFunc());
	for (vector<S_DATA_INTERFACE>::const_iterator itData = vecData_.begin(); itData != vecData_.end(); ++itData)
	{
		// 输出整型数据
		for (map<int, int>::const_iterator itMapInt = itData->_mapInteger.begin(); itMapInt != itData->_mapInteger.end(); ++itMapInt)
			pFixComm.SetItem(itMapInt->first, itMapInt->second);
		// 输出浮点型数据
		for (map<int, double>::const_iterator itMapDbl = itData->_mapDouble.begin(); itMapDbl != itData->_mapDouble.end(); ++itMapDbl)
			pFixComm.SetItem(itMapDbl->first, itMapDbl->second);
		// 输出字符串数据
		for (map<int, string>::const_iterator itMapStr = itData->_mapString.begin(); itMapStr != itData->_mapString.end(); ++itMapStr)
			pFixComm.SetItem(itMapStr->first, itMapStr->second.c_str());
		// 输出buffer数据
		for (map<int, BUFFER>::const_iterator itMapBuff = itData->_mapBuffer.begin(); itMapBuff != itData->_mapBuffer.end(); ++itMapBuff)
		{
			if (itMapBuff->second.empty())
				pFixComm.SetItemBuf(itMapBuff->first, NULL, 0);
			else
			{
				char * pBuf = (char*)(&(itMapBuff->second[0]));
				pFixComm.SetItemBuf(itMapBuff->first, (char*)(&(itMapBuff->second[0])), itMapBuff->second.size());
			}
		}
		pFixComm.Write(pFixComm.GetAci());
		pFixComm.CreateNextAnswer();
	}
	pFixComm.CreateEndMak();
	pFixComm.Write(pFixComm.GetAci());
	return true;
}


#endif