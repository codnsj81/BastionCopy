// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool73.h"
#include "PathFind.h"


// CPathFind 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(CPathFind::IDD, pParent)
{

}

CPathFind::~CPathFind()
{
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATH_FIND, m_PathList);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PATH_SAVE, &CPathFind::OnBnClickedButtonPathSave)
	ON_BN_CLICKED(IDC_BUTTON_PATH_LOAD, &CPathFind::OnBnClickedButtonPathLoad)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_PATHDELETE, &CPathFind::OnBnClickedButtonPathdelete)
END_MESSAGE_MAP()


// CPathFind 메시지 처리기입니다.

void CPathFind::OnBnClickedButtonPathSave()
{

	wofstream SaveFile;

	SaveFile.open(L"../Data/ImagePath.txt",ios::out);


	int iCount = 0;

	for(list<IMAGE_PATH*>::iterator iter = m_listPathInfo.begin();
		 iter != m_listPathInfo.end(); ++iter)
	{

		++iCount;

		SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
		SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
		SaveFile << (*iter)->iCount << L"|";


		if(iCount == m_listPathInfo.size())
			SaveFile<<(*iter)->wstrPath.c_str() << flush;

		else
			SaveFile<<(*iter)->wstrPath.c_str()<<endl;
	}

	SaveFile.close();

	WinExec( "notepad.exe ../Data/ImagePath.txt", SW_SHOW );	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CPathFind::OnBnClickedButtonPathLoad()
{
	
// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wifstream LoadFile;

	LoadFile.open(L"../Data/ImagePath.txt", ios::in);

	TCHAR szObjKey[MIN_STR]		= L"";
	TCHAR szStateKey[MIN_STR]	= L"";
	TCHAR szCount[MIN_STR]		= L"";
	TCHAR szImagePath[MAX_STR]	= L"";

	wstring wstrCombine = L"";

	m_PathList.ResetContent();

	while( !LoadFile.eof() )
	{
		//라인별로 로드를 해보도록 하자.
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImagePath, MAX_STR);

		wstrCombine = wstring(szObjKey) + L"|";
		wstrCombine += wstring(szStateKey) + L"|";
		wstrCombine += wstring(szCount)  + L"|";
		wstrCombine += szImagePath;

		m_PathList.AddString(wstrCombine.c_str());
	}

	LoadFile.close();

	UpdateData(FALSE);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	wstring	wstrCombine = L"";
	TCHAR szBuff[MAX_STR] = L"";		//경로를 저장하는 변수.

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	for(int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szBuff, MAX_STR);

		DirectoryInfoExtration(szBuff, m_listPathInfo);
	}

	ZeroMemory(szBuff, sizeof(szBuff));

	m_PathList.ResetContent();

	for(list<IMAGE_PATH*>::iterator iter = m_listPathInfo.begin();
		iter != m_listPathInfo.end(); ++iter)
	{
		//숫자를 문자로 변형을 하자.
		_itow_s((*iter)->iCount, szBuff, 10);

		wstrCombine = (*iter)->wstrObjKey + L"|" + (*iter)->wstrStateKey + L"|";

		wstrCombine += szBuff;

		wstrCombine += L"|" + (*iter)->wstrPath;

		m_PathList.AddString(wstrCombine.c_str());
	}

	UpdateData(FALSE);
}
void CPathFind::OnBnClickedButtonPathdelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
