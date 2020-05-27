#include "hzpch.h"
#include "Scene.h"
#include "Renderer.h"

namespace Hazel {

	Scene::Scene(float Width, float Height) : m_CameraController(Width / Height, glm::vec3(0.f, 0.f, 5.f)), m_Width(Width), m_Height(Height)
	{
		InitializeScene();
	};

	void Scene::InitializeScene()
	{
		HZ_CORE_INFO("Scene Initialized");
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnInit();
		}
	}

	void Scene::RenderScene(float dt)
	{
		// Update the camera first 
		m_CameraController.OnUpdate(dt);

		// Loop^in the list of all the entitiy and render the one flaged to 

		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bUpdate)
			{
				// trigger on update event
				EnTT->OnUpdate(dt);
			}
		}
	}

	void Scene::BeginScene()
	{
		RenderCommand::SetClearColor(glm::vec4(BackGroundColor, 1.f));
		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
		
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnBeginFrame();
		}
	}

	void Scene::EndScene()
	{
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnEndFrame();
		}
	}

	void Scene::AddEnitity(Entity* newEntinity)
	{
		newEntinity->UniqueID = EntityIndex++;
		Entities.push_back(newEntinity);
	}
	void Scene::RemoveEnitity(uint32_t ID)
	{
		//delete Entities[ID];
		Entities.erase(Entities.begin() + ID);
	}
	void Scene::HandleEvent(Hazel::Event& e)
	{

	}
}