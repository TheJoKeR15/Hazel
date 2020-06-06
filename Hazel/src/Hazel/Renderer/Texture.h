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
		static Ref<Texture2D> Create(uint32_t width, uint32_t height,bool sRGB = false, bool HDR = false,bool bLinear = true);
		static Ref<Texture2D> Create(const std::string& path, bool sRGB = false , bool HDR = false, bool bLinear = true);
		static Ref<Texture2D> CreateDepth(uint32_t width, uint32_t height,  bool bLinear = true);
		// function to get the id of the texture in memory
		virtual uint32_t* GetSlot() = 0;
		virtual uint32_t* GetID() = 0;
		virtual void SetID(uint32_t ID) = 0;
		std::string m_path ;
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