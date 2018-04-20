#pragma once
#include "Include.h"
#include "BitMap.h"
#include "SoundMgr.h"
#include "Curtain.h"
#include "ObjFactory.h"

class CScene
{
protected:
	DWORD m_gTimer;
	E_SCENE_STATE m_gState;
	E_SCENE_ID m_iNextScene;
	CObj* m_pBackGround;
	map<string, CBitMap*> m_MapBmp;
public:
	static CObj*	m_pCurtain;
public:
	virtual void Initialize(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	virtual void OnOpened();
	virtual void OnClosed();
	virtual void Update();
public:
	E_SCENE_ID Progress();
public:
	CScene();
	virtual ~CScene();
};

