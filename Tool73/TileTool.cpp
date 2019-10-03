// TileTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool73.h"
#include "TextureMgr.h"
#include "Tool73View.h"
#include "TileTool.h"
#include "BackGround.h"
#include "MainFrm.h"

// CTileTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTileTool, CPropertyPage)

CTileTool::CTileTool()
	: CPropertyPage(CTileTool::IDD)
{

	m_pChoosingGround = NULL;
	m_bTilePathFull = false;
	m_pNewGround = NULL;
}

CTileTool::~CTileTool()
{
	Release();
}

void CTileTool::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_LISTBOX_TILE, m_TileListBox);
	DDX_Control(pDX, IDC_SLIDER_HIGH, m_HighSlider);
	DDX_Control(pDX, IDC_SLIDER_TILESIZE, m_ScaleSlider);
	DDX_Control(pDX, IDC_SLIDER_GARO, m_GaroSlider);

	m_HighSlider.SetRange(-50, 50);
	m_HighSlider.SetPos(0);
	m_HighSlider.SetTicFreq(10);

	m_ScaleSlider.SetRange(1,20);
	m_ScaleSlider.SetPos(10);
	m_ScaleSlider.SetTicFreq(2);

	m_GaroSlider.SetRange(-50,50);
	m_GaroSlider.SetPos(0);
	m_GaroSlider.SetTicFreq(10);

	DDX_Control(pDX, IDC_COMBO_MAP, m_MapComboBox);

	if(m_MapComboBox.GetCount() != 2)
	{
		m_MapComboBox.AddString(L"TILE");
		m_MapComboBox.AddString(L"GROUND");

	}

	DDX_Control(pDX, IDC_CHECK_TILE_COLLISION, m_ButtonCollsion);
}


BEGIN_MESSAGE_MAP(CTileTool, CPropertyPage)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LISTBOX_TILE, &CTileTool::OnLbnSelchangeListboxTile)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TILE_ERASE, &CTileTool::OnBnClickedButtonTileErase)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP, &CTileTool::OnCbnSelchangeComboMap)
	ON_BN_CLICKED(IDC_CHECK_TILE_COLLISION, &CTileTool::OnBnClickedCheckTileCollision)
	ON_BN_CLICKED(IDC_BUTTON_TILESAVE, &CTileTool::OnBnClickedButtonTilesave)
	ON_BN_CLICKED(IDC_BUTTON_TILE_LOAD, &CTileTool::OnBnClickedButtonTileLoad)
END_MESSAGE_MAP()


// CTileTool 메시지 처리기입니다.

void CTileTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pBackGround =  ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();
	
	CDialog::OnDropFiles(hDropInfo);


	UpdateData(TRUE);

	TCHAR szFullPath[MAX_STR] = L"";	
	TCHAR szName[MAX_STR] = L"";		

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	for(int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_STR);

		lstrcpy(szName, PathFindFileName(szFullPath));

		wstring strName = szName;
		
		//.png를 삭제하도록 하자.
		int iIndex = strName.find(L".png");

		strName.erase(iIndex, 4);
		//확장자가 제거됨.

		//상대경로.
		wstring wstrFullPath = RelativePath(szFullPath);


		//비트맵 만들기
		map<CString, CBitmap*>::iterator iter;

		iter = m_mapBitmap.find( strName.c_str() );

		if(iter != m_mapBitmap.end())
			return;

		TILE_PATH* pTemp = new TILE_PATH;

		pTemp->wstrObjKey = strName;			//확장자를 제거한 파일이름.
		pTemp->wstrImagePath = wstrFullPath;		//상대경로로 변경한 경로를 저장.


		m_vecTilePath.push_back(pTemp);

		m_TileListBox.AddString( pTemp->wstrObjKey.c_str() );

		CTextureMgr::GetInstance()->InsertTexture(
			wstrFullPath.c_str(), 
			strName.c_str(), TEX_SINGLE);

		CImage Image;

		Image.Load(wstrFullPath.c_str());

		CBitmap* pBitmap = new CBitmap;

		pBitmap->Attach( Image.Detach() );
		//pBitmap->Detach();	-> HBitmap을 반환한다.
		//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

		m_mapBitmap.insert( make_pair(strName.c_str(), pBitmap) );
	}



	UpdateData(FALSE);
	CPropertyPage::OnDropFiles(hDropInfo);
}

void CTileTool::OnLbnSelchangeListboxTile()
{

	UpdateData(TRUE);

	g_choosing = TILE_DRAWING;

	int iIndex = m_TileListBox.GetCurSel();

	map<CString, CBitmap*>::iterator iter;

	iter = m_mapBitmap.find( m_vecTilePath[iIndex]->wstrObjKey.c_str() );

	if(iter == m_mapBitmap.end())
		return ;

	((CStatic*)GetDlgItem(IDC_TILE_IMAGE))->SetBitmap( *(iter->second) );


	if(m_MapComboBox.GetCurSel()== 0)
	{
		g_choosing = TILE_DRAWING;

		CString szID = (iter->first);
		szID.Delete(0,4);
		m_DrawID = _ttoi(szID);
		//szID.delete(0,4);

	}



	if(m_MapComboBox.GetCurSel()== 1)
	{
		CString szID = (iter->first);
		szID.Delete(0,6);
		m_DrawID = _ttoi(szID);
		//szID.delete(0,4);
	

		if(m_pNewGround != NULL && m_pNewGround->byOption == 0)
		{
			safe_delete(m_pNewGround);
			m_pBackGround->GetGround()->pop_back();
		}

		g_choosing = GROUND_DRAWING;
		TILE* pGround = new TILE;
		m_pNewGround = pGround;
		m_pNewGround->byDrawID = m_DrawID;
		m_pNewGround->byOption = 0;
		m_pNewGround->bChoosing = true;
		D3DXMatrixScaling(&m_pNewGround->matSize, 1.f, 1.f, 1.f);
		m_pBackGround->PushVecGround(m_pNewGround);



	}



	UpdateData(FALSE);

}


void CTileTool::OnBnClickedButtonTilesetsave()
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

	DWORD dwByte;

	int iSize = m_vecTilePath.size();

	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(size_t index =0; index < m_vecTilePath.size(); index ++)
	{
		wstring* pString = &(m_vecTilePath[index]->wstrObjKey);

		WriteFile(hFile, pString, sizeof(wstring), &dwByte, NULL);

	}

	CloseHandle(hFile);
}

void CTileTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_HighSlider)
		{ // 높이 스크롤

			int nPos = m_HighSlider.GetPos();
			int iSize = m_vecChoosingTile.size();

			for(int i =0; i < iSize ; i++)
			{
				m_vecChoosingTile[i]->vPos.y = m_vecChoosingTile[i]->vOriginPos.y - nPos;
				((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

			}

		}

		else if(pScrollBar == (CScrollBar*)&m_ScaleSlider)
		{ // z크기 스크롤

			if(m_MapComboBox.GetCurSel() == 0)
			{

				float nPos = ((float) m_ScaleSlider.GetPos()) / 10;
				int iSize = m_vecChoosingTile.size();

				for(int i =0; i < iSize ; i++)
				{
					D3DXMatrixScaling(&m_vecChoosingTile[i]->matSize, nPos, nPos, 1.f);
					((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
				}

			}

			else
			{

				float nPos = ((float) m_ScaleSlider.GetPos()) / 10;

				D3DXMatrixScaling(&m_pChoosingGround->matSize, nPos, nPos, 1.f);
				((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

			}

		}

		else if(pScrollBar == (CScrollBar*)&m_GaroSlider)
		{ // 가로 스크롤

			int nPos = m_GaroSlider.GetPos();
			int iSize = m_vecChoosingTile.size();

			for(int i =0; i < iSize ; i++)
			{
				m_vecChoosingTile[i]->vPos.x = m_vecChoosingTile[i]->vOriginPos.x - nPos;
				((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

			}

		}
	}

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTileTool::CancelChoosingTile( PTILE pTile )
{

	vector<PTILE>::iterator iter = m_vecChoosingTile.begin();

	for(iter; iter != m_vecChoosingTile.end() ; iter++)
	{
		if((*iter)->vPos == pTile->vPos)
		{
			m_vecChoosingTile.erase(iter);
			break;
		}
	}

}

void CTileTool::Release()
{
	m_vecTilePath.clear();

	m_vecChoosingTile.clear();

}
void CTileTool::OnBnClickedButtonTileErase()
{

	if(m_MapComboBox.GetCurSel() == 0)
	{
		vector<PTILE> ::iterator iter = m_vecChoosingTile.begin();

		for(iter; iter != m_vecChoosingTile.end(); iter++)
		{
			(*iter)->bChoosing = false;
			(*iter)->vPos.y = (*iter)->vOriginPos.y;
			(*iter)->vPos.x = (*iter)->vOriginPos.x;
			D3DXMatrixScaling(&(*iter)->matSize, 1.f, 1.f, 1.f);
			(*iter)->byOption = 0; 
			(*iter)->byDrawID = 0; 
			(*iter)->bCollision = false;
		}

		m_vecChoosingTile.clear();

	}

	else
	{
		int iIndex = 0;
		vector<PTILE>* pVec = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->
			GetBackGround()->GetGround();

		vector<PTILE>::iterator iter = pVec->begin();


		for(iter; iter!= pVec->end();iter++)
		{
			if((*iter) == m_pChoosingGround)
			{
				safe_delete(m_pChoosingGround);
				pVec->erase(iter);
				break;
			}

		}

	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTileTool::LoadTileBitmap( void )
{


	HANDLE hFile = CreateFile( L"..\\Data\\TileSet.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;


	int iSize;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);
	
		for(int i = 0; i<iSize; i++)
		{	
			wstring pwstringImagePath = L"../Texture/BackGround/Tile/" ;
			wstring* pwstrObjKey = new wstring;
			TILE_PATH* pTilePath = new TILE_PATH;

			ReadFile(hFile, pwstrObjKey, sizeof(wstring), &dwByte, NULL);

			pwstringImagePath.append(*pwstrObjKey);
			pwstringImagePath.append(L".png");

			pTilePath->wstrImagePath = pwstringImagePath;
			pTilePath->wstrObjKey = *pwstrObjKey;

			if(m_bTilePathFull == false)
				m_vecTilePath.push_back(pTilePath);

			m_TileListBox.AddString( pTilePath->wstrObjKey.c_str() );


			CTextureMgr::GetInstance()->InsertTexture(
				pTilePath->wstrImagePath.c_str(), 
				pTilePath->wstrObjKey.c_str(), TEX_SINGLE);

			CImage Image;

			Image.Load(pTilePath->wstrImagePath.c_str());

			CBitmap* pBitmap = new CBitmap;

			pBitmap->Attach( Image.Detach() );
			//pBitmap->Detach();	-> HBitmap을 반환한다.
			//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

			m_mapBitmap.insert( make_pair(pTilePath->wstrObjKey.c_str(), pBitmap) );

	}

	m_bTilePathFull = true;
	CloseHandle(hFile);


}

void CTileTool::OnCbnSelchangeComboMap()
{

	m_TileListBox.ResetContent();


	for(size_t i = 0; i<m_vecTilePath.size(); i++)
		safe_delete(m_vecTilePath[i]);

	map<CString, CBitmap*>::iterator iter = m_mapBitmap.begin();

	for(iter; iter != m_mapBitmap.end(); iter++)
		safe_delete((*iter).second);

	m_mapBitmap.clear();
	m_vecTilePath.clear();
	m_bTilePathFull = false;


	if(m_MapComboBox.GetCurSel() == 0)
		LoadTileBitmap();

	//if(m_MapComboBox.GetCurSel() == 1)
	//	LoadGroundBitmap();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTileTool::NewGround(D3DXVECTOR3 vPoint)
{
	if(m_pNewGround != NULL && m_pNewGround->byOption == 0)
		m_pNewGround->vPos = vPoint;

	if(m_pChoosingGround != NULL && m_pChoosingGround->byOption ==0)
		m_pChoosingGround->vPos = vPoint;



}

void CTileTool::FastenGround()
{

	// 0 : 이동중
	// 1 : 고정

	if(m_pNewGround != NULL )
	{
		if(m_pNewGround->byOption == 0)
			m_pNewGround->byOption = 1;

		m_pNewGround = NULL;
	}

	if(m_pChoosingGround != NULL)
	{
		if(m_pChoosingGround->byOption ==0)
		{
			m_pChoosingGround->byOption = 1;
		}
		else
			m_pChoosingGround->byOption = 0;
	}

	
}

void CTileTool::LoadGroundBitmap( void )
{


	HANDLE hFile = CreateFile( L"..\\Data\\GroundSet2.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;


	int iSize;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		wstring pwstringImagePath = L"../Texture/BackGround/Ground/" ;
		wstring* pwstrObjKey = new wstring;
		TILE_PATH* pTilePath = new TILE_PATH;

		ReadFile(hFile, pwstrObjKey, sizeof(wstring), &dwByte, NULL);

		pwstringImagePath.append(*pwstrObjKey);
		pwstringImagePath.append(L".png");

		pTilePath->wstrImagePath = pwstringImagePath;
		pTilePath->wstrObjKey = *pwstrObjKey;

		if(m_bTilePathFull == false)
			m_vecTilePath.push_back(pTilePath);

		m_TileListBox.AddString( pTilePath->wstrObjKey.c_str() );


		CTextureMgr::GetInstance()->InsertTexture(
			pTilePath->wstrImagePath.c_str(), 
			pTilePath->wstrObjKey.c_str(), TEX_SINGLE);

		CImage Image;

		Image.Load(pTilePath->wstrImagePath.c_str());

		CBitmap* pBitmap = new CBitmap;

		pBitmap->Attach( Image.Detach() );
		//pBitmap->Detach();	-> HBitmap을 반환한다.
		//받은 HBitmap을 CBitmap으로 전환하는 기능이다.

		m_mapBitmap.insert( make_pair(pTilePath->wstrObjKey.c_str(), pBitmap) );

	}

	m_bTilePathFull = true;
	CloseHandle(hFile);

}
void CTileTool::OnBnClickedCheckTileCollision()
{
	for(size_t i =0; i<m_vecChoosingTile.size();i++)
		m_vecChoosingTile[i]->bCollision = (bool) m_ButtonCollsion.GetCheck();



	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTileTool::OnBnClickedButtonTilesave()
{

	if(m_MapComboBox.GetCurSel() == 0)
	{
		CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();
		vector<PTILE>* pVecTile = pBackGround->GetTile();


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

		int iSize = pVecTile->size();

		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i =0; i<iSize; i++)
		{

			WriteFile(hFile, (*pVecTile)[i], sizeof(TILE), &dwByte, NULL);

		}

		CloseHandle(hFile);

	}


	if(m_MapComboBox.GetCurSel() == 1)
	{
		CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();
		vector<PTILE>* pVecGround = pBackGround->GetGround();


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

		int iSize = pVecGround->size();

		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i =0; i<iSize; i++)
		{

			WriteFile(hFile, (*pVecGround)[i], sizeof(TILE), &dwByte, NULL);

		}

		CloseHandle(hFile);

	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTileTool::OnBnClickedButtonTileLoad()
{
	if(m_MapComboBox.GetCurSel() == 0)
	{

		CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();

		HANDLE hFile = CreateFile( L"..\\Data\\TileData.dat",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
			NULL);

		DWORD dwByte;
		int iSize;
		PTILE pTile;

		vector<PTILE>* pVecTile = new vector<PTILE>;
		vector<PTILE>* OriginVecTile = pBackGround->GetTile();

		for(size_t i=0; i< OriginVecTile->size(); i++)
			safe_delete((*OriginVecTile)[i]);

		OriginVecTile->clear();


		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i<iSize; i++)
		{	
			pTile = new TILE; 
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
			pVecTile->push_back(pTile);
		}
		pBackGround->SetTile(*pVecTile);


		CloseHandle(hFile);
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	}

	if(m_MapComboBox.GetCurSel() == 1)
	{

		CBackGround* pBackGround = 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();

		HANDLE hFile = CreateFile( L"..\\Data\\GroundData.dat",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
			NULL);

		DWORD dwByte;
		int iSize;
		PTILE pTile;

		vector<PTILE>* pVecGround = new vector<PTILE>;
		vector<PTILE>* OriginGround = pBackGround->GetGround();

		for(size_t i=0; i< OriginGround->size(); i++)
			safe_delete((*OriginGround)[i]);

		OriginGround->clear();


		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

		for(int i = 0; i<iSize; i++)
		{	
			pTile = new TILE; 
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
			pVecGround->push_back(pTile);
		}
		pBackGround->SetGround(*pVecGround);


		CloseHandle(hFile);
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	}





	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTileTool::ChooseGround( int iIndex )
{
	if(m_pChoosingGround == NULL)
	{
		vector<PTILE>* pVec =  m_pBackGround->GetGround();
		m_pChoosingGround = (*pVec)[iIndex];
		m_pChoosingGround->byOption = 0;
	}

	else
		m_pChoosingGround = NULL;

}
