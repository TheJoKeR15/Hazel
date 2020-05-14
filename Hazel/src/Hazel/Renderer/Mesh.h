#pragma once

#include "glm/glm.hpp"
//#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"

#include "Hazel/Renderer/Vertex.h"

class Texture;

namespace Hazel {
	class Mesh
	{
	public:
		
		std::vector<Vertex>			m_vertices;
		std::vector<unsigned int>	m_indices;
		//std::vector<Texture>	m_textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		
		void Draw(Shader* shader);

	private:
		//  render data
		unsigned int VAO, VBO, EBO;

		Hazel::Ref<Hazel::VertexArray> m_VA;

		void setupMesh();

	};
}

