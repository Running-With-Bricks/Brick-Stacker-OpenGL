#include "pch.hpp"
#include "AppSettings.hpp"

#include "std_split_str.hpp"

namespace BrickStacker
{
	void AppSettings::LoadSettings()
	{
		ResetSettings();

		std::ifstream file{ "Brick-Stacker Settings.ini", std::ios::in };

		if (file.is_open())
		{
			std::string line;

			while (std::getline(file, line))
			{
				std::vector<std::string> separated;
				std::split_str(line, '=', separated);

				if (separated.size() == 2)
					continue;

				if (separated[0] == "IncludeBrickStackerSpecifics" && (separated[1] == "true" || separated[1] == "false"))
				{
					Get().IncludeBrickStackerSpecifics = separated[1] == "true";
				}
				else if (separated[0] == "WireframeBrickRendering" && (separated[1] == "true" || separated[1] == "false"))
				{
					Get().WireframeBrickRendering = separated[1] == "true";
				}
				else if (separated[0] == "WireframeBaseplateRendering" && (separated[1] == "true" || separated[1] == "false"))
				{
					Get().WireframeBaseplateRendering = separated[1] == "true";
				}
				else if (separated[0] == "CollisionRendering" && (separated[1] == "true" || separated[1] == "false"))
				{
					Get().CollisionRendering = separated[1] == "true";
				}
			}
		}

		file.close();
	}

	void AppSettings::ResetSettings()
	{
		Get() = AppSettingsData();
	}

	void AppSettings::SaveSettings()
	{
		std::ofstream file{ "Brick-Stacker Settings.ini" };

		if (file.is_open())
		{
			file << std::string("IncludeBrickStackerSpecifics=") + (Get().IncludeBrickStackerSpecifics ? "true" : "false") << std::endl;
			file << std::string("WireframeBrickRendering=") + (Get().WireframeBrickRendering ? "true" : "false") << std::endl;
			file << std::string("WireframeBaseplateRendering=") + (Get().WireframeBaseplateRendering ? "true" : "false") << std::endl;
			file << std::string("CollisionRendering=") + (Get().CollisionRendering ? "true" : "false") << std::endl;
		}

		file.close();
	}
}
