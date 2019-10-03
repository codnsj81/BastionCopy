#include "StdAfx.h"
#include "Ending.h"
#include "Device.h"
#include "TextureMgr.h"
#include "SoundMgr.h"

CEnding::CEnding(void)
{
}

CEnding::~CEnding(void)
{
	Release();
}

HRESULT CEnding::Initialize( void )
{
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Ending.png", L"Ending", TEX_SINGLE, L"Ending")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/EndingText.png", L"EndingText", TEX_SINGLE, L"EndingText")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	CSoundMgr::GetInstacne()->PlayBGMSound(L"LogoBGM.mp3");

	return S_OK;
}

int CEnding::Update( void )
{
	return 0;
}

void CEnding::Release( void )
{

	return;
}

void CEnding::RenderUI( void )
{

}

void CEnding::BackRender( void )
{
	D3DXMATRIX matTrans;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"Ending"
		,L"Ending");

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


	pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"EndingText"
		,L"EndingText");

	if(pPlayerTex == NULL)
		return ;

	fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
		(float) WINCX / 2 ,
		(float) WINCY / 2 + 200,
		0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEnding::Render( void )
{
	BackRender();

}
