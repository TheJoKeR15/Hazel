#pragma once

#include "Hazel.h"
#include <string>



class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;


	void ImportModel(char*);
	const aiScene* ImportAsset(const std::string& path);

private:
	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

	Hazel::CameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	float m_modelRotation = 0.f;
	glm::vec3 RotationAxis = glm::vec3(0.f, 0.f, 0.f);

	Hazel::Ref<Hazel::Shader> MainShader;

	Hazel::Model* m_Model;

	char m_path[50] = { 0 };

	Hazel::Light* m_Light;
	float AmbiantLight = 0.1f;
	
};

