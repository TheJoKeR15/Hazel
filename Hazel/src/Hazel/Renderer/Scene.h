#pragma once

//#include "Hazel/Core/Core.h"
//#include <assimp/Importer.hpp>	
#include "assimp/Importer.hpp"			// C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

#include "Hazel/Renderer/Entity.h"
#include "CameraController.h"
#include <vector>

namespace Hazel {
	class Scene
	{
	public:
		Scene(float Width, float Height);
		

		void InitializeScene();

		void BeginScene();

		void RenderScene(float dt);

		void EndScene();

		void AddEnitity(Entity* newEntinity);

		void RemoveEnitity(uint32_t ID);

		void HandleEvent(Hazel::Event& e);

		glm::vec3* GetBackGroundColor() { return &BackGroundColor; };

		std::vector<Entity*>& GetEntities() { return Entities; };

		CameraController* GetCameraController() { return &m_CameraController; };
		
	private:
		
		std::vector<Entity*> Entities;

		uint32_t EntityIndex = 0;

		CameraController m_CameraController;

		float m_Width, m_Height;

		glm::vec3 BackGroundColor = glm::vec3(0.1f);

	};

};

