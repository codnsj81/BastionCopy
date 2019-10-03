#pragma once
#include "scene.h"
#include "Include.h"

class CLoading :
	public CScene
{

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	float				m_fAngle;

private:
	static unsigned int __stdcall ImgLoadThreadFunc(void* pArg);
	CRITICAL_SECTION GetCrt(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void RenderUI(void);

public:
	CLoading(void);
	~CLoading(void);
};
