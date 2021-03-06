#pragma once

#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/Shader.h"


namespace Hazel {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void OnWindowResize(uint32_t width, uint32_t height);


		static void BeginScene(Camera& camera);
		static void EndScene();

		static uint32_t GetNextTextureSlot() { return TextureIndex++; };

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static uint32_t TextureIndex;

		static Scope<SceneData> s_SceneData;


	};
}
