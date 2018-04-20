#pragma once
#include "Executer.h"

class CMover : public CExecuter
{
private:
	POINTFLOAT m_tPoint;
	list<int>* m_tPath;
	int m_iWalkTimer;
	int m_iNext, m_iDest;
public:
	virtual E_STAGE_STATE Search();
	virtual E_STAGE_STATE Select();
	virtual E_STAGE_STATE Ready();
	virtual E_STAGE_STATE Execute();
public:
	CMover();
	~CMover();
};

