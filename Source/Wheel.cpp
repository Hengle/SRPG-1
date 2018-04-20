#include "stdafx.h"
#include "Wheel.h"
#include "RoundButton.h"
#include "Marker.h"
#include "Stage.h"
#include "SoundMgr.h"
#include "Attacker.h"

CWheel::CWheel()
{

}

CWheel::~CWheel()
{

}

void CWheel::CreateButton(int iDraw_, E_STAGE_STATE state_)
{
	CRoundButton* button = new CRoundButton();
	button->Initialize();
	button->SetState(state_);
	button->SetDraw(iDraw_);
	m_pButtons.push_back(button);
}

void CWheel::Initialize()
{
	CreateButton(2, E_STAGE_ENDTURN);
	CreateButton(1, E_STAGE_MOVE);
	CreateButton(0, E_STAGE_ATTACK);
	CreateButton(3, E_STAGE_ATTACK);

	m_state = 0;

	int i = 0;
	int angle = 360 / m_pButtons.size();
	vector<CRoundButton*>::iterator it;
	for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
	{
		(*it)->SetAngle(i * angle);
		i++;
	}
}

E_STAGE_STATE CWheel::Update(CMarker* curr_)
{
	m_pCurr = curr_;

	vector<CRoundButton*>::iterator it;
	for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
	{
		(*it)->SetPos(m_pCurr->GetInfo().fX - CStage::m_tCam.x + WINCX / 2,
					  m_pCurr->GetInfo().fY - CStage::m_tCam.y + WINCY / 2);
	}

	switch (m_state)
	{
		case 0:
			m_dwTime = GetTickCount();
			m_enabled = true;
			m_state = 1;
		break;

		case 1:
			for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
			{
				(*it)->AddAngle(10);
				(*it)->AddRadius(-15);
				(*it)->AddAlpha(30);
			}

			if (m_dwTime + 100 < GetTickCount())
			{
				m_state = 2;
			}
		break;

		case 2:
			for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
			{
				m_stageState = (*it)->Click();
				if (m_stageState > E_STAGE_NONPASS)
				{
					GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_SELECT, 0);
					m_dwTime = GetTickCount();
					m_state = 3;
					break;
				}
			}
		break;

		case 3:
			for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
			{
				(*it)->AddAngle(-10);
				(*it)->AddRadius(15);
				(*it)->AddAlpha(-30);
			}

			if (m_dwTime + 100 < GetTickCount())
			{
				m_dwTime = GetTickCount();
				m_enabled = false;
				m_state = 0;
				return m_stageState;
			}
		break;
	}

	return E_STAGE_DECIDE;
}

void CWheel::Render(HDC hdc)
{
	if (m_enabled)
	{
		vector<CRoundButton*>::iterator it;
		for (it = m_pButtons.begin(); it != m_pButtons.end(); ++it)
		{
			(*it)->Render(hdc);
		}
	}
}