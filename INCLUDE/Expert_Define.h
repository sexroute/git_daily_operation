/**@brief ר��ϵͳ�������豸��Ϣö�����ͺ�һЩȫ�ֺ궨��
  *GD_ ����ȫ�ֺ궨��
  *GE_ ����ȫ��ö�ٶ���
  *@author WZQ
  */

#if!defined  AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_
#define AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_


/**ר��ϵͳ����֪ʶ �豸��Ϣ���ö��**/

///�豸���� ע����̬���豸���ͺ�ר��ϵͳ�е��豸����һ�£��ʲ���ת��
enum MACHTYPE
{
	GE_MACHTYPE_COMPR           =0,     //����ѹ����compressor
	GE_MACHTYPE_FAN             =1,     //���fan
	GE_MACHTYPE_TURB            =2,     //���ֻ�turbine
	GE_MACHTYPE_COMPR1          =3,     //����ʽѹ����������+����ѹ����
	GE_MACHTYPE_OTHERS          =4,     //����
	GE_MACHTYPE_PUMP            =5,     //���ı�pump
};

/// ���������
enum CONNTYPE
{
	GE_CONNTYPE_GEAR            =0,    //���������
	GE_CONNTYPE_ELAST           =1,    //���������
	GE_CONNTYPE_OTHERS          =2,    //����
	GE_CONNTYPE_STRAP           =3,    //Ƥ������
	GE_CONNTYPE_SUSPE           =4,    //����ʽ�ṹ
	
};

///ת����ʽ
enum ROTATETYPE
{
	GE_ROTATETYPE_METAL         =0,      //ʵ�Ľ�����
	GE_ROTATETYPE_HOLLOW        =1,      //�ɽ�Һ���п�ת��
	GE_ROTATETYPE_OTHERS        =2,      //����
};

///����ת��
enum WORKREV
{
	GE_WORKREV_BETWEENTWO       =0,      //ת����һ�����ٽ�֮��
	GE_WORKREV_BLOWONE          =1,      //ת�ٵ��ڵ�һ�ٽ�
	GE_WORKREV_UPTWO            =2,      //ת�ٸ��ڵڶ��ٽ�
};

///��������
enum DRIVERTYPE
{
	GE_DRIVERTYPE_INDUSTRYTURB  =0,      //��ҵ���ֻ�
	GE_DRIVERTYPE_MOTORGEAR     =1,      //�綯��������������
	GE_DRIVERTYPE_MOTOR         =2,      //�綯��
	GE_DRIVERTYPE_GASTURB       =3,      //�����ֻ�
	GE_DRIVERTYPE_OTHERS        =4       //����
};

///�������
enum BEARTYPE
{
    GE_BEARTYPE_TILT            =0,      //�����߻������
	GE_BEARTYPE_SLIDE           =1,      //Բ��ʽ�������
	GE_BEARTYPE_ROLL            =2,      //�������
};

///�ܷ���ʽ
enum SEALTYPE
{
	GE_SEALTYPE_LABYRINTH       =0,      //�Թ�ʽ�ܷ�
	GE_SEALTYPE_LOOP            =1,      //����ʽ�ܷ�
	GE_SEALTYPE_OIL             =2,      //�ͷ�
	GE_SEALTYPE_OTHERS          =3,      //����
};

#endif //#define AFX_EXPERT_DEFINE_MIDDLEINFO_WZQ_
