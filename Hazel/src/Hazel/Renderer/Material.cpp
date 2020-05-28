#include "hzpch.h"
#include "Material.h"

Hazel::Material::Material(Hazel::Ref<Hazel::Shader> Shader, std::string Name) : m_Shader(Shader), materialName(Name)
{
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 32.f;
	SpecularIntensity = 1.f;
	bHasAlbedoTexture = false;
	bHasSpecularTexture = false;
	Initialization();
;
}

Hazel::Material::Material(Hazel::Ref<Shader> Shader) : m_Shader(Shader) {
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 32.f;
	SpecularIntensity = 1.f;
	bHasAlbedoTexture = false;
	bHasSpecularTexture = false;
	materialName = "Unnamed Material";
	Initialization();

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
	m_Shader->Bind();
	if (bHasAlbedoTexture)
	{
		m_Albedo->Bind(m_Shader->GetNextTextureSlotIndex());
		m_Shader->SetInt("t_BaseColor", *m_Albedo->GetSlot());
	}

	if (bHasSpecularTexture)
	{
		m_Specular->Bind(m_Shader->GetNextTextureSlotIndex());
		m_Shader->SetInt("t_Specular", *m_Specular->GetSlot());
	}

	m_Shader->SetInt("SpecularStrenght", SpecularExponent);
	m_Shader->SetInt("SpecularExponent", SpecularExponent);
}

void Hazel::Material::Update()
{
	if (m_Albedo)
	{
		m_Albedo->ReBind();
		m_Shader->SetInt("t_BaseColor", *m_Albedo->GetSlot());
	}

	if (m_Specular)
	{

		m_Specular->ReBind();
		m_Shader->SetInt("t_Specular", *m_Specular->GetSlot());
	}
	m_Shader->Bind();
}
