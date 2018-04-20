#pragma once
#include "Obj.h"

class CBackGround : public CObj
{
public:
	virtual void Initialize(void);
	virtual E_SCENE_ID Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBackGround();
	virtual ~CBackGround();
};

