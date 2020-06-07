#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	void Scene::RenderAllPasses()
	{
		// Shadow Pas 
		RenderShadowPass();
		RenderCommand::SetFaceCulling(false);

		// Main pass
		RenderMainPass();

		//Post Process Pass
		RenderPostPass();

		// Bloom Blur
		BloomBlur();

		// Compose the final Image 
		ComposeFinalImage();
	}
}