#pragma once

namespace BrickStacker
{
	//Singleton class for app
	class BSApplication
	{
	public:
		//Returns refernce to the application, since this is a singleton
		static BSApplication& Get()
		{
			static BSApplication Application{};
			return Application;
		}

		void run();

	private:
		BSApplication();
		~BSApplication();
	};
}
