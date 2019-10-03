#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"
#include "TextureMgr.h"
HRESULT CSingleTexture::InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey/* = L"" */, const int& iCount /*= 0*/)
{
	CSingleTexture::m_pTexInfo = new TEX_INFO;

	CTextureMgr::GetInstance()->m_fBarScale += 0.070f;

	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO));

	//��ο� �ִ� �̹��� ������ ���´�.
	if(FAILED( D3DXGetImageInfoFromFile(pPath, &m_pTexInfo->ImageInfo) ))
	{
		MessageBox(g_hWnd, pPath, L"Not Load!", MB_OK);
		return E_FAIL;
	}

	//���� ������ �̹����� �����Ѵ�.
	if(FAILED( D3DXCreateTextureFromFileEx(
		CDevice::GetInstance()->GetDevice(),
		pPath,
		m_pTexInfo->ImageInfo.Width,		//�̹��� ����
		m_pTexInfo->ImageInfo.Height,		//�̹��� ����
		m_pTexInfo->ImageInfo.MipLevels,	//3D���� ��� : �̹����� Ȯ�� / ���
		NULL,		//�̹����� �д� ����� �����ϴ� �ɼ� : 0
		m_pTexInfo->ImageInfo.Format,		//�̹��� ����
		D3DPOOL_MANAGED,
		/*
		D3DPOOL_MANAGED : �׷��� ī�� �޸𸮰� ������ �ý��� �޸� ����ϵ��� �ϴ� �ɼ��̴�.
		
		D3DPOOL_DEFAULT : �׷��� ī���� �޸𸮰� ������ ����� �Ұ���������.
		*/
		D3DX_DEFAULT,		//�׵θ� ǥ���� ��� �Ұ������� ���� �ɼ�.
		D3DX_DEFAULT,		//Ȯ�� / ����ϴ� ��쿡 ��� ó���� �Ұ������� ���� �ɼ�
		NULL,			//������ �̹����� ����
		&m_pTexInfo->ImageInfo,		//�̹����� ���� ������ ��� ���� ������?
		NULL,	//8��Ʈ �̸��� �̹���
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
