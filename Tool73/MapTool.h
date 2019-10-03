#pragma once

#include "Include.h"
#include "Tool73.h"
#include "MySheet.h"

// CMapTool ��ȭ �����Դϴ�.

class CTileTool;


class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMySheet* m_pMySheet;

	CTileTool* GetTileTool()
	{
		return m_pMySheet->GetTileTool();
	}

	CObjectTool*	GetObjectTool()
	{
		return m_pMySheet->GetObjectTool();
	}

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
};
