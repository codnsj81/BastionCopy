#include "StdAfx.h"
#include "Boss.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "PlayerHit.h"
#include "Player.h"
#include "BackGround.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
CBoss::CBoss(void)
{
}

CBoss::CBoss(UNIT_DATA _data)
{
	m_tInfo.vPos = _data.tInfo.vPos;
	m_iHp = _data.iHp;
	m_iAtt = _data.iAttack;
}

CBoss::~CBoss(void)
{
	Release();
}

HRESULT CBoss::Initialize(void)
{
	m_iHp = 120;
	m_ftime = 0.f;
	m_eRenderType = RENDER_WORLD;
	m_tFrame = FRAME(0.f, 0.f, 10.f, 10.f );

	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vDir = D3DXVECTOR3(1.f, 1.f, 1.f);

	m_wstrObjKey = L"Boss";
	m_wstrStateKey = L"Dir_B";

	m_bAttack = false;
	m_bFloating = false;

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;

	m_fSpeed = 30.f;
	m_bCanDamage = false;
	GetPlayer();
	m_eState  =  MSTATE_WAITING;

	CGameObject* pVec = CObjMgr::GetInstacne()->GetObject(OBJ_BACKGROUND)->front();
	m_VecTile = dynamic_cast<CBackGround*>(pVec)->GetTile();

	return S_OK;

}

int CBoss::Update(void)
{
	Progress();
	DirCheck();
	MyFrameMove();

	CGameObject::Update();

	if(m_iHp < 0)
	{
		FRAME pFrame = FRAME(0.f, 0.f, 5.f, 5.f);	
		CGameObject* pEffect = new CEffect(m_tInfo, L"Shot", pFrame);
		pEffect->Initialize();
		CObjMgr::GetInstacne()->AddGameObject(pEffect, OBJ_EFFECT);

		CSoundMgr::GetInstacne()->PlayMonsterSound(L"BossDead.MP3");

		return 1;
	}

	return 0;
}

void CBoss::Render(void)
{
	D3DXMATRIX matTrans, matScale, matRotZ, matWorld;
	const TEX_INFO* pObjTexture;

	pObjTexture = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_wstrStateKey.c_str()
		,(int)m_tFrame.fFrame);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	if(m_eDir == DIR_L || m_eDir == DIR_LD || m_eDir == DIR_LU)
		D3DXMatrixScaling(&matScale , -1.f, 1.f, 1.f);


	if(pObjTexture == NULL)
		return ;


	float fcx = pObjTexture->ImageInfo.Width / 2.f;
	float fcy = pObjTexture->ImageInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans
		,m_tInfo.vPos.x + m_vScroll.x 
		,m_tInfo.vPos.y + m_vScroll.y
		,m_tInfo.vPos.z );


	matWorld = matScale * matTrans;


	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
		pObjTexture->pTexture,
		NULL,
		&D3DXVECTOR3( fcx, fcy, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));



	//HDC hdc = GetDC(g_hWnd);
	//Rectangle(hdc,
	//	static_cast<int> (hdc,m_tRect.left + m_vScroll.x), 
	//	static_cast<int> (m_tRect.top + m_vScroll.y) ,
	//	static_cast<int> (m_tRect.right + m_vScroll.x),
	//	static_cast<int> (m_tRect.bottom + m_vScroll.y));

	return;

}

void CBoss::Release(void)
{
	for (int i = 3043; i < 4583; i++)
	{
		if((*m_VecTile)[i]->byOption == 0 &&
			(*m_VecTile)[i]->bCollision == false)
		{
			(*m_VecTile)[i]->bChoosing = false;
			(*m_VecTile)[i]->bCollision = true;
			(*m_VecTile)[i]->bFloating = false;
			(*m_VecTile)[i]->vShowingPos = D3DXVECTOR3((*m_VecTile)[i]->vPos.x, (*m_VecTile)[i]->vPos.y + 200, 0.f); 
			D3DXMatrixScaling(&(*m_VecTile)[i]->matSize, 0.0f, 0.0f, 1.f);
			(*m_VecTile)[i]->bCollision = true;
			(*m_VecTile)[i]->byOption = 1;

		}
	}
	return;
}

void CBoss::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;
	return;
}

void CBoss::MyFrameMove()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME ;

	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		m_tFrame.fFrame = m_tFrame.fStart;
	}
	return;
}

void CBoss::Damage(int iAtt)
{
	m_iHp -= 10;

	return;
}

void CBoss::Progress()
{
	DWORD dwTime = GetTickCount();

	PlayerDistance();

	CollisionHit();

	if(m_eState == MSTATE_CHASING)
	{ 
		if(dwTime > m_dwTime + 6000)
		{
			CSoundMgr::GetInstacne()->PlayMonsterSound(L"BossJump.MP3");

			m_eState= MSTATE_JUMPING;
			m_fOriginY = m_tInfo.vPos.y;
			m_dwTime = GetTickCount();
		}
		else
			PlayerChasing();
	}
	if(m_eState == MSTATE_ATTACK)
		PlayerAttack();


	if(m_eState == MSTATE_JUMPING)
		Jumping();
	
	if(m_eState == MSTATE_LANDING)
		Landing();


}

void CBoss::DirCheck()
{

	if(m_eDir == DIR_L || m_eDir == DIR_R)
		m_wstrStateKey = L"Dir_R";

	if(m_eDir == DIR_LD || m_eDir == DIR_RD)
		m_wstrStateKey = L"Dir_RB";


	if(m_eDir == DIR_LU || m_eDir == DIR_RU)
		m_wstrStateKey = L"Dir_RT";

	if(m_eDir == DIR_UP )
		m_wstrStateKey = L"Dir_T";

	if(m_eDir == DIR_DOWN)
		m_wstrStateKey = L"Dir_B";

}

void CBoss::PlayerDistance()
{
	m_vPlayerPos = m_pPlayer->GetInfo()->vPos;


	D3DXVECTOR3 vDis = m_tInfo.vPos - m_vPlayerPos;
	m_fDistance = D3DXVec3Length(&vDis);

	if(m_fDistance < 400 && m_eState == MSTATE_WAITING)
	{
		CSoundMgr::GetInstacne()->PlayMonsterSound(L"BossHello.MP3");

		m_dwTime = GetTickCount();
		m_bFloating = true;
		m_eState = MSTATE_CHASING;
	}

	if(m_fDistance < 120 && m_eState == MSTATE_CHASING)
	{
		m_eState = MSTATE_ATTACK;
		m_vAttackPos = m_tInfo.vPos;
		m_bCanDamage = true;
		CSoundMgr::GetInstacne()->PlayMonsterSound(L"BossAtt.MP3");

	}

}

void CBoss::PlayerChasing()
{
	m_tInfo.vDir = m_vPlayerPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	float fCos = D3DXVec3Dot(&m_tInfo.vLook, &m_tInfo.vDir);
	fCos = acos(fCos);

	if(m_tInfo.vPos.y < m_vPlayerPos.y)
		fCos =  2* D3DX_PI - fCos;

	fCos *= 180.f / 3.14f;

	m_tInfo.vPos.x += cosf(fCos/(180.f/3.14f)) * m_fSpeed * GET_TIME;
	m_tInfo.vPos.y += -sinf(fCos/(180.f / 3.14f)) * m_fSpeed * GET_TIME;


	if(fCos  < 22.5f || fCos >=  337.5f)
	{
		m_eDir = DIR_R;
		return;
	}

	if(fCos  < 67.5f && fCos >=  22.5f)
	{
		m_eDir = DIR_RU;
		return;
	}

	if(fCos  < 112.5f && fCos >=  67.5f)
	{
		m_eDir = DIR_UP;
		return;
	}

	if(fCos  < 157.5f && fCos >=  112.5f)
	{
		m_eDir = DIR_LU;
		return;
	}

	if(fCos  < 202.5f && fCos >=  157.5f)
	{
		m_eDir = DIR_L;
		return;
	}

	if(fCos  < 247.5f && fCos >=  202.5f)
	{
		m_eDir = DIR_LD;
		return;
	}

	if(fCos  < 292.5f && fCos >=  247.5f)
	{
		m_eDir = DIR_DOWN;
		return;
	}

	if(fCos  < 337.5f && fCos >=  292.5f)
	{
		m_eDir = DIR_RD;
		return;
	}


}

void CBoss::PlayerAttack()
{

	if(m_bAttack == false)
	{
		if(m_eDir == DIR_RU)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed * 4.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed* 4.f;

		}

		if(m_eDir == DIR_RD)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed* 4.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed* 4.f;
		}			

		if(m_eDir == DIR_LU)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed * 4.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed* 4.f;
		}



		if(m_eDir == DIR_LD)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed* 4.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed* 4.f;
		}		


		if(m_eDir == DIR_UP)
		{
			m_tInfo.vPos.y += GET_TIME * m_fSpeed* 4.f;
		}

		if(m_eDir == DIR_DOWN)
		{
			m_tInfo.vPos.y -= GET_TIME * m_fSpeed * 4.f;
		}	

		if(m_eDir == DIR_L)
		{
			m_tInfo.vPos.x += GET_TIME * m_fSpeed* 4.f;
		}

		if(m_eDir == DIR_R)
		{
			m_tInfo.vPos.x -= GET_TIME * m_fSpeed * 4.f;
		}

	}


	if(m_bAttack == true)
	{

		if(m_eDir == DIR_RU)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed *  6.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed*  6.f;

		}

		if(m_eDir == DIR_RD)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed*   6.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed*   6.f;
		}			

		if(m_eDir == DIR_LU)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed *   6.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed*  6.f;
		}



		if(m_eDir == DIR_LD)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed*   6.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed*   6.f;
		}		


		if(m_eDir == DIR_UP)
		{
			m_tInfo.vPos.y -= GET_TIME * m_fSpeed*   6.f;
		}

		if(m_eDir == DIR_DOWN)
		{
			m_tInfo.vPos.y += GET_TIME * m_fSpeed *  6.f;
		}	

		if(m_eDir == DIR_L)
		{
			m_tInfo.vPos.x -= GET_TIME * m_fSpeed*   6.f;
		}

		if(m_eDir == DIR_R)
		{
			m_tInfo.vPos.x += GET_TIME * m_fSpeed *  6.f;
		}

		CollisionPlayer();

	}

	float fDistance;
	D3DXVECTOR3 vDis = m_tInfo.vPos - m_vAttackPos;
	fDistance = D3DXVec3Length(&vDis);

	if(fDistance > 50 && m_bAttack == false)
	{
		m_bCanDamage = true;
		m_bAttack = true;
	}
	if(fDistance > 70 && m_bAttack == true)
	{
		m_bAttack = false;
		m_eState = MSTATE_CHASING;
	}
}

void CBoss::CollisionHit()
{
	RECT rc = {0};
	for( 
		list<CGameObject*>	::iterator iter = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYERHIT)->begin();
		iter != CObjMgr::GetInstacne()->GetObject(OBJ_PLAYERHIT)->end(); ++iter)
	{
		if(IntersectRect(&rc, &m_tRect, &(*iter)->GetRect()))
		{
			if(dynamic_cast<CPlayerHit*>(*iter)->bHit == false)
			{
				m_iHp -= (*iter)->GetAtt();
				cout<<m_iHp<<endl;
			}

			dynamic_cast<CPlayerHit*>(*iter)->bHit = true;

		}
	}

}

void CBoss::CollisionPlayer()
{
	RECT rc = {0};
	CGameObject* pPlayer = CObjMgr::GetInstacne()->GetObject(OBJ_PLAYER)->front();

	if(IntersectRect(&rc, &m_tRect, &pPlayer->GetRect()))
	{
		if(m_bCanDamage)
			dynamic_cast<CPlayer*> (pPlayer)->Damage(m_iAtt);

		m_bCanDamage = false;
	}

}

void CBoss::Jumping()
{

	float fY = float( m_tInfo.vPos.y +( 30.f * -1 + (0.98 * m_ftime * m_ftime) / 2) );
	m_ftime += 0.5f;

	if(fY > m_fOriginY)
	{
		m_ftime = 0.f;
		m_eState = MSTATE_LANDING;
		return;
	}

	m_tInfo.vPos.y = fY;
}

void CBoss::Landing()
{
	CGameObject* pBack = CObjMgr::GetInstacne()->GetObject(OBJ_BACKGROUND)->front();

	for (int i = 3043; i < 4583; i++)
	{
		bool bResult = dynamic_cast<CBackGround*>(pBack)->Picking(m_tInfo.vPos, i);
		if(bResult)
		{
			(*m_VecTile)[i+ 30]->byOption = 0;
			(*m_VecTile)[i]->byOption = 0;
			(*m_VecTile)[i - 10]->byOption = 0;
			(*m_VecTile)[i - 2]->byOption = 0;
			(*m_VecTile)[i + 2]->byOption = 0;
			(*m_VecTile)[i + 100]->byOption = 0;
			(*m_VecTile)[i - 988]->byOption = 0;
			(*m_VecTile)[i - 141]->byOption = 0;

			(*m_VecTile)[i+ 30]->bCollision = false;
			(*m_VecTile)[i]->bCollision = false;
			(*m_VecTile)[i - 10]->bCollision = false;
			(*m_VecTile)[i - 2]->bCollision = false;
			(*m_VecTile)[i + 2]->bCollision = false;
			(*m_VecTile)[i + 100]->bCollision = false;
			(*m_VecTile)[i - 988]->bCollision = false;
			(*m_VecTile)[i - 141]->bCollision = false;

			break;
		}
	}

	m_eState = MSTATE_CHASING;
}
