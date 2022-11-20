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

	void Discord::SetActivityState(const char* StateText)
	{
		m_Activity.SetState(StateText);
		if (m_ActivityManager != nullptr)
			m_ActivityManager->UpdateActivity(m_Activity, [](discord::Result result) {});
	}

	void Discord::SetActivityDetails(const char* DetailsText)
	{
		m_Activity.SetDetails(DetailsText);
		if (m_ActivityManager != nullptr)
			m_ActivityManager->UpdateActivity(m_Activity, [](discord::Result result) {});
	}

	void Discord::CreateCore()
	{
		if (discord::Core::Create(1043533206659547166, DiscordCreateFlags_NoRequireDiscord, &m_Core) == discord::Result::Ok)
		{
			m_ActivityManager = &m_Core->ActivityManager();
			m_ActivityManager->UpdateActivity(m_Activity, [](discord::Result result) {});
		}
	}

	void Discord::CreateActivity()
	{
		m_Activity = discord::Activity{};
		m_Activity.SetState("Organizing the vertex, vertax array and index buffers");
		m_Activity.GetTimestamps().SetStart(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
}
