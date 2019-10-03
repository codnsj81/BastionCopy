#include "StdAfx.h"
#include "MainLogo.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "MenuPan.h"
#include "UIMgr.h"

CMainLogo::CMainLogo(void)
{
}

CMainLogo::~CMainLogo(void)
{
	Release();
}

HRESULT CMainLogo::Initialize( void )
{
	m_tInfo.vPos = D3DXVECTOR3((float) WINCX / 2, 0.f, 0.f );

	m_bUP = false;
	m_bFasten = false;
	m_bErase = false;

	m_fAlpa = 0.f;
	m_bAlpa = true;

	return S_OK;

}

void CMainLogo::Render( void )
{

	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"MenuLogo"
		,L"MenuLogo");

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

	if(m_bFasten == true)
	{
		D3DXMATRIX matTrans, matScale;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( L"Press"
			,L"Press");

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,WINCX / 2
			,450 
			,0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB((int) m_fAlpa, 255, 255, 255));


	}

	return;

}

int CMainLogo::Update( void )
{
	if(GetAsyncKeyState(VK_SPACE))
		m_bErase = true;

	if(m_bFasten == false)
	{
		if(m_bUP)
		{
			m_tInfo.vPos.y -= 0.05f;

			if(m_tInfo.vPos.y < 200.f)
			{
				m_bFasten = true;
			}
		}

		else
		{
			if(m_tInfo.vPos.y < 250.f)
				m_tInfo.vPos.y += 0.05f;

			else
				m_bUP = true;
				
		}

	}


	else
	{
		if(m_bAlpa)
		{
			m_fAlpa += 300.f * GET_TIME;

			if(m_fAlpa > 255.f)
				m_bAlpa = false;
		}
		else
		{
			m_fAlpa -= 300.f * GET_TIME;

			if(m_fAlpa < 50.f)
				m_bAlpa = true;
		}
	}



	if(m_bErase)
	{
		m_tInfo.vPos.y -= GET_TIME * 300.f;

		if(m_tInfo.vPos.y <= -50)
		{
			CUI* pPan = new CMenuPan;
			pPan->Initialize();
			CUIMgr::GetInstance()->AddGameUI(pPan, UI_ORIGIN);
			return 1;
		}

	}

	return 0;
}

void CMainLogo::Release( void )
{

}
