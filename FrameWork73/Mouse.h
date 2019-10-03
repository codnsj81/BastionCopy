#pragma once

#include "Include.h"

class CMouse	abstract
{
public:
	static D3DXVECTOR3 GetMousePos(void)
	{
		POINT ptMousePos;
		GetCursorPos(&ptMousePos);
		ScreenToClient(g_hWnd, &ptMousePos);

		return D3DXVECTOR3( static_cast<float>(ptMousePos.x),
							static_cast<float>(ptMousePos.y),
							0.f);
	}
};
