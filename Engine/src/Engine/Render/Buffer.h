// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include <unordered_map>

namespace Engine::Render
{
    using namespace Engine;

    /*********************************************************************************
    * SHADERS 
    * 
    *********************************************************************************/

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType InType)
    {
        switch (InType)
        {
            case ShaderDataType::Float      : return 4;
            case ShaderDataType::Float2     : return 4*2;
            case ShaderDataType::Float3     : return 4*3;
            case ShaderDataType::Float4     : return 4*4;
            case ShaderDataType::Mat3       : return 4*3*3;
            case ShaderDataType::Mat4       : return 4*4*4;
            case ShaderDataType::Int        : return 4;
            case ShaderDataType::Int2       : return 4*2;
            case ShaderDataType::Int3       : return 4*3;
            case ShaderDataType::Int4       : return 4*4;
            case ShaderDataType::Bool       : return 1;
        
        default:
            ENGINE_CORE_ASSERT(false, "Unknown shader data type!");
            return 0;
        }
    }

    /*********************************************************************************
    * BUFFERS 
    * 
    *********************************************************************************/


    struct BaseBuffer
    {
        uint8_t* m_data = nullptr;
        uint64_t m_size = 0;

        BaseBuffer() = default;

        BaseBuffer(uint64_t InSize)
        {
            Allocate(InSize);
        }

        BaseBuffer(const BaseBuffer&) = default;

        static BaseBuffer Copy(BaseBuffer InOther)
        {
            BaseBuffer LResult(InOther.m_size);
            memcpy(LResult.m_data,InOther.m_data,InOther.m_size);
            return LResult;
        }

        void Allocate(uint64_t InSize)
        {
            Release();
            m_data = new uint8_t[InSize];
            m_size = InSize;
        }

        void Release()
        {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
        }

        template<typename T>
        T* As()
        {
            return (T*)m_data;
        }

        operator bool() const
        {
            return (bool)m_data;
        }
    };

    struct ScopedBuffer
    {
        ScopedBuffer(BaseBuffer InBuffer) : m_buffer(InBuffer)
        {

        }

        ScopedBuffer(uint64_t InSize) : m_buffer(InSize)
        {

        }

        ~ScopedBuffer()
        {
            m_buffer.Release();
        }

        uint8_t* Data() { return m_buffer.m_data;}
        uint64_t Size() { return m_buffer.m_size; }

        template<typename T>
        T* As()
        {
            return m_buffer.As<T>();
        }

        operator bool() {return m_buffer; }

    private:
        BaseBuffer m_buffer;
    };

    /*********************************************************************************
    * RENDER BUFFERS 
    * 
    *********************************************************************************/

    struct BufferElement
    {
        std::string m_name;
        ShaderDataType m_type = ShaderDataType::Bool;
        uint32_t m_size = 2;
        size_t m_offset;
        bool m_normalized = false;

        BufferElement() = default;
        BufferElement(ShaderDataType InType, const std::string& InName, bool InNormalized = false)
			: m_name(InName), m_type(InType), m_size(ShaderDataTypeSize(InType)), m_offset(0), m_normalized(InNormalized)
        {

        }

        uint32_t GetComponentCount() const
		{
			switch (m_type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3;
				case ShaderDataType::Mat4:    return 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			ENGINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
    };

    class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_Elements)
			{
				element.m_offset = offset;
				offset += element.m_size;
				m_stride += element.m_size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
	};

	// Currently only supports 32-bit index buffers
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}
