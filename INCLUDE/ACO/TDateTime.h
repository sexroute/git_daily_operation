/*
������TDateTime
����������ʱ����
���ã�1. ��һ������������ʱ���ַ����н���������ʱ����Ϣ 
2. �ṩһЩ���õ�����ʱ���У���㷨
��ע��
����֧�ֵ�����ʱ���ʽ���£�
5�����룩
4:5(�ķ�����)
5:3:6(��ʱ��������)��ע��������Сʱ�������賿��ʱ������ʱ�䣩
2-28��2��28�գ�
2-28 5:3:6��2��28�գ�
2008-2-28��2008��2��28�գ�
2008-2-28 17:3:6��2008��2��28��17ʱ3��6�룩

��֧��վλ����ʽ��
-2-  ���趨�·�Ϊ2�£��������ڲ��õ�ǰֵ
2008-- ���趨��
:23: ���趨��
-- :: ȫ��ʡ�ԣ����õ�ǰ����ʱ����ΪĬ��ֵ

������ܽ�����ָ���Ĳ��֣������Ĭ��ֵ��Ĭ��ֵΪ��ǰ���ڣ�
��ʷ��
2008-11-21     ����⣨kevdmx@sina.com��  ����
*/

#ifndef _TDATETIME_H_20081121_
#define _TDATETIME_H_20081121_

//��windows�£����ǿ��ʹ��32λ��time_t���붨�����º꣺
//#ifndef _USE_32BIT_TIME_T
//#define _USE_32BIT_TIME_T
//#endif
#include <Windows.h>
#include <time.h>

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

class TDateTime
{
public:
	//��
	unsigned short sYear;
	//��
	unsigned char sMonth;
	//��
	unsigned char sDay;
	//ʱ
	unsigned char sHour;
	//��
	unsigned char sMinute;
	//��
	unsigned char sSecond;

public:
	//���캯�������õ�ǰ������ʱ����ΪĬ��ֵ
	TDateTime();
	//���캯������time_t���͵ı�����ȡ������ʱ��
	TDateTime(time_t t);
	//���ַ����н���������ʱ�䣬δ�������Ĳ��֣����õ�ǰĬ��ֵ
	BOOL ParseDateTimeString(const char *szDateTime);
	//����Ϊ��ǰ������ʱ��
	BOOL LoadCurrentDateTime();
	//ת��ΪUNIX��ʽ��time_tʱ����������
	time_t ToUnixDatetime();
	//�����趨Ϊ��time_t���ͱ���ָ��������ʱ��ֵ
	void FromUnixDatetime(time_t t);
	//У�鵱ǰ���������ʱ�������Ƿ���ȷ
	BOOL Validate();
	//У��һ��TDateTime���ͱ���������ʱ�������Ƿ���ȷ
	static BOOL Validate(TDateTime *obDateTime);
	//�������Ƿ�������
	static BOOL IsLeapYear(int year);
	//У�����������Ƿ���ȷ
	static BOOL ValidateDate(int year);
	//У���������ݺ��·��Ƿ���ȷ
	static BOOL ValidateDate(int year,int month);
	//ȡ�ø�������ݣ��������·ݺ��е�����
	static int GetDaysOfMonth(int year, int month);
	//У������������������Ƿ���ȷ
	static BOOL ValidateDate(int year, int month, int day);
	//���������Сʱ���ݣ��Ƿ���ȷ
	static BOOL ValidateTime(int hour);
	//У�������Сʱ���������Ƿ���ȷ
	static BOOL ValidateTime(int hour,int minute);
	//У�������ʱ�������Ƿ���ȷ
	static BOOL ValidateTime(int hour, int minute, int second);
	//У�����������ʱ�������Ƿ���ȷ
	static BOOL ValidateDateTime(int year, int month, int day,
		int hour, int minute, int second);


	static char* strptime(const char *buf, const char *format, struct tm *tm);
	static int	strptime_getnum (const char *buf,int *num,int *bi,int *fi,int min,int max);

private:
	//token���Ͷ���
	typedef enum TokenType
	{
		TT_Null = 0,
		TT_Number =1,
		TT_Minus = 2,
		TT_Colon = 4,
		TT_Blank = 8
	};
	//����ʱ�����Ͷ���
	typedef enum TimePart
	{
		TP_Second = 1,
		TP_Minute = 2,
		TP_Hour = 4,
		TP_Day = 8,
		TP_Month = 16,
		TP_Year = 32
	};

private:    
	//����ǰ�����������
	void ZeroDateTime(void);
	//�����ַ�ȡ�ø��ַ�����������
	TDateTime::TokenType GetTokenType(char c);
};

#endif  //#ifndef _TDATETIME_H_20081121_