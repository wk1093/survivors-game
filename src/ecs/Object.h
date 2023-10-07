#pragma once
#include "../gfx/all_sf.h"
#include <unordered_map>
#include <utility>

class Object {
protected:
    Engine& m_engine;
public:
    sf::Sprite sprite;

    Object(Engine& e, const std::string& path): m_engine(e), sprite(e.makeSprite(path)) { }
    Object(Engine& e): m_engine(e), sprite() { }

    Engine& engine() {
        return m_engine;
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setPosition(sf::Vector2f v) {
        sprite.setPosition(v);
    }

    void move(float x, float y) {
        sprite.move(x, y);
    }

    void move(sf::Vector2f v) {
        sprite.move(v);
    }
    // moveTA = move time adjusted
    void moveTA(float x, float y) {
        sprite.move(x*m_engine.dt, y*m_engine.dt);
    }

    void moveTA(sf::Vector2f v) {
        sprite.move(v*m_engine.dt);
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
        // instead of sprite = gfx.makeSprite(path);
        // because that would make a new sprite and lose the position and scale and rotation ...
    }

    [[nodiscard]] Vector2f getPosition() const {
        return sprite.getPosition();
    }

    [[nodiscard]] Vector2f getScale() const {
        return sprite.getScale();
    }

    [[nodiscard]] float getRotation() const {
        return sprite.getRotation();
    }

    [[nodiscard]] Vector2f getOrigin() const {
        return sprite.getOrigin();
    }

    [[nodiscard]] Vector2f getSize() const {
        return {sprite.getGlobalBounds().width, sprite.getGlobalBounds().height};
    }

    virtual void update() = 0;

    void draw() {
        m_engine.draw(sprite);
    }
};