#pragma once

#include <string>

#include "Hazel/Core/Core.h"

namespace Hazel {


	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void ReBind() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		// CALL CreatEmpty() if you want to specify the format ( ussualy when using buffers) because this will creat a texture object in the GPU
		// Create a dummy texture ( blank) by specifiying the dimentions) 
		// CALL SetTextureParameters() after because this will make an RGBA format with no linear filtering and no mipmaps texture
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, bool HDR = false);

		// Create a dummy texture ( blank) by specifiying the dimentions) 
		// CALL SetTextureParameters() after because this will make an RGBA format with no linear filtering and no mipmaps texture
		static Ref<Texture2D> CreateEmpty(uint32_t width, uint32_t height);

		// Create a texture from an image file by providing the path to it (please use double \\ slashes or forrward slashes / )
		// This will try and get the format from the file 
		static Ref<Texture2D> Create(const std::string& path ,bool bsRGB = false,bool bLinear = true);

		
		// Call this after changing the texture parameters ( Texture format , linear filtering ...)
		virtual void GenerateTexture() = 0;
		
		// function to get the texture unit
		virtual uint32_t* GetSlot() = 0;

		// Get the format of this texture see "Hazel/Renderer/Texture.h" for the currently Supported Formats
		virtual TEXTURE_FORMAT GetFormat() { return m_Format; };

		// Get the RendererID of this texture 
		virtual uint32_t* GetID() = 0;

		// USE ONLY FOR DEBUG
		// THIS CHANGE THE ID OF THE TEXTUER THUS CHANGING THE TEXTURE THAT THIS REFERNCES IN THE GPU //
		virtual void SetID(uint32_t ID) = 0;
		
		// the path if the texture was loaded from a file 
		std::string m_path ;

		// Format of the texture 
		// Change this before calling SetTextureParameters()
		TEXTURE_FORMAT m_Format = TEXTURE_FORMAT::RGBA;

		//// is this texture in the sRGB color space (oposit for the Linear color space)
		//bool sRGB = false;

		//// Does the values of the pixel exceed 1 and thus should not be clamped
		//bool HDR = false;

		// Use Linear Filtering when sampling this texture ( as oposite to Nearest Filtering)
		// @TODO : Add Anisotropic Flitering Later
		bool bLinearFiltering = true;

		// Should the GPU generate MipMaps for this Textures
		bool bGenMips = false;

		// Should this Texture Be Clamped at the border or Repeated
		bool bClamp = true;
	};

	class TextureCube : public Texture
	{
	public:
		//static Ref<TextureCube> Create(uint32_t width, uint32_t height);
		static Ref<TextureCube> Create(std::vector<std::string>& faces);
		// function to get the id of the texture in memory
		virtual uint32_t* GetSlot() = 0;
		virtual uint32_t* GetID() = 0;
		
		std::string m_path;
	};


}