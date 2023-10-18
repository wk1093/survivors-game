#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include "../map/all.h"
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

struct MovementState {
    uint8_t last = 2; // 0 = up, 1 = left, 2 = down, 3 = right
};

// T can be PhysicsObject or AnimatedPhysicsObject
template<typename T>
void WasdMovement(MovementState& state, Engine& e, T& po, float speed, WasdMode mode, bool animate = false) {
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

    if (e.isKeyPressed(sf::Keyboard::W)) {
        f(0, -1);
        if (animate) {
            po.setAnimation("walk_up");
            state.last = 0;
        }
    }
    if (e.isKeyPressed(sf::Keyboard::A)) {
        f(-1, 0);
        if (animate) {
            po.setAnimation("walk_left");
            state.last = 1;
        }
    }
    if (e.isKeyPressed(sf::Keyboard::S)) {
        f(0, 1);
        if (animate) {
            po.setAnimation("walk_down");
            state.last = 2;
        }
    }
    if (e.isKeyPressed(sf::Keyboard::D)) {
        f(1, 0);
        if (animate) {
            po.setAnimation("walk_right");
            state.last = 3;
        }
    }
    if (!e.isKeyPressed(sf::Keyboard::W) && !e.isKeyPressed(sf::Keyboard::A) && !e.isKeyPressed(sf::Keyboard::S) && !e.isKeyPressed(sf::Keyboard::D)) {
        switch (state.last) {
            case 0:
                po.setAnimation("idle_up");
                break;
            case 1:
                po.setAnimation("idle_left");
                break;
            case 2:
                po.setAnimation("idle_down");
                break;
            case 3:
                po.setAnimation("idle_right");
                break;
        }
    }
}

template<typename T>
void LockInMap(T& po, Map& m) {
    // keep in borders of map

    Vector2f mapSize = Vector2f((float)m.getWidth(), (float)m.getHeight()) * TILE_SIZEf;
    if (po.getPosition().x + po.getSize().x > mapSize.x-1) {
        po.setPosition(mapSize.x-1 - po.getSize().x, po.getPosition().y);
        po.setVelocity(0, po.getVelocity().y);
    } else if (po.getPosition().x < -TILE_SIZEf/2) {
        po.setPosition(-TILE_SIZEf/2, po.getPosition().y);
        po.setVelocity(0, po.getVelocity().y);
    }
if (po.getPosition().y + po.getSize().y > mapSize.y-1) {
        po.setPosition(po.getPosition().x, mapSize.y-1 - po.getSize().y);
        po.setVelocity(po.getVelocity().x, 0);
    } else if (po.getPosition().y < -TILE_SIZEf/2) {
        po.setPosition(po.getPosition().x, -TILE_SIZEf/2);
        po.setVelocity(po.getVelocity().x, 0);
    }

}

