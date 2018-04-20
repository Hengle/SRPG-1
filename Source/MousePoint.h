#pragma once
#include "Include.h"

class CMousePoint
{
public:
	static POINT GetPos(void)
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		return pt;
	}
public:
	CMousePoint();
	~CMousePoint();
};

