#include "stdafx.h"
#include "Slash.h"
#include "Stage.h"


CSlash::CSlash()
{

}

CSlash::~CSlash()
{

}

void CSlash::Initialize()
{
	m_iRange = 1;
	m_iSphere = 0;
	m_iMultiplier = 1;
	m_pathType = E_PATH_ATTACK;
}

E_EXE_STATE CSlash::Update(void)
{
	switch (m_iState)
	{
		case 0:
			m_Frame = FRAME(0, 7, 0, 30);
			m_dwTime = GetTickCount();
			m_iState = 1;
			GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_SWORD, 0);
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
			if (m_dwTime + 200 < GetTickCount())
			{
				m_iState = 3;
				m_dwTime = GetTickCount();

				GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_DAMAGE, 0);

				vector<CObj*>::iterator it;
				for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
				{
					((CMarker*)(*it))->SetFrame(20, 20);
				}

				m_Frame = FRAME(0, 35, 0, 5);
			}
			break;

		case 3:
			if (m_dwTime + m_Frame.dwTime < GetTickCount())
			{
				m_Frame.iScene++;
				m_dwTime = GetTickCount();
			}

			if (m_Frame.iScene > m_Frame.iLast)
			{
				m_iState = 0;
				m_pTargets.clear();
				return E_EXE_EXECUTE;
			}
		break;

	}
	return E_EXE_READY;
}

void CSlash::Render(HDC hdc)
{
	if (m_iState == 1)
	{
		m_tempInfo.fCX = 192;
		m_tempInfo.fCY = 192;
		vector<CObj*>::iterator it;

		for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
		{
			m_tempInfo.fX = (*it)->GetInfo().fX + WINCX / 2 - CStage::m_tCam.x;
			m_tempInfo.fY = (*it)->GetInfo().fY + WINCY / 2 - CStage::m_tCam.y;

			(*m_pMapBmp)["Slash"]->Draw(&hdc,
				m_tempInfo,
				192 * (m_Frame.iScene % 6),
				192 * (m_Frame.iScene / 6),
				230, RGB(0, 0, 0));
		}
	}

	else if (m_iState == 3)
	{
		m_tempInfo.fCX = 67;
		m_tempInfo.fCY = 67;
		vector<CObj*>::iterator it;

		for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
		{
			m_tempInfo.fX = (*it)->GetInfo().fX + WINCX / 2 - CStage::m_tCam.x;
			m_tempInfo.fY = (*it)->GetInfo().fY + WINCY / 2 - CStage::m_tCam.y;

			(*m_pMapBmp)["Damage"]->Draw(&hdc,
				m_tempInfo,
				67 * (m_Frame.iScene % 6),
				67 * (m_Frame.iScene / 6),
				230, RGB(0, 255, 255));
		}
	}
}

E_SCENE_ID CSlash::Progress()
{
	return E_SCENE_ID();
}

void CSlash::Release()
{

}
