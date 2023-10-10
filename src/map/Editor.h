#pragma once

#include <cmath>

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include "Map.h"

class MapEditor {
    EntityComponentSystem& ecs;
    Map& map;
    Engine& engine;
    std::vector<std::string> tileTypes;
    std::vector<Sprite> tileSprites;
public:
    MapEditor(Engine& engine, Map& map, EntityComponentSystem& ecs) : engine(engine), map(map), ecs(ecs) {}

    void launch() {
        View mapView = engine.getView();
        View oldView = mapView;
        Vector2f size = mapView.getSize();
        Vector2f center = mapView.getCenter();
        mapView.setSize(size.x*3, size.y*3);
        mapView.setCenter(center.x*3, center.y*3);

        tileTypes = recurseDirectory("assets/img/tile");

        View toolbarView = engine.getView();
        toolbarView.setSize(tileTypes.size()*TILE_SIZEf + 30, size.y/3);

        float toolbarScale = 1.0/4.0f;

        toolbarView.setCenter(TILE_SIZEf * (float)tileTypes.size() / 2.0f, TILE_SIZEf/2.0f);
        toolbarView.setViewport(FloatRect(0, 1.0f-toolbarScale, 1, toolbarScale));

        tileSprites.reserve(tileTypes.size());
        for (int i = 0; i < tileTypes.size(); i++) {
            tileSprites.emplace_back(engine.makeSprite(tileTypes[i]));
            tileSprites.back().setPosition((float)i*(TILE_SIZEf+5)-8, 0);
        }

        RectangleShape selector(Vector2f(TILE_SIZEf+10, TILE_SIZEf+10));
        selector.setFillColor(sf::Color::Green);
        selector.setPosition(-8-5, -5);

        RectangleShape toolbarBackground(Vector2f(10000, 10000));
        toolbarBackground.move(-5000, -5000);
        toolbarBackground.setFillColor(sf::Color(100, 100, 100, 100));

        int selection = 0;

        RectangleShape tileSelector(Vector2f(TILE_SIZEf, TILE_SIZEf));
        tileSelector.setFillColor(sf::Color(255, 255, 255, 100));

        sf::Vector2f dragStart;
        bool dragging = false;

        while (engine.isOpen()) {
            engine.update();
            if (engine.isKeyPressed(sf::Keyboard::Escape)) {
                break;
            }

            if (engine.isKeyDown(sf::Keyboard::Left)) {
                selection--;
                if (selection < 0) {
                    selection = tileTypes.size()-1;
                }
            }
            if (engine.isKeyDown(sf::Keyboard::Right)) {
                selection++;
                if (selection >= tileTypes.size()) {
                    selection = 0;
                }
            }

            selector.setPosition((float)selection*(TILE_SIZEf+5)-8-5, -5);

            auto mousePos = engine.getMousePosition(mapView);
            int selectedTileX = (int)std::floor(mousePos.x/TILE_SIZEf);
            int selectedTileY = (int)std::floor(mousePos.y/TILE_SIZEf);

            if (selectedTileX < 0) {
                selectedTileX = 0;
            } else if (selectedTileX >= map.getWidth()) {
                selectedTileX = map.getWidth()-1;
            }
            if (selectedTileY < 0) {
                selectedTileY = 0;
            } else if (selectedTileY >= map.getHeight()) {
                selectedTileY = map.getHeight()-1;
            }

            tileSelector.setPosition(Vector2f(selectedTileX*TILE_SIZEf, selectedTileY*TILE_SIZEf));

            if (engine.isMousePressed(sf::Mouse::Right)) {
                // drag
                if (!dragging) {
                    dragStart = mousePos;
                    dragging = true;
                } else {
                    Vector2f delta = dragStart - mousePos;
                    mapView.move(delta);
                    engine.setView(mapView);
                }
            } else {
                dragging = false;
            }

            if (engine.isKeyPressed(sf::Keyboard::S)) {
                // make hovered object static
                MapObject& mo = map.getMapObject(selectedTileX, selectedTileY);
                if (mo.type == BASIC) {
                    auto* bo = (BasicObject*)mo.obj;
                    Sprite s = bo->getSprite();
                    StaticObject* so = &ecs.makeObject<StaticObject>(s);
                    ecs.unmakeObject(*bo);
                    mo.obj = so;
                    mo.type = STATIC;
                }
            }
            if (engine.isKeyPressed(sf::Keyboard::B)) {
                MapObject& mo = map.getMapObject(selectedTileX, selectedTileY);
                if (mo.type == STATIC) {
                    auto* so = (StaticObject*)mo.obj;
                    Sprite s = so->getSprite();
                    BasicObject* bo = &ecs.makeObject<BasicObject>(s);
                    ecs.unmakeObject(*so);
                    mo.obj = bo;
                    mo.type = BASIC;
                }
            }

            if (engine.isMousePressed(sf::Mouse::Left)) {
                if (selectedTileX < 0 || selectedTileY < 0 || selectedTileX >= map.getWidth() || selectedTileY >= map.getHeight()) {
                    continue;
                }
                MapObject mo = map.getMapObject(selectedTileX, selectedTileY);
                if (mo.type == STATIC) {
                    ((StaticObject*)mo.obj)->setTextureRect(engine.getRect(tileTypes[selection]));
                } else if (mo.type == BASIC) {
                    ((BasicObject*)mo.obj)->setTextureRect(engine.getRect(tileTypes[selection]));
                } else {
                    printf("NOT IMPLEMENTED\n");
                }
            }



            engine.clear();

            engine.setView(mapView);
            ecs.draw(true);
            engine.draw(tileSelector);


            engine.setView(toolbarView);
            engine.draw(toolbarBackground);
            engine.draw(selector);
            for (int i = 0; i < tileSprites.size(); i++) {
                engine.draw(tileSprites[i]);

            }
            engine.render();
        }
        engine.setView(oldView);
    }
};