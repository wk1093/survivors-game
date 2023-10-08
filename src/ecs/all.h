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