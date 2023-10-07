#pragma once

#include "Components.h"

class BasicObject : public Object {
public:
    BasicObject(EntityComponentSystem& ecs, Engine& e, const std::string& path) : Object(e, path) { }

    void update() override { }
};

class AnimatedObject : public Object {
    AnimationComponent m_anim;

public:
    AnimatedObject(EntityComponentSystem& ecs, Engine& e, std::unordered_map<std::string, std::vector<sf::IntRect>> anim) : Object(e), m_anim(anim, e, *this) { }

    void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames) {
        m_anim.addAnimation(name, frames);
    }

    void setAnimation(const std::string& name) {
        m_anim.setAnimation(name);
    }

    void setSpeed(float speed) {
        m_anim.setSpeed(speed);
    }

    void update() override {
        m_anim.update();
    }
};

class PhysicsObject : public Object {
    PhysicsComponent m_physics;

public:
    PhysicsObject(EntityComponentSystem &ecs, Engine &e, const std::string &path) : Object(e, path), m_physics(e, *this, ecs) { }

    void setVelocity(float x, float y) {
        m_physics.setVelocity(x, y);
    }

    void setVelocity(sf::Vector2f v) {
        m_physics.setVelocity(v);
    }

    void setAcceleration(float x, float y) {
        m_physics.setAcceleration(x, y);
    }

    void setAcceleration(sf::Vector2f v) {
        m_physics.setAcceleration(v);
    }

    void addAcceleration(float x, float y) {
        m_physics.addAcceleration(x, y);
    }

    void addAcceleration(sf::Vector2f v) {
        m_physics.addAcceleration(v);
    }

    void addVelocity(float x, float y) {
        m_physics.addVelocity(x, y);
    }

    void addVelocity(sf::Vector2f v) {
        m_physics.addVelocity(v);
    }



    void update() override {
        m_physics.update();
    }



};