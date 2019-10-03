#pragma once



#include "TileTool.h"
#include "ObjectTool.h"

// CMySheet

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)
public:
	CTileTool*	GetTileTool()
	{
		return &m_TileTool;
	}

	CObjectTool* GetObjectTool()
	{
		return &m_ObjectTool;
	}
private:

	CObjectTool m_ObjectTool;
	CTileTool m_TileTool;

public:
	CMySheet(void);
	CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()
};


