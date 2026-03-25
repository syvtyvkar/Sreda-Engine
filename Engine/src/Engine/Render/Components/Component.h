// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Log.h"

namespace Engine {

    class GameObject;

    class Component 
    {
    public:
        virtual ~Component() = default;
        
        virtual void onStart() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}

        GameObject* getGameObject() { return m_gameObject; }
        void setGameObject(GameObject* obj) { m_gameObject = obj; }

    protected:
        GameObject* m_gameObject = nullptr;
        friend class GameObject;
    };

}