#pragma once

#include "../gfx/all_sf.h"
#include <unordered_map>
#include <cmath>
#include "EntityComponentSystem.h"

class AnimationComponent : public Component {
protected:
    std::unordered_map<std::string, std::vector<sf::IntRect>> m_animations; // name, frames, ex: "idle", { {0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32} }
    std::string m_currentAnimation;
    int m_currentFrame;
    float m_frameTime;
    float m_frameTimer;
    float m_speed;
public:
    AnimationComponent(std::unordered_map<std::string, std::vector<sf::IntRect>> anim, Engine& e, Object& o): m_currentFrame(0), m_frameTime(0.1), m_frameTimer(0), m_speed(1), m_animations(std::move(anim)), Component(e, o) { }
    AnimationComponent(Engine& e, Object& o): m_currentFrame(0), m_frameTime(0.1), m_frameTimer(0), m_speed(1), Component(e, o) { }

    void addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames) {
        m_animations[name] = frames;
    }

    void setAnimation(const std::string& name) {
        m_currentAnimation = name;
        m_currentFrame = 0;
        m_frameTimer = 0;
        m_object.setTextureRect(m_animations[name][0]);
    }

    void setSpeed(float speed) {
        m_speed = speed;
    }

    void update() override {
        m_frameTimer += m_engine.dt*m_speed;
        if (m_frameTimer >= m_frameTime) {
            m_frameTimer = 0;
            m_currentFrame++;
            if (m_currentFrame >= m_animations[m_currentAnimation].size()) {
                m_currentFrame = 0;
            }
            m_object.setTextureRect(m_animations[m_currentAnimation][m_currentFrame]);
        }
    }
};

class PhysicsComponent : public Component {
protected:
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;
    float m_friction;
    EntityComponentSystem& m_ecs;  // will be needed for collision detection, but not implemented yet

public:
    PhysicsComponent(Engine& e, Object& o, EntityComponentSystem& ecs): m_velocity(0, 0), m_acceleration(0, 0), m_friction(0.9), m_ecs(ecs), Component(e, o) { }

    void setVelocity(float x, float y) {
        m_velocity = sf::Vector2f(x, y);
    }

    void setVelocity(sf::Vector2f v) {
        m_velocity = v;
    }

    void addVelocity(float x, float y) {
        m_velocity += sf::Vector2f(x, y);
    }

    void addVelocity(sf::Vector2f v) {
        m_velocity += v;
    }

    void setAcceleration(float x, float y) {
        m_acceleration = sf::Vector2f(x, y);
    }

    void setAcceleration(sf::Vector2f v) {
        m_acceleration = v;
    }

    void addAcceleration(float x, float y) {
        m_acceleration += sf::Vector2f(x, y);
    }

    void addAcceleration(sf::Vector2f v) {
        m_acceleration += v;
    }

    void setFriction(float f) {
        m_friction = f;
    }

    void update() override {
        auto prev = m_object.getPosition();
        m_velocity += m_acceleration*m_engine.dt;
        m_velocity *= m_friction;
        m_object.moveTA(m_velocity);
        m_acceleration *= 1.0f-m_friction;
        auto direction = m_object.getPosition()-prev;

        auto aabbs = m_ecs.getStaticObjectAABBs();
        for (auto& aabb : aabbs) {
            if (collides(aabb, m_object)) {
                m_object.move(-direction);
                m_velocity = {0, 0};
                m_acceleration = {0, 0};
            }
        }


        // collision

    }

private:
    bool collides(const sf::FloatRect& a, const sf::FloatRect& b) {
        return a.intersects(b);
    }

    bool collides(const sf::FloatRect& a, const sf::Vector2f& b) {
        return a.contains(b);
    }

    bool collides(const sf::FloatRect& a, const Object& b) {
        return a.intersects(b.getAABB());
    }
};