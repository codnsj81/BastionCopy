#include "StdAfx.h"
#include "MenuPan.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Mouse.h"
#include "SceneMgr.h"

CMenuPan::CMenuPan(void)
{
}

CMenuPan::~CMenuPan(void)
{
}

HRESULT CMenuPan::Initialize( void )
{
	m_bFasten= false;
	m_tInfo.vPos = D3DXVECTOR3((float)WINCX / 2, 900 , 0.f);
	m_tBoy = D3DXVECTOR3(200, 900, 0.f);

	m_Button[0].vPos = D3DXVECTOR3((float)WINCX / 2 ,  300.f, 0.f);
	m_Button[1].vPos = D3DXVECTOR3((float)WINCX / 2 ,  350.f, 0.f);

	m_Button[0].fCX = 200.f;
	m_Button[0].fCY = 40.f;

	m_Button[1].fCX = 200.f;
	m_Button[1].fCY = 40.f;

	m_Button[0].bMouse = false;
	m_Button[1].bMouse = false;

	for(int i= 0 ; i<2 ; i++)
	{
		m_Button[i].tRect.left = LONG(m_Button[i].vPos.x - (m_Button[i].fCX / 2.f)) ;
		m_Button[i].tRect.right = LONG(m_Button[i].vPos.x + (m_Button[i].fCX / 2.f)) ;
		m_Button[i].tRect.top = LONG(m_Button[i].vPos.y - (m_Button[i].fCY/ 2.f));
		m_Button[i].tRect.bottom = LONG(m_Button[i].vPos.y + (m_Button[i].fCY / 2.f));

	}
	

	m_fBoySpeed = 0.f;


	return S_OK;
}

int CMenuPan::Update(void)
{

	if(m_bFasten == false)
	{
		m_tInfo.vPos.y -= GET_TIME * 400.f;
		m_tBoy.y -= GET_TIME *  400.f;
		if(m_tInfo.vPos.y <=(float) WINCY / 2.f)
		{
			m_bFasten = true;
		}
	}

	if(m_bFasten)
		ButtonClick();

	BoyMove();

	return 0;

}

void CMenuPan::Release( void )
{

	return;
}

void CMenuPan::Render( void )
{
	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"MenuPan"
		,L"MenuPan");

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,m_tInfo.vPos.x 
		,m_tInfo.vPos.y 
		,m_tInfo.vPos.z );

	D3DXMatrixScaling(&matScale , 1.f, 1.f, 0.f);

	matScale *= matTrans;
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matScale);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	BoyRender();
	
	if(m_bFasten == true)
		MenuRender();

	return;

}

void CMenuPan::BoyMove()
{
	m_fBoySpeed -= 1 * GET_TIME;

	if(m_bBoyDir == true)
	{
		m_tBoy.y -= -sin(m_fBoySpeed) * GET_TIME * 30.f ;
		if(m_fBoySpeed < 0.f)
		{
			m_bBoyDir = false;
			m_fBoySpeed = 45.f;
		}
	}
	
	else
	{
		m_tBoy.y +=-sin(m_fBoySpeed) * GET_TIME * 30.f ;
		if(m_fBoySpeed < 0.f)
		{
			m_bBoyDir = true;
			m_fBoySpeed = 45.f;
		}
	}

}

void CMenuPan::BoyRender()
{
	D3DXMATRIX matTrans, matScale;

	//BOY
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( L"MenuBoy"
		,L"MenuBoy");

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,m_tBoy.x 
		,m_tBoy.y 
		,m_tBoy.z );

	D3DXMatrixScaling(&matScale , 0.75f, 0.75f, 0.f);

	matScale *= matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matScale);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMenuPan::MenuRender()
{


	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex;
	
	if(m_Button[0].bMouse == false)
	pPlayerTex= 
		CTextureMgr::GetInstance()->GetTexture( L"NewGame"
		,L"NewGame" , 0);
	else	
		pPlayerTex= 
		CTextureMgr::GetInstance()->GetTexture( L"NewGame"
		,L"NewGame" , 1);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,m_Button[0].vPos.x 
		,m_Button[0].vPos.y 
		,m_Button[0].vPos.z );

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	matScale *= matTrans;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matScale);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));




	if(m_Button[1].bMouse == false)
		pPlayerTex= 
		CTextureMgr::GetInstance()->GetTexture( L"ExitGame"
		,L"ExitGame" , 0);
	else
		pPlayerTex= 
		CTextureMgr::GetInstance()->GetTexture( L"ExitGame"
		,L"ExitGame" , 1);

	if(pPlayerTex == NULL)
		return ;

	fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans
		,m_Button[1].vPos.x 
		,m_Button[1].vPos.y 
		,m_Button[1].vPos.z );

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	matScale *= matTrans;
	CDevice::GetInstance()->GetSprite()->SetTransform(&matScale);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CMenuPan::ButtonClick()
{
	D3DXVECTOR3 vMouse = CMouse::GetMousePos();

	for(int i =0; i<2; i++)
	{
		if(vMouse.x < m_Button[i].tRect.right &&
			vMouse.x > m_Button[i].tRect.left &&
			vMouse.y < m_Button[i].tRect.bottom &&
			vMouse.y > m_Button[i].tRect.top)
		{
			m_Button[i].bMouse = true;
			m_tBoyDir = D3DXVECTOR3(m_tBoy.x , m_Button[i].vPos.y,0.f);
			m_tBoyDir = m_tBoyDir - m_tBoy;
			float fDis = D3DXVec3Length(&m_tBoyDir);

			if(fDis > 10)
			{
				D3DXVec3Normalize(&m_tBoyDir, &m_tBoyDir);

				m_tBoy += m_tBoyDir * GET_TIME * 100.f;
			}

			if(i == 0 && GetAsyncKeyState(VK_LBUTTON))
				CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);

		}
		else
			m_Button[i].bMouse = false;
	}

}
