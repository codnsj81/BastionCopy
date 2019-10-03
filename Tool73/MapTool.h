#pragma once

#include "Include.h"
#include "Tool73.h"
#include "MySheet.h"

// CMapTool 대화 상자입니다.

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
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
};
