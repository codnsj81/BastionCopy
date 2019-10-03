#include "StdAfx.h"
#include "MouseUI.h"
#include "Mouse.h"
#include "Device.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
CMouseUI::CMouseUI(void)
{
}

CMouseUI::~CMouseUI(void)
{
}

HRESULT CMouseUI::Initialize( void )
{
	m_wstrObjKey = L"Mouse";
	m_wstrStateKey = L"Default";
	m_fRotate = 0.f;
	m_eRenderType = RENDER_UI;

	return S_OK;
}

int CMouseUI::Update( void )
{
	m_tInfo.vPos = CMouse::GetMousePos();
	SetTexture();

	return 0;
}

void CMouseUI::Render( void )
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;

	const TEX_INFO* pObjTexture = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,m_DrawID);

	if(pObjTexture == NULL)
		return ;

	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;

	D3DXMatrixRotationZ(&matRotZ, m_fRotate);


	D3DXMatrixTranslation(&matTrans
		,m_tInfo.vPos.x
		,m_tInfo.vPos.y
		,m_tInfo.vPos.z );

	matWorld = matRotZ *  matTrans;


	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3( fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMouseUI::Release( void )
{

}

void CMouseUI::SetTexture( void )
{
	if(CSceneMgr::GetInstance()->GetSceneType() == SCENE_MENU)
	{
		m_DrawID = 2;
	}
	if(CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE || 
		CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE2)
	{
		if(GetAsyncKeyState(VK_RBUTTON)|| GetAsyncKeyState(VK_LBUTTON))
		{
			m_DrawID = 1;
		}
		else
			m_DrawID = 0;


		m_fRotate += 0.01f;
	}

}

