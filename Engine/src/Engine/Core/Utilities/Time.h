// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <string>
#include "BaseEngine.h"

namespace Engine::Time
{
    struct Timestep 
    {
    public:
        Timestep(float time = 0.0f) : m_Time(time) {}

        /** @brief  Implicit conversion to float */
        operator float() const { return m_Time; }

        /** @brief  Get time in different units */
        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const { return m_Time * 1000.0f; }

        /** @brief  Arithmetic operations */
        Timestep operator+(const Timestep& other) const {return Timestep(m_Time + other.m_Time);}
        Timestep operator-(const Timestep& other) const {return Timestep(m_Time - other.m_Time);}
        Timestep operator*(float scalar) const {return Timestep(m_Time * scalar);}
        Timestep operator/(float scalar) const {return Timestep(m_Time / scalar);}

        /** @brief Comparison */
        bool operator==(const Timestep& other) const {return m_Time == other.m_Time;}
        bool operator!=(const Timestep& other) const {return m_Time != other.m_Time;}
        bool operator<(const Timestep& other) const {return m_Time < other.m_Time;}
        bool operator>(const Timestep& other) const {return m_Time > other.m_Time;}

    private:
        float m_Time;  // Time in seconds
    };

    /**
     * @brief Time management system
     * 
     * Provides access to:
     * - Time since application start
     * - Delta time between frames
     * - FPS
     * - Time scale (for slow-mo)
     * - Fixed timestep for physics
     */
    class TimeSystem 
    {
    public:
        /** @brief Initialization */
        static void Init();
        static void Shutdown();

        /** @brief Update (call every frame) */
        static void Update();

        /** @brief Time Scale (slow/fast motion) */
        static float GetTime();                    // Time since application start (sec)
        static float GetTimeSeconds();             // Alias for GetTime()
        static double GetTimeDouble();             // More precise version

        /** @brief Time Scale (slow/fast motion) */
        static Timestep GetDeltaTime();            // Last frame time
        static float GetDeltaTimeSeconds();        // Delta time in seconds
        static float GetDeltaTimeMilliseconds();   // Delta time in milliseconds

        /** @brief Time Scale (slow/fast motion) */
        static uint32_t GetFPS();                  // Frames per second
        static float GetFrameTime();               // Frame time (ms)

        /** @brief Time Scale (slow/fast motion) */
        static void SetTimeScale(float scale);     // 1.0 = normal, 0.5 = slow-mo
        static float GetTimeScale();               // Current scale
        static void Pause();                       // Set scale = 0
        static void Resume();                      // Restore scale = 1

        /** @brief Fixed Timestep (for physics) */
        static void SetFixedTimestep(float timestep);  // Default 1/60 = 0.01667
        static float GetFixedTimestep();
        static bool ShouldFixedUpdate();               // Is it time for FixedUpdate?
        static void FixedUpdate();                     // Call on physics update

        /** @brief Singleton */
        static TimeSystem& GetInstance();

    private:
        TimeSystem() = default;
        ~TimeSystem() = default;

        /** @brief Internal data */
        static TimeSystem* s_Instance;

        double m_StartTime = 0.0;        // Application start time
        double m_CurrentTime = 0.0;      // Current time
        double m_LastTime = 0.0;         // Last frame time
        double m_DeltaTime = 0.0;        // Delta time (sec)

        /** @brief FPS counter */
        uint32_t m_FrameCount = 0;
        float m_FPS = 0.0f;
        double m_FPSAccumulator = 0.0;
        static constexpr double FPS_UPDATE_INTERVAL = 0.5;  // Update FPS every 0.5 sec

        /** @brief Time scale */
        float m_TimeScale = 1.0f;
        float m_PausedTimeScale = 1.0f;  // For restoring after pause

        /** @brief Fixed timestep */
        float m_FixedTimestep = 1.0f / 60.0f;  // 60 FPS for physics
        double m_FixedTimeAccumulator = 0.0;
    };

     /** 
      * @brief Timer 
      * */
    class Timer 
    {
    public:
        Timer(const std::string& name = "Timer") : m_Name(name) 
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        ~Timer() 
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - m_StartTime).count();
            ENGINE_LOG_TRACE("[TIMER] {0}: {1:.3f} ms", m_Name, duration);
        }

        /** @brief Get elapsed time without destroying the object */
        double GetElapsedMilliseconds() const 
        {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double, std::milli>(now - m_StartTime).count();
        }

        /** @brief Get elapsed time without destroying the object */
        float GetElapsedSeconds() const {return static_cast<float>(GetElapsedMilliseconds() / 1000.0);}

        /** @brief Reset timer */
        void Reset() {m_StartTime = std::chrono::high_resolution_clock::now();}

    private:
        /** @brief Timer name */
        std::string m_Name;
        std::chrono::high_resolution_clock::time_point m_StartTime;
    };

    /**
     * @brief Macro for quick profiling
     * 
     * Example: ENGINE_PROFILE_SCOPE("RenderFrame")
     */
    #define ENGINE_PROFILE_SCOPE(name) Engine::Timer timer##__LINE__(name)
    #define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(__FUNCTION__)
}
