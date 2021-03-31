/** @interface CZipInterface
  *  @brief 压缩和解压缩接口文件
  *  @author  marble
  *  @version 1.0
  *  @date   2006-4-16
  */

#ifndef _ZIPLIBINTERFACE_DSE_MARBLE_
#define _ZIPLIBINTERFACE_DSE_MARBLE_


#ifdef WRAPPERZLIB_EXPORTS
#define WRAPPERZLIB_API __declspec(dllexport)
#else
#define WRAPPERZLIB_API __declspec(dllimport)
#pragma comment(lib,"ziplib.lib")
#endif


class WRAPPERZLIB_API CZipImpl
{
public:
	CZipImpl(void);
	~CZipImpl(void);
public:
	
	/**解压函数
	  *@param: pInput_ 输入需要解压的字节流指针
	  *@param: pOutput_   解压后输出的的字节流指针,一定确保足够大
	  *@param: iInputLen_  输入的需要解压字节流长度
	  *@param: iOutputLen_ 输出的字节流指针所指向内存的字节长度
      *@param: iUnCompressedByteNum_ 解压缩后实际字节流长度
	  *@return true表示解压缩成功，false表示失败
	  */
	bool UnZip(BYTE *pInput_,long iInputLen_,BYTE *pOutput_,long iOutputLen_,long &iUnZippedByteNum_);
	/**压缩函数
	  *@param: pInput_ 输入需要压缩的字节流指针
	  *@param: pOutput_   压缩后输出的的字节流指针
	  *@param: iInputLen_   需要压缩的字节流长度
	  *@param: iOutputLen_  压缩后输出的字节流指针所指向内存的字节长度
	  *@param: iZippedByteNum_ 解压缩后实际字节流长度
	  *@return true表示解压缩成功，false表示失败
	  */
	bool Zip(BYTE *pInput_,long iInputLen_,BYTE *pOutput_,long iOutputLen_,long &iZippedByteNum_);
	
	/**返回错误信息
	  */
	const char * GetLastError();
};
#endif//_ZIPLIBINTERFACE_DSE_MARBLE_