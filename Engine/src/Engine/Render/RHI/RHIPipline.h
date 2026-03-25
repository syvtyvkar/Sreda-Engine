// (c) Nikita Rogalev. All rights reserved.

#pragma once                    // Защита от множественного включения заголовочного файла

struct PipelineDesc
{
    bool depthTest = true;
    bool blending = false;
};

class IRHIPipeline
{
public:
    virtual ~IRHIPipeline() = default;

    virtual void Bind() const = 0;
};