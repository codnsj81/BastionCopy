#include "StdAfx.h"
#include "MyMenu.h"
#include "UI.h"
#include "MouseUI.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "SoundMgr.h"
#include "MainLogo.h"
#include "ParticleMenu.h"
#include "UIMgr.h"
#include "Device.h"
#include "Factory.h"
#include "ObjMgr.h"
CMyMenu::CMyMenu(void)
{
	CSoundMgr::GetInstacne()->LoadSoundFile();
}

CMyMenu::~CMyMenu(void)
{
	Release();
}

HRESULT CMyMenu::Initialize( void )
{


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Menu/MenuBack.png", L"BackGround", TEX_SINGLE, L"BackGround")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Menu/MenuLogo.png", L"MenuLogo", TEX_SINGLE, L"MenuLogo")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Menu/PressSpace.png", L"Press", TEX_SINGLE, L"Press")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Menu/MenuPan.png", L"MenuPan", TEX_SINGLE, L"MenuPan")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/MenuText/NewGame/NewGame%d.png", L"NewGame", TEX_MULTI, L"NewGame", 2)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}	


	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CMouseUI>::Create(),
		OBJ_MOUSE);

	
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/MenuText/ExitGame/ExitGame%d.png", L"ExitGame", TEX_MULTI, L"ExitGame", 2)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}




	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Menu/MenuBoy.png", L"MenuBoy", TEX_SINGLE, L"MenuBoy")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Effect/Bandibul/%d.png", L"Effect", TEX_MULTI, L"Bandibul", 3)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/UI/Mouse/Default/%d.png", L"Mouse", TEX_MULTI, L"Default", 3)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}

	CSoundMgr::GetInstacne()->PlayBGMSound(L"LogoBGM.mp3");

	CUI* pMainLogo = new CMainLogo;
	pMainLogo->Initialize();
	CUIMgr::GetInstance()->AddGameUI(pMainLogo, UI_ORIGIN);

	CObjMgr::GetInstacne()->AddGameObject(
		CFactory<CParticleMenu>::Create(),
		OBJ_PARTICLE);


	return S_OK;

}

int CMyMenu::Update( void )
{

	CUIMgr::GetInstance()->Update();
	CObjMgr::GetInstacne()->Update();
	return 0;
}

void CMyMenu::Render( void )
{
	BackRender();

	CUIMgr::GetInstance()->Render();
	CObjMgr::GetInstacne()->Render();

	return;
}

void CMyMenu::Release( void )
{
	CObjMgr::GetInstacne()->Release();
	CTextureMgr::GetInstance()->Release();
	CUIMgr::GetInstance()->Release();
	return;
}

void CMyMenu::BackRender( void )
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

void CMyMenu::RenderUI( void )
{

}
