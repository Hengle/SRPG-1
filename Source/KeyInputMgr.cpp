#include "stdafx.h"
#include "KeyInputMgr.h"

CKeyInputMgr::CKeyInputMgr()
{
	memset(m_KeyCur, 0, sizeof(m_KeyCur));
	memset(m_KeyOld, 0, sizeof(m_KeyOld));
	memset(m_KeyMap, 0, sizeof(m_KeyMap));
}

CKeyInputMgr::~CKeyInputMgr()
{
	this->Release();
}


HRESULT CKeyInputMgr::InitInputDevice()
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8
		, (void**)&m_pInput, NULL)))
	{
		//MSGBOX(L"Input Device Failed");
		return E_FAIL;
	}

	m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	m_pKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();

	m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();

	return S_OK;
}

void CKeyInputMgr::SetKeyState()
{
	m_pKeyBoard->GetDeviceState(sizeof(m_byKState), m_byKState);
	m_pMouse->GetDeviceState(sizeof(m_MState), &m_MState);

	memcpy(m_KeyOld, m_KeyCur, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));
	memset(m_KeyMap, 0, sizeof(m_KeyMap));

	memcpy(m_MouseOld, m_MouseCur, sizeof(m_MouseOld));
	memset(m_MouseCur, 0, sizeof(m_MouseCur));
	memset(m_MouseMap, 0, sizeof(m_MouseMap));

	// 여기서 비교?
	for (int i = 0; i < 256; ++i)
	{
		BYTE vKey = m_byKState[i] & 0x80;
		m_KeyCur[i] = (vKey) ? 1 : 0;

		int nOld = m_KeyOld[i];
		int nCur = m_KeyCur[i];

		if (nOld == 0 && nCur == 1)
			m_KeyMap[i] = EINPUT_DOWN;
		else if (nOld == 1 && nCur == 0)
			m_KeyMap[i] = EINPUT_UP;
		else if (nOld == 1 && nCur == 1)
			m_KeyMap[i] = EINPUT_PRESS;
		else
			m_KeyMap[i] = EINPUT_NONE;
	}

	for (int i = 0; i < 3; ++i)
	{
		BYTE vClick = m_MState.rgbButtons[i] & 0x80;
		m_MouseCur[i] = (vClick) ? 1 : 0;

		int nOld = m_MouseOld[i];
		int nCur = m_MouseCur[i];

		if (nOld == 0 && nCur == 1)
			m_MouseMap[i] = EINPUT_DOWN;
		else if (nOld == 1 && nCur == 0)
			m_MouseMap[i] = EINPUT_UP;
		else if (nOld == 1 && nCur == 1)
			m_MouseMap[i] = EINPUT_PRESS;
		else
			m_MouseMap[i] = EINPUT_NONE;
	}

	return;
}



void CKeyInputMgr::Release()
{
	
	if (m_pKeyBoard)
		m_pKeyBoard->Release();
	if (m_pMouse)
		m_pMouse->Release();
	if (m_pInput)
		m_pInput->Release();
		
}

