/**
 * @file srvdef.h
 * @brief Service Code Definition
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/SrvDef.h $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 27, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#ifndef __SRVDEF_H_
#define __SRVDEF_H_

#define SYSTEM_SERVICE_BEGIN    1000

/**
 * @brief System Code End
 */
#define SYSTEM_SERVICE_END      2000

/**
 * @example SrvDef.h
 * System Authentication Process
 * Client                           Server
 * send user profile       ->       check license information
 * compute response        <-       send back a challange
 * send response           ->       check challange/response
 * save session            <-       grant access and assign session
 */
 
// [service] system login function
//#define SERVICE_SYSTEM_LOGIN                                1000
// [in param string] user name
#define STRING_FIELD_SYSTEM_LOGIN_USER_NAME                 0
// [in param string] license
#define STRING_FIELD_SYSTEM_LOGIN_LICENSE                   1
// [in param string] domain
#define STRING_FIELD_SYSTEM_LOGIN_DOMAIN                    2
// [in param string] password
#define STRING_FIELD_SYSTEM_LOGIN_PASSWD                    3
// [in param int] 32 bits attributes 1
#define INT_FIELD_SYSTEM_LOGIN_FLAG1                        4
// [in param int] 32 bits attributes 2
#define INT_FIELD_SYSTEM_LOGIN_FLAG2                        5
// [out param int] system login service return field first 32 bit flag
#define INT_FIELD_RET_SYSTEM_LOGIN_FLAG1                    0
// [out param int] system login service return field second 32 bit flag
#define INT_FIELD_RET_SYSTEM_LOGIN_FLAG2                    1
// [out param string] system login service return field session id
#define STRING_FIELD_RET_SYSTEM_LOGIN_SESSION_ID            2
// [out param boolean] login result
#define BOOLEAN_FIELD_RET_SYSTEM_LOGIN_SUCCESS              3

// [service] change system password
#define SERVICE_SYSTEM_CHANGE_PASSWORD                      1001
// [in param string]
#define STRING_FIELD_SYSTEM_CHANGE_PASSWORD_OLD_PASSWD      0
#define STRING_FIELD_SYSTEM_CHANGE_PASSWORD_NEW_PASSWD      1
#define BOOLEAN_FIELD_RET_SYSTEM_CHANGE_PASSWORD_SUCCESS    0

#endif // #define __SRVDEF_H_
