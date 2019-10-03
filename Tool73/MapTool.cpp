// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "Tool73.h"
#include "MySheet.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
{
	m_pMySheet = NULL;

}

CMapTool::~CMapTool()
{
	safe_delete(m_pMySheet);
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
END_MESSAGE_MAP()

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, 700, 600);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.

}

// CMapTool 메시지 처리기입니다.
