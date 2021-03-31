#pragma once
#include "othercomputer.h"

namespace Computation
{
	/// ×²»÷²âµã¼ÆËãÆ÷
	class CImpactComputer : public COtherComputer
	{
	public:
		void Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_);
	private:
		enum
		{
			IDX_IN_LOW_FREQ,
			IDX_IN_HIGH_FREQ,
			IDX_IN_THRESHOLD,
			IDX_IN_END
		};
	};
}