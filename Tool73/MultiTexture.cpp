#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

//L"../Texture/Tile/Tile%d.png"
//L"Tile"

HRESULT CMultiTexture::InsertTexture(const TCHAR* pPath,
									 const TCHAR* pStateKey /*= L""*/, 
									 const int& iCount /*= 0*/)
{
	//printf("Count : %d ", i)

	TCHAR szPath[MAX_STR] = L"";
	
	vector<TEX_INFO*>	vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, pPath, i);

		TEX_INFO* pTextInfo = new TEX_INFO;
		ZeroMemory(pTextInfo, sizeof(TEX_INFO));

		if(FAILED( D3DXGetImageInfoFromFile(szPath, &pTextInfo->ImageInfo) ))
		{
			MessageBox(g_hWnd, pPath, L"Not Load!", MB_OK);
			return E_FAIL;
		}

		if(FAILED( D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szPath,
			pTextInfo->ImageInfo.Width,		//이미지 가로
			pTextInfo->ImageInfo.Height,		//이미지 세로
			pTextInfo->ImageInfo.MipLevels,	//3D에서 사용 : 이미지를 확대 / 축소
			NULL,		
			pTextInfo->ImageInfo.Format,		//이미지 상태
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,		//테두리 표현을 어떻게 할것인지에 대한 옵션.
			D3DX_DEFAULT,		//확대 / 축소하는 경우에 어떻게 처리를 할것인지에 대한 옵션
			NULL,			//제거할 이미지의 색상
			&pTextInfo->ImageInfo,		//이미지에 대한 정보를 어디에 담을 것인지?
			NULL,	//8비트 미만의 이미지
			&pTextInfo->pTexture ) ) )
		{
			MessageBox(g_hWnd, szPath, L"Multi Texture Load Failed!", MB_OK);
			return E_FAIL;
		}

		vecTexture.push_back(pTextInfo);
	}

	TCHAR* pKey = new TCHAR[MIN_STR];
	lstrcpy(pKey, pStateKey);


	m_mapMultiTexture.insert( make_pair(pKey, vecTexture) );

	return S_OK;
}

const TEX_INFO* CMultiTexture::GetTexture(const TCHAR* pKey /*= L""*/, 
										  const int& iCount /*= 0*/)
{
	
	map<const TCHAR*, vector<TEX_INFO*>>::iterator Finditer;

	Finditer = find_if( m_mapMultiTexture.begin(), m_mapMultiTexture.end(), STRING_COMPARE(pKey) );

	if(Finditer == m_mapMultiTexture.end())
		return NULL;

	return Finditer->second[iCount];
}

void CMultiTexture::Release(void)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter_end;

	iter	 = m_mapMultiTexture.begin();
	iter_end = m_mapMultiTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		//delete [] iter->first;
		SAFE_DELETE_ARRAY(iter->first);

		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();

			safe_delete(iter->second[i]);
		}
		iter->second.clear();
	}

	m_mapMultiTexture.clear();
}

CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}
