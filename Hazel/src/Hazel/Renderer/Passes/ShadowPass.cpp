#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	void Scene::RenderShadowPass()
	{

		ShadowDepthBuffer->Bind();
		m_ShadowPassShader->Bind();

		RenderCommand::SetViewport(0, 0, SHADOW_MAP_Width * ShadowMapScale, SHADOW_MAP_Height * ShadowMapScale);
		m_ShadowPassShader->SetMat4("u_ViewProjection", lightProjection * lightView);

		ClearScene(true);
		for (int i = 0; i < Entities.size(); i++)
		{
			auto EnTT = Entities[i];
			if (EnTT->bCastShadow && EnTT->bUpdate)
			{
				// trigger on update event

				//m_ShadowPassShader->SetMat4("model", EnTT->transform);

				EnTT->DrawShawdowPass(m_ShadowPassShader);
			}
		}
		ShadowDepthBuffer->Bind();
	}
}