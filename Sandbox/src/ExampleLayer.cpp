#include "ExampleLayer.h"
//
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



ExampleLayer::ExampleLayer() 
	: Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f, glm::vec3(0.f,0.f,5.f))
{
	/*
	m_VertexArray = Hazel::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);
	*/


	m_SquareVA = Hazel::VertexArray::Create();

	float squareVertices[4*(3+3+2)] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,1.0f,	0.0f, 1.0f,
	};

	Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Color" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);
	/*
	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		*/

	//Hazel::Scene* MyScene = new Hazel::Scene();
	//MyScene->InitializeScene();

	m_Light = new Hazel::Light(MainShader);

	auto Mat = std::make_shared<Hazel::Material>(MainShader);
	
	m_Model = new Hazel::Model("assets/models/Cube.obj","Model",Mat, MainShader);

	MainShader = m_ShaderLibrary.Load("assets/shaders/MainShader.glsl");
	
	m_Texture = Hazel::Texture2D::Create("assets/textures/Check.png");
	m_Texture->Bind(0);

	MainShader->SetInt("u_Texture", 0);
	MainShader->Bind();


	m_CameraController.GetCamera().SetPosition(glm::vec3(0.f, 0.f, 5.f));
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Hazel::Timestep ts) 
{
	// Update
	m_CameraController.OnUpdate(ts);
	//m_CameraController.GetCamera().SetProjection()
	// Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
		

	MainShader->SetFloat3("CameraPosition", m_CameraController.GetCamera().GetPosition());
	
	m_Texture->Bind(0);
		
	if (m_Light->bActive)
	{
		MainShader->SetFloat("u_LightIntensity", m_Light->Intensity);
		MainShader->SetFloat("u_LightRadius", m_Light->Radius);
		MainShader->SetFloat("u_AmbiantLight", AmbiantLight);
		MainShader->SetFloat3("u_LightPosition", m_Light->position);
		MainShader->SetFloat3("u_LightColor", m_Light->LightColor);
		MainShader->SetFloat("SpecularStrenght", SpecularStrenght);
		MainShader->SetFloat("SpecularExponent", SpecularExponent);
		MainShader->Bind();
	}
	else
	{
		MainShader->SetFloat("u_LightIntensity", 0);
		MainShader->Bind();
	}
	if (m_Model && m_Model->bVisible)
	{
		m_Model->Draw(MainShader);
	}


	Hazel::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender() 
{
	
	ImGui::Begin("Settings");
		ImGui::Begin("Model");
			
			

		ImGui::Begin("Light Properties");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_Light->LightColor));
		ImGui::SliderFloat("Intensity", &m_Light->Intensity, -5.f, 5.f);
		ImGui::SliderFloat("Ambiant", &AmbiantLight, 0.f, 1.f);
		
		ImGui::Text("Position");
		ImGui::SliderFloat("x", &m_Light->position.x, -5.f, 5.f);
		ImGui::SliderFloat("y", &m_Light->position.y, -5.f, 5.f);
		ImGui::SliderFloat("z", &m_Light->position.z, -5.f, 5.f);
		ImGui::Spacing();
		ImGui::Text("Rotation");
		ImGui::SliderAngle("Pitch", &m_Light->rotation.x, -90.f, 90.f);
		ImGui::SliderAngle("Yaw",	&m_Light->rotation.y, -90.f, 90.f);
		ImGui::SliderAngle("Roll",	&m_Light->rotation.z, -90.f, 90.f);
		ImGui::Spacing();
		ImGui::Checkbox("bDraw ", &m_Light->bVisible);
		ImGui::Checkbox("Active ", &m_Light->bActive);
		ImGui::End();

		ImGui::Begin("Material Properties");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_Light->LightColor));
		ImGui::SliderFloat("Specular Intensity", &SpecularStrenght, 0.f, 1.f);
		ImGui::SliderFloat("Specular Exponent", &SpecularExponent, 0.f, 256.f);


		ImGui::End();
		

		ImGui::Begin("Camera Properties");
		//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		//ImGui::BeginChild("Transform");
			ImGui::Text("Position");
			ImGui::SliderFloat("", &m_CameraController.m_CameraPosition.x, -5.f, 5.f,"X = %.3f");
			ImGui::SliderFloat("Camera y", &m_CameraController.m_CameraPosition.y, -5.f, 5.f);
			ImGui::SliderFloat("Camera z", &m_CameraController.m_CameraPosition.z, -5.f, 5.f);
			ImGui::Spacing();
			ImGui::Text("Rotation");
			ImGui::SliderAngle("Pitch", &m_CameraController.GetCamera().Pitch, -90.f, 90.f);
			ImGui::SliderAngle("Yaw", &m_CameraController.GetCamera().Yaw, -90.f, 90.f);
			ImGui::SliderAngle("Roll", &m_CameraController.GetCamera().Roll, -90.f, 90.f);
			ImGui::Spacing();
		//ImGui::EndChild();

		//ImGui::BeginChild("Advanced");
			ImGui::Text("Settings");
			ImGui::SliderFloat("FOV", &m_CameraController.m_FOV, 0.f, 120.f);
			ImGui::SliderFloat("Sensitivity", &m_CameraController.GetCamera().MouseSensitivity, 0.f, 8.f);
			ImGui::SliderFloat("Speed", &m_CameraController.m_CameraTranslationSpeed, 0.f, 8.f);
			ImGui::SliderFloat("Speed", &m_CameraController.m_CameraTranslationSpeed, 0.f, 8.f);
		//ImGui::EndChild();
		//ImGui::End();
			ImGui::Begin("Model Properites");
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_Model->position.x, -5.f, 5.f);
			ImGui::SliderFloat("Y", &m_Model->position.y, -5.f, 5.f);
			ImGui::SliderFloat("Z", &m_Model->position.z, -5.f, 5.f);
			ImGui::Spacing();
			ImGui::Text("Rotation");
			ImGui::SliderAngle("Pitch", &m_Model->rotation.x, -90.f, 90.f);
			ImGui::SliderAngle("Yaw", &m_Model->rotation.y, -90.f, 90.f);
			ImGui::SliderAngle("Roll", &m_Model->rotation.z, -90.f, 90.f);
			ImGui::Spacing();
			ImGui::Checkbox("bDraw ", &m_Model->bVisible);
			ImGui::End();

		ImGui::End();

	ImGui::End();
}

void ExampleLayer::OnEvent(Hazel::Event& e) 
{
	m_CameraController.OnEvent(e);
}

const aiScene* ExampleLayer::ImportAsset(const std::string& path)
{
			// Create an instance of the Importer class
		Assimp::Importer importer;
		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll 
		// propably to request more postprocessing than we do in this example.
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		// If the import failed, report it
		if (!scene)
		{
			HZ_WARN("Failed to import");
			return false;
		}
		// Now we can access the file's contents. 
		
		// We're done. Everything will be cleaned up by the importer destructor
		return scene;
	
}
