#include "StdAfx.h"
#include "RenderMgr.h"
#include "GameObject.h"


void CRenderMgr::AddGameObject(CGameObject* pObj, eRenderType eType)
{
	m_vecRenderObj[eType].push_back(pObj);
}

void CRenderMgr::Render(HDC _hdc)
{
	sort(m_vecRenderObj[RENDER_WORLD].begin(), 
		m_vecRenderObj[RENDER_WORLD].end(),
		CRenderMgr::Compare );

	for(int i = 0; i < RENDER_END; ++i)
	{
		int iCount = m_vecRenderObj[i].size();

		for(int j = 0; j < iCount; ++j)
		{
			m_vecRenderObj[i][j]->Render();
			// i : Type
			// j : °´Ã¼ ¼ö
		}
		m_vecRenderObj[i].clear();
	}
}

bool CRenderMgr::Compare(CGameObject* pDest, CGameObject* pSour)
{
	return (pDest->GetInfo()->vPos.y + pDest->GetInfo()->fCY 
			<
			pSour->GetInfo()->vPos.y + pSour->GetInfo()->fCY);
}

CRenderMgr::CRenderMgr(void)
{
}

CRenderMgr::~CRenderMgr(void)
{
}
