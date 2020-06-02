#pragma once
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class CameraController
	{
	public:
		CameraController(float aspectRatio, glm::vec3 Position);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }
		
		float GetFov() const { return m_FOV; }
		void SetFov(float FOV) { m_FOV = FOV; }

		void SetNewViewPortSize(float Width, float Height)
		{
			ViewPortWidth = Width; ViewPortHeight = Height; 
			m_Camera.SetProjection(m_FOV, ViewPortWidth / ViewPortHeight);
		};

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float ViewPortWidth;
		float ViewPortHeight;
		float m_AspectRatio;
		float m_ZoomLevel = 1.f;
		Camera m_Camera;

		float lastX;
		float lastY;

		//bool m_Rotation;
	public:
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
		float mf_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 1.f;
		float m_FOV = 45.f;
		bool m_firstMouseInput = true;
	};

}
