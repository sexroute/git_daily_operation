/**
 * @file ACOConfig.h
 * @brief ACO configuration provider
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/ACOConfig.h $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 23, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#ifndef __ACOCONFIG_H_
#define __ACOCONFIG_H_

#include <iostream>
#include "aco\Util.h"

// forward declaration
class ACOConfigSAX2Handler;

/**
 * @class ACOConfig
 * @brief ACO configuration provider
 */
class ACOEXPORT ACOConfig{
public:
  /**
   * @brief singleton object instance factory method
   * @return singleton object
   */
  static ACOConfig* Instance();

  /**
   * @brief retrieve server information
   * @return server information
   */
  const std::string& GetServer(){
    return __server;
  }

  /**
   * @brief retrieve license
   * @return license
   */
  const std::string& GetLicense(){
    return __license;
  }

  /**
   * @brief retrieve domain
   * @return domain
   */
  const std::string& GetDomain(){
    return __domain;
  }

  /**
   * @brief retrieve whether packet should be commpressed
   */
  bool IsCompressEnabled(){
    return __compress;
  }

  /**
   * @brief retrieve compression algorithm
   * @return commpression algorithm
   */
  unsigned int GetCompressionAlgorithm(){
    return __compressionalgorithm;
  }

  /**
   * @brief retrieve compression level
   * @return compression level
   */
  unsigned int GetCompressionLevel(){
    return __compressionlevel;
  }

  /**
   * @brief retrieve service version
   * @return service version
   */
  unsigned int GetServiceVersion(){
    return __serviceversion;
  }

  /**
   * @brief retrieve error message in constructing singleton
   * @return error message
   */
  const std::string& GetErrMessage(){
    return __error;
  }

  /**
   * @brief retrieve packet header buffer's length
   * @return packet header buffer's length
   */
  const size_t GetHeaderBlockInitSize(){
    return __header_block_init_size;
  }

  /**
   * @brief retrieve packet body buffer's length
   * @return packet body buffer's length
   */
  const size_t GetBodyBlockInitSize(){
    return __body_block_init_size;
  }

  /**
   * @brief retrieve timeout seconds
   * @return timeout seconds
   */
  const unsigned int GetTimeoutSec(){
    return __timeout_seconds;
  }
  
  /**
   * @brief ssl
   */
  const bool GetSSL(){
    return __ssl;
  }
  
  /**
   * @brief certificate
   */
  const std::string& GetCertificate(){
    return __certificate;
  }

  /**
   * @brief private key
   */
  const std::string& GetPrivateKey(){
    return __privkey;
  }

  /**
   * @brief ca
   */
  const std::string& GetCA(){
    return __ca;
  }

private:
  /**
   * @brief private constructor
   */
  ACOConfig();

  /**
   * @brief server information
   */
  static std::string __server;

  /**
   * @brief license
   */
  static std::string __license;

  /**
   * @brief user's domain
   */
  static std::string __domain;

  /**
   * @brief compress flag
   */
  static bool __compress;

  /**
   * @brief compression algorithm
   */
  static unsigned int __compressionalgorithm;

  /**
   * @brief compression level
   */
  static unsigned int __compressionlevel;

  /**
   * @brief service version
   */
  static unsigned int __serviceversion;

  /**
   * @brief packet header buffer length
   *
   * unit: bytes
   */
  static size_t __header_block_init_size;

  /**
   * @brief packet body buffer length
   * 
   * unit: bytes
   */
  static size_t __body_block_init_size;

  /**
   * @brief timeout seconds
   */
  static unsigned int __timeout_seconds;

  /**
   * @brief error message
   */
  static std::string __error;

  /**
   * @brief singleton object
   */
  static ACOConfig __instance;
  
  /**
   * @brief use ssl
   */
  static bool __ssl;
  
  /**
   * @brief ca
   */
  static std::string __ca;
  
  /**
   * @brief certificate
   */
  static std::string __certificate;
  
  /**
   * @brief private key
   */
  static std::string __privkey;

  /**
   * @brief friend class declaration
   */
  friend class ACOConfigSAX2Handler;
};
#endif // __ACOCONFIG_H_
