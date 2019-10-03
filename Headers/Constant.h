#ifndef __CONSTANT_H_
#define __CONSTANT_H_


#include "Enum.h"
extern HWND g_hWnd;
extern eChoosing g_choosing;

const int WINCX = 1000;
const int WINCY = 600;

const WORD MIN_STR	  = 64;
const WORD MIDDLE_STR = 128;
const WORD MAX_STR	  = 256;


const int TILEX = 70;
const int TILEY = 130;

const int TILECX = 120;
const int TILECY = 66;


const DWORD ITEM_WEAPON = 0x00000001;
const DWORD ITEM_ARMOR  = 0x00000002;
const DWORD ITEM_POTION = 0x00000004;

const DWORD KEY_NULL = 0x00000030;
const DWORD KEY_LEFT = 0x00000001;
const DWORD KEY_RIGHT = 0x00000002;
const DWORD KEY_UP = 0x00000004;
const DWORD KEY_DOWN = 0x00000008;
const DWORD KEY_SPACE = 0X00000010;
const DWORD KEY_LBUTTON = 0X000020;
const DWORD KEY_RBUTTON = 0x000030;
const DWORD KEY_CONTROL = 0x000100;


#endif