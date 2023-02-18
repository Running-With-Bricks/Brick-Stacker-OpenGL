#include "pch.hpp"
#include "Log.hpp"

namespace BrickStacker
{
	Ref<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("BRICK-STACKER");
		s_Logger->set_level(spdlog::level::trace);
	}
}
