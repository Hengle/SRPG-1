#include "stdafx.h"
#include "BackGround.h"

CBackGround::CBackGround()
{
}


CBackGround::~CBackGround()
{
}

void CBackGround::Initialize(void)
{

}

E_SCENE_ID CBackGround::Progress(void)
{

	return E_SCENE_NONPASS;
}

void CBackGround::Render(HDC hdc)
{
	BitBlt(hdc,
		0, 0, WINCX, WINCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		0, 0, SRCCOPY);
}

void CBackGround::Release(void)
{

}

