#pragma once
#include "gameobject.h"
#include "Include.h"
#include "ObjMgr.h"

class CMonster :
	public CGameObject
{
public:
	CMonster(void);
	~CMonster(void);
	CMonster(UNIT_DATA);

protected:

	bool	m_bChoosing;
	bool	m_bFloating;

	//UNIT_DATA	m_MonData;
	CGameObject* m_pPlayer;
	D3DXVECTOR3  m_vPlayerPos;
	eUnitDir     m_eDir;
	bool		m_bCanDamage;
	wstring m_wstrStateKey;
	eMonsterState m_eState;
	void GetPlayer()
	{
		m_pPlayer = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYER)->front();
		m_vPlayerPos = m_pPlayer->GetInfo()->vPos;
	}

public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Update(void)PURE;
	virtual void Render(void)PURE;
	virtual void Release(void)PURE;

	virtual void MyWorldCheck()PURE;
	virtual void MyFrameMove()PURE;
	virtual void DirCheck()PURE;
	
	virtual void Progress()PURE;
	virtual void Damage(int iAtt)PURE;

};
