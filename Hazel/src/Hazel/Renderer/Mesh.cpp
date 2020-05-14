#include "hzpch.h"
#include "Mesh.h"

Hazel::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
   // this->m_textures = textures;

    setupMesh();
}

void Hazel::Mesh::setupMesh()
{
	m_VA = Hazel::VertexArray::Create();

	float squareVertices[4 * (3 + 3 + 2)] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,1.0f,	0.0f, 1.0f,
	};

	//Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(m_vertices);
	//squareVB->SetLayout({
	//	{ Hazel::ShaderDataType::Float3, "a_Position" },
	//	{ Hazel::ShaderDataType::Float3, "a_Color" },
	//	{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
	//	});
	//m_VA->AddVertexBuffer(squareVB);

	//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	//Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	//m_VA->SetIndexBuffer(squareIB);
}
