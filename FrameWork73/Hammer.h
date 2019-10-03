#pragma once
#include "gameobject.h"

class CHammer :
	public CGameObject
{
public:
	CHammer(void);
	~CHammer(void);

	CGameObject* m_pPlayer;

private:
	bool PlayerCollision(void);
	void PlayerDistance(void);
	wstring m_wstrStateKey;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

};
