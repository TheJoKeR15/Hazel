#pragma once
#include "imgui/imgui.h"
#include "Hazel.h"
#include <glm/gtc/type_ptr.hpp>

void Transforms(Hazel::Entity* entt)
{
    if (ImGui::CollapsingHeader("Transforms"))
    {
        ImGui::Text("Position");        
        ImGui::SliderFloat("##X", &entt->position.x, -5.f, 5.f, "X = %.3f");
        ImGui::SliderFloat("##Y", &entt->position.y, -5.f, 5.f, "Y = %.3f");
        ImGui::SliderFloat("##Z", &entt->position.z, -5.f, 5.f, "Z = %.3f");
        ImGui::Separator();
        ImGui::Text("Rotation");
        ImGui::SliderAngle("##X2", &entt->rotation.x, -90.f, 90.f, "Pitch = %.3f");
        ImGui::SliderAngle("##Y2", &entt->rotation.y, -90.f, 90.f, "Yaw = %.3f");
        ImGui::SliderAngle("##Z2", &entt->rotation.z, -90.f, 90.f, "Roll = %.3f");
        ImGui::Separator();
        ImGui::Text("Scale");
        ImGui::SliderFloat("##X3", &entt->scale.x, -90.f, 90.f, "X = %.3f");
        ImGui::SliderFloat("##Y3", &entt->scale.y, -90.f, 90.f, "Y = %.3f");
        ImGui::SliderFloat("##Z3", &entt->scale.z, -90.f, 90.f, "Z = %.3f");
        ImGui::Separator();
        
    }
    
}

void Material(Hazel::Ref<Hazel::Material> Mat)
{
    ImGui::Text(("Material :" + Mat->materialName).c_str());
    if (Mat->m_Albedo) ImGui::Text(("Albedo texture :" + Mat->m_Albedo->m_path).c_str());
    if (Mat->m_Specular)ImGui::Text(("Spec texture :" + Mat->m_Specular->m_path).c_str());
    ImGui::SliderFloat("Specular Intensity :", &Mat->SpecularIntensity, 0, 1);
    ImGui::SliderFloat("Specular Exponent :", &Mat->SpecularExponent, 0, 256.f);
}

void ModelPropriety(Hazel::Model* entt)
{
    Transforms(entt);
    if (ImGui::CollapsingHeader("Meshes"))
    {
        for (int i = 0; i < entt->Getmeshes().size(); i++)
        {
            if (ImGui::TreeNodeEx((entt->Getmeshes()[i].m_name.c_str())))
            {
                Material(entt->Getmeshes()[i].m_Material);
                ImGui::TreePop();
            }

        }

    
    }
    ImGui::Checkbox("Visible", &entt->bVisible);
    ImGui::Checkbox("Cast Shadow", &entt->bCastShadow);
}

void LightPropriety(Hazel::Light* light)
{
    Transforms(light);

    if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::SliderFloat("Light Intensity", &light->Intensity, 0.f, 1000000.f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(light->LightColor));
        ImGui::Checkbox("Active", &light->bActive);
        ImGui::Checkbox("Cast Shadow", &light->bCastShadow);
        ImGui::Checkbox("Visible", &light->bVisible);
    }
    auto point = dynamic_cast<Hazel::PointLight*>(light);
    if (point)
    {
        if (ImGui::CollapsingHeader("Point Light"))
        {
            ImGui::Text((std::string("PointLightID = ")+ std::to_string(point->m_PointLightId)).c_str());
        }
    }
    auto Dir = dynamic_cast<Hazel::DirectionalLight*>(light);
    if (Dir)
    {
        /*
        if (ImGui::CollapsingHeader("Directional Light"))
        {
            
        }
        */
    }
    auto spot = dynamic_cast<Hazel::SpotLight*>(light);
    if (spot)
    {
        if (ImGui::CollapsingHeader("Spot Light"))
        {
            ImGui::SliderAngle("Inner Cuttof", &spot->CutoffInner,0.f,90.f);
            ImGui::SliderAngle("Outer Cuttof", &spot->CutoffOuter, spot->CutoffInner, 90.f);
        }
    }
}

void SkyBoxProperty(Hazel::Skybox* Sky)
{
    if (ImGui::CollapsingHeader("Skybox"))
    {
        ImGui::SliderFloat("Sky Intensity", &Sky->SkyIntensity, 0.f, 1000.f);
        ImGui::SliderFloat("Skybox Scale", &Sky->Sky_scale, 0.f, 10000.f);
        ImGui::Checkbox("Visible", &Sky->bVisible);
    }
    
}

void ShowPropertiesPanel(Hazel::Entity* SelectedEntity)
{
    if (SelectedEntity)
    {
        ImGui::Begin("Properties");
        // MODEL//
        auto model = dynamic_cast<Hazel::Model*>(SelectedEntity);
        auto Light = dynamic_cast<Hazel::Light*>(SelectedEntity);
        auto Sky = dynamic_cast<Hazel::Skybox*>(SelectedEntity);
        if (model)
        {
            ModelPropriety(model);
        }
        //LIGHT//
        else if (Light)
        {
            LightPropriety(Light);
        }
        //SKYBOX//      
        else if (Sky)
        {
            SkyBoxProperty(Sky);
        }
        // NORMAL ENTITY //          
        else
        {
            Transforms(SelectedEntity);
        }
        ImGui::End();

    }
}