
// Tool73View.h : CTool73View Ŭ������ �������̽�
//


#pragma once

//class CSingleTexture;
class CMonster;
class CBackGround;
class CObstacle;
class CTool73Doc;


class CTool73View : public CScrollView
{
private:
//	CSingleTexture* m_pSingleTexture;
	CBackGround*	m_pBackGround;
	CObstacle*		m_pObstacle;
	CMonster*		m_pMonster;


public:
	CMonster* GetMonster();
	CBackGround* GetBackGround();
	CObstacle*	GetObstacle();

protected: // serialization������ ��������ϴ�.
	CTool73View();
	DECLARE_DYNCREATE(CTool73View)

// Ư���Դϴ�.
public:
	CTool73Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTool73View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Tool73View.cpp�� ����� ����
inline CTool73Doc* CTool73View::GetDocument() const
   { return reinterpret_cast<CTool73Doc*>(m_pDocument); }
#endif

