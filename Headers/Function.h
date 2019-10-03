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


//��θ� �����ϴ� �Լ�.(�����)

inline wstring RelativePath(wstring strFullPath)
{
	TCHAR szRelativePath[MAX_STR] = L"";
	TCHAR szDirectoryPath[MAX_STR] = L"";


	//������Ʈ������ �ִ� ��ġ������ ��θ� ������ �ִ� �Լ�.
	GetCurrentDirectory( sizeof(szDirectoryPath), szDirectoryPath );


	PathRelativePathTo( (LPWSTR)szRelativePath,
		szDirectoryPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.c_str(),
		FILE_ATTRIBUTE_NORMAL);
	//�� ��ο��� �ٸ���� ���� ����θ� �����غ���.
	//(�ݵ�� ���� ����̺꿡 �־���Ѵ�).

	return szRelativePath;
}




//���丮�ȿ� �ִ� ������ ������ �ľ��ϴ� �Լ��� ����
static int DirectoryFileCount(const std::wstring& wstrPath)
{
	//png���ϸ� ã�ƺ���.
	wstring wstrTemp = wstrPath + L"\\*.png";

	CFileFind Find;

	Find.FindFile(wstrTemp.c_str());


	int iContinue = 1;

	int iFileCount = 0;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();		//������ ������ 0�����Ѵ�.

		if(!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}




//������ ������ �����ϴ� �Լ��� ����� ����.
static void DirectoryInfoExtration(const std::wstring& wstrPath,
								   std::list<IMAGE_PATH*>& rListPathInfo)
{
	//MFC�����Լ��� ���⼭ ����Ұ��̱� ������.
	CFileFind Find;

	Find.FindFile(wstrPath.c_str());

	int iContinue = 1;

	while(iContinue)
	{
		//���������� �˻����ִ� �Լ�.
		iContinue = Find.FindNextFile();

		if(Find.IsDots())		//.�� ..�� �ɷ����� ���� �Լ�.
			continue;

		//ã�������� ��������
		//���������� �������ִ� �Լ�.

		if(Find.IsDirectory())		//ã�� ������ �����̴�.
		{
			DirectoryInfoExtration(wstring(Find.GetFilePath() + L"\\*.*"), rListPathInfo);	
		}
		else		//�����̸� ��������...
		{
			//ã�� ������ �ý��� ������ ���.
			if(Find.IsSystem())
				continue;

			IMAGE_PATH* pPath = new IMAGE_PATH;

			TCHAR szPathBuff[MAX_STR] = L"";

			lstrcpy(szPathBuff, Find.GetFilePath().operator LPCWSTR() );

			//����� ���� ��θ� �߶�
			PathRemoveFileSpec(szPathBuff);

			pPath->iCount = DirectoryFileCount(szPathBuff);


			//\Texture\Stage\Player\Stand\AKIHA_AKI00_000.png

			//������ �̸��� ����.
			wstring wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			//AKIHA_AKI00_000


			//AKIHA_AKI00_000	-> %d
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";	//���ڿ��� �߶��ִ� �Լ�.
			//AKIHA_AKI00_00%d.png


			//��θ� ����.
			TCHAR szBuff[MAX_STR] = L"";

			lstrcpy(szBuff, Find.GetFilePath().operator LPCWSTR() );
			//\Texture\Stage\Player\Stand\AKIHA_AKI00_000.png


			//������ ��θ� �߶󳽴�.
			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player\Stand\


			//�� ���ڿ��� �����ִ� �Լ�.
			PathCombine(szBuff, szBuff, wstrTextureName.c_str());
			//\Texture\Stage\Player\Stand\AKIHA_AKI00_00%d.png


			//�����
			pPath->wstrPath = RelativePath(szBuff);


			//���� ��θ� ��������.
			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player\Stand


			//������Ʈ Ű ����.
			pPath->wstrStateKey = PathFindFileName(szBuff);


			PathRemoveFileSpec(szBuff);
			//\Texture\Stage\Player

			//������Ʈ Ű ����.
			pPath->wstrObjKey = PathFindFileName(szBuff);

			rListPathInfo.push_back(pPath);

			//�������Ͽ��� ���� ���� �˻�(�� �ܰ踦 �˻��ϰ� �ȴ�)
			Find.FindFile(Find.GetFilePath());

			Find.FindNextFile();
		}
	}


}

#endif