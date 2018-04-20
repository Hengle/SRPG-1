#pragma once
#include "Include.h"
#include "BitMap.h"
#include "MousePoint.h"
#include "SoundMgr.h"

class CObj
{
protected:
	INFO m_Info;
	FRAME m_Frame;
	string m_strDrawID;
	static POINTFLOAT m_tScroll;
	static map<string, CBitMap*>*	m_pMapBmp;
	DWORD m_dwTime;
	float m_fSpeed, m_fAngle;
public:
	RECT GetRect();
public:
	void SetPos(float _fx, float _fy);
public:
	INFO GetInfo(void) const { return m_Info; }
	static void SetBmp(map<string, CBitMap*>*	_pMapBmp) { m_pMapBmp = _pMapBmp; }
	string GetDrawID(void) { return m_strDrawID; }
	void SetDrawID(string _drawId) { m_strDrawID = _drawId; }
public:
	virtual void Initialize(void)PURE;
	virtual E_SCENE_ID Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CObj();
	virtual ~CObj();
};

