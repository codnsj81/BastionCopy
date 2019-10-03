#pragma once
#include "scene.h"

class CLogo :
	public CScene
{
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void RenderUI(void);

private:

	bool m_bAlpa; //t : ¹à¾ÆÁü f: ¾îµÎ¿öÁü
	float m_fAlpa;
	bool m_bLogoRender;
	bool m_bCompanyRender;

	void BackRender(void);
	void LogoRender(void);
	void CompanyRender(void);
	HRESULT ImageLoad(void);

public:
	CLogo(void);
	virtual ~CLogo(void);
};
