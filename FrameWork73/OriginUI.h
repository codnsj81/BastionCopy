#pragma once
#include "UI.h"

class COriginUI :
	public CUI
{
public:
	COriginUI(void);
	~COriginUI(void);

	bool* m_bHammer;
	bool* m_bGun;
	int* m_iPlayerHp;

public:
	void ArmorUI(void);
	void PlayerHp(void);
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

};
