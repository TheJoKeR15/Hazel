#include "GizmoOverlay.h"
#include "RenderLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>


#include "ImGuizmo/ImGuizmo.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#endif // !GLM_ENABLE_EXPERIMENTAL


GizmoOverlay::GizmoOverlay()  : Layer("Guizmo Layer")
{
    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    mCurrentGizmoMode=ImGuizmo::LOCAL;
}

void GizmoOverlay::OnAttach()
{
}

void GizmoOverlay::OnDetach()
{
}

void GizmoOverlay::OnUpdate(Hazel::Timestep ts)
{
    
}

void GizmoOverlay::OnImGuiRender()
{
    //DrawGizmo();
}

void GizmoOverlay::OnEvent(Hazel::Event& e)
{
}

glm::fmat4 GizmoOverlay::DrawGizmo()
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
    
    transform = *SelectedEntity->GetTransform();
    EditTransform(&view[0][0], &projection[0][0], &transform[0][0]);
    glm::decompose(transform, scale, rotation, translation, skew, perspective);

    SelectedEntity->SetPosition(translation);
    //ImGui::SliderFloat3("SizeX", (float*)glm::value_ptr(), -100, 100);
    SelectedEntity->SetRotation(glm::eulerAngles(glm::conjugate(rotation)));
    SelectedEntity->SetScale(scale);
    SelectedEntity->RecalculateTransforms(&transform);

    return transform;
}


void GizmoOverlay::DrawPannels()
{
    ImGui::Begin("Gizmo Settings");
    //editTransformDecomposition
    if (true)
    {
        /*
        if (ImGui::IsKeyPressed(90))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(69))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(82)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        */
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        /*
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(&transform[0][0], matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation, 3);
        ImGui::InputFloat3("Rt", matrixRotation, 3);
        ImGui::InputFloat3("Sc", matrixScale, 3);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &transform[0][0]);
        */
        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(83))
            useSnap = !useSnap;
        ImGui::Checkbox("", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }
    ImGui::End();
}

void GizmoOverlay::EditTransform(const float* cameraView, float* cameraProjection, float* matrix)
{
    //m_matrix = matrix;
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(PosX, PosY, SizeX, SizeY);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    

}

