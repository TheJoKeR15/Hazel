#pragma once

#include "Hazel/Renderer/Texture.h"

#include <glad/glad.h>

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, bool sRGB = false, bool HDR = false, bool bLinear = true);
		OpenGLTexture2D(const std::string& path, bool sRGB = false , bool HDR = false, bool bLinear = true);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0)  override;

		virtual void ReBind() const override;
		
		virtual uint32_t* GetSlot() override { return &m_slot; };

		virtual uint32_t* GetID() override { return &m_RendererID; };

		virtual void SetID(uint32_t ID) override { m_RendererID = ID; };

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		//std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_slot = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		//OpenGLTextureCube(const std::string& path);
		OpenGLTextureCube(std::vector<std::string>& faces);
		virtual ~OpenGLTextureCube();

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0)  override;

		virtual void ReBind() const override;

		virtual uint32_t* GetSlot() override { return &m_slot; };

		virtual uint32_t* GetID() override { return &m_RendererID; };

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTextureCube&)other).m_RendererID;
		}
	private:
		//std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_slot = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
