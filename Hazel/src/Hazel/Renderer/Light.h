#pragma once
#include <Hazel.h>
#include "Hazel/Renderer/Entity.h"
#include "Hazel/Core/Core.h"


namespace Hazel {
	class Light : public Hazel::Entity
	{
	public:
		Light(Hazel::Ref<Hazel::Shader> shader);
		float Intensity = 1.f;
		float Radius = 5.f;
		glm::vec3 LightColor = glm::vec3(1.f);
		bool bActive = true;
		Hazel::Ref<Hazel::Shader> m_shader;
		Model* LightViz ;

		virtual void OnInit() override;

		virtual void OnBeginFrame() override;

		virtual void OnUpdate(float dt) override;

		virtual void OnEndFrame() override;

		void Draw();
		void DrawMesh()
		{
			LightViz->Draw(m_shader, glm::scale(transform, glm::vec3(0.1f)));
		}
	};
}

