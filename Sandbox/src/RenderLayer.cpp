#include "RenderLayer.h"

#include "EntityProprieties.h"


#include "Hazel/Core/Input.h"
#include "Hazel.h"

#include "ImGuizmo/ImGuizmo.h"

RenderLayer::RenderLayer(GizmoOverlay* GizmoLayer) :Layer("Render Layer"), m_Scene(Hazel::Scene(1280.0f, 720.0f)), m_GizmoLayer(GizmoLayer)
    {

        // Init
        // setting more pleasent settings for the camera
        MainShader = m_ShaderLibrary.Load("assets/shaders/MainShader.glsl");
        SkyShader = m_ShaderLibrary.Load("assets/shaders/SkyboxShader.glsl");
        DirectionalShadowMapShader = m_ShaderLibrary.Load("assets/shaders/DirectionalShadowMap.glsl");
    }

    void RenderLayer::OnAttach()
    {

        
        std::vector<std::string> faces = {
            "assets/HDRi/Skybox/right.jpg",
                "assets/HDRi/Skybox/left.jpg",
                "assets/HDRi/Skybox/top.jpg",
                "assets/HDRi/Skybox/bottom.jpg",
                "assets/HDRi/Skybox/front.jpg",
                "assets/HDRi/Skybox/back.jpg"
        };
        auto SkyBox_Texture = Hazel::TextureCube::Create(faces);

        ListOfMaterials.push_back(std::make_shared<Hazel::Material>(MainShader, "MainMaterial"));

        auto text = Hazel::Texture2D::Create("assets/textures/Check.png",true);
        auto text2 = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        //auto text2 = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        auto NewMat = std::make_shared<Hazel::Material>(MainShader, text, text2);
        ListOfMaterials.push_back(NewMat);



        //GetScene()->AddEnitity(new Hazel::Model("assets/Scene.obj","Suzane", ListOfMaterials[1],MainShader));

        //GetScene()->AddEnitity(new Hazel::Model("assets/Sponza/sponza.obj", "Sponza", ListOfMaterials[1],MainShader));
        //GetScene()->GetEntities()[0]->scale = glm::vec3(0.1f);
        //GetScene()->GetEntities().
        GetScene()->AddEnitity(new Hazel::Model("assets/nanosuit/nanosuit.obj", "NanoSuite", ListOfMaterials[1], MainShader, DirectionalShadowMapShader));

        GetScene()->GetEntities()[0]->position = glm::vec3(8.f,0.f,3.f);

        GetScene()->AddEnitity(new Hazel::Model("assets/rock/rock.obj", "Rock", ListOfMaterials[1], MainShader, DirectionalShadowMapShader));
        
        GetScene()->GetEntities()[1]->position = glm::vec3(-5.f, 0.f, 3.f);



       
        
         
        //GetScene()->GetEntities()[3]->SetPosition(glm::vec3(-5.f, 15.f, 3.f));

        

        GetScene()->AddEnitity(new Hazel::PointLight(MainShader, 0));
        GetScene()->GetEntities()[2]->SetPosition(glm::vec3(0.f, 15.f, 3.f));
        //GetScene()->AddEnitity(new Hazel::PointLight(MainShader, 1));

        GetScene()->AddEnitity(new Hazel::DirectionalLight(MainShader));
        GetScene()->GetEntities()[3]->SetRotation(glm::vec3(0.f,45.f,45.f));

        //GetScene()->AddEnitity(new Hazel::SpotLight(MainShader, 0));
        //m_Light = dynamic_cast<Hazel::Light*>(GetScene()->GetEntities()[6]);
        if (GetScene()->GetEntities().size() > 0)
        {
            //SelectEntity(GetScene()->GetEntities()[3]);
        }

        m_Scene.GetCameraController()->GetCamera().SetPosition(glm::vec3(-100.f, 50, 0.f));
        m_Scene.GetCameraController()->GetCamera().SetRotation(glm::vec3(-0.f, 90.f, 0.f));
        m_Scene.GetCameraController()->m_FOV = 70;
        m_Scene.GetCameraController()->m_CameraTranslationSpeed = 50.f;

        auto SkyBox = new Hazel::Skybox(SkyShader, SkyBox_Texture);
        SkyBox->displayName = "SkyBox";

        GetScene()->AddEnitity(new Hazel::Model("assets/Scene.obj", "Scene", ListOfMaterials[1], MainShader));

        GetScene()->AddEnitity(SkyBox);

        GetScene()->InitializeScene(DirectionalShadowMapShader);

        GetScene()->InitializeBuffers(ViewPortSize.x, ViewPortSize.y);
    }

    void RenderLayer::OnDetach()
    {
        
    }


    void RenderLayer::OnUpdate(Hazel::Timestep ts)
    { 
        
        SkyShader->Bind();
        SkyShader->SetMat4("u_ViewProjection", GetScene()->GetCameraController()->GetCamera().GetViewProjectionMatrix());

        MainShader->Bind();
        MainShader->SetMat4("u_ViewProjection", GetScene()->GetCameraController()->GetCamera().GetViewProjectionMatrix());
        MainShader->SetFloat("AmbientLight", GetScene()->GetAmbientLighting());
        MainShader->SetFloat3("CameraPosition", GetScene()->GetCameraController()->GetCamera().GetPosition());

        GetScene()->Light = - GetScene()->GetEntities()[3]->ForwardVector;

        // Begin the scene FRAME START
        GetScene()->BeginScene();

        GetScene()->ClearScene();

        GetScene()->RenderScene(ts);

        // End the Scene
        GetScene()->EndScene();

        m_GizmoLayer->SelectedEntity = SelectedEntity;
        //m_GizmoLayer->matrix = SelectedEntity->transform;
        m_GizmoLayer->view = GetScene()->GetCameraController()->GetCamera().GetViewMatrix();
        m_GizmoLayer->projection = GetScene()->GetCameraController()->GetCamera().GetProjectionMatrix();

        //GetScene()->ClearScene();
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

    static bool first = true;
    void RenderLayer::SetupViewPort()
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport",NULL,flags);
        if (ViewPortSize.x != ImGui::GetWindowSize().x || ViewPortSize.y != ImGui::GetWindowSize().y )
        {
            GetScene()->InitializeBuffers(ViewPortSize.x, ViewPortSize.y);
        }
        if (GetScene()->ShadowMapScale != lastres)
        {
            lastres = GetScene()->ShadowMapScale;
            //GetScene()->InitializeBuffers(ViewPortSize.x, ViewPortSize.y);
        }
        ViewPortSize = ImGui::GetWindowSize();
        ViewPortPosition = ImGui::GetWindowPos();

        ImGui::Image((void*)GetScene()->GetFrameBufferTextrure(), ViewPortSize,ImVec2(0,1), ImVec2(1, 0));
        GetScene()->GetCameraController()->SetNewViewPortSize(ViewPortSize.x, ViewPortSize.y);

        ImGui::End();

        ImGui::Begin("DepthMap", NULL, flags);

        //ViewPortSize = ImGui::GetWindowSize();
        //ViewPortPosition = ImGui::GetWindowPos();

        ImGui::Image((void*)GetScene()->GetShadowMapTextrure(), ImVec2(1024,1024), ImVec2(0, 1), ImVec2(1, 0));
        //GetScene()->GetCameraController()->SetNewViewPortSize(ViewPortSize.x, ViewPortSize.y);

        ImGui::End();

        ImGui::Begin("PostProcess", NULL, flags);

        //ViewPortSize = ImGui::GetWindowSize();
        //ViewPortPosition = ImGui::GetWindowPos();

        ImGui::Image((void*)GetScene()->GetPostProcessTextrure(), ViewPortSize, ImVec2(0, 1), ImVec2(1, 0));
        //GetScene()->GetCameraController()->SetNewViewPortSize(ViewPortSize.x, ViewPortSize.y);

        ImGui::End();

        ImGui::Begin("FinalImage", NULL, flags);

        //ViewPortSize = ImGui::GetWindowSize();
        //ViewPortPosition = ImGui::GetWindowPos();

        ImGui::Image((void*)GetScene()->GetFinalImageTextrure(), ViewPortSize, ImVec2(0, 1), ImVec2(1, 0));
        //GetScene()->GetCameraController()->SetNewViewPortSize(ViewPortSize.x, ViewPortSize.y);

        ImGui::End();

        ImGui::PopStyleVar();
    }

    void RenderLayer::SelectEntity(Hazel::Entity* newSelectedEntity)
    {
        if (SelectedEntity)
        {
            SelectedEntity->bIsSelected = false;
        }
        if (!(SelectedEntity == newSelectedEntity))
        {
            SelectedEntity = newSelectedEntity;
            SelectedEntity->bIsSelected = true;
        }
    }

    void RenderLayer::ShowSceneSettings()
    {
        ImGui::Begin("Scene Settings");
        ImGui::DragFloat("Ambient Lighting ", &GetScene()->GetAmbientLighting());
        ImGui::ColorEdit3("Background Color ", glm::value_ptr(*GetScene()->GetBackGroundColor()));
        ImGui::End();
    }

    void RenderLayer::ShowDebugPanel()
    {
        ImGui::Begin("Debug");
        if (SelectedEntity)
        {
        ImGui::Text(("Currently selected = " + SelectedEntity->displayName).c_str());
        }
        else
        {
            ImGui::Text("Currently selected = NONE " );
        }
        
        ImGui::Text("Frame Buffer pointer = %p", FrameBuffertexture);
        ImGui::Text("ViewPort size = %f x %f", ViewPortSize.x, ViewPortSize.y);
        ImGui::Text("ViewPort Position = %f x %f", ViewPortPosition.x, ViewPortPosition.y);
        char name[16] = { "New Entity" };
        ImGui::InputText("Name :", name, IM_ARRAYSIZE(name));ImGui::SameLine();
        if (ImGui::Button("Add Entity"))
        {

            GetScene()->AddEnitity(new Hazel::Model("assets/models/Cube.obj" ,name, ListOfMaterials[0],MainShader, DirectionalShadowMapShader));
        }
        if (ImGui::Button("Add Sponza"))
        {
            GetScene()->AddEnitity(new Hazel::Model("assets/Sponza/sponza.obj","Sponza", ListOfMaterials[1],  MainShader, DirectionalShadowMapShader));

            auto n = GetScene()->GetEntities().size();
            GetScene()->GetEntities()[n-1]->scale = glm::vec3(0.1f);
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
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
        int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
               
        ImGui::Begin("Outliner");
        bool p_selected = false;
        for (int i = 0; i < GetScene()->GetEntities().size(); i++)
        {
            auto entt = GetScene()->GetEntities()[i];
            std::string name = GetScene()->GetEntities()[i]->displayName;
            

                // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
                ImGuiTreeNodeFlags node_flags = base_flags;
                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;

                // Items 0..2 are Tree Node
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, name.c_str());

                if (ImGui::IsItemClicked())
                    node_clicked = i;

                if (node_open)
                {
                    // Display the meshes of the model//
                    auto model = dynamic_cast<Hazel::Model*>(SelectedEntity);
                    if (model)
                    {
                        for (int i = 0; i < model->Getmeshes().size(); i++)
                        {
                            if (ImGui::TreeNode((model->Getmeshes()[i].m_name.c_str())))
                            {
                                ImGui::TreePop();
                            }

                        }
                    }
                    ImGui::TreePop();
                }
                if (node_clicked != -1)
                {
                    // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
                    if (ImGui::GetIO().KeyCtrl)
                        selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                    else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
                        selection_mask = (1 << node_clicked);           // Click to single-select
                }
                if (node_clicked > -1)
                {
                    SelectEntity(GetScene()->GetEntities()[node_clicked]);
                }
                else
                {
                    //SelectEntity(nullptr);
                }
        }
        ImGui::End();
    }

    void RenderLayer::ShowCameraTab()
    {
        ImGui::Begin("Camera Properties");
        ImGui::Text("Settings");
        {
            ImGui::Text("ManuelMode");
            ImGui::DragFloat("Exposure Compenstaion", &GetScene()->GetCameraController()->ExposureComp, 0.1f, -8.f, 8.f, "%.1f ");
            ImGui::DragFloat("Aperture", &GetScene()->GetCameraController()->aperature,0.5f, 0.1f, 24.f, "f/ %.1f ");
            ImGui::DragFloat("ShutterSpeed", &GetScene()->GetCameraController()->ShutterSpeed, 0.1f, 01.f, 1000.f, "1/%.0f");
            ImGui::DragFloat("ISO", &GetScene()->GetCameraController()->Iso, 1.f, 100.f, 16000.f, "ISO = %.0f");
            ImGui::DragFloat("BloomTreshold", &GetScene()->GetCameraController()->BloomTreshold, 1.f, 0.0f, 3.f, " %.3f");
            ImGui::Separator();
            ImGui::SliderFloat("FOV", &GetScene()->GetCameraController()->m_FOV, 0.f, 120.f, "FOV = %.3f");
            ImGui::SliderFloat("Sensitivity", &GetScene()->GetCameraController()->GetCamera().MouseSensitivity, 0.f, 8.f, "Sensitivity = %.3f");
            ImGui::SliderFloat("Speed", &GetScene()->GetCameraController()->m_CameraTranslationSpeed, 0.f, 8.f, "Speed = %.3f");
            ImGui::SliderFloat("##Far", &GetScene()->GetCameraController()->GetCamera().FarClip, 0.f, 120000.f, "Far = %.3f");
            ImGui::SliderFloat("##Near", &GetScene()->GetCameraController()->GetCamera().NearClip, 0.f, 120.f, "Near = %.3f");
            
        }
        ImGui::Separator();
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
        ImGui::End();
        ImGui::Begin("Shadow Properties");
        if (ImGui::TreeNode("Position"))
        {
            ImGui::SliderFloat("##X", &GetScene()->Light.x, -50.f, 50.f, "X = %.3f");
            ImGui::SliderFloat("##Y", &GetScene()->Light.y, -50.f, 50.f, "Y = %.3f");
            ImGui::SliderFloat("##Z", &GetScene()->Light.z, -50.f, 50.f, "Z = %.3f");
            ImGui::TreePop();
        }
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Rotation"))
        {
            ImGui::SliderAngle("##X", &GetScene()->LightFrustum.Pitch, -90.f, 90.f, "Pitch = %.3f");
            ImGui::SliderAngle("##Y", &GetScene()->LightFrustum.Yaw, -90.f, 90.f, "Yaw = %.3f");
            ImGui::SliderAngle("##Z", &GetScene()->LightFrustum.Roll, -90.f, 90.f, "Roll = %.3f");
            ImGui::TreePop();
        }
        ImGui::Spacing();
        if (ImGui::TreeNodeEx("Settings"))
        {
            //ImGui::SliderFloat("FOV", &GetScene()->LightFrustum, 0.f, 120.f, "FOV = %.3f");
            ImGui::SliderFloat("Sensitivity", &GetScene()->GetCameraController()->GetCamera().MouseSensitivity, 0.f, 8.f, "Sensitivity = %.3f");
            ImGui::SliderFloat("Speed", &GetScene()->GetCameraController()->m_CameraTranslationSpeed, 0.f, 8.f, "Speed = %.3f");
            ImGui::SliderFloat("##Far", &GetScene()->far_plane, 0.f, 120.f, "Far = %.3f");
            ImGui::SliderFloat("##Near", &GetScene()->near_plane, 0.f, 120.f, "Near = %.3f");
            ImGui::SliderFloat("##elft", &GetScene()->up, -30.f, 30.f, "up = %.3f");
            ImGui::SliderFloat("##sdg", &GetScene()->down, -30.f, 30.f, "down = %.3f");
            ImGui::SliderFloat("##ssdfg", &GetScene()->left, -30.f, 30.f, "left = %.3f");
            ImGui::SliderFloat("##sdf", &GetScene()->right, -30.f, 30.f, "right = %.3f");

            ImGui::SliderInt("Resolution", &GetScene()->ShadowMapScale, 0, 10);
            ImGui::SliderFloat("Bias",&GetScene()->ShadowBias, 0.f, 1.f);

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


    void RenderLayer::OnImGuiRender()
    {

        m_GizmoLayer->SizeX = ViewPortSize.x;
        m_GizmoLayer->SizeY = ViewPortSize.y;

        m_GizmoLayer->PosX = ViewPortPosition.x;
        m_GizmoLayer->PosY = ViewPortPosition.y;


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

        ShowPropertiesPanel(SelectedEntity);

        // Temporary
        ImGui::ShowDemoWindow();
        // Gizmo

        ImGui::Begin("Gizmo Debug");
        ImGui::SliderFloat("PosX", &m_GizmoLayer->PosX, -100, 100);
        ImGui::SliderFloat("PosY", &m_GizmoLayer->PosY, -100, 100);

        ImGui::SliderFloat("SizeX", &m_GizmoLayer->SizeX, -100, 100);
        ImGui::SliderFloat("SizeY", &m_GizmoLayer->SizeY, -100, 100);
        ImGui::End();
        //DrawGizmo();
    }

    void RenderLayer::OnEvent(Hazel::Event& e)
    {
        if (Hazel::Input::IsKeyPressed(HZ_KEY_F))
        {
            FocusToSelected();
        }
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

    void RenderLayer::FocusToSelected()
    {
        GetScene()->GetCameraController()->m_CameraPosition = SelectedEntity->position;
    }


