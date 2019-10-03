#pragma once
#include "effectbridge.h"

class CEffectBuff :
	public CEffectBridge
{
private:
	float m_fRotateAngle;
	float m_fSaveTime;

private:
	void WorldMatrix(INFO& rInfo);
	void FrameMove(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(INFO& rInfo);
	virtual void Render(void);
	virtual void Release(void);

public:
	CEffectBuff(void);
	virtual ~CEffectBuff(void);
};
