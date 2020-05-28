#include "RenderLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>



#include "ImGuizmo/ImGuizmo.h"

RenderLayer::RenderLayer(GizmoOverlay* GizmoLayer) :Layer("Render Layer"), m_Scene(Hazel::Scene(1280.0f, 720.0f)), m_GizmoLayer(GizmoLayer)
    {

        // Init
        // setting more pleasent settings for the camera

        Buffer = 0;
        FrameBuffer = Hazel::FrameBuffer::Create(Buffer,1280,720);

        
    }

    void RenderLayer::OnAttach()
    {

        MainShader = m_ShaderLibrary.Load("assets/shaders/MainShader.glsl");

        

        ListOfMaterials.push_back(std::make_shared<Hazel::Material>(MainShader, "MainMaterial"));

        auto text = Hazel::Texture2D::Create("assets/textures/Check.png");
        auto text2 = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        //auto text2 = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        auto NewMat = std::make_shared<Hazel::Material>(MainShader, text, text2);
        ListOfMaterials.push_back(NewMat);



        GetScene()->AddEnitity(new Hazel::Model("assets/models/Suzane.obj","Suzane", ListOfMaterials[1],MainShader));

        //GetScene()->AddEnitity(new Hazel::Model("assets/Sponza/sponza.obj", "Sponza", ListOfMaterials[1],MainShader));

        GetScene()->AddEnitity(new Hazel::Model("assets/nanosuit/nanosuit.obj", "NanoSuite", ListOfMaterials[1], MainShader));

        GetScene()->GetEntities()[1]->position = glm::vec3(8.f,0.f,3.f);

        GetScene()->AddEnitity(new Hazel::Model("assets/rock/rock.obj", "Rock", ListOfMaterials[1], MainShader));
        
        GetScene()->GetEntities()[2]->position = glm::vec3(-5.f, 0.f, 3.f);

        GetScene()->AddEnitity(new Hazel::Light(MainShader));

        
         
        GetScene()->GetEntities()[3]->position = glm::vec3(-5.f, 10.f, 3.f);

        m_Light = dynamic_cast<Hazel::Light*>(GetScene()->GetEntities()[3]);

        if (GetScene()->GetEntities().size() > 0)
        {
            SelectEntity(GetScene()->GetEntities()[0]);
        }

        m_Scene.GetCameraController()->GetCamera().SetPosition(glm::vec3(-100.f, 50, 0.f));
        m_Scene.GetCameraController()->GetCamera().SetRotation(glm::vec3(-0.f, 90.f, 0.f));
        m_Scene.GetCameraController()->m_FOV = 70;
        m_Scene.GetCameraController()->m_CameraTranslationSpeed = 50.f;

        GetScene()->InitializeScene();
    }

    void RenderLayer::OnDetach()
    {
        FrameBuffer->FreeBuffer();
    }


    void RenderLayer::OnUpdate(Hazel::Timestep ts)
    {
        //Bind the ViewportFrameBuffer
        FrameBuffer->Bind();

        FrameBuffertexture = FrameBuffer->AttachColorTexture2D(ViewPortSize.x, ViewPortSize.y ,ViewPortPosition.x,ViewPortPosition.y);
        // This Doesnt Work For the moment 
        FrameBufferDepthTexture = FrameBuffer->AttachDepthTexture2D(ViewPortSize.x, ViewPortSize.y);

        MainShader->Bind();
        

        MainShader->SetFloat("u_LightIntensity", m_Light->Intensity);
        MainShader->SetFloat("u_LightRadius", m_Light->Radius);
        MainShader->SetFloat("u_AmbiantLight", 0.05f);
        MainShader->SetFloat3("u_LightPosition", m_Light->position);
        MainShader->SetFloat3("u_LightColor", m_Light->LightColor);
        MainShader->SetFloat3("CameraPosition", GetScene()->GetCameraController()->GetCamera().GetPosition());


        MainShader->Bind();
        // Begin the scene FRAME START
        GetScene()->BeginScene();

        // Render the Scene
        GetScene()->RenderScene(ts);

        // End the Scene
        GetScene()->EndScene();

        // Unbind the Framebuffer to let ImGui render
        FrameBuffer->Unbind();

        m_GizmoLayer->SelectedEntity = SelectedEntity;
        //m_GizmoLayer->matrix = SelectedEntity->transform;
        m_GizmoLayer->view = GetScene()->GetCameraController()->GetCamera().GetViewMatrix();
        m_GizmoLayer->projection = GetScene()->GetCameraController()->GetCamera().GetProjectionMatrix();

        //GetScene()->ClearScene();
    }

    void RenderLayer::OnImGuiRender()
    {
        
        m_GizmoLayer->SizeX = ViewPortSize.x;
        m_GizmoLayer->SizeY = ViewPortSize.y;

        m_GizmoLayer->PosX = ViewPortPosition.x;
        m_GizmoLayer->PosY = ViewPortPosition.y;

        ImGui::Begin("Gizmo Debug");
        ImGui::SliderFloat("PosX", &m_GizmoLayer->PosX, -100, 100);
        ImGui::SliderFloat("PosY", &m_GizmoLayer->PosY, -100, 100);

        ImGui::SliderFloat("SizeX", &m_GizmoLayer->SizeX, -100, 100);
        ImGui::SliderFloat("SizeY", &m_GizmoLayer->SizeY, -100, 100);
        ImGui::End();

        m_GizmoLayer->DrawGizmo();


        // Setup the main docking space 
        SetupMainDockSpace();

        m_GizmoLayer->DrawPannels();
        
        // setting up the viewport (frame buffer and cie)
        SetupViewPort();

        // Creatte the outliner 
        ShowOutliner();

        // Add the camera properties tab
        ShowCameraTab();

        // add the scene settings tab
        ShowSceneSettings();

        // add the debug Pannel
        ShowDebugPanel();

        // Temporary
        ImGui::ShowDemoWindow();
        // Gizmo
        //DrawGizmo();
    }

    void RenderLayer::OnEvent(Hazel::Event& e)
    {
        GetScene()->GetCameraController()->OnEvent(e);
    }

    float RenderLayer::GetViewPortSizeX()
    {
        return ViewPortSize[0];
    }

    float RenderLayer::GetViewPortSizeY()
    {
        return ViewPortSize[1];
    }



    void RenderLayer::SetupMainDockSpace(bool* p_open)
    {

        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None & ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
            ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::SmallButton("click here"))
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }

        SetupMainMenuBar();

        /*if (ImGui::BeginMenuBar())
        {

            if (ImGui::BeginMenu("Docking"))
             {
                 // Disabling fullscreen would allow the window to be moved to the front of other windows,
                 // which we can't undo at the moment without finer window depth/z control.
                 //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                 if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                 if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                 if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                 if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                 if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                 ImGui::Separator();
                 if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
                     *p_open = false;
                 ImGui::EndMenu();
             }






            ImGui::EndMenuBar();*/

        ImGui::End();
    }

    void RenderLayer::SetupViewPort()
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport",NULL,flags);

        ViewPortSize = ImGui::GetWindowSize();
        ViewPortPosition = ImGui::GetWindowPos();

        ImGui::Image((void*)(intptr_t)FrameBuffertexture, ViewPortSize,ImVec2(0,1), ImVec2(1, 0));
        GetScene()->GetCameraController()->SetNewViewPortSize(ViewPortSize.x, ViewPortSize.y);

        ImGui::End();

        ImGui::PopStyleVar();
    }

    void RenderLayer::SelectEntity(Hazel::Entity* newSelectedEntity)
    {
        if (SelectedEntity)
        {
            SelectedEntity->bIsSelected = false;
        }

        SelectedEntity = newSelectedEntity;
        SelectedEntity->bIsSelected = true;
    }

    void RenderLayer::ShowSceneSettings()
    {
        ImGui::Begin("Scene Settings");
        ImGui::ColorEdit3("Background Color ", glm::value_ptr(*GetScene()->GetBackGroundColor()));
        ImGui::End();
    }

    void RenderLayer::ShowDebugPanel()
    {
        ImGui::Begin("Debug");

        ImGui::Text(("Currently selected = " + SelectedEntity->displayName).c_str());
        ImGui::Text("Frame Buffer pointer = %p", FrameBuffertexture);
        ImGui::Text("ViewPort size = %f x %f", ViewPortSize.x, ViewPortSize.y);
        ImGui::Text("ViewPort Position = %f x %f", ViewPortPosition.x, ViewPortPosition.y);
        char name[16] = { "New Entity" };
        ImGui::InputText("Name :", name, IM_ARRAYSIZE(name));ImGui::SameLine();
        if (ImGui::Button("Add Entity"))
        {

            GetScene()->AddEnitity(new Hazel::Model("assets/models/Cube.obj" ,name, ListOfMaterials[0],MainShader));
        }
        if (ImGui::Button("Add Sponza"))
        {
            GetScene()->AddEnitity(new Hazel::Model("assets/Sponza/sponza.obj","Sponza", ListOfMaterials[1],  MainShader));

            auto n = GetScene()->GetEntities().size();
            GetScene()->GetEntities()[n]->scale = glm::vec3(0.1f);
        }
        for (int i = 0; i < ListOfMaterials.size(); i++)
        {
            std::string strng = std::string("Material ") + std::to_string(i);
            if (ImGui::TreeNode(strng.c_str()))
            {
                ImGui::Text(ListOfMaterials[i]->materialName.c_str());
                if (ListOfMaterials[i]->bHasAlbedoTexture)
                {
                    ImGui::Text("Albedo pointer = %p", ListOfMaterials[i]->m_Albedo);

                }
                if (ListOfMaterials[i]->bHasSpecularTexture)
                {
                    ImGui::Text("Specullar pointer = %p", ListOfMaterials[i]->m_Specular);
                }
                ImGui::TreePop();
            }
        }
        //ImGui::Text("Currently Selected : "); ImGui::SameLine(); ImGui::Text(SelectedEntity->displayName.c_str());
        ImGui::End();
    }

    void RenderLayer::SetupMainMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Text("Test lolo");
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }

            HelpMarker(
                "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n\n"
                " > if io.ConfigDockingWithShift==false (default):" "\n"
                "   drag windows from title bar to dock" "\n"
                " > if io.ConfigDockingWithShift==true:" "\n"
                "   drag windows from anywhere and hold Shift to dock" "\n\n"
                "This demo app has nothing to do with it!" "\n\n"
                "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
                "ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
                "(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
            );

        }
        ImGui::EndMenuBar();
    }

    void RenderLayer::ShowOutliner()
    {
        ImGui::Begin("Outliner");
        if (ImGui::TreeNode("Light"))
        {
            ImGui::ColorEdit3("Color", glm::value_ptr(m_Light->LightColor));
            ImGui::SliderFloat("Intensity", &m_Light->Intensity, -5.f, 5.f);
            if (ImGui::TreeNode("Position"))
            {
                ImGui::SliderFloat("x", &m_Light->position.x, -50.f, 50.f);
                ImGui::SliderFloat("y", &m_Light->position.y, -50.f, 50.f);
                ImGui::SliderFloat("z", &m_Light->position.z, -50.f, 50.f);
                ImGui::TreePop();

            }
            if (ImGui::TreeNode("Advanced"))
            {
                ImGui::Checkbox("Visible ", &m_Light->bVisible);
                ImGui::Checkbox("Active ", &m_Light->bActive);
                ImGui::TreePop();
            }
            ImGui::TreePop();

            
        }
        for (int i = 0; i < GetScene()->GetEntities().size(); i++)
        {
            auto entt = GetScene()->GetEntities()[i];
            std::string name = GetScene()->GetEntities()[i]->displayName;
            if (ImGui::TreeNode(name.c_str()))
            {
                const char* list[] = { "Main Shader" };
                int id;
                //ImGui::Combo("Shader", &id, list, IM_ARRAYSIZE(list));
                if (ImGui::TreeNode("Position"))
                {
                    ImGui::SliderFloat("##X", &entt->position.x, -5.f, 5.f, "X = %.3f");
                    ImGui::SliderFloat("##Y", &entt->position.y, -5.f, 5.f, "Y = %.3f");
                    ImGui::SliderFloat("##Z", &entt->position.z, -5.f, 5.f, "Z = %.3f");
                    ImGui::TreePop();
                }
                ImGui::Separator();
                if (ImGui::TreeNodeEx("Rotation"))
                {
                    ImGui::SliderAngle("##X2", &entt->rotation.x, -90.f, 90.f, "Pitch = %.3f");
                    ImGui::SliderAngle("##Y2", &entt->rotation.y, -90.f, 90.f, "Yaw = %.3f");
                    ImGui::SliderAngle("##Z2", &entt->rotation.z, -90.f, 90.f, "Roll = %.3f");
                    ImGui::TreePop();
                }
                ImGui::Separator();
                if (ImGui::TreeNodeEx("Scale"))
                {
                    ImGui::SliderFloat("##X3", &entt->scale.x, -90.f, 90.f, "X = %.3f");
                    ImGui::SliderFloat("##Y3", &entt->scale.y, -90.f, 90.f, "Y = %.3f");
                    ImGui::SliderFloat("##Z3", &entt->scale.z, -90.f, 90.f, "Z = %.3f");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx("Meshes"))
                {
                    auto model = dynamic_cast<Hazel::Model*>(entt);
                        if (model)
                        {
                            for (int i = 0; i < model->Getmeshes().size(); i++)
                            {
                                if (ImGui::TreeNodeEx((model->Getmeshes()[i].m_name.c_str())))
                                {
                                    ImGui::Text(("Material :" + model->Getmeshes()[i].m_Material->materialName).c_str());
                                    if (model->Getmeshes()[i].m_Material->m_Albedo) ImGui::Text(("Albedo texture :" + model->Getmeshes()[i].m_Material->m_Albedo->m_path).c_str());
                                    if (model->Getmeshes()[i].m_Material->m_Specular)ImGui::Text(("Spec texture :" + model->Getmeshes()[i].m_Material->m_Specular->m_path).c_str());
                                    ImGui::SliderFloat("Specular Intensity :" , &model->Getmeshes()[i].m_Material->SpecularIntensity,0,1);
                                    ImGui::SliderFloat("Specular Exponent :", &model->Getmeshes()[i].m_Material->SpecularExponent, 0, 256.f);
                                    ImGui::TreePop();
                                }
                            }
                        }
                    
                    ImGui::TreePop();
                }
                ImGui::Separator();
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

    void RenderLayer::ShowCameraTab()
    {
        ImGui::Begin("Camera Properties");
        if (ImGui::TreeNode("Position"))
        {
            ImGui::SliderFloat("##X", &GetScene()->GetCameraController()->m_CameraPosition.x, -5.f, 5.f, "X = %.3f");
            ImGui::SliderFloat("##Y", &GetScene()->GetCameraController()->m_CameraPosition.y, -5.f, 5.f, "Y = %.3f");
            ImGui::SliderFloat("##Z", &GetScene()->GetCameraController()->m_CameraPosition.z, -5.f, 5.f, "Z = %.3f");
            ImGui::TreePop();
        }
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Rotation"))
        {
            ImGui::SliderAngle("##X", &GetScene()->GetCameraController()->GetCamera().Pitch, -90.f, 90.f, "Pitch = %.3f");
            ImGui::SliderAngle("##Y", &GetScene()->GetCameraController()->GetCamera().Yaw, -90.f, 90.f, "Yaw = %.3f");
            ImGui::SliderAngle("##Z", &GetScene()->GetCameraController()->GetCamera().Roll, -90.f, 90.f, "Roll = %.3f");
            ImGui::TreePop();
        }
        ImGui::Spacing();
        if (ImGui::TreeNodeEx("Settings"))
        {
            ImGui::SliderFloat("FOV", &GetScene()->GetCameraController()->m_FOV, 0.f, 120.f, "FOV = %.3f");
            ImGui::SliderFloat("Sensitivity", &GetScene()->GetCameraController()->GetCamera().MouseSensitivity, 0.f, 8.f, "Sensitivity = %.3f");
            ImGui::SliderFloat("Speed", &GetScene()->GetCameraController()->m_CameraTranslationSpeed, 0.f, 8.f, "Speed = %.3f");
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void RenderLayer::HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    Hazel::Scene* RenderLayer::GetScene()
    {
        return &m_Scene;
    }

