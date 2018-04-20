#include "stdafx.h"
#include "Mover.h"
#include "Tile.h"
#include "Marker.h"



E_STAGE_STATE CMover::Search()
{
	m_iWalkTimer = 8;
	m_iCurr = ((CMarker*)m_pMarker)->GetCurrent();

	GET_SINGLE(CPathfinder)->Pathfind(m_iCurr,
		((CMarker*)m_pMarker)->GetAgility(),
		((CMarker*)m_pMarker)->GetTeam(), 
		E_PATH_MOVE);
		
	m_pTarget = ((CTile*)m_pTile)->GetTargeted();
	if (m_pTarget &&
		m_pTarget->iType == E_TILE_MOVE)
		m_pTarget->iSelect = 1;

	return E_STAGE_NONPASS;
}

E_STAGE_STATE CMover::Select()
{
	if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(0) == EINPUT_DOWN)
	{
		m_pTarget = ((CTile*)m_pTile)->GetTargeted();

		if (!m_pTarget)
			return E_STAGE_NONPASS;

		m_iDest = m_pTarget->iIndex;
		if (m_pTarget->iType == E_TILE_MOVE)
		{
			GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_SELECT, 0);
			m_tPath = GET_SINGLE(CPathfinder)->ForgePath(m_iDest);
			m_tPath->reverse();
			GET_SINGLE(CPathfinder)->Clear();
			m_eExe = E_EXE_READY;
		}
	}

	else ((CTile*)m_pTile)->HighlightMove();
}

E_STAGE_STATE CMover::Ready()
{
	m_pTarget->iSelect = 1;
	m_pMarker->SetPos(m_pTarget->fX, m_pTarget->fY);

	if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(0) == EINPUT_DOWN)
	{
		((CTile*)m_pTile)->ChangeTile(m_iCurr, -1, E_TEAM_NULL);
		((CTile*)m_pTile)->GetTile(m_iCurr)->bAlive = false;
		m_eExe = E_EXE_EXECUTE;
	}

	else if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(1) == EINPUT_DOWN)
	{
		GET_SINGLE(CSoundMgr)->SoundPlay(E_STAGE_SUD_CANCEL, 0);
		m_tPath->clear();
		m_eExe = E_EXE_SEARCH;
	}

	else return E_STAGE_NONPASS;

	PTILE curr = ((CTile*)m_pTile)->GetTile(m_iCurr);
	m_pMarker->SetPos(curr->fX, curr->fY);
	m_pTarget->iSelect = 0;

	return E_STAGE_NONPASS;
}


E_STAGE_STATE CMover::Execute()
{
	m_iNext = *m_tPath->begin();
	PTILE tile = ((CTile*)m_pTile)->GetTile(m_iCurr);


	if (tile->nears.left == m_iNext)
		((CMarker*)m_pMarker)->Move(-TILECX / 8, 0);
	else if (tile->nears.right == m_iNext)
		((CMarker*)m_pMarker)->Move(TILECX / 8, 0);
	else if (tile->nears.top == m_iNext)
		((CMarker*)m_pMarker)->Move(0, -TILECY / 8);
	else if (tile->nears.bottom == m_iNext)
		((CMarker*)m_pMarker)->Move(0, TILECY / 8);

	m_iWalkTimer--;

	if (m_iWalkTimer == 0)
	{
		m_iWalkTimer = 8;
	}

	else return E_STAGE_NONPASS;

	m_iCurr = m_iNext;
	*(m_tPath->begin()) = NULL;
	m_tPath->erase(m_tPath->begin());
	((CMarker*)m_pMarker)->SetCurrent(m_iCurr);

	if (m_tPath->empty())
	{
		m_iCurr = m_iDest;
		m_iNext = 0;
		((CTile*)m_pTile)
			->ChangeTile(m_iCurr, 
				((CMarker*)m_pMarker)->GetIndex(), 
				((CMarker*)m_pMarker)->GetTeam());

		((CTile*)m_pTile)->GetTile(m_iCurr)->bAlive = true;

		((CTile*)m_pTile)
			->GetTile(m_iDest)
			->iSelect = 0;
		((CMarker*)m_pMarker)->SetCurrent(m_iCurr);
		((CMarker*)m_pMarker)->EquateFrame();
		m_eExe = E_EXE_INIT;
		return E_STAGE_DECIDE;
	}

	return E_STAGE_NONPASS;
}

CMover::CMover()
{
}


CMover::~CMover()
{
}
