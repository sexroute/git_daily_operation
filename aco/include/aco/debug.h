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
	vsprintf(out, fmt, body); // ��ע����ʽ��������ַ��� fmtt 
	va_end(body); // ������ַ��� ou 
	OutputDebugString(out); // ��ע�������ʽ������ַ����������� 
}  
#define TRACE _trace
#define ASSERT(x) {if(!(x)) _asm{int 0x03}} 
#define VERIFY(x) {if(!(x)) _asm{int 0x03}} // ��ע��Ϊ���԰汾ʱ�����ж���Ч 
#else 
#define ASSERT(x) 
#define VERIFY(x) x // ��ע��Ϊ���а汾ʱ�������ж� 
#define TRACE 
#endif   ///_DEBUG_

#endif // __DEBUG_H__ 

