#include "hzpch.h"
#include "Hazel/Renderer/Lights/Light.h"


namespace Hazel
{
	Light::Light(Hazel::Ref<Hazel::Shader> shader) : Entity(0)
	{
		m_shader = shader;
		LightViz = std::make_shared<Hazel::Model>("assets/models/Cube.obj", displayName, std::make_shared<Hazel::Material>(m_shader, "MainLightMaterial"), m_shader);
	}
	void Light::OnInit()
	{
	}

	void Light::OnBeginFrame()
	{
		//		RecalculateTransforms();
	}

	void Light::OnUpdate(float dt)
	{
		

		RecalculateTransforms();

		if (bVisible)
		{
			DrawMesh();
		}
		
		Entity::OnUpdate(dt);
	}

	void Light::OnEndFrame()
	{
	}


	void PointLight::OnInit()
	{
		Light::OnInit();
	}

	void PointLight::OnUpdate(float dt)
	{
		m_shader->SetFloat((std::string("pointLights[") + std::to_string(m_PointLightId) + std::string("].intensity")).c_str(), Intensity);
		m_shader->SetFloat3((std::string("pointLights[") + std::to_string(m_PointLightId) + std::string("].position")), position);
		Light::OnUpdate(dt);
	}

	void PointLight::OnEndFrame()
	{
		Light::OnEndFrame();
	}

	void DirectionalLight::OnInit()
	{
		Light::OnInit();
	}

	void DirectionalLight::OnUpdate(float dt)
	{
		m_shader->SetFloat("DirLight.intensity", Intensity);
		m_shader->SetFloat3("DirLight.direction", ForwardVector);
		Light::OnUpdate(dt);
	}

	void DirectionalLight::OnEndFrame()
	{
		Light::OnEndFrame();
	}

	void SpotLight::OnInit()
	{
		Light::OnInit();
	}

	void SpotLight::OnUpdate(float dt)
	{
		
		m_shader->SetFloat((std::string("spotLights[") + std::to_string(m_SpotLightId) + std::string("].intensity")).c_str(), Intensity);
		m_shader->SetFloat((std::string("spotLights[") + std::to_string(m_SpotLightId) + std::string("].Cutoff_In")).c_str(), CutoffInner);
		m_shader->SetFloat((std::string("spotLights[") + std::to_string(m_SpotLightId) + std::string("].Cutoff_Out")).c_str(), CutoffOuter);
		m_shader->SetFloat3((std::string("spotLights[") + std::to_string(m_SpotLightId) + std::string("].position")), position);
		m_shader->SetFloat3((std::string("spotLights[") + std::to_string(m_SpotLightId) + std::string("].direction")), ForwardVector);
		Light::OnUpdate(dt);
		
	}

	void SpotLight::OnEndFrame()
	{
		Light::OnEndFrame();
	}

}