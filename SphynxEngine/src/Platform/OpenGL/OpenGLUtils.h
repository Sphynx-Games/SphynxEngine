#pragma once

#include "Renderer/Buffer.h"
#include <glad/glad.h>


namespace Sphynx
{
	inline static GLenum DataTypeToOpenGLBaseType(DataType type)
	{
		switch (type)
		{
		case DataType::FLOAT:		return GL_FLOAT;
		case DataType::FLOAT2:		return GL_FLOAT;
		case DataType::FLOAT3:		return GL_FLOAT;
		case DataType::FLOAT4:		return GL_FLOAT;
		case DataType::INT:			return GL_INT;
		case DataType::INT2:		return GL_INT;
		case DataType::INT3:		return GL_INT;
		case DataType::INT4:		return GL_INT;
		case DataType::MAT3:		return GL_FLOAT;
		case DataType::MAT4:		return GL_FLOAT;
		case DataType::BOOL:		return GL_BOOL;
		}
		return GL_NONE;
	}
}
