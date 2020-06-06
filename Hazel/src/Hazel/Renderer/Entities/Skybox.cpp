#include "hzpch.h"
#include "Hazel/Renderer/Entities/Skybox.h"

namespace Hazel
{
	Skybox::Skybox(Ref<Shader> shader, Ref<TextureCube> CubeMap) : Entity(0),TextureCubeMap(CubeMap)
	{
		m_shader = shader;
		Mesh = std::make_shared<Model>("assets/HDRi/Skymesh.obj", "SkyBox", std::make_shared<Material>(m_shader, "SkyBoxMaterial"), m_shader);
		bRecieveShadow = false;
	}
	//"assets/HDRi/Skymesh.obj"
	void Skybox::OnInit()
	{
	}

	void Skybox::OnBeginFrame()
	{
		//		RecalculateTransforms();
	}

	void Skybox::OnUpdate(float dt)
	{
		//m_shader->SetBool("bHasSpeclarTexture", true);
		TextureCubeMap->Bind();
		m_shader->SetInt("Cubemap", 0);
		
		if (bVisible)
		{
			//DrawMesh();
		}

		Entity::OnUpdate(dt);
	}

	void Skybox::OnEndFrame()
	{
	}

	void Skybox::DrawShawdowPass(Ref<Shader> ShadowPassShader)
	{
	}

	void Skybox::DrawMainPass()
	{
		if (bVisible)
		{	
			m_shader->Bind();
			m_shader->SetFloat("SkyIntensity", SkyIntensity);
			
			DrawMesh();
		}
	}
}