/**
 * @file Util.cpp
 * @brief core data structure and utility's defination
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/src/Util.cpp $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 28, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#include "aco/Util.h"
#include <cstring>

unsigned char userprofile::encodecompressionflag(){
  return 0|
    (compress&0x1)|
    (compressionalgorithm&0x7)<<1|
    (compressionlevel&0xF)<<4;
}

void userprofile::decodecompressionflag(unsigned char code){
  compress=code&0x1;
  compressionalgorithm=(code>>1)&0x7;
  compressionlevel=(code>>4)&0xF;
}

unsigned int userprofile::encodefirstflag(){
  // encode several bit fields into a 4 bytes int field
  // fields: encipher,ciphertype,ciphersubtype
  //         checksum,checksumtype,compress,
  //         compressionalgorithm,compressionlevel
  return 0|
    ((compress&0x1)<<24)|
    ((compressionalgorithm&0x7)<<25)|
    ((compressionlevel&0xF)<<28);
}

unsigned int userprofile::encodesecondflag(){
  // encode version and reserved bit fields in to a int field
  return 0|(version&0x7);
}

void userprofile::decodefirstflag(unsigned int code){
  compress=(code>>24)&0x1;
  compressionalgorithm=(code>>25)&0x7;
  compressionlevel=(code>>28)&0xF;  
}

void userprofile::decodesecondflag(unsigned int code){
  version=code&0x7;
}
