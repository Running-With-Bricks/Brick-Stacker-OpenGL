#include "DiscordRichPresence.hpp"

#include <chrono>

namespace BrickStacker
{
	Discord::Discord()
	{
		CreateActivity();
		CreateCore();
	}

	Discord::~Discord()
	{

	}

	void Discord::Update()
	{
		if (m_Core == nullptr || m_Core->RunCallbacks() == discord::Result::NotRunning)
		{
			CreateCore();
		}
	}

	void Discord::CreateCore()
	{
		if (discord::Core::Create(1043533206659547166, DiscordCreateFlags_NoRequireDiscord, &m_Core) == discord::Result::Ok)
		{
			discord::ActivityManager* activityManager = &m_Core->ActivityManager();
			activityManager->UpdateActivity(m_Activity, [](discord::Result result) {});
		}
	}

	void Discord::CreateActivity()
	{
		m_Activity = discord::Activity{};
		m_Activity.SetState("brick building, brick build together part piece construct make create set.");
		m_Activity.GetTimestamps().SetStart(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
}
