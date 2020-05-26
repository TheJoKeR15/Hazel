#pragma once

#include "Hazel.h"

#include "Hazel/Core/Layer.h"

	class RenderLayer : public Hazel::Layer
	{
	public:
		RenderLayer();
		virtual ~RenderLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Hazel::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Hazel::Event& e) override;

		void ShowOutliner();

		void ShowCameraTab();

		void HelpMarker(const char* desc);

		void SetupMainMenuBar();

		void SetupMainDockSpace(bool* p_open = NULL);

		void DrawGizmo();

		void EditTransform(const float* cameraView, float* cameraProjection, float* matrix);

		Hazel::Scene* RenderLayer::GetScene();


	private:

		Hazel::ShaderLibrary m_ShaderLibrary;

		Hazel::Ref<Hazel::Shader> MainShader;

		std::vector<Hazel::Ref<Hazel::Material>> ListOfMaterials;

		//Hazel::CameraController m_CameraController;

		Hazel::Scene m_Scene;
	};
