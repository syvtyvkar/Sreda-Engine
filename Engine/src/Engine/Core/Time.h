// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <string>
#include "Engine/Core/Log.h"

namespace Engine::Time
{
    struct Timestep {
    public:
        Timestep(float time = 0.0f) : m_Time(time) {}

        // Неявное преобразование к float
        operator float() const { return m_Time; }

        // Получение времени в разных единицах
        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f; }

        // Арифметические операции
        Timestep operator+(const Timestep& other) const {
            return Timestep(m_Time + other.m_Time);
        }

        Timestep operator-(const Timestep& other) const {
            return Timestep(m_Time - other.m_Time);
        }

        Timestep operator*(float scalar) const {
            return Timestep(m_Time * scalar);
        }

        Timestep operator/(float scalar) const {
            return Timestep(m_Time / scalar);
        }

        // Сравнение
        bool operator==(const Timestep& other) const {
            return m_Time == other.m_Time;
        }

        bool operator!=(const Timestep& other) const {
            return m_Time != other.m_Time;
        }

        bool operator<(const Timestep& other) const {
            return m_Time < other.m_Time;
        }

        bool operator>(const Timestep& other) const {
            return m_Time > other.m_Time;
        }

    private:
        float m_Time;  // Время в секундах
    };

    /**
     * Система управления временем
     * 
     * Предоставляет доступ к:
     * - Времени с начала приложения
     * - Delta time между кадрами
     * - FPS
     * - Масштабу времени (для slow-mo)
     * - Fixed timestep для физики
     */
    class TimeSystem {
    public:
        // === Инициализация ===
        static void Init();
        static void Shutdown();

        // === Обновление (вызывать каждый кадр) ===
        static void Update();

        // === Основное время ===
        static float GetTime();                    // Время с начала приложения (сек)
        static float GetTimeSeconds();             // Алиас для GetTime()
        static double GetTimeDouble();             // Более точная версия

        // === Delta Time ===
        static Timestep GetDeltaTime();            // Время последнего кадра
        static float GetDeltaTimeSeconds();        // Delta time в секундах
        static float GetDeltaTimeMilliseconds();   // Delta time в миллисекундах

        // === FPS ===
        static uint32_t GetFPS();                  // Кадров в секунду
        static float GetFrameTime();               // Время кадра (мс)

        // === Time Scale (замедление/ускорение времени) ===
        static void SetTimeScale(float scale);     // 1.0 = нормально, 0.5 = slow-mo
        static float GetTimeScale();               // Текущий масштаб
        static void Pause();                       // Установить scale = 0
        static void Resume();                      // Восстановить scale = 1

        // === Fixed Timestep (для физики) ===
        static void SetFixedTimestep(float timestep);  // По умолчанию 1/60 = 0.01667
        static float GetFixedTimestep();
        static bool ShouldFixedUpdate();               // Пора ли делать FixedUpdate?
        static void FixedUpdate();                     // Вызвать при обновлении физики

        // === Singleton ===
        static TimeSystem& GetInstance();

    private:
        TimeSystem() = default;
        ~TimeSystem() = default;

        // === Внутренние данные ===
        static TimeSystem* s_Instance;

        double m_StartTime = 0.0;        // Время запуска приложения
        double m_CurrentTime = 0.0;      // Текущее время
        double m_LastTime = 0.0;         // Время прошлого кадра
        double m_DeltaTime = 0.0;        // Delta time (сек)
        
        // FPS counter
        uint32_t m_FrameCount = 0;
        float m_FPS = 0.0f;
        double m_FPSAccumulator = 0.0;
        static constexpr double FPS_UPDATE_INTERVAL = 0.5;  // Обновлять FPS каждые 0.5 сек

        // Time scale
        float m_TimeScale = 1.0f;
        float m_PausedTimeScale = 1.0f;  // Для восстановления после паузы

        // Fixed timestep
        float m_FixedTimestep = 1.0f / 60.0f;  // 60 FPS для физики
        double m_FixedTimeAccumulator = 0.0;
    };

    class Timer {
    public:
        Timer(const std::string& name = "Timer") 
            : m_Name(name) {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        ~Timer() {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(
                endTime - m_StartTime).count();
            
            ENGINE_LOG_TRACE("[TIMER] {0}: {1:.3f} ms", m_Name, duration);
        }

        // Получить время без уничтожения объекта
        double GetElapsedMilliseconds() const {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double, std::milli>(
                now - m_StartTime).count();
        }

        float GetElapsedSeconds() const {
            return static_cast<float>(GetElapsedMilliseconds() / 1000.0);
        }

        // Сброс таймера
        void Reset() {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

    private:
        std::string m_Name;
        std::chrono::high_resolution_clock::time_point m_StartTime;
    };

    /**
     * @brief Макрос для быстрого профилирования
     * 
     * Пример: ENGINE_PROFILE_SCOPE("RenderFrame")
     */
    #define ENGINE_PROFILE_SCOPE(name) Engine::Timer timer##__LINE__(name)
    #define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(__FUNCTION__)
}