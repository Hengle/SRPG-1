#pragma once
#include "Obj.h"

class CSkill : public CObj
{
protected:
	int m_iIndex, m_iState, m_iRange, m_iSphere;
	float m_iMultiplier;
	INFO m_tempInfo;
	E_PATH_TYPE m_pathType;
	vector<CObj*> m_pTargets;
	CObj* m_pMarker;
public:
	int GetIndex() { return m_iIndex; }
public:
	void SetOrigin(CObj* marker_) { m_pMarker = marker_; }
	void SetTarget(CObj* target_) { m_pTargets.push_back(target_); }
public:
	int GetRange() { return m_iRange; }
	int GetSphere() { return m_iSphere;  }
	E_PATH_TYPE GetPathType() { return m_pathType; }
	float GetAttMultiplier() { return m_iMultiplier; }
public:
	virtual E_EXE_STATE Update(void)PURE;
public:
	virtual void Initialize(void)PURE;
	virtual void Render(HDC hdc) PURE;
	virtual E_SCENE_ID Progress() PURE;
	virtual void Release(void)PURE;
public:
	CSkill();
	~CSkill();
};

