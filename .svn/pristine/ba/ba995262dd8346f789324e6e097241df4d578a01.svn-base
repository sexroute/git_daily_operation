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

#ifndef __ACO_H_
#define __ACO_H_

#include <vector>
#include <list>
#include <string>
#include <map>

#ifndef ACOEXPORT
#ifdef _DEBUG
#pragma  comment(lib,"comm_fund.lib")
#else
#pragma  comment(lib,"comm_fun.lib")
#endif
#endif


#include "aco/Util.h"

/**
 * @brief no error
 */
#define ACO_RET_CODE_NO_ERROR 0

/**
 * @brief time out error
 */
#define ACO_RET_CODE_TIMEOUT  -1
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
 *    ACO __aco;
 *    
 *    const int __id=1;
 *
 *    __aco.Connect();
 *
 *    __aco.SetServiceCode(SERVICE_QUERYNAME);
 *
 *    __aco.SetInt(FIELD_PARIMARYID,__id);
 *
 *    __aco.Send();
 *
 *    __aco.Receive();
 *
 *    std::string __name=__aco.GetString(FIELD_NAME);
 *   
 *    int __age=__aco.GetInt(FIELD_NAME);
 *
 */

/**
 * @class ACO ACO.h "aco/ACO.h"
 * @brief Advanced Communication Object
 * @version 1.0
 */
class ACOImpl;

class ACOEXPORT ACO{
 public:
  /**
   * @brief default constructor
   */
  ACO(bool ssl=false);

  /**
   * @brief destructor
   */
  ~ACO();
  
  /**
   * @brief is ssl enabled connection
   */
  bool IsSSL();

  /**
   * @brief change current cursor to the nth row
   * @param row row number
   * @return whether the chage is successful
   */
  bool Absolute(const unsigned int row);
  

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
   * @brief retrieve BigDecimal data with correspond field code
   * @param nFieldCode field code
   * @return data
   */
  long long GetBigDecimal(const unsigned int nFieldCode);

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
  std::vector<unsigned char> GetBytesEx(const unsigned int nFieldCode);
  std::vector<char> GetBytes(const unsigned int nFieldCode);
  int  GetBytesEx(const unsigned int nFieldCode,char * apBuffer,int anBufferSize);
    
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
  void InsertRow(const unsigned int rowsize=0);

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
  void SetByte(const unsigned int nFieldCode,const char x);
  
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
  bool Connect(unsigned short port,const char* address=NULL);
 
  /**
  * @brief clear all data in the rowset
  * @return whether the operation is successful
  */
  bool Clear();

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
  bool Send(bool bReSend_ = false);

  /**
   * @brief set timeout threshold
   * @param timeoutSec timeout seconds
   */
  void SetTimeout(const int timeoutSec);
  

  /**
   * @brief receive response
   */
  bool Receive();

  /**
   * @brief get remote side IP address
   */
  std::string GetRemoteIP() const;

  /**
   *@brief 发送接收，并查找是否有结果集
   *@return true：有结果集；false：无结果集（第一次调用出现false时，也可能是中间件出现问题）。
   */
  bool	More();

  ///第一次调用时，如果返回true表示，与中间件通讯成功，可能存在无结果集，所以获取结果前，必须调用GetCount判断是否有结果。
  ///其他调用时，返回false表示没有结果
  bool  MoreEx();

  /**得到具体实现的指针
    */
  ACOImpl* GetAcoImpl();
private:
   ACOImpl* _pAcoImpl;
   ///是否已经发送成功
   bool _bSentSuc; 
   ///是否已经接受成功
   bool _bRecievedSuc;
};
#endif // __ACO_H_
