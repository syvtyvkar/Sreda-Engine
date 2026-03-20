// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Time.h"
#include "Engine/Core/Log.h"
#include <chrono>
#include <algorithm>

namespace Engine::Time 
{

    TimeSystem* TimeSystem::s_Instance = nullptr;

    // ========================================================================
    // === ИНИЦИАЛИЗАЦИЯ ===
    // ========================================================================

    void TimeSystem::Init() {
        if (s_Instance) {
            ENGINE_LOG_WARN("Time already initialized!");
            return;
        }

        s_Instance = new TimeSystem();
        
        // Запоминаем время запуска
        auto now = std::chrono::high_resolution_clock::now();
        s_Instance->m_StartTime = std::chrono::duration<double>(
            now.time_since_epoch()).count();
        s_Instance->m_LastTime = s_Instance->m_StartTime;
        s_Instance->m_CurrentTime = s_Instance->m_StartTime;

        ENGINE_LOG_TRACE("Time system initialized");
        ENGINE_LOG_TRACE("Start time: {0}s", s_Instance->m_StartTime);
    }

    void TimeSystem::Shutdown() {
        if (s_Instance) {
            ENGINE_LOG_INFO("Time system shutdown. Final FPS: {0}", s_Instance->m_FPS);
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    // ========================================================================
    // === ОБНОВЛЕНИЕ ===
    // ========================================================================

    void TimeSystem::Update() {
        if (!s_Instance) return;

        // Получаем текущее время
        auto now = std::chrono::high_resolution_clock::now();
        double currentTime = std::chrono::duration<double>(
            now.time_since_epoch()).count();

        // Вычисляем delta time
        s_Instance->m_DeltaTime = currentTime - s_Instance->m_LastTime;
        
        // Ограничиваем delta time (защита от лагов)
        // Если игра зависла на 1 секунду, не хотим чтобы персонаж телепортировался
        s_Instance->m_DeltaTime = std::min(s_Instance->m_DeltaTime, 0.25);
        
        s_Instance->m_LastTime = currentTime;
        s_Instance->m_CurrentTime = currentTime;

        // Применяем time scale
        double scaledDeltaTime = s_Instance->m_DeltaTime * s_Instance->m_TimeScale;

        // === FPS Counter ===
        s_Instance->m_FrameCount++;
        s_Instance->m_FPSAccumulator += s_Instance->m_DeltaTime;

        if (s_Instance->m_FPSAccumulator >= FPS_UPDATE_INTERVAL) {
            s_Instance->m_FPS = static_cast<float>(
                s_Instance->m_FrameCount / s_Instance->m_FPSAccumulator);
            s_Instance->m_FrameCount = 0;
            s_Instance->m_FPSAccumulator = 0.0;
            
            // Логируем FPS каждые 0.5 сек (можно отключить в релизе)
            //ENGINE_LOG_TRACE("FPS: {0}", s_Instance->m_FPS);
        }

        // === Fixed Timestep Accumulator ===
        s_Instance->m_FixedTimeAccumulator += scaledDeltaTime;
    }

    // ========================================================================
    // === ПУБЛИЧНЫЙ API ===
    // ========================================================================

    float TimeSystem::GetTime() {
        return s_Instance ? static_cast<float>(
            s_Instance->m_CurrentTime - s_Instance->m_StartTime) : 0.0f;
    }

    float TimeSystem::GetTimeSeconds() {
        return GetTime();
    }

    double TimeSystem::GetTimeDouble() {
        return s_Instance ? 
            s_Instance->m_CurrentTime - s_Instance->m_StartTime : 0.0;
    }

    Timestep TimeSystem::GetDeltaTime() {
        if (!s_Instance) return Timestep(0.0f);
        return Timestep(static_cast<float>(
            s_Instance->m_DeltaTime * s_Instance->m_TimeScale));
    }

    float TimeSystem::GetDeltaTimeSeconds() {
        return GetDeltaTime().GetSeconds();
    }

    float TimeSystem::GetDeltaTimeMilliseconds() {
        return GetDeltaTime().GetMilliseconds();
    }

    uint32_t TimeSystem::GetFPS() {
        return s_Instance ? static_cast<uint32_t>(s_Instance->m_FPS) : 0;
    }

    float TimeSystem::GetFrameTime() {
        return s_Instance ? 
            static_cast<float>(s_Instance->m_DeltaTime * 1000.0) : 0.0f;
    }

    // === Time Scale ===

    void TimeSystem::SetTimeScale(float scale) {
        if (s_Instance) {
            s_Instance->m_TimeScale = std::max(0.0f, scale);  // Не меньше 0
            ENGINE_LOG_INFO("Time scale set to: {0}", scale);
        }
    }

    float TimeSystem::GetTimeScale() {
        return s_Instance ? s_Instance->m_TimeScale : 1.0f;
    }

    void TimeSystem::Pause() {
        if (s_Instance) {
            s_Instance->m_PausedTimeScale = s_Instance->m_TimeScale;
            s_Instance->m_TimeScale = 0.0f;
            ENGINE_LOG_INFO("Time paused");
        }
    }

    void TimeSystem::Resume() {
        if (s_Instance) {
            s_Instance->m_TimeScale = s_Instance->m_PausedTimeScale;
            ENGINE_LOG_INFO("Time resumed (scale: {0})", s_Instance->m_TimeScale);
        }
    }

    // === Fixed Timestep ===

    void TimeSystem::SetFixedTimestep(float timestep) {
        if (s_Instance && timestep > 0.0f) {
            s_Instance->m_FixedTimestep = timestep;
            ENGINE_LOG_INFO("Fixed timestep set to: {0}s ({1} Hz)", 
                          timestep, 1.0f / timestep);
        }
    }

    float TimeSystem::GetFixedTimestep() {
        return s_Instance ? s_Instance->m_FixedTimestep : 1.0f / 60.0f;
    }

    bool TimeSystem::ShouldFixedUpdate() {
        if (!s_Instance) return false;
        return s_Instance->m_FixedTimeAccumulator >= s_Instance->m_FixedTimestep;
    }

    void TimeSystem::FixedUpdate() {
        if (!s_Instance) return;
        
        if (s_Instance->m_FixedTimeAccumulator >= s_Instance->m_FixedTimestep) {
            s_Instance->m_FixedTimeAccumulator -= s_Instance->m_FixedTimestep;
        }
    }

    // === Singleton ===

    TimeSystem& TimeSystem::GetInstance() {
        ENGINE_ASSERT(s_Instance, "Time not initialized!");
        return *s_Instance;
    }

}