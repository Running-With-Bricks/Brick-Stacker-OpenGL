#pragma once

#include "BrickStacker/Core/Graphics/BSWindow.hpp"

namespace BrickStacker
{
	//Singleton class for app
	class BSApplication
	{
	public:
		//Delete the copying constructors
		BSApplication(const BSApplication&) = delete;
		BSApplication& operator=(const BSApplication&) = delete;

		/// <summary>
		/// Static func that returns refernce to the application, since this is a singleton
		/// </summary>
		/// <returns>Refernce to BSApplication</returns>
		static BSApplication& Get()
		{
			static BSApplication Application{};
			return Application;
		}

		/// <summary>
		/// Runs the game loop
		/// </summary>
		void run();

	private:
		BSApplication();
		~BSApplication();

		BSWindow m_BSWindow{ 800, 600, "Brick Stacker" };
	};
}
