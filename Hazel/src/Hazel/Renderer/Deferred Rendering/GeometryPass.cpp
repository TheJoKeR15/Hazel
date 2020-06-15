#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"


namespace Hazel
{
	void Scene::GeometryPass()
	{
		// Setup the scene
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		GBuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
		RenderCommand::Clear();
		
		m_GeometryPassShader->Bind();
		m_GeometryPassShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
		// Update the camera first 
		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
		// Loop^in the list of all the entitiy and render the one flaged to 
		bool firsttime = true;
		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bUpdate & EnTT->bVisible)
			{
				// Draw
				EnTT->DrawGeometryPass(m_GeometryPassShader);
			}
		}
		GBuffer->Unbind();
	}

}