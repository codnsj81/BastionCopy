#pragma once
#include "gameobject.h"

class CGun :
	public CGameObject
{
public:
	CGun(void);
	~CGun(void);
	CGameObject* m_pPlayer;

private:
	bool PlayerCollision(void);
	wstring m_wstrStateKey;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
