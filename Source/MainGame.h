#pragma once
#include "Include.h"
#include "SceneMgr.h"
#include "YSortMgr.h"
#include "SoundMgr.h"
#include "KeyInputMgr.h"

class CMainGame
{
private:
	HDC m_hdc;
public:
	void Initialize();
	void Progress();
	void Render();
	void Release();
public:
	CMainGame();
	~CMainGame();
};

