#pragma once

// CPathFind ��ȭ �����Դϴ�.
#include "Include.h"
#include "afxwin.h"

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

private:
	list<IMAGE_PATH*> m_listPathInfo;

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CListBox m_PathList;
	afx_msg void OnBnClickedButtonPathSave();
	afx_msg void OnBnClickedButtonPathLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonPathdelete();
};
