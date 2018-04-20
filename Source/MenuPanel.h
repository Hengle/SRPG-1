#pragma once
#include "Obj.h"

class CMenuPanel : public CObj
{
private:
	INFO m_ArrowInfo;
	int m_iScene;
	int m_gState;
	byte m_alpha;
	byte m_arrowAlpha;
public:
	CMenuPanel();
	~CMenuPanel();
public:
	void MoveArrow();
public:
	virtual void Initialize(void);
	virtual E_SCENE_ID Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
};

