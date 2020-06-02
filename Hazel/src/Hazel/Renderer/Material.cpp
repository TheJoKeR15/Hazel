#include "hzpch.h"
#include "Material.h"

Hazel::Material::Material(Hazel::Ref<Hazel::Shader> Shader, const std::string& Name) : m_Shader(Shader), materialName(Name)
{
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 128.f;
	SpecularIntensity = 1.f;
	bHasAlbedoTexture = false;
	bHasSpecularTexture = false;
	Initialization();
;
}

Hazel::Material::Material(Hazel::Ref<Shader> Shader) : m_Shader(Shader) {
	TintBaseColor = glm::vec3(1.f);
	SpecularExponent = 128.f;
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
	SpecularExponent = 128.f;
	SpecularIntensity = 1.f;
	if (Albedo)		bHasAlbedoTexture = true;
	if (Spec) 		bHasSpecularTexture = true;
	Initialization();
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

	m_Shader->SetFloat("SpecularStrenght", SpecularIntensity);
	m_Shader->SetFloat("SpecularExponent", SpecularExponent);
	m_Shader->SetFloat3("BasecolorTint", TintBaseColor);
	m_Shader->Bind();
}

void Hazel::Material::Update()
{
	if (m_Albedo)
	{
		m_Shader->SetBool("bHasAlbedoTexture", true);
		m_Albedo->ReBind();
		m_Shader->SetInt("t_BaseColor", *m_Albedo->GetSlot());
	}

	if (m_Specular)
	{
		m_Shader->SetBool("bHasSpeclarTexture", true);
		m_Specular->ReBind();
		m_Shader->SetInt("t_Specular", *m_Specular->GetSlot());
	}
	m_Shader->Bind();
	m_Shader->SetFloat("SpecularStrenght", SpecularIntensity);
	m_Shader->SetFloat("SpecularExponent", SpecularExponent);
	m_Shader->SetFloat3("BasecolorTint", TintBaseColor);
	m_Shader->Bind();
}
