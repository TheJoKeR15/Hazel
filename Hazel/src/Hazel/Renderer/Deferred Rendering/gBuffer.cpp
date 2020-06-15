#include "hzpch.h"
#include "gBuffer.h"

namespace Hazel {

	gBuffer::gBuffer(int Width, int Height) : m_Width(Width),m_Height(Height)
	{
		InitializeGBuffer();
	}

	gBuffer::~gBuffer()
	{
	}

	void gBuffer::InitializeGBuffer()
	{
		int i = 0;

		// Create the FBO and bind it 
		gBufferFBO = FrameBuffer::Create(Buffer, m_Width, m_Height);
		gBufferFBO->Bind();

		// Now create all the texture attachement and attatch them 
		InitializeTextures();

		gBufferFBO->AttachColorTexture2D(*gBufferColor->GetID(), m_Width, m_Height, 0);i++;
		gBufferFBO->AttachColorTexture2D(*gBufferNormal->GetID(), m_Width, m_Height, 1);i++;
		gBufferFBO->AttachColorTexture2D(*gBufferPosition->GetID(), m_Width, m_Height, 2);i++;
		//gBufferFBO->AttachColorTexture2D(*gBufferA->GetID(), m_Width, m_Height, 3);

		// Now the render buffer and attach it
		RenderBuffer = gBufferFBO->CreateAndAttachRenderBuffer(m_Width, m_Height);

		// Activate MultiRenderTargets
		gBufferFBO->DrawBuffers(i);

	}
	void gBuffer::InitializeTextures()
	{
		//  COLOR + Specular
//-------------------------------------------------------------
		gBufferColor = Texture2D::CreateEmpty(m_Width, m_Height);
		gBufferColor->m_Format = TEXTURE_FORMAT::RGBA;
		gBufferColor->GenerateTexture();

		//  Normal
		//-------------------------------------------------------------
		gBufferNormal = Texture2D::CreateEmpty(m_Width, m_Height);
		gBufferNormal->m_Format = TEXTURE_FORMAT::RGBA32F;
		gBufferNormal->GenerateTexture();

		//  Position
		//-------------------------------------------------------------
		gBufferPosition = Texture2D::CreateEmpty(m_Width, m_Height);
		gBufferPosition->m_Format = TEXTURE_FORMAT::RGBA32F;
		gBufferPosition->GenerateTexture();

		//  Normal
		//-------------------------------------------------------------
		gBufferA = Texture2D::CreateEmpty(m_Width, m_Height);
		gBufferA->m_Format = TEXTURE_FORMAT::RGBA32F;
		gBufferA->GenerateTexture();
	}
}
