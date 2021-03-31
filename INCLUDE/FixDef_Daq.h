#pragma once

///字段码定义
#include  "FixDef_Field.h"

///全局的枚举定义和宏定义
#include "DSE_Define.h"

#define     SERVICE_SERVICE_ON_TEST                       2000

/**发送实时数据到中间模块---------------------------------------------------*/

/**发送指定机组的振动测点实时所有特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *            是否需要判断报警       FIELD_SERVICE_JUDGE_ALARM
  *            快变测点个数           FIELD_SERVICE_FC_CHANN_BUFFSIZE
  *            快变测点Buff           FIELD_SERVICE_CHANN_BUFF
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
 *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
  *            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            快变测点个数            FIELD_SERVICE_FC_CHANN_BUFFSIZE
  *            快变测点报警类型Buff    FIELD_SERVICE_CHANN_BUFF
  */
#define     SERVICE_SERVICE_ON_RECEIVEALLVALUE        24000

/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION　
  *            是否需要判断报警                FIELD_SERVICE_JUDGE_ALARM
  *            快变测点个数           FIELD_SERVICE_CHANN_BUFFSIZE
  *            快变测点Buff           FIELD_SERVICE_CHANN_BUFF
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            快变测点个数            FIELD_SERVICE_CHANN_BUFFSIZE
  *            快变测点报警类型Buff    FIELD_SERVICE_CHANN_BUFF
  */
#define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001


/**发送转速到中间件模块
  *输入参数字段码：
  *                公司ID      FIELD_SYSTEM_COMPANY
  *	    		       分厂名      FIELD_SYSTEM_FACTORY
  *                机组ID      FIELD_SERVICE_PLANTID
  *                机组的转速  FIELD_SERVICE_REV
  *                机组的转速2 FIELD_SERVICE_REV2
  *            机组的转速3     FIELD_SERVICE_REV3
  *            机组的转速4     FIELD_SERVICE_REV4
  *            机组的转速5     FIELD_SERVICE_REV5
  *返回值的字段码：无
  */
#define     SERVICE_SERVICE_ON_RECEIVEREV             24002

/**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
  *输入参数字段码：
  *                公司ID   FIELD_SYSTEM_COMPANY
  *			           分厂名   FIELD_SYSTEM_FACTORY
  *                机组ID      FIELD_SERVICE_PLANTID
  *                起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *                测点类型          FIELD_SERVICE_CHANNTYPE
  *                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *                是否需要判断报警  FIELD_SERVICE_JUDGE_ALARM
  *                快变测点个数           FIELD_SERVICE_CHANN_BUFFSIZE
  *                快变测点Buff           FIELD_SERVICE_CHANN_BUFF
  *                数据采集时间           FIELD_SERVICE_TIME
  *                数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *                开停车状态             FIELD_SYSTEM_STATUS
  *返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
  *                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
  *                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            快变测点个数            FIELD_SERVICE_CHANN_BUFFSIZE
  *            快变测点报警类型Buff    FIELD_SERVICE_CHANN_BUFF
  */
#define     SERVICE_SERVICE_ON_RECEIVEPROC            24003

/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
  *输入参数字段码：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			       分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID                          FIELD_SERVICE_PLANTID
	*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
	*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
	*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
	*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
	*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
	*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*            报警EVENTID                     FIELD_SERVICE_ALARMID
  *返回值的字段码：无
  */
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24004

/**发送指定指定设备的启停车数据到实时中间件
  *输入参数字段码：
  *             公司ID          FIELD_SYSTEM_COMPANY
  *			    分厂名          FIELD_SYSTEM_FACTORY
  *             机组ID：        FIELD_SERVICE_PLANTID
  *             起始索引        FIELD_SERVICE_ON_SEND_STARTID
  *             启停车标志位    FIELD_SERVICE_SUD_STATUS
  *             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
  *             指定机组的转速  FIELD_SERVICE_REV
  *             指定机组的转速2 FIELD_SERVICE_REV2
  *             特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *             通道个数        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *             波形数据        FIELD_SERVICE_ON_SEND_VIBWAVE
  *             一组波形点数     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *             采样频率        FIELD_SERVICE_SAMPLE_FREQ
  *             采样点数        FIELD_SERVICE_SAMPLE_NUM
  *             对应的时间       FIELD_SERVICE_TIME
  *             对应时间的微秒   FIELD_SERVICE_MICROSECOND
  *             启停车类型      FIELD_SERVICE_SUD_TYPE
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_ON_REVSUDDATA            24005

/**通知中间件服务器，需要保存数据
  *输入参数字段码：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			       分厂名   FIELD_SYSTEM_FACTORY
  *返回值字段码：无
  */
#define     SERVICE_SERVICE_ON_SAVEDATA              24006

/**通知中间件服务器，需要判断报警
  *输入参数字段码：
  *        公司ID   FIELD_SYSTEM_COMPANY
  *			   分厂名   FIELD_SYSTEM_FACTORY
  *返回值字段码：无
  */
#define     SERVICE_SERVICE_ON_JUDGEALARM            24007


/**申请指定公司、分厂、设备的启停车事件ID
  *输入参数字段码：
  *             公司ID         FIELD_SYSTEM_COMPANY
  *			        分厂名         FIELD_SYSTEM_FACTORY
  *             机组ID：       FIELD_SERVICE_PLANTID
  *             开始时间       FIELD_SERVICE_OFF_TIMESTART
  *返回值的字段码：
  *             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_OFF_REQSUDEVENTID        24008

/**通知指定启停车事件的启停车过程结束。如果成功返回1，失败返回-1
  *输入参数字段码：
  *             公司ID         FIELD_SYSTEM_COMPANY
  *			        分厂名         FIELD_SYSTEM_FACTORY
  *             机组ID：       FIELD_SERVICE_PLANTID
  *             启停车类型     FIELD_SERVICE_SUD_TYPE
  *             启停车事件ID:  FIELD_SYSTEM_EVENT_ID
  *             结束时间       FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_NOTICESUDSTOP        24009

/**启停车结束后，发送指定机组的振动启停车数据到中间件
  *输入参数字段码：
	*             公司ID          FIELD_SYSTEM_COMPANY
	*			  分厂名          FIELD_SYSTEM_FACTORY
	*             机组ID：        FIELD_SERVICE_PLANTID
	*             起始索引        FIELD_SERVICE_ON_SEND_STARTID
	*             启停车标志位    FIELD_SERVICE_SUD_STATUS
	*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速  FIELD_SERVICE_REV
	*             指定机组的转速2 FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
    *             指定机组的转速4  FIELD_SERVICE_REV4
    *             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据        FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率        FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数        FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*             自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间       FIELD_SERVICE_TIME
	*            对应时间的微秒   FIELD_SERVICE_MICROSECOND
	*             启停车类型      FIELD_SERVICE_SUD_TYPE
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_RECSUDDATA        24010


///快变报警相关的接口

/**申请阈值ID，如果数组长度小于1表示失败
  *输入参数字段码
    *           公司ID          FIELD_SYSTEM_COMPANY
	*			分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           通道个数        FIELD_SERVICE_CHANN_NUM
	*返回值的字段码：
	*           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
	*           数组长度（通道个数） FIELD_SERVICE_CHANN_NUM
  */
#define     SERVICE_SERVICE_ON_REQTHESHOLDID       24011

/**申请指定公司、分厂、设备的快变报警事件ID
  *输入参数字段码：
  *             公司ID         FIELD_SYSTEM_COMPANY
  *			    分厂名         FIELD_SYSTEM_FACTORY
  *             机组ID：       FIELD_SERVICE_PLANTID
	*返回值的字段码：
  *             快变报警事件ID:   FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012

/**更新指定公司、分厂、设备的振动测点快变报警状态
  *输入参数字段码：
  *             公司ID            FIELD_SYSTEM_COMPANY
  *			    分厂名            FIELD_SYSTEM_FACTORY
  *             机组ID：          FIELD_SERVICE_PLANTID
  *             通道索引          FIELD_SERVICE_ON_SEND_STARTID
  *             振动的报警状态    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
  *             振动通道个数      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *返回值的字段码：
  *             成功标识            FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS     24013


/**@brief 发送阈值的信息、学习参数、学习数据，6种特征值一起发。返回-1表示失败，返回大于0表示成功
  *输入参数字段码
    *           公司ID          FIELD_SYSTEM_COMPANY
	*			分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           通道类型        FIELD_SERVICE_CHANNTYPE
	*           通道索引        FIELD_SERVICE_ON_SEND_STARTID
	*           阈值ID          FIELD_SERVICE_THRESHOLD_ID
	*           学习数据的BUFF  FIELD_SERVICE_ON_SEND_VIBWAVE    //数据依次为不同特征值数组链接起来.数组长度=FIELD_SERVICE_LEN
	*           学习数据的长度  FIELD_SERVICE_LEN
	*           阈值的BUFF      FIELD_SERVICE_THRESHOLD_BUFF     //结构体长度为FIELD_SERVICE_CHANN_NUM
	*           学习参数的BUFF  FIELD_SERVICE_LEARNPARAM_BUFF    //结构体长度为FIELD_SERVICE_CHANN_NUM
	*           报警类型        FIELD_SERVICE_ALARM_TYPE
	*           学习数据的开始时间  FIELD_SERVICE_OFF_TIMESTART
	*           学习数据的结束时间  FIELD_SERVICE_OFF_TIMEEND
	*           触发门限学习的类型  FIELD_SYSTEM_TYPE
  *返回值的字段码
  *           成功标识            FIELD_SYSTEM_STATUS
  */
#define    SERVICE_SERVICE_OFF_SAVETHINFO         24014


/**通知指定快变报警事件的报警过程结束。如果成功返回1，失败返回-1
  *输入参数字段码：
  *             公司ID          FIELD_SYSTEM_COMPANY
  *			    分厂名          FIELD_SYSTEM_FACTORY
  *             机组ID：        FIELD_SERVICE_PLANTID
  *             通道类型        FIELD_SERVICE_CHANNTYPE
  *           通道索引        FIELD_SERVICE_ON_SEND_STARTID
  *             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
  *             结束时间        FIELD_SERVICE_OFF_TIMEEND
  *             新阈值ID        FIELD_SERVICE_THRESHOLD_ID
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP        24015

/**发送快变报警数据到中间件保存
  *输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             快变报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速   FIELD_SERVICE_REV
	*             指定机组的转速2  FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
	*             指定机组的转速4  FIELD_SERVICE_REV4
	*             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*            自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*            间隙电压          FIELD_SERVICE_TREND_GAP
	*            成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_RECFASTALARMDATA        24016

/**保存报警后新学习的阈值ID，如果数组长度小于1表示失败
  *输入参数字段码
    *           公司ID          FIELD_SYSTEM_COMPANY
	*			分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           通道个数        FIELD_SERVICE_CHANN_NUM
	*           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
	*返回值的字段码：
  *           成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID   24017


/**添加一条完整的报警日志，失败返回值小于0，成功为0
  *输入参数字段码
  *           公司ID          FIELD_SYSTEM_COMPANY
	*		  分厂名          FIELD_SYSTEM_FACTORY
	*         机组ID：        FIELD_SERVICE_PLANTID
	*         通道ID          FIELD_SERVICE_CHANN1
	*         日志ID          FIELD_SYSTEM_ID
  *           快变报警事件ID  FIELD_SYSTEM_EVENT_ID
  *           通道类型        FIELD_SERVICE_CHANNTYPE
  *           报警类型        FIELD_SERVICE_ALARM_TYPE
  *           开始时间        FIELD_SERVICE_OFF_TIMESTART
  *           结束时间        FIELD_SERVICE_OFF_TIMEEND
	*           阈值ID          FIELD_SERVICE_THRESHOLD_ID
	*           新阈值ID        FIELD_SERVICE_ON_SEND_STARTID
	*返回值的字段码：
  *           成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_ADDALARMLOG                24018

/**新增报警日志，报警开始添加。如果成功返回1，失败返回-1
  *输入参数字段码：
  *             公司ID          FIELD_SYSTEM_COMPANY
  *			    分厂名          FIELD_SYSTEM_FACTORY
  *             机组ID：        FIELD_SERVICE_PLANTID
  *             通道类型        FIELD_SERVICE_CHANNTYPE
	*           通道索引        FIELD_SERVICE_ON_SEND_STARTID
  *             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
  *             起始时间        FIELD_SERVICE_OFF_TIMESTART
  *             报警类型        FIELD_SERVICE_ALARM_TYPE
  *             阈值ID          FIELD_SERVICE_THRESHOLD_ID
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG        24019

/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE         24021

/**发送指定机组的动态测点断网后保存的所有特征值波形数据到中间件模块
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			 分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                 FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数               FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE         24022

/**发送断网后保存的ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
*输入参数字段码：
*            公司ID            FIELD_SYSTEM_COMPANY
*            分厂名            FIELD_SYSTEM_FACTORY
*            机组ID            FIELD_SERVICE_PLANTID
*            起始索引          FIELD_SERVICE_ON_SEND_STARTID
*            测点类型          FIELD_SERVICE_CHANNTYPE
*            过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*            过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            时间值            FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*            测点个数          FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEPROC            24023


/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE         24024


/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			       分厂名   FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID
*返回值的字段码：无
*/
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL      24027

/** 接收数硬件状态
  *输入参数字段码：(n)
  *            数采卡索引             FIELD_SERVICE_DAQ_CARD_INDEX
  *            数采卡数量             FIELD_SERVICE_DAQ_CARD_NUM
  *            数采卡逻辑名称         FIELD_SERVICE_DAQ_CARD_NAME
  *            使用AI通道(CSV)        FIELD_SERVICE_DAQ_USED_AI_CHANNEL
  *            使用AI通道数           FIELD_SERVICE_AI_CHANN_NUM
  *            使用CI通道(CSV)        FIELD_SERVICE_DAQ_USED_CI_CHANNEL
  *            使用CI通道数           FIELD_SERVICE_CI_CHANN_NUM
  *            工作状况(CSV)          FIELD_SERVICE_DAQ_CARD_STATUS
  *返回值的字段码：
  *            接收状态      FIELD_SYSTEM_STATUS   (0正常， 其它 异常)

  */
#define     SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS              24030

/** 接收数采软件状态(n)
  *输入参数字段码：
  *            版本号                                  FIELD_SERVICE_DAQ_SW_VERSION
  *            采集模式                                FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            启动时间                                FIELD_SERVICE_TIME
  *            接收间隙电压状况                        FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            最近一次断网数据发送开始时间            FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            最近一次断网数据发送结杏时间            FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  *返回值的字段码：
  *            接收状态       FIELD_SYSTEM_STATUS    (0正常， 其它 异常)

  */
#define     SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS              24031


///对应4系列中间件的同步数据相关接口，发送数据到监测中心的中间件

/**根据指定设备得到指定索引和长度的振动波形数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            振动测点特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数            FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数            FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            自定义参数数据BUFF      FIELD_SERVICE_LEARNPARAM_BUFF
*            自定义参数数据BUFFSIZE  FIELD_SERVICE_CHANN_BUFFSIZE
*            数据的时间              FIELD_SERVICE_TIME
*            设备运行状态            FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA     24050

/**根据指定设备指定索引和长度的动态数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*		     分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            自定义参数数据BUFF      FIELD_SERVICE_LEARNPARAM_BUFF
*            自定义参数数据BUFFSIZE  FIELD_SERVICE_CHANN_BUFFSIZE
*/
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA     24054

/**发送动态报警数据到中间件保存
*输入参数字段码：
*             公司ID              FIELD_SYSTEM_COMPANY
*			  分厂名              FIELD_SYSTEM_FACTORY
*             机组ID：            FIELD_SERVICE_PLANTID
*             起始索引            FIELD_SERVICE_ON_SEND_STARTID
*             报警类型数组        FIELD_SERVICE_ALARM_TYPE
*             动态报警事件ID      FIELD_SYSTEM_EVENT_ID
*             特征值数据		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据            FIELD_SERVICE_ON_SEND_VIBWAVE
*             总的波形点数        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID
*             对应的时间          FIELD_SERVICE_TIME
*             对应时间的微秒      FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*             压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
*             间隙电压          FIELD_SERVICE_TREND_GAP
*/
#define     SERVICE_SERVICE_OFF_RECDYNALARMDATA          24055


/**发送ALLPROC过程量到中间件模块，并得到指定类型的数据
  *输入参数字段码：
  *                公司ID   FIELD_SYSTEM_COMPANY
  *			       分厂名   FIELD_SYSTEM_FACTORY
  *                机组ID            FIELD_SERVICE_PLANTID
  *                现有数据的时间              FIELD_SERVICE_TIME
  *返回值的字段码：
  *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
   *               数据的时间              FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA       24051

/**发送指定机组的报警状态，为报警状态的结构体
  *输入参数字段码：
  *                公司ID   FIELD_SYSTEM_COMPANY
  *			       分厂名   FIELD_SYSTEM_FACTORY
  *                机组ID   FIELD_SERVICE_PLANTID
  *返回值的字段码：
 *              轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
	*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
	*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
	*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
	*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
	*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
  */
#define     SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE       24052

/**添加一条完整的启停车日志。如果成功返回启停车事件ID，失败返回-1
  *输入参数字段码：
  *             公司ID        FIELD_SYSTEM_COMPANY
  *			    分厂名        FIELD_SYSTEM_FACTORY
  *             机组ID：      FIELD_SERVICE_PLANTID
  *             启停车类型    FIELD_SERVICE_SUD_TYPE
  *             开始时间      FIELD_SERVICE_OFF_TIMESTART
  *             结束时间      FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *             启停车事件ID: FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_OFF_ADDWHOLESUDLOG             24053

/** 发送过程量报警数据到中间件模块
*输入参数字段码：
*                公司ID                 FIELD_SYSTEM_COMPANY
*                分厂名                 FIELD_SYSTEM_FACTORY
*                机组ID                 FIELD_SERVICE_PLANTID
*                报警类型               FIELD_SERVICE_ALARM_TYPE
*                报警事件ID             FIELD_SYSTEM_EVENT_ID
*                对应的时间             FIELD_SERVICE_TIME
*                过程量测点号           FIELD_SYSTEM_CHANN
*                过程量测点类型         FIELD_SYSTEM_CHANN_TYPE
*                过程量特征值           FIELD_SERVICE_CHANN1VALUE
*                转速                   FIELD_SERVICE_REV
*
*返回值的字段码：
*                成功标识               FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_OFF_RECPROCALARMDATA           24056

/** 请求离线巡检数据同步，返回离线测点索引和个数，及同步时间
  *输入参数字段码：
  *                公司          FIELD_SYSTEM_COMPANY
  *			       分厂名        FIELD_SYSTEM_FACTORY
  *                机组ID：      FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *                上次同步时间      FIELD_SERVICE_TIME
  *                离线测点起始索引  FIELD_SERVICE_ON_SEND_STARTID
  *                离线测点个数      FIELD_SERVICE_CHANN_NUM
  *                同步响应          FIELD_SYSTEM_STATUS
                                    （FIELD_SYSTEM_SUCCESS 可以同步/ FIELD_SYSTEM_FAILURE 异常，无法同步）
  *
  */
#define     SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC            24060

/**接收离线巡检数据
  *输入参数字段码：
  *                公司名        FIELD_SYSTEM_COMPANY
  *                分厂名        FIELD_SYSTEM_FACTORY
  *                设备号        FIELD_SERVICE_PLANTID  (如1T1, 1T2)
  *                采集时间      FIELD_SERVICE_TIME
  *                测点名        FIELD_SYSTEM_CHANN
  *                转速          FIELD_SERVICE_REV
  *                特征值        FIELD_SERVICE_OFF_DATA( float)
  *返回值的字段码：
  *                同步状态      FIELD_SYSTEM_STATUS
                                    （FIELD_SYSTEM_SUCCESS/ FIELD_SYSTEM_FAILURE）
  *
  */
#define     SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA            24061




/**获得指定机组的振动通道的组态信息
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
			   振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *返回值的字段码：
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  */
#define     SERVICE_SERVICE_ON_GET_VIBINFO        24066

/**获得指定机组的动态通道的组态信息
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
			   振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *返回值的字段码：
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP

  */
#define     SERVICE_SERVICE_ON_GET_DYNINFO        24067


/**通知中间件数据库满
  *输入参数字段码：
  */
#define     SERVICE_SERVICE_ON_SET_DISK_FULL       24068


/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *				#define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */
#define     SERVICE_SERVICE_ON_ALARM_CONFIRM      24069

/**发送指定机组的振动测点实时所有特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
  *            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE        24100

/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION　
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE        24101


/**发送转速到中间件模块
  *输入参数字段码：
  *                公司ID      FIELD_SYSTEM_COMPANY
  *	    		       分厂名      FIELD_SYSTEM_FACTORY
  *                机组ID      FIELD_SERVICE_PLANTID
  *                机组的转速  FIELD_SERVICE_REV
  *                机组的转速2 FIELD_SERVICE_REV2
  *            机组的转速3     FIELD_SERVICE_REV3
  *            机组的转速4     FIELD_SERVICE_REV4
  *            机组的转速5     FIELD_SERVICE_REV5
  *返回值的字段码：无
  */
#define     SERVICE_SERVICE_ON_SUD_RECEIVEREV             24102

/**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
  *输入参数字段码：
  *                公司ID   FIELD_SYSTEM_COMPANY
  *			           分厂名   FIELD_SYSTEM_FACTORY
  *                机组ID      FIELD_SERVICE_PLANTID
  *                起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *                测点类型          FIELD_SERVICE_CHANNTYPE
  *                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
  *                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
  *                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
  *            报警状态                FIELD_SERVICE_ALARM_STATUS
  *            报警事件ID              FIELD_SYSTEM_EVENT_ID
  */
#define     SERVICE_SERVICE_ON_SUD_RECEIVEPROC            24103

/**发送指定机组振动快变数据到中间件，并得到报警状态
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			   分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
*            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*            快变数据报警状态       FIELD_SERVICE_ALARM_STATUS
*返回值的字段码：：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID
*            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
*            对应的间隙电压          FIELD_SERVICE_TREND_GAP
*            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
*            学习参数结构体的长度    FIELD_SERVICE_LEN
*            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
*/
#define     SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE        24104

/**发送指定机组的动态测点实时快变数据到中间件模块，并得到报警状态和报警事件ID
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			 分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
*            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION　
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*            快变数据报警状态       FIELD_SERVICE_ALARM_STATUS
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID
*            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
*            对应的间隙电压          FIELD_SERVICE_TREND_GAP
*            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
*            学习参数结构体的长度    FIELD_SERVICE_LEN
*            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
*/
#define     SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE        24105

/**发送ALLPROC过程量或轴位移快变数据到中间件模块，并得到报警状态和报警事件ID
  *输入参数字段码：
  *           公司ID   FIELD_SYSTEM_COMPANY
  *			  分厂名   FIELD_SYSTEM_FACTORY
  *           机组ID      FIELD_SERVICE_PLANTID
  *           起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *           测点类型          FIELD_SERVICE_CHANNTYPE
  *           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *            数据采集时间           FIELD_SERVICE_TIME
  *            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
  *            快变数据报警状态       FIELD_SERVICE_ALARM_STATUS
  *返回值的字段码：
  *如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
  *           个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
  *           数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
  */
#define     SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE        24106

/**发送ALLPROC过程量或轴位移快变报警数据到中间件模块
*输入参数字段码：
*           公司ID            FIELD_SYSTEM_COMPANY
*			分厂名            FIELD_SYSTEM_FACTORY
*           机组ID            FIELD_SERVICE_PLANTID
*           起始索引          FIELD_SERVICE_ON_SEND_STARTID
*           测点类型          FIELD_SERVICE_CHANNTYPE
*           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
*           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*           报警事件ID        FIELD_SYSTEM_EVENT_ID
*           对应的时间        FIELD_SERVICE_TIME
*           转速              FIELD_SERVICE_REV
#           报警类型          FIELD_SERVICE_ALARM_TYPE
*返回值的字段码：
*           成功标识          FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_ON_RECPROCFASTALARMDATA           24107

/**发送振动快变报警数据到中间件保存
  *输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             快变报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速   FIELD_SERVICE_REV
	*             指定机组的转速2  FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
	*             指定机组的转速4  FIELD_SERVICE_REV4
	*             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*            自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*            间隙电压          FIELD_SERVICE_TREND_GAP
	*            成功标识      FIELD_SYSTEM_STATUS
  */
#define     SERVICE_SERVICE_ON_RECVIBFASTALARMDATA        24108

/**发送动态快变报警数据到中间件保存
*输入参数字段码：
*             公司ID              FIELD_SYSTEM_COMPANY
*			  分厂名              FIELD_SYSTEM_FACTORY
*             机组ID：            FIELD_SERVICE_PLANTID
*             起始索引            FIELD_SERVICE_ON_SEND_STARTID
*             报警类型数组        FIELD_SERVICE_ALARM_TYPE
*             动态报警事件ID      FIELD_SYSTEM_EVENT_ID
*             特征值数据		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据            FIELD_SERVICE_ON_SEND_VIBWAVE
*             总的波形点数        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID
*             对应的时间          FIELD_SERVICE_TIME
*             对应时间的微秒      FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*             压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
*             间隙电压          FIELD_SERVICE_TREND_GAP
*/
#define     SERVICE_SERVICE_ON_RECDYNFASTALARMDATA          24109

/**启停车结束后，发送指定机组的启停车振动测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_OFF_SUD_VIB_DATA                         24110

/**启停车结束后，发送指定机组的启停车动态测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             动态测点个数         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_OFF_SUD_DYN_DATA                         24111

/**启停车结束后，发送指定机组的启停车过程量测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID               FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             过程量个数           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             报警事件ID           FIELD_SYSTEM_EVENT_ID
	*             报警类型             FIELD_SERVICE_SUD_TYPE
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             过程量特征值         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_OFF_SUD_PROC_DATA                        24113


/**启停车结束后， 同步指定公司、分厂、设备的启停车事件ID
	*输入参数字段码：
	*             公司ID         FIELD_SYSTEM_COMPANY
	*             分厂名         FIELD_SYSTEM_FACTORY
	*             机组ID        FIELD_SERVICE_PLANTID
	*             启停车事件ID   FIELD_SYSTEM_EVENT_ID
	*             启停车事件名称  FIELD_SYSTEM_EVENT_NAME
	*             开始时间       FIELD_SERVICE_OFF_TIMESTART
	*返回值的字段码：
	*             启停车事件ID   FIELD_SYSTEM_EVENT_ID
*/
#define     SERVICE_SERVICE_OFF_SUD_ADD_LOG                          24114

/**发送指定机组振动无线采集数据到中间件
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			   分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*            开停车状态             FIELD_SYSTEM_STATUS
*            是否需要判断报警       FIELD_SERVICE_JUDGE_ALARM
*            快变测点个数           FIELD_SERVICE_FC_CHANN_BUFFSIZE
*            快变测点Buff           FIELD_SERVICE_CHANN_BUFF
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*返回值的字段码：：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID
*            报警事件ID              FIELD_SYSTEM_EVENT_ID
*            报警状态                FIELD_SERVICE_ALARM_STATUS
*            快变测点个数            FIELD_SERVICE_FC_CHANN_BUFFSIZE
*            快变测点报警类型Buff    FIELD_SERVICE_CHANN_BUFF
*/
#define     SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE        24115

/*!
*输入参数字段码
*
*返回值的字段码：
*          数据采集时间：       FIELD_SERVICE_TIME
*/
#define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116

/*!
*输入参数字段码
*
*返回值的字段码：
*          通知状态：  FIELD_SYSTEM_STATUS
*/
#define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117


/*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          测点类型    FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *		     是否修改成功：  FIELD_SYSTEM_STATUS
  */
#define    SERVICE_SERVICE_ON_SAVESPECTIMEDATA    21080

/**启停后处理数据，发送指定机组的启停车振动测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_ON_SUD_VIB_DATA                         24210

/**启停后处理数据，发送指定机组的启停车动态测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             动态测点个数         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_ON_SUD_DYN_DATA                         24211

/**启停后处理数据，发送指定机组的启停车过程量测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID               FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             过程量个数           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             报警事件ID           FIELD_SYSTEM_EVENT_ID
	*             报警类型             FIELD_SERVICE_SUD_TYPE
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             过程量特征值         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	*/
#define     SERVICE_SERVICE_ON_SUD_PROC_DATA                        24212
