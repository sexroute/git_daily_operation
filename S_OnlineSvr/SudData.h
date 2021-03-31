#include "stdafx.h"

#ifndef _ONLINE_SUDDATA_H_
#define _ONLINE_SUDDATA_H_


//启停车设备信息类
class CSudPlantInfo
{
public:
	CSudPlantInfo(void)
	{
	}
	~CSudPlantInfo(void)
	{
		
	}
private:
	struct s_chann
	{
		int index;//处于测点序列中的第几个
		int iRevIndex;//属于那个转速
	};
	string _strPlantID;
    string _strSetID;
    map<string,s_chann> _mapChann;
	pair<string,s_chann> _pairChann;
	int  _iSmpNum;
	int  _iSmpFreq;
	int  _iRev;
	int  _iRev2;
	int  _iRatingRev1;
	int  _iRatingRev2;
	int  _iSize;//the sud channel number of plant
	//最大采样点数
	int _iMaxSamples;
	///频谱数据的df
	float _fFreqDataDf;
//启停车数据锁	
	CMutexRW _lockSudData;
public:
	float         *_pfPhase;
	float         *_pfAmp;
	float         **_ppfWave;

public:
	//initize plant all infomation
	void InitPlantInfo(string ag_strSetID,string ag_strPlantID,int ag_iMaxSamples,int ag_iRatingRev1,int ag_iRatingRev2)
	{
		_strSetID=ag_strSetID;
        _strPlantID=ag_strPlantID;
		_iRatingRev1=ag_iRatingRev1;
		_iRatingRev2=ag_iRatingRev2;
		_iMaxSamples=ag_iMaxSamples<2048?2048:ag_iMaxSamples;
	}
	//添加测点信息
	void AddChann(string ag_strChannID,int ag_index,int ag_iRevIndex)
	{
		_pairChann.first=ag_strChannID;
		_pairChann.second.index=ag_index;
		_pairChann.second.iRevIndex=ag_iRevIndex;
		_mapChann.insert(_pairChann);
	}
	//添加测点结束后，做指针分配内存操作
	void AddChannEnd(float *ag_pfPhase,float *ag_pfAmp,float **ag_pWave)
	{
		_iSize=_mapChann.size();
		_pfAmp=ag_pfAmp;
		_pfPhase=ag_pfPhase;
		_ppfWave=ag_pWave;
	}
	//更新设备相关实时数据参数
	int UpdateDataParam(float *ag_pfPhase,float *ag_pfAmp,float *ag_fWave,int ag_iRev, int ag_iRev2,int ag_iSampleNum,int ag_iSampleFreq,int ag_iRealPointNum,
		int ag_len,
		int ag_startIndex)
	{
		if ((_iSize<1)||(ag_startIndex<0)){
			return -1;
		}
		int reallen=(ag_startIndex+ag_len) > _iSize ?(_iSize-ag_startIndex):ag_len;
		
		_lockSudData.BeginWrite();
		_iRev=ag_iRev;
		_iRev2=ag_iRev2;
		_iSmpFreq=ag_iSampleFreq;
		_iSmpNum=ag_iSampleNum;
		int realPointNum=ag_iRealPointNum<_iMaxSamples? ag_iRealPointNum:_iMaxSamples;
		_fFreqDataDf=float(_iSmpFreq)/realPointNum;
        memcpy(&(_pfAmp[ag_startIndex]),ag_pfAmp,reallen*GD_FLOAT_SIZE);
		memcpy(&(_pfPhase[ag_startIndex]),ag_pfPhase,reallen*GD_FLOAT_SIZE);
		for (int loop=0;loop<reallen;++loop){
			memcpy(_ppfWave[loop+ag_startIndex],&(ag_fWave[loop*ag_iRealPointNum]),realPointNum*GD_FLOAT_SIZE);
			//	writefile(_ppfWave[loop+ag_startIndex],realPointNum,10+loop);
		}
		_lockSudData.EndWrite();
		return reallen;  
	}

	//得到这个设备启停车测点数目
	int GetChannLength(void)
	{
		return _iSize;
	}
	//get the current plant id
	string GetPlantID()
	{
		return _strPlantID;
	}

	//得到指定测点号的波形,返回值为实际读取得波形点数，如果为-1，就是没有指定的通道
	int GetSpecChannWave(string ag_strChannID,float *ag_pfWave,int ag_ilen/*读取得波形点数*/)
	{
		map<string,s_chann>::iterator result;
		result=_mapChann.find(ag_strChannID);
		if (result==(_mapChann.end()))
		{
			return -1;
		}
		_lockSudData.BeginRead();
		int ilen=ag_ilen<_iSmpNum?ag_ilen:_iSmpNum;
		memcpy(ag_pfWave,_ppfWave[result->second.index],ilen*sizeof(float));
		_lockSudData.EndRead();
        return ilen;
	}
	//得到指定测点号的波形和转速
	int GetSpecChannIndex(string ag_strChannID,int &ag_iRev,float &ag_fPhase,float &ag_fAmp,int &ag_iSmpNum,int &ag_iSmpFreq)
	{
		map<string,s_chann>::iterator result;
		result=_mapChann.find(ag_strChannID);
		if (result==(_mapChann.end()))
		{
			ag_iRev=0;
			return -1;
		}
		int index=result->second.index;
		_lockSudData.BeginRead();
		if ((result->second.iRevIndex)==1)
			ag_iRev=_iRev;
		else
			ag_iRev=_iRev2;
		ag_fAmp=_pfAmp[index];
		ag_fPhase=_pfPhase[index];
		ag_iSmpNum=_iSmpNum;
		ag_iSmpFreq=_iSmpFreq;
		_lockSudData.EndRead();
		return index;
	}
};
#endif //_ONLINE_SUDDATA_H_