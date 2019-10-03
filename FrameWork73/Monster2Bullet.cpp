#include "StdAfx.h"
#include "Monster2Bullet.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TimeMgr.h"

CMonster2Bullet::CMonster2Bullet(void)
{
}

CMonster2Bullet::CMonster2Bullet( INFO _info, int _att, float _angle )
{

	m_tInfo = _info;
	m_iAtt = _att;
	m_fAngle = _angle;
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
	m_wstrObjKey = L"Bullet";
	m_wstrStateKey = L"MonsterBullet";

}

CMonster2Bullet::~CMonster2Bullet(void)
{
	Release();
}

HRESULT CMonster2Bullet::Initialize( void )
{
	m_eRenderType = RENDER_WORLD;

	m_fSpeed = 400.f;
	m_bDeath = false;
	
	return S_OK;
}

int CMonster2Bullet::Update( void )
{
	Progress();
	MyWorldCheck();
	MyFrameMove();

	CollisionPlayer();

	if(m_bDeath)
		return 1;
	
	CGameObject::Update();

	return 0;
}

void CMonster2Bullet::Render( void )
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
	return;
}

void CMonster2Bullet::Release( void )
{
	return;

}

void CMonster2Bullet::Progress()
{
	m_tInfo.vPos.x += cosf(m_fAngle / (180.f / 3.14f)) * m_fSpeed * GET_TIME;
	m_tInfo.vPos.y += -sinf(m_fAngle / (180.f / 3.14f)) * m_fSpeed * GET_TIME;

	if(m_tInfo.vPos.x > WINCX - m_vScroll.x || m_tInfo.vPos.x < - m_vScroll.x 
		||m_tInfo.vPos.y >WINCY - m_vScroll.y || m_tInfo.vPos.y < - m_vScroll.y)
		m_bDeath = true;


}

void CMonster2Bullet::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;

}

void CMonster2Bullet::MyFrameMove()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;


	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		m_tFrame.fFrame = m_tFrame.fStart;
	}

}

void CMonster2Bullet::CollisionPlayer()
{
	RECT rc = {0};
	CGameObject* pPlayer = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	if(IntersectRect(&rc, &m_tRect, &pPlayer->GetRect()))
	{
			dynamic_cast<CPlayer*> (pPlayer)->Damage(m_iAtt);
			m_bDeath = true;

	}

}
