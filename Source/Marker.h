#pragma once
#include "Obj.h"
#include "Stage.h"
#include "KeyInputMgr.h"

class CMarker : public CObj
{
private:
	CObj *m_pTile;
	INFO m_pMarker;
	int m_iRemains;
	int m_iIndex;
	int m_iWalkTimer;
	int m_iClass;
	PTILE m_pTarget;
private:
	E_TEAM_ID m_eTeam;
private:
	bool m_bAlive;
	int m_iCurr, m_alpha;
	int m_iMaxHP, m_iHP;
	int m_iAtt;
	int m_iAgi, m_iSpd, m_iRng;
	int m_iMiniAlpha;
public:
	void Move(int fX_, int fY_);
	void Damage(int att);
public:
	int GetIndex() { return m_iIndex; }
	int GetClass() { return m_iClass; }
	int GetHP() { return m_iHP; }
	int GetMaxHP() { return m_iMaxHP; }
	float GetHPPercent() { return float(m_iHP) / float(m_iMaxHP); }
	E_TEAM_ID GetTeam() { return m_eTeam;  }
	bool CheckAlive() { return m_bAlive; }
	bool CheckFullHP()
	{
		if (m_iHP == m_iMaxHP)
			return true;

		else return false;
	}
public:
	void Frame();
	void SetFrame(int start_, int last_) 
	{
		if (m_Frame.iLast!= last_)
		{
			m_Frame.iStart = start_;
			m_Frame.iLast = last_;
			m_Frame.iScene = start_;
		}
	}
	void EquateFrame()
	{
		m_Frame.iLast = m_Frame.iStart;
	}
	void FaceDirection(float fx_, float fy_, bool walk);
public:
	int GetAgility() { return m_iAgi; };
	int GetRange() { return m_iRng; };
public:
	int GetCurrent() { return m_iCurr;  }
	void SetCurrent(int curr_) { m_iCurr = curr_; }
public:
	void Init(int index_, E_TEAM_ID team_);
	void SetAbility(int speed_, int agility_);
	void SetClass(int class_) { m_iClass = class_;  };
	void SetCurr(int tile_);
	void SetTile(CObj *tile_) { m_pTile = tile_; };
	void EndTurn();
public:
	E_STAGE_STATE UpdateWait();
public:
	virtual void Initialize();
	virtual void Render(HDC hdc);
	virtual E_SCENE_ID Progress() { return E_SCENE_NONPASS;  }
	virtual void Release();
public:
	CMarker();
	~CMarker();
};

