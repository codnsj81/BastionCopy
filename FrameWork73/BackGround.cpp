#include "StdAfx.h"
#include "BackGround.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Player.h"
#include "TimeMgr.h"


CBackGround::CBackGround(void)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

void CBackGround::MapDataLoad()
{
	DWORD dwBype = 0;
	HANDLE hFile = CreateFile( L"..\\Data\\TileData.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;
	int iSize;
	PTILE pTile;


	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		pTile->bChoosing = false;
		pTile->bFloating = false;
		pTile->vShowingPos = D3DXVECTOR3(pTile->vPos.x, pTile->vPos.y + 200, 0.f); 
		pTile->matOriginSize = pTile->matSize;
		D3DXMatrixScaling(&pTile->matSize, 0.0f, 0.0f, 1.f);

		m_vecTile.push_back(pTile);
	}


	hFile = CreateFile( L"..\\Data\\GroundData.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		m_vecGround.push_back(pTile);
	}


	CloseHandle(hFile);
}

HRESULT CBackGround::Initialize(void)
{
	m_bOption3 = false;
	/*if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Tile/Tile%d.png", L"Back", TEX_MULTI, L"Tile", 38)))
	{
		ERROR_TEXT(L"멀티텍스쳐 로드 실패!");
		return E_FAIL;
	}*/

	//m_vScroll = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_eRenderType = RENDER_BACKGROUND;

	m_fSpeed = 200.f;

	m_ptTileRender.x = WINCX / TILECX + 2 ;
	m_ptTileRender.y = (LONG) WINCY / (TILECY / 2) + 2;


	m_pPlayer = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	if(SCENE_STAGE == GetStage())
		MapDataLoad();


	if(SCENE_STAGE2 == GetStage())
		MapDataLoad2();

	return S_OK;
}

int CBackGround::Update(void)
{
	if(m_bOption3 == true)
	{
		for(int i = 4576; i <  8406 ; i++)
		{
			if(m_vecTile[i]->byOption == 3)
			{
				PlayerDistance3(i);

			}	
			if(m_vecTile[i]->byOption == 4)
			{
				Option4Progress(i);

			}
			if(m_vecTile[i]->byOption == 5)
			{
				Option5Progress(i);
			}
		}
	}

	return 0;
}

void CBackGround::Render(void)
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;

	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
   			int iCullX = (int) ((-m_vScroll.x) / (TILECX ));
			int iCullY = (int)((-m_vScroll.y) / (TILECY / 2.f));

			int iIndex = (i + iCullY) * TILEX + (j + iCullX);
			
			if(iIndex < 0 || iIndex >= 9100)
				continue;
			//int iIndex = i * TILEX + j;
			
			if(m_vecTile[iIndex]->byOption != 0)
				PlayerDistance(iIndex);

			else
				continue;

			if(iIndex < 0 ||  iIndex >= TILEX * TILEY)
				continue;		//break;

			if(m_vecTile[iIndex]->bChoosing == true)
				NormalRender(iIndex);

			else if(m_vecTile[iIndex]->bFloating == true)
				FloatingRender(iIndex);

			}
		}



	TCHAR szBuf[MIN_STR] = L"";

	for(size_t iIndex =0; iIndex <m_vecGround.size(); iIndex++)
	{


		const TEX_INFO* pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Ground", m_vecGround[iIndex]->byDrawID);


		float fcx = pTileTexture->ImageInfo.Width / 2.f;
		float fcy = pTileTexture->ImageInfo.Height / 2.f;

		D3DXMatrixTranslation(&matTrans
			,m_vecGround[iIndex]->vPos.x + m_vScroll.x
			,m_vecGround[iIndex]->vPos.y + m_vScroll.y
			,m_vecGround[iIndex]->vPos.z);
		
		//D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

		matWorld = m_vecGround[iIndex]->matSize * matTrans;


		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pTileTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, (int) m_iFade,(int) m_iFade,(int) m_iFade));


	}
	
}

void CBackGround::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();

	for(size_t i =0; i<m_vecGround.size(); ++i)
		safe_delete(m_vecGround[i]);

	m_vecGround.clear();
}

void CBackGround::NormalRender( int iIndex )
{

	D3DXMATRIX matTrans, matWorld;

	const TEX_INFO* pTileTexture;

	pTileTexture 
		= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Tile", m_vecTile[iIndex]->byDrawID);

	D3DXMatrixTranslation(&matTrans
		,m_vecTile[iIndex]->vPos.x + m_vScroll.x - 40 * m_vecTile[iIndex]->matSize._11
		,m_vecTile[iIndex]->vPos.y + m_vScroll.y - 68 * m_vecTile[iIndex]->matSize._11
		,m_vecTile[iIndex]->vPos.z);

	matWorld = m_vecTile[iIndex]->matOriginSize * matTrans;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTexture->pTexture,
		NULL,
		&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	//TCHAR szBuf[MIN_STR] = L"";

	//wsprintf(szBuf, L"%d", iIndex);

	//CDevice::GetInstance()->GetFont()->DrawTextW(
	//	CDevice::GetInstance()->GetSprite(),
	//	szBuf,
	//	lstrlen(szBuf),
	//	NULL,
	//	NULL,
	//	D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CBackGround::PlayerDistance(int iIndex)
{
	if(m_vecTile[iIndex]->byOption != 0)
	{
		float fDistance;
		INFO* pInfo = m_pPlayer->GetInfo();
		D3DXVECTOR3 vDis = m_vecTile[iIndex]->vPos - pInfo->vPos;
		fDistance = D3DXVec3Length(&vDis);

		if(fDistance < 300)
		{
			m_vecTile[iIndex]->bFloating = true;

		}
	}
}

void CBackGround::PlayerDistance3(int iIndex)
{
	if(m_vecTile[iIndex]->byOption == 3 &&
		dynamic_cast<CPlayer*>(m_pPlayer)->GetState() == STATE_WALKING)
	{
		float fDistance;
		INFO* pInfo = m_pPlayer->GetInfo();
		D3DXVECTOR3 vDis = m_vecTile[iIndex]->vPos - pInfo->vPos;
		fDistance = D3DXVec3Length(&vDis);

		if(fDistance < 70)
		{
			m_vecTile[iIndex]->byOption = 4;
			m_vecTile[iIndex]->bCollision = true;
			m_vecTile[iIndex]->vOriginPos = m_vecTile[iIndex]->vPos;
			m_vecTile[iIndex]->dwTime = GetTickCount();
		}

	}
}

D3DXVECTOR3 CBackGround::PlayerDistance2(int iIndex)
{
	if(m_vecTile[iIndex]->byOption == 1 || m_vecTile[iIndex]->byOption == 3)
	{
		float fDistance;
		INFO* pInfo = m_pPlayer->GetInfo();
		D3DXVECTOR3 vDis = m_vecTile[iIndex]->vPos - pInfo->vPos;
		fDistance = D3DXVec3Length(&vDis);

		if(fDistance < 100)
		{
			m_vecTile[iIndex]->bFloating = true;
			return m_vecTile[iIndex]->vPos;

		}

	}

	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CBackGround::FloatingRender( int iIndex )
{

	D3DXMATRIX matTrans, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	float fSize = m_vecTile[iIndex]->matSize._11;


	if(m_vecTile[iIndex]->byOption != 0)
	{
		const TEX_INFO* pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Tile", m_vecTile[iIndex]->byDrawID);

		D3DXMatrixTranslation(&matTrans
			,m_vecTile[iIndex]->vShowingPos.x + m_vScroll.x - 40 * m_vecTile[iIndex]->matSize._11
			,m_vecTile[iIndex]->vShowingPos.y + m_vScroll.y - 68 * m_vecTile[iIndex]->matSize._11
			,m_vecTile[iIndex]->vPos.z);

		D3DXMatrixScaling(&m_vecTile[iIndex]->matSize, fSize + 2.f * GET_TIME ,fSize + 2.f * GET_TIME, 1.f);

		matWorld = m_vecTile[iIndex]->matSize * matTrans;
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pTileTexture->pTexture,
			NULL,
			&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	m_vecTile[iIndex]->vShowingPos.y -= 400.f * GET_TIME;

	if(m_vecTile[iIndex]->vShowingPos.y <= m_vecTile[iIndex]->vPos.y )
		m_vecTile[iIndex]->bChoosing = true;


}

bool CBackGround::CollisionPlayer()
{

	D3DXVECTOR3 footPos = D3DXVECTOR3( m_pPlayer->GetInfo()->vPos.x ,  m_pPlayer->GetInfo()->vPos.y 
		+  m_pPlayer->GetInfo()->fCY / 2, 0.f);


	
	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = (int) ((-m_vScroll.x) / (TILECX ));
			int iCullY = (int)((-m_vScroll.y) / (TILECY / 2.f));

			int iIndex = (i + iCullY) * TILEX + (j + iCullX);

			if(iIndex < 0 || iIndex >= 9100 ||
				m_vecTile[iIndex]->byOption != 0)
				continue;

			if(iIndex < 0 ||  iIndex >= TILEX * TILEY)
				continue;	

			if(Picking(footPos , iIndex) )
			{
				return true;
			}
		}
	}

	return false;

}

bool CBackGround::Picking( const D3DXVECTOR3& vPos, const int& iIndex )
{
	float fSlope = (TILECY / 2.f) / (TILECX / 2.f);

	//포인트를 구한다.
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y - TILECY / 2.f ,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x + TILECX / 2.f, 
		m_vecTile[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y + TILECY / 2.f,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x - TILECX / 2.f, 
		m_vecTile[iIndex]->vPos.y ,
		0.f)
	};



	//방향 벡터
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDir[0].y, -vDir[0].x, 0.f),
		D3DXVECTOR3(vDir[1].y, -vDir[1].x, 0.f),
		D3DXVECTOR3(vDir[2].y, -vDir[2].x, 0.f),
		D3DXVECTOR3(vDir[3].y, -vDir[3].x, 0.f),
	};

	//법선벡터들을 단위벡터로 변경을 한다.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = vPos - vPoint[i];

		float fDot = D3DXVec3Dot(&vTemp, &vNormal[i]);

		if(fDot > 0.f)
			return false;

		//내적 결과가 양수면 타일 외부이고
		//			  음수면 타일 내부이다.
	}

	return true;
}

void CBackGround::MapDataLoad2( void )
{
	DWORD dwBype = 0;
	HANDLE hFile = CreateFile( L"..\\Data\\TileData2.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;
	int iSize;
	PTILE pTile;


	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		pTile->bChoosing = false;
		pTile->bCollision = true;
		pTile->bFloating = false;
		pTile->vShowingPos = D3DXVECTOR3(pTile->vPos.x, pTile->vPos.y + 200, 0.f); 
		pTile->matOriginSize = pTile->matSize;
		D3DXMatrixScaling(&pTile->matSize, 0.0f, 0.0f, 1.f);

		m_vecTile.push_back(pTile);
	}


	hFile = CreateFile( L"..\\Data\\GroundData2.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		m_vecGround.push_back(pTile);
	}


	CloseHandle(hFile);

}

eSceneType CBackGround::GetStage( void )
{

	return	CSceneMgr::GetInstance()->GetSceneType();

}

void CBackGround::SetOption3( void )
{
	if(m_bOption3 == false)
	{
		for(int i = 4576; i <  8406 ; i++)
		{
			if(m_vecTile[i]->byOption != 0)
			{
				m_vecTile[i]->byOption = 3;
			}		
		}
		m_bOption3 = true;
	}

}

void CBackGround::Option4Progress( int iIndex )
{

	DWORD dwTime = GetTickCount();

	if(dwTime > m_vecTile[iIndex]->dwTime + 2000)
	{
		m_vecTile[iIndex]->byOption = 5;
	}

	if(m_vecTile[iIndex]->bCollision == true)
	{
		m_vecTile[iIndex]->vPos.x -= 200.f * GET_TIME;
		if(m_vecTile[iIndex]->vPos.x < m_vecTile[iIndex]->vOriginPos.x - 6.f)
		{
			m_vecTile[iIndex]->bCollision = false;
		}
	}

	else
	{
		m_vecTile[iIndex]->vPos.x += 200.f * GET_TIME;
		if(m_vecTile[iIndex]->vPos.x > m_vecTile[iIndex]->vOriginPos.x + 6.f)
		{
			m_vecTile[iIndex]->bCollision = true;
		}
	}
}

void CBackGround::Option5Progress( int iIndex )
{


	D3DXMATRIX matTrans, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	float fSize = m_vecTile[iIndex]->matSize._11;


	if(m_vecTile[iIndex]->byOption != 0)
	{
		const TEX_INFO* pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"BackGround", L"Tile", m_vecTile[iIndex]->byDrawID);

		D3DXMatrixTranslation(&matTrans
			,m_vecTile[iIndex]->vShowingPos.x + m_vScroll.x - 40 
			,m_vecTile[iIndex]->vShowingPos.y + m_vScroll.y - 68 * m_vecTile[iIndex]->matSize._11
			,m_vecTile[iIndex]->vPos.z);

		D3DXMatrixScaling(&m_vecTile[iIndex]->matSize, fSize - 3.f * GET_TIME ,fSize - 3.f * GET_TIME, 1.f);

		matWorld = m_vecTile[iIndex]->matSize * matTrans;
		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pTileTexture->pTexture,
			NULL,
			&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	m_vecTile[iIndex]->vShowingPos.y += 600.f * GET_TIME;

	if(fSize < 0.f )
		m_vecTile[iIndex]->byOption = 0;
}
