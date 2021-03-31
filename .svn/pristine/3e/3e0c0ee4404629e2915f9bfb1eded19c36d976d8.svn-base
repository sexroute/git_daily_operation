#ifndef __DEBUG_H__ 
#define __DEBUG_H__ 
#include ".\tracetool.h"
#ifdef _DEBUG 

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

void _trace(char *fmt, ...)
{ 
	char out[1024]; 
	va_list body; 
	va_start(body, fmt); 
	vsprintf(out, fmt, body); // 译注：格式化输入的字符串 fmtt 
	va_end(body); // 到输出字符串 ou 
	OutputDebugString(out); // 译注：输出格式化后的字符串到调试器 
}  
#define TRACE _trace
#define ASSERT(x) {if(!(x)) _asm{int 0x03}} 
#define VERIFY(x) {if(!(x)) _asm{int 0x03}} // 译注：为调试版本时产生中断有效 
#else 
#define ASSERT(x) 
#define VERIFY(x) x // 译注：为发行版本时不产生中断 
#define TRACE 
#endif   ///_DEBUG_

#endif // __DEBUG_H__ 

