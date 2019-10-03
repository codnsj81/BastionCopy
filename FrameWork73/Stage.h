#pragma once
#include "scene.h"

class CStage :
	public CScene
{
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void RenderUI(void);
	
	void UnitLoad(void);
	void ObjectLoad(void);

public:
	CStage(void);
	virtual ~CStage(void);
};
