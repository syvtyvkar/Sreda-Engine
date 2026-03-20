// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Component.h"
#include "Engine/Core/Types.h"
#include "Engine/Render/Components/Mesh.h"
#include "Engine/Render/Components/Material.h"
#include <vector>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Engine { class Scene; }

namespace Engine {

    class GameObject {
    public:
        GameObject(const std::string& name = "GameObject", Scene* scene = nullptr);
        ~GameObject();

        // Добавить компонент
        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            component->setGameObject(this);
            T* ptr = component.get();
            m_components[typeid(T)] = std::move(component);
            ptr->onStart();
            return ptr;
        }

        // Получить компонент
        template<typename T>
        T* getComponent() {
            auto it = m_components.find(typeid(T));
            if (it != m_components.end()) {
                return dynamic_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        // Transform — отдельное поле (не компонент)
        void SetTransform(Transform InTransformNew) { transform = InTransformNew; }
        Transform GetTransform() const { return transform; }
        Transform transform;

        void update(float deltaTime);
        void render(class RenderAPI* renderer);

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        void setActive(bool active) { m_active = active; }
        bool isActive() const { return m_active; }

        void SetParentScene(Scene* scene) { m_parentScene = scene; }
        Scene* GetParentScene() const { return m_parentScene; }

    private:
        std::string m_name;
        bool m_active = true;
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;


        Scene* m_parentScene = nullptr;
    };

}