#pragma once

#include <chrono>

namespace BrickStacker
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Reset();
		void Stop();
		float Elapsed();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		float m_ElapsedTime = 0;
	};
}
