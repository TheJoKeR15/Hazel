#include "hzpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"
//#include "Hazel/Renderer/Renderer.h"

#include <stb_image.h>
#include <vector>

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height,bool HDR)
		: m_Width(width), m_Height(height)
	{
		HZ_PROFILE_FUNCTION();

		m_InternalFormat = HDR ? GL_RGBA32F : GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool bsRGB, bool bLinear)
		
	{
		m_path = path;
		HZ_PROFILE_FUNCTION();

		int width, height, channels;
		//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		switch (channels)
		{
		case(1):
		{
			m_Format = TEXTURE_FORMAT::RED;internalFormat = dataFormat = GL_RED;
			break;
		}
		case(3):
		{
			internalFormat = bsRGB ? GL_SRGB8 : GL_RGB8;
			dataFormat = GL_RGB;
			m_Format = bsRGB ? TEXTURE_FORMAT::sRGB : TEXTURE_FORMAT::RGB;
			//sRGB = bsRGB;
			break;
		}
		case(4):
		{
			internalFormat = bsRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			dataFormat = GL_RGBA;
			m_Format = bsRGB ? TEXTURE_FORMAT::sRGBA : TEXTURE_FORMAT::RGB;
			//sRGB = bsRGB;
			break;
		}
		default:
			break;
		}
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, bLinear ? GL_LINEAR : GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, bLinear ? GL_LINEAR : GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		if (false)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		stbi_image_free(data);
	}


	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height):
		m_Width(width),m_Height(height)
	{
		// EMPTY TEXTURE
		m_RendererID = 0;
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::GenerateTexture()
	{
		switch (m_Format)
		{
			case(TEXTURE_FORMAT::RED):
			{
				m_InternalFormat = GL_RED;
				m_DataFormat = GL_RED;
				break;
			}
			case(TEXTURE_FORMAT::RGB):
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::RGBA):
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::sRGB):
			{
				m_InternalFormat = GL_SRGB8;
				m_DataFormat = GL_SRGB;
				break;
			}
			case(TEXTURE_FORMAT::sRGBA):
			{
				m_InternalFormat = GL_SRGB8_ALPHA8;
				m_DataFormat = GL_SRGB;
				break;
			}
			case(TEXTURE_FORMAT::RGB16F):
			{
				m_InternalFormat = GL_RGB16F;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::RGBA16F):
			{
				m_InternalFormat = GL_RGBA16F;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::RGB32F):
			{
				m_InternalFormat = GL_RGB32F;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::RGBA32F):
			{
				m_InternalFormat = GL_RGBA32F;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::DEPTH8):
			{
				m_InternalFormat = GL_DEPTH_COMPONENT;
				m_DataFormat = GL_RGB;
				break;
			}
			case(TEXTURE_FORMAT::DEPTH16):
			{
				m_InternalFormat = GL_DEPTH_COMPONENT16;
				m_DataFormat = GL_RGB;
			}
			case(TEXTURE_FORMAT::DEPTH32):
			{
				m_InternalFormat = GL_DEPTH_COMPONENT32;
				m_DataFormat = GL_RGB;


			default:
				break;
			}

		}
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		//Defining the parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bLinearFiltering ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bLinearFiltering ? GL_LINEAR : GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

		if (bGenMips) glGenerateMipmap(GL_TEXTURE_2D);

		// unbind
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) 
	{
		HZ_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
		m_slot = slot;
	}
	void OpenGLTexture2D::ReBind() const
	{
		glBindTextureUnit(m_slot, m_RendererID);
	}

	// Cube maps
	OpenGLTextureCube::OpenGLTextureCube(std::vector<std::string>& faces)
	{

		//HZ_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		int width, height, channels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		m_Width = width;
		m_Height = height;
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTextureCube::SetData(void* data, uint32_t size)
	{
	}
	void OpenGLTextureCube::Bind(uint32_t slot)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}
	void OpenGLTextureCube::ReBind() const
	{
	}
}