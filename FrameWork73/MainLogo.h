#pragma once
#include "ui.h"

class CMainLogo :
	public CUI
{
public:
	CMainLogo(void);
	~CMainLogo(void);

private:
	bool m_bUP; 
	bool m_bErase;
	bool m_bFasten;
	float m_fAlpa;
	bool m_bAlpa;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
