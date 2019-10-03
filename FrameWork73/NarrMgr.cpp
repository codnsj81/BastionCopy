#include "StdAfx.h"
#include "NarrMgr.h"
#include "SoundMgr.h"
#include "Device.h"
#include "SoundMgr.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CNarrMgr);


void CNarrMgr::Release(void)
{

}


HRESULT CNarrMgr::Initialize( void )
{
	m_bOn = false;
	InitData();


	return S_OK;
}

int CNarrMgr::Update( void )
{
	if(m_bOn)
	{
		DWORD dwTime = GetTickCount();
		if(dwTime > m_dwTime + m_pNarr.iTime * 1000)
		{
			
			m_bOn = false;
			
			map<int, NARR>::iterator iter = m_mapNarr.begin();

			iter = m_mapNarr.find(m_pNarr.iName);

			(*iter).second.iName = 0;

		}

	}

	return 0;
}

void CNarrMgr::Render( void )
{
	if(m_bOn)
	{
		//±×¸²ÀÚ
		D3DXMATRIX matTrans;
		const TEX_INFO* pPlayerTex = 
			CTextureMgr::GetInstance()->GetTexture( L"SubTitle"
			,L"SubTitle");

		if(pPlayerTex == NULL)
			return ;

		float fX = float(pPlayerTex->ImageInfo.Width / 2) ;
		float fY = float(pPlayerTex->ImageInfo.Height / 2);

		D3DXMatrixTranslation(&matTrans
			,WINCX/2
			,500.f 
			,0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(
			pPlayerTex->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans
			,50
			,520.f 
			,0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);


		CDevice::GetInstance()->GetFont()->DrawTextW(
			CDevice::GetInstance()->GetSprite(),
			m_pNarr.strScript,
			lstrlen(m_pNarr.strScript),
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void CNarrMgr::InitData()
{
	NARR pNARR;

	pNARR.iName = 1;
	pNARR.strScript = L"Proper story's suppoesd to start at the beginning, Ain't so simple with this one.";
	pNARR.strSound = L"Narrator01.MP3";
	pNARR.iTime = 5;
	pNARR.bOn = false;

	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 2;
	pNARR.strScript = L"Here's a kid whose world got all twisted, leaving him stranded on a rock in the sky.";
	pNARR.strSound = L"Narrator02.MP3";
	pNARR.iTime = 6;

	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 3;
	pNARR.strScript = L"He gets up.";
	pNARR.strSound = L"Narrator03.MP3";
	pNARR.iTime = 1;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));

	pNARR.iName = 4;
	pNARR.strScript = L"Sets off for the Bastion. Where everyone agred to go in case of trouble.";
	pNARR.strSound = L"Narrator04.MP3";
	pNARR.iTime = 4;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));



	pNARR.iName = 5;
	pNARR.strScript = L"The ground forms up under his feet, as if pointing the way. He don't stop to wonder why.";
	pNARR.strSound = L"Narrator05.MP3";
	pNARR.iTime = 5;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 6;
	pNARR.strScript = L"Find his lifelong friend just lying in the road.";
	pNARR.strSound = L"Narrator06.MP3";
	pNARR.iTime = 4;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 7;
	pNARR.strScript = L"Well it's a toucing reunion.";
	pNARR.strSound = L"Narrator07.MP3";
	pNARR.iTime =2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 9;
	pNARR.strScript = L"He sees what's left of the Ripping Walls.";
	pNARR.strSound = L"Narrator09.MP3";
	pNARR.iTime = 2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));



	pNARR.iName = 10;
	pNARR.strScript = L"Years of work undone in an instant. In the Calamity.";
	pNARR.strSound = L"Narrator10.MP3";
	pNARR.iTime = 4;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 11;
	pNARR.strScript = L"That a suvivor?";
	pNARR.strSound = L"Narrator11.MP3";
	pNARR.iTime = 1;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 12;
	pNARR.strScript = L"No ma'am it's a Gasfella, forced out from underground.";
	pNARR.strSound = L"Narrator12.MP3";
	pNARR.iTime = 5;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 13;
	pNARR.strScript = L" Kid pops him good.";
	pNARR.strSound = L"Narrator13.MP3";
	pNARR.iTime = 2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));

	pNARR.iName = 14;
	pNARR.strScript = L"Fella got a piece of him though.";
	pNARR.strSound = L"Narrator14.MP3";
	pNARR.iTime = 2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 16;
	pNARR.strScript = L"An old repeater falls out of the sky. Ain't a gift from Gods, but it'll have to do.";
	pNARR.strSound = L"Narrator16.MP3";
	pNARR.iTime = 6;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));

	pNARR.iName = 17;
	pNARR.strScript = L"Kid's worked up quite a thirst by now, so that fountain looks real inviting.";
	pNARR.strSound = L"Narrator17.MP3";
	pNARR.iTime = 5;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));



	pNARR.iName = 18;
	pNARR.strScript = L"Sometimes you just need a drink.";
	pNARR.strSound = L"Narrator18.MP3";
	pNARR.iTime = 2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 19;
	pNARR.strScript = L"A chool of Squirts tunnels up around him.";
	pNARR.strSound = L"Narrator19.MP3";
	pNARR.iTime = 3;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));


	pNARR.iName = 20;
	pNARR.strScript = L"He sets foor inside one of Caelondia's famous watering holes.";
	pNARR.strSound = L"Narrator20.MP3";
	pNARR.iTime = 4;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));

	pNARR.iName = 21;
	pNARR.strScript = L"The Security takes him for a pretty thief.";
	pNARR.strSound = L"Narrator21.MP3";
	pNARR.iTime = 3;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));

	pNARR.iName = 22;
	pNARR.strScript = L"Special deleveries : Gasfellas";
	pNARR.strSound = L"Narrator22.MP3";
	pNARR.iTime = 2;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));



	pNARR.iName = 23;
	pNARR.strScript = L"And then, he falls to is death... I'm just fooling! ";
	pNARR.strSound = L"Narrator23.MP3";
	pNARR.iTime = 4;
	m_mapNarr.insert(make_pair(pNARR.iName, pNARR));







}

void CNarrMgr::TurnOnNarr( int i )
{
	map<int, NARR>::iterator iter = m_mapNarr.begin();

	iter = m_mapNarr.find(i);
	if(iter == m_mapNarr.end())
	{
		return;
	}

	if((*iter).second.bOn == true)
		return;

	m_dwTime = GetTickCount();
	m_pNarr = (*iter).second;
	m_bOn = true;
	
	CSoundMgr::GetInstacne()->PlayNarrationSound(m_pNarr.strSound);
	if(m_pNarr.iName == 21)
	{
		CSoundMgr::GetInstacne()->PlayEffectSound(L"Security.MP3");
	}
	(*iter).second.bOn = true;
}
