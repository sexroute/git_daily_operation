/*!\file DynPressComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_DYNPRESSCOMPUTER_H)
#define _DYNPRESSCOMPUTER_H

#include "ChanComputer.h"
#include "IProcessor.h"

namespace Computation
{
	class CDynPressComputer : public Computation::CChanComputer
	{
	public:
		virtual void Compute(const CNiReal32Vector &nivfWave_, 
							 const CNiReal32Vector &nivfFFT_, 
							 float fDt_, 
							 float fFref_, 
							 const CNiReal32Vector &nivfParam_, 
							 CNiReal32Vector &nivfEV_);
	protected:
		/// 特征值索引
		enum
		{
			IDX_EV_DYN_0,			///< 平均压力(KPa)
			IDX_EV_DYN_1,			///< 机械效率
			IDX_EV_DYN_2,			///< 吸气容积效率
			IDX_EV_DYN_3,			///< 排气容积效率
			IDX_EV_DYN_4,			///< 单缸指示功率
			IDX_EV_DYN_5,			///< 压缩指数
			IDX_EV_DYN_6,			///< 膨胀系数
			IDX_EV_DYN_7,			///< 实际压力比
			IDX_EV_DYN_8,			///< 峭度
			IDX_EV_DYN_9,			///< 歪度
			IDX_EV_START_IDX,	///< 起始索引
			IDX_EV_END_IDX,		///< 终止索引
			IDX_EV_END
		};
	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_IN_PRESS,		///< 理论进气压力(MPa)
			IDX_IN_OUT_PRESS,	///< 理论排气压力(MPa)
			IDX_IN_CLR_VOL,			///< 余隙容积(dm^3)
			IDX_IN_ROD_LEN,		///< 连杆长度(m)
			IDX_IN_HUB_RDS,		///< 轮毂半径(m)
			IDX_IN_PIST_AREA,		///< 活塞面积(m^2)
			IDX_IN_TOTAL_POW,	///< 总功率(KW)
			IDX_IN_REL_DEG,		///< 相对角度(°)
			IDX_IN_UNITRATIO,       ///< 单位影响系数
			IDX_IN_END
		};
	};
}

#endif  //_DYNPRESSCOMPUTER_H
