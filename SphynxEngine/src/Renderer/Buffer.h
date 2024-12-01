#pragma once

#include "Core/Core.h"
#include <string>
#include <vector>


namespace Sphynx
{
	enum class SPHYNX_API DataType
	{
		NONE = 0,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		INT, INT2, INT3, INT4,
		MAT3, MAT4,
		BOOL
	};

	static uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
		case DataType::FLOAT:		return sizeof(float);
		case DataType::FLOAT2:		return sizeof(float) * 2;
		case DataType::FLOAT3:		return sizeof(float) * 3;
		case DataType::FLOAT4:		return sizeof(float) * 4;
		case DataType::INT:			return sizeof(int32_t);
		case DataType::INT2:		return sizeof(int32_t) * 2;
		case DataType::INT3:		return sizeof(int32_t) * 3;
		case DataType::INT4:		return sizeof(int32_t) * 4;
		case DataType::MAT3:		return sizeof(float) * 3 * 3;
		case DataType::MAT4:		return sizeof(float) * 4 * 4;
		case DataType::BOOL:		return sizeof(bool);
		}

		return 0;
	}

	struct SPHYNX_API LayoutElement
	{
		std::string Name;
		DataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		LayoutElement(DataType type, const std::string& name, bool normalize = false) :
			Name(name),
			Type(type),
			Size(DataTypeSize(type)),
			Offset(0),
			Normalized(normalize)
		{

		}

		uint32_t GetCount() const
		{
			switch (Type)
			{
			case DataType::FLOAT:		return 1;
			case DataType::FLOAT2:		return 2;
			case DataType::FLOAT3:		return 3;
			case DataType::FLOAT4:		return 4;
			case DataType::INT:			return 1;
			case DataType::INT2:		return 2;
			case DataType::INT3:		return 3;
			case DataType::INT4:		return 4;
			case DataType::MAT3:		return 3 * 3;
			case DataType::MAT4:		return 4 * 4;
			case DataType::BOOL:		return 1;
			}

			return 0;
		}

	};

	class SPHYNX_API BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<LayoutElement>& elements) :
			m_Elements(elements),
			m_Stride(0)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<LayoutElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<LayoutElement>::iterator end() { return m_Elements.end(); }

		std::vector<LayoutElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<LayoutElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (LayoutElement& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<LayoutElement> m_Elements;
		uint32_t m_Stride;
	};

	class SPHYNX_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(void* vertices, size_t size, const BufferLayout& layout);
	};

	class SPHYNX_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}
