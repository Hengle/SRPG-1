#include "stdafx.h"
#include "YSortMgr.h"

CYSortMgr::CYSortMgr()
{
}


CYSortMgr::~CYSortMgr()
{
}

void CYSortMgr::AddRenderObj(CObj* pObj)
{
	m_MapObj.insert(make_pair(pObj->GetInfo().fY, pObj));
}

void CYSortMgr::Render(HDC hdc)
{
	for (multimap<float, CObj*>::iterator iter = m_MapObj.begin();
		iter != m_MapObj.end(); ++iter)
	{
		iter->second->Render(hdc);
	}
}

void CYSortMgr::Clear(void)
{
	m_MapObj.clear();
}

