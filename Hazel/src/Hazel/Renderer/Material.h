#pragma once

#include "Hazel/Core/Core.h"
#include <glm/glm.hpp>

class Shader;

namespace Hazel {
	class Material
	{
	public:
		glm::vec3 BaseColor;
		float SpecularIntensity;
		float SpecularExponent;
		Shader* Shader;
	};
}
