#pragma once
#include "math.h"
/*
	�м����ѧϰ������㷨ģ��
	made by DSE.
*/
#ifdef HZ_DIAGNOSE_MW_DLL_INTERNALS
#undef HZ_DIAGNOSE_MW_DLL_INTERNALS
#define HZ_DIAGNOSE_MW_DLL_INTERNALS _declspec(dllexport)					//������ʶ��
#else
#undef HZ_DIAGNOSE_MW_DLL_INTERNALS
#define HZ_DIAGNOSE_MW_DLL_INTERNALS _declspec(dllimport)					//�����ʶ��
#pragma  comment(lib,"Diagnose_MW.lib")
#endif

////////////////////////////////////////////////////////////////////////////////////////
struct S_AxisCenterPosition_CalcInput
{
	double   d_Radius ;				  /*��뾶,��λ:mm*/
	double   d_GapVolt_V;			  /*��ֱ��ʼ��϶��ѹ����λ��mm.*/
	double   d_GapVolt_H;			  /*ˮƽ��ʼ��϶��ѹ����λ��mm.*/
	double   d_DC_V;					  /*��ֱ��ǰֱ��������λ��mm.*/        
	double   d_DC_H;					  /*ˮƽ��ǰֱ��������λ��mm.*/
	int      i_circumgyrate_aspect;    /*��ת���򣬵�λ����.0-��ʱ�� 1-˳ʱ��*/
	int      i_fixing_mode;            /*��װ��ʽ����λ����.2-(45��45)  0-(0-90 ˮƽ����) 1-(0-90 ˮƽ����)*/
};
struct S_AxisCenterPosition
{
	double   AxisCenterPosition_X ;              /*ˮƽ���� ��λ��um*/
	double   AxisCenterPosition_Y;               /*��ֱ���� ��λ��um.*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//����λ�û������ޣ�����ȥ������λ�ÿ��
struct S_AxisCenterPositionThreshold__CalcInput
{
	double *  AxisCenterPosition_X ;              /*ˮƽ�������� ��λ��um*/
	double *  AxisCenterPosition_Y;               /*��ֱ�������� ��λ��um.*/
	int i_length;                                 /*���鳤��*/
	float f_hope;                                 /*����*/
	float f_zoomCorfficient;                      /*�Ŵ�ϵ��*/
	
};
struct S_AxisCenterPosition_threshold
{
	double   d_center_X ;                          /*Բ�� X ��λ��um*/
	double   d_center_Y ;                          /*Բ�� Y ��λ��um*/
	double   d_Radius;								/*�뾶 R ��λ��um.*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//�������
struct S_TrendEnent_CalcInput
{
	double *  pTrendArrChar ;              /*ʱ������ ��λ��um*/
	int		  i_length;								/*ʱ�����г��� ��λ��*/
	int       i_trendType;							/*�������� 1 Сʱ 2 �� 3 �� 4 ����*/
	double    d_threshold;						   /*б������ ��λ��*/
};
struct S_TrendEnent_result
{
	double *  d_k ;								/*ʱ������б�� ��λ��*/
	int		  i_trendEventResult;				/*�����¼���Ͻ�� 0 = �����Ʒ�չ 1 ���ƻ������� 2 ���ƽϿ����� 3 ���ƻ������� 4 ���ƽϿ콵��   */
	int      errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//��ֵ�������޼���
struct S_charThreshold__CalcInput
{
	double *  pChar ;							/* ����ֵ����  ��λ��*/
	int i_length;                                 /*���鳤��*/
	float f_hope;                                 /*����*/
	float f_zoomCorfficient;                      /*�Ŵ�ϵ��*/

};
struct S_charThreshold
{
	double   d_charThreshold_up ;                          /*������ ��λ��*/
	double   d_charThreshold_down ;                          /*������ ��λ��*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//��λ�������޼���  ��֤������λ��0 -- 360����
struct S_phaseThreshold__CalcInput
{
	double *  pPhase ;							/* ��λ����  ��λ����*/
	int i_length;                                 /*���鳤��*/
	float f_hope;                                 /*����*/
	float f_zoomCorfficient;                      /*�Ŵ�ϵ��*/

};
//�������޶��� 1����������[0��180]֮��ʱ ��������[-180 �� ������]
//             2����������[180��360]֮��ʱ ��������[0 �� ������]
struct S_phaseThreshold
{
	double   d_phaseThreshold_up ;                          /*������ ��λ��*/
	double   d_phaseThreshold_down ;                          /*������ ��λ��*/
	bool     b_InOneFour;									/*�����Ƿ���1 4 ����*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//��λ���������¼�������ѧϰ,Ҳ������λ�������  ��֤��λ������ 0 -- 360����,������λ��0 -- 360����
struct S_phaseVarThreshold__CalcInput
{
	double *  pPhase ;							  /* ��λ����  ��λ����*/
	int i_length;                                 /*���鳤��*/
	float f_hope;                                 /*����*/
	float f_zoomCorfficient;                      /*�Ŵ�ϵ��*/

};
struct S_phaseVarResult
{
	double   d_Var;                          /*������ ��λ��*/
	bool     b_InOneFour;					/*�����Ƿ���1 4 ����*/
	int     errorCode;
};


class HZ_DIAGNOSE_MW_DLL_INTERNALS CAnalizeDiagnose_MW
{
	
public:
	CAnalizeDiagnose_MW(void);
	~CAnalizeDiagnose_MW(void);

	//���Ĺ켣���� 2006 9 10
	//����ֵ bool ����������򳬳���Χ false
	bool getAxisCenterPosition(S_AxisCenterPosition_CalcInput arg_CalcInput,S_AxisCenterPosition & outArg_AxisCenterPosition);

	//����λ�û������� 
	//����ֵ bool ����������false
	bool calcAxisCenterPosition_threshold(S_AxisCenterPositionThreshold__CalcInput inArg_AxisCenterPositionthreshold,S_AxisCenterPosition_threshold & outArg_AxisCenterPosition_threshold);

	//��ֵ�������޼��� 
	//����ֵ bool ����������false
	bool calcCharSlowChange_threshold(S_charThreshold__CalcInput inArg,S_charThreshold & outArg);

	//��λ�������޼��� 
	//����ֵ bool ����������false
	bool calcPhaseSlowChange_threshold(S_phaseThreshold__CalcInput inArg,S_phaseThreshold & outArg);

	//�����¼����
	//����ֵ bool ����������򳬳���Χ false
	bool calcTrendEnent(S_TrendEnent_CalcInput inArg_TrendEnent,S_TrendEnent_result & outArg_S_TrendEnent_result);

	//��λ���������¼���������� ��׼��
	//����ֵ bool ����������򳬳���Χ false
	bool calcPhaseVarTrendEnent(S_phaseVarThreshold__CalcInput inArg,S_phaseVarResult & outArg);




	//math
	double GetDistance(const double x1,const double y1,const double x2,const double y2);

};
