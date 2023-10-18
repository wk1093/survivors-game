#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "TextureAtlas.h"
#include <chrono>

/**
 * An SFML Rendering gfx with texture atlas generation
 */
class Engine {
private:
    sf::RenderWindow window;
    TextureAtlas atlas;
    sf::Clock deltaClock;
    sf::Clock clock;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;

    // key states
    bool keyStates[256] = {false};
    bool keyDownStates[256] = {false};
    bool keyUpStates[256] = {false};

    // mouse states
    bool mouseStates[3] = {false};
    bool mouseDownStates[3] = {false};
    bool mouseUpStates[3] = {false};
    int mouseX = 0;
    int mouseY = 0;
    // scroll
    float scroll = 0;


public:
    sf::Time deltaTime;
    float dt = 0;
    sf::Font font; // default font

    Engine(unsigned int width, unsigned int height, const char* title, const char* imageDirectory, const char* sndDirectory) : atlas(imageDirectory) {
        std::chrono::steady_clock::time_point beginw = std::chrono::steady_clock::now();
        window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        std::chrono::steady_clock::time_point endw = std::chrono::steady_clock::now();
        std::cout << "Window created in " << std::chrono::duration_cast<std::chrono::milliseconds>(endw - beginw).count() << "ms" << std::endl;

        font.loadFromFile("assets/fonts/arial.ttf");
        std::chrono::steady_clock::time_point beginf = std::chrono::steady_clock::now();
        std::vector<std::string> files = recurseDirectory(sndDirectory, std::vector<std::string>{".wav", ".ogg"});
        for (const std::string& file : files) {
            sf::SoundBuffer buffer;
            buffer.loadFromFile(file);
            soundBuffers[file] = buffer;
        }
        std::chrono::steady_clock::time_point endf = std::chrono::steady_clock::now();
        std::cout << "Audio loaded in " << std::chrono::duration_cast<std::chrono::milliseconds>(endf - beginf).count() << "ms" << std::endl;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        atlas.build();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Texture atlas built in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

        clock.restart();
    }

    bool isOpen() {
        return window.isOpen();
    }

    void update() {
        deltaTime = deltaClock.restart();
        dt = deltaTime.asSeconds() * 60; // roughly 1 at 60fps
        sf::Event event{};
        for (int i = 0; i < 256; i++) {
            keyDownStates[i] = false;
            keyUpStates[i] = false;
        }
        for (int i = 0; i < 3; i++) {
            mouseDownStates[i] = false;
            mouseUpStates[i] = false;
        }
        scroll = 0;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed) {
                keyStates[event.key.code] = true;
                keyDownStates[event.key.code] = true;
            }
            if (event.type == sf::Event::KeyReleased) {
                keyStates[event.key.code] = false;
                keyUpStates[event.key.code] = true;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                mouseStates[event.mouseButton.button] = true;
                mouseDownStates[event.mouseButton.button] = true;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                mouseStates[event.mouseButton.button] = false;
                mouseUpStates[event.mouseButton.button] = true;
            }
            if (event.type == sf::Event::MouseMoved) {
                mouseX = event.mouseMove.x;
                mouseY = event.mouseMove.y;
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                scroll = event.mouseWheelScroll.delta;
            }
        }
    }

    void render() {
        window.display();
    }

    sf::Sprite makeSprite(const std::string& file) {
        return atlas.makeSprite(file);
    }

    TextureAtlas& getAtlas() {
        return atlas;
    }

    void draw(const sf::Drawable& drawable) {
        window.draw(drawable);
    }

    void clear() {
        window.clear();
    }

    void clear(const sf::Color& color) {
        window.clear(color);
    }

    bool isKeyPressed(sf::Keyboard::Key key) {
        return keyStates[key];
    }

    bool isKeyDown(sf::Keyboard::Key key) {
        // only true on the first frame the key is pressed
        return keyDownStates[key];
    }

    bool isKeyUp(sf::Keyboard::Key key) {
        // only true on the first frame the key is released
        return keyUpStates[key];
    }

    sf::RenderWindow &getWindow() {
        return window;
    }

    sf::View getView() {
        return window.getView();
    }

    void setView(sf::View v) {
        window.setView(v);
    }

    sf::Vector2f getSize() {
        return window.getView().getSize();
    }

    void reLaunch(unsigned int width, unsigned int height, const char* title) {
        sf::View v = window.getView();
        if (isOpen()) {
            window.close();
        }
        window.create(sf::VideoMode(width, height), title);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        window.setView(v);

    }


    sf::Vector2i getMousePosition() const {
        return {mouseX, mouseY};
    }

    sf::Vector2f getMousePosition(const sf::View& view) const {
        return window.mapPixelToCoords(getMousePosition(), view);
    }

    sf::Vector2f mapPixelToCoords(const sf::Vector2i& point, const sf::View& view) const {
        return window.mapPixelToCoords(point, view);
    }

    bool isMousePressed(sf::Mouse::Button button) {
        return mouseStates[button];
    }

    bool isMouseDown(sf::Mouse::Button button) {
        return mouseDownStates[button];
    }

    bool isMouseUp(sf::Mouse::Button button) {
        return mouseUpStates[button];
    }

    sf::IntRect getRect(const std::string& basicString) {
        return atlas.makeRect(basicString);
    }

    [[nodiscard]] float scrollDelta() const {
        return scroll;
    }

    double getTime() {
        return clock.getElapsedTime().asSeconds();
    }

    sf::SoundBuffer getSoundBuffer(const std::string& file) {
        return soundBuffers[file];
    }

    sf::Sound makeSound(const std::string& file) {
        sf::Sound sound;
        sound.setBuffer(getSoundBuffer(file));
        return sound;
    }



};