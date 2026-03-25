// (c) Nikita Rogalev. All rights reserved.

#pragma once                    // Защита от множественного включения заголовочного файла

#include <stdint.h> 

class IRHIVertexBuffer;
class IRHIIndexBuffer;
class IRHIPipeline;

class CommandBuffer
{
public:
    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void BindPipeline(IRHIPipeline* pipeline) = 0;
    virtual void BindVertexBuffer(IRHIVertexBuffer* vb) = 0;
    virtual void BindIndexBuffer(IRHIIndexBuffer* ib) = 0;

    virtual void DrawIndexed(uint32_t count) = 0;
};