#include "StdAfx.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Logo.h"
#include "TimeMgr.h"
#include "SceneMgr.h"

HRESULT CLogo::Initialize(void)
{
	ImageLoad();
	m_bCompanyRender = false;
	m_bLogoRender = true;
	m_fAlpa = 0.f;
	m_bAlpa = true;

	return S_OK;
}

int CLogo::Update(void)
{
	return 0;
}

void CLogo::Render(void)
{

	BackRender();


	if(m_bAlpa)
	{
		m_fAlpa += 70.f * GET_TIME;
		if(m_fAlpa > 255.f)
		{
			m_bAlpa = false;
		}
	}
	else
	{
		m_fAlpa -= 70.f * GET_TIME;
		if(m_fAlpa <0.f)
		{
			m_bAlpa = true;

			if(m_bCompanyRender)
			{
				CSceneMgr::GetInstance()->SetChangeScene(SCENE_MENU);
			}
			if(m_bLogoRender)
			{
				m_bCompanyRender = true;
				m_bLogoRender = false;
			}

		}
	}

	if(m_bLogoRender)
		LogoRender();

	if(m_bCompanyRender)
		CompanyRender();

}

void CLogo::Release(void)
{
	CTextureMgr::GetInstance()->Release();

}

CLogo::CLogo(void)
{
}

CLogo::~CLogo(void)
{
	Release();
}

void CLogo::RenderUI(void)
{

}

HRESULT CLogo::ImageLoad( void )
{
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/MainMenu/LogoBackGround.png", L"BackGround", TEX_SINGLE, L"BackGround")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/MainMenu/LogoCompany.png", L"Company", TEX_SINGLE, L"Company")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/MainMenu/LogoMark.png", L"LogoMark", TEX_SINGLE, L"LogoMark")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}

	return S_OK;

}

void CLogo::BackRender( void )
{

	D3DXMATRIX matTrans;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"BackGround"
		,L"BackGround");

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
		(float) WINCX / 2 ,
		(float) WINCY / 2,
		0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CLogo::LogoRender( void )
{

	D3DXMATRIX matTrans, matScale, matWorld;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"LogoMark"
		,L"LogoMark");

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
		(float) WINCX / 2 ,
		(float) WINCY / 2,
		0.f);

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.f);
	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB((int) m_fAlpa, 255, 255, 255));

}

void CLogo::CompanyRender( void )
{

	D3DXMATRIX matTrans, matScale, matWorld;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"Company"
		,L"Company");

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
		(float) WINCX / 2 ,
		(float) WINCY / 2,
		0.f);

	D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
	matWorld = matScale * matTrans;


	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB((int) m_fAlpa, 255, 255, 255));

}
