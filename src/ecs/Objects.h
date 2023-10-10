#pragma once

#include "Components.h"

std::vector<sf::IntRect> construct(Engine& e, std::vector<std::string> st) {
    // example: construct(e, {"char/sprite_00.png", "char/sprite_01.png"});
    std::vector<sf::IntRect> v;
    v.reserve(st.size());
    for (auto& s : st) {
        v.push_back(e.getRect(std::string("assets/img/") + s));
    }
    return v;

}

class BasicObject : public Object {
public:
    BasicObject(EntityComponentSystem& ecs, Engine& e, const std::string& path) : Object(e, path) { }
    BasicObject(EntityComponentSystem& ecs, Engine& e, Sprite s) : Object(e, std::move(s)) { }

    void update() override { }
};

class AnimatedObject : public Object {
    AnimationComponent m_anim;

public:
    AnimatedObject(EntityComponentSystem& ecs, Engine& e, std::unordered_map<std::string, Animation> anim) : Object(e), m_anim(anim, e, *this) { }

    void addAnimation(const std::string& name, const Animation& frames) {
        m_anim.addAnimation(name, frames);
    }

    void addAnimation(const std::string& name, const std::vector<std::string>& frames, float speed=1) {
        m_anim.addAnimation(name, Animation(construct(m_engine, frames), speed));
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
    PhysicsObject(EntityComponentSystem &ecs, Engine &e, Sprite s) : Object(e, std::move(s)), m_physics(e, *this, ecs) { }

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

class StaticObject : public Object { // by itself, this class is the same as BasicObject, but it is used for static object collision detection
public:
    StaticObject(EntityComponentSystem& ecs, Engine& e, const std::string& path) : Object(e, path) { }
    StaticObject(EntityComponentSystem& ecs, Engine& e, Sprite s) : Object(e, std::move(s)) { }

    void update() override { }

    sf::FloatRect getAABB() {
        return sprite.getGlobalBounds();
    }
};

class AnimatedPhysicsObject : public Object {
    AnimationComponent m_anim;
    PhysicsComponent m_physics;

public:
    AnimatedPhysicsObject(EntityComponentSystem& ecs, Engine& e, std::unordered_map<std::string, Animation> anim) : Object(e), m_anim(std::move(anim), e, *this), m_physics(e, *this, ecs) { }
    AnimatedPhysicsObject(EntityComponentSystem& ecs, Engine& e, Sprite s) : Object(e, std::move(s)), m_physics(e, *this, ecs), m_anim(e, *this) { }
    AnimatedPhysicsObject(EntityComponentSystem& ecs, Engine& e, const std::string& path) : Object(e, path), m_physics(e, *this, ecs), m_anim(e, *this) { }
    AnimatedPhysicsObject(EntityComponentSystem& ecs, Engine& e, const std::string& path, std::unordered_map<std::string, Animation> anim) : Object(e, path), m_anim(std::move(anim), e, *this), m_physics(e, *this, ecs) { }



    void addAnimation(const std::string& name, const Animation& frames) {
        m_anim.addAnimation(name, frames);
    }

    void addAnimation(const std::string& name, const std::vector<std::string>& frames, float speed=1) {
        m_anim.addAnimation(name, Animation(construct(m_engine, frames), speed));
    }

    void setAnimation(const std::string& name) {
        m_anim.setAnimation(name);
    }

    void setSpeed(float speed) {
        m_anim.setSpeed(speed);
    }

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
        m_anim.update();
        m_physics.update();
    }

    AnimationComponent& getAnim() {
        return m_anim;
    }
};