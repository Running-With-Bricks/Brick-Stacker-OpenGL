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
		m_Start = std::chrono::high_resolution_clock::now();
	}

	float Timer::Elapsed()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
	}
}
