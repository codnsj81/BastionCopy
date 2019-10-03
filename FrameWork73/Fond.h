#pragma once
#include "gameobject.h"

class CFond :
	public CGameObject
{
public:
	CFond(void);
	~CFond(void);
	CGameObject* m_pPlayer;


public:
	void PlayerDistance(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
