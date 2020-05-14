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
		std::vector<uint32_t>	m_indices;
		glm::mat4 transform;
		//std::vector<Texture>	m_textures;

		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		
		void Draw(Hazel::Ref<Hazel::Shader> shader);

		void Draw(Hazel::Ref<Hazel::Shader> shader,glm::mat4 Transform);

		void DrawCube(Hazel::Ref<Hazel::Shader> shader, glm::mat4 Transform);

	private:
		//  render data
		unsigned int VAO, VBO, EBO;

		Hazel::Ref<Hazel::VertexArray> m_VA;

		void setupMesh();

		

	};
}

