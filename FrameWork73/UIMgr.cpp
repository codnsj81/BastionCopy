#include "StdAfx.h"
#include "UIMgr.h"
#include "UI.h"
IMPLEMENT_SINGLETON(CUIMgr);


void CUIMgr::Release(void)
{
	for(int i = 0; i < UI_END; ++i)
	{
		UIITER iter     = m_UIList[i].begin();
		UIITER iter_end = m_UIList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete (*iter);
		}
		m_UIList[i].clear();
	}

}

void CUIMgr::Render(void)
{
	for(int i = 0; i < UI_END; ++i)
	{
		UIITER iter     = m_UIList[i].begin();
		UIITER iter_end = m_UIList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}

}

int CUIMgr::Update(void)
{
	for(int i =0; i<UI_END; ++i)
	{
		UIITER iter = m_UIList[i].begin();
		UIITER iter_end = m_UIList[i].end();

		for(iter; iter!= iter_end;)
		{
			if((*iter)->Update() == 1 )
			{
				delete *iter;
				iter = m_UIList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;

}

HRESULT CUIMgr::Initialize(void)
{
	return S_OK;

}

void CUIMgr::AddGameUI(CUI* pUI, eUITYPE eType)
{
	m_UIList[eType].push_back(pUI);

}
