#pragma once

class CGameObject;

template<typename T>
class CFactory abstract		//추상 클래스 : abstract
{
public:
	static CGameObject* Create(void)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CGameObject* Create(float _fx, float _fy)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(_fx, _fy);

		return pObj;
	}

	static CGameObject* Create(D3DXVECTOR3 vPos)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(vPos);

		return pObj;
	}


};
