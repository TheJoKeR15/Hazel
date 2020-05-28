#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Material;



namespace Hazel {
	class Entity
	{
	public:
		Entity(uint32_t ID = 0);

		Entity(std::string name, uint32_t ID = 0);

		// BASE FUNCTIONS //
		
		virtual void OnConstruct();

		virtual void OnInit();

		virtual void OnBeginFrame();

		virtual void OnUpdate(float dt);

		virtual void OnEndFrame();


		// should this entity be updated (every frame)
		bool bUpdate = true;

		// should this entity be Rendered (every frame)
		bool bVisible = true;

		uint32_t UniqueID = 0;


		// the display name of this entity 
		// @TODO : Give each en,tity it's unique name (Entity ID ? ) based on a hash
		std::string displayName;

		// Set the position of this entity in 3D
		void SetPosition(glm::vec3 newPos)
		{
			position = newPos;
		};

		// Set the position of this entity in 3D
		void SetRotation(glm::vec3 newRot)
		{
			rotation = newRot;
		};

		// Set the position of this entity in 3D
		void SetScale(glm::vec3 newscl)
		{
			scale = newscl;
		};
		
		// Recalculate the transform of this entity from : position , scale 
		// @TODO : add Scale
		void RecalculateTransforms()
		{
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0))
				* glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
				* glm::scale(glm::mat4(1.0f), scale);
		};

	
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.0f);

		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.0f);

		glm::vec3 scale = glm::vec3(1.f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
			* glm::scale (glm::mat4(1.0f), scale);

	};

	
}

