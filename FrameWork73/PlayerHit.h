#pragma once
#include "gameobject.h"

class CBridge;

class CPlayerHit :
	public CGameObject
{
public:
	CPlayerHit(INFO _info, int _att, eUnitDir _dir);
	CPlayerHit(void);
	~CPlayerHit(void);

private:
	eUnitDir m_Dir;
	float m_fHitBox;
	float  m_fDistance;
	bool m_bDeath;
	bool m_bCollision;

	FRAME  m_tFrame;
	wstring m_wstrStateKey;
	
	CBridge* m_pBridge;

public:
	bool bHit;

public:
	void SetCollision()
	{
		m_bCollision = true;
	}

	void SetBridge(CBridge* pBridge)
	{
		m_pBridge = pBridge;
	}


public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

	void Position(void);
	void MyWorldCheck();
	void MyFrameMove();


};
