#include "stdafx.h"
#include "MyMenu.h"

CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	this->Release();
}

void CMyMenu::Initialize()
{
	//Load Assets
	m_MapBmp["BackBuffer"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");
	m_MapBmp["BackGround"] = (new CBitMap)->LoadBmp(L"../Texture/Stage/Back/BackGround.bmp");
	m_MapBmp["Night"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Back/Night.bmp");
	m_MapBmp["Menu"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Back/Menu.bmp");
	m_MapBmp["Arrow"] = (new CBitMap)->LoadBmp(L"../Texture/Menu/Back/Arrow.bmp");
	m_MapBmp["Curtain"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");

	//BG
	m_pBackGround = new CBackGround;
	m_pBackGround->SetDrawID("Night");

	//Menu
	m_Menu = CObjFactory<CMenuPanel>::CreateObj(512, 560);
	
	//BGM
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Title.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Tick.wav");
	GET_SINGLE(CSoundMgr)->LoadWave(L"../sound/Start.wav");

	//?
	CObj::SetBmp(&m_MapBmp);
}

void CMyMenu::Update()
{
	m_iNextScene = E_SCENE_NONPASS;

	m_pBackGround->Progress();
	m_iNextScene = m_Menu->Progress();

	if (m_iNextScene > E_SCENE_NONPASS)
		m_gState = E_SCENE_DISABLE;
}

void CMyMenu::Render(HDC hdc)
{
	m_pBackGround->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	m_Menu->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	//Curtain
	CScene::m_pCurtain->Render(m_MapBmp["BackBuffer"]->GetMemDC());

	//º¤¹öÆÛ
	BitBlt(hdc, 0, 0, WINCX, WINCY,
		m_MapBmp["BackBuffer"]->GetMemDC(),
		0, 0, SRCCOPY);
}

void CMyMenu::Release()
{
	for (map<string, CBitMap*>::iterator iter = m_MapBmp.begin();
		iter != m_MapBmp.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_MapBmp.clear();
	SAFE_DELETE(m_pBackGround);

	GET_SINGLE(CSoundMgr)->SoundAllStop();
	GET_SINGLE(CSoundMgr)->SoundReset();
}

