#pragma once

namespace BrickStacker
{
	//Singleton class for app
	class BSApplication
	{
	public:
		//Static func that returns refernce to the application, since this is a singleton
		static BSApplication& Get()
		{
			static BSApplication Application{};
			return Application;
		}

		//Runs the game loop
		void run();

	private:
		BSApplication();
		~BSApplication();
	};
}
