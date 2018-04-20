#include "stdafx.h"
#include "Rangefinder.h"
#include "Tile.h"


CRangefinder::CRangefinder()
{

}


CRangefinder::~CRangefinder()
{

}

void CRangefinder::Pathfind(int iStart_, int iSteps_, E_TEAM_ID eTeam_, E_PATH_TYPE eType_)
{
	PTILE pStart_ = ((CTile*)m_pTile)->GetTile(iStart_);
	pStart_->iStep = 0;
	pStart_->iPath = -1;
	m_open.push_back(pStart_);

	m_maxStep = iSteps_;
	m_eTeam = eTeam_;
	m_eType = eType_;

	while (!m_open.empty())
	{
		CheckOpened();
	}
}

void CRangefinder::CheckOpened()
{
	PTILE pTile = *m_open.begin();
	*m_open.begin() = NULL;
	m_open.erase(m_open.begin());
	m_closed.push_back(pTile);

	pTile->iSelect = 1;

	if (pTile->iStep >= m_maxStep)
		return;

	CheckNear(pTile, pTile->nears.left);
	CheckNear(pTile, pTile->nears.top);
	CheckNear(pTile, pTile->nears.right);
	CheckNear(pTile, pTile->nears.bottom);
}

void CRangefinder::CheckNear(PTILE pCurr, int iNear)
{
	if (iNear == -1)
		return;

	PTILE pNear = ((CTile*)m_pTile)->GetTile(iNear);

	bool closed = (find(m_closed.begin(), m_closed.end(), pNear) != m_closed.end());
	if (closed)
		return;

	bool open = (find(m_open.begin(), m_open.end(), pNear) != m_open.end());
	if (!open)
		m_open.push_back(pNear);

	pNear->iStep = pCurr->iStep + 1;
}


void CRangefinder::Clear()
{
	m_open.clear();

	list<PTILE>::iterator it;
	for (it = m_closed.begin(); it != m_closed.end(); ++it)
	{
		(*it)->iStep = 0;
		(*it)->iSelect = 0;
		(*it) = NULL;
	}

	m_closed.clear();
}

list<int>* CRangefinder::GetTargets(E_PATH_TYPE type_)
{
	list<PTILE>::iterator it;
	for (it = m_closed.begin(); it != m_closed.end(); ++it)
	{
		(*it)->iStep = 0;
		(*it)->iSelect = 0;
		if ((*it)->eTeam > E_TEAM_NULL)
		{
			if (type_ == E_PATH_ATTACK && (*it)->eTeam != m_eTeam)
			{
				if ((*it)->bAlive)
					m_targets.push_back((*it)->iMarker);
			}

			else if (type_ == E_PATH_HEAL && (*it)->eTeam == m_eTeam)
			{
				if ((*it)->bAlive)
					m_targets.push_back((*it)->iMarker);
			}
		}
		(*it) = NULL;
	}

	m_closed.clear();

	return &m_targets;
}
