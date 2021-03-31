/*!\file CRotationAxisDisComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_CROTATIONAXISCOMPUTER_H)
#define _CROTATIONAXISCOMPUTER_H

#include "CRotationRadialDisComputer.h"

namespace Computation
{
	class CRotationAxisDisComputer : public CRotationRadialDisComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
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

#endif  //_CROTATIONAXISCOMPUTER_H
