// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#include <afx.h>
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9math.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>




#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"

#pragma comment (lib, "fmodex_vc.lib")

#include <atlimage.h>		//CImage클래스를 사용하기 위해서 추가하는 헤더.

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")		//경로처리 함수를 사용하기 위한 헤더 + 라이브러리

