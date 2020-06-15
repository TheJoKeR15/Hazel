#pragma once

#include <glm/glm.hpp>

namespace Hazel {

	class Camera
	{
	public:
		Camera(float FOV, float AspectRatio, float Near, float Far);

		void SetProjection(float FOV, float AspectRatio);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		glm::vec3 GetRotation() const { return Rotation; }
		void SetRotation(const glm::vec3& rotation) {
			Pitch = rotation.x; 
			Yaw = rotation.y;
			Roll = rotation.z;
			RecalculateViewMatrix(); 
			UpdateCameraVectors();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		const glm::mat4& GetViewProjectionMatrix() { 
			RecalculateViewMatrix();
			return m_ViewProjectionMatrix; 
		}
		
		void ProcessMouseInput(float xoffset, float yoffset);

	private:
		void RecalculateViewMatrix();
		void UpdateCameraVectors();
		
		
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	public:


		glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };

		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

		glm::vec3 m_CameraRight = { 1.0f, 0.0f, 0.0f };

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };

		float Pitch = 0.f;
		float Yaw = 0.f;
		float Roll = 0.f;

		float FarClip = 0.f;
		float NearClip = 0.f;

		float MouseSensitivity = 1.f;
	};

}
