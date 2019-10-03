// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool73.h"
#include "MiniView.h"
#include "Device.h"
#include "BackGround.h"
#include "mainFrm.h"
#include "Tool73View.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();


	CDevice::GetInstance()->Render_Begin();

	pBackGround->MiniViewRender();

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.