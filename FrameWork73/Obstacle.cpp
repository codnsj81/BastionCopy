#include "StdAfx.h"
#include "Obstacle.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Device.h"
#include "Effect.h"
#include "PlayerHit.h"
#include "Player.h"

CObstacle::CObstacle(void)
{
}

CObstacle::~CObstacle(void)
{
	Release();
	safe_delete(m_vPlayer);
}

HRESULT CObstacle::Initialize( void )
{
	m_eRenderType = RENDER_WORLD;
	m_bFloating = false;
	m_bChoosing = false;
	m_vShowingPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 300, 0.f);
	m_eState = OBJ_STATE_END;
	m_ObjData.bDestruction = true;
	m_iHp = 60;
	for(int i=0; i< 3; i++)
	{
		RECT pRect1 = {(LONG) (m_ObjData.rHitBox[i].vHitBoxPos.x -m_ObjData.rHitBox[i].RectfCX) ,(LONG) (m_ObjData.rHitBox[i].vHitBoxPos.y -m_ObjData.rHitBox[i].RectfCY),
			(LONG)(  m_ObjData.rHitBox[i].vHitBoxPos.x +m_ObjData.rHitBox[i].RectfCX) , (LONG) (m_ObjData.rHitBox[i].vHitBoxPos.y + m_ObjData.rHitBox[i].RectfCY)};

		m_ObjData.rHitBox[i].RectHitBox[0] = pRect1;

		RECT pRect2 = {(LONG) (m_ObjData.rHitBox[i].vHitBoxPos.x -m_ObjData.rHitBox[i].RectfCX2) ,(LONG) (m_ObjData.rHitBox[i].vHitBoxPos.y -m_ObjData.rHitBox[i].RectfCY2),
			(LONG)(  m_ObjData.rHitBox[i].vHitBoxPos.x +m_ObjData.rHitBox[i].RectfCX2) , (LONG) (m_ObjData.rHitBox[i].vHitBoxPos.y + m_ObjData.rHitBox[i].RectfCY2)};

		m_ObjData.rHitBox[i].RectHitBox[1] = pRect2;
	}

	return S_OK;

}

int CObstacle::Update( void )
{


	if(m_tInfo.vPos.x + m_vScroll.x > - 200 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX + 200 &&
		m_tInfo.vPos.y + m_vScroll.y > -100 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY + 100)
	{
		if(m_eState == OBJ_STATE_END)
			GetPlayerDistance();

		if(m_eState == OBJ_STATE_NORMAL || m_eState == OBJ_STATE_HIT)
			CollisionHit();

		if(m_iHp <= 0 )
		{
			FRAME pFrame = FRAME(0.f, 0.f, 5.f, 5.f);
			CGameObject* pEffect = new CEffect(m_tInfo, L"Break", pFrame);
			pEffect->Initialize();
			CObjMgr::GetInstacne()->AddGameObject(pEffect, OBJ_EFFECT);

			return 1;
		}
	}

	return 0;

}

void CObstacle::Render( void )
{

	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{
		if(m_eState == OBJ_STATE_NORMAL || m_eState == OBJ_STATE_HIT)
			NormalRender();

		else if(m_eState == OBJ_STATE_FALLING)
			FloatingRender();

		else if(m_eState == OBJ_STATE_BOUNDING1)
			BoundingRender1();

		else if(m_eState == OBJ_STATE_BOUDNING2)
			BoundingRender2();
	}

	//HitBoxRender();

	return;
}

void CObstacle::Release( void )
{
	m_vPlayer = NULL;
	return;
}

void CObstacle::LoadObjectData( void )
{

}

void CObstacle::HitBoxRender( void )
{

	HDC hdc = GetDC(g_hWnd);

	for(int i =0; i<3; i++)
	{	
			Rectangle(hdc, 
				static_cast<int> (hdc,m_ObjData.rHitBox[i].RectHitBox[0].left + m_vScroll.x), 
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[0].top + m_vScroll.y) ,
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[0].right + m_vScroll.x),
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[0].bottom + m_vScroll.y));


			Rectangle(hdc, 
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[1].left +   m_vScroll.x), 
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[1].top + m_vScroll.y) ,
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[1].right +  m_vScroll.x),
				static_cast<int> (m_ObjData.rHitBox[i].RectHitBox[1].bottom + m_vScroll.y));


	}

}

void CObstacle::GetPlayerDistance( void )
{
	INFO* pInfo = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYER)->front()->GetInfo();
	D3DXVECTOR3 vDis =  m_tInfo.vPos - pInfo->vPos;
	
	m_fDistance = D3DXVec3Length(&vDis);

	if(m_fDistance < 300)
	{
		m_bFloating = true;
		m_eState =OBJ_STATE_FALLING;
	}
	
}

void CObstacle::NormalRender( void )
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
	const TEX_INFO* pObjTexture;


	pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Object", L"Default", m_ObjData.byDrawID);

	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans
		,m_tInfo.vPos.x + m_vScroll.x
		,m_tInfo.vPos.y + m_vScroll.y
		,m_tInfo.vPos.z );

	matWorld = m_ObjData.matSize * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	if(m_eState == OBJ_STATE_NORMAL)
	CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3( fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	else if( m_eState == OBJ_STATE_HIT)

		CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3( fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 0, 0));

		//HitBoxRender();


}

void CObstacle::FloatingRender( void )
{
	const TEX_INFO* pObjTexture;
	pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Object", L"Default", m_ObjData.byDrawID);

	D3DXMATRIX matTrans, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;

		
	D3DXMatrixTranslation(&matTrans
		,m_vShowingPos.x + m_vScroll.x 
		,m_vShowingPos.y + m_vScroll.y 
		,0.f);
	
	matWorld = m_ObjData.matSize * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));



	m_vShowingPos.y += 800.f * GET_TIME ;

	if(m_vShowingPos.y >= m_tInfo.vPos.y )
	{
		m_bChoosing = true;
		m_eState = OBJ_STATE_BOUNDING1;
	}
}

bool CObstacle::CollisionHit()
{
	RECT rc = {0};
	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYERHIT)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_PLAYERHIT)->end(); ++iter)
	{
		for(int i=0; i< 3; i++)
		{
			if(IntersectRect(&rc, &m_ObjData.rHitBox[i].RectHitBox[0], &(*iter)->GetRect()))
			{
				if(dynamic_cast<CPlayerHit*>(*iter)->bHit == false)
				{
					m_iHp -= (*iter)->GetAtt();
					cout<<m_iHp<<endl;
					dynamic_cast<CPlayerHit*>(*iter)->SetCollision();
					m_eState = OBJ_STATE_HIT;
				}
				

				dynamic_cast<CPlayerHit*>(*iter)->bHit = true;
				return true;

			}

			if(IntersectRect(&rc, &m_ObjData.rHitBox[i].RectHitBox[1], &(*iter)->GetRect()))
			{
				if(dynamic_cast<CPlayerHit*>(*iter)->bHit == false)
				{
					m_iHp -= (*iter)->GetAtt();
					cout<<m_iHp<<endl;
					dynamic_cast<CPlayerHit*>(*iter)->SetCollision();
				}

				dynamic_cast<CPlayerHit*>(*iter)->bHit = true;
				m_eState = OBJ_STATE_HIT;
				return true;

			}

		}
	}

	m_eState = OBJ_STATE_NORMAL;
	return false;

}

void CObstacle::BoundingRender1( void )
{
	const TEX_INFO* pObjTexture;
	pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Object", L"Default", m_ObjData.byDrawID);

	D3DXMATRIX matTrans, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans
		,m_vShowingPos.x + m_vScroll.x 
		,m_vShowingPos.y + m_vScroll.y 
		,0.f);

	matWorld = m_ObjData.matSize * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));



	m_vShowingPos.y -= 500.f * GET_TIME ;

	if(m_vShowingPos.y <= m_tInfo.vPos.y - 20.f )
	{
		m_eState = OBJ_STATE_BOUDNING2;
	}

}

void CObstacle::BoundingRender2( void )
{
	const TEX_INFO* pObjTexture;
	pObjTexture = CTextureMgr::GetInstance()->GetTexture(L"Object", L"Default", m_ObjData.byDrawID);

	D3DXMATRIX matTrans, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans
		,m_vShowingPos.x + m_vScroll.x 
		,m_vShowingPos.y + m_vScroll.y 
		,0.f);

	matWorld = m_ObjData.matSize * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));



	m_vShowingPos.y += 500.f * GET_TIME ;

	if(m_vShowingPos.y >= m_tInfo.vPos.y  )
	{
		m_eState = OBJ_STATE_NORMAL;
	}

}
