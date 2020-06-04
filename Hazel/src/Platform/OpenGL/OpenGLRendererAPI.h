#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void InitFrameBuffer() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void ClearDepth() override;
		virtual void FrontCulling() override;
		virtual void BackCulling() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;


		uint32_t Buffer;
		uint32_t FrameBuffertexture;
		Hazel::Ref<Hazel::FrameBuffer> FrameBuffer;
	};


}
