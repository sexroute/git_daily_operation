/*!\file CWindAndPumpVecComputer.h
*  \�������ٶ�����ֵ������
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
		/// ��������ֵ
		virtual void Compute(const CNiReal32Vector &nivfWave_, 
					 const CNiReal32Vector &nivfFFT_, 
					 float fDt_, 
					 float fFref_, 
					 const CNiReal32Vector &nivfParam_, 
					 CNiReal32Vector &nivfEV_);

	private:
		/// �����������
		enum
		{
			IDX_IN_DC,            ///< ֱ����
			IDX_IN_GAP,           ///< ��϶��ѹ
			IDX_IN_END
		};
	};
}

#endif  //_WINDANDPUMPVECCOMPUTER_H
