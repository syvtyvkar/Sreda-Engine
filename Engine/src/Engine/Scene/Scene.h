// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "GameObject.h"
#include "Engine/Render/RendererAPI.h"
#include "Engine/Render/Camera/Camera.h"
#include <vector>
#include <memory>
#include <string>

namespace Engine 
{

    class Scene 
    {
    public:
        Scene(const std::string& name = "Scene");
        ~Scene();

        GameObject* createGameObject(const std::string& name = "GameObject");
        void removeGameObject(GameObject* object);
        void removeGameObject(const std::string& name);
        GameObject* findGameObject(const std::string& name);

        void Update(float deltaTime);
        void Render();

        void Clear();

        const std::vector<TUniquePtr<GameObject>>& getGameObjects() const 
        { 
            return m_gameObjects; 
        }

        const std::string& getName() const { return m_name; }
        Engine::Camera* GetCamera() { return r_camera.get();};

    private:
        std::string m_name;
        TUniquePtr<Engine::Camera> r_camera;
        std::vector<TUniquePtr<GameObject>> m_gameObjects;
    };

}