#pragma once
#include "gameobject.h"

class CGameScroll :
	public CGameObject
{
public:
	CGameScroll(void);
	~CGameScroll(void);

	D3DXVECTOR3 m_playerPos;


public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
