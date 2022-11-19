#pragma once

#include "DiscordGameSDK/discord.h"

namespace BrickStacker
{
	class Discord
	{
	public:
		//Delete the copying constructors
		Discord(const Discord&) = delete;
		Discord& operator=(const Discord&) = delete;

		/// <summary>
		/// Static func that returns refernce to the Discord, since this is a singleton
		/// </summary>
		/// <returns>Refernce to Discord thing</returns>
		static Discord& Get()
		{
			static Discord Discord{};
			return Discord;
		}

		/// <summary>
		/// Updates discord stuff, also does check whether discord was opened to start RPC again
		/// </summary>
		void Update();

	private:
		Discord();
		~Discord();

		void CreateCore();
		void CreateActivity();

		discord::Core* m_Core;
		discord::ActivityManager* m_ActivityManager;
		discord::Activity m_Activity;
	};
}
