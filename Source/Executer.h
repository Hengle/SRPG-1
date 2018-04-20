#pragma once
#include "Pathfinder.h"
#include "Rangefinder.h"

class CExecuter
{
protected:
	E_STAGE_STATE m_eStage;
	E_EXE_STATE m_eExe;
	CObj *m_pMarker, *m_pTile;
	PTILE m_pTarget;
	int m_iCurr;
	vector<CObj*> *m_pMarkers;
	bool enabled;
protected:
	E_STAGE_STATE CancelSelect();
public:
	void Enable(bool switch_) { enabled = switch_; }
	void SetMarker(CObj* pMarker_) { m_pMarker = pMarker_; }
	void SetTile(CObj* pTile_) { m_pTile = pTile_; }
	void SetMarkers(vector<CObj*> *markers_) { m_pMarkers = markers_; }
public:
	E_STAGE_STATE Update(CObj* m_pMarker);
public:
	virtual E_STAGE_STATE Search(void)PURE;
	virtual E_STAGE_STATE Select(void)PURE;
	virtual E_STAGE_STATE Ready(void)PURE;
	virtual E_STAGE_STATE Execute(void)PURE;
	virtual E_STAGE_STATE Wait() { return E_STAGE_NONPASS; };
public:
	CExecuter();
	~CExecuter();
};

