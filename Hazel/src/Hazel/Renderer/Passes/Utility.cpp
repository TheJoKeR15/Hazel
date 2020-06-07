#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"
#include "Commun.h"

namespace Hazel
{
	void Scene::InitializeCommunBuffers(float ViewPortSizeX, float ViewPortSizeY)
	{
		m_ViewPortSizeX = ViewPortSizeX;
		m_ViewPortSizeY = ViewPortSizeY;

		// Viewport Frame buffer
		uint32_t a;
		MainFrameBuffer = Hazel::FrameBuffer::Create(a, (int)ViewPortSizeX, (int)ViewPortSizeY);
		MainFrameBuffer->Bind();
		FrameBuffertexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true, false);
		MainFrameBuffer->AttachColorTexture2D(*FrameBuffertexture2D->GetID(), (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		FrameRenderBuffer = MainFrameBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);
		MainFrameBuffer->Unbind();

		//Init ShadowDepthBuffer
		ShadowDepthBuffer = Hazel::FrameBuffer::Create(Buffer1, SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		ShadowDepthBuffer->Bind();
		ShadowMap = Texture2D::CreateDepth(SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale, false,false);
		ShadowDepthBuffer->AttachDepthTexture2D(*ShadowMap->GetID(), SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		ShadowDepthBuffer->Unbind();

		// Post Process Buffer
		HDRBuffer = Hazel::FrameBuffer::Create(Buffer2, (int)ViewPortSizeX, (int)ViewPortSizeY);
		HDRBuffer->Bind();
		BloomTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		ExposedTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		HDRBuffer->AttachColorTexture2D(*ExposedTexture2D->GetID(), (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		HDRBuffer->AttachColorTexture2D(*BloomTexture2D->GetID(), (int)ViewPortSizeX, (int)ViewPortSizeY, 1);

		HDRRenderBuffer = HDRBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);
		//FrameBuffertexture2D->Bind(m_PostProcessShader->GetNextTextureSlotIndex());
		HDRBuffer->DrawBuffers(2);
		HDRBuffer->Unbind();

		// Final Composition Shader
		//uint32_t Buffer3;
		CompositionBuffer = Hazel::FrameBuffer::Create(Buffer3, (int)ViewPortSizeX, (int)ViewPortSizeY);
		CompositionBuffer->Bind();
		FinalImageTexture2D = Texture2D::Create((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		//FinalImageTexture2D->SetID(FinalImageTexture);
		//FinalImageTexture = CompositionBuffer->CreateColorTexture2D((int)ViewPortSizeX, (int)ViewPortSizeY, false, true);
		CompositionBuffer->AttachColorTexture2D(*FinalImageTexture2D->GetID(), (int)ViewPortSizeX, (int)ViewPortSizeY, 0);
		auto renderbuffer = HDRBuffer->CreateAndAttachRenderBuffer((int)ViewPortSizeX, (int)ViewPortSizeY);

		//ExposedTexture2D->Bind(m_CompositionShader->GetNextTextureSlotIndex());
		//BloomTexture2D->Bind(m_CompositionShader->GetNextTextureSlotIndex());
		CompositionBuffer->Unbind();

		////////////////////////////////
		//// Bloom Buffers 
		////////////////////////////////
		
		for (int i = 0; i < bloomHFBOs.size(); i++)
		{
			

			
			auto dsfactor = DsFactors[i];
			bloomHFBOs[i] = FrameBuffer::Create(bloom_Buffer1, m_ViewPortSizeX/ dsfactor, m_ViewPortSizeY/ dsfactor);
			bloomHFBOs[i]->Bind();
			bloomGaussiansH[i] = Texture2D::Create(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor, false, true, true, true);
			bloomHFBOs[i]->AttachColorTexture2D(*bloomGaussiansH[i]->GetID(), m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomHFBOs[i]->CreateAndAttachRenderBuffer(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomHFBOs[i]->Unbind();

			bloomVFBOs[i] = FrameBuffer::Create(bloom_Buffer1, m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomVFBOs[i]->Bind();
			bloomGaussiansV[i] = Texture2D::Create(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor, false, true, true, true);
			bloomVFBOs[i]->AttachColorTexture2D(*bloomGaussiansV[i]->GetID(), m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomVFBOs[i]->CreateAndAttachRenderBuffer(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomVFBOs[i]->Unbind();

			bloomDFBOs[i] = FrameBuffer::Create(bloom_Buffer1, m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomDFBOs[i]->Bind();
			bloomDownSampled[i] = Texture2D::Create(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor, false, true, true, true);
			bloomDFBOs[i]->AttachColorTexture2D(*bloomDownSampled[i]->GetID(), m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomDFBOs[i]->CreateAndAttachRenderBuffer(m_ViewPortSizeX / dsfactor, m_ViewPortSizeY / dsfactor);
			bloomDFBOs[i]->Unbind();
			
		}

		bloom_FBO1 = FrameBuffer::Create(bloom_Buffer1, m_ViewPortSizeX / 4, m_ViewPortSizeY / 4);
		bloom_FBO1->Bind();
		bloomDownsampl01 = Texture2D::Create(m_ViewPortSizeX / 4, m_ViewPortSizeY / 4, false, true, true, true);
		bloom_FBO1->AttachColorTexture2D(*bloomDownsampl01->GetID(), m_ViewPortSizeX / 4, m_ViewPortSizeY / 4);
		bloom_FBO1->CreateAndAttachRenderBuffer(m_ViewPortSizeX / 4, m_ViewPortSizeY / 4);
		bloom_FBO1->Unbind();


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

	void Hazel::Scene::InitializeCommunShaders()
	{
		m_PostProcessShader = m_ShaderLibrary.Load("assets/shaders/PostProcess.glsl");
		m_CompositionShader = m_ShaderLibrary.Load("assets/shaders/Composition.glsl");
		m_ShadowPassShader = m_ShaderLibrary.Load("assets/shaders/DirectionalShadowMap.glsl");
		m_GaussianBlur = m_ShaderLibrary.Load("assets/shaders/GaussianBlur.glsl");
	}
}
