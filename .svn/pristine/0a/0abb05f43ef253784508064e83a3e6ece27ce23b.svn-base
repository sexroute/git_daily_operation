/*!\file CRotationVecComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_CROTATIONVELOCCOMPUTER_H)
#define _CROTATIONVELOCCOMPUTER_H

#include "CRotationRadialDisComputer.h"

namespace Computation
{
	class CRotationVecComputer : public CRotationRadialDisComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
		/// 输入参数索引
		enum
		{
			IDX_IN_DC,		        ///< 直流量
			IDX_IN_GAP,		        ///< 间隙电压
			IDX_IN_END
		};
	};
}

#endif  //_CROTATIONVELOCCOMPUTER_H
