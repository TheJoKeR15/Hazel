#pragma once

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Core/CoreRenderer.h"
#include <Platform\OpenGL\OpenGLShader.h>


namespace Hazel {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(std::vector<Vertex>& vertices);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		OpenGLIndexBuffer(std::vector<uint32_t> indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t& Index, int Width, int height, bool bDepthOnly = false);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t* GetTexture()  ;

		virtual uint32_t CreateColorTexture2D(int Width, int height, bool bLinear = false, bool HDR = false) override;
		virtual uint32_t CreateDepthTexture2D(int Width, int height) override;
		virtual uint32_t CreateAndAttachRenderBuffer(int Width, int height) override;

		void DrawBuffers(int n) override;

		virtual void AttachColorTexture2D(uint32_t& Texture, int Width, int height,int index = 0)override;
		virtual void AttachDepthTexture2D(uint32_t& Texture, int Width, int height)override;

		virtual void AttachColorCubemap(uint32_t& Texture, int Width, int height)override;
		virtual void AttachDepthCubemap(uint32_t& Texture, int Width, int height)override;

		static uint32_t m_Index ;
		virtual void FreeBuffer() const;
	private:

		
		
		uint32_t Buffer ;

		uint32_t TextureBuffer ;
		uint32_t DepthMap ;
		uint32_t RenderBuffer ;
	};
}