#pragma once

#include "Hazel/Core/Core.h"
#include <glm/glm.hpp>
#include "Hazel.h"

class Shader;

namespace Hazel {
	struct Material
	{
	public:
		Material() {

		};

		~Material() = default;

		void Initialization();

		void Update();


		// Base Parameters
		glm::vec3 TintBaseColor;
		float SpecularIntensity;
		float SpecularExponent;

		// TEXTURES
		Hazel::Ref<Hazel::Texture2D> Albedo;
		Hazel::Ref<Hazel::Texture2D> Specular;
		//Hazel::Ref<Hazel::Texture2D> Normal;

		// BOOLS
		bool bHasAlbedoTexture;
		bool bHasSpecularTexture;

	private:
		Shader* Shader;
	};
}
