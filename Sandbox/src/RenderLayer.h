#pragma once

#include "Hazel.h"

#include "Hazel/Core/Layer.h"
#include "imgui/imgui.h"

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

		void ShowSceneSettings();

		void ShowDebugPanel();

		void HelpMarker(const char* desc);

		void SetupMainMenuBar();

		void SetupMainDockSpace(bool* p_open = NULL);

		void SetupViewPort();



		float GetViewPortSizeX();

		float GetViewPortSizeY();

		void DrawGizmo();

		void EditTransform(const float* cameraView, float* cameraProjection, float* matrix);

		Hazel::Scene* RenderLayer::GetScene();


	private:

		Hazel::ShaderLibrary m_ShaderLibrary;

		Hazel::Ref<Hazel::Shader> MainShader;

		std::vector<Hazel::Ref<Hazel::Material>> ListOfMaterials;

		glm::vec3 lightpos = glm::vec3(0.f, 5.f, 0.f);

		//Hazel::CameraController m_CameraController;

		Hazel::Scene m_Scene;

		uint32_t Buffer ;
		uint32_t FrameBuffertexture;
		uint32_t FrameBufferDepthTexture;
		Hazel::Ref<Hazel::FrameBuffer> FrameBuffer;

		ImVec2 ViewPortSize;
		ImVec2 ViewPortPosition;


	};
