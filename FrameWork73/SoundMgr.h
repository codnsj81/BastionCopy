#pragma once

#include "Include.h"

class CSoundMgr
{
	//���� �Ŵ����� ����� �ο�����.

private:
	FMOD_SYSTEM*	m_pSystem;		//fmod �ý����� ������ ������.
	FMOD_CHANNEL*	m_pEffect;		//ȿ���� ä��.
	FMOD_CHANNEL*	m_pMainBGM;		//����� ä��.
	FMOD_CHANNEL*	m_pSkill;		//��ų ä��.
	FMOD_CHANNEL*   m_pNarration;
	FMOD_CHANNEL*	m_pMonster;		//���� ä��.

	FMOD_CHANNEL*	m_pArSound[4];		//���� ä��.

	FMOD_RESULT		m_Result;		//Fmod ���� üũ.

	unsigned int m_iVersion;		
	
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;


public:
	//���� �������̽��� ����.
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
