// Cacl.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"
#include <math.h>
#include "Calc1.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


float CalcCharValue(float *pwave,int n,int type)
{
   float fcharvalue;
   switch(type)
   {
   case PDE_CHAR_RMS:
        fcharvalue=CalcRMS(pwave,n);
		break;
   case PDE_CHAR_KUR:
   default:
	   fcharvalue=CalcKurtosis(pwave,n);
	   break;
   }
   return fcharvalue;
} 

void SpectralMeasureMent(float *pwave,int n,float *pout,int type) 
{
	
     int nloop=0;
	 CNiReal64Vector niWave,niPhase,niAmp;
	 niWave.SetSize(n);
     niPhase.SetSize(n/2);
     niAmp.SetSize(n/2);
	 for (nloop=0;nloop<n;++nloop){
		 niWave[nloop]=pwave[nloop];
	 }
	 double df;
	 switch(type)
	 {
	 case POWERSPECTRAL:
		
		 break;
	 case PEAKSPECTRAL:
		 CNiMath::AmpPhaseSpectrum(niWave,niAmp,niPhase,df);
		 break;
	 default:
       
		 break;
	 }
	 pout[0]=pout[1]=0;//note:added by marble(2004/06/10) 
}

void EvenlopeSpectral(float *pwave,int n,float *pout,int type)
{
     float *preal,*pimag;
	 int n2;

	 n2=n/2;
	 preal=new float[n];
	 pimag=new float[n];
	 memcpy(preal,pwave,n*sizeof(float));
	 memset(pimag,0,n*sizeof(float));

	 Hilbert(preal,pimag,n);
	 //求模
	 for(int loopn=0;loopn<n;++loopn)
	 {
		 preal[loopn]=sqrt(preal[loopn]*preal[loopn]+pimag[loopn]*pimag[loopn]);     
//		 		 pout[loopn]=sqrt(preal[loopn]*preal[loopn]+pimag[loopn]*pimag[loopn]);     
	 }
	 delete pimag;
     SpectralMeasureMent(preal,n,pout,type);
	 delete preal;
}

void VelocityWave(float *pwave,int n,float *pout)
{

}

///进行DFT，基于时间抽取的
void  Fft2_DIT(float *pr,float *pi,int n,int nflag)
{
    //float *pf_imagine,*pf_real;
	int m,j,n1,k,n2,p,q;
    float fxt,fyt,fee,fsin,fcos,fa,fxx,fyy,ftype;

	m=CalcLog2(n);
	/*pf_imagine=new float[n];
    pf_real=new float[n];
	memcpy(pf_real,pwave,n*sizeof(float));
    memset(pf_imagine,0,n*sizeof(float));*/
//inverse bits
	j=0;
	n1=n-1;
	n2=n/2;
	for(int i=0;i<n1;++i)
	{
		if(i<j)
		{
			fxt=pr[j];
            pr[j]=pr[i];
			pr[i]=fxt;
            
			fxt=pi[j];
            pi[j]=pi[i];
			pi[i]=fxt;

		}
		k=n2;
		while(k<=j)
		{
			j=j-k;
			k=k/2;
		}
		j=j+k;
	}
//calculate FFT	
	if(nflag==0)
        ftype=-2*PI;
	else
		ftype=2*PI;
    for(k=1;k<=m;++k)
	{
		n2=pow(2,k-1);
		n1=pow(2,k);
        fee=ftype/n1;
		for(j=0;j<n2;++j)
		{
            fa=j*fee;
			fcos=cos(fa);
			fsin=sin(fa);
			for(p=j;p<n;p+=n1)
			{
				q=p+n2;
				
				fxt=pr[p];
				fyt=pi[p];

				fxx=pr[q]*fcos-pi[q]*fsin;
				fyy=pr[q]*fsin+pi[q]*fcos;

				pr[p]=fxt+fxx;
				pi[p]=fyt+fyy;
				pr[q]=fxt-fxx;
				pi[q]=fyt-fyy;
			}
		}
	}
	if(nflag==1)
	{
		for(int loopn=0;loopn<n;++loopn)
		{	
			pr[loopn]=pr[loopn]/n;
			pi[loopn]=pi[loopn]/n;
		}

	}
//计算模，并进行标准化
	/*k=n/2;
    fxx=2.0/n;
	for(j=0;j<k;++j)
	{
         pout[j]=fxx*sqrt(pf_real[j]*pf_real[j]+pf_imagine[j]*pf_imagine[j]);
	}
    
    delete pf_imagine;
    delete pf_real;*/
}
//对波形进行加窗
void  WindowedWave(float *pwave, int n,int type)
{
	float  factor;
	int i;
	
	switch(type)
	{ 
	case WINDOW_HANNING:
		for(i=0;i<n;i++)
		{ /*noted(marble):应该是factor=(float)(0.5)*(1-cos(6.2831853*i/(n-1))),但是为了和labview里面一致，就去掉0.5.*/
			factor=(float)(1-cos(6.2831853*i/(n-1)));
		    pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_HAMMING:
		for(i=0;i<n;i++)
		{   
			factor=(float)(0.54-0.46*cos(6.2831853*i/(n-1)));
		    pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_BLACKMAN:
		for(i=0;i<n;i++)
		{   
			factor=(float)(0.42-0.5*cos(2*PI*i/(n-1))+0.08*cos(4*PI*i/(n-1)));
		    pwave[i]=pwave[i]*factor;
		}
		break;
	case WINDOW_NO:
	default:
		break;
	}
}

void  Hilbert(float *preal,float *pimag,int n)
{
   int n2;
  
   Fft2_DIT(preal,pimag,n,0);

   n2=n/2;
   //
   for(int loopn=1;loopn<n2;++loopn)
   {
     preal[loopn]=preal[loopn]*2;
     pimag[loopn]=pimag[loopn]*2;
	 preal[loopn+n2]=0;
     pimag[loopn+n2]=0;
   }
   //
   Fft2_DIT(preal,pimag,n,1);
}

int CalcLog2(int n)
{
	//添加switch语句，是为了减少运算量。常用的就直接赋值，不用计算
	int nlog2;
	
	switch(n)
	{
	case 4096:
        nlog2=12;
		break;
	case 2048:
		nlog2=11;
		break;
	case 1024:
		nlog2=10;
		break;
	case 512:
		nlog2=9;
		break;
	case 256:
		nlog2=8;
		break;
	default:
		for(int i=0;i<16;++i)
		{
			nlog2=i+1;
			n/=2;
			if(n==1) 
				break;
		}
		break;
	}
	return nlog2;  
}
//计算有效值（均方根）
float CalcRMS(float *pwave,int n)
{
	float frms=0;

	for(int loopn=0;loopn<n;++loopn)
	{
        frms+=pwave[loopn]*pwave[loopn];
	}
	frms=frms/n;
	frms=sqrt(frms);
	return frms;
}
//计算峭度值
float CalcKurtosis(float *pwave,int n)
{
	float ans;
 	float aver = 0.,sum1=0.,sum2=0.;
	for(int loopn=0;loopn<n;++loopn)
		aver += pwave[loopn];
	aver = aver/n;//样本序列均值aver

	for(loopn=0;loopn<n;++loopn)
	{   
		ans=pwave[loopn]-aver;
		sum1+=ans*ans*ans*ans;
		sum2+=ans*ans;
	}
	return sum1*n/(sum2*sum2);//返回峭度系数值
   
}
//标准计算轴承的通过频率
S_BearingDefectFreq  CalcBearingDefectFreq(S_BearingPar bearingpar)
{
   S_BearingDefectFreq sdf;
   float fworkfreq2,fdiam,fRadian;
   
   if((bearingpar.fPitchDiam<0.1)||(bearingpar.nNumofRolling<0.1)||(bearingpar.fWorkFreq<0.01)||(bearingpar.fRollingDiam<0.1))
   {
	   MessageBox(NULL,TEXT("对不起，你的输入有误！"),TEXT("警告"),MB_ICONERROR|MB_OK);
	   return sdf;
   }
   fworkfreq2=0.5*bearingpar.fWorkFreq;
   fRadian=(float)((bearingpar.fContackAngle)*3.1415/180);//note(2004/05/19):Modified by Marble 
   fdiam=cos(fRadian)*(bearingpar.fRollingDiam)/(bearingpar.fPitchDiam);
   //内圈通过频率
   sdf.fInnerFreq=fworkfreq2*(1+fdiam)*(bearingpar.nNumofRolling);
   //外圈通过频率
   sdf.fOuterFreq=fworkfreq2*(1-fdiam)*(bearingpar.nNumofRolling);
   //滚动体通过频率
   sdf.fRollingFreq=fworkfreq2*(bearingpar.fPitchDiam/bearingpar.fRollingDiam)*(1-fdiam*fdiam);
   //保持架通过频率
   sdf.fCageFreq=fworkfreq2*(1-fdiam);
   return sdf;
}
//简化近似计算轴承的通过频率,注意：没有滚动体的通过频率
S_BearingDefectFreq  ApCalcBearingDefectFreq(float fworkfreq,int rollingnum)
{
	S_BearingDefectFreq sdf;
	float ftmp;
    
	if((fworkfreq<0.01)||(rollingnum<1))
    {
	   MessageBox(NULL,TEXT("对不起，你的输入有误！"),TEXT("警告"),MB_ICONERROR|MB_OK);
 	   return sdf;
    }
	ftmp=fworkfreq*rollingnum;
    //内圈通过频率
	sdf.fInnerFreq=0.6*ftmp;
	//外圈通过频率
	sdf.fOuterFreq=0.4*ftmp;
	//保持架通过频率
	sdf.fCageFreq=0.4*fworkfreq;
	sdf.fRollingFreq=0;
	return sdf;
}

//calculate cepstrum 
void  CepstrumAnalysis(float *pwave,int n,float *pout)
{
   int n2;
   
   float *pimag=new float[n];
//   float *preal=new float[n];
   memcpy(pout,pwave,n*sizeof(float));
   memset(pimag,0,n*sizeof(float));
  
   Fft2_DIT(pout,pimag,n,0);
   for(int loopn=0;loopn<n;++loopn)
   {
     pout[loopn]=(float)sqrt(pout[loopn]*pout[loopn]+pimag[loopn]*pimag[loopn]);
     if(pout[loopn]==0)
		 pout[loopn]=1.0e-6;
	 pout[loopn]=(float)log(double(pout[loopn]));
   }
   //
   memset(pimag,0,n*sizeof(float));
   Fft2_DIT(pout,pimag,n,1);

   delete pimag;
}