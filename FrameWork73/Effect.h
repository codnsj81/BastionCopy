#pragma once
#include "gameobject.h"

class CEffect :
	public CGameObject
{
public:
	CEffect(void);
	~CEffect(void);
	CEffect(INFO _info, wstring _State, FRAME _frame);

private:
	wstring m_strStateKey;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
