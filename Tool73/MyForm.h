#pragma once


#include "UnitTool.h"
#include "MapTool.h"
#include "EventTool.h"
#include "PathFind.h"
// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

private:
	CUnitTool	m_UnitTool;
	CMapTool	m_MapTool;
	CEventTool	m_EventTool;
	CPathFind	m_PathFind;

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	CMapTool*	GetMapTool()
	{
		return &m_MapTool;
	}
	CUnitTool* GetUnitTool()
	{
		return &m_UnitTool;
	}
private:
	CFont		m_Font;

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CString m_strName;
	// 복사된 이름
	CString m_strCpyName;
	afx_msg void OnBnClickedButtonMap();
	afx_msg void OnBnClickedButtonUnit();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonPathfinder();
	afx_msg void OnBnClickedButtonSaveAll();
	afx_msg void OnBnClickedButtonLoadAll();
};


