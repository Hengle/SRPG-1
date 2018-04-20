#include "stdafx.h"
#include "Scene.h"


CObj* CScene::m_pCurtain = CObjFactory<CCurtain>::CreateObj(WINCX / 2, WINCY / 2 - 1);

CScene::CScene()
{
	m_gState = E_SCENE_INIT;
	m_gTimer = GetTickCount();
	((CCurtain*)m_pCurtain)->SetAlpha(250);
}

CScene::~CScene()
{

}

E_SCENE_ID CScene::Progress()
{
	switch (m_gState)
	{
	case E_SCENE_INIT:
		if (m_gTimer + 500 < GetTickCount())
		{
			m_gState = E_SCENE_OPEN;
			GET_SINGLE(CSoundMgr)->SoundPlay(0, 1);
		}
		break;

	case E_SCENE_OPEN:
		if (((CCurtain*)m_pCurtain)->Open(4)) { m_gState = E_SCENE_ONOPENED; }
		break;

	case E_SCENE_ONOPENED:
		OnOpened();
		m_gState = E_SCENE_UPDATE;
		break;

	case E_SCENE_UPDATE:
		Update();

		break;

	case E_SCENE_END:
		OnClosed();
		m_gState = E_SCENE_CLOSE;
		break;

	case E_SCENE_CLOSE:
		if (((CCurtain*)m_pCurtain)->Close(4)) { m_gState = E_SCENE_ONCLOSED; }
		break;

	case E_SCENE_ONCLOSED:
		m_gState = E_SCENE_CHANGE;
		break;

	case E_SCENE_CHANGE:
		return m_iNextScene;
		break;
	}

	return E_SCENE_NONPASS;
}


void CScene::OnOpened()
{

}

void CScene::OnClosed()
{

}

void CScene::Update()
{

}
