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
        toolbarView.setSize(tileTypes.size()*(TILE_SIZEf+5)/2+16, TILE_SIZEf*2+10);

        float toolbarScale = 1.0/4.0f;

        toolbarView.setCenter((TILE_SIZEf+5) * (float)tileTypes.size() / 4.0f, TILE_SIZEf+5);
        toolbarView.setViewport(FloatRect(0, 1.0f-toolbarScale, 1, toolbarScale));

        tileSprites.reserve(tileTypes.size());
        for (int i = 0; i < tileTypes.size(); i++) {
            tileSprites.emplace_back(engine.makeSprite(tileTypes[i]));
            // 2 rows
            if (i < tileTypes.size()/2) {
                tileSprites.back().setPosition((float)i*(TILE_SIZEf+5)-8, TILE_SIZEf+5);
            } else {
                tileSprites.back().setPosition((float)(i-tileTypes.size()/2)*(TILE_SIZEf+5)-8, 0);
            }
          //  tileSprites.back().setPosition((float)i*(TILE_SIZEf+5)-8, 0);
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
            if (selection < tileTypes.size()/2) {
                selector.setPosition((float)selection*(TILE_SIZEf+5)-8-5, TILE_SIZEf);
            } else {
                selector.setPosition((float)(selection-tileTypes.size()/2)*(TILE_SIZEf+5)-8-5, -5);
            }
            //selector.setPosition((float)selection*(TILE_SIZEf+5)-8-5, -5);

            auto mousePos = engine.getMousePosition(mapView);
            int selectedTileX = (int)std::floor((mousePos.x + TILE_SIZEf/2)/TILE_SIZEf);
            int selectedTileY = (int)std::floor((mousePos.y + TILE_SIZEf/2)/TILE_SIZEf);

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

            tileSelector.setPosition(Vector2f((float)selectedTileX*TILE_SIZEf - TILE_SIZEf/2, (float)selectedTileY*TILE_SIZEf - TILE_SIZEf/2));

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

            if (engine.scrollDelta() != 0) {
                if (engine.isKeyPressed(Keyboard::LControl)) {
                    // scroll through tiles
                    selection -= engine.scrollDelta();
                    if (selection < 0) {
                        selection = tileTypes.size()-1;
                    }
                    if (selection >= tileTypes.size()) {
                        selection = 0;
                    }

                } else {
                    mapView.zoom(1.0f - engine.scrollDelta() / 10.0f);
                    mapView.move(
                            ((sf::Vector2f(mousePos - sf::Vector2f(mapView.getCenter()))).x) * engine.scrollDelta() *
                            0.1f,
                            ((sf::Vector2f(mousePos - sf::Vector2f(mapView.getCenter()))).y) * engine.scrollDelta() *
                            0.1f);
                }
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

            if (engine.isKeyDown(sf::Keyboard::R)) {
                auto* o = (Object*)map.getMapObject(selectedTileX, selectedTileY).obj;
                o->rotate(90);
            }

            if (engine.isMouseDown(sf::Mouse::Middle)) {
                MapObject& mo = map.getMapObject(selectedTileX, selectedTileY);
                if (mo.type == STATIC) {
                    auto* so = (StaticObject*)mo.obj;
                    Sprite s = so->getSprite();
                    BasicObject* bo = &ecs.makeObject<BasicObject>(s);
                    ecs.unmakeObject(*so);
                    mo.obj = bo;
                    mo.type = BASIC;
                } else if (mo.type == BASIC) {
                    auto* bo = (BasicObject*)mo.obj;
                    Sprite s = bo->getSprite();
                    StaticObject* so = &ecs.makeObject<StaticObject>(s);
                    ecs.unmakeObject(*bo);
                    mo.obj = so;
                    mo.type = STATIC;
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
        map.save(engine, "assets/map/test.map");
        engine.setView(oldView);
    }
};