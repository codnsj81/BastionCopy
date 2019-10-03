#include "StdAfx.h"
#include "GameScroll.h"
#include "Player.h"
#include "TimeMgr.h"
#include "UIMgr.h"
#include "ObjMgr.h"
#include "OriginUI.h"
#include "Factory.h"

CGameScroll::CGameScroll(void)
{
}

CGameScroll::~CGameScroll(void)
{
}

HRESULT CGameScroll::Initialize( void )
{
	m_playerPos = D3DXVECTOR3(-1782.f + WINCX / 2, -1014.f + WINCY / 2, 0.f);
	m_vScroll = D3DXVECTOR3(-900.f + WINCX / 2, -1700.f + WINCY / 2, 0.f);
	m_tInfo.vDir = m_vScroll - m_playerPos;

	m_iFade = 0.f;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	return S_OK;

}

int CGameScroll::Update( void )
{
	//m_vScroll -= m_tInfo.vDir * 100.f *GET_TIME;	
	m_vScroll -= m_tInfo.vDir * 1.f;	
	
	if(m_iFade < 100.f )
		m_iFade += 20.f * GET_TIME;
	else if( m_iFade < 240.f)
		m_iFade += 35.f * GET_TIME;

	D3DXVECTOR3 m_Dis = m_playerPos - m_vScroll;
	float fDistance = D3DXVec3Length(&m_Dis);
	if(fDistance < 10.f)
	{

		CUI* pOriginUI = new COriginUI;
		pOriginUI->Initialize();

		CUIMgr::GetInstance()->AddGameUI(pOriginUI, UI_ORIGIN);

		m_iFade = 255.f;
		return 1;
	}

	return 0;


}

void CGameScroll::Render( void )
{
	return;

}

void CGameScroll::Release( void )
{
	return;
}
