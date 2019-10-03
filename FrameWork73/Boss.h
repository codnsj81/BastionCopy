#pragma once
#include "monster.h"

class CBoss :
	public CMonster
{
public:
	CBoss(void);
	CBoss(UNIT_DATA);
	~CBoss(void);
	
private:
	vector<TILE*>* m_VecTile;
	float m_fDistance;
	D3DXVECTOR3 m_vAttackPos;
	bool m_bAttack;
	DWORD m_dwTime;
	float m_fOriginY;
	float m_ftime;

	void PlayerDistance();
	void PlayerChasing();
	void PlayerAttack();

	void Jumping();
	void Landing();

	void CollisionPlayer();
	void CollisionHit();
	void CollisionBullet();
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

	virtual void MyWorldCheck();
	virtual void MyFrameMove();
	virtual void DirCheck();

	virtual void Damage(int iAtt);
	virtual void Progress();
};
