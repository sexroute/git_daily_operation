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
		/// ��������ֵ
		virtual void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	protected:
		/// ����ֵ����
		enum
		{
			IDX_EV_ALL,					///< ͨƵ���ֵ
			IDX_EV_ONE,					///< ��Ƶ���ֵ 
			IDX_EV_TWO,					///< 2��Ƶ���ֵ
			IDX_EV_THREE,				///< 3��Ƶ���ֵ
			IDX_EV_HALF,				///< ��Ƶ���ֵ
			IDX_EV_RESVIB,				///< ����ֵ
			IDX_EV_ONE_PHASE,		    ///< ��Ƶ��λ
			IDX_EV_TWO_PHASE,		    ///< 2��Ƶ��λ
			IDX_EV_THREE_PHASE,	        ///< 3��Ƶ��λ
			IDX_EV_HALF_PHASE,		    ///< ��Ƶ��λ
			IDX_EV_DC,					///< ֱ����
			IDX_EV_GAP,					///< ��϶��ѹ
			IDX_EV_END
		};
	private:
		/// �����������
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

#endif  //_CROTATIONRADIALDISCOMPUTER_H
