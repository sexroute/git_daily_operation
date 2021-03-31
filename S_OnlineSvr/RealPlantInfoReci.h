#pragma once
#include "plantinfo.h"

class CRealPlantInfoReci: public CRealPlantInfo
{
public:
	CRealPlantInfoReci(void);
	~CRealPlantInfoReci(void);
private:
	virtual void Destory();
};
