#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Material;



namespace Hazel {
	class Entity
	{
	public:
		bool bDraw = true;

		Material* Material;

		std::string displayName;

		void SetPosition(glm::vec3 newPos)
		{
			position = newPos;
		};
		
		void RecalculateTransforms() {
		transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
	};

	glm::vec3 position = glm::vec3(0.f, 0.f, 0.0f);

	glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.0f);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
	};

	
}

