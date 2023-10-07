#pragma once

#include "../gfx/all_sf.h"
#include "Object.h"
#include "Component.h"

class EntityComponentSystem {
protected:
    Engine& m_engine;
    std::vector<std::unique_ptr<Object>> m_objects;

public:
    explicit EntityComponentSystem(Engine& e): m_engine(e) { }

    template<typename T, typename... Args>
    T& makeObject(Args&&... args) {
        m_objects.emplace_back(std::make_unique<T>(*this, m_engine, std::forward<Args>(args)...));
        return *static_cast<T*>(m_objects.back().get());
    }

    void update() {
        for (auto& c : m_objects) {
            c->update();
        }
    }

    void draw() {
        for (auto& o : m_objects) {
            o->draw();
        }
    }


};