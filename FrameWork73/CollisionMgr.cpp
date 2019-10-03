#include "StdAfx.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "GameObject.h"
#include "Obstacle.h"

CCollisionMgr* CCollisionMgr::m_pInstance = NULL;


CCollisionMgr::CCollisionMgr(void)
{
}

CCollisionMgr::~CCollisionMgr(void)
{

}

bool CCollisionMgr::CollisionObstacle( CGameObject* pObj, eObjectType eType )
{
	return false;
}
