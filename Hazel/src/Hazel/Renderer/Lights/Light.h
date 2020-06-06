#pragma once

#include <Hazel.h>
#include "Hazel/Renderer/Model.h"
#include "Hazel/Core/Core.h"


namespace Hazel
{
	class Light : public Hazel::Entity
	{
	public:
		Light(Hazel::Ref<Hazel::Shader> shader);

		virtual void OnInit() override;

		virtual void OnBeginFrame() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;

		virtual void DrawShawdowPass(Ref<Shader> ShadowPassShader) override;

		virtual void DrawMainPass(Ref<Shader> MainPassShader) override;
		
		virtual void DrawMainPass() override;

		bool bActive = true;

		float Intensity = 1.f;

		float Radius = 5.f;
		glm::vec3 LightColor = glm::vec3(1.f);
		

		Hazel::Ref<Hazel::Shader> m_shader;

		Hazel::Ref<Hazel::Model> LightViz;

		void DrawMesh()
		{
			LightViz->Draw(m_shader, glm::scale(transform, glm::vec3(0.1f)));
		}
	};



	class PointLight : public Light
	{
	public:
		PointLight(Hazel::Ref<Hazel::Shader> shader, uint32_t id) : Light(shader), m_PointLightId(id)
		{
			displayName = std::string("Point Light ") + std::to_string(id);
		};

		virtual void OnInit() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;

		uint32_t m_PointLightId;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(Hazel::Ref<Hazel::Shader> shader) : Light(shader)
		{
			displayName = std::string("Directional Light ") + std::to_string(UniqueID);
		};

		virtual void OnInit() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;

	};

	class SpotLight : public Light
	{
	public:
		SpotLight(Hazel::Ref<Hazel::Shader> shader, uint32_t id) : Light(shader), m_SpotLightId(id)
		{
			displayName = std::string("Spot Light ") + std::to_string(id);
		};

		virtual void OnInit() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;
		float CutoffOuter = glm::cos(glm::radians(30.f));

		float CutoffInner = glm::cos(glm::radians(10.f));

		uint32_t m_SpotLightId;
	};
}
	
