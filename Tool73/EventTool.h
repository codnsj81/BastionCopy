#pragma once

#include "Include.h"
#include "afxwin.h"
// CEventTool ��ȭ �����Դϴ�.

class CEventTool : public CDialog
{
	DECLARE_DYNAMIC(CEventTool)


public:
	CEventTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEventTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EVENTTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

};
