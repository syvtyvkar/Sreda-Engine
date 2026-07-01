// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"

namespace Engine::Render
{
    class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		
		static TRef<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};
}
