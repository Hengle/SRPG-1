#pragma once
#include "Obj.h"
//#include "Skill.h"

class CSkill;
class CRoundButton : public CObj
{
private:
	bool m_bEnabled;
	E_STAGE_STATE m_state;
	float m_cos, m_sin, m_radius;
	int mouseState, m_alpha, m_iDraw;
	CSkill* m_skill;
public:
	CRoundButton();
	~CRoundButton();
public:
	E_STAGE_STATE Click();
	void SetSkill(CSkill* skill_) { m_skill = skill_; };
	void SetState(E_STAGE_STATE state_) { m_state = state_;  }
	void SetAngle(float angle_) 
	{ 
		m_fAngle = angle_; 
		m_cos = cos(m_fAngle * PI / 180);
		m_sin = sin(m_fAngle * PI / 180);
	}
	void AddAngle(float angle_) { SetAngle(m_fAngle + angle_); }
	void SetRadius(float radius_) { m_radius = radius_; }
	void AddRadius(float radius_) { SetRadius(m_radius + radius_); }
	void AddAlpha(int alpha_) { m_alpha += alpha_; }
	void Enable(bool switch_) { m_bEnabled = switch_;  }
	void SetDraw(int draw_) { m_iDraw = draw_; }
	
public:
	virtual void Initialize();
	virtual void Render(HDC hdc);
	virtual void Release();
	virtual E_SCENE_ID Progress(void);
};

