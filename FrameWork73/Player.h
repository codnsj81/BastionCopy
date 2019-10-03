#pragma once
#include "gameobject.h"

class CPlayer :
	public CGameObject
{
private:
	wstring m_wstrStateKey;
	bool bFrameCheck;
public:
	void Damage(int iAtt)
	{
		m_iHp -= iAtt;
		cout<<m_iHp<<endl;
	}

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	void NormalRender(void);
	void FallingRender(void);
	void ReturningRender(void);
	ePlayerState GetState(void)
	{
		return m_State;
	}

public:
	void CollisionObject(void);
	void Returning(void);
	void Refalling(void);
	void NARRCheck(void);
	void Attack();
	void FakeHammer();
	void FakeGun();
	void HammerAttack();
	void GunAttack();
	void Drift();

	float       m_fFrameSpeed;
	bool		m_bHammer;
	bool		m_bGun;
private:
	ePlayerState m_State;

	bool		m_bDrift;
	D3DXVECTOR3 m_vecGun;
	D3DXVECTOR3 m_vRePos; // 돌아오는
	D3DXVECTOR3  m_vUpPos; // 위에 떠있을 
	D3DXMATRIX m_matSize;

	bool		m_bReturning;
	bool		m_bFalling;
	float		m_fGunAngle;
	int*		m_iZoom;

public:
	void DirCheck();
	void Move();
	void CollisionTile();
	void Drinking();
	void DrinkingSet();

	int* GetPlayerHp()
	{
		return &m_iHp;
	}
	bool* GetGun()
	{
		return &m_bGun;
	}

	bool* GetHammer()
	{
		return &m_bHammer;
	}

private:
	void MyWorldCheck();
	void MyFrameMove();

public:
	CPlayer(void);
	virtual ~CPlayer(void);
};
