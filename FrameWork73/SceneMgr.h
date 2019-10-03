#pragma once

#include "Include.h"

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr);

private:
	CScene* m_pScene;

private:
	bool		m_bChangeCheck;
	eSceneType  m_eType;

public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void RenderUI(void);
	void Release(void);

public:
	eSceneType GetSceneType(void)
	{
		return m_eType;
	}
	void SetScene(eSceneType eType);
	void SetChangeScene(eSceneType eType);
};
