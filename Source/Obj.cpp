#include "stdafx.h"
#include "Obj.h"

map<string, CBitMap*>*	CObj::m_pMapBmp = NULL;

CObj::CObj()
{
}


CObj::~CObj()
{
}

RECT CObj::GetRect()
{
	RECT rt =
	{
		int(m_Info.fX - m_Info.fCX / 2.f),
		int(m_Info.fY - m_Info.fCY / 2.f),
		int(m_Info.fX + m_Info.fCX / 2.f),
		int(m_Info.fY + m_Info.fCY / 2.f)
	};

	return rt;
}

void CObj::SetPos(float _fx, float _fy)
{
	m_Info.fX = _fx;
	m_Info.fY = _fy;
}

