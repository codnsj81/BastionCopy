// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Tool73View.h"
#include "Tool73.h"
#include "ObjectTool.h"
#include "TextureMgr.h"
#include "Obstacle.h"
#include "BackGround.h"

// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CPropertyPage)

CObjectTool::CObjectTool()
	: CPropertyPage(CObjectTool::IDD), m_fXPos(0.f), m_fYPos(0.f)
									,m_iHp(0)
{
	m_ChoosingObject = NULL;
}

CObjectTool::~CObjectTool()
{
	Release();
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{

	m_vecObject = ((CMainFrame*)AfxGetMainWnd())->GetMainView()
		->GetObstacle()->GetObject();


	DDX_Text(pDX, IDC_EDIT_XPOS, m_fXPos);
	DDX_Text(pDX, IDC_EDIT_YPOS, m_fYPos);
	DDX_Text(pDX, IDC_EDIT_OBJ_HP, m_iHp);
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_OBJECT_IMAGELIST, m_ObjectList);
	DDX_Control(pDX, IDC_SLIDER_OBJECT_SCALE, m_OBJScaleSlider);
	m_OBJScaleSlider.SetRange(0,20);
	m_OBJScaleSlider.SetPos(10);
	m_OBJScaleSlider.SetTicFreq(2);

	DDX_Control(pDX, IDC_SLIDER_OBJ_HITBOX_X, m_HitBoxHorizon);
	DDX_Control(pDX, IDC_SLIDER_OBJ_HITBOX_Y, m_HitBoxVertical);
	m_HitBoxHorizon.SetRange(-20,20);
	m_HitBoxHorizon.SetPos(0);
	m_HitBoxHorizon.SetTicFreq(2);

	m_HitBoxVertical.SetRange(-20,20);
	m_HitBoxVertical.SetPos(0);
	m_HitBoxVertical.SetTicFreq(2);

	DDX_Control(pDX, IDC_SLIDER_OBJ_HITBOX_Y2, m_SliederY2);
	DDX_Control(pDX, IDC_SLIDER_OBJ_HITBOX_X2, m_SliderX2);
	m_SliederY2.SetRange(-20,20);
	m_SliederY2.SetPos(0);
	m_SliederY2.SetTicFreq(2);

	m_SliderX2.SetRange(-20,20);
	m_SliderX2.SetPos(0);
	m_SliderX2.SetTicFreq(2);

}


BEGIN_MESSAGE_MAP(CObjectTool, CPropertyPage)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectTool::OnLbnSelchangeList1)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_CHECK_OBJ_DESTROY, &CObjectTool::OnBnClickedCheckObjDestroy)
	ON_BN_CLICKED(IDC_CHECK_OBJ_COLLISION, &CObjectTool::OnBnClickedCheckObjCollision)
	ON_BN_CLICKED(IDC_BUTTON_OBJECTADMIT, &CObjectTool::OnBnClickedButtonObjectadmit)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_OBJERASE, &CObjectTool::OnBnClickedButtonObjerase)
	ON_BN_CLICKED(IDC_BUTTON_OBJHITBOX, &CObjectTool::OnBnClickedButtonObjhitbox)
	ON_BN_CLICKED(IDC_BUTTON_OBJSETSAVE, &CObjectTool::OnBnClickedButtonObjsetsave)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_LOAD, &CObjectTool::OnBnClickedButtonObjLoad)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.

void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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


		m_vecObjectPath.push_back(pTemp);

		m_ObjectList.AddString( pTemp->wstrObjKey.c_str() );

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

void CObjectTool::OnLbnSelchangeList1()
{

	UpdateData(TRUE);

	int iIndex = m_ObjectList.GetCurSel();

	map<CString, CBitmap*>::iterator iter;

	iter = m_mapBitmap.find( m_vecObjectPath[iIndex]->wstrObjKey.c_str() );

	if(iter == m_mapBitmap.end())
		return ;

	((CStatic*)GetDlgItem(IDC_STATIC_OBJECT))->SetBitmap( *(iter->second) );

	CString szID = (iter->first);
	szID.Delete(0,6);
	m_iDrawID = _ttoi(szID);
	//szID.delete(0,4);

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPropertyPage::OnActivate(nState, pWndOther, bMinimized);
	g_choosing = ING_END;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CObjectTool::OnBnClickedCheckObjDestroy()
{

	m_bDestory = IsDlgButtonChecked(IDC_CHECK_OBJ_DESTROY);  

	if(m_bDestory)
		GetDlgItem(IDC_EDIT_OBJ_HP)->EnableWindow(true);
	else
	{
		m_iHp = 0;
		GetDlgItem(IDC_EDIT_OBJ_HP)->EnableWindow(false);
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnBnClickedCheckObjCollision()
{
	m_bCollision = IsDlgButtonChecked(IDC_CHECK_OBJ_COLLISION);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnBnClickedButtonObjectadmit()
{

	g_choosing = OBJ_DRAWING;

	m_Object = new OBJECT_DATA;
	m_Object->byOption = 0;
	m_Object->bCollision = m_bCollision;
	m_Object->bDestruction = m_bDestory;
	m_Object->iHp = m_iHp;
	m_Object->bTemp = false;
	m_Object->byDrawID = m_iDrawID;

	for(int i =0; i<3; i++)
	{
		m_Object->rHitBox[i].RectfCX = 35;
		m_Object->rHitBox[i].RectfCY = 15;
		m_Object->rHitBox[i].RectfCX2 = 15;
		m_Object->rHitBox[i].RectfCY2 = 25;

		m_Object->rHitBox[i].vHitBoxPos = D3DXVECTOR3(0.f,0.f,0.f);

	}
	

	D3DXMatrixScaling(&m_Object->matSize, 1.f, 1.f, 1.f);
	m_vecObject->push_back(m_Object);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::NewObjectMove( D3DXVECTOR3 vPos )
{
	UpdateData(TRUE);

	if(m_Object != NULL )
	{		
		if(m_Object->byOption == 0)
			m_Object->vPos = vPos;

		m_fXPos = m_Object->vPos.x;
		m_fYPos = m_Object->vPos.y;


		RECT pRect = {(LONG) m_Object->vPos.x - 50,(LONG)  m_Object->vPos.y - 50,
			(LONG)  m_Object->vPos.x + 50 , (LONG) m_Object->vPos.y + 50};

		m_Object->HitBox = pRect;

		for(int i=0; i< 3; i++)
		{
			RECT pRect1 = {(LONG) (m_Object->rHitBox[i].vHitBoxPos.x - m_Object->rHitBox[i].RectfCX ),(LONG) ( m_Object->rHitBox[i].vHitBoxPos.y -  m_Object->rHitBox[i].RectfCY),
				(LONG)  (m_Object->rHitBox[i].vHitBoxPos.x + m_Object->rHitBox[i].RectfCY) , (LONG) (m_Object->rHitBox[i].vHitBoxPos.y + m_Object->rHitBox[i].RectfCY)};

			m_Object->rHitBox[i].RectHitBox[0] = pRect1;

			RECT pRect2 = {(LONG) (m_Object->rHitBox[i].vHitBoxPos.x - m_Object->rHitBox[i].RectfCX2) ,(LONG) (m_Object->rHitBox[i].vHitBoxPos.y - m_Object->rHitBox[i].RectfCY2),
				(LONG)(  m_Object->rHitBox[i].vHitBoxPos.x + m_Object->rHitBox[i].RectfCY2) , (LONG) (m_Object->rHitBox[i].vHitBoxPos.y + m_Object->rHitBox[i].RectfCY2)};

			m_Object->rHitBox[i].RectHitBox[1] = pRect2;
		}



		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	}

	UpdateData(FALSE);

}

void CObjectTool::Release( void )
{
	for(size_t i =0; i< m_vecObjectPath.size(); i++)
		safe_delete(m_vecObjectPath[i]);

	m_vecObjectPath.clear();

}

void CObjectTool::FastenObj( void )
{

	if(m_Object != NULL )
	{
		if(m_Object->byOption == 0)
			m_Object->byOption = 1;
		else 
			m_Object->byOption = 0;

		m_Object = NULL;

		((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	}

}


void CObjectTool::OnBnClickedButtonAddHitbox()
{
	

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::ObjectSelect( D3DXVECTOR3 vPos )
{
	int iIndex ;
	m_pObstacle = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetObstacle();
	iIndex = m_pObstacle->GetObstacleIndex(vPos);
	
	if(iIndex == -1)
		return;
	
	if(m_ChoosingObject != NULL)
	{
		m_ChoosingObject = NULL;

	}
	m_ChoosingObject = (*m_vecObject)[iIndex];

	if(m_ChoosingObject->bTemp == true)
		m_ChoosingObject->bTemp = false;
	else
		m_ChoosingObject->bTemp = true;


}

void CObjectTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_ChoosingObject == NULL)
		return;

	vector<PTILE> pVecTile = *(((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile());
	

	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_OBJScaleSlider)
		{
			float nPos  = (float) (m_OBJScaleSlider.GetPos() / 10.f);
			D3DXMatrixScaling(&m_ChoosingObject->matSize, nPos,nPos, 1.f);
			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		}

		if(pScrollBar == (CScrollBar*)&m_HitBoxHorizon)
		{

			int nPos  =m_HitBoxHorizon.GetPos() ;

			for(int i=0; i<3 ; i++)
				m_ChoosingObject->rHitBox[i].RectfCX = static_cast<float>(35 + nPos);

			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		}

		if(pScrollBar == (CScrollBar*)&m_HitBoxVertical)
		{
			int nPos  =m_HitBoxVertical.GetPos() ;

			for(int i=0; i<3 ; i++)
				m_ChoosingObject->rHitBox[i].RectfCY =static_cast<float>(15 + nPos);

			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		}

		if(pScrollBar == (CScrollBar*)&m_SliderX2)
		{

			int nPos  =m_SliderX2.GetPos() ;

			for(int i=0; i<3 ; i++)
				m_ChoosingObject->rHitBox[i].RectfCX2 = static_cast<float>(15 + nPos) ;

			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		}

		if(pScrollBar == (CScrollBar*)&m_SliederY2)
		{
			int nPos  =m_SliederY2.GetPos() ;

			for(int i=0; i<3 ; i++)
				m_ChoosingObject->rHitBox[i].RectfCY2 =static_cast<float>(25 + nPos);

			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
		}

	}

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CObjectTool::OnBnClickedButtonObjerase()
{
	int iIndex = 0;
	vector<pOBJECT_DATA>::iterator iter = m_vecObject->begin();

	for(iter; iter!= m_vecObject->end();iter++)
	{
		if((*iter) == m_ChoosingObject)
		{
			safe_delete(m_ChoosingObject);
			m_vecObject->erase(iter);
			break;
		}

	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnBnClickedButtonObjhitbox()
{
	if(m_ChoosingObject == NULL)
		return;

	g_choosing = OBJ_HITBOXDRAWING;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::GetTileIndex( D3DXVECTOR3 _vPos )
{
	for(int i= 0; i<3; i++)
	{

		if(m_ChoosingObject->rHitBox[i].vHitBoxPos == D3DXVECTOR3(0.f, 0.f, 0.f))
		{
			m_ChoosingObject->rHitBox[i].vHitBoxPos = _vPos;

			RECT pRect1 = {(LONG) (m_ChoosingObject->rHitBox[i].vHitBoxPos.x - m_ChoosingObject->rHitBox[i].RectfCX ),(LONG) ( m_ChoosingObject->rHitBox[i].vHitBoxPos.y -  m_ChoosingObject->rHitBox[i].RectfCY),
				(LONG)  (m_ChoosingObject->rHitBox[i].vHitBoxPos.x + m_ChoosingObject->rHitBox[i].RectfCX) , (LONG) (m_ChoosingObject->rHitBox[i].vHitBoxPos.y + m_ChoosingObject->rHitBox[i].RectfCY)};

			m_ChoosingObject->rHitBox[i].RectHitBox[0] = pRect1;

			RECT pRect2 = {(LONG) (m_ChoosingObject->rHitBox[i].vHitBoxPos.x - m_ChoosingObject->rHitBox[i].RectfCX2) ,(LONG) (m_ChoosingObject->rHitBox[i].vHitBoxPos.y - m_ChoosingObject->rHitBox[i].RectfCY2),
				(LONG)(  m_ChoosingObject->rHitBox[i].vHitBoxPos.x + m_ChoosingObject->rHitBox[i].RectfCX2) , (LONG) (m_ChoosingObject->rHitBox[i].vHitBoxPos.y + m_ChoosingObject->rHitBox[i].RectfCY2)};

			m_ChoosingObject->rHitBox[i].RectHitBox[1] = pRect2;
			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);


			break;
		}
	}


	g_choosing = OBJ_DRAWING;
}

void CObjectTool::OnBnClickedButtonObjsetsave()
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

	int iSize = m_vecObject->size();

	WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i =0; i<iSize; i++)
	{

		WriteFile(hFile,(*m_vecObject)[i], sizeof(OBJECT_DATA), &dwByte, NULL);

	}

	CloseHandle(hFile);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnBnClickedButtonObjLoad()
{

	HANDLE hFile = CreateFile( L"..\\Data\\ObjectData.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// 기본값으로 파일을 생성 시켜라.
		NULL);

	DWORD dwByte;
	int iSize;

	vector<pOBJECT_DATA>* pVecObject = new vector<pOBJECT_DATA>;
	pOBJECT_DATA pData;

	for(size_t i=0; i< m_vecObject->size(); i++)
		safe_delete((*m_vecObject)[i]);

	m_vecObject->clear();


	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i<iSize; i++)
	{	
		pData  = new OBJECT_DATA; 
		ReadFile(hFile, pData, sizeof(OBJECT_DATA), &dwByte, NULL);
		m_vecObject->push_back(pData);
	}


	CloseHandle(hFile);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
