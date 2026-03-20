// (c) Nikita Rogalev. All rights reserved.

//#include <glad/glad.h>

#include "Scene.h"
#include <algorithm>

#include "Engine/Render/Components/Object.h"
#include "Engine/Input/Input.h"

namespace Engine 
{

    Scene::Scene(const std::string& name) : m_name(name) 
    {
        r_camera = std::make_unique<Engine::Camera>(camConfig);
    }

    Scene::~Scene()
    {
        r_camera.reset();
        m_gameObjects.clear();
    }

    GameObject* Scene::createGameObject(const std::string& name) 
    {
        auto object = std::make_unique<GameObject>(name, this);

        object.get()->SetParentScene(this);

        GameObject* ptr = object.get();
        m_gameObjects.push_back(std::move(object));
        return ptr;
    }

    void Scene::removeGameObject(GameObject* object) 
    {
        m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),[object](const std::unique_ptr<GameObject>& obj) {return obj.get() == object;}),m_gameObjects.end());
    }

    void Scene::removeGameObject(const std::string& name) 
    {
        m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),[&name](const std::unique_ptr<GameObject>& obj) {return obj->getName() == name;}),m_gameObjects.end());
    }

    GameObject* Scene::findGameObject(const std::string& name) 
    {
        for (auto& obj : m_gameObjects) 
        {
            if (obj->getName() == name) 
            {
                return obj.get();
            }
        }
        return nullptr;
    }

    void Scene::update(float deltaTime) 
    {
        for (auto& obj : m_gameObjects) 
        {
            obj->update(deltaTime);
        }

        if (Engine::InputSystem::IsKeyPressed(InputKey::W))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Forward, deltaTime);
        }
        if (Engine::InputSystem::IsKeyPressed(InputKey::S))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Backward, deltaTime);
        }
        if (Engine::InputSystem::IsKeyPressed(InputKey::A))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Left, deltaTime);
        }
        if (Engine::InputSystem::IsKeyPressed(InputKey::D))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Right, deltaTime);
        }
        if (Engine::InputSystem::IsKeyPressed(InputKey::Q))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Down, deltaTime);
        }
        if (Engine::InputSystem::IsKeyPressed(InputKey::E))
        {
            r_camera.get()->processKeyboard(Engine::CameraMovement::Up, deltaTime);
        }
    }

    void Scene::render(RenderAPI* renderer) 
    {
        for (auto& obj : m_gameObjects) 
        {
            obj->render(renderer);
        }
    }

    void Scene::clear() 
    {
        m_gameObjects.clear();
        r_camera = {};
        camConfig = {};
    }

}