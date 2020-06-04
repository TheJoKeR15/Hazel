#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"
#include <string>

#include "Shader.h"
#include "Hazel/Core/Core.h"

class Material;



namespace Hazel {
	class Entity
	{
	public:
		Entity();
		Entity(uint32_t ID = 0);

		Entity(std::string name, uint32_t ID = 0);

		// BASE FUNCTIONS //
		
		virtual void OnConstruct();

		virtual void OnInit();

		virtual void OnBeginFrame();

		virtual void OnUpdate(float dt);

		virtual void DrawShawdowPass(Ref<Shader> ShadowPassShader) = 0;

		virtual void DrawMainPass(Ref<Shader> MainPassShader) = 0;
		virtual void DrawMainPass() = 0;

		virtual void OnEndFrame();


		// should this entity be updated (every frame)
		bool bUpdate = true;

		// should this entity be Rendered (every frame)
		bool bVisible = true;

		// should this entity be Rendered (every frame)
		bool bIsSelected = false;

		// should this entity be Rendered in the shadow pass (every frame)
		bool bCastShadow = true;

		// should this entity be Rendered in the shadow pass (every frame)
		bool bRecieveShadow = true;

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

		glm::fmat4* GetTransform() { return &transform; };
		
		// Recalculate the transform of this entity from : position , scale 
		
		void RecalculateTransforms()
		{
			if (!bIsSelected)
			{
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation.r, glm::vec3(1, 0, 0))
				* glm::rotate(glm::mat4(1.0f), rotation.g, glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), rotation.b, glm::vec3(0, 0, 1))
				* glm::scale(glm::mat4(1.0f), scale);
			ForwardVector = WorldForwardVector * transform;

			UpVector = WorldUpVector * transform;

			RightVector = WorldRightVector * transform;
			}
		};
		void RecalculateTransforms(glm::mat4* newTransf)
		{
			transform = *newTransf;

			ForwardVector = transform * WorldForwardVector ;

			UpVector = transform * WorldUpVector;

			RightVector = transform * WorldRightVector;
		};

	
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.0f);

		glm::vec4 ForwardVector = glm::vec4(1.f, 0.f, 0.0f, 0.f);
				
		glm::vec4 UpVector = glm::vec4(0.f, 1.f, 0.0f, 0.f);
				
		glm::vec4 RightVector = glm::vec4(0.f, 1.f, 1.0f, 0.f);
				
		glm::vec4 WorldForwardVector = glm::vec4(1.f, 0.f, 0.0f, 0.f);
				
		glm::vec4 WorldUpVector = glm::vec4(0.f, 1.f, 0.0f, 0.f);
				
		glm::vec4 WorldRightVector = glm::vec4(0.f, 1.f, 1.0f,0.f);

		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.0f);

		glm::vec3 scale = glm::vec3(1.f);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1))
			* glm::scale (glm::mat4(1.0f), scale);

		Hazel::Ref<Hazel::Shader> m_Shader;

		Hazel::Ref<Hazel::Shader> m_ShadowShader;
	};

	
}

