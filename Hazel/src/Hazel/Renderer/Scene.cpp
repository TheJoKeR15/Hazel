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

	void Scene::InitializeScene(Ref<Shader> ShadowPassShader)
	{
		m_PostProcessShader = m_ShaderLibrary.Load("assets/shaders/PostProcess.glsl");
		m_CompositionShader = m_ShaderLibrary.Load("assets/shaders/Composition.glsl");
		m_ShadowPassShader = ShadowPassShader;
		HZ_CORE_INFO("Scene Initialized");
		for (int i = 0; i < Entities.size(); i++)
		{
			// Initialize every compopnent in the scene
			auto EnTT = Entities[i];
			EnTT->OnInit();
		}

		InitializeScreenQuad();

		
	}

	void Scene::InitializeBuffers(float ViewPortSizeX, float ViewPortSizeY)
	{
		m_ViewPortSizeX = ViewPortSizeX;
		m_ViewPortSizeY = ViewPortSizeY;
		// Viewport Frame buffer
		uint32_t a;
		FrameBuffer = Hazel::FrameBuffer::Create(a, (int)ViewPortSizeX, (int)ViewPortSizeY);
		FrameBuffer->Bind();
		FrameBuffertexture = FrameBuffer->CreateColorTexture2D((int)ViewPortSizeX, (int)ViewPortSizeY,false,true);
		FrameBuffer->AttachColorTexture2D(FrameBuffertexture, (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		FrameRenderBuffer = FrameBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);

		FrameBuffertexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY,false,true,false);
		FrameBuffertexture2D->SetID(FrameBuffertexture);
		
		FrameBuffer->Unbind();
		//Init ShadowDepthBuffer
		ShadowDepthBuffer = Hazel::FrameBuffer::Create(Buffer1, SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height* ShadowMapScale);
		ShadowDepthBuffer->Bind();
		DirectionalShadowMap = ShadowDepthBuffer->CreateAndAttachDepthTexture2D(SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		ShadowDepthBuffer->Unbind();
		ShadowMap = Texture2D::Create(SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale,false,false);
		ShadowMap->SetID(DirectionalShadowMap);
		
		// Post Process Buffer
		HDRBuffer = Hazel::FrameBuffer::Create(Buffer2, (int)ViewPortSizeX, (int)ViewPortSizeY);
		HDRBuffer->Bind();
		ExposedTexture = HDRBuffer->CreateColorTexture2D((int)ViewPortSizeX, (int)ViewPortSizeY,false, true);
		BloomTexture = HDRBuffer->CreateColorTexture2D((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		HDRBuffer->AttachColorTexture2D(ExposedTexture, (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		HDRBuffer->AttachColorTexture2D(BloomTexture, (int)ViewPortSizeX, (int)ViewPortSizeY, 1);
		BloomTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		BloomTexture2D->SetID(BloomTexture);
		ExposedTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		ExposedTexture2D->SetID(ExposedTexture);
		HDRRenderBuffer = HDRBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);
		FrameBuffertexture2D->Bind(m_PostProcessShader->GetNextTextureSlotIndex());
		HDRBuffer->DrawBuffers(2);
		HDRBuffer->Unbind();

		// Final Composition Shader
		//uint32_t Buffer3;
		CompositionBuffer = Hazel::FrameBuffer::Create(Buffer3, (int)ViewPortSizeX, (int)ViewPortSizeY);
		CompositionBuffer->Bind();
		FinalImageTexture = CompositionBuffer->CreateColorTexture2D((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		CompositionBuffer->AttachColorTexture2D(FinalImageTexture, (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		auto renderbuffer = HDRBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);
		FinalImageTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		FinalImageTexture2D->SetID(FinalImageTexture);
		ExposedTexture2D->Bind(m_CompositionShader->GetNextTextureSlotIndex());
		BloomTexture2D->Bind(m_CompositionShader->GetNextTextureSlotIndex());
		CompositionBuffer->Unbind();
	}

	void Scene::InitializeScreenQuad()
	{
		m_SquareVA = Hazel::VertexArray::Create();

		float squareVertices[4 * (2 + 2)] = {
			-1.f, -1.f, 		0.0f, 0.0f,
			 1.f, -1.f, 		1.0f, 0.0f,
			 1.f,  1.f, 		1.0f, 1.0f,
			-1.f,  1.f, 		0.0f, 1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float2, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoords" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);
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
		
		//Post Process Pass
		RenderPostPass();

		// Compose the final Image 
		ComposeFinalImage();

	}

	void Scene::RenderMainPass()
	{


		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
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

	void Scene::RenderPostPass()
	{
		
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		HDRBuffer->Bind();
		ClearScene();
		m_PostProcessShader->Bind();
		FrameBuffertexture2D->ReBind();
		m_PostProcessShader->SetInt("HDRImage", *FrameBuffertexture2D->GetSlot());
		
		m_PostProcessShader->SetFloat("ExposureCompensation", m_CameraController.ExposureComp);
		m_PostProcessShader->SetFloat("Aperture", m_CameraController.aperature);
		m_PostProcessShader->SetFloat("ShutterSpeed", 1.f/m_CameraController.ShutterSpeed);
		m_PostProcessShader->SetFloat("Iso", m_CameraController.Iso);

		m_PostProcessShader->SetFloat("BloomTreshold", m_CameraController.BloomTreshold);
		HDRBuffer->Bind();
		
		m_SquareVA->Bind();
		
		Renderer::Submit(m_PostProcessShader, m_SquareVA);

		HDRBuffer->Unbind();
	}

	void Scene::ComposeFinalImage()
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		CompositionBuffer->Bind();
		ClearScene();
		m_CompositionShader->Bind();
		ExposedTexture2D->ReBind();
		BloomTexture2D->ReBind();
		m_CompositionShader->SetInt("ExposedImage", *ExposedTexture2D->GetSlot());
		m_CompositionShader->SetInt("BloomImage", *BloomTexture2D->GetSlot());
		m_CompositionShader->SetFloat("BloomTreshold", m_CameraController.BloomTreshold);
		//CompositionBuffer->Bind();

		m_SquareVA->Bind();

		Renderer::Submit(m_CompositionShader, m_SquareVA);

		CompositionBuffer->Unbind();
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