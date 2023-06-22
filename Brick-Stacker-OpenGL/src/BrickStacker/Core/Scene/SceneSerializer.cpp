#include "pch.hpp"
#include "SceneSerializer.hpp"
#include "BrickStacker/Core/Renderer/RenderCommand.hpp"

#include "BrickStacker/Base/std_split_str.hpp"

namespace BrickStacker
{
	void SceneSerializer::Deserialize(SceneSerializerSettings settings)
	{
		std::ifstream mapFile(settings.FilePath, std::ios::in);

		if (mapFile.is_open())
		{
			std::string line;

			bool wasAmbientEncountered{ false };
			bool wasBaseplateSizeEncountered{ false };
			bool completedSetup{ false };
			Entity currentEntity;

			Entity Lighting{ settings.ActiveScene->GetAllEntitiesWith<LightingComponent>().back(), settings.ActiveScene.get() };
			Entity Baseplate{ settings.ActiveScene->GetAllEntitiesWith<BaseplateComponent>().back(), settings.ActiveScene.get() };

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
						glm::vec3 color{ 0 };

						std::sscanf(separated[2].c_str(), "%f", &color.r);
						std::sscanf(separated[1].c_str(), "%f", &color.g);
						std::sscanf(separated[0].c_str(), "%f", &color.b);

						Baseplate.GetComponent<BaseplateComponent>().Color = color;
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

							uint32_t r = 0;
							std::sscanf(separated[0].c_str(), "%u", &r);

							Lighting.GetComponent<LightingComponent>().SunIntensity = r;
						}
					}
				}
				else if (separated.size() > 0 && separated[0].rfind("\t+", 0) == 0)
				{
					//if line starts with "    +"
					//example: "    +NAME Brick2"
					if (separated[0].rfind("\t+NAME", 0) == 0 || (settings.ProcessBrickStackerSpecifics && separated[0].rfind("\t+$NAME", 0) == 0))
					{
						std::string name = "";
						for (int i = 1; i < separated.size(); i++)
						{
							name += (i == 1 ? "" : " ") + separated[i];
						}

						currentEntity.GetComponent<NameComponent>() = name;
					}
					else if (separated[0].rfind("\t+ROT", 0) == 0)
					{
						int rot{ 0 };
						std::sscanf(separated[1].c_str(), "%d", &rot);

						auto& transform = currentEntity.GetComponent<TransformComponent>();
						if ((rot % 180) != 0)
						{
							float tempXScale = static_cast<float>(transform.Scale.x);
							transform.Scale.x = transform.Scale.z;
							transform.Scale.z = tempXScale;
						}

						transform.Rotation = rot;
					}
					else if (separated[0].rfind("\t+NOCOLLISION", 0) == 0)
					{
						currentEntity.GetComponent<BrickComponent>().Collision = false;
					}
					else if (settings.ProcessBrickStackerSpecifics)
					{
						if (separated[0].rfind("\t+$BP-INVISIBLE", 0) == 0)
							Baseplate.GetComponent<BaseplateComponent>().Visible = false;
						else if (separated[0].rfind("\t+$INVISIBLE", 0) == 0)
							currentEntity.GetComponent<ColorComponent>().Visible = false;
						else if (separated[0].rfind("\t+$PRIMARY_CAMERA", 0) == 0)
							settings.ActiveScene->SetPrimaryCameraEntity(currentEntity);
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

					auto Brick = settings.ActiveScene->CreateBrick("Brick");
					Brick.GetComponent<ColorComponent>() = color;
					Brick.AddOrReplaceComponent<TransformComponent>(position, 0, scale);

					currentEntity = Brick;
				}
				else if (settings.ProcessBrickStackerSpecifics && separated.size() > 0)
				{
					if (separated[0].rfind("$BsCamera", 0) == 0)
					{
						auto Camera = settings.ActiveScene->CreateCamera("Camera");
						auto& cc = Camera.GetComponent<CameraComponent>().camera;

						std::sscanf(separated[1].c_str(), "%f", &cc.FOV);

						std::sscanf(separated[2].c_str(), "%f", &cc.Position.x);
						std::sscanf(separated[3].c_str(), "%f", &cc.Position.y);
						std::sscanf(separated[4].c_str(), "%f", &cc.Position.z);

						std::sscanf(separated[5].c_str(), "%f", &cc.Rotation.x);
						std::sscanf(separated[6].c_str(), "%f", &cc.Rotation.y);
						std::sscanf(separated[7].c_str(), "%f", &cc.Rotation.z);

						std::sscanf(separated[8].c_str(), "%f", &cc.TargetPos.x);
						std::sscanf(separated[9].c_str(), "%f", &cc.TargetPos.y);
						std::sscanf(separated[10].c_str(), "%f", &cc.TargetPos.z);

						std::sscanf(separated[11].c_str(), "%f", &cc.Zoom);
						std::sscanf(separated[12].c_str(), "%f", &cc.Planes.Near);
						std::sscanf(separated[13].c_str(), "%f", &cc.Planes.Far);
						std::sscanf(separated[14].c_str(), "%f", &cc.Distance);

						int type = (int)CameraType::Perspective;
						int behaviour = (int)CameraBehaviour::None;
						std::sscanf(separated[15].c_str(), "%d", &type);
						std::sscanf(separated[16].c_str(), "%d", &behaviour);
						cc.Type = (CameraType)type;
						cc.Behaviour = (CameraBehaviour)behaviour;

						currentEntity = Camera;
					}
				}
			}
		}
		else
			BS_ERROR("Couldn't open {0} to load the map", settings.FilePath);

		mapFile.close();

		for (auto ent : settings.ActiveScene->GetAllEntitiesWith<PhysicsComponent>())
			Entity(ent, settings.ActiveScene.get()).GetComponent<PhysicsComponent>().Update();
	}
	void SceneSerializer::Serialize(SceneSerializerSettings settings)
	{
		std::ofstream outFile(settings.FilePath);
		if (outFile.is_open()) {
			const LightingComponent& lc = Entity(settings.ActiveScene->GetAllEntitiesWith<LightingComponent>().back(), settings.ActiveScene.get()).GetComponent<LightingComponent>();
			auto b = Entity(settings.ActiveScene->GetAllEntitiesWith<BaseplateComponent>().back(), settings.ActiveScene.get());
			const auto& bc = b.GetComponent<BaseplateComponent>();

			outFile << "B R I C K  W O R K S H O P  V0.2.0.0" << std::endl;
			outFile << "Created_With Brick-Stacker" << std::endl;
			outFile << lc.AmbientColor.b << " " << lc.AmbientColor.g << " " << lc.AmbientColor.r << std::endl;
			outFile << bc.Color.b << " " << bc.Color.g << " " << bc.Color.r << " 1" << std::endl;
			outFile << lc.SkyColor.r << " " << lc.SkyColor.g << " " << lc.SkyColor.b << std::endl;
			outFile << bc.Size << std::endl;
			outFile << lc.SunIntensity << std::endl;
			if (settings.ProcessBrickStackerSpecifics && !bc.Visible)
				outFile << "$BP-INVISIBLE";
			outFile << std::endl << std::endl;

			auto& entities = settings.ActiveScene->GetRoot();
			for (auto& entityID : entities)
			{
				auto entity = Entity(entityID, settings.ActiveScene.get());
				if (entity.HasComponent<BrickComponent>())
				{
					const auto& tc = entity.GetComponent<TransformComponent>();
					const auto& bcc = entity.GetComponent<ColorComponent>();
					auto& bbcc = entity.GetComponent<BrickComponent>();

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
					if (!bbcc.Collision)
						outFile << "\t+NOCOLLISION" << std::endl;
					if (tc.Rotation != 0)
						outFile << "\t+ROT " << tc.Rotation << std::endl;
					if (settings.ProcessBrickStackerSpecifics && !bcc.Visible)
						outFile << "\t+$INVISIBLE" << std::endl;
				}
				else if (settings.ProcessBrickStackerSpecifics)
				{
					if (entity.HasComponent<CameraComponent>())
					{
						const auto& cc = entity.GetComponent<CameraComponent>().camera;
						outFile << "$BsCamera " << cc.FOV << " ";
						for (int i = 0; i < 3; i++)
							outFile << cc.Position[i] << " ";
						for (int i = 0; i < 3; i++)
							outFile << cc.Rotation[i] << " ";
						for (int i = 0; i < 3; i++)
							outFile << cc.TargetPos[i] << " ";
						outFile << cc.Zoom << " " << cc.Planes.Near << " " << cc.Planes.Far << " " << cc.Distance << " " << (int)cc.Type << " " << (int)cc.Behaviour << std::endl;
						outFile << "\t+$NAME" << " " << entity.GetComponent<NameComponent>().Name << std::endl;
						if (settings.ActiveScene->GetPrimaryCameraEntity() == entity)
							outFile << "\t+$PRIMARY_CAMERA" << std::endl;
					}
				}
			}

			outFile.close();
			BS_INFO("Writen Scene into {0}", settings.FilePath);
		}
		else
			BS_ERROR("Couldn't open {0} to save the map", settings.FilePath);
	}

	Ref<Scene> SceneSerializer::GetDefaultScene()
	{
		Ref<Scene> defaultScene;

		defaultScene = CreateRef<Scene>();

		auto Baseplate = defaultScene->CreateEntity("Baseplate");
		Baseplate.AddComponent<BaseplateComponent>();
		Baseplate.AddComponent<PhysicsComponent>(defaultScene->m_PhysicsWorld->AddBrick(Baseplate), defaultScene->m_PhysicsWorld);

		auto Lighting = defaultScene->CreateEntity("Lighting");
		Lighting.AddComponent<LightingComponent>();

		auto camera = defaultScene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>();
		camera.GetComponent<CameraComponent>().camera.Position = { 0, 1, -5 };
		defaultScene->SetPrimaryCameraEntity(camera);

		return defaultScene;
	}
}
