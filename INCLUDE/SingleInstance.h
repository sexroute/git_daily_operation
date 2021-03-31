/**
 * @file SingleInstanceApp.h
 * @brief 只让程序运行单实例类Handler声明
 * @author marble
 *
 * $Name: 1.1.1.1 $
 * $Locker$
 * $RCSfile: SingleInstance.h,v $ 
 * $Revision: 1.1.1.1 $
 * $Source: /backup/code/middleware2/INCLUDE/SingleInstance.h,v $
 * $State: Exp $
 * Modified by: $Author: marble $
 * Created:     2005/09/11
 * Last modify: $Date: 2006/06/15 00:36:16 $
 */

#pragma once

/**
 * @class CSingleInstanceApp
 * @brief 只让程序运行单实例类
 */
class CSingleInstance 
{
private:
     HANDLE _hSingle;
     bool _bExist;
public:
	  /**初始化实例
	    *@param  pcAppName表示实例标志字符串
    */
	  CSingleInstance(char *pcAppName)
	  {
	  	  _hSingle=CreateMutex(NULL,TRUE,pcAppName);
      //_bExist:true表示已经有实例运行，false表示没有实例运行
	    	if(_hSingle && ERROR_ALREADY_EXISTS==GetLastError())
       		 _bExist=true;
	    	else
       	   _bExist=false;
	  }
	  virtual ~CSingleInstance()
	  {
	  	if(_hSingle!=NULL)
	  	   ReleaseMutex(_hSingle);
	  }
	  /**判断是否实例已经存在
	    *@return true表示已经有实例运行，false表示没有实例运行
	    */
	  bool IsExist()
	  {
	  	return _bExist;
	  }
	
};
