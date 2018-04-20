#pragma once
#include "Include.h"
#include "MyMenu.h"
#include "Stage.h"
#include "Scene.h"

class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr);
protected:
	CScene* m_pState;
public:
	void SetState(E_SCENE_ID _estate);
	void Progress();
	void Render(HDC hdc);
	void Release();
private:
	CSceneMgr();
public:
	~CSceneMgr();
};

