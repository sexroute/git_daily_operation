#pragma once
#include "stdafx.h"
#include "vibdata.h"
#include "additions/DebugHelper.h"



class EigenValueComputer
{
public:


	static BOOL ComputeDynEigenValue( S_DynCharValue & arefVibCharValue,
										S_ChannInfo & arefChannInfo,
										float afRev,
										DATATYPE_WAVE  *   apNivfWave_, 
										int		    anNivfWaveLen_,
										MACHINETYPE aEnumPlantType,
										CHANNTYPE aEnumChannType);
	

	static float ComputeFirstEigenValue     ( 
										S_ChannInfo & arefChannInfo,
										DATATYPE_WAVE  *  apNivfWave_, 
										int		 anNivfWaveLen_,
										MACHINETYPE aEnumPlantType,									 
										CHANNTYPE aEnumChannType);



	static BOOL ComputeVibEigenValue( S_VibCharValue & arefVibCharValue,
										S_ChannInfo & arefChannInfo,
										float afRev,
										DATATYPE_WAVE  *  apNivfWave_, 
										int		 anNivfWaveLen_,
										MACHINETYPE aEnumPlantType,									 
										CHANNTYPE aEnumChannType);


	static float ComputeAxisEigenValue  (     
											S_ChannInfo & arefChannInfo,
											DATATYPE_WAVE  *  apNivfWave_, 
											float afRev,
											float afFreq,
											int		 anNivfWaveLen_,
											MACHINETYPE aEnumPlantType,									 
											CHANNTYPE aEnumChannType);


};