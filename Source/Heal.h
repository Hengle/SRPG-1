#pragma once
#include "Skill.h"

class CHeal : public CSkill
{
private:
	FRAME m_Frame2;
public:
	CHeal();
	~CHeal();
public:
	E_EXE_STATE Update(void);
public:
	void Initialize();
	void Render(HDC hdc);
	E_SCENE_ID Progress();
	void Release();
};

