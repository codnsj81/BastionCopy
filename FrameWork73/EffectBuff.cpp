#include "StdAfx.h"
#include "EffectBuff.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TimeMgr.h"
#include "Device.h"

void CEffectBuff::WorldMatrix(INFO& rInfo)
{
	D3DXMATRIX matRotZ, matTrans, matScale, matParent;

	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRotateAngle) );

	D3DXMatrixTranslation(&matTrans, rInfo.vPos.x, rInfo.vPos.y, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	matParent = CObjMgr::GetInstacne()->FindGameObject<CPlayer>()->GetInfo()->matWorld;

	m_fRotateAngle += 100 * GET_TIME;

	rInfo.matWorld = matScale * matRotZ * matTrans * matParent;
}

void CEffectBuff::FrameMove(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if(m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = 0.f;
}

HRESULT CEffectBuff::Initialize(void)
{
	m_fSaveTime = 3.f;

	m_wstrStateKey = L"Hammer";

	m_tFrame = FRAME(0.f, 14.f, 14.f);

	return S_OK;
}

int CEffectBuff::Update(INFO& rInfo)
{
	WorldMatrix(rInfo);
	FrameMove();

	m_fSaveTime -= GET_TIME;

	if(m_fSaveTime <= 0.f)
		return 1;

	return 0;
}

void CEffectBuff::Render(void)
{
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	CDevice::GetInstance()->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	CDevice::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	CDevice::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CEffectBridge::Render();
}

void CEffectBuff::Release(void)
{

}

CEffectBuff::CEffectBuff(void)
{
}

CEffectBuff::~CEffectBuff(void)
{
	Release();
}
