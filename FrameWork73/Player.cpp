#include "StdAfx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "NarrMgr.h"
#include "Mouse.h"
#include "PlayerHit.h"
#include "EffectBuff.h"
#include "Effect.h"
#include "BackGround.h"
#include "UIMgr.h"
#include "OriginUI.h"
#include "UI.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"
#include "EffectFactory.h"
#include "PlayerBullet.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "PlayerHit.h"
#include "Obstacle.h"

HRESULT CPlayer::Initialize(void)
{

	m_iFade = 0.f;
	m_eRenderType = RENDER_WORLD;

	m_fFrameSpeed = 1.f;

	m_tFrame = FRAME(0.f, 0.f, 5.f, 5.f);

	m_fSpeed = 150.f;
	//m_tFrame = FRAME(0.f,
	//	dynamic_cast CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey),
	//				12.f);

	if(SCENE_STAGE == CSceneMgr::GetInstance()->GetSceneType())
	{

		m_bGun = false;
		m_bHammer = false;

		CDevice::GetInstance()->SetZoom(100.f);
		m_wstrStateKey = L"Lie";
		m_State = STATE_LYING;
		m_tInfo.vPos = D3DXVECTOR3(2395.f , 3560.f, 0.f);
		m_vScroll = D3DXVECTOR3(-2395.f + WINCX / 2, -3560.f + WINCY / 2, 0.f);
	}

	else
	{
		m_bGun = true;
		m_bHammer = true;

		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		m_State =STATE_RETURNING;
		m_wstrStateKey = L"StandUp";
		m_tInfo.vPos = D3DXVECTOR3(1782.f , -500.f, 0.f);
		//m_vScroll = D3DXVECTOR3(-1782.f + WINCX / 2, -1014.f + WINCY / 2, 0.f);
		m_bReturning = true;
	}



	m_tInfo.vDir = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);

	D3DXMatrixScaling(&m_matSize, 1.f, 1.f, 1.f);

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 80.f;

	m_wstrObjKey = L"Player";
	m_iHp = 200;
	m_iAtt = 20;
	m_bDrift = false;

	m_bFalling = false;

	bFrameCheck = false;


	return S_OK;
}

int CPlayer::Update(void)
{
	if(m_State == STATE_LYING)
	{
		CDevice::GetInstance()->SetZoomOut(7.f * GET_TIME);
		if(m_iFade < 100.f )
			m_iFade += 10.f * GET_TIME;
		else if( m_iFade < 240.f)
			m_iFade += 25.f * GET_TIME;

		if(GetAsyncKeyState('W') || GetAsyncKeyState('S') ||
			GetAsyncKeyState('A') || GetAsyncKeyState('D'))
		{
			m_State = STATE_WAKE_UP;
			m_wstrStateKey = L"WakeUp";
			m_iFade = 255.f;
			m_tFrame = FRAME(0.f, 0.f, 64.f, 64.f);
			CNarrMgr::GetInstance()->TurnOnNarr(3);
		}

	}
	
	if(m_State == STATE_WAKE_UP && bFrameCheck == true)
	{
		m_State = STATE_WALKING;
		m_tFrame = FRAME(32.f, 32.f, 19.f, 51.f);
	}

	if(m_State == STATE_WALKING || m_State == STATE_STANDING )
	{
		Attack();
		Move();
		DirCheck();
		CollisionObject();

	}

	if(bFrameCheck == true && (m_State == STATE_ATTACK ||
							   m_State == STATE_FAKEATTACK))
	{
		if((m_tFrame.fMax == 63 || m_tFrame.fMax == 75) &&
			CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE)
		{
			CUI* pOriginUI = new COriginUI;
			pOriginUI->Initialize();

			CUIMgr::GetInstance()->AddGameUI(pOriginUI, UI_ORIGIN);
			CNarrMgr::GetInstance()->TurnOnNarr(7);

		}
		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		m_State = STATE_STANDING;
		CDevice::GetInstance()->SetZoomOrigin();
		bFrameCheck = false;

	}

	if(bFrameCheck == false && m_State == STATE_FAKEATTACK)
	{
		CDevice::GetInstance()->SetZoomIn(0.2f);
		//(*m_iZoom) += 10;
	}

	if(m_State == STATE_WALKING)
		CollisionTile();

	if(m_State == STATE_RETURNING)
		Returning();

	if(m_State == STATE_DRINKING)
		Drinking();

	if(m_State == STATE_STAND_UP && bFrameCheck == true)
	{
		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		m_State = STATE_STANDING;
		m_playerDir = DIR_RD;
		CDevice::GetInstance()->SetZoomOrigin();
		m_wstrStateKey = L"Dir_LB";
	}

	MyWorldCheck();
	MyFrameMove();


	if(m_State == STATE_DASH)
		Drift();

	NARRCheck();

	CGameObject::Update();

	return 0;
}


void CPlayer::Render(void)
{

	if(m_State != STATE_FALLING)
	{
		NormalRender();
	}

	else if(m_State == STATE_FALLING)
	{
		FallingRender();
	
	}
	else if(m_State == STATE_RETURNING || m_State == STATE_STAND_UP)
	{
		ReturningRender();
	}

}

void CPlayer::Release(void)
{

}

void CPlayer::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
							m_tInfo.vPos.x + m_vScroll.x,
							m_tInfo.vPos.y + m_vScroll.y,
							0.f);

	m_tInfo.matWorld = matTrans;
}

void CPlayer::MyFrameMove()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME * m_fFrameSpeed ;


	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		bFrameCheck = true;
		m_tFrame.fFrame = m_tFrame.fStart;
	}


	else
		bFrameCheck = false;

}

CPlayer::CPlayer(void)
{
}

CPlayer::~CPlayer(void)
{
	//Release();
}

void CPlayer::DirCheck()
{

	CKeyMgr::GetInstance()->SetKeyState();

	if(m_playerDir == DIR_L || m_playerDir == DIR_R)
		m_wstrStateKey = L"Dir_L";

	if(m_playerDir == DIR_LD || m_playerDir == DIR_RD)
		m_wstrStateKey = L"Dir_LB";


	if(m_playerDir == DIR_LU || m_playerDir == DIR_RU)
		m_wstrStateKey = L"Dir_LT";

	if(m_playerDir == DIR_UP )
		m_wstrStateKey = L"Dir_T";

	if(m_playerDir == DIR_DOWN)
		m_wstrStateKey = L"Dir_B";

	if(m_playerDir == DIR_L)
		m_wstrStateKey = L"Dir_L";

	DWORD keyCheck = CKeyMgr::GetInstance()->GetKeyState();
	
	if(keyCheck == NULL && m_State != STATE_STANDING 
		&& m_State != STATE_ATTACK )
	{
		m_State = STATE_STANDING;
		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
	}
	
	if(m_State != STATE_WALKING &&
	( (keyCheck == KEY_LEFT || keyCheck == KEY_RIGHT ||
		keyCheck == KEY_UP || keyCheck == KEY_DOWN) ))
	{
		m_State = STATE_WALKING;
		m_tFrame = FRAME(32.f, 32.f, 19.f, 51.f);
	}

}

void CPlayer::Attack()
{
	if(GetAsyncKeyState(VK_LBUTTON) && m_State != STATE_ATTACK)
	{
		if(m_bHammer)
		{
			
			HammerAttack();


		}
	}
	if(GetAsyncKeyState(VK_RBUTTON) && m_State != STATE_ATTACK)
	{	
		if(m_bGun)
		{
			GunAttack();

		}
	}

}


void CPlayer::HammerAttack()
{
	m_State = STATE_ATTACK;

	int i = rand() % 2;
	switch(i)
	{
	case 0:
		CSoundMgr::GetInstacne()->PlayEffectSound(L"Hammer01.MP3");
		m_tFrame = FRAME(64.f, 64.f, 11.f ,75.f);
		break;

	case 1:
		CSoundMgr::GetInstacne()->PlayEffectSound(L"Hammer02.MP3");
		m_tFrame = FRAME(52.f, 52.f, 11.f, 63.f);
		break;
	}

	if(1 > CObjMgr::GetInstacne()->GetObject(OBJ_PLAYERHIT)->size())
	{
		CGameObject* pHit = new CPlayerHit(m_tInfo, m_iAtt, m_playerDir);
		pHit->Initialize();
		CObjMgr::GetInstacne()->AddGameObject(pHit, OBJ_PLAYERHIT);
	}
}

void CPlayer::Move()
{
	if(GetAsyncKeyState(VK_SPACE) && m_bDrift == false)
	{
		m_tFrame = FRAME(76.f, 76.f, 12.f, 88.f);
		m_bDrift = true;
		m_State = STATE_DASH;
		m_fFrameSpeed = 2.f;
	}

	if(m_bDrift == false)
	{
		m_fFrameSpeed  = 1.f;
		if(GetAsyncKeyState('A'))
		{
			if(!GetAsyncKeyState('W') && !GetAsyncKeyState('S'))
			{
				m_vScroll.x += GET_TIME * m_fSpeed;
				m_tInfo.vPos.x -= GET_TIME * m_fSpeed;
				m_playerDir = DIR_L;
			}

			if(GetAsyncKeyState('W'))
			{

				m_vScroll.x -= cos(35.f) *GET_TIME * m_fSpeed;
				m_vScroll.y += -sin(35.f) * GET_TIME * m_fSpeed;

				m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed;
				m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed;

				m_playerDir = DIR_LU;
			}

			if(GetAsyncKeyState('S'))
			{

				m_vScroll.x -= cos(35.f) *GET_TIME * m_fSpeed;
				m_vScroll.y -= -sin(35.f) * GET_TIME * m_fSpeed;

				m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed;
				m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed;
				m_playerDir = DIR_LD;
			}
			return;

		}


		if(GetAsyncKeyState('D'))
		{	
			if(!GetAsyncKeyState('W') && !GetAsyncKeyState('S'))
			{
				m_vScroll.x -= GET_TIME * m_fSpeed;

				m_tInfo.vPos.x += GET_TIME * m_fSpeed;
				m_playerDir = DIR_R;
			}


			if(GetAsyncKeyState('W'))
			{
				m_vScroll.x += cos(35.f) *GET_TIME * m_fSpeed;
				m_vScroll.y += -sin(35.f) * GET_TIME * m_fSpeed;

				m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed;
				m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed;

				m_playerDir = DIR_RU;
			}

			if(GetAsyncKeyState('S'))
			{

				m_vScroll.x += cos(35.f) *GET_TIME * m_fSpeed;
				m_vScroll.y -= -sin(35.f) * GET_TIME * m_fSpeed;

				m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed;
				m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed;
				m_playerDir = DIR_RD;
			} 
			return;
		}

		if(GetAsyncKeyState('W'))
		{	
			m_vScroll.y += GET_TIME * m_fSpeed;
			m_tInfo.vPos.y -= GET_TIME * m_fSpeed;
			m_playerDir = DIR_UP;
		}
		if(GetAsyncKeyState('S'))
		{	
			m_vScroll.y -= GET_TIME * m_fSpeed;
			m_playerDir = DIR_DOWN;
			m_tInfo.vPos.y += GET_TIME * m_fSpeed;
		}

	}

}

void CPlayer::GunAttack()
{

	m_State = STATE_ATTACK;
	
	m_tFrame = FRAME(89.f, 89.f, 11.f ,100.f);

	m_vecGun = CMouse::GetMousePos();
	m_vecGun = D3DXVECTOR3(m_vecGun.x - m_vScroll.x,
		m_vecGun.y - m_vScroll.y , 0.f);

	m_tInfo.vDir = m_vecGun- m_tInfo.vPos;

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	float fCos = D3DXVec3Dot(&m_tInfo.vLook, &m_tInfo.vDir);

	float m_fGunAngle = acosf( fCos );

	if(m_tInfo.vPos.y < m_vecGun.y)
		m_fGunAngle = 2 * D3DX_PI - m_fGunAngle;

	m_fGunAngle *= 180.f / 3.14f;


	if(m_fGunAngle  < 22.5f || m_fGunAngle >=  337.5f)
	{
		m_playerDir = DIR_R;
	}

	if(m_fGunAngle  < 67.5f && m_fGunAngle >=  22.5f)
	{
		m_playerDir = DIR_RU;
	}

	if(m_fGunAngle  < 112.5f && m_fGunAngle >=  67.5f)
	{
		m_playerDir = DIR_UP;
	}

	if(m_fGunAngle  < 157.5f && m_fGunAngle >=  112.5f)
	{
		m_playerDir = DIR_LU;
	}

	if(m_fGunAngle  < 202.5f && m_fGunAngle >=  157.5f)
	{
		m_playerDir = DIR_L;
	}

	if(m_fGunAngle  < 247.5f && m_fGunAngle >=  202.5f)
	{
		m_playerDir = DIR_LD;
	}

	if(m_fGunAngle  < 292.5f && m_fGunAngle >=  247.5f)
	{
		m_playerDir = DIR_DOWN;
	}

	if(m_fGunAngle  < 337.5f && m_fGunAngle >=  292.5f)
	{
		m_playerDir = DIR_RD;
	}


	if(2 > CObjMgr::GetInstacne()->GetObject(OBJ_PLAERBULLET)->size())
	{
		CGameObject* pBullet = new CPlayerBullet(m_tInfo, m_iAtt, m_fGunAngle);
		pBullet->Initialize();
		CObjMgr::GetInstacne()->AddGameObject(pBullet, OBJ_PLAERBULLET);
	}

}

void CPlayer::Drift()
{

	if(m_playerDir == DIR_UP)
	{
		m_tInfo.vPos.y -= GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y += GET_TIME * m_fSpeed * 1.5f ;
	}

	if(m_playerDir == DIR_DOWN)
	{
		m_tInfo.vPos.y += GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y -= GET_TIME * m_fSpeed* 1.5f ;
	}

	if(m_playerDir == DIR_L)
	{
		m_vScroll.x += GET_TIME * m_fSpeed * 1.5f;
		m_tInfo.vPos.x -= GET_TIME * m_fSpeed * 1.5f;
	}

	if(m_playerDir == DIR_R)
	{
		m_vScroll.x -= GET_TIME * m_fSpeed* 1.5f ;
		m_tInfo.vPos.x += GET_TIME * m_fSpeed * 1.5f;
	}

	if(m_playerDir == DIR_LD)
	{
		m_vScroll.x -= cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y -= -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

		m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

	}

	if(m_playerDir == DIR_RD)
	{

		m_vScroll.x += cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y -= -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

		m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;
	}

	if(m_playerDir == DIR_LU)
	{
		m_vScroll.x -= cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y += -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

		m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

	}
	if(m_playerDir == DIR_RU)
	{
		m_vScroll.x += cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_vScroll.y += -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

		m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed * 1.5f;
		m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed * 1.5f;

		m_playerDir = DIR_RU;

	}


	if(bFrameCheck == true && m_State == STATE_DASH)
	{
		m_State = STATE_STANDING;
		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		m_bDrift = false;
	}



}

void CPlayer::CollisionObject( void )
{


	OBJITER iter_begin = CObjMgr::GetInstacne()->GetList()[OBJ_OBSTACLE].begin();
	OBJITER iter_end = CObjMgr::GetInstacne()->GetList()[OBJ_OBSTACLE].end();
	float fDist;
	D3DXVECTOR3 vPos;

	RECT rc = {0};
	for(iter_begin; iter_begin != iter_end; ++iter_begin)
	{
		vPos = (*iter_begin)->GetInfo()->vPos;
	
		if(vPos.x + m_vScroll.x < 0 ||
			vPos.x + m_vScroll.x > WINCX ||
			vPos.y + m_vScroll.y < 0 ||
			vPos.y + m_vScroll.y > WINCY)
			continue;

		for(int i=0; i<3; i++)
		{
			HitBoxData pHitBoxData = dynamic_cast<CObstacle*>(*iter_begin)->GetHitBox(i);
			
			for(int j =0; j <2 ; j++)
			{
				RECT pRect  = pHitBoxData.RectHitBox[j];

				const D3DXVECTOR3 pSourceInfo = pHitBoxData.vHitBoxPos; 

				if(IntersectRect(&rc, &m_tRect, &pRect ) == TRUE)
				{
					//Rect구조체를 다시 크기를 설정해주는 함수.
					SetRect(&rc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

					//pDestInfo : 충돌 주인공
					//pSourceInfo : 충돌 대상(상대)-> 지형
					if(m_tInfo.vPos.y < pSourceInfo.y)		//위쪽, 위 왼쪽, 위 오른쪽
					{
						if(rc.right > rc.bottom)		//위쪽에서 충돌했다.
						{	
							if(j == 0)
							{
								fDist = pSourceInfo.y - pHitBoxData.RectfCY - m_tInfo.fCY/2.f ;
								m_vScroll.y -= fDist - m_tInfo.vPos.y;
								m_tInfo.vPos.y = fDist;
							}
							else
							{
								fDist = pSourceInfo.y - pHitBoxData.RectfCY2 - m_tInfo.fCY/2.f ;
								m_vScroll.y -= fDist - m_tInfo.vPos.y;
								m_tInfo.vPos.y = fDist;
							}

							continue;
						}
					}
					else		//아래, 아 왼쪽, 아 오른쪽
					{
						if(rc.right > rc.bottom)		//아래쪽에서 충돌했다.
						{	
							if(j == 0)
							{
								fDist = pSourceInfo.y + pHitBoxData.RectfCY + m_tInfo.fCY /2.f;
								m_vScroll.y -= fDist - m_tInfo.vPos.y;
								m_tInfo.vPos.y = fDist;
							}
							else
							{
								fDist = pSourceInfo.y + pHitBoxData.RectfCY2 + m_tInfo.fCY/2.f ;
								m_vScroll.y -= fDist - m_tInfo.vPos.y;
								m_tInfo.vPos.y = fDist;
							}


							continue;
						}
					}

					//공통검사를 하단에서 처리를 하자.
					if(m_tInfo.vPos.x > pSourceInfo.x)		//오른쪽
					{
						if(j == 0)
						{
							fDist = pSourceInfo.x + pHitBoxData.RectfCX + m_tInfo.fCX/2.f ;
							m_vScroll.x -= fDist - m_tInfo.vPos.x;
							m_tInfo.vPos.x = fDist;
						}
						else
						{
							fDist = pSourceInfo.x + pHitBoxData.RectfCX2 + m_tInfo.fCX/2.f ;
							m_vScroll.x -= fDist - m_tInfo.vPos.x;
							m_tInfo.vPos.x = fDist;
						}
					}
					else		//왼쪽
					{
						if(j == 0)
						{
							fDist = pSourceInfo.x - pHitBoxData.RectfCX - m_tInfo.fCX/2.f ;
							m_vScroll.x -= fDist - m_tInfo.vPos.x;
							m_tInfo.vPos.x = fDist;
						}
						else
						{
							fDist = pSourceInfo.x - pHitBoxData.RectfCX2 - m_tInfo.fCX/2.f ;
							m_vScroll.x -= fDist - m_tInfo.vPos.x;
							m_tInfo.vPos.x = fDist;
						}
					}

				}

			}
		}
	}


}

void CPlayer::CollisionTile()
{
	CGameObject* pBack = CObjMgr::GetInstacne()->GetObject(OBJ_BACKGROUND)->front();

	m_bFalling = dynamic_cast<CBackGround*> (pBack)->CollisionPlayer() ;
	
	if(m_bFalling)
	{		
		m_wstrStateKey = L"Drop";
		m_tFrame = FRAME(0.f, 0.f, 30.f, 30.f);
		m_iHp -= 20.f;
		m_State = STATE_FALLING;
		CNarrMgr::GetInstance()->TurnOnNarr(23);
	}
}

void CPlayer::NormalRender( void )
{
	D3DXMATRIX matTrans, matScale;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,(int)m_tFrame.fFrame);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	if(m_State != STATE_LYING && m_State != STATE_DRINKING && m_State != STATE_WAKE_UP)
	{
		if(m_playerDir == DIR_R || m_playerDir == DIR_RD || m_playerDir == DIR_RU)
		{
			D3DXMatrixScaling(&matScale , -1.f, 1.f, 1.f);
			m_tInfo.matWorld = matScale * m_tInfo.matWorld ;

		}

	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, (int) m_iFade,(int) m_iFade,(int) m_iFade));


	//TCHAR szBuf[MIN_STR] = L"";

	//wsprintf(szBuf, L"(%d,%d)",(int) m_tInfo.vPos.x, (int) m_tInfo.vPos.y);

	//CDevice::GetInstance()->GetFont()->DrawTextW(
	//	CDevice::GetInstance()->GetSprite(),
	//	szBuf,
	//	lstrlen(szBuf),
	//	NULL,
	//	NULL,
	//	D3DCOLOR_ARGB(255, 255, 255, 255));



}

void CPlayer::FallingRender( void )
{
	float fSize = m_matSize._11;
	D3DXMATRIX matTrans;

	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,(int)m_tFrame.fFrame);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
							m_tInfo.vPos.x + m_vScroll.x,
							m_tInfo.vPos.y + m_vScroll.y + m_tInfo.fCX,
							0.f);

	D3DXMatrixScaling(&m_matSize, fSize - 0.004f ,fSize + 0.04f, 1.f);

	m_tInfo.matWorld = m_matSize * matTrans ;


	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	if(m_matSize._11 < 0)
	{
		m_State =STATE_RETURNING;
		m_vUpPos = m_tInfo.vPos;
		m_wstrStateKey = L"StandUp";

		for(int i=0; i<9100; i++)
		{
			CGameObject* Tile = CObjMgr::GetInstacne()->GetObject(OBJ_BACKGROUND)->front();
			m_vRePos = dynamic_cast<CBackGround*>(Tile)->PlayerDistance2(i);
			if(m_vRePos != D3DXVECTOR3(0.f, 0.f, 0.f))
			{
				m_vRePos = D3DXVECTOR3 (m_vRePos.x, m_vRePos.y - 50, m_vRePos.z);
				m_tInfo.vDir = m_vRePos - m_vUpPos;
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_bReturning = false;
				m_tInfo.vPos.y -= 500;
				break;
			}
		}

		D3DXMatrixScaling(&m_matSize, 1.f, 1.f, 1.f);

	}
}

void CPlayer::Returning( void )
{

	list<CGameObject*>* pScroll = CObjMgr::GetInstacne()->GetObject(OBJ_SCROLL);


	if(m_bReturning == false)
	{

		m_vUpPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 500, m_tInfo.vPos.z);
		float fDistance = D3DXVec3Length(&(m_vRePos - m_vUpPos));

		if(fDistance > 20)
		{
			m_tInfo.vPos += m_tInfo.vDir * 0.5f;
			m_vScroll -= m_tInfo.vDir * 0.5f;
			/*
			m_vScroll.x -= cos(35.f) *GET_TIME * m_fSpeed;
			m_vScroll.y += -sin(35.f) * GET_TIME * m_fSpeed;

			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed;*/

		}

		else
		{
			m_bReturning = true;
		}
	}

	else
	{
		if(SCENE_STAGE == CSceneMgr::GetInstance()->GetSceneType())
			m_tInfo.vPos.y += GET_TIME * m_fSpeed  * 3;
		else
			m_tInfo.vPos.y += 0.6f;

		if(pScroll->begin() == pScroll->end())
		{
			if(m_tInfo.vPos.y >= (-1) *(m_vScroll.y) + WINCY / 2)
			{
				m_State = STATE_STAND_UP;	
				m_tFrame = FRAME(1.f, 1.f, 65.f, 65.f);

			}
		}
		else if(m_tInfo.vPos.y >= (-1) * (-1314.f + WINCY / 2))
		{
			m_State = STATE_STAND_UP;	
			m_tFrame = FRAME(1.f, 1.f, 65.f, 65.f);

		}


	}

}

void CPlayer::ReturningRender( void )
{
	D3DXMATRIX matTrans , matScale;
	const TEX_INFO* pPlayerTex ;

	if(m_State == STATE_FALLING)
	{
		pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,0);
	}
	else 
	{
		pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,(int)m_tFrame.fFrame);
	}


	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	TCHAR szBuf[MIN_STR] = L"";


}

void CPlayer::FakeHammer()
{
	m_State = STATE_FAKEATTACK;

	int i = rand() % 2;
	switch(i)
	{
	case 0:
		CSoundMgr::GetInstacne()->PlayEffectSound(L"Hammer01.MP3");
		m_tFrame = FRAME(64.f, 64.f, 11.f ,75.f);
		break;

	case 1:
		CSoundMgr::GetInstacne()->PlayEffectSound(L"Hammer02.MP3");
		m_tFrame = FRAME(52.f, 52.f, 11.f, 63.f);
		break;
	}

}

void CPlayer::FakeGun()
{
	m_State = STATE_FAKEATTACK;

	m_tFrame = FRAME(89.f, 89.f, 11.f ,100.f);

}

void CPlayer::NARRCheck( void )
{
	if(CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE)
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3(1590.f, 3924.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		float fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CNarrMgr::GetInstance()->TurnOnNarr(5);
			return;
		}

		vPos = D3DXVECTOR3(2951.f, 2541.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CNarrMgr::GetInstance()->TurnOnNarr(14);

			return;
		}



		vPos = D3DXVECTOR3(2209.f, 3754.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CNarrMgr::GetInstance()->TurnOnNarr(4);

			return;
		}




		vPos = D3DXVECTOR3(1874.f, 3063.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CNarrMgr::GetInstance()->TurnOnNarr(9);

			return;
		}


		vPos = D3DXVECTOR3(3318.f, 2356.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CNarrMgr::GetInstance()->TurnOnNarr(10);

			return;
		}


		vPos = D3DXVECTOR3(5352.f, 1387.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		 fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE2);

			return;
		}


	}


	else if(CSceneMgr::GetInstance()->GetSceneType() == SCENE_STAGE2)
	{

		D3DXVECTOR3 vPos = D3DXVECTOR3(3378.f, 2181.f, 0.f);
		vPos = m_tInfo.vPos - vPos;
		float fDistance = D3DXVec3Length(&vPos);
		if(fDistance < 50)
		{
			CGameObject* pBack = CObjMgr::GetInstacne()->GetObject(OBJ_BACKGROUND)->front();
			dynamic_cast<CBackGround*>(pBack)->SetOption3();
			CSoundMgr::GetInstacne()->PlayBGMSound(L"Escape.mp3");
			m_iAtt += 20;
			return;
		}

	}
}

void CPlayer::Drinking()
{
	if(m_iHp<200)
	{
		m_iHp += 1;
	}
	if(bFrameCheck == true)
	{
		m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		m_State = STATE_STANDING;
		
	}

}

void CPlayer::DrinkingSet()
{
	m_State = STATE_DRINKING;
	m_wstrStateKey = L"Drinking";
	m_tFrame = FRAME(0.f, 0.f, 22.f, 22.f);

	CNarrMgr::GetInstance()->TurnOnNarr(18);
	CSoundMgr::GetInstacne()->PlayEffectSound(L"Gulp.mp3");


	FRAME pFrame = FRAME(0.f, 0.f, 23.f, 23.f);
	CGameObject* pEffect = new CEffect(m_tInfo, L"Potion", pFrame);
	pEffect->Initialize();
	CObjMgr::GetInstacne()->AddGameObject(pEffect, OBJ_EFFECT);

}

