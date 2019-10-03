#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	map<const TCHAR*, CTexture*>		m_mapTexture;

public:
	HRESULT InsertTexture(const TCHAR* pFilePath,			//파일경로
						 const TCHAR* pObjKey,		//객체
						 eTextureType eType,		//싱글? 멀티?
						 const TCHAR* pStateKey = L"",		//객체 상태
						 const int& iCount = 0);			//이미지 장수

	const TEX_INFO* GetTexture( const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	void Release(void);
};
