/*!\file CWindAndPumpVecComputer.h
*  \风电机泵速度特征值计算类
*  \date 2010-11-18
*  \author 
*  \version
*/

#if !defined(_WINDANDPUMPVECCOMPUTER_H)
#define _WINDANDPUMPVECCOMPUTER_H

#include "CRotationRadialDisComputer.h"

namespace Computation
{
	class CWindAndPumpVecComputer : public CRotationRadialDisComputer
	{
	public:
		/// 计算特征值
		virtual void Compute(const CNiReal32Vector &nivfWave_, 
					 const CNiReal32Vector &nivfFFT_, 
					 float fDt_, 
					 float fFref_, 
					 const CNiReal32Vector &nivfParam_, 
					 CNiReal32Vector &nivfEV_);

	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_DC,            ///< 直流量
			IDX_IN_GAP,           ///< 间隙电压
			IDX_IN_END
		};
	};
}

#endif  //_WINDANDPUMPVECCOMPUTER_H
