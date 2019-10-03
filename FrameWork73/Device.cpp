#include "StdAfx.h"
#include "Device.h"

#include "Constant.h"
#include "NarrMgr.h"
IMPLEMENT_SINGLETON(CDevice);

/*
##장치 초기화
1.장치를 조사한다(특정성능에 대해서)
2.장치를 대표하는 객체를 생성한다.
3.장치를 대표하는 객체가 존재할수 있도록 클래스를 디자인한다.
4.전역적으로 사용하기 위해서 싱글톤으로 제작을 한다.
5.장치에 명령을 내리기 위해서는 제작한 클래스를 통해서 사용하게 된다.


##특정정보
-버텍스 프로세싱이라는 정보를 얻어오게 된다.

-버텍스란?
 ->점으로 객체를 늘리고 줄이고 이동하고 회전하고.. 가능하게 되는것디ㅏ...

버텍스 프로세싱 : 점 변환 + 조명 설정



*/

HRESULT CDevice::Initialize3D(void)
{
	m_pD3D = NULL;
	m_pD3dDevice = NULL;

	m_fZoom = 0.f;

	//D3D객체를 생성. -> D3dDevice를 사용하기 위해서.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//장치의 성능 조사. -> 저장하는 변수 선언.
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
									  D3DDEVTYPE_HAL, 
									  &DeviceCaps) ) )
	{
		//초기화가 실패한다면 여기로!
		//AfxMessageBox(L"장치 초기화 실패!");
		ERROR_TEXT(L"장치 초기화 실패!");
		return E_FAIL;
	}

	DWORD vp;

	//불러들인 정보에 따라서
	//점과 조명에 대한 연산을 하드웨어가 할지 소프트웨어가 할지를 판단하게 된다.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			//그래픽 카드를 지원을 한다면
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//Cpu
		//그래픽 카드를 지원하지 않는다면
	}

	//장치를 생성할때 개발자가 세팅해줘야하는 장치 정보
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dp);		//장치 정보를 설정!

	if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_hWnd,
									vp,
									&d3dp,
									&m_pD3dDevice)) )
	{
		//AfxMessageBox(L"디바이스 초기화 실패!");
		ERROR_TEXT(L"디바이스 초기화 실패!");
		return E_FAIL;
	}



	//스프라이트 객체 
	if(FAILED(D3DXCreateSprite( m_pD3dDevice, &m_pSprite )) )
	{
		//AfxMessageBox(L"스프라이트 초기화 실패!");
		ERROR_TEXT(L"스프라이트 초기화 실패!");
		return E_FAIL;
	}


	

	//폰트 객체 
	D3DXFONT_DESC FontInfo;                     // 폰트를 설정한다.
	FontInfo.CharSet              = HANGUL_CHARSET;                 // FaceName을 비워놓으면 Windows Default Font를 사용한다.
	FontInfo.Height                = 20;
	//FontInfo.FaceName				= L"휴먼옛체";
	FontInfo.Width                 = 10 ;
	FontInfo.Weight               = FW_BOLD;
	FontInfo.Quality               = DEFAULT_QUALITY;
	FontInfo.MipLevels          = 1;
	FontInfo.Italic                   = 0;
	FontInfo.OutputPrecision  = OUT_DEFAULT_PRECIS;
	FontInfo.PitchAndFamily   = FF_DONTCARE;


	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont )) )
	{
		ERROR_TEXT(L"폰트 초기화 실패!");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;
	d3dp.BackBufferHeight = WINCY;
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dp.BackBufferCount = 1;	//백버퍼 개수

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;		//이미지를 키웠다 줄였다 할때.. 뭉개지는현상을 보정할것인가?
	//사용하지 않음.

	d3dp.MultiSampleQuality = 0;


	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//백버퍼의 더블 버퍼링 방식을 설정.

	//버퍼 : 내가 그림을 그리고 있는 공간을 의미한다.
	//		무조건 기본 버퍼 한개를 가지고 있고, 백버퍼를 추가적으로 1개더 만들어 
	//		주게 되는데
	//      화면보이는 버퍼를 전면버퍼
	//		사용자(개발자)가 무언가 그리기 위한 버퍼는 후면버퍼이다.

	//내부적으로 각자의 포인터를 가지고 있다.
	//포인터들을 통해서 픽셀들에 접근을해서 색상정보를 변경하게 된다.

	//포인터가 교환되면서 이미지가 교체된 효과를 얻게 된다.
	
	//->스왑 체인을 통해서 버퍼에 그림을 그린다.

	
	d3dp.hDeviceWindow = g_hWnd;		//윈도우 핸들
	
	d3dp.Windowed = TRUE;		//전체화면(flase) / 창모드(TRUE)

	d3dp.EnableAutoDepthStencil = TRUE;
	//스텐실 버퍼
	//깊이 버퍼라고 어느정도 볼수가 있다.
	
	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//전체 모드로 실행하였을때 모니터의 주사율을 따라가도록 설정하는 옵션.

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//주사율과 프레임 사이에 간격을 어떨게 할것인지를 설정하는 옵션.

	//D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 바로 보여준다.

	//D3DPRESENT_INTERVAL_DEFAULT : 출력간격을 다이렉트가 자동적으로 설정하게 된다.
}

void CDevice::Render_Begin(void)
{
//## 렌더링 과정 : 후면버퍼의 렌더링후 전면버퍼로 교체한뒤 화면에 보여주는 과정을 거치게 된다.
//-> 1.버퍼를 지운다 
//-> 2.그린다
//-> 3.전면버퍼를 보여준다.

	m_pD3dDevice->Clear(0,	//버퍼의 개수
						NULL, //위치의 주소값
						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : 후면버퍼, 
						D3DCOLOR_ARGB(255, 0, 0, 255),		//어떤색으로?
						1,	//삭제한후에 깊이버퍼의 초기값
						0);	//스텐실 버퍼의 초기값

	m_pD3dDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);



}

void CDevice::Render_End(void)
{
	RECT pRect = {0,0, WINCX, WINCY};

	RECT pRect1 = {(int) -m_fZoom,(int) -m_fZoom, WINCX + (int) m_fZoom , WINCY + (int) m_fZoom};

	m_pSprite->End();

	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(&pRect, &pRect1, NULL, NULL);//4인자 : RGNDATA
	//1.2인자 : 렉트값을 요구하고 있는데, 이 두개의 렉트비율(크기차이)만큼 확대 / 축소
	//			 1번렉트가 작고, 2번렉트가 크다 : 확대
	//			 1번렉트가 크고, 2번렉트가 작은 : 축소

	//3인자 : 어떤윈도우 핸들에 화면을 출력할것인가?


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

