#include "StdAfx.h"
#include "GameObject.h"

D3DXVECTOR3 CGameObject::m_vScroll ;//= D3DXVECTOR3(-368.f, -1066.f, 0.f);
eUnitDir CGameObject::m_playerDir;
float		 CGameObject::m_iFade;
CGameObject::CGameObject(void)
:m_wstrObjKey(L"")
{
	memset(&m_tInfo, 0, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);

}

CGameObject::~CGameObject(void)
{
}

int CGameObject::Update( void )
{
	m_tRect.left = LONG(m_tInfo.vPos.x - (m_tInfo.fCX / 2.f)) ;
	m_tRect.right = LONG(m_tInfo.vPos.x + (m_tInfo.fCX / 2.f)) ;
	m_tRect.top = LONG(m_tInfo.vPos.y - (m_tInfo.fCY/ 2.f));
	m_tRect.bottom = LONG(m_tInfo.vPos.y + (m_tInfo.fCY / 2.f));
	return 0;
}
