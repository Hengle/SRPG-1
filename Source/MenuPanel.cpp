#include "stdafx.h"
#include "MenuPanel.h"


CMenuPanel::CMenuPanel()
{

}


CMenuPanel::~CMenuPanel()
{

}

void CMenuPanel::Initialize()
{
	m_iScene = 0;
	m_gState = 0;
	m_alpha = m_arrowAlpha = 0;

	m_Info.fCX = 279;
	m_Info.fCY = 200;

	m_ArrowInfo.fX = 432;
	m_ArrowInfo.fY = 515;
	m_ArrowInfo.fCX = 35;
	m_ArrowInfo.fCY = 24;
}

E_SCENE_ID CMenuPanel::Progress(void)
{
	switch (m_gState)
	{
	case 0:
		m_alpha += 30;
		if (m_alpha > 230)
		{
			m_alpha = 230;
			m_gState = 1;
		}
		break;

	case 1:
		m_arrowAlpha = 230;
		MoveArrow();

		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			m_gState = 10;
			m_dwTime = GetTickCount();
			GET_SINGLE(CSoundMgr)->SoundPlay(E_MENU_SUD_START, 0);
		}

		break;

	case 10:
		if (m_dwTime + 1500 < GetTickCount())
			return E_SCENE_STAGE;
		break;

	default:
		break;
	}

	return E_SCENE_NONPASS;
}

void CMenuPanel::MoveArrow()
{
	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		if (m_iScene < 2)
		{
			GET_SINGLE(CSoundMgr)->SoundPlay(E_MENU_SUD_TICK, 0);
			m_iScene++;
			m_ArrowInfo.fY += 45;
		}

		else m_iScene = 2;
	}

	else if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		if (m_iScene > 0)
		{
			GET_SINGLE(CSoundMgr)->SoundPlay(E_MENU_SUD_TICK, 0);
			m_iScene--;
			m_ArrowInfo.fY -= 45;
		}
		else m_iScene = 0;
	}
}
void CMenuPanel::Render(HDC hdc)
{
	(*m_pMapBmp)["Menu"]->Draw(&hdc, m_Info, 0, 0, m_alpha, RGB(255, 0, 255));
	(*m_pMapBmp)["Arrow"]->Draw(&hdc, m_ArrowInfo, 0, 0, m_arrowAlpha, RGB(255, 0, 255));
}

void CMenuPanel::Release(void)
{

}
