#include "stdafx.h"
#include "MainGame.h"

CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	this->Release();
}

void CMainGame::Initialize()
{
	m_hdc = GetDC(g_hWnd);
	
	srand(unsigned(time(NULL)));

	GET_SINGLE(CSoundMgr)->Initialize();
	GET_SINGLE(CSceneMgr)->SetState(E_SCENE_MENU);
	GET_SINGLE(CKeyInputMgr)->InitInputDevice();
}

void CMainGame::Progress()
{
	GET_SINGLE(CKeyInputMgr)->SetKeyState();

	GET_SINGLE(CSceneMgr)->Progress();
}

void CMainGame::Render()
{
	GET_SINGLE(CSceneMgr)->Render(m_hdc);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hdc);
	DESTROY_SINGLE(CSceneMgr);
	DESTROY_SINGLE(CYSortMgr);
	DESTROY_SINGLE(CSoundMgr);
}

