/**@brief 专家系统程序中设备信息枚举类型和一些全局宏定义
  *GD_ 代表全局宏定义
  *GE_ 代表全局枚举定义
  *@author WZQ
  */

#if!defined  AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_
#define AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_


/**专家系统特征知识 设备信息相关枚举**/

///设备类型 注：组态的设备类型和专家系统中的设备类型一致，故不用转化
enum MACHTYPE
{
	GE_MACHTYPE_COMPR           =0,     //离心压缩机compressor
	GE_MACHTYPE_FAN             =1,     //风机fan
	GE_MACHTYPE_TURB            =2,     //汽轮机turbine
	GE_MACHTYPE_COMPR1          =3,     //轴流式压缩机或轴流+离心压缩机
	GE_MACHTYPE_OTHERS          =4,     //其他
	GE_MACHTYPE_PUMP            =5,     //离心泵pump
};

/// 联轴节类型
enum CONNTYPE
{
	GE_CONNTYPE_GEAR            =0,    //齿型联轴节
	GE_CONNTYPE_ELAST           =1,    //弹性联轴节
	GE_CONNTYPE_OTHERS          =2,    //其它
	GE_CONNTYPE_STRAP           =3,    //皮带传动
	GE_CONNTYPE_SUSPE           =4,    //悬臂式结构
	
};

///转子形式
enum ROTATETYPE
{
	GE_ROTATETYPE_METAL         =0,      //实心金属轴
	GE_ROTATETYPE_HOLLOW        =1,      //可进液体中空转子
	GE_ROTATETYPE_OTHERS        =2,      //其它
};

///工作转速
enum WORKREV
{
	GE_WORKREV_BETWEENTWO       =0,      //转速在一、二临界之间
	GE_WORKREV_BLOWONE          =1,      //转速低于第一临界
	GE_WORKREV_UPTWO            =2,      //转速高于第二临界
};

///驱动类型
enum DRIVERTYPE
{
	GE_DRIVERTYPE_INDUSTRYTURB  =0,      //工业汽轮机
	GE_DRIVERTYPE_MOTORGEAR     =1,      //电动机及附属齿轮箱
	GE_DRIVERTYPE_MOTOR         =2,      //电动机
	GE_DRIVERTYPE_GASTURB       =3,      //烟气轮机
	GE_DRIVERTYPE_OTHERS        =4       //其它
};

///轴承类型
enum BEARTYPE
{
    GE_BEARTYPE_TILT            =0,      //可倾瓦滑动轴承
	GE_BEARTYPE_SLIDE           =1,      //圆瓣式滑动轴承
	GE_BEARTYPE_ROLL            =2,      //滚动轴承
};

///密封形式
enum SEALTYPE
{
	GE_SEALTYPE_LABYRINTH       =0,      //迷宫式密封
	GE_SEALTYPE_LOOP            =1,      //浮环式密封
	GE_SEALTYPE_OIL             =2,      //油封
	GE_SEALTYPE_OTHERS          =3,      //其他
};

#endif //#define AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_
