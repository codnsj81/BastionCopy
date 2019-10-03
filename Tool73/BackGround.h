#pragma once

#include "Include.h"

class CTool73View;

class CBackGround
{
private:

	vector<PTILE>		m_vecTile;
	vector<PTILE>		m_vecGround;
	CTool73View*		m_pView;

public:

	void SetTile(vector<PTILE> pVector)
	{
		m_vecTile = pVector;
	}
	
	void SetGround(vector<PTILE> pVector)
	{
		m_vecGround = pVector;
	}


	vector<PTILE>*	GetTile(void)
	{ return &m_vecTile; }


	vector<PTILE>*	GetGround(void)
	{ return &m_vecGround; }


public:
	void SetMainView(CTool73View* pView)
	{	m_pView = pView; }

public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);


public:
	int GetGroundIndex(const D3DXVECTOR3& vPos);
	//현재 내 타일 Index를 얻어 오는 함수./
	void PushVecGround(PTILE pTile);
	int GetTileIndex(const D3DXVECTOR3& vPos);
	void TileChange(const D3DXVECTOR3& vPos, const int& iDrawID);
	void TileChoose(const D3DXVECTOR3& vPos);

	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool GroundPicking(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void MiniViewRender(void);

public:
	CBackGround(void);
	~CBackGround(void);
};
