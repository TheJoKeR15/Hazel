#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	void Hazel::Scene::LightingPass()
	{
		
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		LightPassFBO->Bind();
		ClearScene();
		m_LightPassShader->Bind();

		ShadowMap->Bind(0);
		m_LightPassShader->SetInt("DirectionalShadowMap", 0);

		GBuffer->gBufferColor->Bind(1);
		m_LightPassShader->SetInt("gColor", 1);

		GBuffer->gBufferNormal->Bind(2);
		m_LightPassShader->SetInt("gNormal", 2);

		GBuffer->gBufferPosition->Bind(3);
		m_LightPassShader->SetInt("gPosition", 3);
		
		m_LightPassShader->SetFloat("DirLight.intensity", DirLight->bActive ? DirLight->Intensity : 0.f);
		m_LightPassShader->SetFloat3("DirLight.direction", DirLight->ForwardVector);

		m_LightPassShader->SetFloat3("CameraPosition", m_CameraController.GetCamera().GetPosition());

		/*
		m_LightPassShader->SetFloat((std::string("pointLights[") + std::to_string(m_PointLightId) + std::string("].intensity")).c_str(), bActive ? Intensity : 0.f);
		m_LightPassShader->SetFloat3((std::string("pointLights[") + std::to_string(m_PointLightId) + std::string("].position")), position);*/

		m_LightPassShader->SetFloat("AmbiantLight", GetAmbientLighting());

		LightPassFBO->Bind();
		m_SquareVA->Bind();

		Renderer::Submit(m_LightPassShader, m_SquareVA);

		LightPassFBO->Unbind();
	}
}