#pragma once
#include "Obj.h"

class CCurtain : public CObj
{
private:
	bool m_rendering;
	byte m_bAlpha;
public:
	void SetAlpha(byte alpha_) { m_bAlpha = alpha_; }
	bool Open(byte speed_);
	bool Close(byte speed_);
public:
	virtual void Initialize(void);
	virtual E_SCENE_ID Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CCurtain();
	~CCurtain();
};

