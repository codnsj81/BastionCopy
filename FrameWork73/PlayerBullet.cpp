#include "StdAfx.h"
#include "PlayerBullet.h"
#include "Device.h"
#include "TextureMgr.h"
#include "CollisionMgr.h"
#include "Monster2.h"
#include "Monster1_2.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "Monster1.h"
#include "TimeMgr.h"
#include "Boss.h"

CPlayerBullet::CPlayerBullet(void)
{
}

CPlayerBullet::CPlayerBullet( INFO _info, int _att, float _angle )
{
	m_tInfo = _info;
	m_iAtt = _att;
	m_fAngle = _angle;
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
	m_wstrObjKey = L"Bullet";
	m_wstrStateKey = L"PlayerBullet";


}

CPlayerBullet::~CPlayerBullet(void)
{
	Release();
}

HRESULT CPlayerBullet::Initialize(void)
{
	m_eRenderType = RENDER_WORLD;

	m_fSpeed = 400.f;
	m_bDeath = false;
	return S_OK;

}

int CPlayerBullet::Update(void)
{
	Progress();
	MyWorldCheck();
	MyFrameMove();
	CollisionMonster1();
	CollisionMonster2();
	CollisionBoss();

	if(m_bDeath)
		return 1;

	CGameObject::Update();

	return 0;
}

void CPlayerBullet::Render(void)
{
	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,(int)m_tFrame.fFrame);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);


	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));



	//HDC hdc = GetDC(g_hWnd);
	//Rectangle(hdc, 
	//	static_cast<int> (hdc,m_tRect.left + m_vScroll.x), 
	//	static_cast<int> (m_tRect.top + m_vScroll.y) ,
	//	static_cast<int> (m_tRect.right + m_vScroll.x),
	//	static_cast<int> (m_tRect.bottom + m_vScroll.y));

}

void CPlayerBullet::Release(void)
{

}

void CPlayerBullet::Progress()
{

	m_tInfo.vPos.x += cosf(m_fAngle / (180.f / 3.14f)) * m_fSpeed * GET_TIME;
	m_tInfo.vPos.y += -sinf(m_fAngle / (180.f / 3.14f)) * m_fSpeed * GET_TIME;

	if(m_tInfo.vPos.x > WINCX - m_vScroll.x || m_tInfo.vPos.x < - m_vScroll.x 
		||m_tInfo.vPos.y >WINCY - m_vScroll.y || m_tInfo.vPos.y < - m_vScroll.y)
		m_bDeath = true;

}

void CPlayerBullet::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;

}

void CPlayerBullet::MyFrameMove()
{	
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;


	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		m_tFrame.fFrame = m_tFrame.fStart;
	}


}

void CPlayerBullet::CollisionMonster1( void )
{
	RECT rc = {0};
	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER1)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER1)->end(); ++iter)
	{
		if(IntersectRect(&rc, &m_tRect, &(*iter)->GetRect()))
		{
			m_bDeath = true;
			dynamic_cast<CMonster1*> (*iter)->Damage(m_iAtt);

		}
	}

	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER1_STAGE2)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER1_STAGE2)->end(); ++iter)
	{
		if(IntersectRect(&rc, &m_tRect, &(*iter)->GetRect()))
		{
			m_bDeath = true;
			dynamic_cast<CMonster1_2*> (*iter)->Damage(m_iAtt);

		}
	}

}

void CPlayerBullet::CollisionMonster2( void )
{
	RECT rc = {0};
	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER2)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_MONSTER2)->end(); ++iter)
	{
		if(IntersectRect(&rc, &m_tRect, &(*iter)->GetRect()))
		{
			m_bDeath = true;
			dynamic_cast<CMonster2*> (*iter)->Damage(m_iAtt);

		}
	}

}

void CPlayerBullet::CollisionBoss( void )
{
	
	RECT rc = {0};
	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_BOSS)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_BOSS)->end(); ++iter)
	{
		if(IntersectRect(&rc, &m_tRect, &(*iter)->GetRect()))
		{
			FRAME pFrame = FRAME(0.f, 0.f, 5.f, 5.f);	
			CGameObject* pEffect = new CEffect(m_tInfo, L"Shot", pFrame);
			pEffect->Initialize();
			CObjMgr::GetInstacne()->AddGameObject(pEffect, OBJ_EFFECT);

			m_bDeath = true;
			dynamic_cast<CBoss*> (*iter)->Damage(m_iAtt);

		}
	}
}
