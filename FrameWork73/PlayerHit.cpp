#include "StdAfx.h"
#include "PlayerHit.h"
#include "TimeMgr.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
#include "Monster1.h"
#include "Device.h"
#include "Bridge.h"
CPlayerHit::CPlayerHit(void)
{
}

CPlayerHit::CPlayerHit(INFO _info, int _att, eUnitDir _dir)
:m_pBridge(NULL)
{
	m_iAtt = _att;
	m_bCollision = false;
	m_tInfo = _info;
	m_Dir = _dir;
	m_tInfo.fCY = 100.f;
	m_tInfo.fCX = 100.f;
	m_bDeath = false;
	bHit = false;

}

CPlayerHit::~CPlayerHit(void)
{
	Release();
}

HRESULT CPlayerHit::Initialize(void)
{
	m_eRenderType = RENDER_EFFECT;;

	if(m_pBridge)
		m_pBridge->SetGameObject(this);

	m_fHitBox = 90.f;
	m_tFrame = FRAME(0.f, 0.f, 14.f, 14.f);
	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Hammer";

	m_fDistance = 90;

	Position();

	return S_OK;
}

int CPlayerHit::Update(void)
{
	MyWorldCheck();
	MyFrameMove();

	if(m_bDeath == true)
		return 1;

	CGameObject::Update();

	return 0;

}

void CPlayerHit::Render(void)
{	


	if(m_bCollision == true)
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

	}

}

void CPlayerHit::Release(void)
{

}

void CPlayerHit::Position(void)
{
	if(m_Dir == DIR_DOWN)
	{
		m_tInfo.vPos.y += m_fDistance;
	}
	if(m_Dir == DIR_UP)
	{
		m_tInfo.vPos.y -= m_fDistance;

	}
	if(m_Dir == DIR_L)
	{
		m_tInfo.vPos.x -= m_fDistance;

	}
	if(m_Dir == DIR_LD)
	{
		m_tInfo.vPos.x -= cos(315.f) * m_fDistance;
		m_tInfo.vPos.y -= -sin(315.f) * m_fDistance;

	}
	if(m_Dir == DIR_LU)
	{
		m_tInfo.vPos.x -= cos(315.f) * m_fDistance;
		m_tInfo.vPos.y += -sin(315.f) * m_fDistance;

	}
	if(m_Dir == DIR_R)
	{
		m_tInfo.vPos.x += m_fDistance;

	}
	if(m_Dir == DIR_RD)
	{
		m_tInfo.vPos.x += cos(45.f) *m_fDistance;
		m_tInfo.vPos.y -= -sin(45.f) * m_fDistance;

	}

	if(m_Dir == DIR_RU)
	{
		m_tInfo.vPos.x += cos(315.f) * m_fDistance;
		m_tInfo.vPos.y += -sin(315.f) * m_fDistance;

	}

}

void CPlayerHit::MyFrameMove()
{	
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;


	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		m_bDeath = true;
		m_tFrame.fFrame = m_tFrame.fStart;
	}

}

void CPlayerHit::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;

}
