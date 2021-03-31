/*!\file RodComputer.h
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_RODCOMPUTER_H)
#define _RODCOMPUTER_H

#include "DynPressComputer.h"

namespace Computation
{
	class CRodComputer : public CDynPressComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
		/// �����������
		enum
		{
			IDX_IN_PISROD_LEN,		///< �����˳���(m)
			IDX_IN_HUB_RDS,		    ///< ��챰뾶(m)
			IDX_IN_SENS_POS,		///< ������λ��(m)
			IDX_IN_PIS_THICK,		///< �������(m)
			IDX_IN_PIS_CW_GAP,	    ///< ����-���ױڼ�϶(m)
			IDX_IN_GAP,      	    ///< ��϶��ѹ
			IDX_IN_ROD_LEN,         ///< ���˳���(m)
			IDX_IN_UNITRATIO,       ///< ��λӰ��ϵ��
			IDX_IN_END
		};
	};
}

#endif  //_RODCOMPUTER_H
