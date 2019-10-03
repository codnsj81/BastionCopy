#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"
#include "TextureMgr.h"

void CObjMgr::XmlDataLoad(void)
{


}

void CObjMgr::AddGameObject(CGameObject* pObj, eObjectType eType)
{
	m_ObjList[eType].push_back(pObj);
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;	
}

int CObjMgr::Update(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter     = m_ObjList[i].begin();
		OBJITER iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; )
		{
			if( (*iter)->Update() == 1 )
			{
				delete *iter;
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::Render()
{

	for(int i = 0; i < OBJ_END; ++i)
	{
		if(i == OBJ_SCROLL)
			continue;

		OBJITER iter     = m_ObjList[i].begin();
		OBJITER iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			//(*iter)->Render(_hdc);

			m_vecRender[ (*iter)->GetRenderType() ].push_back(*iter);
		}
	}

	sort(m_vecRender[RENDER_WORLD].begin(), 
		m_vecRender[RENDER_WORLD].end(),
		CObjMgr::Compare );

	for(int i = 0; i < RENDER_END; ++i)
	{
		if(i == OBJ_SCROLL)
			continue;

		int iCount = m_vecRender[i].size();

		for(int j = 0; j < iCount; ++j)
		{
			m_vecRender[i][j]->Render();
			// i : Type
			// j : °´Ã¼ ¼ö
		}
		m_vecRender[i].clear();
	}
}

void CObjMgr::Release(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter     = m_ObjList[i].begin();
		OBJITER iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete (*iter);
		}
		m_ObjList[i].clear();
	}

}

CObjMgr* CObjMgr::m_pInstance = NULL;

CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}

bool CObjMgr::Compare( CGameObject* pDest, CGameObject* pSour )
{
	return (pDest->GetInfo()->vPos.y 
		<
		pSour->GetInfo()->vPos.y);

}
