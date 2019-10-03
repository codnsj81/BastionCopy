#pragma once

#include "Include.h"

class CUI;
class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr);

public:
	void AddGameUI(CUI* pUI, eUITYPE eType);
public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);

private:
	list<CUI*>	m_UIList[UI_END];
};
