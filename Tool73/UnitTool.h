#pragma once

#include "resource.h"
#include "afxwin.h"
#include "Include.h"
#include "Monster.h"
// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

private:
	map<CString, CBitmap*>	m_mapBitmap;
	CString					m_MonsterPath;
	vector<TILE_PATH*>		m_vecMonsterPath;
	pUNIT_DATA				m_tInfo;
	vector<pUNIT_DATA>		m_vecInfo;

	vector<pUNIT_DATA>*		m_vecBoss;
	vector<pUNIT_DATA>*		m_vecMonster1;
	vector<pUNIT_DATA>*		m_VecMonster2;

	int						m_iHp;
	int						m_iAtt;
	
public:
	void LoadBitmap();
	void FastenMonster();
	void MovingMonster(D3DXVECTOR3 vPos);

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNITTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_MonsterCombo;
	afx_msg void OnCbnSelchangeComboMonster();
	afx_msg void OnBnClickedButtonMonsterAdmit();
	CListBox m_UnitListBoc;
	afx_msg void OnBnClickedButtonMonsterSave();
	afx_msg void OnBnClickedButtonMonsterLoad();
};
