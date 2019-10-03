#include "StdAfx.h"
#include "Stage2.h"
#include "Factory.h"
#include "TextureMgr.h"
#include "Potal.h"
#include "Player.h"
#include "NarrMgr.h"
#include "Particle.h"
#include "Monster1_2.h"
#include "Monster2.h"
#include "Boss.h"
#include "UIMgr.h"
#include "SoundMgr.h"
#include "Fond.h"
#include "Gun.h"
#include "MouseUI.h"
#include "Hammer.h"
#include "ObjMgr.h"
#include "OriginUI.h"
#include "Device.h"
#include "BackGround.h"
#include "GameScroll.h"
#include "Obstacle.h"

CStage2::CStage2(void)
{
}

CStage2::~CStage2(void)
{
	Release();
}

HRESULT CStage2::Initialize( void )
{


	
	m_bMon = false;

/////////// 다만들면 지울것
	CNarrMgr::GetInstance()->Initialize();


	CSoundMgr::GetInstacne()->LoadSoundFile();

	if( FAILED(CTextureMgr::GetInstance()
		->ReadImagePath(L"../Data/ImagePath.txt")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/UI/SubTitle/SubTitle.png", L"SubTitle", TEX_SINGLE, L"SubTitle")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}




////////////

	m_dwMonsterTime = GetTickCount();



	//CSoundMgr::GetInstacne()->LoadSoundFile();

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Potal.png", L"Potal", TEX_SINGLE, L"Potal")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;

	}



	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CPlayer>::Create(),
		OBJ_PLAYER);

	CObjMgr::GetInstacne()->AddGameObject(
		CFactory<CParticle>::Create(),
		OBJ_PARTICLE);

	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CMouseUI>::Create(),
		OBJ_MOUSE);


	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CGameScroll>::Create(),
		OBJ_SCROLL);

	CObjMgr::GetInstacne()->AddGameObject( CFactory<CBackGround>::Create(), 
		OBJ_BACKGROUND );

	CObjMgr::GetInstacne()->AddGameObject(CFactory<CFond>::Create(),
		OBJ_FOND);


	CObjMgr::GetInstacne()->AddGameObject(CFactory<CPotal>::Create(),
		OBJ_POTAL);



	UnitLoad();
	ObjectLoad();
	CSoundMgr::GetInstacne()->PlayBGMSound(L"Stage2BGM.MP3");
	CNarrMgr::GetInstance()->TurnOnNarr(20);



	return S_OK;
}

void CStage2::Render( void )
{
	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);

	D3DXMatrixTranslation(&matTrans
		,0
		,0
		,0);

	const TEX_INFO* pTileTexture 
		= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Default", 1);

	matWorld = matScale * matTrans;

	float fcx = pTileTexture->ImageInfo.Width / 2.f;
	float fcy = pTileTexture->ImageInfo.Height / 2.f;


	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fcx, fcy, 0.f),
		NULL,	
		D3DCOLOR_ARGB(255,(int) CGameObject::m_iFade ,
		(int) CGameObject::m_iFade,
		(int) CGameObject::m_iFade));


	CObjMgr::GetInstacne()->Render();
	CNarrMgr::GetInstance()->Render();
	return;

}

void CStage2::RenderUI( void )
{
	CUIMgr::GetInstance()->Render();


	return;
}

void CStage2::Release( void )
{
	//CSoundMgr::GetInstacne()->Release();
	CObjMgr::GetInstacne()->Release();
	CUIMgr::GetInstance()->Release();
	return;

}

void CStage2::UnitLoad( void )
{
	list<CGameObject*>* pBossData = new list<CGameObject*>;
	list<CGameObject*>* pMosnter1Data = new list<CGameObject*>;
	list<CGameObject*>* pMonster2Data = new list<CGameObject*>;


	HANDLE hFile = CreateFile( L"..\\Data\\UnitData2.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;
	int iSize;


	pUNIT_DATA pUnitData;
	CGameObject* pObj;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pUnitData = new UNIT_DATA; 
		ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
		pObj = new CBoss(*pUnitData);
		pObj->Initialize();
		pBossData->push_back(pObj);

		safe_delete(pUnitData);
	}

	CObjMgr::GetInstacne()->SetList(pBossData,OBJ_BOSS);

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pUnitData = new UNIT_DATA; 
		ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
		pObj = new CMonster1_2(*pUnitData);
		pObj->Initialize();
		m_pMosnter1Data.push_back(pObj);
	}
	//CObjMgr::GetInstacne()->SetList(pMosnter1Data, OBJ_MONSTER1_STAGE2);

	m_iter = m_pMosnter1Data.begin();


	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pUnitData = new UNIT_DATA; 
		ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
		pObj = new CMonster2(*pUnitData);
		pObj->Initialize();
		pMonster2Data->push_back(pObj);
	}

	CObjMgr::GetInstacne()->SetList(pMonster2Data, OBJ_MONSTER2);


	CloseHandle(hFile);

}

void CStage2::ObjectLoad( void )
{

	list<CGameObject*>* pObstacleData = new list<CGameObject*>;

	DWORD dwBype = 0;
	HANDLE hFile = CreateFile( L"..\\Data\\ObjectData2.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;
	int iSize;
	pOBJECT_DATA pData;
	CGameObject* pObj;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pData = new OBJECT_DATA; 

		ReadFile(hFile, pData, sizeof(OBJECT_DATA), &dwByte, NULL);
		pObj = new CObstacle(*pData);
		pObj->Initialize();
		pObstacleData->push_back(pObj);
		pObj = NULL;

	}
	CObjMgr::GetInstacne()->SetList(pObstacleData, OBJ_OBSTACLE);



	CloseHandle(hFile);
}

int CStage2::Update( void )
{
	MonsterInit();
	CObjMgr::GetInstacne()->Update();
	CNarrMgr::GetInstance()->Update();

	return 0;
}

void CStage2::MonsterInit( void )
{
	DWORD dwTime = GetTickCount();

	if(m_bMon == false && dwTime > m_dwMonsterTime + 15000)
	{
		(*m_iter)->Initialize();
		CObjMgr::GetInstacne()->AddGameObject((*m_iter),OBJ_MONSTER1_STAGE2);
		m_iter++;
		m_bMon = true;
		m_dwMonsterTime2 = GetTickCount();
	}
	if(m_bMon == true && dwTime > m_dwMonsterTime2 + 200)
	{
		if(m_iter == m_pMosnter1Data.end())
			return;
		else
		{
			m_dwMonsterTime2 = dwTime;
			(*m_iter)->Initialize();
			CObjMgr::GetInstacne()->AddGameObject((*m_iter),OBJ_MONSTER1_STAGE2);
			m_iter++;

		}
	}

}
