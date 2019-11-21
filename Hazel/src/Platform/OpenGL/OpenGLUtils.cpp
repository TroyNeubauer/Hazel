#include "hzpch.h"
#include "OpenGLUtils.h"

#include "FreeImage.h"
#include "OpenGLMacro.h"

namespace Hazel {
	GLenum OpenGLUtils::ShaderDataTypeToGLType(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Float16:	return GL_HALF_FLOAT;
			case ShaderDataType::Float16_2:	return GL_HALF_FLOAT;
			case ShaderDataType::Float16_3:	return GL_HALF_FLOAT;
			case ShaderDataType::Float16_4:	return GL_HALF_FLOAT;
			case ShaderDataType::Float64:	return GL_DOUBLE;
			case ShaderDataType::Float64_2:	return GL_DOUBLE;
			case ShaderDataType::Float64_3:	return GL_DOUBLE;
			case ShaderDataType::Float64_4:	return GL_DOUBLE;
			case ShaderDataType::Int64:		return GL_INT;
			case ShaderDataType::Int64_2:	return GL_INT;
			case ShaderDataType::Int64_3:	return GL_INT;
			case ShaderDataType::Int64_4:	return GL_INT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Int16:		return GL_SHORT;
			case ShaderDataType::Int16_2:	return GL_SHORT;
			case ShaderDataType::Int16_3:	return GL_SHORT;
			case ShaderDataType::Int16_4:	return GL_SHORT;
			case ShaderDataType::Int8:		return GL_BYTE;
			case ShaderDataType::Int8_2:	return GL_BYTE;
			case ShaderDataType::Int8_3:	return GL_BYTE;
			case ShaderDataType::Int8_4:	return GL_BYTE;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Bool:		return GL_BOOL;
			default:
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}

	GLenum OpenGLUtils::TextureFormatTGLType(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RED: return GL_RED;
			case TextureFormat::RGB: return GL_RGB;
			case TextureFormat::RGBA: return GL_RGBA;
			default: HZ_CORE_ASSERT(false, "Invalid GPUTextureFormat"); return 0;
		}
	}

	GLuint OpenGLUtils::Load2DTexture(File* file, uint32_t& width, uint32_t& height, TextureBuilder builder)
	{
		TUtil::Timer timer;

		if (file->Data() == nullptr)
			return false;

		FIMEMORY* memory = FreeImage_OpenMemory((BYTE*) file->Data(), file->Length());
		if (!memory) {
			HZ_CORE_WARN("Failed to open memory for file \"{}\"", file->GetPath().ToString());
			return 0;
		}
		//check the file signature and deduce its format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileTypeFromMemory(memory, file->Length());

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(file->GetPath().ToString());
		if (fif == FIF_UNKNOWN) {
			HZ_CORE_WARN("Unknown image file \"{}\"", file->GetPath().ToString());
			return 0;
		}


		//check that the plugin has reading capabilities and load the file
		FIBITMAP* dib = nullptr;
		GLuint id = 0;

		{
			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_LoadFromMemory(fif, memory);
			else {
				HZ_CORE_WARN("Freeimage plugin cannot read file \"{}\"", file->GetPath().ToString());
				return 0;
			}

			//if the image failed to load, return failure
			if (!dib) {
				HZ_CORE_WARN("Failed to decode image file \"{}\"", file->GetPath().ToString());
				return 0;
			}

			//Determine the Open GL image type
			FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
			GLenum imageFormat = -1, imageType = GL_UNSIGNED_BYTE;
			GLenum gpuFormat = -1;
			unsigned int bpp = FreeImage_GetBPP(dib);
			unsigned int redM = FreeImage_GetRedMask(dib), greenM = FreeImage_GetGreenMask(dib), blueM = FreeImage_GetBlueMask(dib);

			BYTE* bits = FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib), height = FreeImage_GetHeight(dib);
			if ((bits == nullptr) || (width == 0) || (height == 0))
				return false;

			switch (type)
			{
			case FIT_BITMAP://! standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
			{
				if (bpp == 8)
				{
					imageFormat = GL_RED;
				}
				else
				{
					if (!redM || !greenM || !blueM) {
						HZ_CORE_ASSERT(false, "Image \"{}\" missing one RGB component R:0x{:x}, G:0x{:x}, B:0x{:x}", file->GetPath().ToString(), redM, greenM, blueM);
						goto end;
					}
						if (bpp == 24) {
							if (blueM == 0xFF0000 && greenM == 0xFF00 && redM == 0xFF) {
#ifdef HZ_LITTLE_ENDIAN
								imageFormat = GL_RGB;
#else
								imageFormat = GL_BGR;
#endif
							}
							else if (redM == 0xFF0000 && greenM == 0xFF00 && blueM == 0xFF) {
#ifdef HZ_LITTLE_ENDIAN
								imageFormat = GL_BGR;
#else
								imageFormat = GL_RGB;
#endif
							}
							else {
								HZ_CORE_ASSERT(false, "Unknown image format Image \"{}\"", file->GetPath().ToString());
								goto end;
							}
						}
						else if (bpp == 32) {
							if (blueM == 0xFF0000 && greenM == 0xFF00 && redM == 0xFF) {
#ifdef HZ_LITTLE_ENDIAN
								imageFormat = GL_RGBA;
#else
								imageFormat = GL_BGRA;
#endif
							}
							else if (redM == 0xFF0000 && greenM == 0xFF00 && blueM == 0xFF) {
#ifdef HZ_LITTLE_ENDIAN
								imageFormat = GL_BGRA;
#else
								imageFormat = GL_RGBA;
#endif
							}
							else {
								HZ_CORE_ASSERT(false, "Unknown image format Image \"{}\"", file->GetPath().ToString());
								goto end;
							}
						}
						else {
							HZ_CORE_ASSERT(false, "Unknown bits per pixel {}, Image \"{}\"", bpp, file->GetPath().ToString());
							goto end;
						}
					}
					break;
				case FIT_UINT16:
					imageType = GL_UNSIGNED_SHORT;
					imageFormat = GL_RED;
					break;
				case FIT_RGB16:	//! 48-bit RGB image	: 3 x 16-bit
					imageType = GL_UNSIGNED_SHORT;
					imageFormat = GL_RGB;
					break;
				case FIT_RGBA16://! 64-bit RGBA image	: 4 x 16-bit",
					imageType = GL_UNSIGNED_SHORT;
					imageFormat = GL_RGBA;
					break;
				default:
					HZ_CORE_ASSERT(false, "Unknown image type! {}, Image: \"{}\", bpp {}, R:0x{:x}, G:0x{:x}, B:0x{:x}", (int)type, file->GetPath().ToString(), bpp, redM, greenM, blueM);
					goto end;
				}
			}


			//generate an OpenGL texture ID for this texture
			id = Load2DTexture(width, height, bits, imageFormat, imageType, builder);
		}
	end:
		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(memory);

		timer.Stop().Print("Reading texture took");
		return id;
	}

	GLuint OpenGLUtils::Load2DTexture(uint32_t width, uint32_t height, void* data, int imageFormat, int imageType, TextureBuilder builder)
	{
		GLuint id;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		//store the texture data for OpenGL use
		GLenum gpuFormat = TextureFormatTGLType(builder.GetFormat());
		glTexImage2D(GL_TEXTURE_2D, 0, gpuFormat, width, height, 0, imageFormat, imageType, data);
		if (builder.IsMipmap()) {
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			if (builder.IsAnisotropic()) {
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 4.0f);
			}
		}
		else if (builder.IsNearest())
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (builder.IsClampEdges())
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		return id;
	}


}

