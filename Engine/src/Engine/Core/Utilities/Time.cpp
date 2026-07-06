// (c) Nikita Rogalev. All rights reserved.

#include "Time.h"
#include "BaseEngine.h"
#include <chrono>
#include <algorithm>

#include "Engine/Core/Base/EngineCore.h"

namespace Engine::Time 
{
    using namespace Engine;

    // ========================================================================
    // === INITIALIZATION ===
    // ========================================================================

    void TimeSystem::Init() 
    {
        // Record startup time
        auto now = std::chrono::high_resolution_clock::now();
        m_StartTime = std::chrono::duration<double>(
            now.time_since_epoch()).count();
        m_LastTime = m_StartTime;
        m_CurrentTime = m_StartTime;

        ENGINE_LOG_TRACE("Time system initialized");
        ENGINE_LOG_TRACE("Start time: {0}s", m_StartTime);
    }

    void TimeSystem::Shutdown() 
    {
        ENGINE_LOG_INFO("Time system shutdown. Final FPS: {0}", m_FPS);
    }

    // ========================================================================
    // === UPDATE ===
    // ========================================================================

    void TimeSystem::Update() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) ENGINE_ASSERT(false, "TimeSystem crash!");

        // Get current time
        auto now = std::chrono::high_resolution_clock::now();
        double currentTime = std::chrono::duration<double>(
            now.time_since_epoch()).count();

        // Calculate delta time
        ctx.GetTimeSystem()->m_DeltaTime = currentTime - ctx.GetTimeSystem()->m_LastTime;
        
        // Clamp delta time (protection against lag spikes)
        // If the game froze for 1 second, we don't want the character to teleport
        ctx.GetTimeSystem()->m_DeltaTime = std::min(ctx.GetTimeSystem()->m_DeltaTime, 0.25);
        
        ctx.GetTimeSystem()->m_LastTime = currentTime;
        ctx.GetTimeSystem()->m_CurrentTime = currentTime;

        // Apply time scale
        double scaledDeltaTime = ctx.GetTimeSystem()->m_DeltaTime * ctx.GetTimeSystem()->m_TimeScale;

        // === FPS Counter ===
        ctx.GetTimeSystem()->m_FrameCount++;
        ctx.GetTimeSystem()->m_FPSAccumulator += ctx.GetTimeSystem()->m_DeltaTime;

        if (ctx.GetTimeSystem()->m_FPSAccumulator >= FPS_UPDATE_INTERVAL) 
        {
            ctx.GetTimeSystem()->m_FPS = static_cast<float>(ctx.GetTimeSystem()->m_FrameCount / ctx.GetTimeSystem()->m_FPSAccumulator);
            ctx.GetTimeSystem()->m_FrameCount = 0;
            ctx.GetTimeSystem()->m_FPSAccumulator = 0.0;
            //ENGINE_LOG_TRACE("FPS: {0}", s_Instance->m_FPS);
        }
        ctx.GetTimeSystem()->m_FixedTimeAccumulator += scaledDeltaTime;
    }

    // ========================================================================
    // === PUBLIC API ===
    // ========================================================================

    float TimeSystem::GetTime() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return 0.f;
        return static_cast<float>(ctx.GetTimeSystem()->m_CurrentTime - ctx.GetTimeSystem()->m_StartTime);
    }

    double TimeSystem::GetTimeDouble() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return 0.0;
        return ctx.GetTimeSystem()->m_CurrentTime - ctx.GetTimeSystem()->m_StartTime;
    }

    Timestep TimeSystem::GetDeltaTime() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return Timestep(0.0f);
        return Timestep(static_cast<float>(ctx.GetTimeSystem()->GetDeltaTimeSeconds()));
    }

    uint32_t TimeSystem::GetFPS() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return 0;
        return static_cast<uint32_t>(ctx.GetTimeSystem()->m_FPS);
    }

    float TimeSystem::GetFrameTime() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return 0.f;
        return static_cast<float>(ctx.GetTimeSystem()->m_DeltaTime * 1000.0);
    }

    // === Time Scale ===

    void TimeSystem::SetTimeScale(float scale) 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return;
        ctx.GetTimeSystem()->m_TimeScale = std::max(0.0f, scale);  // Not less than 0
        ENGINE_LOG_INFO("Time scale set to: {0}", scale);
    }

    float TimeSystem::GetTimeScale() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return 1.f;
        return ctx.GetTimeSystem()->m_TimeScale;
    }

    void TimeSystem::Pause() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return;
        ctx.GetTimeSystem()->m_PausedTimeScale = ctx.GetTimeSystem()->m_TimeScale;
        ctx.GetTimeSystem()->m_TimeScale = 0.0f;
        ENGINE_LOG_INFO("Time paused");
    }

    void TimeSystem::Resume() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return;
        ctx.GetTimeSystem()->m_TimeScale = ctx.GetTimeSystem()->m_PausedTimeScale;
        ENGINE_LOG_INFO("Time resumed (scale: {0})", ctx.GetTimeSystem()->m_TimeScale);
    }

    // === Fixed Timestep ===

    void TimeSystem::SetFixedTimestep(float timestep) 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) return;
        if (timestep > 0.0f) 
        {
            ctx.GetTimeSystem()->m_FixedTimestep = timestep;
            ENGINE_LOG_INFO("Fixed timestep set to: {0}s ({1} Hz)", timestep, 1.0f / timestep);
        }
    }

    float TimeSystem::GetFixedTimestep() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) ENGINE_ASSERT(false, "TimeSystem no valid!");
        return ctx.GetTimeSystem()->m_FixedTimestep / 60.0f;
    }

    bool TimeSystem::ShouldFixedUpdate() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) ENGINE_ASSERT(false, "TimeSystem no valid!");
        return ctx.GetTimeSystem()->m_FixedTimeAccumulator >= ctx.GetTimeSystem()->m_FixedTimestep;
    }

    void TimeSystem::FixedUpdate() 
    {
        auto& ctx = EngineCore::GetEngineContext();
        if (!ctx.GetTimeSystem()) ENGINE_ASSERT(false, "TimeSystem no valid!");
        
        if (ctx.GetTimeSystem()->m_FixedTimeAccumulator >= ctx.GetTimeSystem()->m_FixedTimestep) {
            ctx.GetTimeSystem()->m_FixedTimeAccumulator -= ctx.GetTimeSystem()->m_FixedTimestep;
        }
    }
}
