#pragma once

#include "PlayerHit.h"

class CGameObject;

template<typename T>
class CEffectFactory abstract
{
public:
	static CGameObject* CreateEffect(void)
	{
		CGameObject* pEffect = new CPlayerHit;
		CBridge* pBridge	 = new T;

		dynamic_cast<CPlayerHit*>(pEffect)->SetBridge(pBridge);
		pEffect->Initialize();
		pBridge->Initialize();

		return pEffect;
	}
};
