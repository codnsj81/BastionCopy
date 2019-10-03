#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	map<const TCHAR*, CTexture*>		m_mapTexture;

public:
	HRESULT InsertTexture(const TCHAR* pFilePath,			//���ϰ��
						 const TCHAR* pObjKey,		//��ü
						 eTextureType eType,		//�̱�? ��Ƽ?
						 const TCHAR* pStateKey = L"",		//��ü ����
						 const int& iCount = 0);			//�̹��� ���

	const TEX_INFO* GetTexture( const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	void Release(void);
};
