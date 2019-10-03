#include "StdAfx.h"
#include "Loading.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "Include.h"
#include "Device.h"
CLoading::CLoading(void)
{
	memset(&m_Crt, 0, sizeof(CRITICAL_SECTION));
}

CLoading::~CLoading(void)
{
}

HRESULT CLoading::Initialize( void )
{
	m_fAngle = 0;
	CTextureMgr::GetInstance()->
		InsertTexture(L"../Texture/LoadingBar.png", L"BAR", TEX_SINGLE);

	CTextureMgr::GetInstance()->
		InsertTexture(L"../Texture/BackGround/BlackBack/BlackBack.png", L"Black", TEX_SINGLE);


	CTextureMgr::GetInstance()->
		InsertTexture(L"../Texture/Menu/MenuDeco.png", L"Deco", TEX_SINGLE);


	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(
		NULL
		,0
		,ImgLoadThreadFunc
		,this
		,0
		,NULL);

	return S_OK;
}

int CLoading::Update( void )
{

	if(GetAsyncKeyState(VK_RETURN))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);
	}

	return 0;
}

void CLoading::Release( void )
{

	return;
}

void CLoading::Render( void )
{

	//##배경
	D3DXMATRIX matScale, matTrans, matRot,  matWorld;
	const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Black");

	if(pTexture == NULL)
		return;

	D3DXMatrixScaling(&matScale
		, 1.f
		, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matTrans = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTexture->pTexture
		,NULL
		,NULL
		,NULL
		,D3DCOLOR_ARGB(255, 255, 255, 255));

	// 바

	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Deco");

	if(pTexture == NULL)
		return;

	float fcx = pTexture->ImageInfo.Width / 2.f;
	float fcy = pTexture->ImageInfo.Height / 2.f;

	D3DXMatrixScaling(&matScale
		, 1.f
		, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 800.f, 400.f, 0.f);

	D3DXMatrixRotationZ(&matRot, m_fAngle);

	matWorld = matScale* matRot * matTrans;

	m_fAngle += 0.003f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTexture->pTexture
		,NULL
		,&D3DXVECTOR3( fcx, fcy, 0.f)
		,NULL
		,D3DCOLOR_ARGB(255, 255, 255, 255));


	//##기존 Text출력부분.

	D3DXMatrixTranslation(&matTrans
		, 100.f, 500.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite()
		,CTextureMgr::GetInstance()->GetCurrentPath().c_str()
		,lstrlen(CTextureMgr::GetInstance()->GetCurrentPath().c_str())
		,NULL
		,NULL
		,D3DCOLOR_ARGB(255, 255, 255, 255));
	//Text출력부분.
}

void CLoading::RenderUI( void )
{

	return;
}

unsigned int __stdcall CLoading::ImgLoadThreadFunc( void* pArg )
{

	CLoading* pLogo = (CLoading*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	//스테이지에 있던 리소스 로딩 코드들을 다 들고 온다.


	if(FAILED( CTextureMgr::GetInstance()->
		ReadImagePath(L"../Data/ImagePath.txt") ))
	{
		ERROR_MSG(L"MultiTexture Load Failed!");
		return E_FAIL;
	}

	int iA = 0;

	while(true)
	{
		++iA;

		if(iA > 999999)
			break;

		wstring strPath2 = L"Loading..";
		CTextureMgr::GetInstance()->SetCurrentPath(strPath2);
	}


	//이부분은 좀있다가.
	wstring strPath = L"Load Complete(Press ENTER)";
	CTextureMgr::GetInstance()->SetCurrentPath(strPath);

	LeaveCriticalSection(&pLogo->GetCrt());
	_endthreadex(0);

	return 0;
}

CRITICAL_SECTION CLoading::GetCrt( void )
{

	return m_Crt;
}
