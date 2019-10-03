#include "StdAfx.h"
#include "Stage.h"
#include "Factory.h"
#include "TextureMgr.h"
#include "Player.h"
#include "NarrMgr.h"
#include "Particle.h"
#include "Monster1.h"
#include "Monster2.h"
#include "Boss.h"
#include "UIMgr.h"
#include "SoundMgr.h"
#include "Fond.h"
#include "Gun.h"
#include "MouseUI.h"
#include "Hammer.h"
#include "ObjMgr.h"
#include "Device.h"
#include "BackGround.h"
#include "Obstacle.h"

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::Initialize(void)
{
	/*if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Tile/Tile%d.png", L"Back", TEX_MULTI, L"Tile", 38)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}*/

	CNarrMgr::GetInstance()->Initialize();

	if( FAILED(CTextureMgr::GetInstance()
		->ReadImagePath(L"../Data/ImagePath.txt")))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}


	//////////////

	

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/TileOption0.png", L"Blind", TEX_SINGLE, L"Tile")))
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


	///////////////

	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CPlayer>::Create(),
		OBJ_PLAYER);

	//CObjMgr::GetInstacne()->AddGameObject( 
	//	CFactory<CObstacle>::Create(),
	//	OBJ_OBSTACLE);


	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CMouseUI>::Create(),
		OBJ_MOUSE);


	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CHammer>::Create(),
		OBJ_ITEM);


	CObjMgr::GetInstacne()->AddGameObject( 
		CFactory<CGun>::Create(),
		OBJ_ITEM);

	CObjMgr::GetInstacne()->AddGameObject(
		CFactory<CParticle>::Create(),
		OBJ_PARTICLE);

	CObjMgr::GetInstacne()->AddGameObject(
		CFactory<CFond>::Create(),
		OBJ_FOND);

	CObjMgr::GetInstacne()->AddGameObject( CFactory<CBackGround>::Create(), 
		OBJ_BACKGROUND );


	UnitLoad();
	ObjectLoad();


	CSoundMgr::GetInstacne()->PlayBGMSound(L"Stage1BGM.mp3");
	CNarrMgr::GetInstance()->TurnOnNarr(1);

	return S_OK;
}

int CStage::Update(void)
{
	CObjMgr::GetInstacne()->Update();
	CNarrMgr::GetInstance()->Update();

	return 0;
}

void CStage::Render(void)
{

	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);

	D3DXMatrixTranslation(&matTrans
		,0
		,0
		,0);

	const TEX_INFO* pTileTexture 
		= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Default", 0);

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

}

void CStage::Release(void)
{
	CObjMgr::GetInstacne()->Release();
	CUIMgr::GetInstance()->Release();
	return;
}

void CStage::UnitLoad(void)
{
	list<CGameObject*>* pBossData = new list<CGameObject*>;
	list<CGameObject*>* pMosnter1Data = new list<CGameObject*>;
	list<CGameObject*>* pMonster2Data = new list<CGameObject*>;


	HANDLE hFile = CreateFile( L"..\\Data\\UnitData.dat",
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
		pObj = new CMonster1(*pUnitData);
		pObj->Initialize();
		pMosnter1Data->push_back(pObj);
	}
	CObjMgr::GetInstacne()->SetList(pMosnter1Data, OBJ_MONSTER1);


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

void CStage::ObjectLoad( void )
{

	list<CGameObject*>* pObstacleData = new list<CGameObject*>;

	DWORD dwBype = 0;
	HANDLE hFile = CreateFile( L"..\\Data\\ObjectData.dat",
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

void CStage::RenderUI(void)
{
	CUIMgr::GetInstance()->Render();


}
