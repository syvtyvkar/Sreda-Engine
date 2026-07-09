// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include <string>

namespace Engine::Render
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void MakeCurrent() = 0;
		virtual void BeginFrame() {}
		virtual void EndFrame() {}
	};
}
