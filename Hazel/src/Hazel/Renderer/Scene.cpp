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
		ShadowMap = Texture2D::Create(SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
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

	void Scene::InitializeScene(Ref<Shader> ShadowPassShader)
	{
		m_ShadowPassShader = ShadowPassShader;
		HZ_CORE_INFO("Scene Initialized");
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnInit();
		}
	}

	void Scene::InitializeBuffers(float ViewPortSizeX, float ViewPortSizeY)
	{
		m_ViewPortSizeX = ViewPortSizeX;
		m_ViewPortSizeY = ViewPortSizeY;
		// Viewport Frame buffer
		uint32_t a;
		FrameBuffer = Hazel::FrameBuffer::Create(a, ViewPortSizeX, ViewPortSizeY);
		FrameBuffer->Bind();
		FrameBuffertexture = FrameBuffer->AttachColorTexture2D(ViewPortSizeX, ViewPortSizeY, 0, 0);
		FrameRenderBuffer = FrameBuffer->AttachRenderBuffer(ViewPortSizeX, ViewPortSizeY);
		FrameBuffer->Unbind();
		//Init ShadowDepthBuffer
		ShadowDepthBuffer = Hazel::FrameBuffer::Create(Buffer1, SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height* ShadowMapScale);
		ShadowDepthBuffer->Bind();
		DirectionalShadowMap = ShadowDepthBuffer->AttachDepthTexture2D(SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		ShadowDepthBuffer->Unbind();

		
		ShadowMap->SetID(DirectionalShadowMap);

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
		// Shadow Pas 
		// Better Shadow mapping
		//RenderCommand::SetFaceCulling(true);
		RenderShadowPass();
		RenderCommand::SetFaceCulling(false);
		// Main pass
		RenderMainPass();


	}

	void Scene::RenderMainPass()
	{


		RenderCommand::SetViewport(0, 0, m_ViewPortSizeX, m_ViewPortSizeY);
		FrameBuffer->Bind();
		ClearScene();
		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
		// Update the camera first 
		
		// Loop^in the list of all the entitiy and render the one flaged to 
		bool firsttime = true;
		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bUpdate & EnTT->bVisible)
			{
				if (EnTT->bRecieveShadow)
				{
					if (firsttime)
					{
						//EnTT->m_Shader->SetInt("DirectionalShadowMap", DirectionalShadowMap);
						ShadowMap->Bind(EnTT->m_Shader->GetNextTextureSlotIndex());
						firsttime = false;
					}
					
					EnTT->m_Shader->SetInt("DirectionalShadowMap", *ShadowMap->GetSlot());
					EnTT->m_Shader->SetMat4("lightSpaceMatrix", lightProjection * lightView);
					EnTT->m_Shader->SetFloat("ShadowBias", ShadowBias);
					//EnTT->m_Shader->SetFloat3("TEXELSIZE", glm::vec3(1.f/ (SHADOW_MAP_Width * ShadowMapScale), 1.f/(SHADOW_MAP_Height * ShadowMapScale),0.0f));
				}
				// trigger on update event
				EnTT->DrawMainPass();
			}
		}
		FrameBuffer->Unbind();
	}

	void Scene::RenderShadowPass()
	{

		ShadowDepthBuffer->Bind();
		m_ShadowPassShader->Bind();

		RenderCommand::SetViewport(0, 0, SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		m_ShadowPassShader->SetMat4("u_ViewProjection", lightProjection * lightView);
		
		ClearScene(true);
		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bCastShadow && EnTT->bUpdate)
			{
				// trigger on update event
				
				//m_ShadowPassShader->SetMat4("model", EnTT->transform);
				
				EnTT->DrawShawdowPass(m_ShadowPassShader);
			}
		}
		ShadowDepthBuffer->Bind();
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
		newEntinity->UniqueID = EntityIndex++;
		
		Entities.push_back(newEntinity);
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