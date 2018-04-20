#pragma once
#include "Skill.h"

class CFire : public CSkill
{
private:
	int m_iCount, m_iAngle;

	vector<PFIRE>::iterator fIt;
	vector<PFIRE> m_tFires;
private:
	void AddFire(int index_) 
	{
		PFIRE fire_ = new FIRE;
		m_tFires.push_back(fire_);
		fire_->iIndex = index_;
		fire_->msDraw = "";
	}
	void DrawFire();
	bool FrameFire(PFIRE fire);
public:
	CFire();
	~CFire();
public:
	E_EXE_STATE Update(void);
public:
	void Initialize();
	void Render(HDC hdc);
	E_SCENE_ID Progress();
	void Release();
};

