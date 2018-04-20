#include "stdafx.h"
#include "RoundButton.h"
#include "KeyInputMgr.h"
#include "Attacker.h"


CRoundButton::CRoundButton()
{
	Initialize();
}


CRoundButton::~CRoundButton()
{

}

void CRoundButton::Initialize()
{
	m_Info.fCX = 50;
	m_Info.fCY = 50;
	m_radius = 200;
	m_alpha = 0;
}

E_STAGE_STATE CRoundButton::Click()
{
	float fx_ = m_Info.fX + m_cos * m_radius - CMousePoint::GetPos().x;
	float fy_ = m_Info.fY + m_sin * m_radius - CMousePoint::GetPos().y;

	if (fx_ * fx_ + fy_ * fy_ < 625)
	{
		if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(0) == EINPUT_DOWN)
		{
			if (m_skill)
			{
				CAttacker::m_pSkill = m_skill;
				m_skill->Initialize();
			}

			return m_state;
		}
		mouseState = 2;
	}

	else mouseState = 0;
	

	return E_STAGE_NONPASS;
}

void CRoundButton::Render(HDC hdc)
{
	INFO temp;
	temp = m_Info;
	temp.fX += m_cos * m_radius;
	temp.fY += m_sin * m_radius;
	
	(*m_pMapBmp)["Wheel"]->Draw(&hdc,
		temp,
		50 * mouseState, 50 * m_iDraw, 
		m_alpha, RGB(0, 0, 12));
}

void CRoundButton::Release()
{

}

E_SCENE_ID CRoundButton::Progress(void)
{
	return E_SCENE_ID();
}
