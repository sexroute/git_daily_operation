/*!\file RodComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_RODCOMPUTER_H)
#define _RODCOMPUTER_H

#include "DynPressComputer.h"

namespace Computation
{
	class CRodComputer : public CDynPressComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_PISROD_LEN,		///< 活塞杆长度(m)
			IDX_IN_HUB_RDS,		    ///< 轮毂半径(m)
			IDX_IN_SENS_POS,		///< 传感器位置(m)
			IDX_IN_PIS_THICK,		///< 活塞厚度(m)
			IDX_IN_PIS_CW_GAP,	    ///< 活塞-气缸壁间隙(m)
			IDX_IN_GAP,      	    ///< 间隙电压
			IDX_IN_ROD_LEN,         ///< 连杆长度(m)
			IDX_IN_UNITRATIO,       ///< 单位影响系数
			IDX_IN_END
		};
	};
}

#endif  //_RODCOMPUTER_H
