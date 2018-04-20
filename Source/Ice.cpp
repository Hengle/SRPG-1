#include "stdafx.h"
#include "Stage.h"
#include "Math.h"

#include "Ice.h"


void CIce::Initialize()
{
	m_iRange = 6;
	m_iSphere = 1;
	m_iMultiplier = 1;
	m_iState = 0;
	m_pathType = E_PATH_ATTACK;

	ice = new FIRE;
	ice->msDraw = "";
	ice->frame = FRAME(0, 29, 0, 30);
	ice->alpha = 255;
}

E_EXE_STATE CIce::Update(void)
{
	vector<CObj*>::iterator it;
	switch (m_iState)
	{
		case 0:
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
				ice->pf.x = m_pMarker->GetInfo().fX;
				ice->pf.y = m_pMarker->GetInfo().fY;
				m_iState = 2;
				m_dwTime = GetTickCount();
			}
			break;

		case 2:
			if (m_dwTime + 200 < GetTickCount())
			{
				m_iState = 3;
				m_dwTime = GetTickCount();

				GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_ICECREATE, 0);
				m_Frame.iScene = m_Frame.iStart;

				ice->dwTime = GetTickCount();
				ice->msDraw = "Ice";
				ice->iTime = 0;
				ice->pfZoom.y = ice->pf.y;

				((CMarker*)m_pMarker)->SetFrame(16, 16);
			}
			break;

		case 3:
			CreateIce(ice);
			ice->pf.y = CMath::EaseIn(
				ice->iTime,
				ice->pfZoom.y,
				m_pMarker->GetInfo().fY - 100 - ice->pfZoom.y,
				20
			);


			CStage::m_tCam.x = ice->pf.x;
			CStage::m_tCam.y = ice->pf.y;

			ice->iTime++;

			if (ice->iTime > 20)
			{
				m_dwTime = GetTickCount();
				ice->iTime = 0;
				ice->pfZoom.x = ice->pf.x;
				ice->pfZoom.y = ice->pf.y;
				m_iState = 4;
			}
		break;

		case 4:
			CreateIce(ice);
			if (m_dwTime + 500 < GetTickCount())
			{
				if (ice->iTime == 0)
					GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_ICE, 0);
				ice->pf.x = CMath::EaseIn(
					ice->iTime,
					ice->pfZoom.x,
					m_pTargets[0]->GetInfo().fX - ice->pfZoom.x,
					60
					);

				ice->pf.y = CMath::EaseIn(
					ice->iTime,
					ice->pfZoom.y,
					m_pTargets[0]->GetInfo().fY - ice->pfZoom.y,
					60
				);
				CStage::m_tCam.x = ice->pf.x;
				CStage::m_tCam.y = ice->pf.y;

				ice->iTime++;

				if (ice->iTime > 60)
				{
					m_dwTime = GetTickCount();
					ice->dwTime = GetTickCount();
					ice->iTime = 0;
					ice->pfZoom.x = ice->pf.x;
					ice->pfZoom.y = ice->pf.y;
					m_iState = 5;
					GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_ICEBREAK, 0);

					for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
					{
						((CMarker*)(*it))->SetFrame(20, 20);
					}
				}
			}
		break;

		case 5:
			if (BreakIce(ice))
				m_iState = 6;
		break;

		case 6:

			if (m_dwTime + 1000 < GetTickCount())
			{
				m_iState = 0;
				m_pTargets.clear();
				delete ice;
				return E_EXE_EXECUTE;
			}

			break;
	}

	return E_EXE_READY;
}

bool CIce::BreakIce(PFIRE fire)
{
	if (fire->dwTime + fire->frame.dwTime > GetTickCount())
	{
		fire->frame.iScene++;
		fire->dwTime = GetTickCount();
	}

	if (fire->frame.iScene > fire->frame.iLast)
	{
		fire->frame.iScene = fire->frame.iLast;
		return true;
	}

	return false;
}

void CIce::CreateIce(PFIRE fire)
{
	if (fire->dwTime + fire->frame.dwTime > GetTickCount())
	{
		fire->frame.iScene++;
		fire->dwTime = GetTickCount();
	}

	if (fire->frame.iScene > 15)
		fire->frame.iScene = 10;

}


void CIce::Render(HDC hdc)
{
	if (m_iState == 1)
	{
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

	m_tempInfo.fX = ice->pf.x + WINCX / 2 - CStage::m_tCam.x;
	m_tempInfo.fY = ice->pf.y + WINCY / 2 - CStage::m_tCam.y;
	if (ice->msDraw == "")
		return;

	m_tempInfo.fCX = 384;
	m_tempInfo.fCY = 384;
	(*m_pMapBmp)["Ice"]->Draw(&hdc,
		m_tempInfo,
		m_tempInfo.fCX * (ice->frame.iScene % 5),
		m_tempInfo.fCY * (ice->frame.iScene / 5),
		ice->alpha,
		RGB(0, 0, 0));
}

E_SCENE_ID CIce::Progress()
{
	return E_SCENE_ID();
}

void CIce::Release()
{

}


CIce::CIce()
{
}


CIce::~CIce()
{
}
