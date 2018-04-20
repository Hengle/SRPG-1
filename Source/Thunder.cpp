#include "stdafx.h"
#include "Stage.h"
#include "Thunder.h"


CThunder::CThunder()
{

}
CThunder::~CThunder()
{

}

void CThunder::Initialize()
{
	m_iRange = 4;
	m_iSphere = 1;
	m_iMultiplier = 2;
	m_pathType = E_PATH_ATTACK;
}

E_EXE_STATE CThunder::Update(void)
{
	switch (m_iState)
	{
	case 0:
		m_Frame = FRAME(0, 24, 0, 30);
		m_dwTime = GetTickCount();
		m_iCount = 0;
		m_iState = 1;

		for (int i = 0; i < m_pTargets.size(); i++)
		{
			AddFire(i);
		}

		GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_MAGIC3, 0);
		((CMarker*)m_pMarker)->SetFrame(21, 21);
		break;

	case 1:
		if (m_dwTime + m_Frame.dwTime < GetTickCount())
		{
			m_Frame.iScene++;
			m_dwTime = GetTickCount();
		}

		if (m_Frame.iScene > m_Frame.iLast)
		{
			m_iState = 2;
			m_dwTime = GetTickCount();
		}
		break;

	case 2:
			if (CStage::ZoomCam(
				m_pTargets[m_iCount]->GetInfo().fX,
				m_pTargets[m_iCount]->GetInfo().fY,
				10
			))
			{
				((CMarker*)m_pMarker)->SetFrame(16, 16);
				m_iState = 3;
			}
		break;

	case 3:
		m_tFires[m_iCount]->iState = 0;
		((CMarker*)m_pTargets[m_iCount])->SetFrame(21, 21);
		m_iCount++;
		m_dwTime = GetTickCount();

		if (m_iCount >= m_pTargets.size())
		{
			m_iState = 4;
		}
		else m_iState = 2;
	break;
	
	case 4:
		if (m_dwTime + 1000 < GetTickCount())
		{
			m_iState = 0;
			m_pTargets.clear();
			m_tFires.clear();
			return E_EXE_EXECUTE;
		}
	break;
	}

	DrawFire();

	return E_EXE_READY;
}


void CThunder::DrawFire()
{
	int time_;
	vector<CObj*>::iterator it;
	for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
	{
		if ((*fIt)->bFiring)
		{
			switch ((*fIt)->iState)
			{
			case 0:
				(*fIt)->dwTime = GetTickCount();
				(*fIt)->frame = FRAME(0, 24, 0, 50);
				(*fIt)->pf.x = m_pTargets[(*fIt)->iIndex]->GetInfo().fX;
				(*fIt)->pf.y = m_pTargets[(*fIt)->iIndex]->GetInfo().fY;
				(*fIt)->alpha = 255;
				(*fIt)->msDraw = "Thunder";

				GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_THUNDER, 0);
				(*fIt)->iState = 1;
				break;

			case 1:
				if (FrameFire(*fIt))
				{
					(*fIt)->frame.iScene = (*fIt)->frame.iLast;
					(*fIt)->msDraw = "";
					(*fIt)->iState = 2;
				}
			break;
			}
		}
	}
}

bool CThunder::FrameFire(PFIRE fire)
{
	if (fire->dwTime + fire->frame.dwTime > GetTickCount())
	{
		fire->frame.iScene++;
		fire->dwTime = GetTickCount();
	}

	if (fire->frame.iScene > fire->frame.iLast)
	{
		return true;
	}

	return false;
}


void CThunder::Render(HDC hdc)
{
	if (m_iState == 1)
	{
		vector<CObj*>::iterator it;
		m_tempInfo.fX = m_pMarker->GetInfo().fX + WINCX / 2 - CStage::m_tCam.x;
		m_tempInfo.fY = m_pMarker->GetInfo().fY + WINCY / 2 - CStage::m_tCam.y;
		m_tempInfo.fCX = 129;
		m_tempInfo.fCY = 129;

		(*m_pMapBmp)["Magic"]->Draw(&hdc,
			m_tempInfo,
			129 * (m_Frame.iScene % 5),
			129 * (m_Frame.iScene / 5),
			200, RGB(0, 0, 0));
	}

	for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
	{
		if ((*fIt)->msDraw == "")
			continue;

		m_tempInfo.fX = (*fIt)->pf.x + WINCX / 2 - CStage::m_tCam.x;
		m_tempInfo.fY = (*fIt)->pf.y + WINCY / 2 - CStage::m_tCam.y - 50;

		if ((*fIt)->msDraw == "Thunder")
		{
			m_tempInfo.fCX = 192;
			m_tempInfo.fCY = 192;
			(*m_pMapBmp)[(*fIt)->msDraw]->Draw(&hdc,
				m_tempInfo,
				192 * ((*fIt)->frame.iScene % 5),
				192 * ((*fIt)->frame.iScene / 5),
				(*fIt)->alpha,
				RGB(0, 0, 0));

			m_tempInfo.fY += 50;

			(*m_pMapBmp)["ThunderShock"]->Draw(&hdc,
				m_tempInfo,
				192 * ((*fIt)->frame.iScene % 5),
				192 * ((*fIt)->frame.iScene / 5),
				(*fIt)->alpha,
				RGB(0, 0, 0));
		}
	}
}

E_SCENE_ID CThunder::Progress()
{
	return E_SCENE_ID();
}

void CThunder::Release()
{

}
