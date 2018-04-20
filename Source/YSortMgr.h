#pragma once
#include "Include.h"
#include "Obj.h"

class CYSortMgr
{
	DECLARE_SINGLETON(CYSortMgr);
private:
	multimap<float, CObj*> m_MapObj;
public:
	void AddRenderObj(CObj* pObj);
	void Render(HDC hdc);
	void Clear(void);
private:
	CYSortMgr();
public:
	~CYSortMgr();
};

