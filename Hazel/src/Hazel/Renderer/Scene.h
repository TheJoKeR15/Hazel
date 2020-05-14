#pragma once

//#include "Hazel/Core/Core.h"
//#include <assimp/Importer.hpp>	
#include "assimp/Importer.hpp"			// C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

#include "Hazel/Renderer/Entity.h"
#include <vector>

namespace Hazel {
	class Scene
	{
	public:
		Scene();
		

		void Initialize();

		void RenderScene();

		void AddEnitity(Entity* newEntinity);

		std::vector<Entity*> Entities;
	};
};

