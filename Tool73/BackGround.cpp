#include "StdAfx.h"
#include "BackGround.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Tool73View.h"
#include "MyForm.h"
#include "TileTool.h"
#include "MainFrm.h"

HRESULT CBackGround::Initialize(void)
{
	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float fX = j * TILECX + (i % 2) * (TILECX / 2.f);
			float fY = i * (TILECY / 2.f);

			pTile->vPos  = D3DXVECTOR3(fX, fY, 0.f);

			D3DXMatrixScaling(&pTile->matSize, 1.f, 1.f, 1.f);
			pTile->matOriginSize = pTile->matSize;

			pTile->byDrawID = 0;
			pTile->byOption = 0;
			pTile->bChoosing = false;	
			pTile->vShowingPos = pTile->vPos;
			pTile->bCollision = false; 
			pTile->vOriginPos = pTile->vPos;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

int CBackGround::Update(void)
{
	return 0;
}

void CBackGround::TileChange(const D3DXVECTOR3& vPos
							 , const int& iDrawID)
{
	int iIndex = GetTileIndex(vPos);

	if(iIndex == -1)
		return;

	m_vecTile[iIndex]->byDrawID = iDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

void CBackGround::TileChoose( const D3DXVECTOR3& vPos)
{
	int iIndex = GetTileIndex(vPos);

	CTileTool* pTileTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetTileTool();

	if(iIndex == -1)
		return;

	if(m_vecTile[iIndex]->bChoosing == false)
	{
		m_vecTile[iIndex]->bChoosing = true ;
		pTileTool->SetChoosingTile(m_vecTile[iIndex]);
	}

	else
	{
		m_vecTile[iIndex]->bChoosing = false;
		pTileTool->CancelChoosingTile(m_vecTile[iIndex]);

	}

}


int CBackGround::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if( Picking(vPos, i) )
		{
			return i;
		}
	}

	return -1;
}


bool CBackGround::Picking(const D3DXVECTOR3& vPos
						  , const int& iIndex)
{
	float fSlope = (TILECY / 2.f) / (TILECX / 2.f);

	//포인트를 구한다.
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y - TILECY / 2.f * m_vecTile[iIndex]->matSize._11,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x + TILECX / 2.f * m_vecTile[iIndex]->matSize._11, 
		m_vecTile[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y + TILECY / 2.f * m_vecTile[iIndex]->matSize._11,
		0.f),

		D3DXVECTOR3( m_vecTile[iIndex]->vPos.x - TILECX / 2.f * m_vecTile[iIndex]->matSize._11, 
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



void CBackGround::Render(void)
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
	const TEX_INFO* pTileTexture;


	// Gound


	int vecSize = m_vecGround.size();


	for(int index  = 0; index< vecSize; index++)
	{

		const TEX_INFO* pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"Ground", L"Ground", m_vecGround[index]->byDrawID);

		float fcx = pTileTexture->ImageInfo.Width / 2.f * m_vecGround[index]->matSize._11;
		float fcy = pTileTexture->ImageInfo.Height / 2.f* m_vecGround[index]->matSize._11;

		D3DXMatrixTranslation(&matTrans
			,m_vecGround[index]->vPos.x- m_pView->GetScrollPos(0) 
			,m_vecGround[index]->vPos.y- m_pView->GetScrollPos(1) 
			,m_vecGround[index]->vPos.z );

		matWorld = m_vecGround[index]->matSize * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		switch(m_vecGround[index]->byOption)
		{
		case 0:
			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3( fcx, fcy, 0.f),
				NULL,
				D3DCOLOR_ARGB(162, 255, 255, 255));
			break;
		case 1:

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3( fcx, fcy, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}

		D3DXMatrixTranslation(&matTrans
			,m_vecGround[index]->vPos.x- m_pView->GetScrollPos(0) + fcx - TILECX / 2
			,m_vecGround[index]->vPos.y- m_pView->GetScrollPos(1) + fcy - TILECY /2
			,m_vecGround[index]->vPos.z );


		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		matWorld = matScale * matTrans;


		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);


		pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"Blind", L"Tile", 1);


		CDevice::GetInstance()->GetSprite()->Draw(
			pTileTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(162, 255, 255, 255));


	}



	//TILE
	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;


			if(iIndex < 0 ||  iIndex >= TILEX * TILEY)
				continue;		//break;

			if(m_vecTile[iIndex]->byOption == 0)
			{
				 pTileTexture 
					= CTextureMgr::GetInstance()->GetTexture(L"Blind", L"Tile", m_vecTile[iIndex]->byDrawID);
				 
			D3DXMatrixTranslation(&matTrans
									,m_vecTile[iIndex]->vPos.x- m_pView->GetScrollPos(0)
									,m_vecTile[iIndex]->vPos.y- m_pView->GetScrollPos(1)
									,m_vecTile[iIndex]->vPos.z);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			}
			else
			{
				pTileTexture 
					= CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);
			
				float fcx = pTileTexture->ImageInfo.Width / 2.f;
				float fcy = pTileTexture->ImageInfo.Height / 2.f;

			D3DXMatrixTranslation(&matTrans
									,m_vecTile[iIndex]->vPos.x- m_pView->GetScrollPos(0) - 40 * m_vecTile[iIndex]->matSize._11
									,m_vecTile[iIndex]->vPos.y- m_pView->GetScrollPos(1) - 68 * m_vecTile[iIndex]->matSize._11
									,m_vecTile[iIndex]->vPos.z);


			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f) );


			matWorld = m_vecTile[iIndex]->matSize *matRotZ * matTrans;



			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			if(m_vecTile[iIndex]->bChoosing == true)

				CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 0, 0));

			else
				CDevice::GetInstance()->GetSprite()->Draw(
					pTileTexture->pTexture,
					NULL,
					&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));


			}

			//충돌표시

			if(m_vecTile[iIndex]->bCollision == true)
			{

				pTileTexture 
					= CTextureMgr::GetInstance()->GetTexture(L"Option", L"Option", 2);

			D3DXMatrixTranslation(&matTrans
									,m_vecTile[iIndex]->vPos.x- m_pView->GetScrollPos(0)
									,m_vecTile[iIndex]->vPos.y- m_pView->GetScrollPos(1)
									,m_vecTile[iIndex]->vPos.z);




			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f) );

			D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);

			matWorld = matScale *matRotZ * matTrans;



			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
				NULL,
				D3DCOLOR_ARGB(200, 255, 255, 255));

			}

		}

	}

}

void CBackGround::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		//delete m_vecTile[i];
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();
}

void CBackGround::MiniViewRender(void)
{
	
	const TEX_INFO* pTileTexture;
	D3DXMATRIX matTrans, matScale, matWorld;

	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;

			if(iIndex < 0 ||  iIndex >= TILEX * TILEY)
				continue;		//break;


			if(m_vecTile[iIndex]->byOption == 0)
			{
				 pTileTexture 
					= CTextureMgr::GetInstance()->GetTexture(L"Blind", L"Tile", m_vecTile[iIndex]->byDrawID);
			}
			else
			{
				pTileTexture 
					= CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);

			}
			D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.f);

			D3DXMatrixTranslation(&matTrans
				,m_vecTile[iIndex]->vPos.x * 0.4f
				,m_vecTile[iIndex]->vPos.y * 0.4f
				,m_vecTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(TILECX / 2, TILECY / 2, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));


		}
	}

	//Ground


	int vecSize = m_vecGround.size();


	for(int index  = 0; index< vecSize; index++)
	{

		const TEX_INFO* pTileTexture 
			= CTextureMgr::GetInstance()->GetTexture(L"Ground", L"Ground", m_vecGround[index]->byDrawID);

		float fcx = pTileTexture->ImageInfo.Width / 2.f;
		float fcy = pTileTexture->ImageInfo.Height / 2.f;

		D3DXMatrixTranslation(&matTrans
			,m_vecGround[index]->vPos.x *0.4f
			,m_vecGround[index]->vPos.y *0.4f
			,m_vecGround[index]->vPos.z );


		matWorld = m_vecGround[index]->matSize * matTrans;

		D3DXMatrixScaling(&matScale,  m_vecGround[index]->matSize._11 * 0.4f, 
			m_vecGround[index]->matSize._11 * 0.4f, 
			m_vecGround[index]->matSize._11 * 0.4f);


		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		switch(m_vecGround[index]->byOption)
		{
		case 0:
			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3( fcx, fcy, 0.f),
				NULL,
				D3DCOLOR_ARGB(162, 255, 255, 255));
			break;
		case 1:

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3( fcx, fcy, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}

	}



}

CBackGround::CBackGround(void)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

void CBackGround::PushVecGround( PTILE pTile )
{
	m_vecGround.push_back(pTile);

}

bool CBackGround::GroundPicking( const D3DXVECTOR3& vPos, const int& iIndex )
{

	float fSlope = (TILECY / 2.f) / (TILECX / 2.f);

	//포인트를 구한다.
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3( m_vecGround[iIndex]->vPos.x, 
		m_vecGround[iIndex]->vPos.y - TILECY / 2.f * m_vecGround[iIndex]->matSize._11,
		0.f),

		D3DXVECTOR3( m_vecGround[iIndex]->vPos.x + TILECX / 2.f * m_vecGround[iIndex]->matSize._11, 
		m_vecGround[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3( m_vecGround[iIndex]->vPos.x, 
		m_vecGround[iIndex]->vPos.y + TILECY / 2.f * m_vecGround[iIndex]->matSize._11,
		0.f),

		D3DXVECTOR3( m_vecGround[iIndex]->vPos.x - TILECX / 2.f * m_vecGround[iIndex]->matSize._11, 
		m_vecGround[iIndex]->vPos.y ,
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

int CBackGround::GetGroundIndex( const D3DXVECTOR3& vPos )
{

	for(size_t i = 0; i < m_vecGround.size(); ++i)
	{
		if( GroundPicking(vPos, i) )
		{
			return i;
		}
	}

	return -1;
}
