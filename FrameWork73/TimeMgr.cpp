#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr);

void CTimeMgr::InitTime(void)
{
	//QueryPerformanceCounter() : 고해상의 타미어의 현재 값을 얻는다.
	//							  1 / 10000
	//							 cpu : 시간을 보내준다.

	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);

	//QueryPerformanceFrequency() : cpu가 1초동안 진동수를 반환한다.
}

void CTimeMgr::SetTime(void)
{
	//호출이 될때마다 새로운 시간을 구하기 위해서.
	QueryPerformanceCounter(&m_FrameTime);

	//새로구한 시간과 초기(Init)화 되었을때의 시간의 차이를 구한다.
	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart )
	{
		//프로세스가 부하가 걸렸을때 시간이 뒤로 밀리는것을 방지하기 위해서.

		//cpu tick을 새롭게 갱신.
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//메인게임에서 Update()호출되고 다음 Update()까지의 
	//인터벌값을 진동수로 나누면 1초동안 갈수 있는 프레임의 크기가 나온다.(시간값)

	
	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;
	
	m_FixTime = m_FrameTime;
}

float CTimeMgr::GetTime(void)
{
	return m_fTime;
}

void CTimeMgr::Release(void)
{
	//safe_delete(this);
}
