#pragma once
#include "Hazel/Core/Core.h"

class Material;



namespace Hazel {
	class Entity
	{
	public:
		bool bRender = true;

		Material* Material;
	};
}

