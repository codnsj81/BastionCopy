#pragma once

//##중재자 패턴(메디에이터)
//객체들을 한군데 모아서 관리해주는 디자인 패턴.

#include "Include.h"

class CGameObject;
class CObjMgr
{
private:
	list<CGameObject*>		m_ObjList[OBJ_END];

	//##Render
	vector<CGameObject*>	m_vecRender[RENDER_END];

	//Test Item Load

public:
	void XmlDataLoad(void);

private:
	static bool Compare(CGameObject* pDest, CGameObject* pSour);

public:
	void AddGameObject(CGameObject* pObj, eObjectType eType);

	template<typename T>
	T* FindGameObject(int iIndex = 0)
	{
		for(size_t i = 0; i < OBJ_END; ++i)
		{
			for(list<CGameObject*>::iterator iter = m_ObjList[i].begin();
				iter != m_ObjList[i].end(); ++iter)
			{
				if( typeid(T) == typeid( *(*iter) ) )
				{
					if(iIndex == 0)
						return dynamic_cast<T*>( (*iter) );
					else
						--iIndex;
				}
			}
		}

		return NULL;
	}

	list<CGameObject*>* GetList(void)
	{
		return m_ObjList;
	}

	list<CGameObject*>* GetObject(eObjectType type)
	{
		return &m_ObjList[type];
	}

	void SetList(list<CGameObject*>* list, eObjectType type)
	{
		m_ObjList[type] = *list;
	
	}

public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);

private:
	static CObjMgr* m_pInstance;

public:
	static CObjMgr* GetInstacne(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CObjMgr;

		return m_pInstance;
	}

	void Destroy(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
