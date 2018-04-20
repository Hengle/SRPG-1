#pragma once
#include "Skill.h"

class CSlash : public CSkill
{
public:
	CSlash();
	~CSlash();
public:
	E_EXE_STATE Update(void);
public:
	void Initialize();
	void Render(HDC hdc);
	E_SCENE_ID Progress();
	void Release();
};

