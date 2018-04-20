#include "stdafx.h"
#include "SceneMgr.h"


CSceneMgr::CSceneMgr()
	:m_pState(NULL)
{

}

CSceneMgr::~CSceneMgr()
{
	this->Release();
}

void CSceneMgr::SetState(E_SCENE_ID _estate)
{
	SAFE_DELETE(m_pState);

	switch (_estate)
	{
	case E_SCENE_MENU:
		m_pState = new CMyMenu;
		break;

	case E_SCENE_EDIT:
		m_pState = new CStage;
		break;

	case E_SCENE_STAGE:
		m_pState = new CStage;
		break;
	}

	m_pState->Initialize();
}

void CSceneMgr::Progress()
{
	E_SCENE_ID Sceneid = E_SCENE_NONPASS;
	Sceneid = m_pState->Progress();

	if (Sceneid > E_SCENE_NONPASS)
	{
		this->SetState(Sceneid);
		return;
	}
}

void CSceneMgr::Render(HDC hdc)
{
	m_pState->Render(hdc);
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pState);
}

