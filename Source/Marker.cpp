#include "stdafx.h"
#include "Marker.h"
#include "Tile.h"


CMarker::CMarker()
{

}


CMarker::~CMarker()
{

}

void CMarker::Init(int index_, E_TEAM_ID team_)
{
	m_iIndex = index_;
	m_eTeam = team_;
}

void CMarker::SetAbility(int speed_, int agility_)
{
	m_iSpd = speed_;
	m_iAgi = agility_;
}

void CMarker::SetCurr(int tile_)
{
	m_iCurr = tile_;
	m_Info.fX = TILECX * (tile_ % STAGEX) + TILECX / 2;
	m_Info.fY = TILECY * (tile_ / STAGEX) + TILECY / 2;
	((CTile*)m_pTile)->ChangeTile(tile_, m_iIndex, m_eTeam);
}

void CMarker::Initialize()
{
	m_iMiniAlpha = 0;
	m_Info.fCX = 64;
	m_Info.fCY = 64;
	m_alpha = 230;

	m_pMarker.fX = 936;
	m_pMarker.fY = 85;

	m_pMarker.fCX = 30;
	m_pMarker.fCY = 30;

	m_Frame = FRAME(0, 0, 0, 100);
	m_dwTime = GetTickCount();
	m_bAlive = true;
	m_iSpd = m_iAgi = m_iRng = m_iAtt = 3;
	m_iMaxHP = m_iHP = 10;
}

void CMarker::Move(int fX_, int fY_)
{
	float tempX = m_Info.fX + fX_;
	float tempY = m_Info.fY + fY_;

	FaceDirection(tempX, tempY, true);
	m_Info.fX = tempX;
	m_Info.fY = tempY;

	CStage::m_tCam.x = m_Info.fX;
	CStage::m_tCam.y = m_Info.fY;
}

void CMarker::FaceDirection(float fx_, float fy_, bool walk)
{
	if (fx_ < m_Info.fX)
	{
		if (walk)
			SetFrame(4, 7);
		else SetFrame(4, 4);
	}

	else if (fx_ > m_Info.fX)
	{
		if (walk)
			SetFrame(8, 11);
		else SetFrame(8, 8);
	}

	else if (fy_ < m_Info.fY)
	{
		if (walk)
			SetFrame(12, 15);
		else SetFrame(12, 12);
	}

	else
	{
		if (walk)
			SetFrame(0, 3);
		else SetFrame(0, 0);
	}
}

void CMarker::Damage(int att)
{
	m_iHP -= att;
	m_iMiniAlpha = 250;

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		m_bAlive = false;
		((CTile*)m_pTile)->GetTile(m_iCurr)->bAlive = false;
		SetFrame(22, 22);

		if (m_eTeam != E_TEAM_PLAYER)
		{
			PTILE tile = ((CTile*)m_pTile)->GetTile(m_iCurr);
			tile->eTeam = E_TEAM_NULL;
			tile->bFull = false;
			tile->iMarker = -1;
		}
	}

	else if (m_iHP >= m_iMaxHP)
	{
		m_iMiniAlpha = 250;
		m_iHP = m_iMaxHP;
	}
}

void CMarker::Frame()
{
	if (m_Frame.dwTime + m_dwTime < GetTickCount())
	{
		m_Frame.iScene++;
		m_dwTime = GetTickCount();
	}

	if (m_Frame.iScene > m_Frame.iLast)
		m_Frame.iScene = m_Frame.iStart;
}

void CMarker::EndTurn()
{
	m_pMarker.fX = 936;
}

E_STAGE_STATE CMarker::UpdateWait()
{
	m_pMarker.fX -= m_iSpd;

	if (m_pMarker.fX < 80)
		return E_STAGE_FOCUS;

	return E_STAGE_WAIT;
}

void CMarker::Render(HDC hdc)
{
	Frame();

	if (!m_bAlive && m_eTeam != E_TEAM_PLAYER)
	{
		m_alpha -= 2;
	}

	if (m_alpha < 0)
		m_alpha = 0;
	//Character
	float x = WINCX / 2 - CStage::m_tCam.x;
	float y = WINCY / 2 - CStage::m_tCam.y;

	INFO temp;
	temp = m_Info;
	temp.fX += x;
	temp.fY += y;

	(*m_pMapBmp)[m_strDrawID]->Draw(&hdc, temp, 
		64 * (m_Frame.iScene % 4) + 256 * (m_iClass % 4), 
		64 * (m_Frame.iScene / 4) + 384 * (m_iClass / 4),
		m_alpha, RGB(0, 0, 0));

	if (m_bAlive)
	{
		if (m_eTeam == E_TEAM_PLAYER)
			m_pMarker.fY -= 20;
		else m_pMarker.fY += 20;

		(*m_pMapBmp)["Marker"]->Draw(&hdc,
			m_pMarker, 
			30 * (m_iClass % 4), 
			30 * (m_iClass / 4), 
			255, SRCCOPY);

		m_pMarker.fY = 85;
	}

	temp.fCX = 64 * GetHPPercent();
	temp.fCY = 5;
	temp.fY += 24;

	m_iMiniAlpha --;
	if (m_iMiniAlpha < 1)
		m_iMiniAlpha = 1;

	(*m_pMapBmp)["MiniBar"]->Draw(&hdc, temp,
		0, 0, m_iMiniAlpha, SRCCOPY);

	
	/*
	x += 16;
	y += 16;

	Rectangle(hdc,
		int(m_Info.fX + x - m_Info.fCX / 2),
		int(m_Info.fY + y - m_Info.fCY / 2 - 30),
		int(m_Info.fX + x - m_Info.fCX / 2 + m_iHP * 3),
		int(m_Info.fY + y - m_Info.fCY / 2 - 20));
	*/
		
		
}

void CMarker::Release()
{

}
