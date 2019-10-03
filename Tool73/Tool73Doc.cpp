
// Tool73Doc.cpp : CTool73Doc 클래스의 구현
//

#include "stdafx.h"
#include "Tool73.h"

#include "Tool73Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTool73Doc

IMPLEMENT_DYNCREATE(CTool73Doc, CDocument)

BEGIN_MESSAGE_MAP(CTool73Doc, CDocument)
END_MESSAGE_MAP()


// CTool73Doc 생성/소멸

CTool73Doc::CTool73Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTool73Doc::~CTool73Doc()
{
}

BOOL CTool73Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTool73Doc serialization

void CTool73Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTool73Doc 진단

#ifdef _DEBUG
void CTool73Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTool73Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTool73Doc 명령
