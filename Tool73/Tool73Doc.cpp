
// Tool73Doc.cpp : CTool73Doc Ŭ������ ����
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


// CTool73Doc ����/�Ҹ�

CTool73Doc::CTool73Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CTool73Doc::~CTool73Doc()
{
}

BOOL CTool73Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CTool73Doc serialization

void CTool73Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CTool73Doc ����

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


// CTool73Doc ���
