/**
 * @file SyncHelperWrapper.h
 * @brief 线程同步助手库Wrapper Facade Class 声明
 * @author 孙晓光
 *
 * $Name: 1.1.1.1 $
 * $Locker$
 * $RCSfile: SyncHelperWrapper.h,v $ 
 * $Revision: 1.1.1.1 $
 * $Source: /backup/code/middleware2/INCLUDE/SyncHelperWrapper.h,v $
 * $State: Exp $
 * Modified by: $Author: marble $
 * Created:     2004/08/17
 * Last modify: $Date: 2006/06/15 00:36:16 $
*/

#ifndef __SYNCHELPERWRAPPER_H_
#define __SYNCHELPERWRAPPER_H_

#pragma comment(lib,"SyncHelperWrapper.lib")

#include <exception>

#ifdef SYNCHELPERWRAPPER_EXPORTS
#define SYNCHELPERWRAPPER_API __declspec(dllexport)
#else
#define SYNCHELPERWRAPPER_API __declspec(dllimport)
#endif
/**
 * @class SyncHelperWrapper
 * @brief Wrapper Facade of SyncHelper Library
 */
class SYNCHELPERWRAPPER_API SyncHelperWrapper{
private:
	/**
	 * @brief typedef of HSYNCHELPER
	 */
	typedef unsigned int HSYNCHELPER;

	/**
	 * @brief implementation of SyncHelper factory
	 */
	HSYNCHELPER (*__factory)();

	/**
	 * @brief implementation of SyncHelper destructor
	 * @return bool whether the destruction is successful
	 */
	bool (*__destructor)(HSYNCHELPER synchelper);

	/**
	 * @brief implementatin of beginning multi read
	 */
	void(*__begin_read)(HSYNCHELPER synchelper);

	/**
	 * @brief implementatin of end multi read
	 */
	void(*__end_read)(HSYNCHELPER synchelper);

	/**
	 * @brief implementatin of beginning exclusive write
	 */
	void(*__begin_write)(HSYNCHELPER synchelper);

	/**
	 * @brief implementatin of end exclusive write
	 */
	void(*__end_write)(HSYNCHELPER synchelper);

	/**
	 * @brief handle of SyncHelper
	 */
	HSYNCHELPER __synchelper;

public:
	/**
	 * @brief default constructor
	 */
	SyncHelperWrapper() throw(std::exception);

	/**
	 * @brief destructor
	 */
	~SyncHelperWrapper();

	/**
	 * @brief begin multi read
	 */
	inline void BeginRead();

	/**
	 * @brief end multi read
	 */
	inline void EndRead();

	/**
	 * @brief begin exclusive write
	 */
	inline void BeginWrite();

	/**
	 * @brief end exclusive write
	 */
	inline void EndWrite();
};

#endif //__SYNCHELPERWRAPPER_H_