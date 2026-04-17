// (c) Nikita Rogalev. All rights reserved.

#pragma once                                    // Защита от множественного включения

#include "Engine/Core/Utilities/Types.h"                  // Основные типы движка (Color, Transform и др.)
#include "Engine/Render/Components/Mesh.h"      // Класс меша (геометрия)
#include "Engine/Render/Components/Material.h"  // Класс материала (шейдеры, текстуры, параметры)
#include <memory>                               // Для std::unique_ptr
#include <string>                               // Для std::string

namespace Engine 
{

    /**
     * @class RenderAPI
     * @brief Абстрактный интерфейс для графического рендерера.
     *
     * Определяет набор методов, которые должны быть реализованы для конкретного
     * графического API (например, OpenGL, Vulkan, DirectX). Позволяет отрисовывать
     * меши с материалами, управлять состоянием рендеринга и настройками окна.
     */
    class RenderAPI 
    {
    public:
        virtual ~RenderAPI() = default;

        // Инициализация
        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void beginFrame() = 0;   // Подготовка к кадру
        virtual void endFrame() = 0;     // Present, swap buffers

        /**
         * @brief Устанавливает область вывода (viewport).
         * @param x, y Левый нижний угол области (в пикселях).
         * @param width, height Ширина и высота области.
         */
        virtual void setViewport(int x, int y, int width, int height) = 0;
        /**
         * @brief Задаёт цвет, которым будет очищаться экран при вызове clear().
         * @param color Цвет в формате RGBA (компоненты от 0 до 1).
         */
        virtual void setClearColor(const Color& color) = 0;
        /**
         * @brief  Очищает буферы цвета (и глубины, если включено) согласно установленному цвету.
         */
        virtual void clear() = 0;

        // === Отрисовка ===
        /*virtual void drawPrimitive(PrimitiveType type, unsigned int vertexCount, unsigned int instanceCount = 1) = 0;
        virtual void drawIndexed(unsigned int indexCount, unsigned int instanceCount = 1) = 0;

        // === Состояние ===
        virtual void enableDepthTest(bool enable) = 0;
        virtual void enableBlending(bool enable) = 0;
        virtual void setCullMode(CullMode mode) = 0;*/

        /**
         * @brief Отрисовывает материал (примитив) с заданной трансформацией.
         * @param InMaterial Указатель на материал, определяющий шейдеры и параметры.
         * @param InTransform Трансформация (позиция, поворот, масштаб) для объекта.
         */
        virtual void drawMaterial(Material* InMaterial, Transform InTransform) = 0;
        /**
         * @brief Отрисовывает меш с заданной трансформацией.
         * @param InMesh Указатель на меш (вершины, индексы).
         * @param InTransform Трансформация объекта.
         */
        virtual void drawMesh(Mesh* InMesh, Transform InTransform) = 0;
        /**
         * @brief Подготавливает меш к отрисовке (загружает буферы, настраивает VAO и т.д.).
         * @param InMesh Указатель на меш.
         * @param InTransform Трансформация
         */
        virtual void setupMesh(Mesh* InMesh, Transform InTransform) = 0;
        /**
         * @brief Удаляет меш из памяти рендерера (освобождает связанные ресурсы).
         * @param InMesh Указатель на меш.
         * @param InTransform 
         */
        virtual void removeMesh(Mesh* InMesh, Transform InTransform) = 0;

        // Получить текущий API
        virtual std::string getName() const = 0;
        //virtual const RendererCapabilities& getCapabilities() const = 0;
        // === Получение нативного объекта ===
        //virtual void* getNativeDevice() const = 0;

        /**
         * @brief Глобальное состояние камеры
         */
        virtual void setViewProjection(const Matrix4& view, const Matrix4& proj) = 0;

        virtual void setDepthTest(bool enabled) = 0;
        virtual void setBlending(bool enabled) = 0;
    };

    
    /**
     * @class RenderAPIFactory
     * @brief Фабрика для создания экземпляра рендерера в зависимости от платформы/конфигурации.
     *
     * Скрывает детали конкретной реализации за единым интерфейсом RenderAPI.
     * В зависимости от макросов  возвращает соответствующий объект.
     */
    class RenderAPIFactory 
    {
    public:
    /**
         * @brief Создаёт уникальный указатель на объект рендерера.
         * @return std::unique_ptr<RenderAPI> Указатель на созданный рендерер,
         *         либо nullptr, если ни одна реализация не доступна.
         */
        static std::unique_ptr<RenderAPI> create();
    };

}