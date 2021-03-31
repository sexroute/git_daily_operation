/**
 * @file ACO.h
 * @brief Advanced Communication Object
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/ACO.h $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 5, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#ifndef __ACOBASE_H_
#define __ACOBASE_H_

#include <vector>
#include <list>
#include <string>
#include <map>


/**
 * @example ACO.h
 * ACO usage method:
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
 * @class ACO ACO.h "aco/ACO.h"
 * @brief Advanced Communication Object
 * @version 1.0
 */
class ACOBase
{
 public:
  /**
   * @brief default constructor
   */
   ACOBase(bool ssl=false)
   {
   }

  /**
   * @brief destructor
   */
  virtual ~ACOBase()
  {
  }
  
  //返回值true：结果集；false：无结果集（第一次调用出现false时，也可能是中间件出现问题）。
  virtual bool	More() = 0;

  ///第一次调用时，如果返回true表示，与中间件通讯成功，可能存在无结果集，所以获取结果前，必须调用GetCount判断是否有结果。
  ///其他调用时，返回false表示没有结果
  virtual bool  MoreEx() = 0;

  /**
   * @brief change current cursor to the nth row
   * @param row row number
   * @return whether the chage is successful
   */
  virtual bool Absolute(const unsigned int row) = 0;
  
  /**
   * @brief change current cursor to the result set's
   * first row.
   * @return whether the change is successful
   */
  virtual bool First() = 0;


  /**
   * @brief retrieve Boolean data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual bool GetBoolean(const unsigned int nFieldCode) = 0;    
    
  /**
   * @brief retrieve Byte data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual char GetByte(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve Bytes data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual std::vector<unsigned char> GetBytes(const unsigned int nFieldCode) = 0;


    
  /**
   * @brief retrieve Date data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual std::string GetDate(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve Double data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual double GetDouble(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve Float data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual float GetFloat(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve Int data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual int GetInt(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve current row's number
   * @return row number
   */
  virtual unsigned int GetRow() = 0;

  /**
   * @brief retrieve Short data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual short GetShort(const unsigned int nFieldCode) = 0;
    
  /**
   * @brief retrieve String data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  virtual std::string GetString(const unsigned int nFieldCode) = 0;

 
  /**
   * @brief retrieve unsigned int field
   * @param nFieldCode field code
   * @return unsigned int value
   */
  virtual unsigned int GetUInt(const unsigned int nFieldCode) = 0;
  
 /**
   * @brief insert a new empty row into rowset
   * @param rowsize the pre allocated size of newly inserted row
   */
  virtual void InsertRow(const unsigned int rowsize=0) = 0;

 
  /**
   * @brief judge whether current cursor is located 
   * at the first row
   * @return judgement
   */
  virtual bool IsFirst() = 0;

  /**
   * @brief judge whether current cursor is located
   * at the last row
   * @return judgement
   */
  virtual bool IsLast() = 0;

  /**
   * @brief move current cursor to the last row
   * @return whether the operation is successful
   */
  virtual bool Last() = 0;

  /**
   * @brief move current cursor to the next row
   * @return whether the operation is successful
   */
  virtual bool Next() = 0;

  /**
   * @brief move current cursor to the previous row
   * @return whether the operation is successful
   */
  virtual bool Previous() = 0;

  /**
   * @brief move current cursor
   * @param rows offset to the current postion
   * @return whether the operation is successful
   */
  virtual bool Relative(const int rows) = 0;
  
  
  /**
   * @brief set Boolean field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetBoolean(const unsigned int nFieldCode,const bool x) = 0;
  
  /**
   * @brief set Byte field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetByte(const unsigned int nFieldCode,const char x) = 0;
  
  /**
   * @brief set Bytes field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetBytes(const unsigned int nFieldCode,const char* x,const unsigned int size) = 0;
  
  /**
   * @brief set Date field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetDate(const unsigned int nFieldCode,const char* x) = 0;

  /**
   * @brief set Double field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetDouble(const unsigned int nFieldCode,const double x) = 0;
  
  /**
   * @brief set Float field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetFloat(const unsigned int nFieldCode,const float x) = 0;
  
  /**
   * @brief set Integer field
   * @param nFieldCode field code
   * @param x data
   */
 virtual  void SetInt(const unsigned int nFieldCode,const int x) = 0;
  
  /**
   * @brief set Short field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetShort(const unsigned int nFieldCode,const short x) = 0;
  
  /**
   * @brief Set String field
   * @param nFieldCode field code
   * @param x data
   */
  virtual void SetString(const unsigned int nFieldCode,const char* x) = 0;

  /**
   * @brief set unsigned int field
   * @param nFieldCode field code
   * @param x unsigned int value
   */
  virtual void SetUInt(const unsigned int nFieldCode,const unsigned int x) = 0;
  
 /**
   * @brief judge whether the nRowIndex row has the nFieldCode field
   * @param nFieldCode field code
   * @param nRowIndex row index
   * @return judgement
   */
  virtual bool HasField(const unsigned int nFieldCode,const int nRowIndex) = 0;
  
  /**
   * @brief judge whether the current row has the nFieldCode field
   * @param nFieldCode field code
   * @return judgement
   */
  virtual bool HasField(const unsigned int nFieldCode) = 0;

  /**
   * @brief retrieve service code
   * @Return service code
   */
  virtual unsigned int GetServiceCode() = 0;
  
  /**
   * @brief set service code
   * @param code service code
   */
  virtual void SetServiceCode(const unsigned int code) = 0;

  /**
   * @brief retrieve version
   * @return version
   */
  virtual unsigned int GetVersion() = 0;

  /**
   * @brief set retrieve
   * @param version
   * 
   * version belongs to [0,8]
   */
  virtual void SetVersion(const unsigned int version) = 0;
  
  /**
   * @brief connect to specified server and use manual login mode
   * @param port port number
   * @param address server peer address
   * @return whether the connection is established
   */
  virtual bool Connect(unsigned short port,const char* address=NULL) = 0;


  /**
   * @brief close the connection
   * @return whether the operation is successful
   */
  virtual bool Close() = 0;
  
  /**
   * @brief retrieve return code
   * @return return code, greater than 0 if no error, 
   * less than 0 if there are some errors.
   */
  virtual int GetRetCode() = 0;
  
  /**
   * @brief set return code
   * @param code return code
   */
  virtual void SetRetCode(const int code) = 0;

  /**
   * @brief retrieve the rowset's row count 
   * @return row count
   */
  virtual unsigned int GetRowCount() = 0;
  
  /**
   * @brief retrieve the current row's number of fields
   * @return field count
   */
  virtual unsigned int GetFieldCount() = 0;
  
  /**
   * @brief retrieve the selected row's number of fields
   * @param row row count
   * @return field count
   */
  virtual unsigned int GetFieldCount(const unsigned int row) = 0;

  /**
   * @brief send request
   * @return return code
   */
  virtual bool Send() = 0;

  /**
   * @brief set timeout threshold
   * @param timeoutSec timeout seconds
   */
  virtual void SetTimeout(const int timeoutSec) = 0;
  
  /**
   * @brief clear all data in the rowset
   * @return whether the operation is successful
   */
  virtual bool Clear() = 0;
  
  /**
   * @brief receive response
   */
  virtual bool Receive() = 0;

  /**
   * @brief get remote side IP address
   */
  virtual std::string GetRemoteIP() const = 0;


  ///得到指定时间属性的字段码的值
  virtual struct tm GetTime (const int &vid) = 0;
  ///设置用户名和密码
  virtual void SetUsrPwd( const char *pcUsr_, const char *pcPwd_) = 0;

  virtual int GetBytesEx(const unsigned int nFieldCode,char * apBuffer,int anBufferSize) = 0;
};
#endif // __ACOBASE_H_
