#pragma once
#include "Include.h"
#include "Obj.h"
#include "Rangefinder.h"

class CTile : public CObj
{
private:
	POINTFLOAT pfMouse;
	vector<PTILE> m_vecTile;
	bool m_bBoxShowUp, m_bTextShowUp;
	bool m_bOverTile;
	int m_iTarget, m_iPrevTarget;
	INFO m_tInfo;
private:
	void InitTile();
	void TargetTile();
	PTILE CreateTile(float fx_, float fy_, int index_);
	void CheckKey();
	void SaveData();
public:
	void LoadData();
	PTILE GetTile(int iIndex) { return m_vecTile[iIndex]; };
	PTILE GetTargeted() 
	{
		if (m_iTarget > -1)
			return m_vecTile[m_iTarget];
		else return nullptr;
	};
	void HighlightMove();
	void HighlightAct(int range, E_TEAM_ID team_, E_PATH_TYPE type_);
	void ChangeTile(int curr_, int marker_, E_TEAM_ID team_);
	bool OverTile() { return m_bOverTile; }
public:
	void CheckSave();
public:
	virtual void Initialize();
	virtual E_SCENE_ID Progress();
	virtual void Render(HDC hdc);
	virtual void Release();
public:
	CTile();
	virtual ~CTile();
};

