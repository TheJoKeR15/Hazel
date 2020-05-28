#include "hzpch.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Material.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Hazel::Ref<Hazel::Material> Material, std::string name) : m_name(name)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_Material = Material;

    setupMesh();
}


void Mesh::Draw(Hazel::Ref<Hazel::Shader> shader, glm::mat4 Transform)
{
    
	m_Material->Update();

	Hazel::Renderer::Submit(Hazel::Ref<Hazel::Shader>(shader), m_VA, Transform);
}


void Mesh::setupMesh()
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
