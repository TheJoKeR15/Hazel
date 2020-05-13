#include "ExampleLayer.h"

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

	
	MainShader = m_ShaderLibrary.Load("assets/shaders/MainShader.glsl");
	
	m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Texture->Bind(0);

	MainShader->SetInt("u_Texture", 0);
	MainShader->Bind();
	
	//m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
	
	//m_FlatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
	//m_FlatColorShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	//m_ShaderLibrary.Add("flat",m_FlatColorShader);
	//m_ShaderLibrary.Load("flat");
	//m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	//m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
	//m_FlatColorShader = m_ShaderLibrary.Get("Texture");

	//m_Texture->Bind();
	//m_FlatColorShader->Bind();
	//m_FlatColorShader->SetInt("u_Texture", 0);

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

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));

	//m_FlatColorShader->Bind();
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

	
	//Front
	m_Texture->Bind(0);
	glm::vec3 pos(0.f, 0.f, 0.5f);
	m_SquareColor = glm::vec3(0.8f, 0.f, 0.f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	MainShader->SetFloat3("u_Color", m_SquareColor);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *  scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	//right
	m_SquareColor = glm::vec3(0.2f, 0.8f, 0.f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f,0.f,0.f)) * glm::rotate(glm::mat4(1.f),glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)) * scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	//left
	m_SquareColor = glm::vec3(0.0f, 0.f, 0.8f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.f, 0.f)) * glm::rotate(glm::mat4(1.f), glm::radians(+90.f), glm::vec3(0.f, 1.f, 0.f)) * scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	//top
	m_SquareColor = glm::vec3(0.f, 0.4f, 0.f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.5f, 0.f)) * glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)) * scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	//bottom
	m_SquareColor = glm::vec3(0.1f, 0.2f, 0.4f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -0.5f, 0.f)) * glm::rotate(glm::mat4(1.f), glm::radians(+90.f), glm::vec3(1.f, 0.f, 0.f)) * scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	//back
	m_SquareColor = glm::vec3(0.8f, 0.3f, 0.1f);
	//m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -0.5f)) * glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f)) * scale;
	Hazel::Renderer::Submit(MainShader, m_SquareVA, transform);

	/*
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}
	*/
	//auto textureShader = m_ShaderLibrary.Get("Texture");

	//m_Texture->Bind();
	//Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	//m_ChernoLogoTexture->Bind();
	//Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	// Hazel::Renderer::Submit(m_Shader, m_VertexArray);
	
	Hazel::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender() 
{
	
	ImGui::Begin("Settings");
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
		ImGui::End();
	ImGui::End();
}

void ExampleLayer::OnEvent(Hazel::Event& e) 
{
	m_CameraController.OnEvent(e);
}
