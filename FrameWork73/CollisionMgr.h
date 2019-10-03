#pragma once
#include "Include.h"

class CGameObject;

class CCollisionMgr
{
private:
	static CCollisionMgr* m_pInstance;

public:
	static CCollisionMgr* GetInstacne(void)
	{gr
		if(m_pInstance == NULL)
			m_pInstance = new CCollisionMgr;
		return m_pInstance;
	}

	void Destroy(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

public:
	bool CollisionObstacle(CGameObject* pObj, eObjectType eType);

private:
	CCollisionMgr(void);
public:
	~CCollisionMgr(void);
};
