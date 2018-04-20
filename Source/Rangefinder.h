#pragma once
#include "Pathfinder.h"

class CTile;
class CObj;

class CRangefinder : public CPathfinder
{
	DECLARE_SINGLETON(CRangefinder);
private:
	list<int> m_targets;
private:
	virtual void CheckOpened();
	virtual void CheckNear(PTILE pCurr, int iNear);
public:
	virtual void Pathfind(int iStart_, int iSteps_, E_TEAM_ID eTeam_, E_PATH_TYPE eType_);
	virtual void Clear();
public:
	list<int>* GetTargets(E_PATH_TYPE type_);
public:
	CRangefinder();
	~CRangefinder();
};

