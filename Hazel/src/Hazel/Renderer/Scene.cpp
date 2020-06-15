#include "hzpch.h"
#include "Scene.h"
#include "Renderer.h"



namespace Hazel {

	Scene::Scene(float Width, float Height, Ref<Shader> shader) : m_CameraController(Width / Height, glm::vec3(0.f, 0.f, 5.f))
		, m_Width(Width), m_Height(Height) , m_shader(shader)
	{
		//InitializeScene();
	};
	Scene::Scene(float Width, float Height) : m_CameraController(Width / Height, glm::vec3(0.f, 0.f, 5.f))
		, m_Width(Width), m_Height(Height)
	{
		
		//InitializeScene();
	};

	Scene::~Scene()
	{
		for (int i = 0; i < Entities.size(); i++)
		{
			// Delete every entity 
			delete Entities[i];
			
		}
	}

	void Scene::InitializeScene()
	{
		
		
		HZ_CORE_INFO("Scene Initialized");
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnInit();
		}
		InitializeCommunShaders();
		InitializeScreenQuad();

	}



	void Scene::RenderScene(float dt)
	{
		lightProjection = glm::ortho(left, right, down, up, near_plane, far_plane);
		lightView = glm::lookAt(Light,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		m_CameraController.OnUpdate(dt);

		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			if (EnTT->bUpdate)
			{
				EnTT->OnUpdate(dt);
			}
			
		}
		//////////////////////
		// MAIN RENDER LOOP // 
		RenderAllPasses();
		//////////////////////
	}

	

	




	void Scene::BeginScene()
	{

		//ClearScene();

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
		auto pointlight =	dynamic_cast<PointLight*>(newEntinity);
		auto dirlight =		dynamic_cast<DirectionalLight*>(newEntinity);
		auto spotlight =	dynamic_cast<SpotLight*>(newEntinity);
		newEntinity->UniqueID = EntityIndex++;
		
		Entities.push_back(newEntinity);
		if (pointlight)
		{
			PointLights.push_back(pointlight);
		}
		if (dirlight)
		{
			DirLight = dirlight;
		}
		if (spotlight)
		{
			SpotLights.push_back(spotlight);
		}
	}

	//void Scene::AddPointLight(PointLight* newEntinity)
	//{
	//	newEntinity->UniqueID = EntityIndex++;
	//	newEntinity->m_id = PointLightIndex++;
	//	Entities.push_back(newEntinity);
	//	PointLights.push_back(newEntinity);
	//}
	void Scene::AddDirectionalLight(Entity* newEntinity)
	{
	}
	void Scene::AddSpotLight(Entity* newEntinity)
	{
	}
	void Scene::RemoveEnitity(uint32_t ID)
	{
		//delete Entities[ID];
		Entities.erase(Entities.begin() + ID);
	}
	void Scene::HandleEvent(Event& e)
	{

	}
	void Scene::ClearScene(bool Depth)
	{
		if (!Depth)
		{
			RenderCommand::SetClearColor(glm::vec4(BackGroundColor, 1.f));
			RenderCommand::Clear();
		}
		else
		{
			RenderCommand::ClearDepth();
		}
		//Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
	}
}