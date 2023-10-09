#pragma once

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
        tileTypes.push_back("assets/img/empty.png");

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



        while (engine.isOpen()) {
            engine.update();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                break;
            }

            engine.clear();

            engine.setView(mapView);
            ecs.draw();

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