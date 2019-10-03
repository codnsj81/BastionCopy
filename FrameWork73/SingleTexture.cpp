#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"
#include "TextureMgr.h"
HRESULT CSingleTexture::InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey/* = L"" */, const int& iCount /*= 0*/)
{
	CSingleTexture::m_pTexInfo = new TEX_INFO;

	CTextureMgr::GetInstance()->m_fBarScale += 0.070f;

	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO));

	//경로에 있는 이미지 정보를 얻어온다.
	if(FAILED( D3DXGetImageInfoFromFile(pPath, &m_pTexInfo->ImageInfo) ))
	{
		MessageBox(g_hWnd, pPath, L"Not Load!", MB_OK);
		return E_FAIL;
	}

	//얻어온 정보로 이미지를 생성한다.
	if(FAILED( D3DXCreateTextureFromFileEx(
		CDevice::GetInstance()->GetDevice(),
		pPath,
		m_pTexInfo->ImageInfo.Width,		//이미지 가로
		m_pTexInfo->ImageInfo.Height,		//이미지 세로
		m_pTexInfo->ImageInfo.MipLevels,	//3D에서 사용 : 이미지를 확대 / 축소
		NULL,		//이미지의 읽는 방식을 설정하는 옵션 : 0
		m_pTexInfo->ImageInfo.Format,		//이미지 상태
		D3DPOOL_MANAGED,
		/*
		D3DPOOL_MANAGED : 그래픽 카드 메모리가 꽉차면 시스템 메모를 사용하도록 하는 옵션이다.
		
		D3DPOOL_DEFAULT : 그래픽 카드의 메모리가 꽉차면 출력이 불가능해진다.
		*/
		D3DX_DEFAULT,		//테두리 표현을 어떻게 할것인지에 대한 옵션.
		D3DX_DEFAULT,		//확대 / 축소하는 경우에 어떻게 처리를 할것인지에 대한 옵션
		NULL,			//제거할 이미지의 색상
		&m_pTexInfo->ImageInfo,		//이미지에 대한 정보를 어디에 담을 것인지?
		NULL,	//8비트 미만의 이미지
		&m_pTexInfo->pTexture ) ) )
	{
		MessageBox(g_hWnd, pPath, L"Single Texture Load Failed!", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

const TEX_INFO* CSingleTexture::GetTexture(const TCHAR* pKey /*= L""*/, const int& iCount /*= 0*/)
{
	return m_pTexInfo;
}

void CSingleTexture::Release(void)
{
	if(m_pTexInfo != NULL)
	{
		m_pTexInfo->pTexture->Release();
		safe_delete(m_pTexInfo);
	}
}

CSingleTexture::CSingleTexture(void)
:m_pTexInfo(NULL)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}
