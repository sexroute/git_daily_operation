/*!\file CWindAndPumpAccComputer.h
*  \brief
*  \date 2009-8-15
*  \author 
*  \version
*/

#if !defined(_CWINDANDPUMPACCCOMPUTER_H)
#define _CWINDANDPUMPACCCOMPUTER_H

#include "CRotationRadialDisComputer.h"

namespace Computation
{
	class CWindAndPumpAccComputer :public CRotationRadialDisComputer
	{
	public:

		virtual void Compute(const CNiReal32Vector &nivfWave_, 
							 const CNiReal32Vector &nivfFFT_, 
							 float fDt_, 
							 float fFref_, 
							 const CNiReal32Vector &nivfParam_, 
							 CNiReal32Vector &nivfEV_);
	private:
		enum
		{
			IDX_IN_FC,
			IDX_IN_END
		};
	};
}
#endif  //_CWINDANDPUMPACCCOMPUTER_H