#pragma once

//#include "Include.h"

class CKeyMgr
{
private:
	DWORD m_dwKey;
	DWORD m_dwTime;
	bool m_bLButton;

public:
	void SetKeyState(void);
	DWORD GetKeyState(void);

private:
	static CKeyMgr* m_pInstance;

public:
	static CKeyMgr* GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CKeyMgr;

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
	CKeyMgr(void);
public:
	~CKeyMgr(void);
};
