#pragma once
#include "gameobject.h"

class CMouseUI :
	public CGameObject
{
public:
	CMouseUI(void);
	~CMouseUI(void);

	void SetTexture(void);

private:
	float m_fRotate;
	wstring m_wstrStateKey;
	BYTE  m_DrawID;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

};
