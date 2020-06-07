#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	void Scene::RenderMainPass()
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		MainFrameBuffer->Bind();
		ClearScene();
		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
		// Update the camera first 

		// Loop^in the list of all the entitiy and render the one flaged to 
		bool firsttime = true;
		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bUpdate & EnTT->bVisible)
			{
				if (EnTT->bRecieveShadow)
				{
					ShadowMap->Bind(3);
					EnTT->m_Shader->SetInt("DirectionalShadowMap", 3);
					EnTT->m_Shader->SetMat4("lightSpaceMatrix", lightProjection * lightView);
					EnTT->m_Shader->SetFloat("ShadowBias", ShadowBias);
					//EnTT->m_Shader->SetFloat3("TEXELSIZE", glm::vec3(1.f/ (SHADOW_MAP_Width * ShadowMapScale), 1.f/(SHADOW_MAP_Height * ShadowMapScale),0.0f));
				}
				// trigger on update event
				EnTT->DrawMainPass();
			}
		}
		MainFrameBuffer->Unbind();
	}
}