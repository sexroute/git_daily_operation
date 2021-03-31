/*!\file CRotationAccComputer.h
*  \��ת��е���ٶ�����ֵ������
*  \date 2009-12-02
*  \author 
*  \version
*/

#if !defined(_ROTATIONVIBCOMPUTER_H)
#define _ROTATIONVIBCOMPUTER_H

#include "CRotationRadialDisComputer.h"

namespace Computation
{
	class CRotationAccComputer : public CRotationRadialDisComputer
	{
	public:
		/// ��������ֵ
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
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

#endif  //_ROTATIONVIBCOMPUTER_H
