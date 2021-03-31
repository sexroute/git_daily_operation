#ifdef BHXZ_EXPORT_DLL
#define BHXZ_API extern "C" __declspec(dllexport)
#else
#define BHXZ_API extern "C" __declspec(dllimport)
#endif
#ifndef BHXZ_TESTCASE_API
#define  BHXZ_TESTCASE_API

using namespace BHKD;

/// 机器类型
enum PlantType
{
	MACH_ROTATION,			    ///< 旋转机械
	MACH_RECIPROCATION,	    ///< 往复式机械
	MACH_WIND,                      ///< 风电
	MACH_PUMP,                     ///< 机泵
	MACH_OTHER				       ///< 其它机械
};


enum ComputerType
{
	CRotationRadialDisComputer		=1,
	CRotationAxisDisComputer		=2,
	CRotationAccComputer			=3,
	CWindAccComputer				=4,
	CReciprocationAccComputer		=5,
	CReciprocationVecComputer		=6,
	CWindAndPumpVecComputer			=7,
	CDynPressComputer				=8,
	CRodComputer					=9,
	CLoadComputer					=10,
	CImpactComputer					=11,
	COtherComputer					=100,
	CPumpAccComputer				=12,
};



BHXZ_API BOOL ComputeVibEigenValue(   S_VibCharValue & arefVibCharValue,
								      S_ChannInfo & arefChannInfo,
									  float afRev,
									  float *  apNivfWave_, 
									  int		 anNivfWaveLen_,
							          PlantType aEnumPlantType,									 
									  CHANNTYPE aEnumChannType);

BHXZ_API BOOL ComputeDynEigenValue(   S_DynCharValue & arefVibCharValue,
								      S_ChannInfo & arefChannInfo,
									  float afRev,
									  float *  apNivfWave_, 
									  int		 anNivfWaveLen_,
									  PlantType aEnumPlantType,
									  CHANNTYPE aEnumChannType);


BHXZ_API float ComputeFirstEigenValue (     
											S_ChannInfo & arefChannInfo,
											float  *  apNivfWave_, 
											int		 anNivfWaveLen_,
											PlantType aEnumPlantType,									 
											CHANNTYPE aEnumChannType);


BHXZ_API float ComputeAxisEigenValue  (     
											S_ChannInfo & arefChannInfo,
											float  *  apNivfWave_, 
											float afRev,
											float afFreq,
											int		 anNivfWaveLen_,
											PlantType aEnumPlantType,									 
											CHANNTYPE aEnumChannType);


#endif