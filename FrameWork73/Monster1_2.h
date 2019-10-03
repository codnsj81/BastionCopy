#pragma once
#include "Monster.h"

class CMonster1_2
	:public CMonster
{
public:
	CMonster1_2(void);
	CMonster1_2(UNIT_DATA);
	~CMonster1_2(void);


private:
	float m_OriginY;
	float m_OriginX;
	float m_fDistance;
	D3DXVECTOR3 m_vAttackPos;
	bool	m_bAttack;
	eMonsterState m_eState;
	eMonsterBoxState   m_eBoxState;
	bool m_bVibe;
	DWORD m_dwTime;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

	void Box_FallingProgress(void);
	void Box_FloatingProgress(void);
	void Box_VibratingProgess(void);
	void Box_PopingProgress(void);

	virtual void MyWorldCheck();
	virtual void MyFrameMove();
	virtual void DirCheck();

	virtual void Damage(int iAtt)
	{
		m_iHp -= iAtt;
		//cout<<m_iHp<<endl;
	}

private:
	void PlayerDistance();
	void PlayerChasing();
	void PlayerAttack();


	virtual void Progress();
	virtual void CollisionHit();
	virtual void CollisionPlayer();

};
