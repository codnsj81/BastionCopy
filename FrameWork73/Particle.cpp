#include "StdAfx.h"
#include "Particle.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Device.h"

CParticle::CParticle(void)
{
}

CParticle::~CParticle(void)
{
}

HRESULT CParticle::Initialize(void)
{
	m_eRenderType = RENDER_PARTICE;
	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Bandibul";

	m_Bandibul[0].fSpeed = 130.f,
	m_Bandibul[1].fSpeed  = 145.f;
	m_Bandibul[2].fSpeed  = 110.f;
	m_Bandibul[3].fSpeed  = 155.f;
	m_Bandibul[4].fSpeed  =  124.f;

	m_Bandibul[0].vPos = D3DXVECTOR3(200.f, (float) WINCY, 0.f);
	m_Bandibul[1].vPos = D3DXVECTOR3(500.f,(float)  WINCY, 0.f);
	m_Bandibul[2].vPos = D3DXVECTOR3(800.f,(float)  WINCY, 0.f);
	m_Bandibul[3].vPos = D3DXVECTOR3(400.f, (float) WINCY, 0.f);
	m_Bandibul[4].vPos = D3DXVECTOR3(900.f, (float) WINCY, 0.f);


	m_Bandibul[0].iSecond = 3;
	m_Bandibul[1].iSecond = 1;
	m_Bandibul[2].iSecond = 9;
	m_Bandibul[3].iSecond = 5;
	m_Bandibul[4].iSecond = 2;

	m_Bandibul[0].fAngle = 11.f;
	m_Bandibul[1].fAngle = 45.f;
	m_Bandibul[2].fAngle = 20.f;
	m_Bandibul[3].fAngle = 3.f;
	m_Bandibul[4].fAngle = 30.f;







	for(int i =0; i<5 ; i++)
	{
		m_Bandibul[i].dwTime = GetTickCount();
		m_Bandibul[i].dwAlpaTime = GetTickCount();
		m_Bandibul[i].iAlpa = 255.f;
		m_Bandibul[i].bAlpa = false;
		m_Bandibul[i].bDir  = true;
	}

	return S_OK;
}

int CParticle::Update(void)
{
	XProgress();
	return 0;
}

void CParticle::Render(void)
{

	DWORD dwTime = GetTickCount();

	D3DXMATRIX matTrans;
	const TEX_INFO* pPlayerTex ;

	for(int i =0; i<5 ; i++)
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
			,i / 2);

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans,
		m_Bandibul[i].vPos.x,
		m_Bandibul[i].vPos.y,
			0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB((int) m_Bandibul[i].iAlpa, 255, 255, 255));

	}

}

void CParticle::Release(void)
{

}


void CParticle::XProgress()
{
	for(int i=0; i< 5; i++)
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
