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
			IDX_IN_DC,            ///< ֱ����
			IDX_IN_GAP,           ///< ��϶��ѹ
			IDX_IN_SSK,           ///< ����������ϵ��K
			IDX_IN_UNITRATIO,     ///< ��λӰ��ϵ��
			IDX_IN_END
		};
	};
}

#endif  //_CROTATIONAXISCOMPUTER_H
