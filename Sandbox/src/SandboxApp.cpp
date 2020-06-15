#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"
#include "RenderLayer.h"
#include "GizmoOverlay.h"

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		auto m_Gizmo = new GizmoOverlay();
		
		//PushLayer();
		PushLayer(new RenderLayer(m_Gizmo));
		//PushLayer(new ExampleLayer());
		//PushLayer(new Sandbox2D());
		PushLayer(m_Gizmo);
	}

	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
