#pragma once

#include "Include.h"

class CGameObject
{
protected:
	INFO		m_tInfo;
	float		m_fSpeed;
	wstring		m_wstrObjKey;
	int			m_iHp;
	int			m_iAtt;
	RECT		m_tRect;
	eRenderType m_eRenderType;
	static	eUnitDir	m_playerDir;
	


protected:
	FRAME m_tFrame;

public:
	eRenderType GetRenderType(void)
	{
		return m_eRenderType;
	}

	const wstring& GetObjKey(void)
	{
		return m_wstrObjKey;
	}

public:
	static D3DXVECTOR3 m_vScroll;
	static float			m_iFade;
	
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Update(void);
	virtual void Render(void)PURE;
	virtual void Release(void)PURE;

public:
	void SetPos(const D3DXVECTOR3& vPos)
	{
		m_tInfo.vPos = vPos;
	}

	void SetPos(const float fX, const float fY)
	{
		m_tInfo.vPos.x = fX;
		m_tInfo.vPos.y = fY;
		m_tInfo.vPos.z = 0.f;
	}

	INFO* GetInfo()
	{
		return &m_tInfo;
	}
	RECT GetRect()
	{
		return m_tRect;
	}


	int GetAtt()
	{
		return m_iAtt;

	}


public:
	CGameObject(void);
	virtual ~CGameObject(void);
};
