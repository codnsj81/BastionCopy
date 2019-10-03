#ifndef __DEFINE_H_
#define __DEFINE_H_

#define SAFE_DELETE(p){ if(p){ delete p; p = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] p;/* p = NULL;*/ } }

#define ptTileRender POINT{WINCX / TILECX + 2, (LONG) WINCY / (TILECY/2) + 2}

#define GET_TIME CTimeMgr::GetInstance()->GetTime()

class CGameObject;
typedef list<CGameObject*>::iterator OBJITER;

class CUI;
typedef list<CUI*> ::iterator UIITER;

#endif