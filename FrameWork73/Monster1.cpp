#include "StdAfx.h"
#include "Monster1.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "PlayerHit.h"
#include "SceneMgr.h"
#include "Player.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "device.h"
#include "NarrMgr.h"
#include "SoundMgr.h"

CMonster1::CMonster1(void)
{
}

CMonster1::CMonster1(UNIT_DATA _data)
{
	m_tInfo.vPos = _data.tInfo.vPos;
	m_iHp = 60;
	m_iAtt = _data.iAttack;
}

CMonster1::~CMonster1(void)
{
	Release();
}

HRESULT CMonster1::Initialize(void)
{
	m_eRenderType = RENDER_WORLD;

	m_tFrame = FRAME(0.f,0.f, 2.f, 2.f);
	
	m_bAttack = false;

	
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 60.f;

	m_eDir = DIR_L;
	m_eState = MSTATE_WAITING;
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vDir = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_bFloating = false; /// 플레이어 인식 bool
	m_fSpeed = 100.f;
	m_bCanDamage = false;

	m_wstrObjKey = L"Monster1";
	m_wstrStateKey = L"Dir_L";

	GetPlayer();
	return S_OK;

}

int CMonster1::Update(void)
{
	Progress();
	DirCheck();

	CollisionHit();
	//MyWorldCheck();
	//MyFrameMove();
	CGameObject::Update();

	if(m_iHp < 0)
	{

		FRAME pFrame = FRAME(0.f, 0.f, 5.f, 5.f);
		CGameObject* pEffect = new CEffect(m_tInfo, L"Break", pFrame);
		pEffect->Initialize();
		CObjMgr::GetInstacne()->AddGameObject(pEffect, OBJ_EFFECT);
		CNarrMgr::GetInstance()->TurnOnNarr(13);
		
		return 1;

	}
	return 0;

}

void CMonster1::Render(void)
{
	if(m_tInfo.vPos.x + m_vScroll.x > 0 &&
		m_tInfo.vPos.x + m_vScroll.x < WINCX &&
		m_tInfo.vPos.y + m_vScroll.y > 0 &&
		m_tInfo.vPos.y + m_vScroll.y < WINCY)
	{

		D3DXMATRIX matTrans, matScale, matRotZ, matWorld;

		const TEX_INFO* pObjTexture = 
			CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
			,m_wstrStateKey.c_str()
			,0);

		if(pObjTexture == NULL)
			return ;


		float fcx = pObjTexture->ImageInfo.Width / 2.f;
		float fcy = pObjTexture->ImageInfo.Height / 2.f;


		D3DXMatrixTranslation(&matTrans
			,m_tInfo.vPos.x + m_vScroll.x 
			,m_tInfo.vPos.y + m_vScroll.y
			,m_tInfo.vPos.z );

		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 1.f);

		if(m_eDir == DIR_R || m_eDir == DIR_RD || m_eDir == DIR_RU)
			D3DXMatrixScaling(&matScale , -0.5f, 0.5f, 1.f);

		matWorld = matScale * matTrans;


		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		if(m_bAttack)
			CDevice::GetInstance()->GetSprite()->Draw(
			pObjTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fcx, fcy, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 50, 50));

		else
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

	}
	return;
}

void CMonster1::Release(void)
{
	return;

}

void CMonster1::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;
	return;

}

void CMonster1::MyFrameMove()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME * 1.5f;

	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		m_tFrame.fFrame = m_tFrame.fStart;
	}
	return;

}

void CMonster1::Progress()
{	
	PlayerDistance();

	if(m_eState == MSTATE_CHASING)
		PlayerChasing();

	if(m_eState == MSTATE_ATTACK)
		PlayerAttack();

	
}

void CMonster1::PlayerDistance()
{
	//거리 계산
	m_vPlayerPos = m_pPlayer->GetInfo()->vPos;


	D3DXVECTOR3 vDis = m_tInfo.vPos - m_vPlayerPos;
	m_fDistance = D3DXVec3Length(&vDis);

	if(m_fDistance < 400 &&
		SCENE_STAGE == CSceneMgr::GetInstance()->GetSceneType())
	{
		CNarrMgr::GetInstance()->TurnOnNarr(11);

		if(m_fDistance<200)
			CNarrMgr::GetInstance()->TurnOnNarr(12);

	}

	if(m_fDistance < 70 && m_eState != MSTATE_ATTACK)
	{
		m_bFloating = true;
		m_eState = MSTATE_CHASING;
	}

	if(m_fDistance < 30 && m_eState == MSTATE_CHASING)
	{
		m_eState = MSTATE_ATTACK;
		m_vAttackPos = m_tInfo.vPos;
		m_bCanDamage = true;
		CSoundMgr::GetInstacne()->PlayMonsterSound(L"Monster1Att.MP3");

	}
}

void CMonster1::PlayerChasing()
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

void CMonster1::PlayerAttack()
{
	if(m_bAttack == false)
	{
		if(m_eDir == DIR_RU)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed * 1.1f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed* 1.1f;

		}

		if(m_eDir == DIR_RD)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed* 1.1f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed* 1.1f;
		}			

		if(m_eDir == DIR_LU)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed * 1.1f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed* 1.1f;
		}



		if(m_eDir == DIR_LD)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed* 1.1f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed* 1.1f;
		}		


		if(m_eDir == DIR_UP)
		{
			m_tInfo.vPos.y += GET_TIME * m_fSpeed* 1.1f;
		}

		if(m_eDir == DIR_DOWN)
		{
			m_tInfo.vPos.y -= GET_TIME * m_fSpeed * 1.1f;
		}	

		if(m_eDir == DIR_L)
		{
			m_tInfo.vPos.x += GET_TIME * m_fSpeed* 1.1f;
		}

		if(m_eDir == DIR_R)
		{
			m_tInfo.vPos.x -= GET_TIME * m_fSpeed * 1.1f;
		}

	}


	if(m_bAttack == true)
	{

		if(m_eDir == DIR_RU)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed *  4.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed*  4.f;

		}

		if(m_eDir == DIR_RD)
		{
			m_tInfo.vPos.x -= cos(35.f) *GET_TIME * m_fSpeed*  4.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed*  4.f;
		}			

		if(m_eDir == DIR_LU)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed *  4.f;
			m_tInfo.vPos.y -= -sin(35.f) * GET_TIME * m_fSpeed* 4.f;
		}



		if(m_eDir == DIR_LD)
		{
			m_tInfo.vPos.x += cos(35.f) *GET_TIME * m_fSpeed*  4.f;
			m_tInfo.vPos.y += -sin(35.f) * GET_TIME * m_fSpeed*  4.f;
		}		


		if(m_eDir == DIR_UP)
		{
			m_tInfo.vPos.y -= GET_TIME * m_fSpeed*  4.f;
		}

		if(m_eDir == DIR_DOWN)
		{
			m_tInfo.vPos.y += GET_TIME * m_fSpeed * 4.f;
		}	

		if(m_eDir == DIR_L)
		{
			m_tInfo.vPos.x -= GET_TIME * m_fSpeed*  4.f;
		}

		if(m_eDir == DIR_R)
		{
			m_tInfo.vPos.x += GET_TIME * m_fSpeed *  4.f;
		}

		CollisionPlayer();

	}

	float fDistance;
	D3DXVECTOR3 vDis = m_tInfo.vPos - m_vAttackPos;
	fDistance = D3DXVec3Length(&vDis);

	if(fDistance > 80 && m_bAttack == false)
	{
		m_bCanDamage = true;
		m_bAttack = true;
	}
	if(fDistance > 130 && m_bAttack == true)
	{
		m_bAttack = false;
		m_eState = MSTATE_CHASING;
	}

}

void CMonster1::DirCheck()
{
	if(m_eDir == DIR_L || m_eDir == DIR_R)
		m_wstrStateKey = L"Dir_L";

	if(m_eDir == DIR_LD || m_eDir == DIR_RD)
		m_wstrStateKey = L"Dir_LB";


	if(m_eDir == DIR_LU || m_eDir == DIR_RU)
		m_wstrStateKey = L"Dir_LT";

	if(m_eDir == DIR_UP )
		m_wstrStateKey = L"Dir_T";

	if(m_eDir == DIR_DOWN)
		m_wstrStateKey = L"Dir_B";


}

void CMonster1::CollisionHit()
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


void CMonster1::CollisionPlayer()
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
