#pragma once
#include "Include.h"

class CTile;
class CObj;

class CPathfinder
{
	DECLARE_SINGLETON(CPathfinder);
protected:
	CObj *m_pTile;
	list<PTILE> m_open;
	list<PTILE> m_closed;
	list<int> m_path;
	int m_maxStep;
protected:
	E_TEAM_ID m_eTeam;
	E_PATH_TYPE m_eType;
public:
	list<int>* ForgePath(int iIndex);
	void SetTile(CObj *tile_) { m_pTile = tile_; };
	virtual void Pathfind(int iStart_, int iSteps_, E_TEAM_ID eTeam_, E_PATH_TYPE eType_);
	virtual void Clear();
protected:
	virtual void CheckOpened();
	virtual void CheckNear(PTILE pCurr, int iNear);
public:
	CPathfinder();
	~CPathfinder();
};

