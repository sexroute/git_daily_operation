/*!\file OtherComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include "OtherComputer.h"
using namespace Computation;

void COtherComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	nivfEV_.SetSize(1);
	nivfEV_[0] = (nivfWave_.GetSize()>0) ? nivfWave_.Sum()/nivfWave_.GetSize() : 0;
}
