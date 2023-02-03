#include "pch.hpp"

#include "BrickStacker/Application/Application.hpp"
#include "BrickStacker/Utils/Log.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	BrickStacker::Log::Init();
	BrickStacker::Log::GetLogger()->trace("Started the Logger");
	BrickStacker::Application& Application = BrickStacker::Application::Get();

	try
	{
		Application.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
