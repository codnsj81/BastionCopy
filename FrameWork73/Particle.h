#pragma once
#include "gameobject.h"

class CParticle :
	public CGameObject
{
public:
	CParticle(void);
	~CParticle(void);

	PARTICLE	 m_Bandibul[5]; 

private:
	wstring m_wstrStateKey;
	
public:
	void XProgress(void);
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
