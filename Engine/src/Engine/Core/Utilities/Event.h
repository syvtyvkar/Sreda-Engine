// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard for header file

#include <string>
#include "Engine/Core/Utilities/Log.h"                    // Logging system include
#include <cstdint>                              // For fixed-width integer types
#include <functional>                           // For std::function
#include <memory>                               // For smart pointers
#include <vector>                               // For std::vector
#include <unordered_map>                        // For std::unordered_map
#include <mutex>                                // For std::mutex, std::lock_guard
#include <atomic>                               // For std::atomic

namespace Engine
{
    using DelegateHandle = uint64_t;                        // Type for unique subscription ID
    constexpr DelegateHandle InvalidDelegateHandle = 0;     // Constant representing an invalid handle

    /**
     * Multicast delegate (Observer design pattern).
     * Allows subscribing to events and notifying all subscribers.
     * 
     * Args - parameter types passed on invocation.
     * 
     * Example:
     * 
     * // Declaration
     * Delegate<int, float> OnDamage;
     * 
     * // Subscription
     * OnDamage.Subscribe([](int damage, float multiplier) {YOUR CODE;});
     * 
     * // Invocation
     * OnDamage.Broadcast(50, 2.0f);
     * 
     * Thread-safe implementation (uses mutex).
     * Supports priority subscription.
     */

    template<typename... Args>
    class Delegate 
    {
    public:
        using CallbackType = std::function<void(Args...)>;          // Handler function type

        Delegate() = default;
        ~Delegate() = default;

        DelegateHandle Subscribe(CallbackType callback) // Adds a handler without priority, returns unique handle for unsubscription
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            DelegateHandle handle = s_NextHandle.fetch_add(1);                          // Generate new unique handle
            m_Callbacks[handle] = std::move(callback);
            return handle;
        }

        DelegateHandle SubscribeWithPriority(CallbackType callback, int priority = 0) // Adds a handler with specified priority (higher number = earlier execution)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            DelegateHandle handle = s_NextHandle.fetch_add(1);
            m_Callbacks[handle] = std::move(callback);
            m_Priorities[handle] = priority;
            SortByPriority();                                                           // Re-sort call list by priority
            return handle;
        }

        void Unsubscribe(DelegateHandle handle)                                         // Removes a handler by its handle
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Callbacks.erase(handle);
            m_Priorities.erase(handle);
        }

        void Broadcast(Args... args)  // Notifies all subscribers, passing them args... Exceptions in handlers are ignored (caught and suppressed)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto callbacksCopy = m_Callbacks;
            for (const auto& [handle, callback] : callbacksCopy) {
                if (callback) {
                    try {
                        callback(args...);
                    } catch (...) {
                        // Ignore errors in callbacks
                    }
                }
            }
        }

        void Clear()  // Removes all subscribers
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Callbacks.clear();
            m_Priorities.clear();
        }

        size_t GetSubscriberCount() const // Returns the number of current subscribers
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Callbacks.size();
        }

        bool HasSubscribers() const {return GetSubscriberCount() > 0;} // Checks if there is at least one subscriber

        // Operators for convenience
        void operator()(Args... args) { Broadcast(args...); }                               // Call as function
        DelegateHandle operator+=(CallbackType callback) { return Subscribe(callback); }    // Add subscriber via +=
        void operator-=(DelegateHandle handle) { Unsubscribe(handle); }                     // Remove via -=


        template<typename T>
        DelegateHandle Subscribe(T* object, void (T::*method)(Args...))
        {
            return Subscribe([object, method](Args... args) { (object->*method)(args...); });
        }

        // Подписка на константный метод-член с сигнатурой void(Args...) const
        template<typename T>
        DelegateHandle Subscribe(const T* object, void (T::*method)(Args...) const)
        {
            return Subscribe([object, method](Args... args) { (object->*method)(args...); });
        }

    private:
        void SortByPriority() // Helper method to sort m_Callbacks according to priorities
        {
            std::vector<std::pair<DelegateHandle, int>> sortedHandles;
            for (const auto& [handle, priority] : m_Priorities) {
                sortedHandles.emplace_back(handle, priority);
            }
            std::sort(sortedHandles.begin(), sortedHandles.end(),[](const auto& a, const auto& b) { return a.second > b.second; }); // Sort by descending priority
            
            decltype(m_Callbacks) sortedCallbacks;                  // Create a new ordered callback map
            for (const auto& [handle, priority] : sortedHandles) 
            {
                if (m_Callbacks.find(handle) != m_Callbacks.end()) 
                {
                    sortedCallbacks[handle] = m_Callbacks[handle];
                }
            }
            m_Callbacks = std::move(sortedCallbacks);
        }

        std::unordered_map<DelegateHandle, CallbackType> m_Callbacks;   // Handler storage by handle
        std::unordered_map<DelegateHandle, int> m_Priorities;           // Priorities for each handle
        mutable std::mutex m_Mutex;                                     // Mutex for thread safety
        static std::atomic<DelegateHandle> s_NextHandle;                // Unique handle generator
    };

    template<typename... Args>  // Initialize static handle counter (start at 1, since 0 is reserved as Invalid)
    std::atomic<DelegateHandle> Delegate<Args...>::s_NextHandle{1};

/*********************************************************/
// Set of macros for convenient declaration of specific delegate types (singletons) with different parameter counts.

    // Delegate with no parameters (singleton)
    #define ADD_DELEGATE(DelegateName) \
        class DelegateName : public Engine::Delegate<> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<>; \
        };

    // Delegate with one parameter (singleton)
    #define ADD_DELEGATE_ONE_PARAM(DelegateName, ParamType1) \
        class DelegateName : public Engine::Delegate<ParamType1> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1>; \
            using Param1 = ParamType1; \
        };

    // Delegate with two parameters (singleton)
    #define ADD_DELEGATE_TWO_PARAMS(DelegateName, ParamType1, ParamType2) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
        };

    // Delegate with three parameters (singleton)
    #define ADD_DELEGATE_THREE_PARAMS(DelegateName, ParamType1, ParamType2, ParamType3) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2, ParamType3> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2, ParamType3>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
            using Param3 = ParamType3; \
        };

    // Delegate with four parameters (singleton)
    #define ADD_DELEGATE_FOUR_PARAMS(DelegateName, ParamType1, ParamType2, ParamType3, ParamType4) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2, ParamType3, ParamType4> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2, ParamType3, ParamType4>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
            using Param3 = ParamType3; \
            using Param4 = ParamType4; \
        };

/****************************************************************/

    // Macros for binding class methods to delegates (use std::bind)
    #define BIND_DELEGATE_METHOD(DelegateClass, Method) \
        std::bind(&std::decay_t<decltype(*this)>::Method, this, \
                  std::placeholders::_1, std::placeholders::_2, \
                  std::placeholders::_3, std::placeholders::_4)

    // Macro for creating lambda expressions with captures (simply passes the capture list)
    #define BIND_DELEGATE_LAMBDA(...) \
        [__VA_ARGS__]

    // Macro for declaring events (not singleton, just delegate alias)
    #define ADD_EVENT(ClassType, EventName, ...) \
        class EventName : public Engine::Delegate<__VA_ARGS__> { \
        public: \
            using Super = Engine::Delegate<__VA_ARGS__>; \
        };

    // Macros for declaring multicast delegates (synonyms for ADD_DELEGATE_*, but with different macro name)
    #define ADD_MULTICAST_DELEGATE(DelegateName) \
        class DelegateName : public Engine::Delegate<> { \
        public: \
            using Super = Engine::Delegate<>; \
        };

    #define ADD_MULTICAST_DELEGATE_ONE_PARAM(DelegateName, ParamType1) \
        class DelegateName : public Engine::Delegate<ParamType1> { \
        public: \
            using Super = Engine::Delegate<ParamType1>; \
            using Param1 = ParamType1; \
        };

    #define ADD_MULTICAST_DELEGATE_TWO_PARAMS(DelegateName, ParamType1, ParamType2) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2> { \
        public: \
            using Super = Engine::Delegate<ParamType1, ParamType2>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
        };

}
