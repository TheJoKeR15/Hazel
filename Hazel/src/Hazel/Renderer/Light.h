#pragma once
#include <Hazel.h>
#include "Hazel/Renderer/Entity.h"
#include "Hazel/Core/Core.h"


namespace Hazel {
	class Light : public Hazel::Entity
	{
	public:
		Light();
		float Intensity = 1.f;
		float Radius = 5.f;
		glm::vec3 LightColor = glm::vec3(1.f);
		bool bActive = true;

		Model* LightViz ;

		void DrawVizualisationMesh(Ref<Hazel::Shader> shader)
		{
			RecalculateTransforms();
			LightViz->Draw(shader, glm::scale( transform,glm::vec3(0.1f)));
		};

		
	};
}

