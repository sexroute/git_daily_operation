/**
 * @file Util.h
 * @brief core data structure and utility's declaration
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 107 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/Util.h $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 8, 2004
 *
 * $LastChangedDate: 2008-02-21 11:13:07 +0800 (Thu, 21 Feb 2008) $
 */

#ifndef __UTIL_H_
#define __UTIL_H_

#ifdef WIN32

#ifdef COMPILINGACO
  #define ACOEXPORT   __declspec(dllexport)
#else
  #define ACOEXPORT   __declspec(dllimport)
#endif

#else
#define ACOEXPORT
#endif

//#include <ace/CDR_Stream.h>
#include <cstring>
#include <string>
#include <list>

/**
 * @brief length of username in bytes
 */
#define USERNAME_LEN 64

/**
 * @brief length of password in bytes
 */
#define PASSWD_LEN 64

/**
 * @brief length of license key in bytes
 */
#define LICENSE_LEN 1024

/**
 * @brief length of domain in bytes
 */
#define DOMAIN_LEN 128

/**
 * @brief length of session id
 */
#define SESSIONID_LEN 128

/**
 * @brief service version 1
 */
#define SRV_VERSION_1 0

/**
 * @brief service version 2
 */
#define SRV_VERSION_2 1

/**
 * @brief service version 3
 */
#define SRV_VERSION_3 2

/**
 * @brief service version 4
 */
#define SRV_VERSION_4 3

/**
 * @brief service version 5
 */
#define SRV_VERSION_5 4

/**
 * @brief service version 6
 */
#define SRV_VERSION_6 5

/**
 * @brief service version 7
 */
#define SRV_VERSION_7 6

/**
 * @brief service version 8
 */
#define SRV_VERSION_8 7

/**
 * @brief total compression algorithm number
 */
#define COMPRESSION_ALGORITHM_NUMBER 8

/**
 * @brief Deflation compression algorithm
 *
 * comment: delfation is a LZ77 variant compress method which 
 * emits compressed data as sequence of blocks
 */
#define DEFLATION_COMPRESS 0

/**
 * @brief no compression
 */
#define NO_COMPRESSION 0

/**
 * @brief best speed compression
 */
#define BEST_SPEED 1

/**
 * @brief default compression level
 */
#define DEFAULT_COMPRESSION 5

/**
 * @brief best compression
 */
#define BEST_COMPRESSION 9

/**
 * @brief compression level 1
 */
#define LEVEL1_COMPRESSION

/**
 * @brief compression level 2
 */
#define LEVEL2_COMPRESSION

/**
 * @brief compression level 3
 */
#define LEVEL3_COMPRESSION

/**
 * @brief compression level 4
 */
#define LEVEL4_COMPRESSION

/**
 * @brief compression level 5
 */
#define LEVEL5_COMPRESSION

/**
 * @brief compression level 6
 */
#define LEVEL6_COMPRESSION

/**
 * @brief compression level 7
 */
#define LEVEL7_COMPRESSIO

/**
 * @brief compression level 8
 */
#define LEVEL8_COMPRESSION

/**
 * @brief compression level 9
 */
#define LEVEL9_COMPRESSION

/**
 * @struct userprofile util.h "aco/util.h"
 * @brief user profile structure
 */
struct ACOEXPORT userprofile{
  /**
   * @brief plain user name
   */
  char username[USERNAME_LEN+1];

  /**
   * @brief plain user password
   */
  char passwd[PASSWD_LEN+1];

  /**
   * @brief license key
   *
   * digital signed license key used to validate the client side function.
   */
  char license[LICENSE_LEN+1];

  /**
   * @brief user domain
   *
   * information used to identify the user's domain
   */
  char domain[DOMAIN_LEN+1];

  /**
   * @brief session id;
   *
   * session id used to identify current user
   */
  char sessionid[SESSIONID_LEN+1];

  /**
   * @brief logged flag
   * whether the user is loged on the server
   */
  char logged;

  /**
   * 8 bit flags used to identify client's compress properties
   * 
   * +------------------------------------------+
   * | bit0                                bit7 |
   * |  |    0   |   000   |      0000      |   |
   * |   compress algorithm  compress level     |
   * +------------------------------------------+
   */

  /**
   * @brief whether the following communication is compressed.
   * 
   * bit0: 0=plain stream
   *       1=compressed stream
   */
  unsigned int compress:1;

  /**
   * @brief compression algorithm
   * 
   * +------------------------------------------+
   * |           | bit1                    bit3 |
   * | type      |  0           0            0  |
   * +------------------------------------------+
   * | deflation |  0           0            0  |
   * +------------------------------------------+
   *
   * comment: deflation is an LZ77 variant compress method which 
   * emits compressed data as sequence of blocks.
   */
  unsigned int compressionalgorithm:3;

  /**
   * @brief compression level
   * 
   * +------------------------------------------+
   * |                     | bit4          bit7 |
   * | level               |  0    0    0    0  |
   * +------------------------------------------+
   * | NO_COMPRESSION      |  0    0    0    0  |
   * | BEST_SPEED          |  0    0    0    1  |
   * | LEVEL2_COMPRESSION  |  0    0    1    0  |
   * | LEVEL3_COMPRESSION  |  0    0    1    1  |
   * | LEVEL4_COMPRESSION  |  0    1    0    0  |
   * | DEFAULT_COMPRESSION |  0    1    0    1  |
   * | LEVEL6_COMPRESSION  |  0    1    1    0  |
   * | LEVEL7_COMPRESSION  |  0    1    1    1  |
   * | LEVEL8_COMPRESSION  |  1    0    0    0  |
   * | BEST_COMPRESSION    |  1    0    0    1  |
   * +------------------------------------------+
   */
  unsigned int compressionlevel:4;

  /**
   * 32 bit flags used to identify client's properties
   * 
   * +------------------------------------------+
   * | bit0                              bit7   |
   * |  |  000  |          00000          |     |
   * |   version          reserved              |
   * | bit8                             bit15   |
   * |  |   0000                   0000   |     |
   * |               reserved                   |
   * | bit16                            bit23   |
   * |  |   0000                   0000   |     |
   * |               reserved                   |
   * | bit24                            bit31   |
   * |  |   0000                   0000   |     |
   * |               reserved                   |
   * +------------------------------------------+
   */

  /**
   * @brief client version used to identify the service version
   * 
   * +------------------------------------------+
   * |         | bit0                      bit2 |
   * | version |  0            0            0   |
   * +------------------------------------------+
   * |  1.0.0  |  0            0            0   |
   * |  2.0.0  |  0            0            1   |
   * |  3.0.0  |  0            1            0   |
   * |  4.0.0  |  0            1            1   |
   * |  5.0.0  |  1            0            0   |
   * |  6.0.0  |  1            0            1   |
   * |  7.0.0  |  1            1            0   |
   * |  8.0.0  |  1            1            1   |
   * +------------------------------------------+
   * comment: 3 bit fields ensure concurrent existence of eight  service
   * versions
   */
  unsigned int version:3;

  /**
   * @brief reserved bit fields
   */
  unsigned int reserved:29;

  /**
   * @brief encode 8 bits flag that contains compression properities
   */
  unsigned char encodecompressionflag();

  /**
   * @brief decode 8 bits flag that contains compression properities
   * @param code encoded flag
   */
  void decodecompressionflag(unsigned char code);

  /**
   * @brief encode first 32 bits flag
   * @return encoded integer
   */
  unsigned int encodefirstflag();

  /**
   * @brief encode second 32 bits flag
   * @return encoded integer
   */
  unsigned int encodesecondflag();

  /**
   * @brief decode first 32 bits flags
   * @param code encoded flags
   */
  void decodefirstflag(unsigned int code);

  /**
   * @brief decode second 32 bits flags
   * @param code encoded flags
   */
  void decodesecondflag(unsigned int code);
  
  /**
   * @brief default constructor
   */
  userprofile(){
    std::memset(this,0,sizeof(userprofile));
  }
};

/**
 * @struct sockheader util.h "aco/util.h"
 * @brief sock stream header
 */
struct sockheader{
  /**
   * @brief aco flag
   *
   * acoflag is used to verify whether the packet is a
   * valid aco packet. which always equals to 'A''C'
   * 'O''P'.
   */
  static const char acoflag[4];

  /**
   * @brief byte order
   * 
   * comment: 1 stands for little-endian
   *          0 stands for big-endian
   */
  unsigned char byteorder;

  /**
   * @brief client version
   */
  unsigned char version;

  /**
   * @brief header length
   * 
   * comment: mutable field that used to retrieve
   * the actual package length from const functions
   * and methods
   */
  mutable unsigned short headerlength;

  /**
   * @brief request service code
   */
  unsigned int servicecode;

  /**
   * @brief indicating the body is been compressed or not
   */
  unsigned char compress;

  /**
   * @brief body length;
   */
  unsigned int bodylength;

  /**
   * @brief body's original size before compression
   * Equal to bodylength when there is no compression.
   */
  unsigned int origlength;

  /**
   * @brief user domain
   *
   * information used to identify the user's domain
   */
  char domain[DOMAIN_LEN];

  /**
   * @brief session id;
   *
   * session id used to identify current user
   */
  char sessionid[SESSIONID_LEN];
};

/**
 * @brief operator<< that used to pack the userprofile structure
 * to ACE_OutputCDR
 * @param cdr ACE_OutputCDR that contains the userprofile
 * @param profile user profile information
 */
//int operator<< (ACE_OutputCDR &cdr, const userprofile& profile);

/**
 * @brief operator>> that used to extract the user profile
 * information from  ACE_InputCDR
 * @param cdr ACE_InputCDR that contains the user profile
 * @param profile userprofile structure
 */
//int operator>> (ACE_InputCDR &cdr, userprofile& profile);

#endif // __UTIL_H_
