#include "stdafx.h"
#include "Executer.h"
#include "SoundMgr.h"
#include "KeyInputMgr.h"

CExecuter::CExecuter()
{
	enabled = true;
}


CExecuter::~CExecuter()
{

}

E_STAGE_STATE CExecuter::Update(CObj* pMarker_)
{
	m_pMarker = pMarker_;
	m_eStage = E_STAGE_NONPASS;
	switch (m_eExe)
	{
		case E_EXE_INIT:
			if (!enabled)
				return E_STAGE_DECIDE;
			m_eExe = E_EXE_SEARCH;

		case E_EXE_SEARCH:
			m_eStage = Search();
			m_eExe = E_EXE_SELECT;
			break;

		case E_EXE_SELECT:
			m_eStage = Select();
			return CancelSelect();
			break;

		case E_EXE_READY:
			m_eStage = Ready();
			break;

		case E_EXE_EXECUTE:
			m_eStage = Execute();
			enabled = false;
			break;

		case E_EXE_WAIT:
			m_eStage = Wait();
			break;
	}

	return m_eStage;
}

E_STAGE_STATE CExecuter::CancelSelect()
{
	if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(1) == EINPUT_DOWN)
	{
		GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_CANCEL, 0);
		GET_SINGLE(CPathfinder)->Clear();
		GET_SINGLE(CRangefinder)->Clear();
		m_eExe = E_EXE_INIT;
		return E_STAGE_DECIDE;
	}

	else return E_STAGE_NONPASS;
}