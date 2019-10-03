// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool73.h"
#include "Obstacle.h"
#include "MyForm.h"
#include "BackGround.h"
#include "Tool73View.h"
#include "MainFrm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
	, m_strName(_T(""))
	, m_strCpyName(_T(""))
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_MAP, &CMyForm::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON_UNIT, &CMyForm::OnBnClickedButtonUnit)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_PATHFINDER, &CMyForm::OnBnClickedButtonPathfinder)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ALL, &CMyForm::OnBnClickedButtonSaveAll)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ALL, &CMyForm::OnBnClickedButtonLoadAll)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_Font.CreatePointFont(150, L"�ü�");

}


void CMyForm::OnBnClickedButtonMap()
{

	if(m_MapTool.GetSafeHwnd() == NULL)
		m_MapTool.Create(IDD_MAPTOOL);

	m_MapTool.ShowWindow(SW_SHOW);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMyForm::OnBnClickedButtonUnit()
{

	if(m_UnitTool.GetSafeHwnd() == NULL)
		m_UnitTool.Create(IDD_UNITTOOL);

	m_UnitTool.ShowWindow(SW_SHOW);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMyForm::OnBnClickedButton2()
{
		if(m_EventTool.GetSafeHwnd() == NULL)
		m_EventTool.Create(IDD_EVENTTOOL);

	m_EventTool.ShowWindow(SW_SHOW);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMyForm::OnBnClickedButtonPathfinder()
{

	if(m_PathFind.GetSafeHwnd() == NULL)
		m_PathFind.Create(IDD_PATHFIND);

	m_PathFind.ShowWindow(SW_SHOW);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMyForm::OnBnClickedButtonSaveAll()
{
	CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();
	vector<PTILE>* pVecTile = pBackGround->GetTile();
	vector<PTILE>*	pVecGround = pBackGround->GetGround();

	CObstacle* pObstacle = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetObstacle();
	vector<pOBJECT_DATA>* pVecObject = pObstacle->GetObject();

	CFileDialog Dlg(
		FALSE
		,L"dat"
		,L"*.dat"
		,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		,L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return ;


	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";		//�����ų ���(������)


	HANDLE hFile = CreateFile( Dlg.GetPathName(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,		//������ ������ ����� ������ ������Ű�� �ɼ�
		FILE_ATTRIBUTE_NORMAL,// �⺻������ ������ ���� ���Ѷ�.
		NULL);

	DWORD dwByte;		//�����ų ���(������)

	int iSize = pVecTile->size();
	
	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i =0; i<iSize; i++)
	{
		
		WriteFile(hFile, (*pVecTile)[i], sizeof(TILE), &dwByte, NULL);

	}

	iSize = pVecGround->size();

	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i =0; i<iSize; i++)
	{

		WriteFile(hFile, (*pVecGround)[i], sizeof(TILE), &dwByte, NULL);

	}


	iSize = pVecObject->size();

	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i =0; i<iSize; i++)
	{

		WriteFile(hFile, (*pVecObject)[i], sizeof(OBJECT_DATA), &dwByte, NULL);

	}

	CloseHandle(hFile);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMyForm::OnBnClickedButtonLoadAll()
{
	

	CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();

	CObstacle* pObstacle = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetObstacle();
	vector<pOBJECT_DATA>* pOriginObject = pObstacle->GetObject();
	vector<pOBJECT_DATA>* pVecObject = new vector<pOBJECT_DATA>;

	for(size_t i=0; i< pOriginObject->size(); i++)
		safe_delete((*pOriginObject)[i]);


	HANDLE hFile = CreateFile( L"..\\Data\\MapTest.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// �⺻������ ������ ���� ���Ѷ�.
		NULL);

	DWORD dwByte;
	int iSize;
	PTILE pTile;

	vector<PTILE>* pVecTile = new vector<PTILE>;
	vector<PTILE>* pVecGround = new vector<PTILE>;

	vector<PTILE>* OriginVecGround = pBackGround->GetGround();
	vector<PTILE>* OriginVecTile = pBackGround->GetTile();

	for(size_t i=0; i< OriginVecTile->size(); i++)
		safe_delete((*OriginVecTile)[i]);

	OriginVecTile->clear();

	for(size_t i=0; i< OriginVecGround->size(); i++)
		safe_delete((*OriginVecGround)[i]);

	OriginVecTile->clear();
	OriginVecGround->clear();



	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		pVecTile->push_back(pTile);
	}
	pBackGround->SetTile(*pVecTile);



	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pTile = new TILE; 
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		pVecGround->push_back(pTile);
	}
	pBackGround->SetGround(*pVecGround);



	pOBJECT_DATA pData;
	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pData = new OBJECT_DATA; 
		ReadFile(hFile, pData, sizeof(OBJECT_DATA), &dwByte, NULL);
		pVecObject->push_back(pData);
	}
	pObstacle->SetObject(*pVecObject);



	CloseHandle(hFile);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
