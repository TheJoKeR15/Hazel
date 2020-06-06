#include "hzpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

namespace Hazel {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<Vertex>& vertices)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		if (vertices.size() > 0)
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		else
		{
			HZ_ERROR("No vertex in verticies");
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t> indices, uint32_t count)
		: m_Count(count)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0],  GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t &Index ,int Width, int height, bool bDepthOnly) : Buffer(Index)
	{
		HZ_PROFILE_FUNCTION();

		glGenFramebuffers(1, &Buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, Buffer);
		if (bDepthOnly)
		{
			// create the Depth attachement
			glGenTextures(1, &DepthMap);
		}
		else
		{
			glGenTextures(1, &TextureBuffer);

			// Creating a texture for a framebuffer

			glGenRenderbuffers(1, &RenderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);

		}
		// create the texture attachement


		//Bind the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//OpenGLFrameBuffer::m_Index++;
		//Unbind();

		
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		FreeBuffer();
	}



	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, Buffer);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t* OpenGLFrameBuffer::GetTexture() 
	{
		return &TextureBuffer;
	}

	uint32_t OpenGLFrameBuffer::CreateColorTexture2D(int Width,int height, bool bLinear , bool HDR)
	{
		uint32_t newTexture = 0;
		glGenTextures(1, &newTexture);
		glBindTexture(GL_TEXTURE_2D, newTexture);
		// Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0,HDR ? GL_RGBA16F : GL_RGB, Width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		//Defining the parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bLinear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bLinear ? GL_LINEAR : GL_NEAREST);

		return newTexture;
	}

	uint32_t OpenGLFrameBuffer::CreateDepthTexture2D(int width, int height)
	{
		// Resize the viewport to the size of the Buffer
		glViewport(0, 0, width, height);

		glGenTextures(1, &DepthMap);
		glBindTexture(GL_TEXTURE_2D, DepthMap);


		
		// Generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//Defining the parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		return DepthMap;
	}

	uint32_t OpenGLFrameBuffer::CreateAndAttachRenderBuffer(int width, int height)
	{

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return RenderBuffer;
	}

	void OpenGLFrameBuffer::DrawBuffers(int n)
	{
		auto ColorAttachements = new GLenum [n];
		for (int i = 0; i < n; i++)
		{
			ColorAttachements[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(n, ColorAttachements);
	}

	void OpenGLFrameBuffer::AttachColorTexture2D(uint32_t& Texture, int Width, int height, int index)
	{
		//attach it to the framebuffer:
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, Texture, 0);
	}

	void OpenGLFrameBuffer::AttachDepthTexture2D(uint32_t& Texture, int Width, int height)
	{
		Bind();
		//glBindTexture(GL_TEXTURE_2D, Texture);
		//attach it to the framebuffer:
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void OpenGLFrameBuffer::AttachColorCubemap(uint32_t& Texture, int Width, int height)
	{
	}

	void OpenGLFrameBuffer::AttachDepthCubemap(uint32_t& Texture, int Width, int height)
	{
	}

	void OpenGLFrameBuffer::FreeBuffer() const
	{

		glDeleteFramebuffers(1, &Buffer);
	}

}
