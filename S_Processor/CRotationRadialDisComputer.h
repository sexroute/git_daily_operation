/*!\file CRotationRadialDisComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_CROTATIONRADIALDISCOMPUTER_H)
#define _CROTATIONRADIALDISCOMPUTER_H

#include "ChanComputer.h"

namespace Computation
{
	class CRotationRadialDisComputer : public CChanComputer
	{
	public:
		/// 计算特征值
		virtual void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	protected:
		/// 特征值索引
		enum
		{
			IDX_EV_ALL,					///< 通频峰峰值
			IDX_EV_ONE,					///< 工频峰峰值 
			IDX_EV_TWO,					///< 2倍频峰峰值
			IDX_EV_THREE,				///< 3倍频峰峰值
			IDX_EV_HALF,				///< 分频峰峰值
			IDX_EV_RESVIB,				///< 残振值
			IDX_EV_ONE_PHASE,		    ///< 工频相位
			IDX_EV_TWO_PHASE,		    ///< 2倍频相位
			IDX_EV_THREE_PHASE,	        ///< 3倍频相位
			IDX_EV_HALF_PHASE,		    ///< 分频相位
			IDX_EV_DC,					///< 直流量
			IDX_EV_GAP,					///< 间隙电压
			IDX_EV_END
		};
	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_DC,            ///< 直流量
			IDX_IN_GAP,           ///< 间隙电压
			IDX_IN_SSK,           ///< 传感器比例系数K
			IDX_IN_UNITRATIO,     ///< 单位影响系数
			IDX_IN_END
		};
	};
}

#endif  //_CROTATIONRADIALDISCOMPUTER_H
