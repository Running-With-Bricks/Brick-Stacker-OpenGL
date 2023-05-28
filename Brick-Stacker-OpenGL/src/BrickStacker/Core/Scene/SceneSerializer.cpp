#include "pch.hpp"
#include "SceneSerializer.hpp"
#include "BrickStacker/Core/Renderer/RenderCommand.hpp"

namespace std
{
	void split_str(const std::string& str, const char delim, std::vector<std::string>& out)
	{
		// create a stream from the string  
		std::stringstream s(str);

		std::string s2;
		while (std::getline(s, s2, delim))
		{
			out.push_back(s2); // store the string in s2  
		}
	}
}

namespace BrickStacker
{
	void SceneSerializer::Deserialize(const std::string& FilePath, Ref<Scene> ActiveScene)
	{
		std::ifstream mapFile(FilePath, std::ios::in);

		if (mapFile.is_open())
		{
			std::string line;

			bool wasAmbientEncountered{ false };
			bool wasBaseplateSizeEncountered{ false };
			bool completedSetup{ false };
			Entity currentBrick;

			Entity Lighting{ ActiveScene->GetAllEntitiesWith<LightingComponent>().back(), ActiveScene.get() };
			Entity Baseplate{ ActiveScene->GetAllEntitiesWith<BaseplateComponent>().back(), ActiveScene.get() };

			while (std::getline(mapFile, line))
			{
				std::vector<std::string> separated;
				std::split_str(line, ' ', separated);

				if (!completedSetup)
				{
					if (separated.size() == 3)
					{
						if (!wasAmbientEncountered)
						{
							wasAmbientEncountered = true;
							//Ambient Color
							glm::vec3 color{ 0 };

							std::sscanf(separated[2].c_str(), "%f", &color.r);
							std::sscanf(separated[1].c_str(), "%f", &color.g);
							std::sscanf(separated[0].c_str(), "%f", &color.b);

							Lighting.GetComponent<LightingComponent>().AmbientColor = color;
						}
						else
						{
							//Sky Color
							glm::vec3 color{ 0 };

							std::sscanf(separated[0].c_str(), "%f", &color.r);
							std::sscanf(separated[1].c_str(), "%f", &color.g);
							std::sscanf(separated[2].c_str(), "%f", &color.b);

							Lighting.GetComponent<LightingComponent>().SkyColor = color;
							RenderCommand::SetClearColor(color, 1);
						}
					}
					else if (separated.size() == 4)
					{
						//Baseplate Color + Alpha
						glm::vec4 color{ 0 };

						std::sscanf(separated[2].c_str(), "%f", &color.r);
						std::sscanf(separated[1].c_str(), "%f", &color.g);
						std::sscanf(separated[0].c_str(), "%f", &color.b);
						std::sscanf(separated[3].c_str(), "%f", &color.a);

						Baseplate.GetComponent<ColorComponent>() = color;
					}
					else if (separated.size() == 1)
					{
						if (!wasBaseplateSizeEncountered)
						{
							wasBaseplateSizeEncountered = true;
							//Baseplate Size

							float r = 0;
							std::sscanf(separated[0].c_str(), "%f", &r);

							Baseplate.GetComponent<BaseplateComponent>() = r;
						}
						else
						{
							completedSetup = true;
							//Sun Intensity

							float r = 0;
							std::sscanf(separated[0].c_str(), "%f", &r);

							Lighting.GetComponent<LightingComponent>().SunIntensity = r;
						}
					}
				}
				else if (separated.size() > 0 && separated[0].rfind("\t+", 0) == 0)
				{
					//if line starts with "    +"
					//example: "    +NAME Brick2"
					if (separated[0].rfind("\t+NAME", 0) == 0)
					{
						std::string name = "";
						for (int i = 1; i < separated.size(); i++)
						{
							name += (i == 1 ? "" : " ") + separated[i];
						}

						currentBrick.GetComponent<NameComponent>() = name;
					}
					else if (separated[0].rfind("\t+ROT", 0) == 0)
					{
						int rot{ 0 };
						std::sscanf(separated[1].c_str(), "%d", &rot);

						auto& transform = currentBrick.GetComponent<TransformComponent>();
						if ((rot % 180) != 0)
						{
							float tempXScale = static_cast<float>(transform.Scale.x);
							transform.Scale.x = transform.Scale.z;
							transform.Scale.z = tempXScale;
						}

						transform.Rotation = rot;
					}
				}
				else if (separated.size() == 10)
				{
					//Brick Start
					glm::vec3 position{ 0 };
					glm::vec3 scale{ 0 };
					glm::vec4 color{ 0, 0, 0, 1 };

					std::sscanf(separated[0].c_str(), "%f", &position.x);
					std::sscanf(separated[2].c_str(), "%f", &position.y);
					std::sscanf(separated[1].c_str(), "%f", &position.z);

					std::sscanf(separated[3].c_str(), "%f", &scale.x);
					std::sscanf(separated[5].c_str(), "%f", &scale.y);
					std::sscanf(separated[4].c_str(), "%f", &scale.z);

					std::sscanf(separated[6].c_str(), "%f", &color.r);
					std::sscanf(separated[7].c_str(), "%f", &color.g);
					std::sscanf(separated[8].c_str(), "%f", &color.b);
					std::sscanf(separated[9].c_str(), "%f", &color.a);

					position += scale * .5f;

					auto Brick = ActiveScene->CreateEntity("Brick");
					Brick.AddComponent<BrickComponent>();
					Brick.AddComponent<ColorComponent>(color);
					Brick.AddComponent<TransformComponent>(position, 0, scale);

					currentBrick = Brick;
				}
			}
		}
		else
			BS_ERROR("Couldn't open {0} to load the map");

		mapFile.close();
	}
	void SceneSerializer::Serialize(const std::string& FilePath, Ref<Scene> ActiveScene)
	{
		std::ofstream outFile(FilePath);
		if (outFile.is_open()) {
			const LightingComponent& lc = Entity(ActiveScene->GetAllEntitiesWith<LightingComponent>().back(), ActiveScene.get()).GetComponent<LightingComponent>();
			auto b = Entity(ActiveScene->GetAllEntitiesWith<BaseplateComponent>().back(), ActiveScene.get());
			const auto& bc = b.GetComponent<BaseplateComponent>();
			const auto& cc = b.GetComponent<ColorComponent>();

			outFile << "B R I C K  W O R K S H O P  V0.2.0.0" << std::endl;
			outFile << "Created_With_Brick-Stacker" << std::endl;
			outFile << lc.AmbientColor.b << " " << lc.AmbientColor.g << " " << lc.AmbientColor.r << std::endl;
			outFile << cc.Color.b << " " << cc.Color.g << " " << cc.Color.r << " " << cc.Color.a << std::endl;
			outFile << lc.SkyColor.r << " " << lc.SkyColor.g << " " << lc.SkyColor.b << std::endl;
			outFile << bc.Size << std::endl;
			outFile << bc.Size << std::endl;
			outFile << std::endl << std::endl;

			auto& view = ActiveScene->GetAllEntitiesWith<BrickComponent>();
			for (auto& entityIT = view.rbegin(); entityIT != view.rend(); entityIT++)
			{
				auto entity = Entity(*entityIT, ActiveScene.get());
				const auto& tc = entity.GetComponent<TransformComponent>();
				const auto& bcc = entity.GetComponent<ColorComponent>();

				auto scl = tc.Scale;
				if ((tc.Rotation % 180) != 0)
				{
					float tempXScale = static_cast<float>(scl.x);
					scl.x = scl.z;
					scl.z = tempXScale;
				}
				const auto pos = tc.Position - scl * 0.5f;

				outFile << pos.x << " " << pos.z << " " << pos.y << " " << scl.x << " " << scl.z << " " << scl.y << " " << bcc.Color.r << " " << bcc.Color.g << " " << bcc.Color.b << " " << bcc.Color.a << std::endl;
				outFile << "\t+NAME " << entity.GetComponent<NameComponent>().Name << std::endl;
				if (tc.Rotation != 0)
					outFile << "\t+ROT " << tc.Rotation << std::endl;
			}
			outFile.close();
			BS_INFO("Writen Scene into {0}", FilePath);
		}
		else
			BS_ERROR("Couldn't open {0} to save the map");
	}

	Ref<Scene> SceneSerializer::GetDefaultScene()
	{
		static Ref<Scene> defaultScene;

		if (!defaultScene.get())
		{
			defaultScene = CreateRef<Scene>();
			auto camera = defaultScene->CreateEntity("Camera");
			camera.AddComponent<CameraComponent>();
			camera.GetComponent<CameraComponent>().camera.Position = { 0, 1, -5 };
			defaultScene->SetPrimaryCameraEntity(camera);

			auto Baseplate = defaultScene->CreateEntity("Baseplate");
			Baseplate.AddComponent<BaseplateComponent>();
			Baseplate.AddComponent<ColorComponent>(glm::vec4(0.20f, 0.51f, 0.14f, 1));

			auto Lighting = defaultScene->CreateEntity("Lighting");
			Lighting.AddComponent<LightingComponent>();
		}

		return defaultScene;
	}
}
