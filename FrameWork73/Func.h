#pragma once

class CStringCMP
{
private:
	const TCHAR* m_pString;

public:
	explicit CStringCMP(const TCHAR* pKey)
		:m_pString(pKey)
	{}
	~CStringCMP(){}

public:
	template<class T>
	bool operator () (T data)
	{
		return !lstrcmp(data.first, m_pString);
	}
};