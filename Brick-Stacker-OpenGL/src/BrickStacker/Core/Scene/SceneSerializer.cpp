#include "pch.hpp"
#include "SceneSerializer.hpp"
#include "Entity.hpp"

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

			auto LightComponents = ActiveScene->GetAllEntitiesWith<LightingComponent>();
			Entity Lighting;
			if (LightComponents.size() == 0)
			{
				Lighting = ActiveScene->CreateEntity("Lighting");
				Lighting.AddComponent<LightingComponent>();
			}
			else
				Lighting = Entity(LightComponents[0], ActiveScene.get());

			auto BaseplateComponents = ActiveScene->GetAllEntitiesWith<BaseplateComponent, ColorComponent>();
			Entity Baseplate;
			if (BaseplateComponents.size() == 0)
			{
				Baseplate = ActiveScene->CreateEntity("Baseplate");
				Baseplate.AddComponent<BaseplateComponent>();
				Baseplate.AddComponent<ColorComponent>(glm::vec4(0.20f, 0.51f, 0.14f, 1));
			}
			else
			{
				Baseplate = Entity(BaseplateComponents.front(), ActiveScene.get());
			}

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

							std::sscanf(separated[2].c_str(), "%f", &color.r);
							std::sscanf(separated[1].c_str(), "%f", &color.g);
							std::sscanf(separated[0].c_str(), "%f", &color.b);

							Lighting.GetComponent<LightingComponent>().SkyColor = color;
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

						transform.Rotation.y = static_cast<float>(rot);
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
					Brick.AddComponent<TransformComponent>(position, glm::vec3(0), scale);

					currentBrick = Brick;
				}
			}
		}
		else
			BS_ERROR("Couldn't open {0} to load the map");

		mapFile.close();
	}
}
