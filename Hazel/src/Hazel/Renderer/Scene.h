#pragma once

//#include "Hazel/Core/Core.h"
//#include <assimp/Importer.hpp>	
#include "assimp/Importer.hpp"			// C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

#include "Hazel/Renderer/Entity.h"
#include "Hazel/Renderer/Lights/Light.h"
#include "CameraController.h"
#include <vector>

namespace Hazel
{
	class Scene
	{
	public:
		Scene(float Width, float Height, Hazel::Ref<Hazel::Shader> shader);
		
		Scene(float Width, float Height);

		~Scene();
		void InitializeScene();

		void BeginScene();

		void RenderScene(float dt);

		void EndScene();

		void AddEnitity(Entity* newEntinity);

		//void AddPointLight(PointLight* newEntinity);

		void AddDirectionalLight(Entity* newEntinity);

		void AddSpotLight(Entity* newEntinity);

		void RemoveEnitity(uint32_t ID);

		void HandleEvent(Hazel::Event& e);

		void ClearScene();

		glm::vec3* GetBackGroundColor() { return &BackGroundColor; };

		const std::vector<Entity*>& GetEntities() { return Entities; };

		//const std::vector<PointLight*>& GetPointLights() { return PointLights; };

		CameraController* GetCameraController() { return &m_CameraController; };

		float& GetAmbientLighting() { return m_AmbientLighting; };
		
	private:
		
		std::vector<Entity*> Entities;

		//std::vector<PointLight*> PointLights;

		uint32_t EntityIndex = 0;

		uint32_t PointLightIndex = 0;

		CameraController m_CameraController;

		float m_Width, m_Height;

		float m_AmbientLighting = 0.01f;

		Hazel::Ref<Hazel::Shader> m_shader;

		glm::vec3 BackGroundColor = glm::vec3(0.1f);

	};

}

