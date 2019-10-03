#pragma once
#include "gameobject.h"

class CParticleMenu :
	public CGameObject
{
public:
	CParticleMenu(void);
	~CParticleMenu(void);

	PARTICLE	 m_Bandibul[15]; 

private:
	wstring m_wstrStateKey;

public:
	void XProgress(void);
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
