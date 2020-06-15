#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {
	class gBuffer
	{
	public:
		gBuffer(int Width,int Height);
		~gBuffer();

		void InitializeGBuffer();

		void InitializeTextures();

		void Bind() { gBufferFBO->Bind(); };

		void Unbind() { gBufferFBO->Unbind(); };

		
		Ref<FrameBuffer> gBufferFBO;

		// Stores Albedo And specular
		Ref<Texture2D> gBufferColor;
		// World space normal
		Ref<Texture2D> gBufferNormal;
		// Pixel position
		Ref<Texture2D> gBufferPosition;
		// Extra 1
		Ref<Texture2D> gBufferA;
		//Ref<Texture2D> gBufferB;
		
		
	private:
	uint32_t Buffer;
	uint32_t RenderBuffer;
	uint32_t m_Width;
	uint32_t m_Height;
	};
}
