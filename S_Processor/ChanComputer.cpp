#include "stdafx.h"
#include "ChanComputer.h"

int Computation::writefile(double * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;

	char filenm[100]={0};

	CString strtmp;

	strtmp.Format("c:\\wave%d.txt",index);

	memcpy(filenm,strtmp,strtmp.GetLength());

	fileWave.open(filenm,ios::out);

	fileWave<<"	";

	for(int i=0;i<ag_num;i++)
	{
		fileWave<< ag_fwave[i]<<"	";
	}

	fileWave<<"\n";

	fileWave.close();

	return ag_num;
}