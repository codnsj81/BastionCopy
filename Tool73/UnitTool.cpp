// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool73.h"
#include "MainFrm.h"
#include "UnitTool.h"
#include "TextureMgr.h"
#include "Tool73View.h"
// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(CUnitTool::IDD, pParent), m_iHp(0), m_iAtt(0) ,m_tInfo(NULL)
{
	LoadBitmap();
}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{

	m_vecBoss = 
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMonster()->GetBoss();

	m_vecMonster1 = 
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMonster()->GetMonster1();

	m_VecMonster2 = 
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMonster()->GetMonster2();



	DDX_Text(pDX, IDC_EDIT_MONSTER_HP, m_iHp);
	DDX_Text(pDX, IDC_EDIT_MONSTER_ATT, m_iAtt);

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MONSTER, m_MonsterCombo);

	if(m_MonsterCombo.GetCount() != 3)
	{
		m_MonsterCombo.AddString(L"BOSS");
		m_MonsterCombo.AddString(L"MONSTER1");
		m_MonsterCombo.AddString(L"MONSTER2");
	}
	DDX_Control(pDX, IDC_LIST1, m_UnitListBoc);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER, &CUnitTool::OnCbnSelchangeComboMonster)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ADMIT, &CUnitTool::OnBnClickedButtonMonsterAdmit)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_SAVE, &CUnitTool::OnBnClickedButtonMonsterSave)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_LOAD, &CUnitTool::OnBnClickedButtonMonsterLoad)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.

void CUnitTool::OnCbnSelchangeComboMonster()
{
	UpdateData(TRUE);

	int iClick = m_MonsterCombo.GetCurSel() ;

	map<CString, CBitmap*>::iterator iter;

	switch(iClick)
	{
	case 0:

		iter = m_mapBitmap.find(L"Boss" );

		if(iter == m_mapBitmap.end())
			return ;

		((CStatic*)GetDlgItem(IDC_STATIC_MONSTER))->SetBitmap( *(iter->second) );

		m_iHp = 180;
		m_iAtt = 5;

		break;


	case 1:
		iter = m_mapBitmap.find(L"Monster1" );

		if(iter == m_mapBitmap.end())
			return ;

		((CStatic*)GetDlgItem(IDC_STATIC_MONSTER))->SetBitmap( *(iter->second) );

		m_iHp = 70;
		m_iAtt = 10;

		break;

	case 2:

		iter = m_mapBitmap.find(L"Monster2" );

		if(iter == m_mapBitmap.end())
			return ;

		((CStatic*)GetDlgItem(IDC_STATIC_MONSTER))->SetBitmap( *(iter->second) );

		m_iHp = 60;
		m_iAtt = 8;


		break;

	}

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CUnitTool::LoadBitmap()
{
	//BOSS
	wstring pwstringImagePath = L"../Texture/Monster/Boss/Stand/0.png" ;
	wstring pwstrObjKey = L"Boss";
	TILE_PATH* pTilePath = new TILE_PATH;

	pTilePath->wstrImagePath = pwstringImagePath;
	pTilePath->wstrObjKey = pwstrObjKey;

		m_vecMonsterPath.push_back(pTilePath);


	CImage Image;

	Image.Load(pTilePath->wstrImagePath.c_str());

	CBitmap* pBitmap = new CBitmap;

	pBitmap->Attach( Image.Detach() );
	//pBitmap->Detach();	-> HBitmap을 반환한다.
	//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

	m_mapBitmap.insert( make_pair(pTilePath->wstrObjKey.c_str(), pBitmap) );

	//MONSTER1


	pwstringImagePath = L"../Texture/Monster/Monster1/Dir_B/0.png" ;
	pwstrObjKey = L"Monster1";
	pTilePath = new TILE_PATH;

	pTilePath->wstrImagePath = pwstringImagePath;
	pTilePath->wstrObjKey = pwstrObjKey;

	m_vecMonsterPath.push_back(pTilePath);


	Image.Load(pTilePath->wstrImagePath.c_str());

	pBitmap = new CBitmap;

	pBitmap->Attach( Image.Detach() );
	//pBitmap->Detach();	-> HBitmap을 반환한다.
	//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

	m_mapBitmap.insert( make_pair(pTilePath->wstrObjKey.c_str(), pBitmap) );

	//MONSTER2



	pwstringImagePath = L"../Texture/Monster/Monster2/Dir_B/0.png" ;
	pwstrObjKey = L"Monster2";
	pTilePath = new TILE_PATH;

	pTilePath->wstrImagePath = pwstringImagePath;
	pTilePath->wstrObjKey = pwstrObjKey;

	m_vecMonsterPath.push_back(pTilePath);


	Image.Load(pTilePath->wstrImagePath.c_str());

	pBitmap = new CBitmap;

	pBitmap->Attach( Image.Detach() );
	//pBitmap->Detach();	-> HBitmap을 반환한다.
	//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

	m_mapBitmap.insert( make_pair(pTilePath->wstrObjKey.c_str(), pBitmap) );

}

void CUnitTool::OnBnClickedButtonMonsterAdmit()
{
	int iClick = m_MonsterCombo.GetCurSel();
	g_choosing = MONSTER_DRAWING;

	if(m_tInfo == NULL)
	{
		m_tInfo = new UNIT_DATA;
		m_tInfo->bChoosing = false;
		m_tInfo->bFloating = true;
		
		switch(iClick)
		{
		case 0:
			m_vecBoss->push_back(m_tInfo);
			break;
		case 1:
			m_vecMonster1->push_back(m_tInfo);
			break;
		case 2:
			m_VecMonster2->push_back(m_tInfo);
			break;
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CUnitTool::MovingMonster( D3DXVECTOR3 vPos )
{
	if(m_tInfo != NULL && m_tInfo->bFloating == true)
		m_tInfo->tInfo.vPos = vPos;

}

void CUnitTool::FastenMonster()
{
	if(m_tInfo != NULL)
	{
		m_tInfo->bFloating = false;
		m_tInfo->iHp = m_iHp;
		m_tInfo->iAttack = m_iAtt;

		m_tInfo = NULL;
	}
}

void CUnitTool::OnBnClickedButtonMonsterSave()
{
		CFileDialog Dlg(
			FALSE
			,L"dat"
			,L"*.dat"
			,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
			,L"*.dat", this);

		if(Dlg.DoModal() == IDCANCEL)
			return ;


		Dlg.m_ofn.lpstrInitialDir = L"..\\Data";		//저장시킬 경로(절대경로)


		HANDLE hFile = CreateFile( Dlg.GetPathName(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,		//파일이 있으면 덮어쓰고 없으면 생성시키는 옵션
			FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
			NULL);

		DWORD dwByte;		//저장시킬 경로(절대경로)

		//Boss
		int iSize = m_vecBoss->size();

		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i =0; i<iSize; i++)
		{

			WriteFile(hFile, (*m_vecBoss)[i], sizeof(UNIT_DATA), &dwByte, NULL);

		}

		
		//Monster1
		iSize = m_vecMonster1->size();

		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i =0; i<iSize; i++)
		{

			WriteFile(hFile, (*m_vecMonster1)[i], sizeof(UNIT_DATA), &dwByte, NULL);

		}

		
		//Monster2
		iSize = m_VecMonster2->size();

		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i =0; i<iSize; i++)
		{

			WriteFile(hFile, (*m_VecMonster2)[i], sizeof(UNIT_DATA), &dwByte, NULL);

		}

		CloseHandle(hFile);

	

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CUnitTool::OnBnClickedButtonMonsterLoad()
{
	vector<pUNIT_DATA>* pBossData = new vector<pUNIT_DATA>;
	vector<pUNIT_DATA>* pMonster1Data = new vector<pUNIT_DATA>;
	vector<pUNIT_DATA>* pMonster2Data = new vector<pUNIT_DATA>;


		HANDLE hFile = CreateFile( L"..\\Data\\UnitData.dat",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
			NULL);

		DWORD dwByte;
		int iSize;

		for(size_t i=0; i< m_vecBoss->size(); i++)
			safe_delete((*m_vecBoss)[i]);

		m_vecBoss->clear();

		for(size_t i=0; i< m_vecMonster1->size(); i++)
			safe_delete((*m_vecMonster1)[i]);

		m_vecMonster1->clear();


		for(size_t i=0; i< m_VecMonster2->size(); i++)
			safe_delete((*m_VecMonster2)[i]);

		m_VecMonster2->clear();

		pUNIT_DATA pUnitData;

		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i<iSize; i++)
		{	
			pUnitData = new UNIT_DATA; 
			ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
			m_vecBoss->push_back(pUnitData);
		}

		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i<iSize; i++)
		{	
			pUnitData = new UNIT_DATA; 
			ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
			m_vecMonster1->push_back(pUnitData);
		}


		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i<iSize; i++)
		{	
			pUnitData = new UNIT_DATA; 
			ReadFile(hFile, pUnitData, sizeof(UNIT_DATA), &dwByte, NULL);
			m_VecMonster2->push_back(pUnitData);
		}


		CloseHandle(hFile);
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
