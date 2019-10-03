#pragma once

#include"Include.h"

class CTool73View;

class CMonster
{
public:
	CMonster(void);
	~CMonster(void);

private:

	vector<pUNIT_DATA>		m_vecBoss;
	vector<pUNIT_DATA>		m_vecMonster1;
	vector<pUNIT_DATA>		m_VecMonster2;

	CTool73View*			m_pView;

public:


	void SetMainView(CTool73View* pView)
	{
		m_pView = pView;
	}


	vector<pUNIT_DATA>*	GetBoss()
	{
		return &m_vecBoss;
	}

	vector<pUNIT_DATA>*	GetMonster1()
	{
		return &m_vecMonster1;
	}


	vector<pUNIT_DATA>*	GetMonster2()
	{
		return &m_VecMonster2;
	}
public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);
};
