#include "stdafx.h"
#include "Monster.h"



CMonster::CMonster()
{
}


CMonster::~CMonster()
{
}

void CMonster::FollowPlayer()
{
	float fWidth = m_pPlayer->GetInfo().fX - m_Info.fX;
	float fHeight = m_pPlayer->GetInfo().fY - m_Info.fY;
	float fDistance = sqrt(fWidth*fWidth + fHeight*fHeight);

	//��ũ�ڻ������� �������� ����/x
	m_fAngle = acosf(fWidth / fDistance) * 180 / PI; // ȣ�������� ����

	if (m_pPlayer->GetInfo().fY > m_Info.fY)	// �÷��̾� > ����
		m_fAngle = 360 - m_fAngle;

	m_Info.fX += cosf(m_fAngle * PI / 180) * m_fSpeed;
	m_Info.fY += -sinf(m_fAngle * PI / 180) * m_fSpeed;
}

void CMonster::Render(HDC hdc)
{
	// ���� �ø�
	float fWidth = m_Info.fX - m_pPlayer->GetInfo().fX;
	float fHeight = m_Info.fY - m_pPlayer->GetInfo().fY;
	float fDistance = sqrt(fHeight*fHeight + fWidth*fWidth);

	if (fDistance > WINCX / 2 + 100)
		return;

	TransparentBlt(hdc,
		int((m_Info.fX - m_Info.fCX / 2) + m_tScroll.x),
		int((m_Info.fY - m_Info.fCY / 2) + m_tScroll.y),
		(int)m_Info.fCX, (int)m_Info.fCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		0, 0,
		(int)m_Info.fCX, (int)m_Info.fCY,
		RGB(0, 255, 255));
}