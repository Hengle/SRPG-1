#pragma once
#include "Obj.h"
#include "RoundButton.h"
#include "Slash.h"
#include "Heal.h"
#include "Fire.h"

class CMarker;
class CWheel : public CObj
{
public:
	vector<CRoundButton*> m_pButtons;
	E_STAGE_STATE m_stageState;
	CMarker* m_pCurr;
	int m_state;
	bool m_enabled;
public:
	void SetSkillAt(int id_, CSkill* skill_) { m_pButtons[id_]->SetSkill(skill_); }
	void CreateButton(int iDraw_, E_STAGE_STATE state_);
	void AddButton(CRoundButton* button) { m_pButtons.push_back(button); }
	void Enable(bool switch_) { m_enabled = switch_; }
public:
	void Initialize();
	E_STAGE_STATE Update(CMarker* curr_);
	void Render(HDC hdc);
public:
	virtual E_SCENE_ID Progress(void) { return E_SCENE_NONPASS;  };
	virtual void Release(void) {};
public:
	CWheel();
	~CWheel();
};

