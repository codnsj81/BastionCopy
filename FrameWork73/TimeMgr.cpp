#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr);

void CTimeMgr::InitTime(void)
{
	//QueryPerformanceCounter() : ���ػ��� Ÿ�̾��� ���� ���� ��´�.
	//							  1 / 10000
	//							 cpu : �ð��� �����ش�.

	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);

	//QueryPerformanceFrequency() : cpu�� 1�ʵ��� �������� ��ȯ�Ѵ�.
}

void CTimeMgr::SetTime(void)
{
	//ȣ���� �ɶ����� ���ο� �ð��� ���ϱ� ���ؼ�.
	QueryPerformanceCounter(&m_FrameTime);

	//���α��� �ð��� �ʱ�(Init)ȭ �Ǿ������� �ð��� ���̸� ���Ѵ�.
	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart )
	{
		//���μ����� ���ϰ� �ɷ����� �ð��� �ڷ� �и��°��� �����ϱ� ���ؼ�.

		//cpu tick�� ���Ӱ� ����.
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//���ΰ��ӿ��� Update()ȣ��ǰ� ���� Update()������ 
	//���͹����� �������� ������ 1�ʵ��� ���� �ִ� �������� ũ�Ⱑ ���´�.(�ð���)

	
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
