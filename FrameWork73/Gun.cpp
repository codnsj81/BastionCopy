#include "StdAfx.h"
#include "Gun.h"
#include "ObjMgr.h"
#include "NarrMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Player.h"
#include "SoundMgr.h"

CGun::CGun(void)
{
}

CGun::~CGun(void)
{
}

HRESULT CGun::Initialize(void)
{
	m_eRenderType = RENDER_WORLD;
	m_tInfo.vPos = D3DXVECTOR3(4887.f, 1751.f, 0.f);
	m_tInfo.fCX = 1.f;
	m_tInfo.fCY = 1.f;
	m_wstrObjKey =L"Item";
	m_wstrStateKey = L"Default";

	m_pPlayer = CObjMgr ::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	CGameObject ::Update();

	return S_OK;
}

int CGun::Update(void)
{

	if(m_tInfo.vPos.x > -m_vScroll.x - WINCX /2 &&
		m_tInfo.vPos.x <-m_vScroll.x + WINCX /2 &&
		m_tInfo.vPos.y > -m_vScroll.y - WINCY /2 &&
		m_tInfo.vPos.y <-m_vScroll.y + WINCY /2)
	{
		if(PlayerCollision())
			return 1;
	}
	return 0;
}

void CGun::Render(void)
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		D3DXMATRIX matTrans, matScale;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
			,m_wstrStateKey.c_str()
			,4);

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

void CGun::Release(void)
{

}

bool CGun::PlayerCollision(void)
{
	RECT rc = {0};

	if(IntersectRect(&rc, &m_tRect, &m_pPlayer->GetRect()))
	{
		dynamic_cast<CPlayer*>(m_pPlayer)->FakeGun();
		dynamic_cast<CPlayer*>(m_pPlayer)->m_fFrameSpeed = 0.6f;
		dynamic_cast<CPlayer*>(m_pPlayer)->m_bGun = true;
		CSoundMgr::GetInstacne()->PlayEffectSound(L"ItemGet.MP3");
		CNarrMgr::GetInstance()->TurnOnNarr(16);
		return true;
	}
	else
		return false;

}
