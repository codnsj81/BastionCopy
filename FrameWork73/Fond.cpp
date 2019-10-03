#include "StdAfx.h"
#include "Fond.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TextureMgr.h"
#include "NarrMgr.h"
#include "Device.h"
#include "SceneMgr.h"

CFond::CFond(void)
{
}

CFond::~CFond(void)
{
}

HRESULT CFond::Initialize( void )
{
	m_eRenderType = RENDER_WORLD;
	m_wstrObjKey = L"Object";
	
	if(CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE)
		m_tInfo.vPos = D3DXVECTOR3(5613.f, 1549.f, 0.f);
	else
		m_tInfo.vPos = D3DXVECTOR3(3571.f, 838.f, 0.f);

	m_tInfo.fCX = 1.f;
	m_tInfo.fCY = 1.f;


	m_pPlayer = CObjMgr ::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	CGameObject ::Update();

	return S_OK;
}

int CFond::Update( void )
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		PlayerDistance();
	}
	return 0;
}

void CFond::Render( void )
{


	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		D3DXMATRIX matTrans, matScale;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
			,L"Default"
			,24);

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

void CFond::Release( void )
{

}
void CFond::PlayerDistance( void )
{
	float fDistance;
	INFO* pInfo = m_pPlayer->GetInfo();
	D3DXVECTOR3 vDis = m_tInfo.vPos - pInfo->vPos;
	fDistance = D3DXVec3Length(&vDis);

	if(fDistance < 400)
	{
		CNarrMgr::GetInstance()->TurnOnNarr(17);
		if(GetAsyncKeyState('E'))
		{
			dynamic_cast<CPlayer*>(m_pPlayer)->DrinkingSet();

		}

	}

}
