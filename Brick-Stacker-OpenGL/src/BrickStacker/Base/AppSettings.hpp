#pragma once
#include "pch.hpp"

namespace BrickStacker
{
	class AppSettings
	{
	private:
		AppSettings() = default;
		~AppSettings() = default;

		struct AppSettingsData
		{
			bool IncludeBrickStackerSpecifics = true;

			bool WireframeBrickRendering = false;
			bool WireframeBaseplateRendering = false;
			bool CollisionRendering = false;
		};
	public:
		AppSettingsData data{};

		AppSettings(const AppSettings&) = delete;
		AppSettings& operator=(const AppSettings&) = delete;

		static AppSettingsData& Get()
		{
			static AppSettings AppSettings{};
			return AppSettings.data;
		}

		static void LoadSettings();
		static void ResetSettings();
		static void SaveSettings();

	};
}
