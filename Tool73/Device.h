#pragma once

#include "Include.h"
//#include "../Headers/Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);

private:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pD3dDevice;

	LPD3DXSPRITE			m_pSprite;

public:
	HRESULT Initialize3D(void);
	void SetParameters(D3DPRESENT_PARAMETERS& d3dp);		// 실제 장치를 설정하는 기능의 함수.

	//void AAA(int** p){*p}
	//void BBB(int*& p){p = }

public:
	void Render_Begin(void);
	void Render_End(void);
	void Render_End(HWND& rhWnd);

	void Release(void);

public:
	LPDIRECT3DDEVICE9 GetDevice(void)
	{
		return m_pD3dDevice;
	}

	LPD3DXSPRITE GetSprite(void)
	{
		return m_pSprite;
	}


};
