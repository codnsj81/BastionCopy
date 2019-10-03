#pragma once

#include "Include.h"
class CNarrMgr
{

	DECLARE_SINGLETON(CNarrMgr);

private:
	map<int, NARR> m_mapNarr;
	bool m_bOn;
	DWORD m_dwTime;
	NARR m_pNarr;

public:

	void TurnOnNarr(int i);

	void InitData();
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Release(void);
};
