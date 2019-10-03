#ifndef __FUNCTION_H_
#define __FUNCTION_H_

template<typename T>
static inline void safe_delete(T& rData)
{
	if(rData)
	{
		delete rData;
		rData = NULL;
	}
}


//경로를 추출하는 함수.(상대경로)

inline wstring RelativePath(wstring strFullPath)
{
	TCHAR szRelativePath[MAX_STR] = L"";
	TCHAR szDirectoryPath[MAX_STR] = L"";


	//프로젝트파일이 있는 위치까지의 경로를 얻을수 있는 함수.
	GetCurrentDirectory( sizeof(szDirectoryPath), szDirectoryPath );


	PathRelativePathTo( (LPWSTR)szRelativePath,
		szDirectoryPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.c_str(),
		FILE_ATTRIBUTE_NORMAL);
	//한 경로에서 다른경로 가는 상대경로를 추출해보자.
	//(반드시 같은 드라이브에 있어야한다).

	return szRelativePath;
}




//디렉토리안에 있는 파일의 개수를 파악하는 함수를 제작
static int DirectoryFileCount(const std::wstring& wstrPath)
{
	//png파일만 찾아보자.
	wstring wstrTemp = wstrPath + L"\\*.png";

	CFileFind Find;

	Find.FindFile(wstrTemp.c_str());


	int iContinue = 1;

	int iFileCount = 0;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();		//파일의 끝나면 0리턴한다.

		if(!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}




//폴더의 정보를 추출하는 함수를 만들어 보자.
static void DirectoryInfoExtration(const std::wstring& wstrPath,
								   std::list<IMAGE_PATH*>& rListPathInfo)
{
	//MFC전용함수를 여기서 사용할것이기 때문에.
	CFileFind Find;

	Find.FindFile(wstrPath.c_str());

	int iContinue = 1;

	while(iContinue)
	{
		//파일유무를 검사해주는 함수.
		iContinue = Find.FindNextFile();

		if(Find.IsDots())		//.과 ..을 걸러내기 위한 함수.
			continue;

		//찾은파일이 폴더인지
		//파일인지를 구분해주는 함수.

		if(Find.IsDirectory())		//찾은 파일이 폴더이다.
		{
			DirectoryInfoExtration(wstring(Find.GetFilePath() + L"\\*.*"), rListPathInfo);	
		}
		else		//파일이면 이쪽으로...
		{
			//찾은 파일이 시스템 파일인 경우.
			if(Find.IsSystem())
				continue;

			IMAGE_PATH* pPath = new IMAGE_PATH;

			TCHAR szPathBuff[MAX_STR] = L"";

			lstrcpy(szPathBuff, Find.GetFilePath().operator LPCWSTR() );

			//저장된 현재 경로를 잘라냄
			PathRemoveFileSpec(szPathBuff);

			pPath->iCount = DirectoryFileCount(szPathBuff);


			//\Texture\Stage\Player\Stand\AKIHA_AKI00_000.png

			//파일의 이름만 얻어보자.
			wstring wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			//AKIHA_AKI00_000


			//AKIHA_AKI00_000	-> %d
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";	//문자열을 잘라주는 함수.
			//AKIHA_AKI00_00%d.png


			//경로를 얻어보자.
			TCHAR szBuff[MAX_STR] = L"";

			lstrcpy(szBuff, Find.GetFilePath().operator LPCWSTR() );
			//\Texture\Stage\Player\Stand\AKIHA_AKI00_000.png


			//파일의 경로를 잘라낸다.
			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player\Stand\


			//두 문자열을 합쳐주는 함수.
			PathCombine(szBuff, szBuff, wstrTextureName.c_str());
			//\Texture\Stage\Player\Stand\AKIHA_AKI00_00%d.png


			//상대경로
			pPath->wstrPath = RelativePath(szBuff);


			//현재 경로를 추출하자.
			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player\Stand


			//스테이트 키 설정.
			pPath->wstrStateKey = PathFindFileName(szBuff);


			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player

			//오브젝트 키 설정.
			pPath->wstrObjKey = PathFindFileName(szBuff);

			rListPathInfo.push_back(pPath);

			//현재파일에서 다음 파일 검색(윗 단계를 검색하게 된다)
			Find.FindFile(Find.GetFilePath());

			Find.FindNextFile();
		}
	}


}

#endif