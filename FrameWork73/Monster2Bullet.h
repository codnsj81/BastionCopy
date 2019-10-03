#pragma once
#include "gameobject.h"

class CMonster2Bullet :
	public CGameObject
{
public:
	CMonster2Bullet(void);
	CMonster2Bullet(INFO _info, int _att, float _angle);
	~CMonster2Bullet(void);
	
private:
	float m_fAngle;
	FRAME m_tFrame;
	int m_iAtt;
	bool m_bDeath;
	wstring m_wstrStateKey;


public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

	void MyWorldCheck();
	void MyFrameMove();
	void CollisionPlayer();

	void Progress();

};
