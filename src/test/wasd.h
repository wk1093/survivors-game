#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include <functional>

/*
 * convert this:
 * WasdMovement(aPhysicsObject, speed, WasdMode::ACCELERATION);
 * to
 * if (Engine::isKeyPressed(sf::Keyboard::W)) {
 * test.addAcceleration(0, -speed);
 * }
 * if (Engine::isKeyPressed(sf::Keyboard::A)) {
 * test.addAcceleration(-speed, 0);
 * }
 * if (Engine::isKeyPressed(sf::Keyboard::S)) {
 * test.addAcceleration(0, speed);
 * }
 * if (Engine::isKeyPressed(sf::Keyboard::D)) {
 * test.addAcceleration(speed, 0);
 * }
 * or WasdMove::VELOCITY: addVelocity or WasdMove::POSITION: moveTA
 */

enum class WasdMode {
    ACCELERATION,
    VELOCITY,
    POSITION
};

void WasdMovement(PhysicsObject& po, float speed, WasdMode mode) {
    std::function<void(float, float)> f;
    switch (mode) {
        case WasdMode::ACCELERATION:
            f = [&](float x, float y) { po.addAcceleration(x*speed, y*speed); };
            break;
        case WasdMode::VELOCITY:
            f = [&](float x, float y) { po.addVelocity(x*speed, y*speed); };
            break;
        case WasdMode::POSITION:
            f = [&](float x, float y) { po.moveTA(x*speed, y*speed); };
            break;
    }

    if (Engine::isKeyPressed(sf::Keyboard::W)) {
        f(0, -1);
    }
    if (Engine::isKeyPressed(sf::Keyboard::A)) {
        f(-1, 0);
    }
    if (Engine::isKeyPressed(sf::Keyboard::S)) {
        f(0, 1);
    }
    if (Engine::isKeyPressed(sf::Keyboard::D)) {
        f(1, 0);
    }
}

