#pragma once
#include "scene.h"

class CStage2 :
	public CScene
{
	
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void RenderUI(void);

	void UnitLoad(void);
	void ObjectLoad(void);
	void MonsterInit(void);
	D3DXVECTOR3* m_vScroll;

private:
	list<CGameObject*> m_pMosnter1Data ;
	list<CGameObject*>::iterator m_iter;
	DWORD	m_dwMonsterTime;
	DWORD	m_dwMonsterTime2;
	bool		m_bMon;

public:
	CStage2(void);
	virtual ~CStage2(void);
};
