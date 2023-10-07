#pragma once
#include "../engine/all_sf.h"
#include <unordered_map>
#include <utility>

class Object {
protected:
    Engine& m_engine;
public:
    sf::Sprite sprite;
    sf::Vector2f pos;

    Object(Engine& e, const std::string& path): m_engine(e), sprite(e.makeSprite(path)), pos(0, 0) { }

    Engine& engine() {
        return m_engine;
    }

    void setPosition(float x, float y) {
        pos = sf::Vector2f(x, y);
        sprite.setPosition(pos);
    }

    void move(float x, float y) {
        pos += sf::Vector2f(x, y);
        sprite.setPosition(pos);
    }

    void move(sf::Vector2f v) {
        pos += v;
        sprite.setPosition(pos);
    }
    // moveTA = move time adjusted
    void moveTA(float x, float y) {
        pos += sf::Vector2f(x*m_engine.dt, y*m_engine.dt);
        sprite.setPosition(pos);
    }

    void moveTA(sf::Vector2f v) {
        pos += v*m_engine.dt;
        sprite.setPosition(pos);
    }

    void scale(float x, float y) {
        sprite.scale(x, y);
    }

    void scale(sf::Vector2f v) {
        sprite.scale(v);
    }

    void rotate(float angle) {
        sprite.rotate(angle);
    }

    void rotateTA(float angle) {
        sprite.rotate(angle*m_engine.dt);
    }

    void setRotation(float angle) {
        sprite.setRotation(angle);
    }

    void setOrigin(float x, float y) {
        sprite.setOrigin(x, y);
    }

    void setOrigin(sf::Vector2f v) {
        sprite.setOrigin(v);
    }

    void setTextureRect(int x, int y, int w, int h) {
        sprite.setTextureRect(sf::IntRect(x, y, w, h));
    }

    void setTextureRect(sf::IntRect rect) {
        sprite.setTextureRect(rect);
    }

    void setSprite(const std::string& path) {
        sprite.setTexture(m_engine.getAtlas().getTexture());
        sprite.setTextureRect(m_engine.getAtlas().makeRect(path));
        // instead of sprite = engine.makeSprite(path);
        // because that would make a new sprite and lose the position and scale and rotation ...
    }

    void update() {

    }

    void draw() {
        m_engine.draw(sprite);
    }
};

class Component {
protected:
    Engine& m_engine;
    Object& m_object; // refrence to the object that this component is attached to

public:
    Component(Engine& e, Object& o): m_engine(e), m_object(o) { }

    virtual void update() = 0;
    virtual void draw() = 0;
};

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

    void draw() override {

    }
};