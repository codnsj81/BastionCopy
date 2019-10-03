#pragma once

#include "Include.h"

class CDevice;
class CMainGame
{
private:
	CDevice* m_pDevice;

public:
	HRESULT Initialize(void);
	int Update(void);
	void Render(void);
	void Release(void);

public:
	CMainGame(void);
	~CMainGame(void);

private:
	DWORD m_dwTime;		//WORD
	TCHAR m_szFPS[128];
	int	  m_iFps;

};
