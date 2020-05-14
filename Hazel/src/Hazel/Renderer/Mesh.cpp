#include "hzpch.h"
#include "Mesh.h"
#include "Renderer.h"

Hazel::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
   // this->m_textures = textures;

    setupMesh();
}


void Hazel::Mesh::Draw(Hazel::Ref<Hazel::Shader> shader, glm::mat4 Transform)
{
	Hazel::Renderer::Submit(Ref<Shader>(shader), m_VA, Transform);
}

void Hazel::Mesh::DrawCube(Hazel::Ref<Hazel::Shader> shader, glm::mat4 Transform)
{
}

void Hazel::Mesh::setupMesh()
{
	m_VA = Hazel::VertexArray::Create();

	Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(m_vertices);
	
	squareVB->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Normal" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_VA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(m_indices);
	m_VA->SetIndexBuffer(squareIB);
	
}
