#pragma once

class CMarker;
class CAIMgr
{
public:
	CMarker* m_pMarker;
	list<CMarker*>* m_pTargets;

public:
	void SetPossible(list<CMarker*> targets_) { m_pTargets = &targets_; }
	void SetMarker(CMarker* marker_) { m_pMarker = marker_; }
public:
	void Update();
public:
	CAIMgr();
	~CAIMgr();
};

