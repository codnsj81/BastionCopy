#include "StdAfx.h"
#include "KeyMgr.h"
#include "Constant.h"

CKeyMgr::CKeyMgr(void)
:m_dwKey(0)
{
	m_bLButton = false;
	m_dwTime = 0;

}

CKeyMgr::~CKeyMgr(void)
{
}

DWORD CKeyMgr::GetKeyState(void)
{
		return m_dwKey;
}

void CKeyMgr::SetKeyState()
{
	m_dwKey = 0;

	
	if(GetAsyncKeyState(VK_LBUTTON) && m_bLButton == false)
	{
		m_dwKey |= KEY_LBUTTON;
		m_bLButton = true;
		m_dwTime = GetTickCount();
	}

	if(GetAsyncKeyState(VK_RBUTTON) && m_bLButton == false)
	{
		m_dwKey |= KEY_RBUTTON;
		m_bLButton = true;
		m_dwTime = GetTickCount();
	}


	if(m_dwTime + 900 < GetTickCount())
		m_bLButton = false;


	if(GetAsyncKeyState('A'))
	{
		m_dwKey |= KEY_LEFT;
	}
	if(GetAsyncKeyState('D'))
	{
		m_dwKey |= KEY_RIGHT;
	}
	if(GetAsyncKeyState('S'))
	{
		m_dwKey |= KEY_DOWN;
	}
	
	if(GetAsyncKeyState('W'))
	{
		m_dwKey |= KEY_UP;
	}

	if(GetAsyncKeyState(VK_SPACE))
	{
		m_dwKey |= KEY_SPACE;
	}
	
	if(GetAsyncKeyState(VK_CONTROL))
	{
		m_dwKey |= KEY_CONTROL;
	}

}

CKeyMgr* CKeyMgr::m_pInstance =NULL;