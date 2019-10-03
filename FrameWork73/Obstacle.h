#pragma once
#include "gameobject.h"

class CObstacle :
	public CGameObject
{
private:
	OBJECT_DATA m_ObjData;
	float m_fDistance;
	bool m_bChoosing;
	bool m_bDestruct;
	D3DXVECTOR3 m_vShowingPos;
	D3DXVECTOR3* m_vPlayer;
	eObjState m_eState;
	bool m_bFloating;
	//bool m_bNormal;

public:
	CObstacle(OBJECT_DATA _data)
		:m_ObjData(_data)
	{

		m_tInfo.vPos = m_ObjData.vPos;
	}
	CObstacle(void);
	bool CollisionHit();
	virtual ~CObstacle(void);
	HitBoxData GetHitBox(int i)
	{
		return m_ObjData.rHitBox[i];
	}

public:
	void FloatingRender(void);
	void BoundingRender1(void);
	void BoundingRender2(void);
	void NormalRender(void);
	void GetPlayerPos(D3DXVECTOR3* _vPos);
	void LoadObjectData(void);
	void HitBoxRender(void);
	void GetPlayerDistance(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
