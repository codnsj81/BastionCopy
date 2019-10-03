#pragma once

#include "Include.h"
class CUI
{
public:
	CUI(void);
	~CUI(void);

protected:
	INFO m_tInfo;
	wstring		m_wstrObjKey;

public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Update(void)PURE;
	virtual void Render(void)PURE;
	virtual void Release(void)PURE;
};
