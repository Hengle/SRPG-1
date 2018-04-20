#pragma once
#include "Include.h"
#include "Scene.h"
#include "Obj.h"
#include "ObjFactory.h"
#include "BackGround.h"
#include "MenuPanel.h"

class CMyMenu : public CScene
{
private:
	CObj* m_Menu;
	list<CObj*>	 m_ButtonList;
public:
	virtual void Initialize();
	virtual void Render(HDC hdc);
	virtual void Release();
public:
	//virtual void OnOpened();
	//virtual void OnClosed();
	virtual void Update();
public:
	CMyMenu();
	virtual ~CMyMenu();
};

