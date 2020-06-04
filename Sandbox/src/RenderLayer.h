#pragma once

#include "Hazel.h"


#include "Hazel/Core/Layer.h"
#include "imgui/imgui.h"
#include "GizmoOverlay.h"



	class RenderLayer : public Hazel::Layer
	{
	public:
		RenderLayer(GizmoOverlay* GizmoLayer);
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

		void SelectEntity(Hazel::Entity* newSelectedEntity);

		float GetViewPortSizeX();

		float GetViewPortSizeY();

		void FocusToSelected();

		Hazel::Scene* RenderLayer::GetScene() {
			return &m_Scene;
		};


	private:

		Hazel::ShaderLibrary m_ShaderLibrary;

		Hazel::Ref<Hazel::Shader> MainShader;

		Hazel::Ref<Hazel::Shader> SkyShader;

		Hazel::Ref<Hazel::Shader> DirectionalShadowMapShader;

		Hazel::Light* m_Light;

		std::vector<Hazel::Ref<Hazel::Material>> ListOfMaterials;

		glm::vec3 lightpos = glm::vec3(0.f, 5.f, 0.f);

		Hazel::Entity* SelectedEntity = nullptr;

		GizmoOverlay* m_GizmoLayer;

		//Hazel::CameraController m_CameraController;

		Hazel::Scene m_Scene;

		uint32_t Buffer ;
		uint32_t FrameBuffertexture;
		uint32_t FrameBufferDepthTexture;
		Hazel::Ref<Hazel::FrameBuffer> FrameBuffer;

		uint32_t DirectionalShadowMap;

		Hazel::Ref<Hazel::FrameBuffer> DepthFrameBuffer;

		unsigned int ShadowMapScale = 1;

		ImVec2 ViewPortSize;
		ImVec2 ViewPortPosition;

		int lastres = 1;



	};
