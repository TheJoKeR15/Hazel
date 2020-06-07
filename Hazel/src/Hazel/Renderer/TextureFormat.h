#pragma once
// Type of Texture in the GPU
enum class TEXTURE_FORMAT
{
	RGB,
	RGBA,

	sRGB,
	sRGBA,

	RGB16F,
	RGBA16F,

	RGB32F,
	RGBA32F,

	DEPTH8,
	DEPTH16,
	DEPTH32,

	// One component
	RED
};
