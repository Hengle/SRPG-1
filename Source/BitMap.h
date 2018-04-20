#pragma once
#include "Include.h"

class CBitMap
{
public:
	HDC			m_hdc, m_MemDC, m_alphaDC;
	HBITMAP		m_bitMap, m_oldbitMap, m_alphaBitmap, m_alphaOld;
	BLENDFUNCTION m_blend;
public:
	CBitMap* LoadBmp(PTCHAR pFileName);
	void Draw(HDC *hdc, INFO m_Info, long copyX, long copyY, byte alpha, UINT transparent);
	void Release();
public:
	HDC GetMemDC() { return m_MemDC; }
public:
	CBitMap();
	~CBitMap();
};

