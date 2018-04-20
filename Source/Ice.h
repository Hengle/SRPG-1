#pragma once
#include "Skill.h"

class CIce : public CSkill
{
private:
	PFIRE ice;
public:
	E_EXE_STATE Update(void);
	void CreateIce(PFIRE fire);
	bool BreakIce(PFIRE fire);
public:
	void Initialize();
	void Render(HDC hdc);
	E_SCENE_ID Progress();
	void Release();
public:
	CIce();
	~CIce();
};

