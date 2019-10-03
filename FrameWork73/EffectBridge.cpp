#include "StdAfx.h"
#include "EffectBridge.h"
#include "TextureMgr.h"
#include "GameObject.h"
#include "Device.h"

HRESULT CEffectBridge::Initialize(void)
{
	return S_OK;
}

int CEffectBridge::Update(INFO& rInfo)
{
	return 0;
}

void CEffectBridge::Render(void)
{
	const TEX_INFO* pEffectTex = 
		CTextureMgr::GetInstance()->GetTexture(
		m_pObject->GetObjKey().c_str(),
		m_wstrStateKey.c_str(),
		static_cast<int>(m_tFrame.fFrame) );

	if(pEffectTex == NULL)
		return;

	float fX = pEffectTex->ImageInfo.Width / 2.f;
	float fY = pEffectTex->ImageInfo.Height / 2.f;
	
	CDevice::GetInstance()->GetSprite()->SetTransform(
		&m_pObject->GetInfo()->matWorld
		);

	CDevice::GetInstance()->GetSprite()->Draw(
		pEffectTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(50, 255, 255, 255)
		);

}

void CEffectBridge::Release(void)
{

}

CEffectBridge::CEffectBridge(void)
{
}

CEffectBridge::~CEffectBridge(void)
{
	Release();
}
