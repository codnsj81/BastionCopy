#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	wstring m_wstrCurrentPath;

	map<const TCHAR*, CTexture*>		m_mapTexture;

public:
	float m_fBarScale;


	wstring& GetCurrentPath(void)
	{
		return m_wstrCurrentPath;
	}

	void SetCurrentPath(wstring& rPath)
	{
		m_wstrCurrentPath = rPath;
	}

public:
	HRESULT InsertTexture(const TCHAR* pFilePath,			//���ϰ��
						 const TCHAR* pObjKey,		//��ü
						 eTextureType eType,		//�̱�? ��Ƽ?
						 const TCHAR* pStateKey = L"",		//��ü ����
						 const int& iCount = 0);			//�̹��� ���

	const TEX_INFO* GetTexture( const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	HRESULT ReadImagePath(wstring wstrImagePath);


	void Release(void);
};
