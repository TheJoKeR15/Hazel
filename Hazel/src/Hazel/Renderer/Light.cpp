#include "hzpch.h"
#include "Light.h"

namespace Hazel {

	Hazel::Light::Light(Hazel::Ref<Hazel::Shader> shader)
	{
		m_shader = shader;
		LightViz = new Hazel::Model("assets/models/Cube.obj","Light", std::make_shared<Hazel::Material>(shader, "MainMaterial"),shader);
	}
	void Light::OnInit()
	{

	}

	void Light::OnBeginFrame()
	{
//		RecalculateTransforms();
	}

	void Light::OnUpdate(float dt)
	{
//		RecalculateTransforms();
		if (bVisible)
		{
			Draw();
		}
	}

	void Light::OnEndFrame()
	{
	}

	void Light::Draw()
	{
		
		
	}


}
