#pragma once

#include "Include.h"

class CRenderMgr
{
private:
	vector<CGameObject*>		m_vecRenderObj[RENDER_END];

public:
	void AddGameObject(CGameObject* pObj, eRenderType eType);
	void Render(HDC _hdc);

private:
	static bool Compare(CGameObject* pDest, CGameObject* pSour);


DECLARE_SINGLETON(CRenderMgr);

};
