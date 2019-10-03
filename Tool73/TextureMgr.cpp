#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr);

HRESULT CTextureMgr::InsertTexture(const TCHAR* pFilePath, /*파일경로 */ 
								   const TCHAR* pObjKey, /*객체 */ 
								   eTextureType eType, /*싱글? 멀티? */ 
								   const TCHAR* pStateKey /*= L""*/, /*객체 상태 */ 
								   const int& iCount /*= 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if( m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObjKey) );

	if(iter == m_mapTexture.end())
	{
		//##ObjectKey없을때 즉 새로운 키값일 경우에 추가한다.
		
		CTexture* pTexture = NULL;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED( pTexture->InsertTexture(pFilePath, pStateKey, iCount) ))
		{
			//AfxMessageBox()
			return E_FAIL;
		}

		TCHAR* pKey = new TCHAR[MIN_STR];
		lstrcpy(pKey, pObjKey);

		m_mapTexture.insert( make_pair(pKey, pTexture) );
	}

	return S_OK;
}

//int  Test();			//
//int* Test();		//
//int& Test();	//

const TEX_INFO* CTextureMgr::GetTexture(const TCHAR* pObjKey, 
										const TCHAR* pStateKey /*= L""*/,
										const int& iCount /*= 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if( m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObjKey) );

	if(iter == m_mapTexture.end())
		return NULL;

	return iter->second->GetTexture(pStateKey, iCount);
}

void CTextureMgr::Release(void)
{
	map<const TCHAR*, CTexture*>::iterator iter		= m_mapTexture.begin();
	map<const TCHAR*, CTexture*>::iterator iter_end = m_mapTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		SAFE_DELETE_ARRAY(iter->first);
		SAFE_DELETE(iter->second);
	}
	m_mapTexture.clear();
}
