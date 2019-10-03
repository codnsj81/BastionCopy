#pragma once
#include "ui.h"

class CMenuPan :
	public CUI
{
public:
	CMenuPan(void);
	~CMenuPan(void);

private:
	bool m_bFasten;
	bool m_bBoyDir;
	D3DXVECTOR3	m_tBoy;
	D3DXVECTOR3 m_tBoyDir;
	float m_fBoySpeed;
	BUTTON		m_Button[2];

public:
	void BoyMove();
	void BoyRender();
	void MenuRender();
	void ButtonClick();

	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
