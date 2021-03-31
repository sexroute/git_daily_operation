/*!\file ChanComputer.h
*  \brief 测点计算抽象基类和计算环境类的声明
*  \date 2009-2-16
*  \author 
*  \version
*/

#if !defined(_CHANCOMPUTER_H)
#define _CHANCOMPUTER_H
#include "IProcessor.h"
#include <fstream>
#include <iostream>



namespace Computation
{
	
	/// 测点计算类：抽象基类
	class CChanComputer
	{
	public:
		virtual void Compute(	
								const CNiReal32Vector &nivfWave_, 
								const CNiReal32Vector &nivfFFT_, 
								float fDt_, 
								float fFref_, 
								const CNiReal32Vector &nivfParam_, 
								CNiReal32Vector &nivfEV_) = 0;

	};


	int writefile(double * ag_fwave, int ag_num,int index);


}

#endif  //_CHANCOMPUTER_H
