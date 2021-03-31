/**
* @file ACOImpl.h
* @brief Advanced Communication Object
* @author Xiaoguang Sun
*
* $LastChangedRevision: 106 $
*
* $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/ACOImpl.h $
*
* $LastChangedBy: marble $
*
* Created:     Jul. 5, 2004
*
* $LastChangedDate: 2010-02-27 23:25:36 +0800 (Tue, 12 Feb 2008) $
*/

#ifndef __ACOImpl_H_
#define __ACOImpl_H_

#include "aco/Util.h"
#include <vector>
#include <list>
#include <string>
#include <map>

#include <aco/BHACO.h>
using namespace BHAco;
using namespace std;
/**
* @brief no error
*/
#define ACO_RET_CODE_NO_ERROR 0

/**
* @brief time out error
*/
#define ACO_RET_CODE_TIMEOUT  -1


/**
* @example ACOImpl.h
* ACOImpl usage method:
*
* 1. client peer usage:
*
*    Firstly, setup user profile. Such as user name,
*     domain, public key and so on that required to 
*      establish connection. Then invoke method 
*     'Connect()' to connect to remote server.
*
*      Secondly, after the connection is established,
*      the client peer can use 'SetServiceCode()' method
*       to setup requested service code. Then setup the
*          the request data field based on the predefined
*      invoking rule. After the request fields are
*          constructed successfully, the client can invoke
*      'Send()' method to send the request to the server.
*        When return from this method, the client peer can
*      invoke 'Receive' method and use the predefined 
*      return rule to fetch the result. If any exception 
*      is thrown when invoking methods. There must exist
*      some problems needed to be deal with.
*
*       e.g. We need to request a service with code 
*      SERVICE_QUERYNAME. And the parameter needed by this
*      service is a single integer field with code 
*      FIELD_PRIMARYID. As the definition of return result 
*      set has two fields, one is string field FIELD_NAME 
*      and the other integer field is FIELD_AGE. These
*      code is a sample.
*

*
*/


/**
* @class ACOImpl ACOImpl.h "aco/ACOImpl.h"
* @brief Advanced Communication Object
* @version 1.0
*/
class ACOEXPORT ACOImpl{
public:
	/**
	* @brief enumerate of data types
	*/
	
	/**
	* @brief typedef bytestream
	*
	*/
	typedef Buffers bytestream;

	/**
	* @brief typedef variantitem
	*/
	

	/**
	* @brief typedef variantfield
	*/
	typedef SFieldContent variantfield;

	/**
	* @brief typedef of variantrow
	*/
	//typedef std::vector<variantfield> variantrow;
	typedef RowDetail variantrow;

	/**
	* @brief typedef of fielditerator
	*/
	typedef variantrow::iterator fielditerator;

	/**
	* @brief typedef of constfielditerator
	*/
	//typedef variantrow::const_iterator constfielditerator;

	/**
	* @brief typedef of variantresultset
	*/
	typedef std::vector<variantrow> variantresultset;

	/**
	* @brief typedef of cursor
	*/
	typedef variantresultset::iterator cursor;

public:
	/**
	* @brief default constructor
	*/
	ACOImpl(bool ssl=false);

	/**
	* @brief destructor
	*/
	~ACOImpl();

	/**
	* @brief change current cursor to the nth row
	* @param row row number
	* @return whether the chage is successful
	*/
	bool Absolute(const unsigned int row);

	/**
	* @brief change current cursor to the result set's 
	* ending position. It's after the last row.
	*/
	void AfterLast();

	/**
	* @brief clear all warning messages
	*/
	void ClearWarnings();

	/**
	* @brief change current cursor to the result set's
	* first row.
	* @return whether the change is successful
	*/
	bool First();


	/**
	* @brief retrieve Boolean data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	bool GetBoolean(const unsigned int nFieldCode);    

	/**
	* @brief retrieve Byte data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	char GetByte(const unsigned int nFieldCode);

	/**
	* @brief retrieve Bytes data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	std::vector<char> GetBytes(const unsigned int nFieldCode);
    std::vector<unsigned char> GetBytesEx(const unsigned int nFieldCode);
	int GetBytesEx(const unsigned int nFieldCode,char * apBuffer,int anBufferSize);
	/**
	* @brief retrieve Date data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	std::string GetDate(const unsigned int nFieldCode);
    tm GetDateTime(const unsigned int nFieldCode);
	/**
	* @brief retrieve Double data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	double GetDouble(const unsigned int nFieldCode);

	/**
	* @brief retrieve Float data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	float GetFloat(const unsigned int nFieldCode);

	/**
	* @brief retrieve Int data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	int GetInt(const unsigned int nFieldCode);

	/**
	* @brief retrieve current row's number
	* @return row number
	*/
	unsigned int GetRow();

	/**
	* @brief retrieve Short data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	short GetShort(const unsigned int nFieldCode);

	/**
	* @brief retrieve String data with correspond field code
	* @param nFieldCode field code
	* @return data
	*/
	std::string GetString(const unsigned int nFieldCode);


	/**
	* @brief retrieve unsigned int field
	* @param nFieldCode field code
	* @return unsigned int value
	*/
	unsigned int GetUInt(const unsigned int nFieldCode);

	/**
	* @brief retrieve warning messages in the operation
	* @return warning messages list
	*/
	std::list<std::string> GetWarnings();

	/**
	* @brief insert a new empty row into rowset
	* @param rowsize the pre allocated size of newly inserted row
	*/
	void InsertRow();

	/**
	* @brief insert a new empty row into rowset
	* @param rowsize the pre allocated size of newly inserted row
	*/
	void InsertRow(const unsigned int rowsize);

	/**
	* @brief judge whether current cursor is located 
	*  after the last row
	* @return judgement
	*/
	bool IsAfterLast();

	/**
	* @brief judge whether current cursor is located
	*  before the first row
	* @return judgement
	*/
	bool IsBeforeFirst();

	/**
	* @brief judge whether current cursor is located 
	* at the first row
	* @return judgement
	*/
	bool IsFirst();

	/**
	* @brief judge whether current cursor is located
	* at the last row
	* @return judgement
	*/
	bool IsLast();

	/**
	* @brief move current cursor to the last row
	* @return whether the operation is successful
	*/
	bool Last();

	/**
	* @brief move current cursor to the next row
	* @return whether the operation is successful
	*/
	bool Next();

	/**
	* @brief move current cursor to the previous row
	* @return whether the operation is successful
	*/
	bool Previous();

	/**
	* @brief move current cursor
	* @param rows offset to the current postion
	* @return whether the operation is successful
	*/
	bool Relative(const int rows);


	/**
	* @brief set Boolean field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetBoolean(const unsigned int nFieldCode,const bool x);

	/**
	* @brief set Byte field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetByte(const unsigned int nFieldCode,const unsigned char& x);


	/**
	* @brief set Bytes field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetBytes(const unsigned int nFieldCode,const char* x,const unsigned int size);

	/**
	* @brief set Date field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetDate(const unsigned int nFieldCode,const char* x);

	/**
	* @brief set Datetime field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetDateTime(const unsigned int nFieldCode,const tm& x_);
	
	/**
	* @brief set Datetime field 格式为：2010-8-9 10:11:2
	* @param nFieldCode field code
	* @param x data
	*/
	void SetDateTime(const unsigned int nFieldCode,const char* x);
	/**
	* @brief set Double field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetDouble(const unsigned int nFieldCode,const double x);

	/**
	* @brief set Float field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetFloat(const unsigned int nFieldCode,const float x);

	/**
	* @brief set Integer field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetInt(const unsigned int nFieldCode,const int x);

	/**
	* @brief set Short field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetShort(const unsigned int nFieldCode,const short x);

	/**
	* @brief Set String field
	* @param nFieldCode field code
	* @param x data
	*/
	void SetString(const unsigned int nFieldCode,const char* x);


	/**
	* @brief set unsigned int field
	* @param nFieldCode field code
	* @param x unsigned int value
	*/
	void SetUInt(const unsigned int nFieldCode,const unsigned int x);

	/**
	* @brief set warning messages
	* @param warnings warning messages
	*/
	void SetWarnings(const std::list<std::string>& warnings);

	/**
	* @brief add warning
	* @param warning warning message
	*/
	void AddWarning(const std::string& warning);

	/**
	* @brief judge whether the nRowIndex row has the nFieldCode field
	* @param nFieldCode field code
	* @param nRowIndex row index
	* @return judgement
	*/
	bool HasField(const unsigned int nFieldCode,const int nRowIndex);

	/**
	* @brief judge whether the current row has the nFieldCode field
	* @param nFieldCode field code
	* @return judgement
	*/
	bool HasField(const unsigned int nFieldCode);

	/**
	* @brief retrieve current user's profile
	* @return user profile
	*/
	userprofile GetUserProfile();

	/**
	* @brief set user profile
	* @param profile user profile
	*/
	void SetUserProfile(const userprofile& profile);

	/**
	* @brief retrieve service code
	* @Return service code
	*/
	unsigned int GetServiceCode();

	/**
	* @brief set service code
	* @param code service code
	*/
	void SetServiceCode(const unsigned int code);

	/**
	* @brief retrieve version
	* @return version
	*/
	unsigned int GetVersion();

	/**
	* @brief set retrieve
	* @param version
	* 
	* version belongs to [0,8]
	*/
	void SetVersion(const unsigned int version);

	/**
	* @brief connect to specified server and use manual login mode
	* @param port port number
	* @param address server peer address
	* @return whether the connection is established
	*/
	bool Connect(unsigned short port,const char* address);

	/**
	* @brief close the connection
	* @return whether the operation is successful
	*/
	bool Close();

	/**
	* @brief retrieve return code
	* @return return code, greater than 0 if no error, 
	* less than 0 if there are some errors.
	*/
	int GetRetCode();

	/**
	* @brief set return code
	* @param code return code
	*/
	void SetRetCode(const int code);

	/**
	* @brief retrieve the rowset's row count 
	* @return row count
	*/
	unsigned int GetRowCount();

	/**
	* @brief retrieve the current row's number of fields
	* @param row row count
	* @return field count
	*/
	unsigned int GetFieldCount();

	/**
	* @brief retrieve the selected row's number of fields
	* @param row row count
	* @return field count
	*/
	unsigned int GetFieldCount(const unsigned int row);

	/**
	* @brief send request
	* @return return code
	*/
	bool Send(bool bResend_ );

	/**
	* @brief set timeout threshold
	* @param timeoutSec timeout seconds
	*/
	void SetTimeout(const int timeoutSec);

	/**
	* @brief clear all data in the rowset
	* @return whether the operation is successful
	*/
	bool Clear();

	/**
	* @brief receive response
	*/
	bool Receive();

	/**
	* @brief get remote side IP address
	*/
	std::string GetRemoteIP() const;

	unsigned short GetRemotePort() const;



	/*  void ClearAllObjects();

	void SetObject(const int oid,void* obj,void* arg,bool (*destructor)(void*,void*));

	void* GetObject(const int oid);
	*/
	/**
	* @brief clear all variables

	void ClearAllVariables();
	*/
	/**
	* @brief set connection specified variable

	void SetVariable(const int vid,const char* val);
	void SetVariable(const int vid,const int val);
	void SetVariable(const int vid,const bool val);
	void SetVariable(const int vid,const float val);
	void SetVariable(const int vid,const double val);
	*/  

	/**
	* @brief get connection specified variable
	bool GetVariable(const int vid,std::string& val);
	bool GetVariable(const int vid,int& val);
	bool GetVariable(const int vid,bool& val);
	bool GetVariable(const int vid,float& val);
	bool GetVariable(const int vid,double& val);
	*/

    ///获取当前结果集,返回条数
    int GetResultset(SDataPacket& des_);
	///设置当前结果集,返回条数
	int SetResultset(const SDataPacket& des_);
    

private:
	/**
	* @brief user profile
	*/
	userprofile __user_profile;

	/**
	* @brief return code
	*/
	int __ret_code;

	/**
	* @brief whether the connection is active
	*/
	bool __active_connection;

	/**
	* @brief timeout threshold seconds
	*/
	int __timeout_value;


	/**
	* @brief underlying output packet builder for
	*  packet header data marshalling
	*/
	//ACE_OutputCDR __header_output_cdr;

	/**
	* @brief underlying output packet builder for
	*  packet body data marshalling
	*/
	//ACE_OutputCDR __body_output_cdr;

	/**
	* @brief input CDR stream for header data demarshalling
	*/
	//ACE_InputCDR __header_input_cdr;

	/**
	* @brief input CDR stream for body data demarshalling
	*/
	//ACE_InputCDR __body_input_cdr;

private:
	/**
	* @brief current row
	*/
	cursor __current_row;

	/**
	* @brief resultset's end cursor
	*/
	cursor __end_row;

	/**
	* @brief current field
	*/
	fielditerator __current_field;

	/**
	* @brief row's end field;
	*/
	fielditerator __end_field;

	/**
	* @brief error number;
	*/
	int __error;

#define RECONNECT_MAXCOUNTS 3
	/**@brief  重新连接的次数,0表示没有重连，如果在超时范围内可以重连，超过超时时间就不重连
	*/
	int __nReconnects;

	/**信息数据包
	  */
	SDataPacket _dataPacket;


	///链接IP地址
	std::string          _sIPAddr;
	//端口
	unsigned short  _iPort;

	///链接代理
	RequestPrx __connProxy;
	///通讯上下文
	Ice::CommunicatorPtr __communicator;
private:

	/**
	* @brief shake hands with server and login
	*/
	inline bool Login();

	/**
	* @brief receive packet header
	*/
	inline bool ReceiveHeader();

	/**
	* @brief receive packet body
	*/
	bool ReceiveBody();

	/**
	* @brief send the specified resultset and sockheader
	* @param dataset
	* @param sockheader
	*/
	bool Send();

	void AddField(const unsigned int nFieldCode,const SFieldContent& sfc_);

	/**
	* @brief reconnect to specified server and use manual login mode
	* @param port port number
	* @param address server peer address
	* @return whether the connection is established
	*/
	bool Reconnect(unsigned short port,const char* address);

	/**@brief variantresultset copy
	  */
	int resultsetclone(const variantresultset& source_, variantresultset& des_);

	/**@brief variantrow copy
	*/
	int variantrowclone(const variantrow& source_, variantrow& des_);
    
	/**@brief 检查指定行是否有指定字段
	  *@return 返回0表示有字段，小于0 则表示没有该字段
	  */
	int checkvariantrowfield(const cursor row_, const unsigned int &nfield_,const EFieldType& ft_);

	tm ParseDateTime(const string& sDateTime_);

	/**异常处理
	  */
	void handleException(const IceUtil::Exception& e);
	
	/** size of data packet 
	  */
	unsigned int GetDataPacketSize();
	unsigned int GetDataPacketSize(const SDataPacket& dp_);

};
#endif // __ACOImpl_H_
