#pragma once
#include "Skill.h"
class CThunder :
	public CSkill
{
private:
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
	int m_iCount;
public:
	E_EXE_STATE Update(void);
public:
	void Initialize();
	void Render(HDC hdc);
	E_SCENE_ID Progress();
	void Release();
public:
	CThunder();
	~CThunder();
};

