#include "pch.hpp"

#include "BrickStacker/Application/Application.hpp"
#include "BrickStacker/Utils/Log.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifdef BS_CUTOUT_CONSOLE && BS_PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	BrickStacker::Log::Init();
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
