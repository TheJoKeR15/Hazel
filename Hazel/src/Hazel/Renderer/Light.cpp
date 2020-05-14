#include "hzpch.h"
#include "Light.h"

namespace Hazel {

	Hazel::Light::Light()
	{
		LightViz = new Hazel::Model("assets/models/Cube.obj");
	}
}
