#include "stdafx.h"
#include "Stage.h"
#include "Math.h"
#include "Fire.h"



CFire::CFire()
{
}


CFire::~CFire()
{
}


void CFire::Initialize()
{
	m_iRange = 4;
	m_iSphere = 2;
	m_iMultiplier = 1;
	m_pathType = E_PATH_ATTACK;
}

E_EXE_STATE CFire::Update(void)
{
	switch (m_iState)
	{
	case 0:
		m_Frame = FRAME(0, 24, 0, 30);
		m_dwTime = GetTickCount();
		m_iCount = m_pTargets.size();
		m_iAngle = 360 / m_pTargets.size();
		m_iState = 1;

		for (int i = 0; i < m_iCount; i++)
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
		if (m_dwTime + 200 < GetTickCount())
		{
			m_iState = 3;
			m_dwTime = GetTickCount();

			m_iCount--;
			GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_FIRECAST, 0);
			m_Frame.iScene = m_Frame.iStart;

			m_tFires[m_iCount]->bFiring = true;
			m_tFires[m_iCount]->iState = 0;

			((CMarker*)m_pMarker)->SetFrame(16, 16);

			m_Frame = FRAME(0, 63, 0, 5);
		}
		break;

	case 3:
		if (m_dwTime + m_Frame.dwTime < GetTickCount())
		{
			m_Frame.iScene++;
			m_dwTime = GetTickCount();

			if (m_Frame.iScene > m_Frame.iLast)
			{
				m_iCount--;

				if (m_iCount < 0)
				{
					m_iState = 4;
					return E_EXE_READY;
				}

				GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_FIRECAST, 0);
				m_Frame.iScene = m_Frame.iStart;

				m_tFires[m_iCount]->bFiring = true;
				m_tFires[m_iCount]->iState = 0;
			}
		}
		break;

	case 4:
		for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
		{
			if ((*fIt)->iState != 2)
				continue;
			m_iState = 5;
		}
		break;

	case 5:
		for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
		{
			(*fIt)->iState = 3;
			GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_FIREWIND, 0);

			m_iState = 6;
		}

		break;

	case 6:
		for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
		{
			if ((*fIt)->iState != 6)
				continue;

			m_iState = 7;
			m_dwTime = GetTickCount();
		}
		break;

	case 7:

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


void CFire::DrawFire()
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
				(*fIt)->frame = FRAME(0, 31, 0, 50);
				(*fIt)->pf.x = m_pMarker->GetInfo().fX;
				(*fIt)->pf.y = m_pMarker->GetInfo().fY;
				(*fIt)->alpha = 0;
				(*fIt)->dist = 120;
				(*fIt)->msDraw = "Fireball";

				(*fIt)->iState = 1;
				break;

			case 1:
				//pos
				(*fIt)->pf.x = m_pMarker->GetInfo().fX
					+ cos(((m_iAngle * (*fIt)->iIndex) - 90) * PI / 180) * (120 / (*fIt)->dist);
				(*fIt)->pf.y = m_pMarker->GetInfo().fY
					+ sin(((m_iAngle * (*fIt)->iIndex) - 90)  * PI / 180) * (120 / (*fIt)->dist);

				(*fIt)->dist -= 3;

				if ((*fIt)->dist < 1)
				{
					(*fIt)->dist = 1;
				}

				//alpha
				(*fIt)->alpha += 5;
				FrameFire(*fIt);

				if ((*fIt)->alpha > 255)
				{
					(*fIt)->iState = 2;
					(*fIt)->alpha = 255;
					
				}
				break;

			case 2:
				FrameFire(*fIt);
				(*fIt)->iTime = 0;
				(*fIt)->pfZoom.x = (*fIt)->pf.x;
				(*fIt)->pfZoom.y = (*fIt)->pf.y;
				break;

			case 3:
				time_ = 20;
				FrameFire(*fIt);

				CStage::ZoomCam(
					(*m_pTargets.begin())->GetInfo().fX,
					(*m_pTargets.begin())->GetInfo().fY,
					19
				);

				
				
				(*fIt)->pf.x = CMath::EaseIn(
					(*fIt)->iTime,
					(*fIt)->pfZoom.x, 
					m_pTargets[(*fIt)->iIndex]->GetInfo().fX - (*fIt)->pfZoom.x, 
					time_
					);

				(*fIt)->pf.y = CMath::EaseIn(
					(*fIt)->iTime,
					(*fIt)->pfZoom.y, 
					m_pTargets[(*fIt)->iIndex]->GetInfo().fY - (*fIt)->pfZoom.y,
					time_
					);

				(*fIt)->iTime++;

				if ((*fIt)->iTime > 20)
				{
					CStage::m_bZooming = false;
					(*fIt)->iState = 4;
				}
				break;

			case 4:
				(*fIt)->dwTime = GetTickCount();
				(*fIt)->frame = FRAME(0, 19, 0, 50);
				(*fIt)->msDraw = "Fire";
				(*fIt)->iState = 5;
				GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_FIREBOOM, 0);

				for (it = m_pTargets.begin(); it != m_pTargets.end(); ++it)
				{
					((CMarker*)(*it))->SetFrame(20, 20);
				}

				break;

			case 5:
				if (FrameFire(*fIt))
				{
					(*fIt)->frame.iScene = (*fIt)->frame.iLast;
					(*fIt)->iState = 6;
				}

				break;
			}
		}
	}
}

bool CFire::FrameFire(PFIRE fire)
{
	if (fire->dwTime + fire->frame.dwTime > GetTickCount())
	{
		fire->frame.iScene++;
		fire->dwTime = GetTickCount();
	}

	if (fire->frame.iScene > fire->frame.iLast)
	{
		fire->frame.iScene = fire->frame.iStart;
		return true;
	}

	return false;
}

void CFire::Render(HDC hdc)
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

	
	if (m_iState == 3)
	{
		//Cast
		m_tempInfo.fX = m_pMarker->GetInfo().fX + WINCX / 2 - CStage::m_tCam.x;
		m_tempInfo.fY = m_pMarker->GetInfo().fY + WINCY / 2 - CStage::m_tCam.y;
		m_tempInfo.fCX = 192;
		m_tempInfo.fCY = 192;

		(*m_pMapBmp)["Firecast"]->Draw(&hdc,
			m_tempInfo,
			192 * (m_Frame.iScene % 5),
			192 * (m_Frame.iScene / 5),
			120, RGB(0, 0, 0));
	}

	m_tempInfo.fCX = 128;
	m_tempInfo.fCY = 128;

	//Fires
	for (fIt = m_tFires.begin(); fIt != m_tFires.end(); fIt++)
	{
		if ((*fIt)->msDraw == "")
			continue;

		m_tempInfo.fX = (*fIt)->pf.x + WINCX / 2 - CStage::m_tCam.x;
		m_tempInfo.fY = (*fIt)->pf.y + WINCY / 2 - CStage::m_tCam.y;

		if ((*fIt)->msDraw == "Fire")
		{
			m_tempInfo.fCX = 192;
			m_tempInfo.fCY = 192;
			(*m_pMapBmp)[(*fIt)->msDraw]->Draw(&hdc,
				m_tempInfo,
				192 * ((*fIt)->frame.iScene % 5),
				192 * ((*fIt)->frame.iScene / 5),
				(*fIt)->alpha,
				RGB(0, 0, 0));

			continue;
		}

		(*m_pMapBmp)[(*fIt)->msDraw]->Draw(&hdc,
			m_tempInfo,
			128 * ((*fIt)->frame.iScene % 8),
			128 * ((*fIt)->frame.iScene / 8),
			(*fIt)->alpha, 
			RGB(0, 0, 0));
	}
}

E_SCENE_ID CFire::Progress()
{
	return E_SCENE_ID();
}

void CFire::Release()
{

}
