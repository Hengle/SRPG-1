#include "stdafx.h"
#include "Attacker.h"
#include "Tile.h"
#include "Marker.h"
#include "Slash.h"


CSkill* CAttacker::m_pSkill = NULL;

E_STAGE_STATE CAttacker::Search()
{
	m_iCurr = ((CMarker*)m_pMarker)->GetCurrent();
	GET_SINGLE(CPathfinder)->Pathfind(m_iCurr,
		m_pSkill->GetRange(),
		((CMarker*)m_pMarker)->GetTeam(),
		m_pSkill->GetPathType());

	m_pTarget = ((CTile*)m_pTile)->GetTargeted();
	
	return E_STAGE_NONPASS;
}

E_STAGE_STATE CAttacker::Select()
{
	if (GET_SINGLE(CKeyInputMgr)->GetMouseClickState(0) == EINPUT_DOWN)
	{
		m_pTarget = ((CTile*)m_pTile)->GetTargeted();

		if (!m_pTarget)
			return E_STAGE_NONPASS;

		if (m_pTarget->iType == E_TILE_ACT ||
			m_pTarget->iType == E_TILE_TARGET)
		{
			((CMarker*)m_pMarker)->FaceDirection(m_pTarget->fX, m_pTarget->fY, false);

			m_tTargets = GET_SINGLE(CRangefinder)->GetTargets(m_pSkill->GetPathType());
			if (m_tTargets->empty())
			{
				return E_STAGE_NONPASS;
			}
				
			GET_SINGLE(CPathfinder)->Clear();

			//Temporary, will organize
			m_pSkill->SetOrigin(m_pMarker);
			list<int>::iterator it;
			for (it = m_tTargets->begin(); it != m_tTargets->end(); ++it)
			{
				m_pSkill->SetTarget(m_pMarkers->at(*it));
			}

			m_eExe = E_EXE_READY;
		}
	}

	else ((CTile*)m_pTile)
		->HighlightAct(
			m_pSkill->GetSphere(),
			((CMarker*)m_pMarker)->GetTeam(), 
			m_pSkill->GetPathType());
}

E_STAGE_STATE CAttacker::Ready()
{
	m_eExe = m_pSkill->Update();
	if (m_eExe == E_EXE_EXECUTE)
		m_dwTime = GetTickCount();	
	return E_STAGE_NONPASS;
}

E_STAGE_STATE CAttacker::Execute()
{
	list<int>::iterator it;

	CMarker* marker;
	for (it = m_tTargets->begin(); it != m_tTargets->end(); ++it)
	{
		marker = ((CMarker*)(m_pMarkers->at(*it)));
		marker->FaceDirection(m_pMarker->GetInfo().fX, m_pMarker->GetInfo().fY, false);
		m_damage = int(2 * m_pSkill->GetAttMultiplier());
		marker->Damage(m_damage);
		((CMarker*)m_pMarker)->FaceDirection(marker->GetInfo().fX, marker->GetInfo().fY, false);
	}

	m_eExe = E_EXE_WAIT;
	m_dwTime = GetTickCount();
	return E_STAGE_NONPASS;
}


E_STAGE_STATE CAttacker::Wait()
{
	if (m_dwTime + 500 < GetTickCount())
	{
		m_tTargets->clear();
		m_eExe = E_EXE_INIT;
		return E_STAGE_FOCUS;
	}

	return E_STAGE_NONPASS;
}

void CAttacker::Render(HDC hdc)
{
	list<int>::iterator it;

	CMarker* marker;
	INFO temp;

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));

	if (m_eExe == E_EXE_READY)
		m_pSkill->Render(hdc);

	else if (m_eExe == E_EXE_WAIT)
	{
		for (it = m_tTargets->begin(); it != m_tTargets->end(); ++it)
		{
			temp = m_pMarkers->at(*it)->GetInfo();
			rect = {
				0,
				0,
				200,
				200,
			};

			swprintf(wcTmp, 128, L"%d", m_damage);
			DrawText(hdc, wcTmp, -1, &rect, NULL);
		}
	}

}

CAttacker::CAttacker()
{

}


CAttacker::~CAttacker()
{

}
