// (c) Nikita Rogalev. All rights reserved.

//#include <glad/glad.h>

#include "Scene.h"
#include <algorithm>

#include "GameObject.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Camera/Camera.h"     
#include "Engine/Core/FileSystem/ResourceManager.h"  
#include "Engine/Render/Renderer.h"  

namespace Engine 
{

    Scene::Scene(const std::string& name) : m_name(name) 
    {
        createGameObject("NewObj");
    }

    Scene::~Scene()
    {
        r_camera.reset();
        m_gameObjects.clear();
    }

    GameObject* Scene::createGameObject(const std::string& name) 
    {
        auto object = CreateUniquePtr<GameObject>();
        GameObject* ptr = object.get();
        m_gameObjects.push_back(std::move(object));
        return ptr;
    }

    void Scene::removeGameObject(GameObject* object) 
    {
       // m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),[object](const TUniquePtr<GameObject>& obj) {return obj.get() == object;}),m_gameObjects.end());
    }

    void Scene::removeGameObject(const std::string& name) 
    {
       // m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),[&name](const TUniquePtr<GameObject>& obj) {return obj->getName() == name;}),m_gameObjects.end());
    }

    GameObject* Scene::findGameObject(const std::string& name) 
    {
        /*for (auto& obj : m_gameObjects) 
        {
            if (obj->getName() == name) 
            {
                return obj.get();
            }
        }*/
        return nullptr;
    }

    void Scene::Update(float deltaTime) 
    {
        for (auto& obj : m_gameObjects) 
        {
            obj->Update(deltaTime);
        }
    }

    void Scene::Render() 
    {
        for (auto& obj : m_gameObjects) 
        {
            obj->Render();
        }
    }

    void Scene::Clear() 
    {
        m_gameObjects.clear();
        r_camera = {};
    }

}
