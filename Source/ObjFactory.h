#pragma once

class CObj;
template <typename T>
class CObjFactory
{
public:
	static CObj* CreateObj(void)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* CreateObj(float _fx, float _fy)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(_fx, _fy);
		return pObj;
	}

public:
	CObjFactory(void);
	~CObjFactory(void);
};
