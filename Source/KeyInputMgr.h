#pragma once
#include "Include.h"

class CKeyInputMgr
{
	DECLARE_SINGLETON(CKeyInputMgr);
private:
	LPDIRECTINPUT8 m_pInput;
private:
	LPDIRECTINPUTDEVICE8 m_pKeyBoard;
	BYTE m_byKState[256];

	BYTE m_KeyCur[256];
	BYTE m_KeyOld[256];
	BYTE m_KeyMap[256];

	BYTE m_MouseCur[3];
	BYTE m_MouseOld[3];
	BYTE m_MouseMap[3];
private:
	LPDIRECTINPUTDEVICE8 m_pMouse;
	DIMOUSESTATE m_MState;
public:
	BYTE GetKeyBoardState(short nKey){ return m_KeyMap[nKey]; }
	BYTE GetMouseClickState(short nMouse) { return m_MouseMap[nMouse]; }
	const DIMOUSESTATE* GetMouseState() { return &m_MState; }
public:
	HRESULT InitInputDevice();
	void SetKeyState();
	void Release();

private:
	CKeyInputMgr();
public:
	~CKeyInputMgr();
};

