#pragma once
#include "afxwin.h"
#include "Include.h"
#include "afxcmn.h"

class CObstacle;
// CObjectTool 대화 상자입니다.

class CObjectTool : public CPropertyPage
{
	DECLARE_DYNAMIC(CObjectTool)

private:

private:
	CString					m_ObjectPath;
	vector<pOBJECT_DATA>*	m_vecObject;
	pOBJECT_DATA			m_ChoosingObject;
	vector<TILE_PATH*>		m_vecObjectPath;
	vector<PTILE>			m_HitBoxTile;

	CObstacle*				m_pObstacle;

	pOBJECT_DATA			m_Object;
	map<CString, CBitmap*>	m_mapBitmap;

	bool						m_bCollision;
	bool						m_bDestory;
	int							m_iHp;

	int							m_iDrawID;
	float						m_fXPos;
	float						m_fYPos;

public:
	CObjectTool();
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJECTTOOL };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Release(void);
	void FastenObj(void);
	void GetTileIndex(D3DXVECTOR3 _vPos);

public:
	void NewObjectMove(D3DXVECTOR3 vPos);
	void ObjectSelect(D3DXVECTOR3 vPos);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg void OnBnClickedCheckObjDestroy();
	afx_msg void OnBnClickedCheckObjCollision();
	afx_msg void OnBnClickedButtonObjectadmit();
	CListBox m_ObjectList;
	afx_msg void OnBnClickedButtonAddHitbox();
	CSliderCtrl m_OBJScaleSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonObjerase();
	afx_msg void OnBnClickedButtonObjhitbox();
	CSliderCtrl m_HitBoxHorizon;
	CSliderCtrl m_HitBoxVertical;
	afx_msg void OnBnClickedButtonObjsetsave();
	afx_msg void OnBnClickedButtonObjLoad();
	CSliderCtrl m_SliderX2;
	CSliderCtrl m_SliederY2;
};
