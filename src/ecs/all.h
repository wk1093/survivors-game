#pragma once

#include "Object.h"
#include "Component.h"
#include "EntityComponentSystem.h"
#include "Components.h"
#include "Objects.h"

std::vector<sf::FloatRect> EntityComponentSystem::getStaticObjectAABBs() {
    std::vector<sf::FloatRect> aabbs;
    for (auto& o : m_objects) {
        // if the unique_ptr is a StaticObject
        if (auto so = dynamic_cast<StaticObject*>(o.get())) {
            aabbs.push_back(so->getAABB());
        }
    }
    return aabbs;

}

void EntityComponentSystem::draw(bool debug) {
    for (auto& o : m_objects) {
        o->draw();
        if (debug) {
            if (dynamic_cast<StaticObject*>(o.get())) {
                RectangleShape r;
                r.setSize(Vector2f(o->getGlobalBounds().width, o->getGlobalBounds().height));
                r.setPosition(o->getGlobalBounds().left, o->getGlobalBounds().top);
                r.setFillColor(Color(100, 0, 0, 100));
                m_engine.draw(r);
                continue;
            }
        }
    }
}