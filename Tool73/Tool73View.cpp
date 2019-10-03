
// Tool73View.cpp : CTool73View Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTool73View::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CTool73View ����/�Ҹ�

CTool73View::CTool73View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTool73View::~CTool73View()
{
	safe_delete(m_pBackGround);
	safe_delete(m_pObstacle);

	//CDevice::GetInstance()->Destroy();
}

BOOL CTool73View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CTool73View �׸���

void CTool73View::OnDraw(CDC* pDC)
{
	CTool73Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	
	CDevice::GetInstance()->Render_Begin();

	m_pBackGround->Render();
	m_pObstacle->Render();
	m_pMonster->Render();

	CDevice::GetInstance()->Render_End();
}


// CTool73View �μ�


void CTool73View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTool73View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTool73View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTool73View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CTool73View ����

#ifdef _DEBUG
void CTool73View::AssertValid() const
{
	CView::AssertValid();
}

void CTool73View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool73Doc* CTool73View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool73Doc)));
	return (CTool73Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool73View �޽��� ó����

void CTool73View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	//_CrtSetBreakAlloc( 12201 );
	g_hWnd = m_hWnd;

	//##â �׵θ��� ��� ���� �Լ�.
	
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY / 2) * TILEY ) );

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	pMainFrame->GetWindowRect(&rcWindow);
	//��ü �������� ũ�⸦ ������ �Լ�.

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
	//�������� ũ�⸦ 0,0���� �������� �������Ѵ�.

	RECT rcMainView;

	GetClientRect(&rcMainView);
	//���� Viewâ�� ũ�⸦ ��� ���� �Լ�.


	//���� ������ ũ��
	float fX = float(rcWindow.right - rcMainView.right);

	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL, //������ ũ�⸦ ����� ���·� �����ġ�� �����Ѱ������� ���� ����
		0,
		0,
		WINCX + (int) fX,
		WINCY + (int) fY,
		SWP_NOZORDER);		







	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/HitBox0.png", L"HitBox0", TEX_SINGLE, L"HitBox0")))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/HitBox1.png", L"HitBox1", TEX_SINGLE, L"HitBox1")))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Monster/Monster1/Dir_B/0.png", L"Monster1", TEX_SINGLE, L"Monster1")))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Monster/Monster2/Dir_B/0.png", L"Monster2", TEX_SINGLE, L"Monster2")))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}


	
	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png", L"Back", TEX_MULTI, L"Tile", 124)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}

	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/TileOption%d.png", L"Blind", TEX_MULTI, L"Tile", 3)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/Ground/Ground%d.png", L"Ground", TEX_MULTI, L"Ground", 11)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/Object/Default/Object%d.png", L"Object", TEX_MULTI, L"Object", 70)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/TileOption/TileOption%d.png", L"Option", TEX_MULTI, L"Option", 6)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
		return;
	}


	if( FAILED(CTextureMgr::GetInstance()
		->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png", L"Back", TEX_MULTI, L"BackGround", 3)))
	{
		AfxMessageBox(L"��Ƽ�ؽ��� �ε� ����!");
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CDevice::GetInstance()->Destroy();

	CTextureMgr::GetInstance()->Destroy();

	//safe_delete(m_pSingleTexture);
}

void CTool73View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


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

