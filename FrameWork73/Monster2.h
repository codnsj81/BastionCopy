#pragma once
#include "monster.h"

class CMonster2 :
	public CMonster
{
public:
	CMonster2(void);
	~CMonster2(void);
	CMonster2(UNIT_DATA);

private:
	float m_fDistance;
	float m_fAngle;
	DWORD m_dwShotTime;

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

	void CollisionHit();
	void AimToPlayer();
	void Shot();
	void PlayerDistance();
};
