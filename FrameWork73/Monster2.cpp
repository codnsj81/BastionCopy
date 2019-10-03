#include "StdAfx.h"
#include "Monster2.h"
#include "TextureMgr.h"
#include "Device.h"
#include "PlayerHit.h"
#include "SoundMgr.h"
#include "NarrMgr.h"
#include "Monster2Bullet.h"

CMonster2::CMonster2(void)
{
}

CMonster2::CMonster2(UNIT_DATA _data)
{
	m_tInfo.vPos = _data.tInfo.vPos;
	m_iHp = _data.iHp;
	m_iAtt = _data.iAttack;

}

CMonster2::~CMonster2(void)
{
}

HRESULT CMonster2::Initialize(void)
{

	m_eRenderType = RENDER_WORLD;

	m_fDistance = 0.f;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 90.f;

	m_eDir = DIR_L;
	m_eState = MSTATE_WAITING;
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vDir = D3DXVECTOR3(1.f, 1.f, 1.f);


	m_wstrObjKey = L"Monster2";
	m_wstrStateKey = L"Dir_L";

	m_dwShotTime = 0;

	GetPlayer();
	return S_OK;


}

int CMonster2::Update(void)
{
	Progress();
	DirCheck();
	CollisionHit();

	if(m_iHp < 0)
		return 1;

	CGameObject::Update();

	return 0;
}

void CMonster2::Render(void)
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

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

		if(m_eDir == DIR_R || m_eDir == DIR_RD || m_eDir == DIR_RU)
			D3DXMatrixScaling(&matScale , -1.f, 1.f, 1.f);

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


	}
	return;

}

void CMonster2::Release(void)
{
	return;
}

void CMonster2::MyWorldCheck()
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matTrans;
	return;
}

void CMonster2::MyFrameMove()
{
	return;
}

void CMonster2::Damage(int iAtt)
{
	m_iHp -= iAtt;
	//cout<<m_MonData.iHp<<endl;
	return;
}

void CMonster2::Progress()
{

	PlayerDistance();

	if(m_eState == MSTATE_ATTACK)
	{
		DWORD dwTime = GetTickCount();

		if(dwTime > m_dwShotTime + 3000)
		{
			m_dwShotTime = dwTime;
			CSoundMgr::GetInstacne()->PlayMonsterSound(L"Monster2Att.MP3");
			AimToPlayer();
		}
	}


}

void CMonster2::DirCheck()
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

void CMonster2::PlayerDistance()
{
	
	m_vPlayerPos = m_pPlayer->GetInfo()->vPos;


	D3DXVECTOR3 vDis = m_tInfo.vPos - m_vPlayerPos;
	m_fDistance = D3DXVec3Length(&vDis);

	if(m_fDistance < 300 && m_eState == MSTATE_WAITING)
	{
		CNarrMgr::GetInstance()->TurnOnNarr(21);
		m_dwShotTime = GetTickCount();
		m_eState = MSTATE_ATTACK;
	}


}

void CMonster2::AimToPlayer()
{

	m_tInfo.vDir = m_vPlayerPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	float fCos = D3DXVec3Dot(&m_tInfo.vLook, &m_tInfo.vDir);
	fCos = acos(fCos);

	if(m_tInfo.vPos.y < m_vPlayerPos.y)
		fCos =  2* D3DX_PI - fCos;
	
	m_fAngle = fCos * 180.f / 3.14f;

	Shot();


	if(m_fAngle  < 22.5f || m_fAngle >=  337.5f)
	{
		m_eDir = DIR_R;
		return;
	}

	if(m_fAngle  < 67.5f && m_fAngle >=  22.5f)
	{
		m_eDir = DIR_RU;
		return;
	}

	if(m_fAngle  < 112.5f && m_fAngle >=  67.5f)
	{
		m_eDir = DIR_UP;
		return;
	}

	if(m_fAngle  < 157.5f && m_fAngle >=  112.5f)
	{
		m_eDir = DIR_LU;
		return;
	}

	if(m_fAngle  < 202.5f && m_fAngle >=  157.5f)
	{
		m_eDir = DIR_L;
		return;
	}

	if(m_fAngle  < 247.5f && m_fAngle >=  202.5f)
	{
		m_eDir = DIR_LD;
		return;
	}

	if(m_fAngle  < 292.5f && m_fAngle >=  247.5f)
	{
		m_eDir = DIR_DOWN;
		return;
	}

	if(m_fAngle  < 337.5f && m_fAngle >=  292.5f)
	{
		m_eDir = DIR_RD;
		return;
	}

}

void CMonster2::Shot()
{
	CGameObject* pBullet = new CMonster2Bullet(m_tInfo, m_iAtt, m_fAngle);
	pBullet->Initialize();
	CObjMgr::GetInstacne()->AddGameObject(pBullet, OBJ_MONSTERBULLET);


}

void CMonster2::CollisionHit()
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
