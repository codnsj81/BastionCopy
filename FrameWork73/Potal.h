#pragma once
#include "gameobject.h"

class CPotal :
	public CGameObject
{
public:
	CPotal(void);
	~CPotal(void);

	CGameObject* m_pPlayer;
	float		m_fAngle;
private:
	bool PlayerCollision(void);


public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
