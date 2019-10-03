#include "StdAfx.h"
#include "ParticleMenu.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Device.h"

CParticleMenu::CParticleMenu(void)
{
}

CParticleMenu::~CParticleMenu(void)
{
}

HRESULT CParticleMenu::Initialize(void)
{
	m_eRenderType = RENDER_PARTICE;
	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Bandibul";

	m_Bandibul[0].fSpeed = 130.f,
	m_Bandibul[1].fSpeed  = 145.f;
	m_Bandibul[2].fSpeed  = 110.f;
	m_Bandibul[3].fSpeed  = 155.f;
	m_Bandibul[4].fSpeed  =  124.f;
	m_Bandibul[5].fSpeed = 130.f,
	m_Bandibul[6].fSpeed  = 145.f;
	m_Bandibul[7].fSpeed  = 160.f;
	m_Bandibul[8].fSpeed  = 155.f;
	m_Bandibul[9].fSpeed  =  118.f;
	m_Bandibul[10].fSpeed = 100.f,
	m_Bandibul[11].fSpeed  = 95.f;
	m_Bandibul[12].fSpeed  = 110.f;
	m_Bandibul[13].fSpeed  = 150.f;
	m_Bandibul[14].fSpeed  =  120.f;


	for( int i =0 ; i< 15; i++)
	{
		m_Bandibul[i].vPos = D3DXVECTOR3(rand() % 1000, (float) WINCY, 0.f);
		m_Bandibul[i].iSecond = rand() % 10;
		m_Bandibul[i].fAngle = rand() % 45;

	}


	for(int i =0; i<15 ; i++)
	{
		m_Bandibul[i].dwTime = GetTickCount();
		m_Bandibul[i].dwAlpaTime = GetTickCount();
		m_Bandibul[i].iAlpa = 255.f;
		m_Bandibul[i].bAlpa = false;
		m_Bandibul[i].bDir  = true;
	}

	return S_OK;
}

int CParticleMenu::Update(void)
{
	XProgress();
	return 0;
}

void CParticleMenu::Render(void)
{

	DWORD dwTime = GetTickCount();

	D3DXMATRIX matTrans , matScale;
	const TEX_INFO* pPlayerTex ;

	for(int i =0; i<15 ; i++)
	{	
		if(dwTime > m_Bandibul[i].dwAlpaTime + 1000 * m_Bandibul[i].iSecond)
		{
			if(m_Bandibul[i].bAlpa = false)
			{
				m_Bandibul[i].bAlpa = true;
			}

			if(m_Bandibul[i].iAlpa > 0.f)
				m_Bandibul[i].iAlpa -= 0.5f;

			else
			{
				m_Bandibul[i].vPos.x = rand() % 1000;
				m_Bandibul[i].vPos.y = (float) WINCY;
				m_Bandibul[i].iAlpa = 255;
				m_Bandibul[i].bAlpa = false;
				m_Bandibul[i].dwAlpaTime  = dwTime;
			}
		}

		pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
			,m_wstrStateKey.c_str()
			,i % 3);

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans,
			m_Bandibul[i].vPos.x,
			m_Bandibul[i].vPos.y,
			0.f);

		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

		matTrans = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB((int) m_Bandibul[i].iAlpa, 255, 255, 255));

	}

}

void CParticleMenu::Release(void)
{

}


void CParticleMenu::XProgress()
{
	for(int i=0; i< 15; i++)
	{

		if(m_Bandibul[i].bDir == true) // ru
		{

			m_Bandibul[i].fAngle -= 1.f  * i * GET_TIME;

			m_Bandibul[i].vPos.x -= cos(m_Bandibul[i].fAngle) *GET_TIME * m_Bandibul[i].fSpeed ;
			m_Bandibul[i].vPos.y -= GET_TIME * m_Bandibul[i].fSpeed ;

			if(m_Bandibul[i].fAngle < 0.f)
			{
				m_Bandibul[i].bDir = false;
				m_Bandibul[i].fAngle = 45.f;
			}



		}

		if(m_Bandibul[i].bDir == false) // du
		{
			m_Bandibul[i].fAngle -= 1.f  * i * GET_TIME;

			m_Bandibul[i].vPos.x += cos(m_Bandibul[i].fAngle) *GET_TIME * m_Bandibul[i].fSpeed ;
			m_Bandibul[i].vPos.y -= GET_TIME * m_Bandibul[i].fSpeed ;

			if(m_Bandibul[i].fAngle < 0.f)
			{
				m_Bandibul[i].bDir = true;
				m_Bandibul[i].fAngle = 45.f;
			}


		}

	}


}
