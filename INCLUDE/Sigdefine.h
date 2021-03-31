#pragma  once

#ifdef SIGMATH_EXPORTS
#define CLASS_DLL_DSE_SigMath  __declspec(dllexport)
#else
#define CLASS_DLL_DSE_SigMath  __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"sigmathd.lib")
#else
#pragma comment(lib,"sigmath.lib")
#endif  
#endif


#include <vector>
#include <map>
#include <complex>
using namespace std;


#define PI 3.1415926

//-----------------------------------------------------------------------//
/** \struct  S_BearingPar
* \brief  ������в����ṹ�塣���������й��ϣ�ͨ����Ƶ��ʱ����Ҫ�Ĳ�����
*/
struct S_BearingPar
{
	float   fRollingDiam;          /*! ������й����壨���飩��ֱ������λ��mm.*/
	float   fContackAngle;         /*! ������е�ѹ���ǣ��ֳƽӴ��ǣ�����λ��DEG.*/
	int     nNumofRolling;         /*! ������й�����ĸ���.*/
	float   fPitchDiam;            /*! ������еĽھ�����λ��mm.*/
};

//-----------------------------------------------------------------------//
/** \struct  S_BearingDefectFreq
* \brief  ������й��ϣ�ͨ����Ƶ�ʽṹ�塣����������еĸ��ֹ���Ƶ�ʡ�
*/
struct S_BearingDefectFreq
{
	float   fInnerFreq;             /*! ���������Ȧͨ��Ƶ�ʣ���λ��Hz.*/        
	float   fRollingFreq;           /*! ������й����壨���飩��ͨ��Ƶ�ʣ���λ��Hz.*/
	float   fOuterFreq;             /*! ���������Ȧͨ��Ƶ�ʣ���λ��Hz.*/
	float   fCageFreq;              /*! ������б��ּܵ�ͨ��Ƶ�ʣ���λ��Hz.*/
};


///�˲�������ʽ��ϵ��
class CFilterPolyCoef
{
public:
	CFilterPolyCoef()
	{
		_iOrder = 0;
		for (int i= 0; i< 5; ++i)
		{
			_fg[i] = .0;
			_fh[i] = .0;
		}
	}
public:
	///��ĸϵ��
	float _fh[5];
	///����ϵ��
	float _fg[5];
	///����ʽ�Ľ���
	int _iOrder;
};

///�źŲ����ṹ��
struct SSigParam
{
	SSigParam()
	{
		_fFreq = 0;
		_fAmp  = 0;
		_fPhase= 0;
	}
	///�ź�Ƶ��
	double _fFreq; 
	///�źŷ�ֵ
	double _fAmp;
	///�ź���λ
	double _fPhase;
};

#define DSE_DataIsMiddle(a,low,high)  ( ((a)>(low)) && ((a)<(high)) )
#define DSE_DeletePoint(pt)          { if(0 != (pt)) { delete [] (pt); (pt)= 0;} } 

///���ں���ִ�У��������ͷ������ε����
#define BH_FunExec_ErrorReturnInt(rs) { if( (rs) < 0 ) return (rs); }


/**����ֵ��˵��
  * 0 : �ɹ�
  *-1 ����������
  *-2 ��
  */
enum E_DseSigRet
{
	DSESIG_SUCC = 0,
	DSESIG_ERR_PARAM = -1,         ///�����������
	DSESIG_ERR_VALUE_BEYOND = -2,  ///ֵԽ��
	DSESIG_ERR_SUBFUN      = -3,   ///�Ӻ����ڲ��������
	DSESIG_ERR_OUTPARAM = -4,      ///��Ҫ����Ĳ�������
	DSESIG_ERR_FILTER   = -5,      ///�˲�����
	DSESIG_ERR_FFT      = -6,      ///FFT����
	DSESIG_ERR_UNKNOWN = -10
};

/** ����FFT֮ǰ��ʱ�����мӴ������崰��ö�ٱ���.*/
enum E_WINOW_TYPE
{
	WINDOW_NO,              /*!< not window.*/
	WINDOW_HANNING,         /*!< hanning.������*/
	WINDOW_HAMMING,         /*!< hamming.������*/
	WINDOW_BLACKMANHARRIS,  /*!< Blackman-Harris����������-����˹*/
	WINDOW_EXTRACTBLACKMAN, /*!< Exact Blackman*/
	WINDOW_BLACKMAN,        /*!< blackman.*/
	WINDOW_FLATTOP,         /*!< Flat Top*/
	WINDOW_BLACKMANHARRIS4, /*!< blackmanharris 4-term.*/
	WINDOW_BLACKMANHARRIS7, /*!< blackmanharris 7-term.*/
    WINDOW_LowSidelobe      /*!< Low Sidelobe*/
};

/**���Ĺ켣��״ö�ٶ���,Class={1 2 3 4 5}={Բ/��Բ,�㽶����8����8������}*/
enum E_OrbitShape
{
   OrbitShape_Unknown = 0, ///δ֪
   OrbitShape_Circle  = 1,  ///Բ
   OrbitShape_Ellipse = 1,  ///��Բ
   OrbitShape_Banana  = 2,  ///�㽶
   OrbitShape_In8     = 3,  ///��8
   OrbitShape_Out8    = 4,  ///��8
   OrbitShape_Disorder= 5  ///����   
};
///��������ö�ٶ���
enum E_FeatureValueType
{
    E_FeatureValueType_RMS = 0,
	E_FeatureValueType_Effective  = 1,      ///��ֵ(��Ч��ֵ)
    E_FeatureValueType_PeakPeak_Effective = 2,  ///���ֵ(��Ч���ֵ)
	E_FeatureValueType_Kur = 3,
	E_FeatureValueType_Skew = 4,
	E_FeatureValueType_Peak_Statistic  = 5,     ///ͳ���������ֵ
	E_FeatureValueType_PeakPeak_Statistic = 6,  ///ͳ����������ֵ
	E_FeatureValueType_AVERAGE_P       = 7,     ///ƽ����ֵ
	E_FeatureValueType_Peak_Direct     = 8,  ///ֱ�Ӽ������ֵ
	E_FeatureValueType_PeakPeak_Direct = 9   ///ֱ�Ӽ�������ֵ
};

/** Ƶ�׷������͵�ö��. */
enum E_SpectrumType
{
	E_SpectrumType_PEAK = 0,        /*��ֵ��*/
	E_SpectrumType_POWER,           /*����(��ֵ)��*/
    E_SpectrumType_Peak_Peak,       /*��-��ֵ��*/
	E_SpectrumType_RMS,             /*��Чֵ��*/
	E_SpectrumType_POWER_RMS,       /*���ʣ���Чֵ����*/
};

///ת�������˾��򴫸����İ�װ��ʽ
enum E_AxisSensorMounting
{
	E_AxisSensorMounting_LeftTop = 0,       ///����
    E_AxisSensorMounting_RightTop= 1,       ///����
	E_AxisSensorMounting_TopTop  = 2,       ///����
	E_AxisSensorMounting_BottomBottom = 3,  ///����
    E_AxisSensorMounting_LeftBottom = 4,    ///����
    E_AxisSensorMounting_RightBottom= 5,    ///����
	E_AxisSensorMounting_Unknown =10        ///δ֪
};


/**����λ�û������λ�ü�������ṹ�������
  *��ֱˮƽ�Ķ��壨���������жϵ�ǰ�᣺վ���������������˿���ˮƽ����Ϊx���Ҳ�Ϊ���򣬴�ֱ����Ϊy���Ϸ�Ϊ���򣩣�
  *    1������Ǵ�ֱ�����ƽ�е��氲װ�������ʵ��������ɡ�
  *    2����������ϰ�װ�������°�װ��ʽ��������˳ʱ�뷽����ת���������Ĳ��Ϊ��ֱV������һ����㼴Ϊˮƽ
  *
  */
struct S_AxisLocationInputInfo
{
    S_AxisLocationInputInfo()
	{
		fRadius = .0;
		fGapVolt_V = .0;
		fGapVolt_H = .0;
		fDC_H = .0;
		fDC_V = .0;
		fSensitivity = 8.0;
		sensorMounting = E_AxisSensorMounting_TopTop;
	}

	float   fRadius ;			 /*����߻����˰뾶, ��λ:mm*/
	float   fGapVolt_V;			 /*��ֱ��ʼ��϶��ѹ����λ��V.*/
	float   fGapVolt_H;		     /*ˮƽ��ʼ��϶��ѹ����λ��V.*/
	float   fDC_V;			     /*��ֱ��ǰֱ��������λ��V.*/        
	float   fDC_H;			     /*ˮƽ��ǰֱ��������λ��V.*/
	float   fSensitivity;        //�����ȡ���λ��V/mm.
	E_AxisSensorMounting sensorMounting;//��װ��ʽ
};

///�ź�ѹ��������������Ϣ����֧�ֶ��߳�
class CCompressedParamInfo
{
public:
	CCompressedParamInfo()
	{
         _ilevel = 5;
	}

	~CCompressedParamInfo()
	{
      _vCompressedWpCoef.clear();
	  _vCompressedWpCoefIdx.clear();
	  _vBasisTreeIdx.clear();
	}
	void SetLevel(int level = 5){_ilevel = level;}
	const int GetLevel() const {return _ilevel;}
	const vector<double> GetCompressedWpCoef() const{return _vCompressedWpCoef;}
	const vector<unsigned int> GetCompressedWpCoefIdx() const{return _vCompressedWpCoefIdx;}
	void SetCompressedWpCoefIdx(const vector<unsigned int>& vCompressedWpcoefIdx_) {
		_vCompressedWpCoefIdx.clear();
		_vCompressedWpCoefIdx.insert(_vCompressedWpCoefIdx.begin(),vCompressedWpcoefIdx_.begin(),vCompressedWpcoefIdx_.end());
		}
	void SetCompressedWpCoef(const vector<double>& vCompressedWpcoef_) {
		_vCompressedWpCoef.clear();
		_vCompressedWpCoef.insert(_vCompressedWpCoef.begin(),vCompressedWpcoef_.begin(),vCompressedWpcoef_.end());
	}
	const vector<unsigned int> GetCompressedBasisTreeIdx() const{return _vBasisTreeIdx;}
	void SetCompressedBasisTreeIdx(const vector<unsigned int>& vCompressedBasisTreeIdx_) {
		_vBasisTreeIdx.clear();
		_vBasisTreeIdx.insert(_vBasisTreeIdx.begin(),vCompressedBasisTreeIdx_.begin(),vCompressedBasisTreeIdx_.end());
	}
private:
	int _ilevel;
	vector<double> _vCompressedWpCoef;
	vector<unsigned int> _vCompressedWpCoefIdx;
	vector<unsigned int> _vBasisTreeIdx;///������Ϊ1ֵ������
};