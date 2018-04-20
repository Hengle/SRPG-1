#include "stdafx.h"
#include "BitMap.h"

CBitMap::CBitMap()
{

}

CBitMap::~CBitMap()
{
	this->Release();
}

CBitMap* CBitMap::LoadBmp(PTCHAR pFileName)
{
	m_hdc = GetDC(g_hWnd); // 화면디씨
	m_MemDC = CreateCompatibleDC(m_hdc); // 이미지 한장을 보관하기 위한디씨

	m_bitMap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (!m_bitMap)
	{
		MessageBox(g_hWnd, pFileName, L"File Load False", NULL);
	}

	// SelectObject함수의 결과값 : m_MemDC가 m_bitmap을 가지기 전에 있던 값을 리턴한다.
	m_oldbitMap = (HBITMAP)SelectObject(m_MemDC, m_bitMap);

	ReleaseDC(g_hWnd, m_hdc);
	return this;
}

void CBitMap::Draw(HDC *hdc, INFO m_Info, long copyX, long copyY, byte alpha, UINT transparent)
{
	if (alpha <= 0)
		return;

	else if (alpha >= 255)
	{
		alpha = 255;
		TransparentBlt(*hdc,
			int(m_Info.fX - m_Info.fCX / 2),
			int(m_Info.fY - m_Info.fCY / 2),
			int(m_Info.fCX), int(m_Info.fCY),
			m_MemDC,
			copyX, copyY,
			int(m_Info.fCX), int(m_Info.fCY),
			transparent);
	}

	else
	{
		if (!m_alphaDC)
		{
			m_alphaDC = CreateCompatibleDC(*hdc);
			m_alphaBitmap = (HBITMAP)CreateCompatibleBitmap(*hdc, m_Info.fCX, m_Info.fCY);
			m_alphaOld = (HBITMAP)SelectObject(m_alphaDC, m_alphaBitmap);

			m_blend.BlendOp = AC_SRC_OVER;
			m_blend.BlendFlags = 0;
			m_blend.AlphaFormat = 0;
		}

		TransparentBlt(m_alphaDC,
			0, 0,
			int(m_Info.fCX), int(m_Info.fCY),
			*hdc,
			int(m_Info.fX - m_Info.fCX / 2),
			int(m_Info.fY - m_Info.fCY / 2),
			int(m_Info.fCX), int(m_Info.fCY),
			SRCCOPY);

		TransparentBlt(m_alphaDC,
			0, 0,
			int(m_Info.fCX), int(m_Info.fCY),
			m_MemDC,
			copyX, copyY,
			int(m_Info.fCX), int(m_Info.fCY),
			transparent);

		m_blend.SourceConstantAlpha = alpha;

		AlphaBlend(*hdc,
			int(m_Info.fX - m_Info.fCX / 2),
			int(m_Info.fY - m_Info.fCY / 2),
			int(m_Info.fCX), int(m_Info.fCY),
			m_alphaDC,
			0, 0,
			int(m_Info.fCX), int(m_Info.fCY),
			m_blend);
	}

}

void CBitMap::Release()
{
	SelectObject(m_alphaDC, m_alphaOld);
	DeleteObject(m_alphaBitmap);
	DeleteObject(m_alphaOld);
	DeleteDC(m_alphaDC);

	SelectObject(m_MemDC, m_oldbitMap);
	DeleteObject(m_bitMap);
	DeleteObject(m_oldbitMap);
	DeleteDC(m_MemDC);
}