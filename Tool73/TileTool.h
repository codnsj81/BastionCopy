#pragma once
#include "afxwin.h"
#include "Include.h"
#include "afxcmn.h"

class CBackGround;
// CTileTool 대화 상자입니다.

class CTileTool : public CPropertyPage
{
	DECLARE_DYNAMIC(CTileTool)

private:
	CString					m_TilePath;
	vector<TILE_PATH*>		m_vecTilePath;
	map<CString, CBitmap*>	m_mapBitmap;

	vector<PTILE>		m_vecChoosingTile;
	PTILE				m_pNewGround;
	bool			m_bTilePathFull;
	CBackGround*	m_pBackGround;

private:
	void Release();

public:
	bool					m_bCollision;
	int						m_DrawID;

public:
	void FastenGround();
	void NewGround(D3DXVECTOR3 vPoint);
	void SetChoosingTile(PTILE pTile)
	{
		m_vecChoosingTile.push_back(pTile);
	}
	void CancelChoosingTile(PTILE pTile);
	void LoadTileBitmap(void);
	void LoadGroundBitmap(void);
	void ChooseGround(int iIndex);
	PTILE				m_pChoosingGround;

public:
	CTileTool();
	virtual ~CTileTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TILETOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_TileListBox;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeListboxTile();
	afx_msg void OnBnClickedButtonTilesetsave();

public:

	CSliderCtrl m_HighSlider;
	CSliderCtrl m_ScaleSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTileErase();
	CComboBox m_MapComboBox;
	afx_msg void OnCbnSelchangeComboMap();
	CSliderCtrl m_GaroSlider;
	CButton m_ButtonCollsion;
	afx_msg void OnBnClickedCheckTileCollision();
	afx_msg void OnBnClickedButtonTilesave();
	afx_msg void OnBnClickedButtonTileLoad();
};
