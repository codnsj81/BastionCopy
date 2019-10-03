#pragma once

#include "Include.h"

class CSoundMgr
{
	//사운드 매니저의 기능을 부여하자.

private:
	FMOD_SYSTEM*	m_pSystem;		//fmod 시스템을 관리할 포인터.
	FMOD_CHANNEL*	m_pEffect;		//효과음 채널.
	FMOD_CHANNEL*	m_pMainBGM;		//배경음 채널.
	FMOD_CHANNEL*	m_pSkill;		//스킬 채널.
	FMOD_CHANNEL*   m_pNarration;
	FMOD_CHANNEL*	m_pMonster;		//몬스터 채널.

	FMOD_CHANNEL*	m_pArSound[4];		//몬스터 채널.

	FMOD_RESULT		m_Result;		//Fmod 에러 체크.

	unsigned int m_iVersion;		
	
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;


public:
	//실제 인터페이스를 정의.
	void Initialize(void);
	void LoadSoundFile(void);
	
	void PlayEffectSound(TCHAR* pSoundKey);
	void PlayBGMSound(TCHAR* pSoundKey);
	void PlaySkillSound(TCHAR* pSoundKey);
	void PlayMonsterSound(TCHAR* pSoundKey);
	void PlayNarrationSound(TCHAR* pSoundKey);

	void StopBGM(void);
	void StopALL(void);

public:
	map<TCHAR*, FMOD_SOUND*>*	GetSoundMap(void)
	{
		return &m_mapSound;
	}

private:
	void ErrorCheck(FMOD_RESULT _result);

private:
	static CSoundMgr* m_pInstance;

public:
	static CSoundMgr* GetInstacne(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSoundMgr;

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
	CSoundMgr(void);
public:
	~CSoundMgr(void);
};
