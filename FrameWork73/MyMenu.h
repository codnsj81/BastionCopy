#pragma once
#include "scene.h"

class CMyMenu :
	public CScene
{
public:
	CMyMenu(void);
	~CMyMenu(void);

private:
	void BackRender(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void RenderUI(void);
};
