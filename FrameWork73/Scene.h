#pragma once

#include "Include.h"

class CScene
{
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Update(void)PURE;
	virtual void Render(void)PURE;
	virtual void Release(void)PURE;
	virtual void RenderUI(void)PURE;

public:
	CScene(void);
	virtual ~CScene(void);
};
