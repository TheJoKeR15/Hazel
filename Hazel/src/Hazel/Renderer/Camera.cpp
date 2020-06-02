#include "hzpch.h"
#include "Hazel/Renderer/Camera.h"
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	// TODO: Init camera projection on construct
	Camera::Camera(float FOV, float AspectRatio, float Near, float Far)
		: m_ViewMatrix(1.0f), m_ProjectionMatrix(glm::perspective(glm::radians(FOV), AspectRatio, Near, Far)) , NearClip(Near) , FarClip(Far)
	{
		HZ_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void Camera::SetProjection(float FOV, float AspectRatio)
	{
		HZ_PROFILE_FUNCTION();

		//m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ProjectionMatrix = glm::perspective(glm::radians(FOV),AspectRatio, NearClip, FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::ProcessMouseInput(float xoffset, float yoffset)
	{
		xoffset *= MouseSensitivity * 0.01f;
		yoffset *= MouseSensitivity * 0.01f;

		Yaw += std::min(89.f,std::max(-89.f,xoffset));
		Pitch += std::min(89.f, std::max(-89.f, yoffset));

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		

		// Update Front, Right and Up Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	void Camera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();
		/*
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), -Pitch,glm::vec3(1,0,0)) 
				* glm::rotate(glm::mat4(1.0f), -Yaw, glm::vec3(0, 1, 0))
					* glm::rotate(glm::mat4(1.0f), -Roll, glm::vec3(0, 0, 1));
			
		m_ViewMatrix = glm::inverse(transform);
		*/
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::UpdateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.z = - cos((Yaw)) * cos((Pitch));
		front.y = -sin((Pitch));
		front.x = sin((Yaw)) * cos((Pitch));

		m_CameraFront = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
		
	}

}