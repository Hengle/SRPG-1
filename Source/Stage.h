#pragma once
#include "Scene.h"
#include "ObjFactory.h"
#include "BackGround.h"
#include "YSortMgr.h"
#include "Marker.h"
#include "Mover.h"
#include "Attacker.h"
#include "AIExecuter.h"
#include "Thunder.h"
#include "Slash.h"
#include "Ice.h"
#include "Wheel.h"

class Executer;

class CStage : public CScene
{
private:
	int m_victoryAlpha;

	list<CObj*>	m_ObjList[E_OBJ_END];
	DWORD m_dwZenTime;
	
	CWheel* m_pWheelMenu;
	CObj* m_pTile;
	CObj* m_pCurr;
	CMarker* m_pTarget;
	vector<CObj*> m_pMarkers;
	vector<CMarker*> m_pAllies;
	vector<CMarker*> m_pEnemies;
	CExecuter* m_pMover, *m_pAttacker, *m_pAI;

	CSlash* m_slash;
	CHeal* m_heal;
	CFire* m_fire;
	CIce* m_ice;
	CThunder* m_thunder;

	RECT rect;
	wchar_t wcTmp[128];

	E_STAGE_STATE m_eStage;
	
private:
	void UpdateMarkers();
	void CheckVictory();
	void CheckGameover();
	void CheckClass(int class_);
	void FocusCam();
public:
	static bool ZoomCam(float fx_, float fy_, int time_);
	static bool m_bZooming;
	static POINTFLOAT m_tCam;
	static int m_iZoomTime;
	static float m_zoomX;
	static float m_zoomY;
	CObj* CreateMarker(int index_, E_TEAM_ID team_, int curr_, int agi_, int spd_, int class_);
public:
	virtual void Initialize();
	virtual void Render(HDC hdc);
	virtual void Release();
	virtual E_SCENE_ID Progress();
public:
	virtual void Update();

public:
	CStage();
	virtual ~CStage();
};

