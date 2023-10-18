#pragma once

#include <utility>

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include "../map/all.h"

class Button {
public:
    std::string text="???";
    Vector2f pos={0,0};
    Vector2f size={100, 50};
    Color color=Color::Black;
    Color textColor=Color::White;
    Color clickedColor=Color::Red;
    Engine& e;
    bool clicked=false;
    bool enabled=true;

public:
    Button(Engine& e, std::string text, Vector2f pos, Vector2f size, Color color, Color textColor) : e(e), text(std::move(text)), pos(pos), size(size), color(color), textColor(textColor) {}
    Button(Engine& e, std::string text, Vector2f pos, Vector2f size, Color color) : e(e), text(std::move(text)), pos(pos), size(size), color(color) {}
    Button(Engine& e, std::string text, Vector2f pos, Vector2f size) : e(e), text(std::move(text)), pos(pos), size(size) {}

    void draw() {
        RectangleShape rect(size);
        rect.setPosition(pos);
        rect.setFillColor(clicked ? clickedColor : color);
        e.draw(rect);

        Text t(text, e.font, 20);
        t.setPosition(pos);
        if (!enabled) {
            t.setFillColor(Color(100, 100, 100));
        } else {
            t.setFillColor(textColor);
        }
        e.draw(t);
    }

    void update() {
        if (!enabled) {
            clicked = false;
            return;
        }
        if (e.isMouseUp(sf::Mouse::Left) || !e.isMousePressed(sf::Mouse::Left)) {
            clicked = false;
        }
        Vector2f mp = Vector2f(e.getMousePosition());
        if (mp.x >= pos.x && mp.x <= pos.x + size.x && mp.y >= pos.y && mp.y <= pos.y + size.y) {
            if (e.isMouseDown(sf::Mouse::Left)) {
                clicked = true;
            }

        }
    }
};

void MapGenMenu(Engine& e, EntityComponentSystem& ecs, Map** map) {
    // 2 numbers on the screen next to each other
    // width and height
    // scrolling while hovering over them changes them
    // a done button creates a new map with those dimensions and returns to the dev menu
    // a cancel button returns to the dev menu

    int width = 10;
    int height = 10;

    Button doneButton(e, "Done", {0, 100}, {100, 50}, Color(50, 50, 50), Color::White);
    Button cancelButton(e, "Cancel", {0, 150}, {100, 50}, Color(50, 50, 50), Color::White);

    while (e.isOpen()) {
        e.update();
        e.clear();

        doneButton.update();
        cancelButton.update();

        if (doneButton.clicked) {
            MapGenerator::generate("assets/map/test.map", width, height, "grass/sprite_00");
            *map = new Map("assets/map/test.map", ecs);
            return;
        } else if (cancelButton.clicked) {
            return;
        }

        if (e.scrollDelta() > 0) {
            if (e.getMousePosition().x < 640) {
                width++;
            } else {
                height++;
            }
        } else if (e.scrollDelta() < 0 && (width > 1 || height > 1)) {
            if (e.getMousePosition().x < 640) {
                width--;
            } else {
                height--;
            }
        }

        Text widthText(std::to_string(width), e.font, 20);
        widthText.setPosition(0, 0);
        e.draw(widthText);

        Text heightText(std::to_string(height), e.font, 20);
        heightText.setPosition(640, 0);
        e.draw(heightText);

        doneButton.draw();
        cancelButton.draw();

        e.render();
    }
}


bool DevMenu(Engine& e, EntityComponentSystem& ecs, Map** map) {

    Button newButton(e, "New Map", {0, 0}, {100, 50}, Color(50, 50, 50), Color::White);
    Button loadButton(e, "Load Map", {0, 50}, {100, 50}, Color(50, 50, 50), Color::White);
    Button editButton(e, "Edit Map", {0, 100}, {100, 50}, Color(50, 50, 50), Color::White);
    Button playButton(e, "Play Map", {0, 150}, {100, 50}, Color(50, 50, 50), Color::White);
    Button exitButton(e, "Exit", {0, 200}, {100, 50}, Color(50, 50, 50), Color::White);

    while (e.isOpen()) {
        e.update();
        e.clear();

        if (*map != nullptr) {
            newButton.enabled = false;
            loadButton.enabled = false;
            editButton.enabled = true;
            playButton.enabled = true;
        } else {
            editButton.enabled = false;
            playButton.enabled = false;
            newButton.enabled = true;
            loadButton.enabled = true;
        }

        newButton.update();
        loadButton.update();
        editButton.update();
        playButton.update();
        exitButton.update();

        if (newButton.clicked) {
            e.update();
            MapGenMenu(e, ecs, map);
            e.update();
            e.reLaunch(1280, 720, "Dev");
            e.update();
        } else if (loadButton.clicked) {
            *map = new Map("assets/map/test.map", ecs);
        } else if (editButton.clicked) {
            e.update();
            MapEditor(e, **map, ecs).launch();
            e.update();
            e.reLaunch(1280, 720, "Dev");
            e.update();

        } else if (playButton.clicked) {
            e.update();
            return false;
        } else if (exitButton.clicked) {
            return true;
        }


        newButton.draw();
        loadButton.draw();
        editButton.draw();
        playButton.draw();
        exitButton.draw();

        e.render();



    }
    return true;

}