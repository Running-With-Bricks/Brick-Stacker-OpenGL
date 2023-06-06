#include "pch.hpp"

#ifdef BS_PLATFORM_WINDOWS

#include "BrickStacker/Utils/PlatformUtils.hpp"
#include "BrickStacker/Application/Application.hpp"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace BrickStacker {

	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetWindowHandle());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();

	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetWindowHandle());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	bool FileDialogs::ChangeFile(Ref<Scene> scene)
	{
		if (scene->FilePath.empty())
		{
			int msgboxID = MessageBox(
				glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetWindowHandle()),
				(LPCWSTR)L"Your map is not saved!\nDo you want to save it?",
				(LPCWSTR)L"Unsaved map",
				MB_ICONWARNING | MB_YESNOCANCEL | MB_DEFBUTTON1
			);

			switch (msgboxID)
			{
			case IDYES:
				return !SaveFile("Brick-Hill Map (*.brk)\0*.brk\0").empty();
				break;
			case IDNO:
				return true;
				break;
			case IDCANCEL:
				return false;
				break;
			}

			return false;
		}
		else //TODO: when implementing history(Ctrl+Z/Y) don't forget to do a check if scene was edited
			return true;
	}
}

#endif