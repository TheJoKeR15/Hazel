#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	void Scene::RenderPostPass()
	{

		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		HDRBuffer->Bind();
		ClearScene();
		m_PostProcessShader->Bind();
		LightingPassTexture2D->Bind(0);
		m_PostProcessShader->SetInt("HDRImage", *LightingPassTexture2D->GetSlot());

		m_PostProcessShader->SetFloat("ExposureCompensation", m_CameraController.ExposureComp);
		m_PostProcessShader->SetFloat("Aperture", m_CameraController.aperature);
		m_PostProcessShader->SetFloat("ShutterSpeed", 1.f / m_CameraController.ShutterSpeed);
		m_PostProcessShader->SetFloat("Iso", m_CameraController.Iso);

		m_PostProcessShader->SetFloat("BloomTreshold", m_CameraController.BloomTreshold);
		HDRBuffer->Bind();

		m_SquareVA->Bind();

		Renderer::Submit(m_PostProcessShader, m_SquareVA);

		HDRBuffer->Unbind();
	}
}