#pragma once
#include <Hazel.h>
#include "Hazel/Renderer/Entity.h"

#include "Hazel/Renderer/Model.h"
#include "Hazel/Core/Core.h"


namespace Hazel
{
	class Skybox : public Entity
	{
	public:
		Skybox(Ref<Shader> shader, Ref<TextureCube> CubeMap);

		virtual void OnInit() override;

		virtual void OnBeginFrame() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;

		virtual void DrawShawdowPass(Ref<Shader> ShadowPassShader) override;

		virtual void DrawMainPass(Ref<Shader> MainPassShader) override {};
		
		virtual void DrawMainPass() override;

		Ref<Shader> m_shader;

		Ref<Model> Mesh;

		Ref<TextureCube> TextureCubeMap;

		float Sky_scale = 600.f;

		float SkyIntensity = 1.0f;

		void DrawMesh()
		{
			
			Mesh->Draw(m_shader, glm::scale(transform, glm::vec3(Sky_scale)));
		}
	};
}