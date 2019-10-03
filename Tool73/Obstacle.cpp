#include "StdAfx.h"
#include "Obstacle.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Tool73View.h"
#include "MainFrm.h"

CObstacle::CObstacle(void)
{
}

CObstacle::~CObstacle(void)
{
	Release();
}

HRESULT CObstacle::Initialize( void )
{
	return S_OK;
}

int CObstacle::Update( void )
{
	return 0;

}

void CObstacle::Render( void )
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
	const TEX_INFO* pObjTexture;


	for(size_t iIndex = 0; iIndex<m_vecObject.size(); iIndex++)
	{
		pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Object", L"Object", m_vecObject[iIndex]->byDrawID);

		float fcx = pObjTexture->ImageInfo.Width / 2.f;
		float fcy = pObjTexture->ImageInfo.Height / 2.f;


		D3DXMatrixTranslation(&matTrans
			,m_vecObject[iIndex]->vPos.x- m_pView->GetScrollPos(0) 
			,m_vecObject[iIndex]->vPos.y- m_pView->GetScrollPos(1) 
			,m_vecObject[iIndex]->vPos.z );



		matWorld = m_vecObject[iIndex]->matSize * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);


		if(m_vecObject[iIndex]->bTemp == false)
		{
			switch(m_vecObject[iIndex]->byOption)
			{
			case 0:
				CDevice::GetInstance()->GetSprite()->Draw(
					pObjTexture->pTexture,
					NULL,
					&D3DXVECTOR3( fcx, fcy, 0.f),
					NULL,
					D3DCOLOR_ARGB(162, 255, 255, 255));
				break;
			case 1:

				CDevice::GetInstance()->GetSprite()->Draw(
					pObjTexture->pTexture,
					NULL,
					&D3DXVECTOR3( fcx, fcy, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			}
		}

		else
			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 0, 255, 255));
	}

	HitBoxRender();
}

void CObstacle::Release( void )
{

	for(size_t i =0; i< m_vecObject.size(); i++)
		safe_delete(m_vecObject[i]);

	m_vecObject.clear();
}

int CObstacle::GetObstacleIndex( const D3DXVECTOR3& vPos )
{

	for(size_t i = 0; i < m_vecObject.size(); ++i)
	{
		if( Picking(vPos, i) )
		{
			return i;
		}
	}

	return -1;
}

bool CObstacle::Picking( const D3DXVECTOR3& vPos, const int& iIndex )
{
	size_t i =0;
	if(vPos.x > m_vecObject[iIndex]->HitBox.left &&
		vPos.x < m_vecObject[iIndex]->HitBox.right &&
		vPos.y > m_vecObject[iIndex]->HitBox.top &&
		vPos.y < m_vecObject[iIndex]->HitBox.bottom)
		return true;

	return false;

	
}

void CObstacle::HitBoxRender(void)
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
	const TEX_INFO* pObjTexture;



	for(size_t iIndex = 0; iIndex <m_vecObject.size(); iIndex++)
	{

		for(int i =0; i<3; i++)
		{	
			pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"HitBox0", L"HitBox0");

			float fcx = pObjTexture->ImageInfo.Width / 2.f;
			float fcy = pObjTexture->ImageInfo.Height / 2.f;


			D3DXMatrixTranslation(&matTrans
				,m_vecObject[iIndex]->rHitBox[i].vHitBoxPos.x- m_pView->GetScrollPos(0) 
				,m_vecObject[iIndex]->rHitBox[i].vHitBoxPos.y- m_pView->GetScrollPos(1) 
				,m_vecObject[iIndex]->rHitBox[i].vHitBoxPos.z );


			D3DXMatrixScaling(&matScale, m_vecObject[iIndex]->rHitBox[i].RectfCX / 35.f
				,m_vecObject[iIndex]->rHitBox[i].RectfCY/ 15.f , 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				CDevice::GetInstance()->GetSprite()->Draw(
				pObjTexture->pTexture,
				NULL,
				&D3DXVECTOR3( fcx, fcy, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));


				pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"HitBox1", L"HitBox1");

				fcx = pObjTexture->ImageInfo.Width / 2.f;
				fcy = pObjTexture->ImageInfo.Height / 2.f;
				
				D3DXMatrixScaling(&matScale,m_vecObject[iIndex]->rHitBox[i].RectfCX2 / 15.f
					,m_vecObject[iIndex]->rHitBox[i].RectfCY2 / 25.f , 0.f);

				matWorld = matScale * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				CDevice::GetInstance()->GetSprite()->Draw(
					pObjTexture->pTexture,
					NULL,
					&D3DXVECTOR3( fcx, fcy, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			
		}

	}
}
