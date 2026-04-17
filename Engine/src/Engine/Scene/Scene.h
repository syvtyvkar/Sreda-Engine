// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Render/Components/Object.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/Camera.h"
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

        void update(float deltaTime);
        void render(RenderAPI* renderer);
        void OnContextRecreated(RenderAPI* renderer);

        void clear();

        const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const 
        { 
            return m_gameObjects; 
        }

        const std::string& getName() const { return m_name; }
        Engine::Camera* GetCamera() { return r_camera.get();};

        void SetParentRender(class RenderAPI* scene) { m_parentRender = scene; }
        class RenderAPI* GetParentRender() const { return m_parentRender; }

    private:
        std::string m_name;
        std::unique_ptr<Engine::Camera> r_camera;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        class RenderAPI* m_parentRender = nullptr;
    };

}