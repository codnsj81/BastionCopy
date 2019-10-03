
// Tool73View.cpp : CTool73View 클래스의 구현
//

#include "stdafx.h"
#include "Tool73.h"

#include "Tool73Doc.h"
#include "Tool73View.h"
#include "Include.h"
#include "MiniView.h"
#include "ObjectTool.h"
#include "MyForm.h"
#include "Obstacle.h"
#include "Device.h"
//#include "SingleTexture.h"
#include "UnitTool.h"
#include "TileTool.h"
#include "TextureMgr.h"

#include "BackGround.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CTool73View, CScrollView)

BEGIN_MESSAGE_MAP(CTool73View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTool73View::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CTool73View 생성/소멸

CTool73View::CTool73View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTool73View::~CTool73View()
{
	safe_delete(m_pBackGround);
	safe_delete(m_pObstacle);

	//CDevice::GetInstance()->Destroy();
}

BOOL CTool73View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTool73View 그리기

void CTool73View::OnDraw(CDC* pDC)
{
	CTool73Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	
	CDevice::GetInstance()->Render_Begin();

	m_pBackGround->Render();
	m_pObstacle->Render();
	m_pMonster->Render();

	CDevice::GetInstance()->Render_End();
}


// CTool73View 인쇄


void CTool73View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTool73View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTool73View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTool73View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CTool73View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTool73View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTool73View 진단

#ifdef _DEBUG
void CTool73View::AssertValid() const
{
	CView::AssertValid();
}

void CTool73View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool73Doc* CTool73View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool73Doc)));
	return (CTool73Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool73View 메시지 처리기

void CTool73View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	//_CrtSetBreakAlloc( 12201 );
	g_hWnd = m_hWnd;

	//##창 테두리를 얻어 오는 함수.
	
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY / 2) * TILEY ) );

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	pMainFrame->GetWindowRect(&rcWindow);
	//전체 프레임의 크기를 언어오는 함수.

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
	//프레임의 크기를 0,0으로 기준으로 재조정한다.

	RECT rcMainView;

	GetClientRect(&rcMainView);
	//순수 View창의 크기를 얻어 오는 함수.


	//가로 프레임 크기
	float fX = float(rcWindow.right - rcMainView.right);

	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL, //프레임 크기를 계산한 상태로 출력위치를 결정한것인지에 대한 인자
		0,
		0,
		WINCX + (int) fX,
		WINCY + (int) fY,
		SWP_NOZORDER);		







	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(FAILED( CDevice::GetInstance()->Initialize3D() ))
	{
		AfxMessageBox(L"Device Create Failed!");
		return;
	}

	//m_pSingleTexture = new CSingleTexture;
	//m_pSingleTexture->InsertTexture(L"../Texture/Cube.png");
	//m_pSingleTexture->InsertTexture(L"../Texture/Tile/Tile14.png");

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();
	m_pBackGround->SetMainView(this);

	m_pObstacle = new CObstacle;
	m_pObstacle->Initialize();
	m_pObstacle->SetMainView(this);

	m_pMonster = new CMonster;
	m_pMonster->Initialize();
	m_pMonster->SetMainView(this);


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Monster/Boss/Stand/0.png", L"Boss", TEX_SINGLE, L"Boss")))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/HitBox0.png", L"HitBox0", TEX_SINGLE, L"HitBox0")))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/HitBox1.png", L"HitBox1", TEX_SINGLE, L"HitBox1")))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Monster/Monster1/Dir_B/0.png", L"Monster1", TEX_SINGLE, L"Monster1")))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Monster/Monster2/Dir_B/0.png", L"Monster2", TEX_SINGLE, L"Monster2")))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


	
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png", L"Back", TEX_MULTI, L"Tile", 124)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/TileOption%d.png", L"Blind", TEX_MULTI, L"Tile", 3)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/Ground/Ground%d.png", L"Ground", TEX_MULTI, L"Ground", 11)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Object/Default/Object%d.png", L"Object", TEX_MULTI, L"Object", 70)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/TileOption/TileOption%d.png", L"Option", TEX_MULTI, L"Option", 6)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png", L"Back", TEX_MULTI, L"BackGround", 3)))
	{
		AfxMessageBox(L"멀티텍스쳐 로드 실패!");
		return;
	}


}

CBackGround* CTool73View::GetBackGround()
{
	return m_pBackGround;
}

void CTool73View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CDevice::GetInstance()->Destroy();

	CTextureMgr::GetInstance()->Destroy();

	//safe_delete(m_pSingleTexture);
}

void CTool73View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
//	CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
	
	CTileTool* pTileTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetTileTool();
	CObjectTool* pObjectTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetObjectTool();
	CUnitTool* pUnitTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetUnitTool();

	D3DXVECTOR3 vMousePos = D3DXVECTOR3((float) point.x + GetScrollPos(0), (float) point.y + GetScrollPos(1), 0.f);

	if(g_choosing == TILE_DRAWING)
	{
		m_pBackGround->TileChange(vMousePos, pTileTool->m_DrawID);
		Invalidate(FALSE);
	}

	CMiniView* pMiniView = ((CMainFrame*)AfxGetMainWnd())->GetMiniView();


	pMiniView->Invalidate(FALSE);

	if(g_choosing == GROUND_DRAWING)
		pTileTool->FastenGround();

	else if(g_choosing == OBJ_DRAWING)
		pObjectTool->FastenObj();

	if(g_choosing == OBJ_HITBOXDRAWING)
	{
		pObjectTool->GetTileIndex(vMousePos);
	}


	if(g_choosing == MONSTER_DRAWING)
	{
		pUnitTool->FastenMonster();
	}
	CView::OnLButtonDown(nFlags, point);

}

void CTool73View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CScrollView::OnRButtonDown(nFlags, point);

	D3DXVECTOR3 vMousePos = D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f);

	if(g_choosing == TILE_DRAWING)
	{
		CTileTool* pTileTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetTileTool();
		m_pBackGround->TileChoose(vMousePos);
	}

	if(g_choosing ==OBJ_DRAWING)
	{
		CObjectTool* pObjectTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetObjectTool();
		pObjectTool->ObjectSelect(vMousePos);

	}

	if(g_choosing == GROUND_DRAWING)
	{
		CTileTool* pTileTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetTileTool();
		
		if(pTileTool->m_pChoosingGround != NULL)
			pTileTool->m_pChoosingGround  = NULL;
		
		else
		{
			int i = m_pBackGround->GetGroundIndex(vMousePos);
			
			if(i != -1)
				pTileTool->ChooseGround(i);
		}

	}
	Invalidate(FALSE);

}

void CTool73View::OnMouseMove( UINT nFlags, CPoint point )
{

	CScrollView::OnMouseMove(nFlags, point);

	if(g_choosing == GROUND_DRAWING)
	{

		CTileTool* pTileTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetTileTool();

		D3DXVECTOR3 vMousePos = D3DXVECTOR3((float)  point.x + GetScrollPos(0), (float)  point.y + GetScrollPos(1), 0.f);

		pTileTool->NewGround(vMousePos);
		Invalidate(FALSE);

	}

	if(g_choosing == OBJ_DRAWING)
	{

		CObjectTool* pObjectTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool()->GetObjectTool();

		D3DXVECTOR3 vMousePos = D3DXVECTOR3((float) point.x + GetScrollPos(0), (float) point.y + GetScrollPos(1), 0.f);

		pObjectTool->NewObjectMove(vMousePos);


	}

	if(g_choosing == MONSTER_DRAWING)
	{
		CUnitTool* pUnitTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetUnitTool();

		D3DXVECTOR3 vMousePos = D3DXVECTOR3((float) point.x + GetScrollPos(0), (float) point.y + GetScrollPos(1), 0.f);


		pUnitTool->MovingMonster(vMousePos);
		Invalidate(FALSE);
	}
}

CObstacle* CTool73View::GetObstacle()
{
	return m_pObstacle;

}

CMonster* CTool73View::GetMonster()
{
	return m_pMonster;
}

