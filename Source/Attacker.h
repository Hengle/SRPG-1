#pragma once
#include "Executer.h"
#include "Skill.h"

class CAttacker : public CExecuter
{
protected:
	list<int>* m_tTargets;
	DWORD m_dwTime;
	TCHAR m_tc;

	int m_damage;
	RECT rect;
	wchar_t wcTmp[128];
	INFO text;

public:
	static CSkill* m_pSkill;
public:
	int GetState() { return m_eExe; }
	void Render(HDC hdc);
public:
	virtual E_STAGE_STATE Search();
	virtual E_STAGE_STATE Select();
	virtual E_STAGE_STATE Ready();
	virtual E_STAGE_STATE Execute();
	virtual E_STAGE_STATE Wait();
public:
	CAttacker();
	~CAttacker();
};

