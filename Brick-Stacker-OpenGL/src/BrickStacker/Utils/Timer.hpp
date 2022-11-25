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
		float Elapsed();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
