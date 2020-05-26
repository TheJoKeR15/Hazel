#include "hzpch.h"
#include "Material.h"

Hazel::Material::Material(Hazel::Ref<Hazel::Shader> Shader, std::string Name) : m_Shader(Shader), materialName(Name)
{
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 32.f;
	SpecularIntensity = 1.f;
	bHasAlbedoTexture = false;
	bHasSpecularTexture = false;
;
}

Hazel::Material::Material(Hazel::Ref<Shader> Shader) : m_Shader(Shader) {
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 32.f;
	SpecularIntensity = 1.f;
	bHasAlbedoTexture = false;
	bHasSpecularTexture = false;
	materialName = "Unnamed Material";

};

Hazel::Material::Material(Hazel::Ref<Shader> Shader, Hazel::Ref<Hazel::Texture2D> Albedo, Hazel::Ref<Hazel::Texture2D> Spec)
	: m_Shader(Shader), m_Albedo(Albedo), m_Specular(Spec)
{
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 32.f;
	SpecularIntensity = 1.f;
	if (Albedo)		bHasAlbedoTexture = true;
	if (Spec) 		bHasSpecularTexture = true;
};

void Hazel::Material::Initialization()
{

}

void Hazel::Material::Update()
{

}
