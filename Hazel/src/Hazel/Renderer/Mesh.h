#pragma once

#include "glm/glm.hpp"
#include "Hazel/Renderer/Material.h"
#include "Hazel/Renderer/VertexArray.h"

#include "Hazel/Renderer/Vertex.h"



	class Mesh
	{
	public:
		
		std::vector<Vertex>			m_vertices;
		std::vector<uint32_t>	m_indices;
		//glm::mat4 transform;
		Hazel::Ref<Hazel::Material>	m_Material;

		std::string m_name;

		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Hazel::Ref<Hazel::Material> Material,const std::string& name = "UNNAMED MESH");
		
		void Draw(Hazel::Ref<Hazel::Shader> shader);

		void Draw(Hazel::Ref<Hazel::Shader> shader,glm::mat4 Transform);

	private:
		//  render data
		unsigned int VAO, VBO, EBO;

		Hazel::Ref<Hazel::VertexArray> m_VA;

		void setupMesh();

		

	};


