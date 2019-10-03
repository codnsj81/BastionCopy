#include "StdAfx.h"
#include "Device.h"

#include "Constant.h"
#include "NarrMgr.h"
IMPLEMENT_SINGLETON(CDevice);

/*
##��ġ �ʱ�ȭ
1.��ġ�� �����Ѵ�(Ư�����ɿ� ���ؼ�)
2.��ġ�� ��ǥ�ϴ� ��ü�� �����Ѵ�.
3.��ġ�� ��ǥ�ϴ� ��ü�� �����Ҽ� �ֵ��� Ŭ������ �������Ѵ�.
4.���������� ����ϱ� ���ؼ� �̱������� ������ �Ѵ�.
5.��ġ�� ����� ������ ���ؼ��� ������ Ŭ������ ���ؼ� ����ϰ� �ȴ�.


##Ư������
-���ؽ� ���μ����̶�� ������ ������ �ȴ�.

-���ؽ���?
 ->������ ��ü�� �ø��� ���̰� �̵��ϰ� ȸ���ϰ�.. �����ϰ� �Ǵ°͵�...

���ؽ� ���μ��� : �� ��ȯ + ���� ����



*/

HRESULT CDevice::Initialize3D(void)
{
	m_pD3D = NULL;
	m_pD3dDevice = NULL;

	m_fZoom = 0.f;

	//D3D��ü�� ����. -> D3dDevice�� ����ϱ� ���ؼ�.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//��ġ�� ���� ����. -> �����ϴ� ���� ����.
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
									  D3DDEVTYPE_HAL, 
									  &DeviceCaps) ) )
	{
		//�ʱ�ȭ�� �����Ѵٸ� �����!
		//AfxMessageBox(L"��ġ �ʱ�ȭ ����!");
		ERROR_TEXT(L"��ġ �ʱ�ȭ ����!");
		return E_FAIL;
	}

	DWORD vp;

	//�ҷ����� ������ ����
	//���� ���� ���� ������ �ϵ��� ���� ����Ʈ��� ������ �Ǵ��ϰ� �ȴ�.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			//�׷��� ī�带 ������ �Ѵٸ�
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//Cpu
		//�׷��� ī�带 �������� �ʴ´ٸ�
	}

	//��ġ�� �����Ҷ� �����ڰ� ����������ϴ� ��ġ ����
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dp);		//��ġ ������ ����!

	if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_hWnd,
									vp,
									&d3dp,
									&m_pD3dDevice)) )
	{
		//AfxMessageBox(L"����̽� �ʱ�ȭ ����!");
		ERROR_TEXT(L"����̽� �ʱ�ȭ ����!");
		return E_FAIL;
	}



	//��������Ʈ ��ü 
	if(FAILED(D3DXCreateSprite( m_pD3dDevice, &m_pSprite )) )
	{
		//AfxMessageBox(L"��������Ʈ �ʱ�ȭ ����!");
		ERROR_TEXT(L"��������Ʈ �ʱ�ȭ ����!");
		return E_FAIL;
	}


	

	//��Ʈ ��ü 
	D3DXFONT_DESC FontInfo;                     // ��Ʈ�� �����Ѵ�.
	FontInfo.CharSet              = HANGUL_CHARSET;                 // FaceName�� ��������� Windows Default Font�� ����Ѵ�.
	FontInfo.Height                = 20;
	//FontInfo.FaceName				= L"�޸տ�ü";
	FontInfo.Width                 = 10 ;
	FontInfo.Weight               = FW_BOLD;
	FontInfo.Quality               = DEFAULT_QUALITY;
	FontInfo.MipLevels          = 1;
	FontInfo.Italic                   = 0;
	FontInfo.OutputPrecision  = OUT_DEFAULT_PRECIS;
	FontInfo.PitchAndFamily   = FF_DONTCARE;


	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont )) )
	{
		ERROR_TEXT(L"��Ʈ �ʱ�ȭ ����!");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;
	d3dp.BackBufferHeight = WINCY;
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dp.BackBufferCount = 1;	//����� ����

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;		//�̹����� Ű���� �ٿ��� �Ҷ�.. �������������� �����Ұ��ΰ�?
	//������� ����.

	d3dp.MultiSampleQuality = 0;


	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//������� ���� ���۸� ����� ����.

	//���� : ���� �׸��� �׸��� �ִ� ������ �ǹ��Ѵ�.
	//		������ �⺻ ���� �Ѱ��� ������ �ְ�, ����۸� �߰������� 1���� ����� 
	//		�ְ� �Ǵµ�
	//      ȭ�麸�̴� ���۸� �������
	//		�����(������)�� ���� �׸��� ���� ���۴� �ĸ�����̴�.

	//���������� ������ �����͸� ������ �ִ�.
	//�����͵��� ���ؼ� �ȼ��鿡 �������ؼ� ���������� �����ϰ� �ȴ�.

	//�����Ͱ� ��ȯ�Ǹ鼭 �̹����� ��ü�� ȿ���� ��� �ȴ�.
	
	//->���� ü���� ���ؼ� ���ۿ� �׸��� �׸���.

	
	d3dp.hDeviceWindow = g_hWnd;		//������ �ڵ�
	
	d3dp.Windowed = TRUE;		//��üȭ��(flase) / â���(TRUE)

	d3dp.EnableAutoDepthStencil = TRUE;
	//���ٽ� ����
	//���� ���۶�� ������� ������ �ִ�.
	
	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//��ü ���� �����Ͽ����� ������� �ֻ����� ���󰡵��� �����ϴ� �ɼ�.

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//�ֻ����� ������ ���̿� ������ ��� �Ұ������� �����ϴ� �ɼ�.

	//D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ٷ� �����ش�.

	//D3DPRESENT_INTERVAL_DEFAULT : ��°����� ���̷�Ʈ�� �ڵ������� �����ϰ� �ȴ�.
}

void CDevice::Render_Begin(void)
{
//## ������ ���� : �ĸ������ �������� ������۷� ��ü�ѵ� ȭ�鿡 �����ִ� ������ ��ġ�� �ȴ�.
//-> 1.���۸� ����� 
//-> 2.�׸���
//-> 3.������۸� �����ش�.

	m_pD3dDevice->Clear(0,	//������ ����
						NULL, //��ġ�� �ּҰ�
						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : �ĸ����, 
						D3DCOLOR_ARGB(255, 0, 0, 255),		//�������?
						1,	//�������Ŀ� ���̹����� �ʱⰪ
						0);	//���ٽ� ������ �ʱⰪ

	m_pD3dDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);



}

void CDevice::Render_End(void)
{
	RECT pRect = {0,0, WINCX, WINCY};

	RECT pRect1 = {(int) -m_fZoom,(int) -m_fZoom, WINCX + (int) m_fZoom , WINCY + (int) m_fZoom};

	m_pSprite->End();

	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(&pRect, &pRect1, NULL, NULL);//4���� : RGNDATA
	//1.2���� : ��Ʈ���� �䱸�ϰ� �ִµ�, �� �ΰ��� ��Ʈ����(ũ������)��ŭ Ȯ�� / ���
	//			 1����Ʈ�� �۰�, 2����Ʈ�� ũ�� : Ȯ��
	//			 1����Ʈ�� ũ��, 2����Ʈ�� ���� : ���

	//3���� : ������� �ڵ鿡 ȭ���� ����Ұ��ΰ�?


}

void CDevice::Render_End(HWND& rhWnd)
{
	m_pSprite->End();

	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, rhWnd, NULL);
}

void CDevice::Release(void)
{
	m_pSprite->Release();
	m_pD3dDevice->Release();
	m_pD3D->Release();
}

void CDevice::SetZoomOut( float _zoom )
{
	{
		if(m_fZoom >= 0)
		{
			m_fZoom -= _zoom;
		}

		else
		{
			CNarrMgr::GetInstance()->TurnOnNarr(2);
		}
	}

}


/*
CDevice::CDevice(void)
:m_pD3D(NULL)
,m_pD3dDevice(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}
*/

