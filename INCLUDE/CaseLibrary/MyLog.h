#pragma once

// 案例库的日志类

class CMyLog
{
public:
	CMyLog(void);
	~CMyLog(void);

public:
	static void Trace_CreateHead_ID(int nID_);

	static void Trace_Log(char* str);

    static void Clear_Log();
};
