/*!\file VelocComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_VELOCCOMPUTER_H)
#define _VELOCCOMPUTER_H

#include "VibComputer.h"

namespace Computation
{
	class CVelocComputer : public CVibComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_DC,		        ///< 直流量
			IDX_IN_GAP,		        ///< 间隙电压
			IDX_IN_CHANTYPE,		///< 设备类型
			IDX_IN_END
		};
	};
}

#endif  //_VELOCCOMPUTER_H
