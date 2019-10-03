#pragma once
#include "gameobject.h"

class CPlayerBullet :
	public CGameObject
{
private:
	float m_fAngle;
	FRAME m_tFrame;
	int m_iAtt;
	bool m_bDeath;
	wstring m_wstrStateKey;

public:
	void Progress();

public:
	CPlayerBullet(void);
	CPlayerBullet(INFO _info, int _att, float _angle);
	virtual ~CPlayerBullet(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	
	void CollisionMonster1(void);
	void CollisionMonster2(void);
	void CollisionBoss(void);

	void MyWorldCheck();
	void MyFrameMove();
};
