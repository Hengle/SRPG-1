#include "stdafx.h"
#include "Curtain.h"


CCurtain::CCurtain()
{

}


CCurtain::~CCurtain()
{

}

void CCurtain::Initialize(void)
{
	m_rendering = true;
	m_bAlpha = 255;
	m_Info.fX = WINCX;
	m_Info.fY = WINCY;
	m_Info.fCX = WINCX;
	m_Info.fCY = WINCY;
}

E_SCENE_ID CCurtain::Progress(void)
{
	return E_SCENE_NONPASS;
}

bool CCurtain::Open(byte speed_)
{
	m_bAlpha -= speed_;
	if (m_bAlpha < speed_)
	{
		m_rendering = false;
		m_bAlpha = 0;
		return true;
	}
		
	else return false;
}

bool CCurtain::Close(byte speed_)
{
	m_rendering = true;
	m_bAlpha += speed_;
	if (m_bAlpha > 255 - speed_)
	{
		m_bAlpha = 255 - speed_;
		return true;
	}

	else return false;
}

void CCurtain::Render(HDC hdc)
{
	if (!m_rendering)
		return;

	(*m_pMapBmp)["BackGround"]->Draw(&hdc, m_Info, WINCX, WINCY, m_bAlpha, SRCCOPY);
}

void CCurtain::Release(void)
{

}

