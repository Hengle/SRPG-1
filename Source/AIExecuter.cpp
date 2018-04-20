#include "stdafx.h"
#include "AIExecuter.h"
#include "AIFinder.h"
#include "Tile.h"
#include "Marker.h"

CSkill* CAIExecuter::m_pSkill = NULL;

E_STAGE_STATE CAIExecuter::Search()
{
	m_iWalkTimer = 8;
	m_iCurr = ((CMarker*)m_pMarker)->GetCurrent();

	GET_SINGLE(CAIFinder)->Clear();
	GET_SINGLE(CAIFinder)->Pathfind(m_iCurr,
		((CMarker*)m_pMarker)->GetTeam(),
		m_pSkill->GetPathType());

	if (GET_SINGLE(CAIFinder)->CheckSecond())
	{
		GET_SINGLE(CAIFinder)->Clear();
		GET_SINGLE(CAIFinder)->Pathfind(m_iCurr,
			((CMarker*)m_pMarker)->GetTeam(),
			m_pSkill->GetPathType());
	}

	return E_STAGE_NONPASS;
}

E_STAGE_STATE CAIExecuter::Select()
{
	m_pSkill->Initialize();
	m_tPath = GET_SINGLE(CAIFinder)->ForgePath();
	m_tPath->reverse();
	
	((CTile*)m_pTile)->ChangeTile(m_iCurr, -1, E_TEAM_NULL);
	((CTile*)m_pTile)->GetTile(m_iCurr)->bAlive = false;
	m_eExe = E_EXE_READY;
	return E_STAGE_NONPASS;
}

E_STAGE_STATE CAIExecuter::Ready()
{
	m_iNext = *m_tPath->begin();
	PTILE tile = ((CTile*)m_pTile)->GetTile(m_iCurr);

	if (GET_SINGLE(CAIFinder)->GetMaxSteps() < m_pSkill->GetRange() + 1 ||
		GET_SINGLE(CAIFinder)->GetSteps() > ((CMarker*)m_pMarker)->GetAgility() ||
		(GET_SINGLE(CAIFinder)->CheckSecond() && ((CTile*)m_pTile)->GetTile(m_iNext)->eTeam > -1))
	{
		((CTile*)m_pTile)
			->ChangeTile(m_iCurr,
				((CMarker*)m_pMarker)->GetIndex(),
				((CMarker*)m_pMarker)->GetTeam());

		((CTile*)m_pTile)->GetTile(m_iCurr)->bAlive = true;

		m_attackState = 0;
		((CMarker*)m_pMarker)->SetCurrent(m_iCurr);
		((CMarker*)m_pMarker)->EquateFrame();
		m_eExe = E_EXE_EXECUTE;
		return E_STAGE_NONPASS;
	}
	
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
	GET_SINGLE(CAIFinder)->RemoveSteps();

	return E_STAGE_NONPASS;
}

E_STAGE_STATE CAIExecuter::Execute()
{
	list<int>::iterator it; 
	E_EXE_STATE temp;
	switch (m_attackState)
	{
		case 0:
			if (m_tPath->size() > m_pSkill->GetRange())
			{
				GET_SINGLE(CAIFinder)->OffSecond();
				m_eExe = E_EXE_INIT;
				m_tPath->clear();
				return E_STAGE_ENDTURN;
			}

			GET_SINGLE(CRangefinder)->Clear();
			GET_SINGLE(CRangefinder)->Pathfind(GET_SINGLE(CAIFinder)->GetTarget(), 
					m_pSkill->GetSphere(), 
					((CMarker*)m_pMarker)->GetTeam(), 
					m_pSkill->GetPathType());
					
			m_pTarget = ((CTile*)m_pTile)->GetTile(GET_SINGLE(CAIFinder)->GetTarget());
			((CMarker*)m_pMarker)->FaceDirection(m_pTarget->fX, m_pTarget->fY, false);

			m_tTargets = GET_SINGLE(CRangefinder)->GetTargets(m_pSkill->GetPathType());
			if (m_tTargets->empty())
			{
				m_eExe = E_EXE_INIT;
				return E_STAGE_ENDTURN;
			}

			//Temporary, will organize
			m_pSkill->SetOrigin(m_pMarker);
			for (it = m_tTargets->begin(); it != m_tTargets->end(); ++it)
			{
				m_pSkill->SetTarget(m_pMarkers->at(*it));
			}

			m_attackState = 1;

			break;
		case 1:
			temp = m_pSkill->Update();
			if (temp == E_EXE_EXECUTE)
			{
				m_dwTime = GetTickCount();
				m_attackState = 2;
			}

			break;
		case 2:
			CMarker* marker;
			for (it = m_tTargets->begin(); it != m_tTargets->end(); ++it)
			{
				marker = ((CMarker*)(m_pMarkers->at(*it)));
				marker->FaceDirection(m_pMarker->GetInfo().fX, m_pMarker->GetInfo().fY, false);
				marker->Damage(int(2 * m_pSkill->GetAttMultiplier()));
				((CMarker*)m_pMarker)->FaceDirection(marker->GetInfo().fX, marker->GetInfo().fY, false);
			}

			m_attackState = 3;
			break;

		case 3:
			if (CStage::ZoomCam(m_pMarker->GetInfo().fX, m_pMarker->GetInfo().fY, 20))
			{
				m_tTargets->clear();
				m_eExe = E_EXE_INIT;

				return E_STAGE_ENDTURN;
			}
			break;
	}
	
	return E_STAGE_NONPASS;
}


void CAIExecuter::Render(HDC hdc)
{
	if (m_attackState == 1)
		m_pSkill->Render(hdc);
}

CAIExecuter::CAIExecuter()
{
}


CAIExecuter::~CAIExecuter()
{
}
