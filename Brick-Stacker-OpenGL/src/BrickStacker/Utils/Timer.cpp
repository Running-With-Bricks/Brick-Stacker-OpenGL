#include "pch.hpp"
#include "Timer.hpp"

namespace BrickStacker
{
	Timer::Timer()
	{
		Reset();
	}

	Timer::~Timer()
	{

	}

	void Timer::Reset()
	{
		m_ElapsedTime = 0;
		m_Start = std::chrono::high_resolution_clock::now();
	}

	void Timer::Stop()
	{
		m_ElapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
	}

	float Timer::Elapsed()
	{
		if (!m_ElapsedTime)
			Stop();

		return m_ElapsedTime;
	}
}
