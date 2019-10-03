#pragma once
#include "gameobject.h"


class CBackGround :
	public CGameObject
{
private:
	vector<TILE*>		m_vecTile;
	vector<PTILE>		m_vecGround;
	bool				m_bOption3;
	CGameObject*		m_pPlayer;
	POINT			m_ptTileRender;

private:
	void FloatingRender(int iIndex);
	void MapDataLoad2(void);
	void NormalRender(int iIndex);
	void MapDataLoad(void);


	eSceneType GetStage(void);
public:
	vector<TILE*>* GetTile(void)
	{
		return &m_vecTile;
	}
	void PlayerDistance(int iIndex);
	void PlayerDistance3(int iIndex);
	D3DXVECTOR3 PlayerDistance2(int iIndex);

	void Option4Progress(int iIndex);
	void Option5Progress(int iIndex);

	bool CollisionPlayer();
	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void SetOption3(void);

public:
	virtual HRESULT Initialize(void);
	
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	CBackGround(void);
	virtual ~CBackGround(void);
};
