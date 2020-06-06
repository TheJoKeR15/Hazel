#pragma once

//#include "Hazel/Core/Core.h"
//#include <assimp/Importer.hpp>	
#include "assimp/Importer.hpp"			// C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

#include "Hazel/Renderer/Entity.h"
#include "Hazel/Renderer/Lights/Light.h"
#include "Hazel/Renderer/Buffer.h"
#include "CameraController.h"
#include <vector>

namespace Hazel
{
	class Scene
	{
	public:
		Scene(float Width, float Height, Ref<Shader> shader);
		
		Scene(float Width, float Height);

		~Scene();
		void InitializeScene(Ref<Shader> ShadowPassShader);

		void InitializeBuffers(float ViewPortSizeX, float ViewPortSizeY);

		void InitializeScreenQuad();

		void BeginScene();

		void RenderScene(float dt);

		void RenderMainPass();

		void RenderShadowPass();

		void RenderPostPass();

		void ComposeFinalImage();

		void EndScene();

		void AddEnitity(Entity* newEntinity);

		//void AddPointLight(PointLight* newEntinity);

		void AddDirectionalLight(Entity* newEntinity);

		void AddSpotLight(Entity* newEntinity);

		void RemoveEnitity(uint32_t ID);

		void HandleEvent(Hazel::Event& e);

		void ClearScene(bool Depth = false);

		glm::vec3* GetBackGroundColor() { return &BackGroundColor; };

		uint32_t GetFrameBufferTextrure() { return  *FrameBuffertexture2D->GetID(); };

		uint32_t GetShadowMapTextrure() { return *ShadowMap->GetID(); };

		uint32_t GetPostProcessTextrure() { return *ExposedTexture2D->GetID(); };

		uint32_t GetFinalImageTextrure() { return *FinalImageTexture2D->GetID(); };

		const std::vector<Entity*>& GetEntities() { return Entities; };

		//const std::vector<PointLight*>& GetPointLights() { return PointLights; };

		CameraController* GetCameraController() { return &m_CameraController; };

		float& GetAmbientLighting() { return m_AmbientLighting; };

		int ShadowMapScale = 8;
		float ShadowBias = 0.001f;

		Hazel::Camera LightFrustum = Hazel::Camera(45, 1, 0.01f, 1000.f);

		glm::vec3 Light = glm::vec3(-2.f, 4.0f, -1);

		float near_plane = -40.f, far_plane = 40.f;

		float up = 40.f, down = -40.f, left = -40.f, right = 40.f;
		
	private:
		
		Hazel::ShaderLibrary m_ShaderLibrary;

		std::vector<Entity*> Entities;
		//std::vector<PointLight*> PointLights;

		uint32_t EntityIndex = 0;
		uint32_t PointLightIndex = 0;

		CameraController m_CameraController;

		float m_Width, m_Height;
		float m_AmbientLighting = 0.01f;

		Ref<Shader> m_shader;
		Ref<Shader> m_ShadowPassShader;
		Ref<Shader> m_PostProcessShader;
		Ref<Shader> m_CompositionShader;

		glm::vec3 BackGroundColor = glm::vec3(0.1f);

		uint32_t Buffer1;
		uint32_t Buffer2;
		uint32_t Buffer3;

		Ref<Texture2D> FrameBuffertexture2D;
		uint32_t FrameRenderBuffer;
		Ref<FrameBuffer> FrameBuffer;
		
		
		Ref<Texture2D> ExposedTexture2D;

		uint32_t HDRRenderBuffer;
		Ref<Hazel::FrameBuffer> HDRBuffer;

		
		Ref<Texture2D> BloomTexture2D;

		Ref<Hazel::FrameBuffer> CompositionBuffer;
		Ref<Texture2D> FinalImageTexture2D;
		

		float quadVertices[24] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		
		Ref<VertexArray> m_SquareVA;

		uint32_t DirectionalShadowMap;
		Ref<Texture2D> ShadowMap;
		Ref<Hazel::FrameBuffer> ShadowDepthBuffer;

		

		int  SHADOW_MAP_Width = 1024;
		int SHADOW_MAP_Height = 1024;

		float m_ViewPortSizeX;
		float m_ViewPortSizeY;

		

		glm::mat4 lightView = glm::lookAt(Light,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	};

}

