#pragma once

#include "Hazel/Core/Core.h"
#include <glm/glm.hpp>
//#include "Hazel.h"
#include "Texture.h"
#include "Shader.h"


namespace Hazel {
	struct Material
	{
	public:
		Material(Hazel::Ref <Hazel::Shader> Shader,std::string Name);

		Material(Hazel::Ref <Hazel::Shader> Shader);

		Material(Hazel::Ref <Hazel::Shader> Shader, Hazel::Ref<Hazel::Texture2D> Albedo, Hazel::Ref<Hazel::Texture2D> Spec);


		~Material() = default;

		void Initialization();

		void Update();


		Hazel::Ref <Hazel::Shader> GetShader() { return m_Shader; };

		std::string materialName;

		// Base Parameters
		glm::vec3 TintBaseColor;
		float SpecularIntensity;
		float SpecularExponent;

		// TEXTURES
		Hazel::Ref<Hazel::Texture2D> m_Albedo;
		Hazel::Ref<Hazel::Texture2D> m_Specular;
		//Hazel::Ref<Hazel::Texture2D> Normal;

		// BOOLS
		bool bHasAlbedoTexture = false;
		bool bHasSpecularTexture = false;

	private:
		Hazel::Ref <Hazel::Shader> m_Shader;
	};
}
