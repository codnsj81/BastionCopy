#ifndef __STRUCT_H_
#define __STRUCT_H_

typedef struct tagStrCompare
{
	template<class T>
	bool operator () (T Data)
	{
		return !lstrcmp(Data.first, m_pName);
	}

	const TCHAR* m_pName;
	tagStrCompare(const TCHAR* pKey)
		:m_pName(pKey){}
}STRING_COMPARE;



typedef struct tagFrame
{
	float fStart;
	float fFrame;
	float fCount;
	float fMax;

	//tagFrame(){}
	tagFrame(float _fStart = 0.f, float _fFrame = 0.f, float _fCount = 0.f, float _fMax = 0.f)
		:fStart(_fStart), fFrame(_fFrame), fCount(_fCount), fMax(_fMax){}

}FRAME;


typedef struct tagNarr
{
	TCHAR* strScript;
	TCHAR* strSound;
	int iName;
	int iTime;
	bool bOn;

}NARR , PNARR;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;

	float fCX;
	float fCY;

	D3DXMATRIX matWorld;
}INFO, *PINFO, *pINFO;

typedef struct tagParticle
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;

	float fSpeed;
	DWORD dwTime;
	DWORD dwAlpaTime;
	
	int iSecond;
	float iAlpa;
	float fAngle;
	
	bool bAlpa;
	bool bDir;

}PARTICLE;

typedef struct tagButton
{
	RECT tRect;
	D3DXVECTOR3 vPos;
	bool bMouse;
	float fCX;
	float fCY;
}BUTTON;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		ImageInfo;
}TEX_INFO;

typedef struct tagObjHitBox
{
	D3DXVECTOR3 vHitBoxPos;
	RECT RectHitBox[2];

	float RectfCX;
	float RectfCY;

	float RectfCX2;
	float RectfCY2;

}HitBoxData;

typedef struct tagObject
{
	bool bCollision;
	bool bDestruction;
	bool bTemp;
	int	iHp;

	HitBoxData rHitBox[3];

	RECT HitBox;

	D3DXMATRIX	matSize;
	
	BYTE byDrawID;
	BYTE byOption;

	D3DXVECTOR3 vPos;

	tagObject(TCHAR* _Potal, bool _Collision, BYTE _iDrawID
		,INFO _Info, RECT _rect)
		: bCollision(_Collision), byDrawID(_iDrawID)

	{}
	tagObject()
	{
	}

}OBJECT_DATA, *pOBJECT_DATA;



typedef struct tagTile 
{
	D3DXVECTOR3 vShowingPos;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vOriginPos;
	D3DXMATRIX	matSize;
	D3DXMATRIX	matOriginSize;
	

	bool bCollision; // 보스 관련
	bool bChoosing;
	bool bFloating;

	DWORD dwTime;

	BYTE byDrawID;
	BYTE byOption;

}TILE, *PTILE, *pTILE;


typedef struct tagUnitData
{
	//std::wstring wstrName;
	wstring wstrName;
	int		iAttack;
	int		iHp;

	INFO	tInfo;
	bool	bChoosing;
	bool	bFloating;

}UNIT_DATA, *pUNIT_DATA;



typedef struct tagTilePath
{
	wstring wstrObjKey;
	wstring wstrImagePath;

}TILE_PATH;


typedef struct tagImagePath
{
	//std::wstring wstrName;
	wstring wstrObjKey;
	wstring wstrStateKey;
	wstring wstrPath;

	int iCount;		//

}IMAGE_PATH;


#endif