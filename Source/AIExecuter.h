#pragma once
#include "Include.h"
#include "Executer.h"
#include "Skill.h"

class CAIExecuter : public CExecuter
{
private:
	POINTFLOAT m_tPoint;
	list<int>* m_tPath;
	int m_iWalkTimer;
	int m_attackState;
	int m_iNext, m_iDest;
	list<int>* m_tTargets;
	DWORD m_dwTime;
public:
	static CSkill* m_pSkill;
public:
	void Render(HDC hdc);
public:
	virtual E_STAGE_STATE Search();
	virtual E_STAGE_STATE Select();
	virtual E_STAGE_STATE Ready();
	virtual E_STAGE_STATE Execute();
public:
	CAIExecuter();
	~CAIExecuter();
};

