#include "StdAfx.h"
#include "Effect.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CEffect::CEffect(void)
{
}

CEffect::CEffect( INFO _info, wstring _State, FRAME _frame)
{
	m_tFrame = _frame;
	m_tInfo = _info;
	m_eRenderType = RENDER_EFFECT;
	m_strStateKey = _State;
	m_wstrObjKey = L"Effect";
}

CEffect::~CEffect(void)
{
}

HRESULT CEffect::Initialize( void )
{
	return S_OK;

}

int CEffect::Update( void )
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME * 2.f;


	if(m_tFrame.fMax < m_tFrame.fFrame)
	{
		return 1;
	}

	else
		return 0;

}

void CEffect::Render( void )
{
	D3DXMATRIX matTrans, matScalse;
	const TEX_INFO* pPlayerTex = 
		CTextureMgr::GetInstance()->GetTexture( m_wstrObjKey.c_str()
		,m_strStateKey.c_str()
		,(int)m_tFrame.fFrame);

	if(pPlayerTex == NULL)
		return ;

	float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
	float fY = float(pPlayerTex->ImageInfo.Height / 2);

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		0.f);

	if(m_strStateKey ==  L"Shot")
	{
		D3DXMatrixScaling(&matScalse, 1.7f, 1.7f, 0.f);
		matTrans = matScalse * matTrans;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPlayerTex->pTexture,
		NULL,
		&D3DXVECTOR3( fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEffect::Release( void )
{

}
