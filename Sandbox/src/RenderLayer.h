#pragma once

#include "Hazel.h"

#include "Hazel/Core/Layer.h"

namespace Hazel{}
class RenderLayer : public Hazel::Layer
{
public:
	RenderLayer();
	virtual ~RenderLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event & e) override;

	void ShowOutliner();

	void ShowCameraTab();

	void HelpMarker();

	void SetupMainMenuBar();

	void SetupMainDockSpace();


private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::CameraController m_CameraController;
};

