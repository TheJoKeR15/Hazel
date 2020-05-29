#include "hzpch.h"
#include "Hazel/Renderer/CameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	CameraController::CameraController(float aspectRatio, glm::vec3 Position)
		: m_AspectRatio(aspectRatio), m_Camera(45.f,aspectRatio,0.1f,1000.f) ,m_CameraPosition(Position)
	{
		m_Camera.SetPosition(m_CameraPosition);
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		
		//m_Camera.SetPosition(glm::vec3(0.f));

		m_Camera.SetProjection(m_FOV,m_AspectRatio, 0.1f,10000.f);

		// moce Left and Right
		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition -= m_CameraTranslationSpeed * ts * m_Camera.m_CameraRight;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{

			m_CameraPosition += m_CameraTranslationSpeed * ts * m_Camera.m_CameraRight;
		}

		// move Forward and backward
		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition += m_CameraTranslationSpeed * ts * m_Camera.m_CameraFront;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition -= m_CameraTranslationSpeed * ts * m_Camera.m_CameraFront;
		}

		// moce Up and Down
		if (Input::IsKeyPressed(HZ_KEY_Q))
		{
			m_CameraPosition -= m_CameraTranslationSpeed * ts * m_Camera.m_CameraUp;
		}
		else if (Input::IsKeyPressed(HZ_KEY_E))
		{

			m_CameraPosition += m_CameraTranslationSpeed * ts * m_Camera.m_CameraUp;
		}


		
		
		if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			if (m_firstMouseInput)
			{
				lastX = Input::GetMouseX();
				lastY = Input::GetMouseY();
				m_firstMouseInput = false;
			}

			float xoffset = Input::GetMouseX() - lastX;
			float yoffset = Input::GetMouseY() - lastY; // reversed since y-coordinates go from bottom to top

			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();

			m_Camera.ProcessMouseInput(xoffset, yoffset);
		}
		else
		{
			m_firstMouseInput = true;
		}

		m_Camera.SetPosition(m_CameraPosition);
		

		

		//m_CameraTranslationSpeed = m_ZoomLevel;
		/*
				if (m_Rotation)
				{
					if (Input::IsKeyPressed(HZ_KEY_Q))
						m_CameraRotation += m_CameraRotationSpeed * ts;
					if (Input::IsKeyPressed(HZ_KEY_E))
						m_CameraRotation -= m_CameraRotationSpeed * ts;

					if (m_CameraRotation > 180.0f)
						m_CameraRotation -= 360.0f;
					else if (m_CameraRotation <= -180.0f)
						m_CameraRotation += 360.0f;

					m_Camera.SetRotation(m_CameraRotation);
				}
				*/
		
		}
		
	
	void CameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		//m_ZoomLevel -= e.GetYOffset() * 1.f;
		//m_ZoomLevel = std::max(m_ZoomLevel, 90.f);
		//m_CameraPosition.z += e.GetYOffset() * .0001f;
		//m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		//m_Camera.SetFov(m_ZoomLevel, 1024.f, 720.f, 0.f, 1000.f);
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		//m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Camera.SetProjection(m_ZoomLevel, ViewPortWidth /ViewPortHeight, 0.1f, 1000.f);
		return false;
	}

}