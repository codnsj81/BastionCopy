#include "StdAfx.h"
#include "MainGame.h"
#include "Device.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

HRESULT CMainGame::Initialize(void)
{
	//AllocConsole(); 
	//freopen( "CONOUT$",  "wt", stdout);
	SetConsoleTitleA("Debug");
	//_CrtSetBreakAlloc( 11801 );

	m_dwTime = GetTickCount();
	//m_szFPS[128] = ;
	//memset(m_szFPS, 0, sizeof(m_szFPS));
	lstrcpy(m_szFPS, L"");

	m_iFps = 0;

	if( FAILED(m_pDevice->Initialize3D()) )
	{
		ERROR_TEXT(L"Device Init Failed");
		return E_FAIL;
	}

	CTimeMgr::GetInstance()->InitTime();

	CTextureMgr::GetInstance()->m_fBarScale = 0.f;

	CSceneMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE2);

	return S_OK;
}

int CMainGame::Update(void)
{
	CTimeMgr::GetInstance()->SetTime();
	CSceneMgr::GetInstance()->Update();

	return 0;
}

void CMainGame::Render(void)
{
	

	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();
	CSceneMgr::GetInstance()->RenderUI();

	m_pDevice->Render_End();


	//CSceneMgr::GetInstance()->RenderUI();

	//m_pDevice->Render_End2();


	if(m_dwTime + 1000 < GetTickCount())
	{
		m_dwTime = GetTickCount();

		wsprintf(m_szFPS, L"BASTION / FPS : %d", m_iFps);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFps = 0;
	}

	++m_iFps;
	
}

void CMainGame::Release(void)
{
	m_pDevice->Destroy();

	CSceneMgr::GetInstance()->Destroy();
	CTextureMgr::GetInstance()->Destroy();
}

CMainGame::CMainGame(void)
:m_pDevice( CDevice::GetInstance() )
{
}

CMainGame::~CMainGame(void)
{
	Release();
}
