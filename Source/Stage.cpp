#include "stdafx.h"
#include "Stage.h"
#include "Tile.h"
#include "Slash.h"
#include "Thunder.h"
#include "Math.h"
#include "AIFinder.h"

POINTFLOAT CStage::m_tCam = { 0, };
int CStage::m_iZoomTime = 0;
float CStage::m_zoomX = 0;
float CStage::m_zoomY = 0;
bool CStage::m_bZooming = false;

CStage::CStage()
{
}


CStage::~CStage()
{
	this->Release();
}

void CStage::Initialize()
{
	//Load Assets
	m_MapBmp["BackBuffer"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");
	m_MapBmp["BackGround"] = (new CBitMap)->LoadBmp(L"../Texture/Stage/Back/BackGround.bmp");
	m_MapBmp["Victory"] = (new CBitMap)->LoadBmp(L"../Texture/Stage/Back/Victory.bmp");
	m_MapBmp["Gameover"] = (new CBitMap)->LoadBmp(L"../Texture/Stage/Back/Gameover.bmp");
	m_MapBmp["PlayGround"] = (new CBitMap)->LoadBmp(L"../Texture/PlayGround.bmp");
	m_MapBmp["Night"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Back/Night.bmp");
	m_MapBmp["Curtain"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");
	m_MapBmp["Tile"] = (new CBitMap)->LoadBmp(L"../Texture/Tile/Tile.bmp");
	m_MapBmp["Rend"] = (new CBitMap)->LoadBmp(L"../Texture/Tile/Rend.bmp");
	m_MapBmp["Highlight"] = (new CBitMap)->LoadBmp(L"../Texture/Tile/Highlight.bmp");
	m_MapBmp["Slash"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Slash.bmp");
	m_MapBmp["Swordsman"] = (new CBitMap)->LoadBmp(L"../Texture/Character/Swordsman.bmp");
	m_MapBmp["Face"] = (new CBitMap)->LoadBmp(L"../Texture/Character/Face.bmp");
	m_MapBmp["Damage"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Damage.bmp");
	m_MapBmp["Magic"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Magic.bmp");
	m_MapBmp["Healing"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Healing.bmp");
	m_MapBmp["Curing"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Curing.bmp");
	m_MapBmp["Fire"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Fire.bmp");
	m_MapBmp["Fireball"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Fireball.bmp");
	m_MapBmp["Firecast"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Firecast.bmp");
	m_MapBmp["Thunder"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Thunder.bmp");
	m_MapBmp["ThunderShock"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/ThunderShock.bmp");
	m_MapBmp["Ice"] = (new CBitMap)->LoadBmp(L"../Texture/Skill/Ice.bmp");
	m_MapBmp["Wheel"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Wheel.bmp");
	m_MapBmp["Portrait"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Portrait.bmp");
	m_MapBmp["Queue"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Queue.bmp");
	m_MapBmp["Marker"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Marker.bmp");
	m_MapBmp["Bars"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Bars.bmp");
	m_MapBmp["MiniBar"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/MiniBar.bmp");
	m_MapBmp["Info"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Info.bmp");

	//BG
	m_pBackGround = new CBackGround;
	m_pBackGround->SetDrawID("Night");
	
	//Tile
	m_pTile = CObjFactory<CTile>::CreateObj();
	((CTile*)m_pTile)->LoadData();

	m_tCam.y = 1000;

	//Marker
	CreateMarker(0, E_TEAM_PLAYER, 425, 5, 15, 4);
	CreateMarker(1, E_TEAM_PLAYER, 423, 5, 13, 1);
	CreateMarker(2, E_TEAM_PLAYER, 452, 5, 18, 7);

	CreateMarker(3, E_TEAM_COM1, 302, 4, 14, 9);
	CreateMarker(4, E_TEAM_COM1, 328, 4, 15, 8);
	CreateMarker(5, E_TEAM_COM1, 358, 4, 16, 8);

	CreateMarker(6, E_TEAM_COM1, 461, 4, 13, 9);
	CreateMarker(7, E_TEAM_COM1, 543, 4, 14, 8);
	CreateMarker(8, E_TEAM_COM1, 548, 4, 15, 9);

	CreateMarker(9, E_TEAM_COM1, 344, 4, 15, 8);
	CreateMarker(10, E_TEAM_COM1, 205, 4, 15, 9);


	m_victoryAlpha = 245;

	//Skills
	m_slash = new CSlash;
	m_slash->Initialize();
	//
	m_heal = new CHeal;
	m_heal->Initialize();
	//
	m_thunder = new CThunder;
	m_thunder->Initialize();
	//
	m_fire = new CFire;
	m_fire->Initialize();
	//
	m_ice = new CIce;
	m_ice->Initialize();

	//WheelMenu
	m_pWheelMenu = new CWheel();
	m_pWheelMenu->Initialize();
	m_pWheelMenu->SetSkillAt(2, m_slash);
	m_pWheelMenu->SetSkillAt(3, m_heal);
	
	//Executers
	m_pMover = new CMover();
	m_pMover->SetTile(m_pTile);
	//
	m_pAttacker = new CAttacker();
	m_pAttacker->SetTile(m_pTile);
	m_pAttacker->SetMarkers(&m_pMarkers);
	//
	m_pAI = new CAIExecuter();
	m_pAI->SetTile(m_pTile);
	m_pAI->SetMarkers(&m_pMarkers);


	CAIExecuter::m_pSkill = m_thunder;

	//BGM
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Stage.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Victory.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Gameover.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Tick.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Tick2.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Turn.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Boo.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Damage.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Sword.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Magic.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Magic2.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Magic3.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Healing.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Firecast.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Firewind.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Fireboom.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Thunder.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Ice.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/IceCreate.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/IceBreak.wav");

	//MISC
	GET_SINGLE(CPathfinder)->SetTile(m_pTile);
	GET_SINGLE(CRangefinder)->SetTile(m_pTile);
	GET_SINGLE(CAIFinder)->SetTile(m_pTile);
	m_eStage = E_STAGE_WAIT;

	CObj::SetBmp(&m_MapBmp);
}

CObj* CStage::CreateMarker(int index_, E_TEAM_ID team_, int curr_, int agi_, int spd_, int class_)
{
	CObj* marker_ = CObjFactory<CMarker>::CreateObj();
	((CMarker*)marker_)->Init(index_, team_);
	((CMarker*)marker_)->SetTile(m_pTile);
	((CMarker*)marker_)->SetCurr(curr_);
	((CMarker*)marker_)->SetAbility(spd_, agi_);
	((CMarker*)marker_)->SetDrawID("Swordsman");
	((CMarker*)marker_)->SetClass(class_);

	if (team_ == E_TEAM_PLAYER)
	{
		m_pAllies.push_back((CMarker*)marker_);
	}
		
	else
	{
		m_pEnemies.push_back((CMarker*)marker_);
	}

	m_pMarkers.push_back(marker_);

	PTILE tile = ((CTile*)m_pTile)->GetTile(curr_);
	tile->iMarker = index_;
	tile->eTeam = team_;
	tile->bAlive = true;

	return marker_;
}

void CStage::Update()
{
	m_pTile->Progress();
	UpdateMarkers();
}

void CStage::FocusCam()
{
	
	if (CStage::m_tCam.x < WINCX / 2)	// 왼쪽으로 벗어나려고 한다면
		CStage::m_tCam.x = WINCX / 2;

	else if (CStage::m_tCam.x >(TILECX * STAGEX) - WINCX / 2)	// 오른쪽으로 벗어나려 한다면
		CStage::m_tCam.x = (TILECX * STAGEX) - WINCX / 2;

	if (CStage::m_tCam.y < WINCY / 2)	// 위쪽으로 벗어나려 한다면
		CStage::m_tCam.y = WINCY / 2;

	else if (CStage::m_tCam.y >(TILECY * STAGEY) - WINCY / 2)
		CStage::m_tCam.y = (TILECY * STAGEY) - WINCY / 2;

}

bool CStage::ZoomCam(float fx_, float fy_ , int time_)
{
	if (!m_bZooming)
	{
		m_zoomX = m_tCam.x;
		m_zoomY = m_tCam.y;
	}

	m_bZooming = true;
	m_tCam.x = CMath::EaseOut(m_iZoomTime, m_zoomX, fx_ - m_zoomX, time_);
	m_tCam.y = CMath::EaseOut(m_iZoomTime, m_zoomY, fy_ - m_zoomY, time_);
	m_iZoomTime++;

	if (m_iZoomTime > time_)
	{
		m_iZoomTime = 0;
		m_bZooming = false;
		return true;
	}

	else return false;
}

void CStage::UpdateMarkers()
{
	PTILE tile = ((CTile*)m_pTile)->GetTargeted();

	if (tile->iMarker > -1)
	{
		m_pTarget = (CMarker*)m_pMarkers[tile->iMarker];
	}

	else m_pTarget = NULL;

	vector<CObj*>::iterator it;
	switch (m_eStage)
	{
		case E_STAGE_WAIT:
			for (it = m_pMarkers.begin(); it != m_pMarkers.end(); ++it)
			{
				if (!(((CMarker*)(*it))->CheckAlive()))
					continue;

				m_eStage = ((CMarker*)(*it))->UpdateWait();
				if (m_eStage == E_STAGE_FOCUS)
				{
					GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_SELECT2, 0);
					m_pCurr = *it;

					int iClass = ((CMarker*)m_pCurr)->GetClass();
					CheckClass(iClass);
					break;
				}
			}
		break;

		case E_STAGE_FOCUS:
			if (ZoomCam(m_pCurr->GetInfo().fX, m_pCurr->GetInfo().fY, 20))
			{
				m_eStage = E_STAGE_DECIDE;
			}
				
		break;

		case E_STAGE_DECIDE:
			CheckVictory();

			if (m_eStage == E_STAGE_VICTORY)
				return;

			if (((CMarker*)m_pCurr)->GetTeam() == E_TEAM_PLAYER)
				m_eStage = m_pWheelMenu->Update((CMarker*)m_pCurr);
				
			else
			{
				m_eStage = m_pAI->Update((CMarker*)m_pCurr);
				if (m_eStage == E_STAGE_NONPASS)
					m_eStage = E_STAGE_DECIDE;
			}
		break;

		case E_STAGE_MOVE:
			m_eStage = m_pMover->Update(m_pCurr);
			if (m_eStage == E_STAGE_NONPASS)
				m_eStage = E_STAGE_MOVE;
		break;

		case E_STAGE_ATTACK:
			m_eStage = m_pAttacker->Update(m_pCurr);
			if (m_eStage == E_STAGE_NONPASS)
				m_eStage = E_STAGE_ATTACK;
		break;

		case E_STAGE_ENDTURN:
			((CMarker*)m_pCurr)->EndTurn();
			m_pMover->Enable(true);
			m_pAttacker->Enable(true);
			m_pAI->Enable(true);
			m_eStage = E_STAGE_WAIT;

			CheckVictory();
			CheckGameover();
		break;

		case E_STAGE_VICTORY:

			break;

		case E_STAGE_GAMEOVER:

			break;
	}
}

void CStage::CheckVictory()
{
	vector<CMarker*>::iterator it;
	for (it = m_pEnemies.begin(); it != m_pEnemies.end(); it++)
	{
		if ((*it)->CheckAlive())
			return;
	}

	GET_SINGLE(CSoundMgr)->SoundAllStop();
	GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_VICTORY, 1);
	m_eStage = E_STAGE_VICTORY;
}

void CStage::CheckGameover()
{
	vector<CMarker*>::iterator it;
	for (it = m_pAllies.begin(); it != m_pAllies.end(); it++)
	{
		if ((*it)->CheckAlive())
			return;
	}

	GET_SINGLE(CSoundMgr)->SoundAllStop();
	GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_GAMEOVER, 1);
	m_eStage = E_STAGE_GAMEOVER;
}

void CStage::CheckClass(int class_)
{
	switch (class_)
	{
	case 0:
		break;

	case 1:
		m_pWheelMenu->SetSkillAt(0, m_slash);
		m_pWheelMenu->SetSkillAt(3, m_heal);
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		m_pWheelMenu->SetSkillAt(0, m_slash);
		m_pWheelMenu->SetSkillAt(3, m_thunder);
		break;

	case 5:

		break;

	case 6:
		break;

	case 7:
		m_pWheelMenu->SetSkillAt(0, m_slash);
		m_pWheelMenu->SetSkillAt(3, m_fire);
		break;

	case 8:
		CAIExecuter::m_pSkill = m_slash;
		break;

	case 9:
		CAIExecuter::m_pSkill = m_ice;
		break;
	}

}

void CStage::Render(HDC hdc)
{
	//Focus Cam
	FocusCam();

	//BG
	m_pBackGround->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	BitBlt(m_MapBmp["BackBuffer"]->GetMemDC(), 0, 0, WINCX, WINCY,
		m_MapBmp["PlayGround"]->GetMemDC(),
		m_tCam.x - WINCX / 2, m_tCam.y - WINCY / 2, SRCCOPY);

	//Tiles
	m_pTile->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	BitBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
		80, 80,
		864, 20,
		m_MapBmp["Queue"]->GetMemDC(),
		0, 0,
		SRCCOPY);

	//Markers
	vector<CObj*>::iterator it;
	for (it = m_pMarkers.begin(); it != m_pMarkers.end(); ++it)
	{
		(*it)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	}

	float x_ = CMousePoint::GetPos().x - 100;
	float y_ = CMousePoint::GetPos().y - 100;


	if (m_pTarget &&
		(((CAttacker*)m_pAttacker)->GetState() == E_EXE_SELECT ||
			((CAttacker*)m_pMover)->GetState() == E_EXE_SELECT))
	{
		BitBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			x_,
			y_,
			200, 100,
			m_MapBmp["Info"]->GetMemDC(),
			0, 0, SRCCOPY);

		TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			x_ + 112,
			y_ + 28,
			int(76.f * m_pTarget->GetHPPercent()), 15,
			m_MapBmp["Bars"]->GetMemDC(),
			0, 0,
			76, 15,
			SRCCOPY);

		TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			x_ + 2,
			y_ + 2,
			96, 96,
			m_MapBmp["Face"]->GetMemDC(),
			96 * (m_pTarget->GetClass() % 4),
			96 * (m_pTarget->GetClass() / 4),
			96, 96,
			RGB(0, 0, 0));

		rect = {
			int(x_) + 146,
			int(y_) + 27,
			int(x_) + 180,
			int(y_) + 100,
		};
	}

	//Menu
	m_pWheelMenu->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	//Skill
	((CAttacker*)m_pAttacker)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	((CAIExecuter*)m_pAI)->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
		30, 30,
		100, 100,
		m_MapBmp["Portrait"]->GetMemDC(),
		0, 0,
		100, 100,
		RGB(0, 0, 0));
	//

	if (m_eStage == E_STAGE_VICTORY)
	{
		TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			0, 0,
			WINCX, WINCY,
			m_MapBmp["Victory"]->GetMemDC(),
			0, 0,
			WINCX, WINCY,
			RGB(255, 255, 255));
	}
	
	else if (m_eStage == E_STAGE_GAMEOVER)
	{
		TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			0, 0,
			WINCX, WINCY,
			m_MapBmp["Gameover"]->GetMemDC(),
			0, 0,
			WINCX, WINCY,
			RGB(3, 0, 0));
	}

	if (m_eStage > E_STAGE_WAIT)
	{
		int class_ = ((CMarker*)m_pCurr)->GetClass();
		TransparentBlt(m_MapBmp["BackBuffer"]->GetMemDC(),
			32, 32,
			96, 96,
			m_MapBmp["Face"]->GetMemDC(),
			96 * (class_ % 4),
			96 * (class_ / 4),
			96, 96, 
			RGB(0, 0, 0));
	}

	//Curtain
	CScene::m_pCurtain->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	BitBlt(hdc, 0, 0, WINCX, WINCY,
		m_MapBmp["BackBuffer"]->GetMemDC(),
		0, 0, SRCCOPY);
}

void CStage::Release()
{

}

E_SCENE_ID CStage::Progress()
{
	return E_SCENE_NONPASS;
}


