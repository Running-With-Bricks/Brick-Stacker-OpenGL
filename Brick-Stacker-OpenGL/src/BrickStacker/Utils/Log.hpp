#pragma once

#include "BrickStacker/Core/Core.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

namespace BrickStacker
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; };
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#ifdef BS_DEBUG
	#define BS_ENABLE_ASSERTS
#endif


#define BS_TRACE(...) ::BrickStacker::Log::GetLogger()->trace(__VA_ARGS__)
#define BS_INFO(...)  ::BrickStacker::Log::GetLogger()->info(__VA_ARGS__)
#define BS_WARN(...)  ::BrickStacker::Log::GetLogger()->warn(__VA_ARGS__)
#define BS_ERROR(...) ::BrickStacker::Log::GetLogger()->error(__VA_ARGS__)
#define BS_FATAL(...) ::BrickStacker::Log::GetLogger()->critical(__VA_ARGS__)

#ifdef BS_ENABLE_ASSERTS
	#define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define BS_ASSERT(x, ...) { if(!(x)) { BS_ERROR(__VA_ARGS__); }; }
#endif
