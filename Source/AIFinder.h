#pragma once
#include "Pathfinder.h"

class CTile;
class CObj;

class CAIFinder : public CPathfinder
{
	DECLARE_SINGLETON(CAIFinder);
private:
	int m_iTarget;
	int m_iMaxStep, m_iSteps;
	bool m_bFound, m_second;
	list<int> m_targets;
private:
	void AddPath(PTILE near_, PTILE curr_);
	virtual void CheckOpened();
	virtual void CheckNear(PTILE pCurr, int iNear);
public:
	bool CheckFound() { return m_bFound; }
	bool CheckSecond() { return m_second; }
	void OffSecond() { m_second = false; }
	int GetTarget() { return m_iTarget; }
	int GetSteps() { return m_iSteps; }
	int GetMaxSteps() { return m_iMaxStep; }
	void RemoveSteps() { m_iMaxStep -= 1; m_iSteps++; }
	virtual void Pathfind(int iStart_, E_TEAM_ID eTeam_, E_PATH_TYPE ePath_);
	virtual void Clear();
	list<int>* ForgePath();
public:
	CAIFinder();
	~CAIFinder();
};

