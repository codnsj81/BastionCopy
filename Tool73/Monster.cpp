#include "StdAfx.h"
#include "Monster.h"
#include "Texture.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Tool73View.h"

CMonster::CMonster(void)
{
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Initialize( void )
{

	return S_OK;
}

int CMonster::Update( void )
{
	return 0;
}

void CMonster::Render( void )
{
	//boss

	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;

	const TEX_INFO* pObjTexture;

	for(size_t iIndex=0; iIndex<m_vecBoss.size(); iIndex++)
	{	
		pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Boss", L"Boss");

		float fcx = pObjTexture->ImageInfo.Width / 2.f;
		float fcy = pObjTexture->ImageInfo.Height / 2.f;


		D3DXMatrixTranslation(&matTrans
			,m_vecBoss[iIndex]->tInfo.vPos.x- m_pView->GetScrollPos(0) 
			,m_vecBoss[iIndex]->tInfo.vPos.y- m_pView->GetScrollPos(1) 
			,m_vecBoss[iIndex]->tInfo.vPos.z );


		matWorld = matTrans;



		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);


		if(m_vecBoss[iIndex]->bFloating == false)

			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		else
			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(130, 255, 255, 255));

	}


	//monster1

	for(size_t iIndex=0; iIndex<m_vecMonster1.size(); iIndex++)
	{
		pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Monster1", L"Monster1");

		float fcx = pObjTexture->ImageInfo.Width / 2.f;
		float fcy = pObjTexture->ImageInfo.Height / 2.f;


		D3DXMatrixTranslation(&matTrans
			,m_vecMonster1[iIndex]->tInfo.vPos.x- m_pView->GetScrollPos(0) 
			,m_vecMonster1[iIndex]->tInfo.vPos.y- m_pView->GetScrollPos(1) 
			,m_vecMonster1[iIndex]->tInfo.vPos.z );


		matWorld = matTrans;


		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);


		if(m_vecMonster1[iIndex]->bFloating == false)

			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		else
			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(130, 255, 255, 255));
	

	}
	
	//monster2;
	for(size_t iIndex=0; iIndex<m_VecMonster2.size(); iIndex++)
	{

		pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Monster2", L"Monster2");

		float fcx = pObjTexture->ImageInfo.Width / 2.f;
		float fcy = pObjTexture->ImageInfo.Height / 2.f;


		D3DXMatrixTranslation(&matTrans
			,m_VecMonster2[iIndex]->tInfo.vPos.x- m_pView->GetScrollPos(0) 
			,m_VecMonster2[iIndex]->tInfo.vPos.y- m_pView->GetScrollPos(1) 
			,m_VecMonster2[iIndex]->tInfo.vPos.z );



		matWorld = matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);


		if(m_VecMonster2[iIndex]->bFloating == false)

			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		else
			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(130, 255, 255, 255));

	}

}

void CMonster::Release( void )
{
	for(size_t i =0 ; i< m_vecBoss.size(); i++)
		safe_delete(m_vecBoss[i]);

	m_vecBoss.clear();

	for(size_t i =0 ; i< m_vecMonster1.size(); i++)
		safe_delete(m_vecMonster1[i]);

	m_vecMonster1.clear();


	for(size_t i =0; i< m_VecMonster2.size(); i++)
		safe_delete(m_VecMonster2[i]);

	m_VecMonster2.clear();



}
