#include "stdafx.h"
#include "Tile.h"
#include "AIFinder.h"


CAIFinder::CAIFinder()
{

}


CAIFinder::~CAIFinder()
{

}


list<int>* CAIFinder::ForgePath()
{
	PTILE curr = ((CTile*)m_pTile)->GetTile(m_iTarget);

	while (curr->iPath > -1)
	{
		m_path.push_back(curr->iIndex);
		curr = ((CTile*)m_pTile)->GetTile(curr->iPath);
	}

	return &m_path;
}


void CAIFinder::Pathfind(int iStart_, E_TEAM_ID eTeam_, E_PATH_TYPE eType_)
{
	m_bFound = false;
	m_iMaxStep = m_iSteps = 0;

	PTILE pStart_ = ((CTile*)m_pTile)->GetTile(iStart_);

	pStart_->iStep = 0;
	pStart_->iPath = -1;
	m_open.push_back(pStart_);

	m_eTeam = eTeam_;
	m_eType = eType_;

	while (!m_open.empty() && !m_bFound)
	{
		CheckOpened();
	}

	m_second = m_bFound ? false : true;
}

void CAIFinder::CheckOpened()
{
	PTILE pTile = *m_open.begin();
	*m_open.begin() = NULL;
	m_open.erase(m_open.begin());
	m_closed.push_back(pTile);

	CheckNear(pTile, pTile->nears.left);
	CheckNear(pTile, pTile->nears.top);
	CheckNear(pTile, pTile->nears.right);
	CheckNear(pTile, pTile->nears.bottom);
}

void CAIFinder::CheckNear(PTILE pCurr, int iNear)
{
	if (m_bFound || iNear == -1)
		return;
	
	PTILE pNear = ((CTile*)m_pTile)->GetTile(iNear);
	if (pNear->iType == E_TILE_BLOCKED)
		return;

	bool closed = (find(m_closed.begin(), m_closed.end(), pNear) != m_closed.end());
	if (closed)
		return;

	if (pNear->eTeam > -1 && !m_second)
	{
		if (pNear->bAlive)
		{
			if (pNear->eTeam != m_eTeam &&
				m_eType == E_PATH_ATTACK)
			{
				m_bFound = true;
				m_iTarget = pNear->iIndex;

				AddPath(pNear, pCurr);
			}

			else if (pNear->eTeam == m_eTeam &&
				m_eType == E_PATH_HEAL)
			{
				m_bFound = true;
				m_iTarget = pNear->iIndex;

				AddPath(pNear, pCurr);
			}
		}

		return;
	}

	bool open = (find(m_open.begin(), m_open.end(), pNear) != m_open.end());
	if (!open)
		m_open.push_back(pNear);

	AddPath(pNear, pCurr);
	return;
}

void CAIFinder::AddPath(PTILE near_, PTILE curr_)
{
	near_->iStep = curr_->iStep + 1;
	if (m_iMaxStep < near_->iStep)
		m_iMaxStep = near_->iStep;
	near_->iPath = curr_->iIndex;
}

void CAIFinder::Clear()
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