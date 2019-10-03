#pragma once
#include "monster.h"

class CMonster1 :
	public CMonster
{
public:
	CMonster1(void);
	CMonster1(UNIT_DATA);
	~CMonster1(void);

private:
	float m_fDistance;
	D3DXVECTOR3 m_vAttackPos;
	bool	m_bAttack;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

	virtual void MyWorldCheck();
	virtual void MyFrameMove();
	virtual void DirCheck();
	
	virtual void Damage(int iAtt)
	{
		m_iHp -= iAtt;
		cout<<m_iHp<<endl;
	}

private:
	void PlayerDistance();
	void PlayerChasing();
	void PlayerAttack();

	
	virtual void Progress();
	virtual void CollisionHit();
	virtual void CollisionPlayer();


};
