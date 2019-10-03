#include "StdAfx.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "Logo.h"
#include "Ending.h"
#include "Stage2.h"
#include "MyMenu.h"
#include "Loading.h"
IMPLEMENT_SINGLETON(CSceneMgr);

HRESULT CSceneMgr::Initialize(void)
{
	m_pScene = NULL;

	m_bChangeCheck = false;
	m_eType = SCENE_END;

	return S_OK;
}

int CSceneMgr::Update(void)
{
	if(m_bChangeCheck)
	{
		m_bChangeCheck = false;
		SetScene(m_eType);
	}

	if(m_pScene)
		m_pScene->Update();

	return 0;
}

void CSceneMgr::Render(void)
{
	if(m_pScene)
		m_pScene->Render();
}

void CSceneMgr::Release(void)
{
	safe_delete(m_pScene);
}

void CSceneMgr::SetScene(eSceneType eType)
{
	safe_delete(m_pScene);

	switch (eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;

	case SCENE_STAGE:
		m_pScene = new CStage;
		break;

	case SCENE_MENU:
		m_pScene = new CMyMenu;
		break;

	case SCENE_STAGE2:
		m_pScene = new CStage2;
		break;

	case SCENE_LOADING:
		m_pScene = new CLoading;
		break;

	case SCENE_ENDING:
		m_pScene = new CEnding;
		break;
	}

	m_pScene->Initialize();
}

void CSceneMgr::SetChangeScene(eSceneType eType)
{
	m_bChangeCheck = true;
	m_eType		   = eType;
}

void CSceneMgr::RenderUI(void)
{
	if(m_pScene)
		m_pScene->RenderUI();

}

