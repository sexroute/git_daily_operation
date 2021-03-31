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
		/// ����ֵ����
		enum
		{
			IDX_EV_DYN_0,			///< ƽ��ѹ��(KPa)
			IDX_EV_DYN_1,			///< ��еЧ��
			IDX_EV_DYN_2,			///< �����ݻ�Ч��
			IDX_EV_DYN_3,			///< �����ݻ�Ч��
			IDX_EV_DYN_4,			///< ����ָʾ����
			IDX_EV_DYN_5,			///< ѹ��ָ��
			IDX_EV_DYN_6,			///< ����ϵ��
			IDX_EV_DYN_7,			///< ʵ��ѹ����
			IDX_EV_DYN_8,			///< �Ͷ�
			IDX_EV_DYN_9,			///< ���
			IDX_EV_START_IDX,	///< ��ʼ����
			IDX_EV_END_IDX,		///< ��ֹ����
			IDX_EV_END
		};
	private:
		/// �����������
		enum
		{
			IDX_IN_IN_PRESS,		///< ���۽���ѹ��(MPa)
			IDX_IN_OUT_PRESS,	///< ��������ѹ��(MPa)
			IDX_IN_CLR_VOL,			///< ��϶�ݻ�(dm^3)
			IDX_IN_ROD_LEN,		///< ���˳���(m)
			IDX_IN_HUB_RDS,		///< ��챰뾶(m)
			IDX_IN_PIST_AREA,		///< �������(m^2)
			IDX_IN_TOTAL_POW,	///< �ܹ���(KW)
			IDX_IN_REL_DEG,		///< ��ԽǶ�(��)
			IDX_IN_UNITRATIO,       ///< ��λӰ��ϵ��
			IDX_IN_END
		};
	};
}

#endif  //_DYNPRESSCOMPUTER_H
