#include <stdlib.h>
#include <string.h>
#include <aco/tdatetime.h>

//中国的时间区域为格林威治的东八区，故需要一个时间偏移量
const int TIMEZONE_8 = 8*60*60;

TDateTime::TDateTime()
{
	LoadCurrentDateTime();
}

TDateTime::TDateTime(time_t t)
{
	FromUnixDatetime(t);
}    

TDateTime::TokenType 
TDateTime::GetTokenType(char c)
{
	if ((c>='0') && (c<='9'))
		return TT_Number;
	else if ('-'==c){
		return TT_Minus;
	}else if ('/'==c){
		return TT_Minus;
	}else if (' ' == c){
		return TT_Blank;
	}else if(':'==c){
		return TT_Colon;
	}else{
		return TT_Null;
	}
}

BOOL 
TDateTime::ParseDateTimeString(const char *szDateTime)
{
	int len = strlen(szDateTime) - 1;
	TimePart timePart = TP_Second;
	int pw =1;//加权
	signed short year=-1;
	signed char month=-1,day=-1,hour=-1,minute=-1,second=-1;
	//过滤尾部空格
	while((len>=0) && (' ' == szDateTime[len]))
		len--;
	if (len<0) return FALSE;


	while(len>=0){

		char c = szDateTime[len];

		switch( GetTokenType(c))
		{
		case TT_Null: goto ParseDateTimeString_FALSE;
		case TT_Number:
			switch(timePart)
			{
			case TP_Second:
				if (second<0 )
					second = c-'0';
				else
					second += (c-'0') * pw;
				pw *= 10;
				break;
			case TP_Minute:
				if (minute<0 )
					minute = c-'0';
				else
					minute += (c-'0') * pw;
				pw *= 10;
				break;
			case TP_Hour:
				if (hour<0 )
					hour = c-'0';
				else
					hour += (c-'0') * pw;
				pw *= 10;
				break;
			case TP_Day:
				if (day<0 )
					day = c-'0';
				else
					day += (c-'0') * pw;
				pw *= 10;
				break;
			case TP_Month:
				if (month<0 )
					month = c-'0';
				else
					month += (c-'0') * pw;
				pw *= 10;
				break;
			case TP_Year:
				if (year<0 )
					year = c-'0';
				else
					year += (c-'0') * pw;
				pw *= 10;
				break;
			default:
				return FALSE;
			}
			break;
		case TT_Minus:
			switch(timePart)
			{
			case TP_Second: //如果没有给定时间信息，则跳过，直接升级到日期
				pw =1;
				timePart = TP_Month; //提升                
				day = second;
				second = -1;  //将解析到的秒信息改为日期信息
				break;
			case TP_Minute: goto ParseDateTimeString_FALSE;                
			case TP_Hour: goto ParseDateTimeString_FALSE;
			case TP_Day:
				pw =1;
				timePart = TP_Month; //提升                
				break;
			case TP_Month:
				pw =1;
				timePart = TP_Year; //提升                
				break;
			case TP_Year: goto ParseDateTimeString_FALSE;                
			default: goto ParseDateTimeString_FALSE;
			}
			break;
		case TT_Colon:
			switch(timePart)
			{
			case TP_Second:
				pw =1;
				timePart = TP_Minute; //提升                    
				break;
			case TP_Minute: 
				pw =1;
				timePart = TP_Hour; //提升                    
				break;
			case TP_Hour: goto ParseDateTimeString_FALSE;
			case TP_Day: goto ParseDateTimeString_FALSE;                
			case TP_Month: goto ParseDateTimeString_FALSE;                
			case TP_Year: goto ParseDateTimeString_FALSE;                
			default: goto ParseDateTimeString_FALSE;
			}
			break;
		case TT_Blank:
			if (TP_Hour == timePart){
				timePart = TP_Day; //提升
				pw =1;
			}else if (TP_Year == timePart){  //前导空格
				goto ParseDateTimeString_OK;//认为结束                
			}else{//在其他部分不能出现空格
				goto ParseDateTimeString_FALSE;
			}
			break;
		}
		len -- ;
	}

ParseDateTimeString_OK:
	if (year>0)
		sYear = year;
	if (month>0)
		sMonth = month;
	if (day>0)
		sDay = day;
	if (hour>=0)
		sHour = hour;
	if (minute>=0)
		sMinute = minute;
	if (second>=0)
		sSecond = second;

	if (Validate()){
		return TRUE;
	}else{
		ZeroDateTime();
		return FALSE;
	}

ParseDateTimeString_FALSE:
	ZeroDateTime();
	return FALSE;
}

BOOL 
TDateTime::LoadCurrentDateTime()
{    
	time_t t;
	time(&t);
	FromUnixDatetime(t);    
	return TRUE;
}

void 
TDateTime::FromUnixDatetime(time_t t)
{
	t += TIMEZONE_8;
	struct tm *tt = gmtime( &t);
	sYear = tt->tm_year+1900;
	sMonth = tt->tm_mon+1;
	sDay = tt->tm_mday;
	sHour = tt->tm_hour;
	sMinute = tt->tm_min;
	sSecond = tt->tm_sec;    
	//free(tt);
}

void 
TDateTime::ZeroDateTime(void)
{
	sYear = 0;
	sMonth = 0;
	sDay = 0;
	sHour = 0;
	sMinute = 0;
	sSecond = 0;
}


/// <summary>
/// 判定给定的年份是否是润年
/// </summary>
/// <param name="year">需要判定的年份</param>
/// <returns>true:给定的年份是润年。false:给定的年份不是润年。</returns>
BOOL 
TDateTime::IsLeapYear(int year)
{
	return ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0));                               
}

/// <summary>
/// 判定给定的年份是否有效。
/// </summary>
/// <param name="year">给定的年份</param>
/// <returns>true:有效,false:无效</returns>
BOOL 
TDateTime::ValidateDate(int year)
{
	return (year > 0) && (year <= 9999);
}

/// <summary>
/// 判定给定的年月是否有效
/// </summary>
/// <param name="year">给定的年份</param>
/// <param name="month">给定的月份</param>
/// <returns>true:有效。false:无效。</returns>
BOOL 
TDateTime::ValidateDate(int year,int month)
{
	if (!ValidateDate(year))
		return FALSE;
	return (month > 0) && (month < 13);
}

/// <summary>
/// 得到一个月份的天数
/// </summary>
/// <param name="year">年</param>
/// <param name="month">月</param>
/// <returns>返回该年该月的总天数，如果给定的参数有错误，则返回0</returns>
int 
TDateTime::GetDaysOfMonth(int year, int month)
{
	if (!ValidateDate(year, month))
	{
		return 0;
	}

	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		return 30;
	}
	else if (month == 1 || month == 3 || month == 5
		|| month == 7 || month == 8 || month == 10 || month == 12)
	{
		return 31;
	}
	else if (2 == month)
	{
		if (IsLeapYear(year))//如果是闰年
		{
			return 29;
		}
		else
		{
			return 28;
		}
	}

	return 0;
}

/// <summary>
/// 判定给定的年月日是否是一个有效的日期
/// </summary>
/// <param name="year">给定的年份</param>
/// <param name="month">给定的月份</param>
/// <param name="day">给定的日子</param>
/// <returns>true:给定的年月日是一个有效的日期。false:不是一个有效的日期。</returns>
BOOL 
TDateTime::ValidateDate(int year, int month, int day)
{
	if (!ValidateDate(year, month))
		return FALSE;

	if ((day < 1) || (day > GetDaysOfMonth(year, month)))
		return FALSE;

	return TRUE;                        
}

/// <summary>
/// 判定给定的小事是否有效
/// </summary>
/// <param name="hour">给定的小时</param>
/// <returns>true:有效;false:无效</returns>
BOOL 
TDateTime::ValidateTime(int hour)
{
	return (hour >= 0) && (hour < 24);
}

/// <summary>
/// 判定给定的小时和分钟是否有效。
/// </summary>
/// <param name="hour">给定的小时</param>
/// <param name="minute">给定的分钟</param>
/// <returns>true:有效;false:无效</returns>
BOOL 
TDateTime::ValidateTime(int hour,int minute)
{
	if (!ValidateTime(hour))
		return FALSE;
	return (minute >= 0) && (minute < 60);
}

/// <summary>
/// 判定给定的小时、分钟、秒时否有效
/// </summary>
/// <param name="hour">给定的小时</param>
/// <param name="minute">给定的分钟</param>
/// <param name="second">给定的秒</param>
/// <returns>true:有效;false:无效</returns>
BOOL 
TDateTime::ValidateTime(int hour, int minute, int second)
{
	if (!ValidateTime(hour,minute))
		return FALSE;
	return (second >= 0) && (second < 60);
}

/// <summary>
/// 判定给定的年月日时分秒是否是一个有效的日期时间
/// </summary>
/// <param name="year">给定的年份</param>
/// <param name="month">给定的月份</param>
/// <param name="day">给定的日子</param>
/// <param name="hour">给定的小时</param>
/// <param name="minute">给定的分钟</param>
/// <param name="second">给定的秒</param>
/// <returns>true:有效;false:无效</returns>
BOOL 
TDateTime::ValidateDateTime(int year, int month, int day,
							int hour, int minute, int second)
{
	return ValidateDate(year, month, day)
		&& ValidateTime(hour, minute, second);
}

BOOL 
TDateTime::Validate()
{
	return Validate(this);
}    

BOOL 
TDateTime::Validate(TDateTime *obDateTime)
{
	return ValidateDateTime(obDateTime->sYear,obDateTime->sMonth, obDateTime->sDay,
		obDateTime->sHour, obDateTime->sMinute, obDateTime->sSecond);
}

time_t 
TDateTime::ToUnixDatetime()
{    
	tm tt;
	tt.tm_year = sYear - 1900;
	tt.tm_mon = sMonth -1;
	tt.tm_mday = sDay;
	tt.tm_hour = sHour;
	tt.tm_min = sMinute;
	tt.tm_sec = sSecond;
	return mktime(&tt); 
}

char* TDateTime::strptime(const char *buf, const char *format, struct tm *tm)
{
	int bi = 0;
	int fi = 0;
	bool percent = false;

	if (!buf || !format)
		return 0;

	while (format[fi] != '\0')
	{
		if (percent)
		{
			percent = false;
			switch (format[fi])
			{
			case '%':                        // an escaped %
				if (buf[bi] == '%')
				{
					++fi;
					++bi;
				}
				else
					return const_cast<char*> (buf + bi);
				break;

				/* not supported yet: weekday via locale long/short names
				case 'a':                        / * weekday via locale * /
				/ * FALL THROUGH * /
				case 'A':                        / * long/short names * /
				break;
				*/

				/* not supported yet:
				case 'b':                        / * month via locale * /
				/ * FALL THROUGH * /
				case 'B':                        / * long/short names * /
				/ * FALL THROUGH * /
				case 'h':
				break;
				*/

				/* not supported yet:
				case 'c':                        / * %x %X * /
				break;
				*/

				/* not supported yet:
				case 'C':                        / * date & time -  * /
				/ * locale long format * /
				break;
				*/

			case 'd':                        /* day of month (1-31) */
				/* FALL THROUGH */
			case 'e':
				if (!strptime_getnum
					(buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
					return const_cast<char*> (buf + bi);

				break;

			case 'D':                        /* %m/%d/%y */
				if (!strptime_getnum
					(buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
					return const_cast<char*> (buf + bi);

				--fi;
				tm->tm_mon--;

				if (buf[bi] != '/')
					return const_cast<char*> (buf + bi);

				++bi;

				if (!strptime_getnum
					(buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
					return const_cast<char*> (buf + bi);

				--fi;
				if (buf[bi] != '/')
					return const_cast<char*> (buf + bi);
				++bi;
				if (!strptime_getnum
					(buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
					return const_cast<char*> (buf + bi);
				if (tm->tm_year < 69)
					tm->tm_year += 100;
				break;

			case 'H':                        /* hour (0-23) */
				/* FALL THROUGH */
			case 'k':
				if (!strptime_getnum
					(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
					return const_cast<char*> (buf + bi);
				break;

				/* not supported yet:
				case 'I':                        / * hour (0-12) * /
				/ * FALL THROUGH * /
				case 'l':
				break;
				*/

			case 'j':                        /* day of year (0-366) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
					return const_cast<char*> (buf + bi);

				tm->tm_yday--;
				break;

			case 'm':                        /* an escaped % */
				if (!strptime_getnum
					(buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
					return const_cast<char*> (buf + bi);

				tm->tm_mon--;
				break;

			case 'M':                        /* minute (0-59) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
					return const_cast<char*> (buf + bi);

				break;

				/* not supported yet:
				case 'p':                        / * am or pm for locale * /
				break;
				*/

				/* not supported yet:
				case 'r':                        / * %I:%M:%S %p * /
				break;
				*/

			case 'R':                        /* %H:%M */
				if (!strptime_getnum
					(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
					return const_cast<char*> (buf + bi);

				--fi;
				if (buf[bi] != ':')
					return const_cast<char*> (buf + bi);
				++bi;
				if (!strptime_getnum
					(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
					return const_cast<char*> (buf + bi);

				break;

			case 'S':                        /* seconds (0-61) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
					return const_cast<char*> (buf + bi);
				break;

			case 'T':                        /* %H:%M:%S */
				if (!strptime_getnum
					(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
					return const_cast<char*> (buf + bi);

				--fi;
				if (buf[bi] != ':')
					return const_cast<char*> (buf + bi);
				++bi;
				if (!strptime_getnum
					(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
					return const_cast<char*> (buf + bi);

				--fi;
				if (buf[bi] != ':')
					return const_cast<char*> (buf + bi);
				++bi;
				if (!strptime_getnum
					(buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
					return const_cast<char*> (buf + bi);

				break;

			case 'w':                        /* day of week (0=Sun-6) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
					return const_cast<char*> (buf + bi);

				break;

				/* not supported yet: date, based on locale
				case 'x':                        / * date, based on locale * /
				break;
				*/

				/* not supported yet:
				case 'X':                        / * time, based on locale * /
				break;
				*/

			case 'y':                        /* the year - 1900 (0-99) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
					return const_cast<char*> (buf + bi);

				if (tm->tm_year < 69)
					tm->tm_year += 100;
				break;

			case 'Y':                        /* the full year (1999) */
				if (!strptime_getnum
					(buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
					return const_cast<char*> (buf + bi);

				tm->tm_year -= 1900;
				break;

			default:                        /* unrecognised */
				return const_cast<char*> (buf + bi);
			} /* switch (format[fi]) */

		}
		else
		{ /* if (percent) */
			if (format[fi] == '%')
			{
				percent = true;
				++fi;
			}
			else
			{
				if (format[fi] == buf[bi])
				{
					++fi;
					++bi;
				}
				else
					return const_cast<char*> (buf + bi);
			}
		} /* if (percent) */
	} /* while (format[fi] */

	return const_cast<char*> (buf + bi);
}

int	TDateTime::strptime_getnum (const char *buf,
					 int *num,
					 int *bi,
					 int *fi,
					 int min,
					 int max)
{
	int i = 0, tmp = 0;

	while (isdigit (buf[i]))
	{
		tmp = (tmp * 10) + (buf[i] - '0');
		if (max && (tmp > max))
			return 0;
		++i;
	}

	if (tmp < min)
		return 0;
	else if (i)
	{
		*num = tmp;
		(*fi)++;
		*bi += i;
		return 1;
	}
	else
		return 0;
}

/*
int main(int argc, char** argv)
{
	TDateTime dtm;

	if (argc<2){
		printf("parsedt datetime_string");
		return 1;
	}

	if (!dtm.ParseDateTimeString(argv[1])){
		printf("Error!\n");
		return 1;
	}

	printf("year:%d, month:%d, day:%d, hour:%d, minute:%d, second:%d\n",
		dtm.sYear, dtm.sMonth, dtm.sDay, dtm.sHour, dtm.sMinute, dtm.sSecond);

	TDateTime dtm2(dtm.ToUnixDatetime());
	printf("year:%d, month:%d, day:%d, hour:%d, minute:%d, second:%d\n",
		dtm2.sYear, dtm2.sMonth, dtm2.sDay, dtm2.sHour, dtm2.sMinute, dtm2.sSecond);
	return 0;
}
*/