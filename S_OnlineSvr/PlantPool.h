#pragma once
#include "StdAfx.h"
#include "PlantInfo.h"
template<class T>
class CInfoPool
{
public:
	CInfoPool()
	{
		_mapGroup.clear();
		_map_num_Group.clear();
	}
	~CInfoPool()
	{
		MAP_GROUP::iterator it;
		T *pPlantPool=NULL;
		for (it=_mapGroup.begin();it!=_mapGroup.end();++it) 
		{
			pPlantPool=it->second;
			if (pPlantPool!=NULL)
				delete pPlantPool;
			pPlantPool=NULL;
		}
		_mapGroup.clear();
	}
	T * GetSpecInfoPool(string sName_)
	{
		MAP_GROUP::iterator it;
		it=_mapGroup.find(sName_);
		CString strTmp;
		if (it==_mapGroup.end()) 
			return NULL;
		return it->second;
	}
	T * GetSpecInfoPool(int index_,string &sName_)
	{
		if ((index_ < 0)||(index_ > (_mapGroup.size()-1)))
			return NULL;
		MAP_GROUP::iterator it=_mapGroup.begin();
		for (int loop=0;loop<index_;++loop)	
			++it;
		if (it==_mapGroup.end()) 
			return NULL;
		sName_=it->first;
		return it->second;
	}

	T * GetSpecInfoPoolEx(int index_)
	{
		MAP_NUM_GROUP::iterator it=_map_num_Group.find(index_);		
		if (it==_map_num_Group.end()) 
		{	return NULL;
		}		
		return it->second;
	}

	void AddInfoPool(string sName_,T *pT_)
	{
		_mapGroup.insert(make_pair(sName_,pT_));
		int lnCurrentSize = _map_num_Group.size();
		this->_map_num_Group[lnCurrentSize] = pT_;
	}
	int GetInfoNum()
	{
		return _mapGroup.size();
	}
private:
	typedef map<string,T * > MAP_GROUP;
	typedef map<int,T * > MAP_NUM_GROUP;
	MAP_GROUP _mapGroup;
	MAP_NUM_GROUP _map_num_Group;
};

typedef CInfoPool<CPlantInfoOnLine> CPlantInfoPool;
typedef CInfoPool<CPlantInfoPool>   CFactoryInfoPool;
typedef CInfoPool<CFactoryInfoPool> CCompanyInfoPool;
