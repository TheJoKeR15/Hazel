#pragma once

#include "Hazel.h"
#include <string>


namespace ImGuizmo {
enum OPERATION;
enum MODE;
};

class GizmoOverlay : public Hazel::Layer
{
public:
	GizmoOverlay();
	virtual ~GizmoOverlay() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

	glm::fmat4 DrawGizmo();

	void DrawPannels();

	void EditTransform(const float* cameraView, float* cameraProjection, float* matrix);

	float PosX = 0;
	float PosY = 0;
		  
	float SizeX	= 0;
	float SizeY	= 0;

	glm::fmat4 view;
	//float* m_matrix;
	glm::fmat4 projection;
	glm::fmat4 transform;

	glm::vec3 scale;
	glm::quat rotation;// = glm::quat(1, 0.2, 0, 0);
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	Hazel::Entity* SelectedEntity;

	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
	bool useSnap = false;
	float snap[3] = { 1.f, 1.f, 1.f };
	float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	float boundsSnap[3] = { 0.1f, 0.1f, 0.1f };
	bool boundSizing = false;
	bool boundSizingSnap = false;
};





