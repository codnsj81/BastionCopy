#include "StdAfx.h"
#include "Hammer.h"
#include "ObjMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "NarrMgr.h"
#include "Player.h"
#include "Factory.h"
#include "ObjMgr.h"
#include "UIMgr.h"
#include "OriginUI.h"
#include "UI.h"

CHammer::CHammer(void)
{
}

CHammer::~CHammer(void)
{
}

HRESULT CHammer::Initialize(void)
{
	m_eRenderType = RENDER_WORLD;
	m_tInfo.vPos = D3DXVECTOR3(724.f, 3639.f, 0.f);
	m_tInfo.fCX = 1.f;
	m_tInfo.fCY = 1.f;
	m_wstrObjKey =L"Item";
	m_wstrStateKey = L"Default";

	m_pPlayer = CObjMgr ::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	CGameObject ::Update();

	return S_OK;
}

int CHammer::Update(void)
{
	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{

		if(PlayerCollision())
			return 1;

		PlayerDistance();
	}
	return 0;
}

void CHammer::Render(void)
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY )

	{
		D3DXMATRIX matTrans, matScale;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
			,m_wstrStateKey.c_str()
			,3);

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,m_tInfo.vPos.x + m_vScroll.x
			,m_tInfo.vPos.y + m_vScroll.y
			,m_tInfo.vPos.z );

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));


	}


}

void CHammer::Release(void)
{

}

bool CHammer::PlayerCollision(void)
{
	RECT rc = {0};

	if(IntersectRect(&rc, &m_tRect, &m_pPlayer->GetRect()))
	{
		dynamic_cast<CPlayer*>(m_pPlayer)->FakeHammer();
		dynamic_cast<CPlayer*>(m_pPlayer)->m_fFrameSpeed = 0.6f;
		dynamic_cast<CPlayer*>(m_pPlayer)->m_bHammer = true;
		


		
		return true;
	}
	else
		return false;

}

void CHammer::PlayerDistance( void )
{
	float fDistance;
	INFO* pInfo = m_pPlayer->GetInfo();
	D3DXVECTOR3 vDis = m_tInfo.vPos - pInfo->vPos;
	fDistance = D3DXVec3Length(&vDis);

	if(fDistance < 400)
	{
		CNarrMgr::GetInstance()->TurnOnNarr(6);

	}

}
