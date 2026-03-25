// (c) Nikita Rogalev. All rights reserved.

//#include <glad/glad.h>

#include "Object.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/Tools/MeshFactory"
#include "Engine/Render/Components/Mesh.h"
#include "Engine/Core/FileSystem/ResourceManager.h"
#include "Engine/Render/Scene.h"
#include "Engine/Core/Log.h"

namespace Engine {

    GameObject::GameObject(const std::string& name, Scene* scene) : m_name(name), m_parentScene(scene)
    {
        if (!scene)
        {
            ENGINE_LOG_ERROR("Error create {} mesh! No valid scene!!!");
            return;
        }
        std::unique_ptr<Mesh> LMesh = MeshFactory::CreateCube();
        LMesh.get()->setGameObject(this);
        if (LMesh.get()->m_materialMesh)
        {
            LMesh.get()->m_materialMesh.get()->loadShader(Engine::ResourceManager::getResourcePath("Resources/Shaders/camera.vert"), Engine::ResourceManager::getResourcePath("Resources/Shaders/camera.frag"));
        }
        Transform LTansform = Transform();
        LTansform.scale = Vector3(1,1,1);
        LTansform.position = Vector3(0,0,-5);
        LMesh.get()->SetTransform(LTansform);
        scene->GetParentRender()->setupMesh(LMesh.get(), LTansform);
        Mesh* ptr = LMesh.get();
        m_components[typeid(Mesh)] = std::move(LMesh);
        ptr->onStart();
    }

    GameObject::~GameObject()
    {
        m_parentScene = nullptr;
        m_components.clear();
    };

    void GameObject::update(float deltaTime) 
    {
        if (!m_active) return;

        for (auto& [type, component] : m_components) {
            component->onUpdate(deltaTime);
        }
    }

    void GameObject::render(RenderAPI* renderer) {
        if (!m_active || !renderer) return;

        auto* mesh = getComponent<Mesh>();

        if (mesh && mesh->m_materialMesh) {
            //renderer->drawMaterial(mesh->m_materialMesh.get(),transform);
            if (GetParentScene())
            {
                mesh->m_materialMesh.get()->use(GetParentScene()->GetCamera());
                renderer->drawMesh(mesh,transform);
            }
        }
    }

}