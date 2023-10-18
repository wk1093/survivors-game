#pragma once

#include "../gfx/all_sf.h"
#include "Object.h"
#include "Component.h"

class EntityComponentSystem {
protected:
    Engine& m_engine;
    std::vector<std::vector<std::unique_ptr<Object>>> m_objects;

public:
    explicit EntityComponentSystem(Engine& e): m_engine(e) {
        // there is 5 layers
        m_objects.reserve(5);
    }

    template<typename T, typename... Args>
    T& makeObject(Args&&... args) {
        m_objects[0].emplace_back(std::make_unique<T>(*this, m_engine, std::forward<Args>(args)...));
        return *static_cast<T*>(m_objects[0].back().get());
    }

    // makeObject given layer
    template<typename T, unsigned int N, typename... Args>
    T& makeObject(Args&&... args) {
        if (N >= m_objects.size()) {
            m_objects.resize(N+1);
        }
        m_objects[N].emplace_back(std::make_unique<T>(*this, m_engine, std::forward<Args>(args)...));
        return *static_cast<T*>(m_objects[N].back().get());
    }

    // unmake object
    void unmakeObject(Object& o, int layer = -1) {
        if (layer == -1) {
            // for all layers
            for (auto& c : m_objects) {
                for (auto it = c.begin(); it != c.end(); it++) {
                    if (it->get() == &o) {
                        c.erase(it);
                        return;
                    }
                }
            }
            return;
        }
        if (layer >= m_objects.size()) {
            return;
        }
        for (auto it = m_objects[layer].begin(); it != m_objects[layer].end(); it++) {
            if (it->get() == &o) {
                m_objects[layer].erase(it);
                return;
            }
        }
    }

    // return a list of AABBs of all StaticObjects in the ECS
    std::vector<sf::FloatRect> getStaticObjectAABBs();

    void update() {
        for (auto& c : m_objects) {
            for (auto& o : c) {
                o->update();
            }
        }
    }

    void draw(bool debug = false);


};