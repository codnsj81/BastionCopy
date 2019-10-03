#pragma once

#include "Include.h"

class CTool73View;

class CObstacle
{
private:
	vector<pOBJECT_DATA>	m_vecObject;
	CTool73View*			m_pView;

public:
	vector<pOBJECT_DATA>* GetObject()
	{
		return &m_vecObject;
	}
	
	void SetObject(vector<pOBJECT_DATA> pVector)
	{
		m_vecObject = pVector;
	}

	void SetMainView(CTool73View* pView)
	{
		m_pView = pView;
	}
public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);
	void HitBoxRender(void);

public:
	int GetObstacleIndex(const D3DXVECTOR3& vPos);
	void ObstacleSelect(const D3DXVECTOR3& vPos, const int& iDrawID);

	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);



public:
	CObstacle(void);
	~CObstacle(void);
};
