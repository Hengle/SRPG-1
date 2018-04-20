#include "stdafx.h"
#include <algorithm>
#include "Pathfinder.h"
#include "Tile.h"


CPathfinder::CPathfinder()
{

}

CPathfinder::~CPathfinder()
{

}

list<int>* CPathfinder::ForgePath(int iIndex)
{
	PTILE curr = ((CTile*)m_pTile)->GetTile(iIndex);
	
	while (curr->iPath > -1)
	{
		m_path.push_back(curr->iIndex);
		curr = ((CTile*)m_pTile)->GetTile(curr->iPath);
	}

	return &m_path;
}

void CPathfinder::Pathfind(int iStart_, int iSteps_, E_TEAM_ID eTeam_, E_PATH_TYPE eType_)
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

	if (eType_ == E_PATH_HEAL)
		pStart_->iType = E_TILE_ACT;
	else pStart_->iType = E_TILE_NONE;
}

void CPathfinder::CheckOpened()
{
	PTILE pTile = *m_open.begin();
	*m_open.begin() = NULL;
	m_open.erase(m_open.begin());
	m_closed.push_back(pTile);

	switch (m_eType)
	{
		case E_PATH_MOVE:
			if (pTile->eTeam != m_eTeam)
				pTile->iType = E_TILE_MOVE;
		break;

		case E_PATH_ATTACK:
			if (pTile->iType != E_TILE_BLOCKED)
				pTile->iType = E_TILE_ACT;

			if (pTile->eTeam > -1)
			{
				if (pTile->eTeam != m_eTeam)
					pTile->iType = E_TILE_TARGET;
			}
				
			break;
		case E_PATH_HEAL:
			if (pTile->iType != E_TILE_BLOCKED)
				pTile->iType = E_TILE_ACT;

			if (pTile->eTeam > -1 && pTile->eTeam == m_eTeam)
			{
				pTile->iType = E_TILE_TARGET;
			}
		break;
	}

	if (pTile->iStep >= m_maxStep)
		return;

	CheckNear(pTile, pTile->nears.left);
	CheckNear(pTile, pTile->nears.top);
	CheckNear(pTile, pTile->nears.right);
	CheckNear(pTile, pTile->nears.bottom);
}

void CPathfinder::CheckNear(PTILE pCurr, int iNear)
{
	if (iNear == -1)
		return;

	PTILE pNear = ((CTile*)m_pTile)->GetTile(iNear);

	if (m_eType == E_PATH_MOVE)
	{
		if (pNear->iType == E_TILE_BLOCKED)
			return;

		if (pNear->eTeam > -1)
		{
			if (pNear->eTeam != m_eTeam)
				return;
		}
	}

	bool closed = (find(m_closed.begin(), m_closed.end(), pNear) != m_closed.end());
	if (closed)
		return;

	bool open = (find(m_open.begin(), m_open.end(), pNear) != m_open.end());
	if (!open)
		m_open.push_back(pNear);

	pNear->iStep = pCurr->iStep + 1;
	pNear->iPath = pCurr->iIndex;
}

void CPathfinder::Clear()
{
	m_open.clear();
	
	list<PTILE>::iterator it;
	for (it = m_closed.begin(); it != m_closed.end(); ++it)
	{
		(*it)->iPath = -2;
		(*it)->iStep = 0;
		(*it)->iSelect = 0;
		if ((*it)->iType != E_TILE_BLOCKED)
			(*it)->iType = E_TILE_NONE;
		(*it) = NULL;
	}

	m_closed.clear();
}

