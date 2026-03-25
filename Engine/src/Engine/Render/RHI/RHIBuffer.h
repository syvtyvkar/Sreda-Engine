// (c) Nikita Rogalev. All rights reserved.

#pragma once            // Защита от множественного включения заголовочного файла

#include <cstdint>      //целочисленные типы данных с фиксированным размером.

class IRHIVertexBuffer
{
public:
    virtual ~IRHIVertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

class IRHIIndexBuffer
{
public:
    virtual ~IRHIIndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual uint32_t GetCount() const = 0;
};