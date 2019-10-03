#include "StdAfx.h"
#include "OriginUI.h"
#include "Device.h"
#include "Player.h"
#include "ObjMgr.h"
#include "TextureMgr.h"

COriginUI::COriginUI(void)
{
}

COriginUI::~COriginUI(void)
{
}

HRESULT COriginUI::Initialize(void)
{
	CGameObject* pPlayer = CObjMgr ::GetInstacne()->GetObject(OBJ_PLAYER)->front();
	m_bGun = dynamic_cast<CPlayer*>(pPlayer)->GetGun();
	m_bHammer = dynamic_cast<CPlayer*>(pPlayer)->GetHammer();
	m_iPlayerHp = dynamic_cast<CPlayer*>(pPlayer)->GetPlayerHp();
	
	m_tInfo.vPos = D3DXVECTOR3(WINCX / 2 , WINCY / 2, 0.f);
	return S_OK;

}

int COriginUI::Update(void)
{

	return 0;
}

void COriginUI::Render(void)
{

	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"UI"
		,L"Default"
		,8);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,m_tInfo.vPos.x 
		,m_tInfo.vPos.y 
		,m_tInfo.vPos.z );

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	ArmorUI();
	PlayerHp();
	
	return;
}

void COriginUI::Release(void)
{

	return;
}

void COriginUI::ArmorUI( void )
{

	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex;

	if(*m_bHammer)
	{
		pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( L"UI"
			,L"Default"
			,3);

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,788.f
			,520.f 
			,m_tInfo.vPos.z );

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	if(*m_bGun)
	{
		pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( L"UI"
			,L"Default"
			,4);

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,850.f
			,520.f
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

void COriginUI::PlayerHp( void )
{
	D3DXMATRIX matTrans, matScale, matWorld;
	float Hp =   (float)(*m_iPlayerHp) / 200.f;

	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"UI"
		,L"Default"
		,9);

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,60.f 
		,35.f 
		,m_tInfo.vPos.z );

	D3DXMatrixScaling(&matScale, Hp, 1.f, 0.f);

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3(0.f , 0.f, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}
