#include "StdAfx.h"
#include "Potal.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Ending.h"
CPotal::CPotal(void)
{
}

CPotal::~CPotal(void)
{
}

HRESULT CPotal::Initialize( void )
{
	m_eRenderType = RENDER_WORLD;

	m_fAngle = 0.f;
	m_pPlayer = CObjMgr ::GetInstacne()->GetObject(OBJ_PLAYER)->front();
	
	m_tInfo.fCX = 1.f;
	m_tInfo.fCY = 1.f;


	m_tInfo.vPos = D3DXVECTOR3(736.f, 3921.f, 0.f);
	CGameObject::Update();
	return S_OK;
}

int CPotal::Update( void )
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		PlayerCollision();

		m_fAngle += 1.f * GET_TIME;

		if(m_fAngle> 360.f)
			m_fAngle = 0.f;
	}

	return 0;
}

void CPotal::Render( void )
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		D3DXMATRIX matTrans, matScale , matRot;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture(L"Potal"
			,L"Potal");

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,m_tInfo.vPos.x + m_vScroll.x
			,m_tInfo.vPos.y + m_vScroll.y
			,m_tInfo.vPos.z );


		D3DXMatrixRotationZ(&matRot, m_fAngle);


		matTrans = matRot * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CPotal::Release( void )
{

}

bool CPotal::PlayerCollision( void )
{
	RECT rc = {0};

	if(IntersectRect(&rc, &m_tRect, &m_pPlayer->GetRect()))
	{
		CSceneMgr::GetInstance()->SetChangeScene(SCENE_ENDING);
		return true;
	}
	else
		return false;

}
