#include "stdafx.h"
#include "Stage.h"
#include "Heal.h"


CHeal::CHeal()
{

}


CHeal::~CHeal()
{

}

void CHeal::Initialize()
{
	m_iRange = 3;
	m_iSphere = 0;
	m_iMultiplier = -1.5;
	m_pathType = E_PATH_HEAL;
}

E_EXE_STATE CHeal::Update(void)
{
	switch (m_iState)
	{
	case 0:
		//if (((CMarker*)(*m_pTargets.begin()))->)


		m_Frame = FRAME(0, 24, 0, 30);
		m_dwTime = GetTickCount();
		m_iState = 1;
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
		if (m_dwTime + 200 < GetTickCount())
		{
			m_iState = 3;
			m_dwTime = GetTickCount();

			GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_HEALING, 0);

			((CMarker*)m_pMarker)->SetFrame(16, 16);

			m_Frame = FRAME(0, 63, 0, 5);
			m_Frame2 = FRAME(0, 29, 0, 5);
		}
		break;

	case 3:
		if (m_dwTime + m_Frame.dwTime < GetTickCount())
		{
			m_Frame.iScene++;
			m_Frame2.iScene++;
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

void CHeal::Render(HDC hdc)
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

	else if (m_iState == 3)
	{
		vector<CObj*>::iterator it;
		for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
		{
			m_tempInfo.fCX = 92;
			m_tempInfo.fCY = 92;
			m_tempInfo.fX = (*it)->GetInfo().fX + WINCX / 2 - CStage::m_tCam.x;
			m_tempInfo.fY = (*it)->GetInfo().fY + WINCY / 2 - CStage::m_tCam.y;

			(*m_pMapBmp)["Healing"]->Draw(&hdc,
				m_tempInfo,
				92 * (m_Frame.iScene % 8),
				92 * (m_Frame.iScene / 8),
				200, RGB(0, 0, 0));

			m_tempInfo.fCX = 192;
			m_tempInfo.fCY = 192;

			(*m_pMapBmp)["Curing"]->Draw(&hdc,
				m_tempInfo,
				192 * (m_Frame.iScene % 5),
				192 * (m_Frame.iScene / 6),
				200, RGB(0, 255, 0));
		}
	}
}

E_SCENE_ID CHeal::Progress()
{
	return E_SCENE_ID();
}

void CHeal::Release()
{

}
